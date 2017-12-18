/***************************************************************
 * Name:      td5mapeditorGridPanel.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-09
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef td5mapeditorGridPanel_H
#define td5mapeditorGridPanel_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

//#include <wx/grid.h>

#include "resource.h"

#include "td5mapeditorApp.h"
#include "td5mapeditorDoc.h"
#include "td5mapeditorView.h"
#include "ewxGrid.h"

class td5mapeditorGridPanel : public wxPanel
{
    DECLARE_CLASS(td5mapeditorGridPanel)

    public:
        td5mapeditorGridPanel(wxWindow* parent, wxView *view, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);
        virtual ~td5mapeditorGridPanel();

        void UpdateGrid(td5mapeditorDoc *doc);
        void OnSize(wxSizeEvent& event);
        void OnCopy(wxCommandEvent& event);
        void OnPaste(wxCommandEvent& event);
        void OnRClick(wxGridEvent& event);
        void OnCellChange(wxGridEvent& event);
        void OnKeyDown(wxKeyEvent& event);
        void OnGridCopy(wxGridRangeSelectEvent& event);
        void OnGridPaste(wxGridRangeSelectEvent& event);
        void OnRangeSelect(wxGridRangeSelectEvent& event);
        void OnCellSelect(wxGridEvent& event);
        void SetValueType(int type);
        void OnSumCellValue(int value);
        void OnAddOne(){OnSumCellValue(1);};
        void OnSubtractOne(){OnSumCellValue(-1);};
        void OnEditRangeOfValues(){};
        void SetGridCellTextColour(int row, int col, short diff);

        ewxGrid *m_grid;
        int m_gridCols;
        int m_gridRows;

    protected:
        wxToolBar *GetToolBar();
        wxMenuBar *GetMenuBar();

    private:
        td5mapeditorView *m_view;
        int m_valueType;
        wxMenu *context_menu;

    public:
        DECLARE_EVENT_TABLE()
};

#define GRID_VALUE_BASE                 100
#define GRID_VALUE_CURRENT              101
#define GRID_VALUE_DIFF                 102

#endif // td5mapeditorGridPanel_H
