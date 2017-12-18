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

DEFINE_EVENT_TYPE(wxEVT_GRID_COPY_TO_CLIPBOARD)
DEFINE_EVENT_TYPE(wxEVT_GRID_PASTE_FROM_CLIPBOARD)

BEGIN_EVENT_TABLE(ewxGrid, wxGrid)
    EVT_GRID_RANGE_SELECT(ewxGrid::OnRangeSelect)
END_EVENT_TABLE()

ewxGrid::ewxGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
    wxGrid(parent,id,pos,size,style,name), m_selection(wxT(""))
{

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

void ewxGrid::SetColLabelStringValue(int col, wxString strvalue)
{
    SetColLabelValue(col, strvalue);
}

void ewxGrid::SetRowLabelStringValue(int row, wxString strvalue)
{
    SetRowLabelValue(row, strvalue);
}


