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
#include "ewxDynArray.h"

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
        float GetCellFloatValue(int row, int col);

        int GetColLabelIntValue(int col);
        float GetColLabelFloatValue(int col);

        int GetRowLabelIntValue(int row);
        float GetRowLabelFloatValue(int row);

        void SetCellIntValue(int row, int col, int value);
        void SetColLabelIntValue(int col, int value);
        void SetRowLabelIntValue(int row, int value);
        void SetCellFloatValue(int row, int col, float value);
        void SetColLabelFloatValue(int col, float value);
        void SetRowLabelFloatValue(int row, float value);
        void SetColLabelStringValue(int col, wxString value);
        void SetRowLabelStringValue(int row, wxString strvalue);

        void OnRangeSelect(wxGridRangeSelectEvent& event);

        void SetRowLabelTextColour(int row, wxColour labelColor);
        void SetColLabelTextColour(int col, wxColour labelColor);

        wxColour GetRowLabelTextColour(int row);
        wxColour GetColLabelTextColour(int col);

        void InitLabelsColour(int numRows, int numCols);

        // Overrides
        virtual void DrawRowLabels( wxDC& dc, const wxArrayInt& rows );
        virtual void DrawRowLabel( wxDC& dc, int row );

        virtual void DrawColLabels( wxDC& dc, const wxArrayInt& cols );
        virtual void DrawColLabel( wxDC& dc, int col );

    protected:

    private:
        int SendEvent( const wxEventType type, int row, int col, wxMouseEvent& mouseEv );

        wxString m_selection;
        ewxRange m_selectionRange;

        ewxDynArray<wxColour> m_rowLabelTextColors;
        ewxDynArray<wxColour> m_colLabelTextColors;

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


// Base class for the row/column header cells renderers
class WXDLLIMPEXP_ADV ewxGridHeaderLabelsRenderer
    : public wxGridCornerHeaderRenderer
{
public:
    // Draw header cell label
    virtual void DrawLabel(const wxGrid& grid,
                           wxDC& dc,
                           const wxString& value,
                           const wxRect& rect,
                           int horizAlign,
                           int vertAlign,
                           int textOrientation,
                           wxColour textColor) const;
};

class WXDLLIMPEXP_ADV ewxGridRowHeaderRenderer
    : public ewxGridHeaderLabelsRenderer
{
};

class WXDLLIMPEXP_ADV ewxGridColumnHeaderRenderer
    : public ewxGridHeaderLabelsRenderer
{
};

class WXDLLIMPEXP_ADV ewxGridRowHeaderRendererDefault
    : public ewxGridRowHeaderRenderer
{
public:
    virtual void DrawBorder(const wxGrid& grid,
                            wxDC& dc,
                            wxRect& rect) const;
};

// Column header cells renderers
class WXDLLIMPEXP_ADV ewxGridColumnHeaderRendererDefault
    : public ewxGridColumnHeaderRenderer
{
public:
    virtual void DrawBorder(const wxGrid& grid,
                            wxDC& dc,
                            wxRect& rect) const;
};



#endif // EWXGRID_H
