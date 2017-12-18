/***************************************************************
 * Name:      td5mapeditorChildFrame.cpp
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-09
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/cmdproc.h>
#include <wx/printdlg.h>

#include "td5mapeditorChildFrame.h"
#include "dlgEditRangeOfValues.h"
#include "dlgEditTag.h"
#include "td5mapTuner.h"
#include "td5mapeditorPrint.h"


IMPLEMENT_CLASS(td5mapeditorChildFrame, wxDocMDIChildFrame)
BEGIN_EVENT_TABLE(td5mapeditorChildFrame, wxDocMDIChildFrame)
    EVT_ACTIVATE(td5mapeditorChildFrame::OnActivate)
    /*EVT_CLOSE(td5mapeditorChildFrame::OnCloseWindow)*/
    EVT_MENU(wxID_COPY,td5mapeditorChildFrame::OnCopy)
    EVT_MENU(wxID_PASTE,td5mapeditorChildFrame::OnPaste)
    EVT_MENU(ID_TOOLS_ZOOM_MINUS,td5mapeditorChildFrame::OnZoomMinus)
    EVT_MENU(ID_TOOLS_ZOOM_PLUS,td5mapeditorChildFrame::OnZoomPlus)
    EVT_MENU(ID_TOOLS_CURR_VIEW,td5mapeditorChildFrame::OnCurrentView)
    EVT_MENU(ID_TOOLS_ORIG_VIEW,td5mapeditorChildFrame::OnOriginalMapView)
    EVT_MENU(ID_TOOLS_DIFF_VIEW,td5mapeditorChildFrame::OnDifferencesView)
    EVT_MENU(ID_TOOLS_ADD_ONE,td5mapeditorChildFrame::OnAddOne)
    EVT_MENU(ID_TOOLS_SUBTRACT_ONE,td5mapeditorChildFrame::OnSubtractOne)
    EVT_MENU(ID_TOOLS_EDIT_RANGE_OF_VALUES,td5mapeditorChildFrame::OnEditRangeOfValues)
    EVT_MENU(ID_FILE_PRINT_SETUP, td5mapeditorChildFrame::OnPrintSetup)
    EVT_MENU(ID_FILE_PREVIEW, td5mapeditorChildFrame::OnPrintPreview)
    EVT_MENU(ID_FILE_PRINT, td5mapeditorChildFrame::OnPrint)
    EVT_MENU(ID_FILE_EXPORT_TUNING, td5mapeditorChildFrame::OnExportTuning)
    EVT_MENU(ID_FILE_IMPORT_TUNING, td5mapeditorChildFrame::OnImportTuning)
    EVT_MENU(ID_FILE_LOAD_XDF, td5mapeditorChildFrame::OnLoadXDF)
    EVT_MENU(ID_TOOLS_EDIT_TAG,td5mapeditorChildFrame::OnEditTag)
    EVT_SIZE(td5mapeditorChildFrame::OnSize)
    EVT_SPLITTER_SASH_POS_CHANGED(SPLITTER_MAIN, td5mapeditorChildFrame::OnSplitterSashPosChanged)
    EVT_SPLITTER_SASH_POS_CHANGED(SPLITTER_GRID, td5mapeditorChildFrame::OnSplitterSashPosChanged)
    EVT_SPLITTER_SASH_POS_CHANGED(SPLITTER_GRAPH, td5mapeditorChildFrame::OnSplitterSashPosChanged)
END_EVENT_TABLE()

td5mapeditorChildFrame::td5mapeditorChildFrame(wxDocument* doc, wxView* view, wxMDIParentFrame* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxDocMDIChildFrame(doc, view, parent, id, title, pos, size, style, wxT("td5mapeditorChildFrame"))
{
    m_parentFrame = parent;
    m_showType = SHOW_CURRENT;

    canvasMainGraph = (td5mapeditorCanvas *) NULL;
    canvasDiffGraph = (td5mapeditorCanvas *) NULL;
    panelInfo = (td5mapeditorInfoPanel *) NULL;
    panelGrid = (td5mapeditorGridPanel *) NULL;

/*
#ifdef __WXMSW__
    SetIcon(wxString(isCanvas ? _T("chart") : _T("notepad")));
#endif
#ifdef __X__
    SetIcon(wxIcon(_T("doc.xbm")));
#endif
*/
    // Make a menubar
    wxMenu *file_menu = new wxMenu;

    file_menu->Append(wxID_NEW, _T("&New..."));
    file_menu->Append(wxID_OPEN, _T("&Open..."));
    file_menu->Append(wxID_CLOSE, _T("&Close"));
    file_menu->Append(wxID_SAVE, _T("&Save"));
    file_menu->Append(wxID_SAVEAS, _T("Save &As..."));

    file_menu->AppendSeparator();
    file_menu->Append(ID_FILE_EXPORT_TUNING, _T("&Export Tuning"));
    file_menu->Append(ID_FILE_IMPORT_TUNING, _T("&Import Tuning"));

    file_menu->AppendSeparator();
    file_menu->Append(ID_FILE_LOAD_XDF, _T("Load &XDF"));

    file_menu->AppendSeparator();
    file_menu->Append(ID_FILE_PRINT, _T("&Print..."));
    file_menu->Append(ID_FILE_PRINT_SETUP, _T("Print Se&tup..."));
    file_menu->Append(ID_FILE_PREVIEW, _T("Print Pre&view"));

    file_menu->AppendSeparator();
    file_menu->Append(wxID_EXIT, _T("E&xit"));


    wxMenu *edit_menu = (wxMenu *) NULL;

    edit_menu = new wxMenu;
    edit_menu->Append(wxID_UNDO, _T("&Undo"));
    edit_menu->Append(wxID_REDO, _T("&Redo"));
    edit_menu->AppendSeparator();
    edit_menu->Append(wxID_COPY, _T("&Copy\tCtrl-C"));
    edit_menu->Append(wxID_PASTE, _T("&Paste\tCtrl-V"));

    doc->GetCommandProcessor()->SetEditMenu(edit_menu);


    wxMenu *tools_menu = (wxMenu *) NULL;

    tools_menu = new wxMenu;
    tools_menu->Append(ID_TOOLS_ZOOM_MINUS, _T("Zoom &Out\tCtrl-O"));
    tools_menu->Append(ID_TOOLS_ZOOM_PLUS, _T("Zoom &In\tCtrl-I"));
    tools_menu->AppendSeparator();
    tools_menu->AppendRadioItem(ID_TOOLS_CURR_VIEW, _T("Cu&rrent View\tCtrl-R"));
    tools_menu->AppendRadioItem(ID_TOOLS_ORIG_VIEW, _T("Ori&ginal Map View\tCtrl-G"));
    tools_menu->AppendRadioItem(ID_TOOLS_DIFF_VIEW, _T("&Differences View\tCtrl-D"));
    tools_menu->AppendSeparator();
    tools_menu->Append(ID_TOOLS_ADD_ONE, _T("Value(s) +1\tCtrl-+"));
    tools_menu->Append(ID_TOOLS_SUBTRACT_ONE, _T("Value(s) -1\tCtrl-+"));
    tools_menu->Append(ID_TOOLS_EDIT_RANGE_OF_VALUES, _T("&Edit Range of Values\tCtrl-E"));
    tools_menu->AppendSeparator();
    tools_menu->Append(ID_TOOLS_EDIT_TAG, _T("&Edit Tag\tCtrl-T"));


    wxMenu *help_menu = new wxMenu;
    help_menu->Append(TD5MAPEDITOR_ABOUT, _T("&About"));

    wxMenuBar *menu_bar = new wxMenuBar;

    menu_bar->Append(file_menu, _T("&File"));
    menu_bar->Append(edit_menu, _T("&Edit"));
    menu_bar->Append(tools_menu, _T("&Tools"));
    menu_bar->Append(help_menu, _T("&Help"));

    //// Associate the menu bar with the frame
    SetMenuBar(menu_bar);

    GetMenuBar()->Enable(wxID_PASTE, false);
    GetMenuBar()->Enable(ID_TOOLS_EDIT_RANGE_OF_VALUES, false);

    GetMainFrame()->SetStatusBarText(wxString::Format(_T("Selection: %d,%d -> %d,%d"), 0, 0, 0, 0), 5);
    GetMainFrame()->SetStatusBarText(wxString::Format(_T("Col: %d"), 0), 3);
    GetMainFrame()->SetStatusBarText(wxString::Format(_T("Row: %d"), 0), 4);

/*
    GetToolBar()->EnableTool( wxID_COPY, true );
    GetToolBar()->EnableTool( ID_TOOLS_ZOOM_MINUS, true );
    GetToolBar()->EnableTool( ID_TOOLS_ZOOM_PLUS, true );
    GetToolBar()->EnableTool( ID_TOOLS_CURR_VIEW, true );
    GetToolBar()->EnableTool( ID_TOOLS_ORIG_VIEW, true );
    GetToolBar()->EnableTool( ID_TOOLS_DIFF_VIEW, true );
*/

    GetMainFrame()->EnableChildButtons();

#ifndef __WINDOWS__
    Maximize();
#endif
}

td5mapeditorChildFrame::~td5mapeditorChildFrame()
{
    //dtor
}


void td5mapeditorChildFrame::OnActivate(wxActivateEvent& event)
{
    td5mapeditorDoc* doc = (td5mapeditorDoc*)GetDocument();

    if(event.GetActive())
    {
        switch (m_showType)
        {
            case SHOW_CURRENT:
                GetToolBar()->ToggleTool( ID_TOOLS_CURR_VIEW, true );
                break;

            case SHOW_BASE:
                GetToolBar()->ToggleTool( ID_TOOLS_ORIG_VIEW, true );
                break;

            case SHOW_DIFF:
                GetToolBar()->ToggleTool( ID_TOOLS_DIFF_VIEW, true );
                break;
        }

        doc->Update(GetView());
    }
    //event.Skip();
}

void td5mapeditorChildFrame::OnCopy(wxCommandEvent& WXUNUSED(event))
{
    wxCommandEvent event;
    panelGrid->OnCopy(event);
}

void td5mapeditorChildFrame::OnPaste(wxCommandEvent& WXUNUSED(event))
{
    wxCommandEvent event;
    panelGrid->OnPaste(event);
}

void td5mapeditorChildFrame::OnAddOne(wxCommandEvent& WXUNUSED(event))
{
    panelGrid->OnAddOne();
}

void td5mapeditorChildFrame::OnSubtractOne(wxCommandEvent& WXUNUSED(event))
{
    panelGrid->OnSubtractOne();
}

td5mapeditorView *td5mapeditorChildFrame::GetView()
{
    return (td5mapeditorView *) wxDocMDIChildFrame::GetView();
}

void td5mapeditorChildFrame::OnSize(wxSizeEvent&
                                  #ifdef __WXUNIVERSAL__
                                  event
                                  #else
                                  WXUNUSED(event)
                                  #endif
                                  )
{
    int w, h;
    GetClientSize(&w, &h);
    splitterMain->SetSize(w, h);

    // FIXME: On wxX11, we need the MDI frame to process this
    // event, but on other platforms this should not
    // be done.
#ifdef __WXUNIVERSAL__
    event.Skip();
#endif
}

// Creates a canvas. Called from view.cpp when a new drawing
// view is created.
void td5mapeditorChildFrame::CreateCanvas(wxView *view)
{
    int width, height;
    GetClientSize(&width, &height);

    // Main splitter window
    splitterMain = new wxSplitterWindow(this, SPLITTER_MAIN, wxPoint(0, 0), wxSize(width, height), wxSP_3D /*| wxSP_3DSASH*/);
    splitterMain->SetSashGravity(0.0);

    panelInfo = new td5mapeditorInfoPanel(splitterMain, GetView(), ID_INFO_PANE);
    //panelInfo->SetBackgroundColour(*wxWHITE);
    panelInfo->Show(true);


    // Grid splitter window
    splitterGrid = new wxSplitterWindow(splitterMain, SPLITTER_GRID, wxPoint(0, 0), /*wxSize(width - 250, height)*/ wxSize(0,0), wxSP_3D /*| wxSP_3DSASH*/);
    splitterGrid->SetSashGravity(0.25);

    splitterMain->SplitVertically(panelInfo, splitterGrid, 250);
    splitterMain->SetMinimumPaneSize(220);

    panelGrid = new td5mapeditorGridPanel(splitterGrid, GetView(),ID_GRID_PANE);
    panelGrid->SetBackgroundColour(*wxWHITE);
    panelGrid->Show(true);

    // Graph splitter window
    splitterGraph = new wxSplitterWindow(splitterGrid, SPLITTER_GRAPH, wxPoint(0, 0), /*wxSize(width - 250, height - 250)*/wxSize(0,0), wxSP_3D /*| wxSP_3DSASH*/);
    splitterGraph->SetSashGravity(0.5);

    splitterGrid->SplitHorizontally(panelGrid, splitterGraph, 200);
    splitterGrid->SetMinimumPaneSize(100);

    canvasMainGraph = new td5mapeditorCanvas(splitterGraph, view, CANVAS_MAIN, ID_CANVAS_MAIN_PANE );
    canvasMainGraph->SetZoomLevel(100);
    canvasMainGraph->SetBackgroundColour(*wxWHITE);
    canvasMainGraph->SetGraphShow(GDC_SHOW_CURRENT);
    canvasMainGraph->Show(true);

    canvasDiffGraph = new td5mapeditorCanvas(splitterGraph, view, CANVAS_DIFF, ID_CANVAS_MAIN_PANE );
    canvasDiffGraph->SetZoomLevel(100);
    canvasDiffGraph->SetBackgroundColour(*wxWHITE);
    canvasDiffGraph->SetGraphShow(GDC_SHOW_DIFF);
    canvasDiffGraph->Show(true);

    splitterGraph->SplitVertically(canvasDiffGraph, canvasMainGraph);
    splitterGraph->SetMinimumPaneSize(200);

}

void td5mapeditorChildFrame::OnSplitterSashPosChanged(wxSplitterEvent& event)
{
    splitterMain->Refresh();
    splitterGrid->Refresh();
    splitterGraph->Refresh();
}

void td5mapeditorChildFrame::OnZoomMinus(wxCommandEvent& WXUNUSED(event))
{
    canvasMainGraph->DecreaseZoomLevel();
    canvasDiffGraph->DecreaseZoomLevel();
}

void td5mapeditorChildFrame::OnZoomPlus(wxCommandEvent& WXUNUSED(event))
{
    canvasMainGraph->IncreaseZoomLevel();
    canvasDiffGraph->IncreaseZoomLevel();
}

void td5mapeditorChildFrame::SetShowType(int type)
{
    m_showType = type;

    switch (m_showType)
    {
        case SHOW_CURRENT:
            panelGrid->SetValueType(GRID_VALUE_CURRENT);
            canvasMainGraph->SetGraphShow(GDC_SHOW_CURRENT);
            canvasDiffGraph->SetGraphShow(GDC_SHOW_DIFF);
            GetToolBar()->ToggleTool( ID_TOOLS_CURR_VIEW, true );
            GetMenuBar()->Check( ID_TOOLS_CURR_VIEW, true );
            break;

        case SHOW_BASE:
            panelGrid->SetValueType(GRID_VALUE_BASE);
            canvasMainGraph->SetGraphShow(GDC_SHOW_BASE);
            canvasDiffGraph->SetGraphShow(GDC_SHOW_DIFF);
            GetToolBar()->ToggleTool( ID_TOOLS_ORIG_VIEW, true );
            GetMenuBar()->Check( ID_TOOLS_ORIG_VIEW, true );
            break;

        case SHOW_DIFF:
            panelGrid->SetValueType(GRID_VALUE_DIFF);
            canvasMainGraph->SetGraphShow(GDC_SHOW_BASE_CURRENT);
            canvasDiffGraph->SetGraphShow(GDC_SHOW_DIFF);
            GetToolBar()->ToggleTool( ID_TOOLS_DIFF_VIEW, true );
            GetMenuBar()->Check( ID_TOOLS_DIFF_VIEW, true );
            break;
    }
}

void td5mapeditorChildFrame::OnCurrentView(wxCommandEvent& WXUNUSED(event))
{
    SetShowType(SHOW_CURRENT);
}

void td5mapeditorChildFrame::OnOriginalMapView(wxCommandEvent& WXUNUSED(event))
{
    SetShowType(SHOW_BASE);
}

void td5mapeditorChildFrame::OnDifferencesView(wxCommandEvent& WXUNUSED(event))
{
    SetShowType(SHOW_DIFF);
}

void td5mapeditorChildFrame::SetSelection(int col, int row, wxWindowID sender, wxWindowID destination)
{
    td5mapeditorDoc* doc = (td5mapeditorDoc*)GetDocument();
    doc->SetSelectionRange(ewxRange(row, col, row, col));

    GetMainFrame()->SetStatusBarText(wxString::Format(_T("Col: %d"), col), 3);
    GetMainFrame()->SetStatusBarText(wxString::Format(_T("Row: %d"), row), 4);

    if((sender != ID_GRID_PANE) && ((destination == ID_GRID_PANE) || (destination ==ID_ALL_PANES)))
    {
        panelGrid->m_grid->SetGridCursor(row, col);
        panelGrid->m_grid->SelectBlock(row, col, row, col);
        panelGrid->m_grid->MakeCellVisible(row, col);
    }

    //if((sender != ID_CANVAS_MAIN_PANE) && ((destination == ID_CANVAS_MAIN_PANE) || (destination ==ID_ALL_PANES)))
        canvasMainGraph->SetSelection(col, row);

    //if((sender != ID_CANVAS_DIFF_PANE) && ((destination == ID_CANVAS_DIFF_PANE) || (destination ==ID_ALL_PANES)))
        canvasDiffGraph->SetSelection(col, row);
}

void td5mapeditorChildFrame::SetSelectionRange(ewxRange range, wxWindowID sender, wxWindowID destination)
{
    td5mapeditorDoc* doc = (td5mapeditorDoc*)GetDocument();
    doc->SetSelectionRange(range);

    GetMainFrame()->SetStatusBarText(wxString::Format(_T("Selection: %d,%d -> %d,%d"),
                                                      range.leftCol,
                                                      range.topRow,
                                                      range.rightCol,
                                                      range.bottomRow)
                                     , 5);

    if((sender != ID_GRID_PANE) && ((destination == ID_GRID_PANE) || (destination ==ID_ALL_PANES)))
    {
        panelGrid->m_grid->SetGridCursor(range.bottomRow, range.rightCol);
        panelGrid->m_grid->SelectBlock(range.topRow, range.leftCol, range.bottomRow, range.rightCol);
        panelGrid->m_grid->MakeCellVisible(range.bottomRow, range.rightCol);
    }

    //if((sender != ID_CANVAS_MAIN_PANE) && ((destination == ID_CANVAS_MAIN_PANE) || (destination ==ID_ALL_PANES)))
        canvasMainGraph->SetSelectionRange(range.leftCol, range.topRow, range.rightCol, range.bottomRow);

    //if((sender != ID_CANVAS_DIFF_PANE) && ((destination == ID_CANVAS_DIFF_PANE) || (destination ==ID_ALL_PANES)))
        canvasDiffGraph->SetSelectionRange(range.leftCol, range.topRow, range.rightCol, range.bottomRow);
}

void td5mapeditorChildFrame::OnEditRangeOfValues(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorDoc* doc = (td5mapeditorDoc*)GetDocument();
    td5mapTable * table = doc->GetSelectedMapTable();
    td5mapTuner tuner(table);
    tuneData data;
    ewxRange range = doc->GetSelectionRange();

    if((range.GetCols() > 1) && (range.GetRows() > 1))
    {
        dlgEditRangeOfValues dlg(this, RANGE_TABLE);
        if ( dlg.ShowModal() == wxID_OK )
        {
            long tempvalue;
            dlg.m_ltValue->GetValue().ToLong(&tempvalue); data.ltValue = tempvalue;
            dlg.m_rtValue->GetValue().ToLong(&tempvalue); data.rtValue = tempvalue;
            dlg.m_lbValue->GetValue().ToLong(&tempvalue); data.lbValue = tempvalue;
            dlg.m_rbValue->GetValue().ToLong(&tempvalue); data.rbValue = tempvalue;
            dlg.m_constValue->GetValue().ToLong(&tempvalue); data.constValue = tempvalue;

            tuner.DoPlaneTuning(range, data);

            doc->Modify(true);
            doc->SetUpdateFlag(GRID_PANEL | GRAPH_PANEL);
            doc->Update();
        }
    }
    else if((range.GetCols() > 1) && (range.GetRows() > 0))
    {
        dlgEditRangeOfValues dlg(this, RANGE_HORIZ_LINE);
        if ( dlg.ShowModal() == wxID_OK )
        {
            long tempvalue;
            dlg.m_ltValue->GetValue().ToLong(&tempvalue); data.ltValue = data.lbValue = tempvalue;
            dlg.m_rtValue->GetValue().ToLong(&tempvalue); data.rtValue = data.lbValue = tempvalue;
            dlg.m_constValue->GetValue().ToLong(&tempvalue); data.constValue = tempvalue;

            tuner.DoPlaneTuning(range, data);

            doc->Modify(true);
            doc->SetUpdateFlag(GRID_PANEL | GRAPH_PANEL);
            doc->Update();
        }
    }
    else if((range.GetCols() > 0) && (range.GetRows() > 1))
    {
        dlgEditRangeOfValues dlg(this, RANGE_VERT_LINE);
        if ( dlg.ShowModal() == wxID_OK )
        {
            long tempvalue;
            dlg.m_ltValue->GetValue().ToLong(&tempvalue); data.ltValue = data.rtValue = tempvalue;
            dlg.m_lbValue->GetValue().ToLong(&tempvalue); data.lbValue = data.rbValue = tempvalue;
            dlg.m_constValue->GetValue().ToLong(&tempvalue); data.constValue = tempvalue;

            tuner.DoPlaneTuning(range, data);

            doc->Modify(true);
            doc->SetUpdateFlag(GRID_PANEL | GRAPH_PANEL);
            doc->Update();
        }
    }

}
wxRect td5mapeditorChildFrame::DeterminePrintSize()
{
    wxSize scr = wxGetDisplaySize();

    // determine position and size (shifting 16 left and down)
    wxRect rect(0,0,600,800);
    rect.x += 16;
    rect.y += 16;
    rect.width = wxMin (rect.width, (scr.x - rect.x));
    rect.height = wxMin (rect.height, (scr.x - rect.y));

    return rect;
}


void td5mapeditorChildFrame::OnPrintSetup (wxCommandEvent &WXUNUSED(event))
{
#if wxUSE_PRINTING_ARCHITECTURE
    (*g_pageSetupData) = * g_printData;
    wxPageSetupDialog pageSetupDialog(this, g_pageSetupData);
    pageSetupDialog.ShowModal();
    (*g_printData) = pageSetupDialog.GetPageSetupData().GetPrintData();
    (*g_pageSetupData) = pageSetupDialog.GetPageSetupData();
#endif // wxUSE_PRINTING_ARCHITECTURE
}

void td5mapeditorChildFrame::OnPrintPreview (wxCommandEvent &WXUNUSED(event))
{
    td5mapeditorDoc* doc = (td5mapeditorDoc*)GetDocument();

#if wxUSE_PRINTING_ARCHITECTURE
    wxPrintDialogData printDialogData( *g_printData);
    wxPrintPreview *preview =
        new wxPrintPreview (new td5mapeditorPrint (doc),
                            new td5mapeditorPrint (doc),
                            &printDialogData);
    if (!preview->Ok()) {
        delete preview;
        wxMessageBox (_("There was a problem with previewing.\n\
                         Perhaps your current printer is not correctly?"),
                      _("Previewing"), wxOK);
        return;
    }
    wxRect rect = DeterminePrintSize();
    wxPreviewFrame *frame = new wxPreviewFrame (preview, this, _("Print Preview"));
    frame->SetSize (rect);
    frame->Centre(wxBOTH);
    frame->Initialize();
    frame->Show(true);
#endif // wxUSE_PRINTING_ARCHITECTURE
}

void td5mapeditorChildFrame::OnPrint (wxCommandEvent &WXUNUSED(event))
{
    td5mapeditorDoc* doc = (td5mapeditorDoc*)GetDocument();

#if wxUSE_PRINTING_ARCHITECTURE
    wxPrintDialogData printDialogData( *g_printData);
    wxPrinter printer (&printDialogData);
    td5mapeditorPrint printout (doc);
    if (!printer.Print (this, &printout, true)) {
        if (wxPrinter::GetLastError() == wxPRINTER_ERROR) {
        wxMessageBox (_("There was a problem with printing.\n\
                         Perhaps your current printer is not correctly?"),
                      _("Previewing"), wxOK);
            return;
        }
    }
    (*g_printData) = printer.GetPrintDialogData().GetPrintData();
#endif // wxUSE_PRINTING_ARCHITECTURE
}

void td5mapeditorChildFrame::OnExportTuning (wxCommandEvent &WXUNUSED(event))
{
    td5mapeditorDoc* doc = (td5mapeditorDoc*)GetDocument();

    wxFileDialog filedlg(this,
        _("Save the tuning file as..."),
        wxEmptyString,
        wxEmptyString,
        _("Tuning files (*.t5n)|*.t5n|All files (*.*)|*.*"),
        wxFD_SAVE | wxFD_CHANGE_DIR );

    if (filedlg.ShowModal() == wxID_OK)
    {
        doc->ExportTuning(filedlg.GetPath());

    }
}

void td5mapeditorChildFrame::OnImportTuning (wxCommandEvent &WXUNUSED(event))
{
    td5mapeditorDoc* doc = (td5mapeditorDoc*)GetDocument();

    wxFileDialog filedlg(this,
        _("Choose a tuning file to open"),
        wxEmptyString,
        wxEmptyString,
        _("Tuning files (*.t5n)|*.t5n|All files (*.*)|*.*"),
        wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR );

    if (filedlg.ShowModal() == wxID_OK)
    {
        doc->ImportTuning(filedlg.GetPath());

    }
}

void td5mapeditorChildFrame::OnLoadXDF(wxCommandEvent& event)
{
    td5mapeditorDoc* doc = (td5mapeditorDoc*)GetDocument();
    wxFileDialog filedlg(this,
        _("Choose a TunerPro XDF file to open"),
        wxEmptyString,
        wxEmptyString,
        _("TunerPro files (*.xdf)|*.xdf|All files (*.*)|*.*"),
        wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR );

    if (filedlg.ShowModal() == wxID_OK)
    {
        doc->LoadXDF(filedlg.GetPath());

    }
}

void td5mapeditorChildFrame::OnEditTag (wxCommandEvent &WXUNUSED(event))
{
    td5mapeditorDoc* doc = (td5mapeditorDoc*)GetDocument();

    dlgEditTag dlg(this);
    dlg.SetTag(doc->GetMapTag());

    if ( dlg.ShowModal() == wxID_OK )
    {
        doc->SetMapTag(dlg.GetTag());
        doc->Modify(true);
        doc->SetUpdateFlag(INFO_PANEL);
        doc->Update();
    }

}
