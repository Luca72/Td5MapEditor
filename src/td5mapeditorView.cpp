/***************************************************************
 * Name:      td5mapeditorView.cpp
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-07
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#include "td5mapeditorMainFrame.h"
#include "td5mapeditorChildFrame.h"

#include "res/td5mapeditor.xpm"

IMPLEMENT_DYNAMIC_CLASS(td5mapeditorView, wxView)

BEGIN_EVENT_TABLE(td5mapeditorView, wxView)
END_EVENT_TABLE()

td5mapeditorView::td5mapeditorView()
{
    frame = (td5mapeditorChildFrame *) NULL;
}


td5mapeditorView::~td5mapeditorView()
{

}

td5mapeditorDoc *td5mapeditorView::GetDoc()
{
  return (td5mapeditorDoc *) GetDocument()  ;
}

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool td5mapeditorView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
    frame = (td5mapeditorChildFrame *) wxGetApp().CreateChildFrame(doc, this, true);
    //SetTitle(GetDoc()->GetMapName() /*_T("Td5 Map")*/);

    frame->SetIcon(td5mapeditor_xpm);
    frame->CreateCanvas(this);

#ifdef __X__
    // X seems to require a forced resize
    int x, y;
    frame->GetSize(&x, &y);
    frame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
#endif
    frame->Show(true);
    Activate(true);

    return true;
}

// Sneakily gets used for default print/preview
// as well as drawing on the screen.
void td5mapeditorView::OnDraw(wxDC *dc)
{

}

void td5mapeditorView::OnUpdate(wxView *WXUNUSED(sender), wxObject *WXUNUSED(hint))
{
    if (GetInfoPanel())
        GetInfoPanel()->UpdateList((td5mapeditorDoc *)GetDocument());

    if (GetGridPanel())
        GetGridPanel()->UpdateGrid((td5mapeditorDoc *)GetDocument());

    if (GetMainGraphCanvas())
    {
        GetMainGraphCanvas()->ResetSelectionRange();
        GetMainGraphCanvas()->Refresh();
    }

    if (GetDiffGraphCanvas())
    {
        GetDiffGraphCanvas()->ResetSelectionRange();
        GetDiffGraphCanvas()->Refresh();
    }

}

void td5mapeditorView::SetTitle(wxString title)
{
    frame->SetTitle(title);
}

void td5mapeditorView::SetInfoPanelFlags(bool hideEsoticMaps, bool hideUnmodifiedMaps)
{
    if (GetInfoPanel())
        GetInfoPanel()->SetFlags(hideEsoticMaps, hideUnmodifiedMaps);
}

// Clean up windows used for displaying the view.
bool td5mapeditorView::OnClose(bool deleteWindow)
{
    if (!GetDocument()->Close())
        return false;

    // Clear the canvas in  case we're in single-window mode,
    // and the canvas stays.
    GetMainGraphCanvas()->ClearBackground();
    GetMainGraphCanvas()->m_view = (wxView *) NULL;

    GetDiffGraphCanvas()->ClearBackground();
    GetDiffGraphCanvas()->m_view = (wxView *) NULL;

    wxString s(wxTheApp->GetAppName());
    if (frame)
        frame->SetTitle(s);

    SetFrame((wxFrame*)NULL);

    Activate(false);

    if (deleteWindow)
    {
        delete frame;
        return true;
    }

    return true;
}

td5mapeditorCanvas *td5mapeditorView::GetMainGraphCanvas()
{
    return frame->canvasMainGraph;
}

td5mapeditorCanvas *td5mapeditorView::GetDiffGraphCanvas()
{
    return frame->canvasDiffGraph;
}

td5mapeditorInfoPanel *td5mapeditorView::GetInfoPanel()
{
    return frame->panelInfo;
}

td5mapeditorGridPanel *td5mapeditorView::GetGridPanel()
{
    return frame->panelGrid;
}

