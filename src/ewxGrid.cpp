#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/grid.h>
#include <wx/clipbrd.h>
#include <wx/tokenzr.h>
#include <stdexcept>

#include "ewxGrid.h"

IMPLEMENT_CLASS(ewxGrid, wxGrid)

ewxGridRowHeaderRendererDefault rowRend;
ewxGridColumnHeaderRendererDefault colRend;


DEFINE_EVENT_TYPE(wxEVT_GRID_COPY_TO_CLIPBOARD)
DEFINE_EVENT_TYPE(wxEVT_GRID_PASTE_FROM_CLIPBOARD)

BEGIN_EVENT_TABLE(ewxGrid, wxGrid)
    EVT_GRID_RANGE_SELECT(ewxGrid::OnRangeSelect)
END_EVENT_TABLE()

ewxGrid::ewxGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
    wxGrid(parent,id,pos,size,style,name), m_selection(wxT(""))
{
}

void ewxGrid::InitLabelsColour(int numRows, int numCols)
{
    m_rowLabelTextColors.resize(numRows);
    m_colLabelTextColors.resize(numCols);

    if(numRows > 0)
        for(int r= 0; r < numRows; r++)
            SetRowLabelTextColour(r, *wxBLACK);

    if(numCols > 0)
        for(int c= 0; c < numCols; c++)
            SetColLabelTextColour(c, *wxBLACK);
}




ewxGrid::~ewxGrid()
{

}

void ewxGrid::CopyToClipboard()
{
    if (!IsSelection())
            return;

    // Write some text to the clipboard
    // These data objects are held by the clipboard,
    // so do not delete them in the app.

    int row = 0;
    int col = 0;
    int bottomrow = 0;
    int bottomcol = 0;

    m_selection.Clear();
    bool newline=true;

    for (int nRow=0;nRow<GetNumberRows();++nRow)
    {
        bool selected=false;
        newline=true;

        for (int nCol=0;nCol<GetNumberCols();++nCol)
        {
            if (IsInSelection(nRow,nCol))
            {
                // Add a line break if this is not the first line:
                if (newline && m_selection != wxT("") )
                {
                    m_selection << wxT("\n");
                }

                if (!newline)
                {
                    m_selection << wxT("\t");
                }

                newline=false;

                try
                {
                    m_selection << GetCellValue(nRow,nCol);
                    selected=true;
                }
                catch (const std::out_of_range& e)
                {
                    throw e;
                }
            }
        }
    }

    if (wxTheClipboard->Open())
    {
        wxTheClipboard->SetData(new wxTextDataObject(m_selection));
        wxTheClipboard->Close();
    }

    wxGridRangeSelectEvent gridEvt( GetId(),
    wxEVT_GRID_COPY_TO_CLIPBOARD,
    this,
    wxGridCellCoords( row, col ),
    wxGridCellCoords( bottomrow, bottomcol),
    true,
    false, false,
    false, false );
    GetEventHandler()->ProcessEvent( gridEvt );
}

void ewxGrid::PasteFromClipboard()
{
    // Get some text to the clipboard
    // These data objects are held by the clipboard,
    // so do not delete them in the app.
    m_selection.Clear();

    int row = GetGridCursorRow();
    int col = GetGridCursorCol();

    wxTextDataObject clipboard;
    if (wxTheClipboard->Open())
    {
        wxTheClipboard->GetData(clipboard);
        wxTheClipboard->Close();
    }

    m_selection = clipboard.GetText();
    wxStringTokenizer tokens;
    tokens.SetString(m_selection);

    wxString strdata;

    wxStringTokenizer rows;
    rows.SetString(m_selection, _T("\n"));

    wxStringTokenizer cols;
    cols.SetString(rows.GetNextToken(), _T("\t"));

    int selrows = rows.CountTokens() + 1;
    int selcols = cols.CountTokens();
    int bottomrow = 0;
    int bottomcol = 0;

    for (int nRow = 0; nRow < selrows; ++nRow)
    {
        for (int nCol = 0; nCol < selcols; ++nCol)
        {
            strdata = tokens.GetNextToken();

            if(((row + nRow) < GetNumberRows()) && ((col + nCol) < GetNumberCols()))
            {
                try
                {
                    SetCellValue(row + nRow, col + nCol, strdata);
                }
                catch (const std::out_of_range& e)
                {
                    throw e;
                }

                bottomrow = row + nRow;
                bottomcol = col + nCol;
            }
        }
    }

    int endselcols, endselrows;

    if((col + selcols) < GetNumberCols())
        endselcols = col + selcols - 1;
    else
        endselcols = GetNumberCols() - 1;

    if((row + selrows) < GetNumberRows())
        endselrows = row + selrows - 1;
    else
        endselrows = GetNumberRows() - 1;

    SelectBlock(row, col, endselrows, endselcols);

    wxGridRangeSelectEvent gridEvt( GetId(),
        wxEVT_GRID_PASTE_FROM_CLIPBOARD,
        this,
        wxGridCellCoords( row, col ),
        wxGridCellCoords( bottomrow, bottomcol),
        true,
        false, false,
        false, false );
    GetEventHandler()->ProcessEvent( gridEvt );
}

void ewxGrid::OnRangeSelect(wxGridRangeSelectEvent& event)
{
    m_selectionRange = ewxRange(event.GetTopRow(),
                                event.GetLeftCol(),
                                event.GetBottomRow(),
                                event.GetRightCol());

    event.Skip();
}

bool ewxGrid::GetSelectionCoordinates(ewxRange &range)
{
    if(!IsSelection())
        return false;

    range = m_selectionRange;

    return true;
}

int ewxGrid::GetCellIntValue(int row, int col)
{
    long value;
    GetCellValue(row, col).ToLong(&value);
    return (int) value;
}

float ewxGrid::GetCellFloatValue(int row, int col)
{
    double value;
    //GetCellValue(row, col).ToCDouble(&value);
    GetCellValue(row, col).ToDouble(&value);
    return (float) value;
}


void ewxGrid::SetCellIntValue(int row, int col, int value)
{
    wxString strvalue;
    strvalue.Printf(_T("%d"), value);
    SetCellValue( row, col, strvalue);
}

void ewxGrid::SetColLabelIntValue(int col, int value)
{
    wxString strvalue;
    strvalue.Printf(_T("%d"), value);
    SetColLabelValue(col, strvalue);
}

void ewxGrid::SetRowLabelIntValue(int row, int value)
{
    wxString strvalue;
    strvalue.Printf(_T("%d"), value);
    SetRowLabelValue(row, strvalue);
}

void ewxGrid::SetCellFloatValue(int row, int col, float value)
{
    wxString strvalue;
    strvalue.Printf(_T("%.2f"), value);
    SetCellValue( row, col, strvalue);
}

void ewxGrid::SetColLabelFloatValue(int col, float value)
{
    wxString strvalue;
    strvalue.Printf(_T("%.1f"), value);
    SetColLabelValue(col, strvalue);
}

void ewxGrid::SetRowLabelFloatValue(int row, float value)
{
    wxString strvalue;
    strvalue.Printf(_T("%.1f"), value);
    SetRowLabelValue(row, strvalue);
}

void ewxGrid::SetColLabelStringValue(int col, wxString strvalue)
{
    SetColLabelValue(col, strvalue);
}

void ewxGrid::SetRowLabelStringValue(int row, wxString strvalue)
{
    SetRowLabelValue(row, strvalue);
}

void ewxGrid::SetCellStringValue(int row, int col, wxString strvalue)
{
    SetCellValue( row, col, strvalue);
}

int ewxGrid::GetColLabelIntValue(int col)
{
    long value;
    GetColLabelValue(col).ToLong(&value);
    return (int) value;
}

float ewxGrid::GetColLabelFloatValue(int col)
{
    double value;
    GetColLabelValue(col).ToDouble(&value);
    return (float) value;
}

int ewxGrid::GetRowLabelIntValue(int row)
{
    long value;
    GetRowLabelValue(row).ToLong(&value);
    return (int) value;
}

float ewxGrid::GetRowLabelFloatValue(int row)
{
    double value;
    GetRowLabelValue(row).ToDouble(&value);
    return (float) value;
}


// Overrides
void ewxGrid::DrawRowLabels( wxDC& dc, const wxArrayInt& rows)
{
    if ( !m_numRows )
        return;

    const size_t numLabels = rows.GetCount();
    for ( size_t i = 0; i < numLabels; i++ )
    {
        DrawRowLabel( dc, rows[i] );
    }
}


void ewxGrid::DrawRowLabel( wxDC& dc, int row)
{
    if ( GetRowHeight(row) <= 0 || m_rowLabelWidth <= 0 )
        return;

    wxGridCellAttrProvider * const
        attrProvider = m_table ? m_table->GetAttrProvider() : NULL;

    // notice that an explicit static_cast is needed to avoid a compilation
    // error with VC7.1 which, for some reason, tries to instantiate (abstract)
    // wxGridRowHeaderRenderer class without it
    const ewxGridRowHeaderRenderer&
        rend = static_cast<const ewxGridRowHeaderRenderer&>(rowRend);

    wxRect rect(0, GetRowTop(row), m_rowLabelWidth, GetRowHeight(row));
    rend.DrawBorder(*this, dc, rect);

    int hAlign, vAlign;
    GetRowLabelAlignment(&hAlign, &vAlign);

    rend.DrawLabel(*this, dc, GetRowLabelValue(row), rect, hAlign, vAlign, wxHORIZONTAL, GetRowLabelTextColour(row));
}

void ewxGrid::SetRowLabelTextColour(int row, wxColour labelColor)
{
    m_rowLabelTextColors[row] = labelColor;
}

void ewxGrid::SetColLabelTextColour(int col, wxColour labelColor)
{
    m_colLabelTextColors[col] = labelColor;
}

wxColour ewxGrid::GetRowLabelTextColour(int row)
{
    return m_rowLabelTextColors[row];
}

wxColour ewxGrid::GetColLabelTextColour(int col)
{
   return m_colLabelTextColors[col];
};


void ewxGrid::DrawColLabels( wxDC& dc,const wxArrayInt& cols )
{
    if ( !m_numCols )
        return;

    const size_t numLabels = cols.GetCount();
    for ( size_t i = 0; i < numLabels; i++ )
    {
        DrawColLabel( dc, cols[i] );
    }
}

void ewxGrid::DrawColLabel(wxDC& dc, int col)
{
    if ( GetColWidth(col) <= 0 || m_colLabelHeight <= 0 )
        return;

    int colLeft = GetColLeft(col);

    wxRect rect(colLeft, 0, GetColWidth(col), m_colLabelHeight);
    wxGridCellAttrProvider * const
        attrProvider = m_table ? m_table->GetAttrProvider() : NULL;
    const ewxGridColumnHeaderRenderer&
        rend = static_cast<ewxGridColumnHeaderRenderer&>(colRend);

		// It is reported that we need to erase the background to avoid display
		// artefacts, see #12055.
		wxDCBrushChanger setBrush(dc, m_colWindow->GetBackgroundColour());
		dc.DrawRectangle(rect);

		rend.DrawBorder(*this, dc, rect);

    int hAlign, vAlign;
    GetColLabelAlignment(&hAlign, &vAlign);
    const int orient = GetColLabelTextOrientation();

    rend.DrawLabel(*this, dc, GetColLabelValue(col), rect, hAlign, vAlign, orient, GetColLabelTextColour(col));
}


// ewxGridHeaderLabelsRenderer and related classes

void ewxGridHeaderLabelsRenderer::DrawLabel(const wxGrid& grid,
                                           wxDC& dc,
                                           const wxString& value,
                                           const wxRect& rect,
                                           int horizAlign,
                                           int vertAlign,
                                           int textOrientation,
                                           wxColour textColor) const
{
    dc.SetBackgroundMode(wxBRUSHSTYLE_TRANSPARENT);
    dc.SetTextForeground(textColor);
    dc.SetFont(grid.GetLabelFont());
    grid.DrawTextRectangle(dc, value, rect, horizAlign, vertAlign, textOrientation);
}


void ewxGridRowHeaderRendererDefault::DrawBorder(const wxGrid& WXUNUSED(grid),
                                                wxDC& dc,
                                                wxRect& rect) const
{
    dc.SetPen(wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW)));
    dc.DrawLine(rect.GetRight(), rect.GetTop(),
                rect.GetRight(), rect.GetBottom());
    dc.DrawLine(rect.GetLeft(), rect.GetTop(),
                rect.GetLeft(), rect.GetBottom());
    dc.DrawLine(rect.GetLeft(), rect.GetBottom(),
                rect.GetRight() + 1, rect.GetBottom());

    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(rect.GetLeft() + 1, rect.GetTop(),
                rect.GetLeft() + 1, rect.GetBottom());
    dc.DrawLine(rect.GetLeft() + 1, rect.GetTop(),
                rect.GetRight(), rect.GetTop());

    rect.Deflate(2);
}

void ewxGridColumnHeaderRendererDefault::DrawBorder(const wxGrid& WXUNUSED(grid),
                                                   wxDC& dc,
                                                   wxRect& rect) const
{
    dc.SetPen(wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW)));
    dc.DrawLine(rect.GetRight(), rect.GetTop(),
                rect.GetRight(), rect.GetBottom());
    dc.DrawLine(rect.GetLeft(), rect.GetTop(),
                rect.GetRight(), rect.GetTop());
    dc.DrawLine(rect.GetLeft(), rect.GetBottom(),
                rect.GetRight() + 1, rect.GetBottom());

    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(rect.GetLeft(), rect.GetTop() + 1,
                rect.GetLeft(), rect.GetBottom());
    dc.DrawLine(rect.GetLeft(), rect.GetTop() + 1,
                rect.GetRight(), rect.GetTop() + 1);

    rect.Deflate(2);
}
