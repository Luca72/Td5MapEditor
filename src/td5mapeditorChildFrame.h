/***************************************************************
 * Name:      td5mapeditorChildFrame.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-09
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef TD5MAPEDITORCHILDFRAME_H
#define TD5MAPEDITORCHILDFRAME_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/splitter.h>

#include "resource.h"

#include "td5mapeditorApp.h"
#include "td5mapeditorView.h"
#include "td5mapeditorMainFrame.h"
#include "td5mapeditorInfoPanel.h"
#include "td5mapeditorGridPanel.h"

#include "ewxGrid.h"
#include "ewxDynArray.h"

#define SPLITTER_MAIN     100
#define SPLITTER_GRID     101
#define SPLITTER_GRAPH    102

class td5mapeditorInfoPanel;
class td5mapeditorGridPanel;

class td5mapeditorChildFrame : public wxDocMDIChildFrame
{
    DECLARE_CLASS(td5mapeditorChildFrame)

    public:
        td5mapeditorChildFrame(wxDocument* doc, wxView* view, wxMDIParentFrame* parent, wxWindowID id, const wxString& title,
                               const wxPoint& pos, const wxSize& size, long style);
    virtual ~td5mapeditorChildFrame();
    void CreateCanvas(wxView *view);
    void OnActivate(wxActivateEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);
    void OnAddOne(wxCommandEvent& WXUNUSED(event));
    void OnSubtractOne(wxCommandEvent& WXUNUSED(event));
    void OnEditRangeOfValues(wxCommandEvent& WXUNUSED(event));
    void OnZoomMinus(wxCommandEvent& event);
    void OnZoomPlus(wxCommandEvent& event);
    void OnCurrentView(wxCommandEvent& event);
    void OnOriginalMapView(wxCommandEvent& event);
    void OnDifferencesView(wxCommandEvent& event);
    void OnPrintSetup(wxCommandEvent& event);
    void OnPrintPreview(wxCommandEvent& event);
    void OnPrint(wxCommandEvent& event);
    void OnExportTuning(wxCommandEvent& event);
    void OnImportTuning(wxCommandEvent& event);
    void OnLoadXDF(wxCommandEvent& event);
    void OnHexCompare(wxCommandEvent& event);
    void OnEditTag(wxCommandEvent& WXUNUSED(event));

    td5mapeditorMainFrame *GetMainFrame() {return (td5mapeditorMainFrame*) m_parentFrame;}
    void SetShowType(int type);
    int GetShowType(){return m_showType;};
    void SetSelection(int col, int row, wxWindowID sender, wxWindowID destination = ID_ALL_PANES);
    void SetSelectionRange(ewxRange range, wxWindowID sender, wxWindowID destination = ID_ALL_PANES);

    wxSplitterWindow *splitterMain;
    wxSplitterWindow *splitterGrid;
    wxSplitterWindow *splitterGraph;
    td5mapeditorInfoPanel *panelInfo;
    td5mapeditorGridPanel *panelGrid;
    td5mapeditorCanvas *canvasDiffGraph;
    td5mapeditorCanvas *canvasMainGraph;

    void OnSplitterSashPosChanged(wxSplitterEvent& event);

    protected:
    private:
    wxRect DeterminePrintSize();
    td5mapeditorView *GetView();
    wxToolBar *GetToolBar(){return GetMainFrame()->toolBar;};
    wxFrame *m_parentFrame;
    int m_showType;

public:
    DECLARE_EVENT_TABLE()
};

#define SHOW_BASE                 10
#define SHOW_CURRENT              11
#define SHOW_DIFF                 12

// HEX compare modal dialog
class HexCompareDialog : public wxDialog
{
    DECLARE_CLASS(HexCompareDialog)
public:
    HexCompareDialog(wxWindow *parent);

    ewxGrid *m_grid;

	struct DIFF_STRUCT {
		wxWord base;
		wxWord current;
		wxUint32 address;
	};

	ewxDynArray<DIFF_STRUCT> m_diffList;

    wxDECLARE_EVENT_TABLE();
};


#endif // TD5MAPEDITORCHILDFRAME_H
