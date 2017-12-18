/***************************************************************
 * Name:      td5mapeditorMainFrame.cpp
 * Purpose:   Code for Application Frame
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

#include <wx/toolbar.h>

#include "td5mapeditorMainFrame.h"
#include "td5mapeditorChildFrame.h"
#include "td5mapeditorView.h"
#include "td5mapeditorApp.h"
#include "version.h"


#ifndef __WINDOWS__
#include "res/filenew.xpm"
#include "res/fileopen.xpm"
#include "res/filesave.xpm"
#include "res/editcopy.xpm"
#include "res/editcut.xpm"
#include "res/editpaste.xpm"
#include "res/fileprint.xpm"
#include "res/toolszoomminus.xpm"
#include "res/toolszoomplus.xpm"
#include "res/toolsdiff.xpm"
#include "res/toolsorig.xpm"
#include "res/toolscurr.xpm"
#include "res/toolsaddone.xpm"
#include "res/toolssubone.xpm"
#include "res/toolseditrange.xpm"
#else
#include "res/win32/filenew.xpm"
#include "res/win32/fileopen.xpm"
#include "res/win32/filesave.xpm"
#include "res/win32/editcopy.xpm"
#include "res/win32/editcut.xpm"
#include "res/win32/editpaste.xpm"
#include "res/win32/fileprint.xpm"
#include "res/win32/toolszoomminus.xpm"
#include "res/win32/toolszoomplus.xpm"
#include "res/win32/toolsdiff.xpm"
#include "res/win32/toolsorig.xpm"
#include "res/win32/toolscurr.xpm"
#include "res/win32/toolsaddone.xpm"
#include "res/win32/toolssubone.xpm"
#include "res/win32/toolseditrange.xpm"
#endif
td5mapeditorMainFrame *mainframe = (td5mapeditorMainFrame *) NULL;

td5mapeditorMainFrame *GetMainFrame(void)
{
  return mainframe;
}


IMPLEMENT_CLASS(td5mapeditorMainFrame, wxDocMDIParentFrame)
BEGIN_EVENT_TABLE(td5mapeditorMainFrame, wxDocMDIParentFrame)
#ifdef __WINDOWS__
    EVT_MENU(wxID_COPY,td5mapeditorMainFrame::OnCopy)
    EVT_MENU(wxID_PASTE,td5mapeditorMainFrame::OnPaste)
    EVT_MENU(ID_TOOLS_ZOOM_MINUS,td5mapeditorMainFrame::OnZoomMinus)
    EVT_MENU(ID_TOOLS_ZOOM_PLUS,td5mapeditorMainFrame::OnZoomPlus)
    EVT_MENU(ID_TOOLS_CURR_VIEW,td5mapeditorMainFrame::OnCurrentView)
    EVT_MENU(ID_TOOLS_ORIG_VIEW,td5mapeditorMainFrame::OnOriginalMapView)
    EVT_MENU(ID_TOOLS_DIFF_VIEW,td5mapeditorMainFrame::OnDifferencesView)
    EVT_MENU(ID_TOOLS_ADD_ONE,td5mapeditorMainFrame::OnAddOne)
    EVT_MENU(ID_TOOLS_SUBTRACT_ONE,td5mapeditorMainFrame::OnSubtractOne)
    EVT_MENU(ID_TOOLS_EDIT_RANGE_OF_VALUES,td5mapeditorMainFrame::OnEditRangeOfValues)
#endif
    EVT_MENU(TD5MAPEDITOR_ABOUT, td5mapeditorMainFrame::OnAbout)
    EVT_TIMER(wxID_ANY, td5mapeditorMainFrame::OnCyclicTimerEvent)
END_EVENT_TABLE()

td5mapeditorMainFrame::td5mapeditorMainFrame()
{
    m_cyclicTimer.SetOwner( this, wxID_ANY );
    m_cyclicTimer.Start( 200, wxTIMER_CONTINUOUS );
}

td5mapeditorMainFrame::td5mapeditorMainFrame(wxDocManager *manager, wxFrame *frame, const wxString& title,
    const wxPoint& pos, const wxSize& size, long type):
  wxDocMDIParentFrame(manager, frame, wxID_ANY, title, pos, size, type, _T("td5mapeditorMainFrame"))
{
    CreateLayout();
    m_cyclicTimer.SetOwner( this, wxID_ANY );
    m_cyclicTimer.Start( 200, wxTIMER_CONTINUOUS );
}

bool td5mapeditorMainFrame::Create(wxDocManager *manager, wxFrame *frame, const wxString& title,
    const wxPoint& pos, const wxSize& size, long type)
{
    if(!wxDocMDIParentFrame::Create(manager, frame, wxID_ANY, title, pos, size, type, _T("td5mapeditorMainFrame")))
        return false;

    CreateLayout();

    return true;
}

#ifdef __WXGTK__
/*
wxMDIClientWindow* td5mapeditorMainFrame::OnCreateClient()
{
    //return wxDocMDIParentFrame::OnCreateClient();

    m_clientWindow = (wxMDIClientWindow*) new wxMDIClientWindow( this );
    return m_clientWindow;

}
*/
#endif

void td5mapeditorMainFrame::CreateLayout()
{
    toolBar = CreateToolBar(wxNO_BORDER|wxTB_HORIZONTAL|wxTB_FLAT, ID_TOOLBAR, _T("Toolbar"));

    // Set up toolbar
    wxBitmap* toolBarBitmaps[15];

    toolBarBitmaps[0] = new wxBitmap( filenew_xpm );
    toolBarBitmaps[1] = new wxBitmap( fileopen_xpm );
    toolBarBitmaps[2] = new wxBitmap( filesave_xpm );
    toolBarBitmaps[3] = new wxBitmap( editcopy_xpm );
    toolBarBitmaps[4] = new wxBitmap( editcut_xpm );
    toolBarBitmaps[5] = new wxBitmap( editpaste_xpm );
    toolBarBitmaps[6] = new wxBitmap( fileprint_xpm );

    toolBarBitmaps[7] = new wxBitmap( toolszoomminus_xpm );
    toolBarBitmaps[8] = new wxBitmap( toolszoomplus_xpm );
    toolBarBitmaps[9] = new wxBitmap( toolsdiff_xpm );
    toolBarBitmaps[10] = new wxBitmap( toolsorig_xpm );
    toolBarBitmaps[11] = new wxBitmap( toolscurr_xpm );
    toolBarBitmaps[12] = new wxBitmap( toolsaddone_xpm );
    toolBarBitmaps[13] = new wxBitmap( toolssubone_xpm );
    toolBarBitmaps[14] = new wxBitmap( toolseditrange_xpm );

    int width = 16;
    int currentX = 5;

    toolBar->AddTool(wxID_NEW, *(toolBarBitmaps[0]), wxNullBitmap, false, currentX, wxDefaultCoord, (wxObject *) NULL, _T("New file"));
    currentX += width + 5;
    toolBar->AddTool(wxID_OPEN, *(toolBarBitmaps[1]), wxNullBitmap, false, currentX, wxDefaultCoord, (wxObject *) NULL, _T("Open file"));
    currentX += width + 5;
    toolBar->AddTool(wxID_SAVE, *(toolBarBitmaps[2]), wxNullBitmap, false, currentX, wxDefaultCoord, (wxObject *) NULL, _T("Save file"));
    currentX += width + 5;
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_COPY, *(toolBarBitmaps[3]), wxNullBitmap, false, currentX, wxDefaultCoord, (wxObject *) NULL, _T("Copy"));
    currentX += width + 5;
    toolBar->AddTool(wxID_CUT, *(toolBarBitmaps[4]), wxNullBitmap, false, currentX, wxDefaultCoord, (wxObject *) NULL, _T("Cut"));
    currentX += width + 5;
    toolBar->AddTool(wxID_PASTE, *(toolBarBitmaps[5]), wxNullBitmap, false, currentX, wxDefaultCoord, (wxObject *) NULL, _T("Paste"));
    currentX += width + 5;
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_PRINT, *(toolBarBitmaps[6]), wxNullBitmap, false, currentX, wxDefaultCoord, (wxObject *) NULL, _T("Print"));
    currentX += width + 5;

    toolBar->AddSeparator();
    toolBar->AddTool(ID_TOOLS_ZOOM_MINUS, *(toolBarBitmaps[7]), wxNullBitmap, false, currentX, wxDefaultCoord, (wxObject *) NULL, _T("Zoom Decrease"));
    currentX += width + 5;
    toolBar->AddTool(ID_TOOLS_ZOOM_PLUS, *(toolBarBitmaps[8]), wxNullBitmap, false, currentX, wxDefaultCoord, (wxObject *) NULL, _T("Zoom Increase"));
    currentX += width + 5;
    toolBar->AddSeparator();
    toolBar->AddRadioTool(ID_TOOLS_CURR_VIEW, _T("Current view"), *(toolBarBitmaps[11]), wxNullBitmap, _T("Switch To Current View"));
    currentX += width + 5;
    toolBar->AddRadioTool(ID_TOOLS_ORIG_VIEW, _T("Original view"), *(toolBarBitmaps[10]), wxNullBitmap, _T("Switch To Original Map View"));
    currentX += width + 5;
    toolBar->AddRadioTool(ID_TOOLS_DIFF_VIEW, _T("Differences view"), *(toolBarBitmaps[9]), wxNullBitmap, _T("Switch To Differences View"));
    currentX += width + 5;
    toolBar->AddSeparator();
    toolBar->AddTool(ID_TOOLS_ADD_ONE, *(toolBarBitmaps[12]), wxNullBitmap, false, currentX, wxDefaultCoord, (wxObject *) NULL, _T("Value(s) +1"));
    currentX += width + 5;
    toolBar->AddTool(ID_TOOLS_SUBTRACT_ONE, *(toolBarBitmaps[13]), wxNullBitmap, false, currentX, wxDefaultCoord, (wxObject *) NULL, _T("Value(s) -1"));
    currentX += width + 5;
    toolBar->AddTool(ID_TOOLS_EDIT_RANGE_OF_VALUES, *(toolBarBitmaps[14]), wxNullBitmap, false, currentX, wxDefaultCoord, (wxObject *) NULL, _T("Edit Range of Values"));

    DisableChildButtons();

    toolBar->Realize();

    // Can delete the bitmaps since they're reference counted
    int i;
    for (i = 0; i < /*15*/7; i++)
        delete toolBarBitmaps[i];

    statusBar = CreateStatusBar(7);
    int widths[7] = {-2, -1, 100, 65, 65, 170, 120};
    statusBar->SetStatusWidths(7, widths);
    int styles[7] = {wxSB_FLAT, wxSB_FLAT, wxSB_NORMAL, wxSB_NORMAL, wxSB_NORMAL, wxSB_NORMAL};
    statusBar->SetStatusStyles(7, styles);
}

void td5mapeditorMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
    wxString content(_T("Td5 Map Editor"));
    content += _T(" ");
    content += wxString::Format(wxT("%ld"), AutoVersion::MAJOR);
    content += _T(".");
    content += wxString::Format(wxT("%ld"), AutoVersion::MINOR);
    content += _T(".");
    content += wxString::Format(wxT("%ld"), AutoVersion::BUILD);
    content += _T(".");
    content += wxString::Format(wxT("%ld"), AutoVersion::REVISION);
#ifdef __WIN32__
    content += _T("\n");
    content += _T("Windows 32-bit release");
#endif
#ifdef __LINUX_X86_64__
    content += _T("\n");
    content += _T("Linux x86_64 version");
#endif
#ifdef __LINUX_I386__
    content += _T("\n");
    content += _T("Linux i386 version");
#endif
    content += _T("\n");
    content += _T("Author: Luca Veronesi");

    (void)wxMessageBox(content, _T("About Td5 Map Editor"));
}


void td5mapeditorMainFrame::EnableChildButtons()
{
    toolBar->EnableTool( wxID_COPY, true );
    toolBar->EnableTool( ID_TOOLS_ZOOM_MINUS, true );
    toolBar->EnableTool( ID_TOOLS_ZOOM_PLUS, true );
    toolBar->EnableTool( ID_TOOLS_CURR_VIEW, true );
    toolBar->EnableTool( ID_TOOLS_ORIG_VIEW, true );
    toolBar->EnableTool( ID_TOOLS_DIFF_VIEW, true );
    toolBar->EnableTool( ID_TOOLS_ADD_ONE, true );
    toolBar->EnableTool( ID_TOOLS_SUBTRACT_ONE, true );
    toolBar->EnableTool( ID_TOOLS_EDIT_RANGE_OF_VALUES, true );
}

void td5mapeditorMainFrame::DisableChildButtons()
{
    toolBar->EnableTool( wxID_CUT, false );
    toolBar->EnableTool( wxID_PASTE, false );
    toolBar->EnableTool( wxID_COPY, false );
    toolBar->EnableTool( ID_TOOLS_ZOOM_MINUS, false );
    toolBar->EnableTool( ID_TOOLS_ZOOM_PLUS, false );
    toolBar->EnableTool( ID_TOOLS_CURR_VIEW, false );
    toolBar->EnableTool( ID_TOOLS_ORIG_VIEW, false );
    toolBar->EnableTool( ID_TOOLS_DIFF_VIEW, false );
    toolBar->EnableTool( ID_TOOLS_ADD_ONE, false );
    toolBar->EnableTool( ID_TOOLS_SUBTRACT_ONE, false );
    toolBar->EnableTool( ID_TOOLS_EDIT_RANGE_OF_VALUES, false );
}

void td5mapeditorMainFrame::OnCopy(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorChildFrame *activeChild = (td5mapeditorChildFrame *) GetActiveChild();
    wxCommandEvent event;
    activeChild->OnCopy(event);
}

void td5mapeditorMainFrame::OnPaste(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorChildFrame *activeChild = (td5mapeditorChildFrame *) GetActiveChild();
    wxCommandEvent event;
    activeChild->OnPaste(event);
}

void td5mapeditorMainFrame::OnZoomMinus(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorChildFrame *activeChild = (td5mapeditorChildFrame *) GetActiveChild();
    wxCommandEvent event;
    activeChild->OnZoomMinus(event);
}

void td5mapeditorMainFrame::OnZoomPlus(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorChildFrame *activeChild = (td5mapeditorChildFrame *) GetActiveChild();
    wxCommandEvent event;
    activeChild->OnZoomPlus(event);
}

void td5mapeditorMainFrame::OnCurrentView(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorChildFrame *activeChild = (td5mapeditorChildFrame *) GetActiveChild();
    wxCommandEvent event;
    activeChild->OnCurrentView(event);
}

void td5mapeditorMainFrame::OnOriginalMapView(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorChildFrame *activeChild = (td5mapeditorChildFrame *) GetActiveChild();
    wxCommandEvent event;
    activeChild->OnOriginalMapView(event);
}

void td5mapeditorMainFrame::OnDifferencesView(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorChildFrame *activeChild = (td5mapeditorChildFrame *) GetActiveChild();
    wxCommandEvent event;
    activeChild->OnDifferencesView(event);
}

void td5mapeditorMainFrame::OnAddOne(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorChildFrame *activeChild = (td5mapeditorChildFrame *) GetActiveChild();
    wxCommandEvent event;
    activeChild->OnAddOne(event);
}
void td5mapeditorMainFrame::OnSubtractOne(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorChildFrame *activeChild = (td5mapeditorChildFrame *) GetActiveChild();
    wxCommandEvent event;
    activeChild->OnSubtractOne(event);
}
void td5mapeditorMainFrame::OnEditRangeOfValues(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorChildFrame *activeChild = (td5mapeditorChildFrame *) GetActiveChild();
    wxCommandEvent event;
    activeChild->OnEditRangeOfValues(event);
}

void td5mapeditorMainFrame::OnCyclicTimerEvent( wxTimerEvent& WXUNUSED(event) )
{
}

