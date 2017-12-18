/***************************************************************
 * Name:      ewxGrid.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-20
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef EWXGRID_H
#define EWXGRID_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/grid.h>

#include "ewxRange.h"

class ewxGrid : public wxGrid
{
    DECLARE_CLASS(ewxGrid)

    public:
        ewxGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxWANTS_CHARS, const wxString& name = wxGridNameStr);
        virtual ~ewxGrid();
        wxString GetSelection() const {return m_selection;}
        void CopyToClipboard();
        void PasteFromClipboard();
        bool GetSelectionCoordinates(ewxRange &range);
        int GetCellIntValue(int row, int col);
        void SetCellIntValue(int row, int col, int value);
        void SetColLabelIntValue(int col, int value);
        void SetRowLabelIntValue(int row, int value);
        void SetColLabelStringValue(int col, wxString value);
        void SetRowLabelStringValue(int row, wxString strvalue);

        void OnRangeSelect(wxGridRangeSelectEvent& event);

    protected:

    private:
        int SendEvent( const wxEventType type, int row, int col, wxMouseEvent& mouseEv );

        wxString m_selection;
        ewxRange m_selectionRange;

        DECLARE_EVENT_TABLE()
};

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_ADV, wxEVT_GRID_COPY_TO_CLIPBOARD, 1650)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_ADV, wxEVT_GRID_PASTE_FROM_CLIPBOARD, 1651)
END_DECLARE_EVENT_TYPES()

#define EVT_GRID_CMD_COPY_TO_CLIPBOARD(id, fn)      wx__DECLARE_GRIDRANGESELEVT(COPY_TO_CLIPBOARD, id, fn)
#define EVT_GRID_CMD_PASTE_FROM_CLIPBOARD(id, fn)   wx__DECLARE_GRIDRANGESELEVT(PASTE_FROM_CLIPBOARD, id, fn)

#define EVT_GRID_COPY_TO_CLIPBOARD(fn)        EVT_GRID_CMD_COPY_TO_CLIPBOARD(wxID_ANY, fn)
#define EVT_GRID_PASTE_FROM_CLIPBOARD(fn)     EVT_GRID_CMD_PASTE_FROM_CLIPBOARD(wxID_ANY, fn)

#endif // EWXGRID_H
