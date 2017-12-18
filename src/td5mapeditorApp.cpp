/***************************************************************
 * Name:      td5mapeditorApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-07
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "td5mapeditorApp.h"
#include "td5mapeditorDoc.h"
#include "td5mapeditorMainFrame.h"
#include "td5mapeditorChildFrame.h"
#include "td5mapeditorView.h"
#include "version.h"

#include "res/td5mapeditor.xpm"

IMPLEMENT_APP(td5mapeditorApp);

#if wxUSE_PRINTING_ARCHITECTURE
//! global print data, to remember settings during the session
wxPrintData *g_printData = (wxPrintData*) NULL;
wxPageSetupData *g_pageSetupData = (wxPageSetupData*) NULL;
#endif // wxUSE_PRINTING_ARCHITECTURE


td5mapeditorApp::td5mapeditorApp(void)
{
    m_docManager = (wxDocManager *) NULL;
}

bool td5mapeditorApp::OnInit()
{
    //// Create a document manager
    m_docManager = new wxDocManager;

    //// Create a template relating drawing documents to their views
    (void) new wxDocTemplate((wxDocManager *) m_docManager, _T("Td5 Map Editor"), _T("*.map"), _T(""), _T("map"), _T("Drawing Doc"), _T("Td5 Editor View"),
          CLASSINFO(td5mapeditorDoc), CLASSINFO(td5mapeditorView));

    wxString title(_T("Td5 Map Editor"));
    title += _T(" ");
    title += wxString::Format(wxT("%ld"), AutoVersion::MAJOR);
    title += _T(".");
    title += wxString::Format(wxT("%ld"), AutoVersion::MINOR);
    title += _T(".");
    title += wxString::Format(wxT("%ld"), AutoVersion::BUILD);
    title += _T(".");
    title += wxString::Format(wxT("%ld"), AutoVersion::REVISION);
    title += _T("b-");
    title += _T("unstable");

    //// Create the main frame window
    /*
    mainframe = new td5mapeditorMainFrame((wxDocManager *) m_docManager, (wxFrame *) NULL,
                      title , wxPoint(0, 0), wxSize(1024, 768),
                      wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE);
    */
    mainframe = new td5mapeditorMainFrame;
    mainframe->Create((wxDocManager *) m_docManager, (wxFrame *) NULL,
                      title , wxPoint(0, 0), wxSize(1024, 768),
                      wxDEFAULT_FRAME_STYLE | wxNO_FULL_REPAINT_ON_RESIZE);


    //// Give it an icon (this is ignored in MDI mode: uses resources)
    mainframe->SetIcon(td5mapeditor_xpm);

    //// Make a menubar
    wxMenu *file_menu = new wxMenu;
    wxMenu *tools_menu = (wxMenu *) NULL;
    wxMenu *edit_menu = (wxMenu *) NULL;

    file_menu->Append(wxID_NEW, _T("&New...\tCtrl-N"));
    file_menu->Append(wxID_OPEN, _T("&Open...\tCtrl-X"));

    file_menu->AppendSeparator();
    file_menu->Append(wxID_EXIT, _T("E&xit\tAlt-X"));

    // A nice touch: a history of files visited. Use this menu.
    m_docManager->FileHistoryUseMenu(file_menu);

    wxMenu *help_menu = new wxMenu;
    help_menu->Append(TD5MAPEDITOR_ABOUT, _T("&About\tF1"));

    wxMenuBar *menu_bar = new wxMenuBar;

    menu_bar->Append(file_menu, _T("&File"));
    if (edit_menu)
    menu_bar->Append(edit_menu, _T("&Edit"));
    if (tools_menu)
    menu_bar->Append(tools_menu, _T("&Tools"));
    menu_bar->Append(help_menu, _T("&Help"));

#ifdef __WXMAC__
    wxMenuBar::MacSetCommonMenuBar(menu_bar);
#endif //def __WXMAC__
    //// Associate the menu bar with the main frame
    mainframe->SetMenuBar(menu_bar);

    mainframe->Centre(wxBOTH);
#ifndef __WXMAC__
    mainframe->Show(true);
#endif //ndef __WXMAC__

#if wxUSE_PRINTING_ARCHITECTURE
    // initialize print data and setup
    g_printData = new wxPrintData;
    g_pageSetupData = new wxPageSetupDialogData;
    // set up default (A4) paper size
    g_pageSetupData->SetPaperSize(wxSize(210,297));
    g_pageSetupData->SetMarginBottomRight(wxPoint(20,20));
    g_pageSetupData->SetMarginTopLeft(wxPoint(20,20));
#endif // wxUSE_PRINTING_ARCHITECTURE

    SetTopWindow(mainframe);
    return true;
}


int td5mapeditorApp::OnExit(void)
{
#if wxUSE_PRINTING_ARCHITECTURE
    // delete global print data and setup
    if (g_printData) delete g_printData;
    if (g_pageSetupData) delete g_pageSetupData;
#endif // wxUSE_PRINTING_ARCHITECTURE

    delete m_docManager;

    return 0;
}

/*
 * Centralised code for creating a document frame.
 * Called from view.cpp, when a view is created.
 */


wxMDIChildFrame *td5mapeditorApp::CreateChildFrame(wxDocument *doc, wxView *view, bool isCanvas)
{
    int width, height;
#ifndef __WINDOWS__
    GetMainFrame()->GetClientSize(&width, &height);
#else
    width = 800; height = 600;
#endif

    //// Make a child frame
    td5mapeditorChildFrame *subframe =
    new td5mapeditorChildFrame(doc, view, GetMainFrame(), wxID_ANY, _T(""),
                             wxPoint(0,0), wxSize(width, height),
                             wxDEFAULT_FRAME_STYLE |
                             wxNO_FULL_REPAINT_ON_RESIZE);

    return subframe;
}

