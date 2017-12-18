/***************************************************************
 * Name:      td5mapeditorGridPanel.cpp
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
#include <wx/clipbrd.h>

#include "td5mapeditorGridPanel.h"
#include "td5mapeditorDoc.h"
#include "td5mapeditorView.h"
#include "ewxRange.h"

IMPLEMENT_CLASS(td5mapeditorGridPanel, wxPanel)

BEGIN_EVENT_TABLE(td5mapeditorGridPanel, wxPanel)
    EVT_SIZE(td5mapeditorGridPanel::OnSize)
    EVT_MENU(wxID_COPY,td5mapeditorGridPanel::OnCopy)
    EVT_MENU(wxID_PASTE,td5mapeditorGridPanel::OnPaste)
    EVT_MENU(ID_CONTEXTMENU_MODIFY_LABEL_VALUE,td5mapeditorGridPanel::OnModifyLabelValue)
    EVT_GRID_CELL_RIGHT_CLICK(td5mapeditorGridPanel::OnRClick)
    EVT_GRID_CELL_CHANGE(td5mapeditorGridPanel::OnCellChange)
    EVT_GRID_COPY_TO_CLIPBOARD(td5mapeditorGridPanel::OnGridCopy)
    EVT_GRID_PASTE_FROM_CLIPBOARD(td5mapeditorGridPanel::OnGridPaste)
    EVT_GRID_RANGE_SELECT(td5mapeditorGridPanel::OnRangeSelect)
    EVT_GRID_SELECT_CELL(td5mapeditorGridPanel::OnCellSelect)
    EVT_KEY_DOWN( td5mapeditorGridPanel::OnKeyDown )
    EVT_GRID_LABEL_RIGHT_CLICK(td5mapeditorGridPanel::OnLabelRClick)
END_EVENT_TABLE()

td5mapeditorGridPanel::td5mapeditorGridPanel(wxWindow* parent, wxView *view, wxWindowID id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, style, _T("Info Panel"))
{
    m_view = (td5mapeditorView *) view;

    m_valueType = GRID_VALUE_CURRENT;
    m_gridCols = 10;
    m_gridRows = 6;
    m_clicked_label_row = - 1;
    m_clicked_label_col = - 1;

    // Create a wxGrid object
    m_grid = new ewxGrid( this,
                       -1,
                       wxPoint( 0, 0 ),
                       wxSize( 10, 10 ) );

    m_grid->CreateGrid( m_gridRows, m_gridCols );
    m_grid->InitLabelsColour(m_gridRows, m_gridCols);
    m_grid->SetDefaultCellFont(wxFont(8, wxDEFAULT, wxNORMAL, wxNORMAL, false));
    m_grid->SetLabelFont(wxFont(8, wxDEFAULT, wxNORMAL, wxNORMAL, false));
    m_grid->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);
    m_grid->SetDefaultRowSize(18, true);
    m_grid->SetRowLabelSize(50);
    m_grid->SetDefaultColSize(52, true);
    m_grid->SetColLabelSize(20);

    context_menu = new wxMenu();
    context_menu->Append(wxID_COPY, wxT("&Copy"));
    context_menu->Append(wxID_PASTE, wxT("&Paste"));
    context_menu->Enable(wxID_PASTE, false);

    label_context_menu = new wxMenu();
    label_context_menu->Append(ID_CONTEXTMENU_MODIFY_LABEL_VALUE, wxT("&Modify Value"));
    label_context_menu->Enable(ID_CONTEXTMENU_MODIFY_LABEL_VALUE, true);
}

td5mapeditorGridPanel::~td5mapeditorGridPanel()
{
    if(context_menu)
        delete context_menu;
    if(label_context_menu)
        delete label_context_menu;
}

void td5mapeditorGridPanel::OnSize(wxSizeEvent&
                                  #ifdef __WXUNIVERSAL__
                                  event
                                  #else
                                  WXUNUSED(event)
                                  #endif
                                  )
{
    int w, h;

    GetClientSize(&w, &h);
    m_grid->SetSize(w, h);

    // FIXME: On wxX11, we need the MDI frame to process this
    // event, but on other platforms this should not
    // be done.
#ifdef __WXUNIVERSAL__
    event.Skip();
#endif
}

void td5mapeditorGridPanel::SetValueType(int type)
{
    m_valueType = type;
    td5mapeditorDoc *doc = (td5mapeditorDoc *) m_view->GetDocument();
    doc->SetUpdateFlag(GRID_PANEL |GRAPH_PANEL);
    UpdateGrid(doc);
}

wxToolBar *td5mapeditorGridPanel::GetToolBar()
{
    return m_view->GetChildFrame()->GetMainFrame()->toolBar;
}

wxMenuBar *td5mapeditorGridPanel::GetMenuBar()
{
    return m_view->GetChildFrame()->GetMenuBar();
}

void td5mapeditorGridPanel::SetGridCellTextColour(int row, int col, short diff)
{
    if ((diff == 0) || (m_valueType == GRID_VALUE_BASE))
        m_grid->SetCellTextColour(row, col, *wxBLACK);
    else
    {
        if (diff > 0)
            m_grid->SetCellTextColour(row, col, *wxRED);
        else
            m_grid->SetCellTextColour(row, col, *wxBLUE);
    }
}

void td5mapeditorGridPanel::SetGridRowLabelTextColour(int row, short diff)
{
    if ((diff == 0) || (m_valueType == GRID_VALUE_BASE))
        m_grid->SetRowLabelTextColour(row, *wxBLACK);
    else
    {
        if (diff > 0)
            m_grid->SetRowLabelTextColour(row, *wxRED);
        else
            m_grid->SetRowLabelTextColour(row, *wxBLUE);
    }
}

void td5mapeditorGridPanel::SetGridColLabelTextColour(int col, short diff)
{
    if ((diff == 0) || (m_valueType == GRID_VALUE_BASE))
        m_grid->SetColLabelTextColour(col, *wxBLACK);
    else
    {
        if (diff > 0)
            m_grid->SetColLabelTextColour(col, *wxRED);
        else
            m_grid->SetColLabelTextColour(col, *wxBLUE);
    }
}

void td5mapeditorGridPanel::UpdateGrid(td5mapeditorDoc *doc)
{
    if(!doc->GetUpdateFlag(GRID_PANEL))
        return;

    //m_grid->ClearGrid();
    if((m_gridCols > 0) && (m_gridRows > 0))
    {
        m_grid->DeleteCols(0, m_gridCols);
        m_grid->DeleteRows(0, m_gridRows);
    }

	m_gridCols = doc->GetSelectedMapTable()->m_cols;
	m_gridRows = doc->GetSelectedMapTable()->m_rows;

    m_grid->InsertCols(0, m_gridCols);
    m_grid->InsertRows(0, m_gridRows);
    m_grid->InitLabelsColour(m_gridRows, m_gridCols);

    wxString strdata;
    short rawdata = 0;
    float sizeddata = 0.0;

	for (int row = 0; row < m_grid->GetNumberRows(); row++)
    {
        if(doc->GetSelectedMapTable()->m_singlevalue == false)
        {
            if((m_valueType == GRID_VALUE_CURRENT) || (m_valueType == GRID_VALUE_DIFF))
            {
                if(doc->GetSelectedMapTable()->m_rowlabelsized == false)
                {
                    SetGridRowLabelTextColour(row, doc->GetSelMapRowLabelDiffRawValue(row));
                    m_grid->SetRowLabelIntValue(row, doc->GetSelMapRowHeaderCurrentRawValue(row));
                }
                else
                {
                    SetGridRowLabelTextColour(row, doc->GetSelMapRowLabelDiffRawValue(row));
                    m_grid->SetRowLabelFloatValue(row, doc->GetSelMapRowHeaderCurrentSizedValue(row));
                }
            }
            if(m_valueType == GRID_VALUE_BASE)
            {
                if(doc->GetSelectedMapTable()->m_rowlabelsized == false)
                {
                    SetGridRowLabelTextColour(row, doc->GetSelMapRowLabelDiffRawValue(row));
                    m_grid->SetRowLabelIntValue(row, doc->GetSelMapRowHeaderBaseRawValue(row));
                }
                else
                {
                    SetGridRowLabelTextColour(row, doc->GetSelMapRowLabelDiffRawValue(row));
                    m_grid->SetRowLabelFloatValue(row, doc->GetSelMapRowHeaderBaseSizedValue(row));
                }

            }
        }
        else
            m_grid->SetRowLabelStringValue(row, "");
    }

    for (int col = 0; col < m_grid->GetNumberCols(); col++)
    {
        if(doc->GetSelectedMapTable()->m_singlevalue == false)
        {
            if((m_valueType == GRID_VALUE_CURRENT) || (m_valueType == GRID_VALUE_DIFF))
            {
                if(doc->GetSelectedMapTable()->m_collabelsized == false)
                {
                    SetGridColLabelTextColour(col, doc->GetSelMapColLabelDiffRawValue(col));
                    m_grid->SetColLabelIntValue(col, doc->GetSelMapColHeaderCurrentRawValue(col));
                }
                else
                {
                    SetGridColLabelTextColour(col, doc->GetSelMapColLabelDiffRawValue(col));
                    m_grid->SetColLabelFloatValue(col, doc->GetSelMapColHeaderCurrentSizedValue(col));
                }
            }
            if(m_valueType == GRID_VALUE_BASE)
            {
                if(doc->GetSelectedMapTable()->m_collabelsized == false)
                {
                    SetGridColLabelTextColour(col, doc->GetSelMapColLabelDiffRawValue(col));
                    m_grid->SetColLabelIntValue(col, doc->GetSelMapColHeaderBaseRawValue(col));
                }
                else
                {
                    SetGridColLabelTextColour(col, doc->GetSelMapColLabelDiffRawValue(col));
                    m_grid->SetColLabelFloatValue(col, doc->GetSelMapColHeaderBaseSizedValue(col));
                }
            }
        }
        else
            m_grid->SetColLabelStringValue(col, "");
    }

	for (int row = 0; row < m_grid->GetNumberRows(); row++)
        for (int col = 0; col < m_grid->GetNumberCols(); col++)
        {
            if(m_valueType == GRID_VALUE_CURRENT)
            {
                if(doc->GetSelectedMapTable()->m_datasized == false)
                    rawdata = doc->GetSelMapCurrentRawValue(col, row);
                else
                    sizeddata = doc->GetSelMapCurrentSizedValue(col, row);
            }
            if(m_valueType == GRID_VALUE_BASE)
            {
                if(doc->GetSelectedMapTable()->m_datasized == false)
                    rawdata = doc->GetSelMapBaseRawValue(col, row);
                else
                    sizeddata = doc->GetSelMapBaseSizedValue(col, row);
            }
            if(m_valueType == GRID_VALUE_DIFF)
            {
                if(doc->GetSelectedMapTable()->m_datasized == false)
                    rawdata = doc->GetSelMapDiffRawValue(col, row);
                else
                    sizeddata = doc->GetSelMapDiffSizedValue(col, row);
            }

            if(doc->GetSelectedMapTable()->m_datasized == false)
                m_grid->SetCellIntValue(row, col, rawdata);
            else
                m_grid->SetCellFloatValue(row, col, sizeddata);

            SetGridCellTextColour(row, col, doc->GetSelMapDiffRawValue(col, row));
		}

    doc->ResetUpdateFlag(GRID_PANEL);
}


void td5mapeditorGridPanel::OnCopy(wxCommandEvent& WXUNUSED(event))
{
    m_grid->CopyToClipboard();
}

void td5mapeditorGridPanel::OnPaste(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorDoc *doc = (td5mapeditorDoc *) m_view->GetDocument();
    m_grid->PasteFromClipboard();
    doc->Modify(true);
    doc->SetUpdateFlag(INFO_PANEL | GRAPH_PANEL);
    doc->Update(NULL);
}

void td5mapeditorGridPanel::OnRClick(wxGridEvent& event)
{
    if (wxTheClipboard->Open())
    {
        if (wxTheClipboard->IsSupported( wxDF_TEXT ))
            context_menu->Enable(wxID_PASTE, true);
        else
            context_menu->Enable(wxID_PASTE, false);

        wxTheClipboard->Close();
    }

    m_view->GetFrame()->PopupMenu(context_menu);
}

void td5mapeditorGridPanel::OnLabelRClick(wxGridEvent& event)
{
    td5mapeditorDoc *doc = (td5mapeditorDoc *) m_view->GetDocument();
    m_clicked_label_row = event.GetRow();
    m_clicked_label_col = event.GetCol();

    if(m_valueType == GRID_VALUE_CURRENT)
    {
        if(doc->GetSelectedMapTable()->IsSingleValue())
        {
            wxMessageBox( wxT("Single value tables does not\nallow header editing"), wxT("Invalid Header"), wxICON_ERROR);
            return;
        }

        if(doc->GetSelectedMapTable()->IsBidimensional() &&  (m_clicked_label_row > -1))
        {
            wxMessageBox( wxT("Bidimensional tables does not\nallow row header editing"), wxT("Invalid Header"), wxICON_ERROR);
            return;
        }

        if((m_clicked_label_row == -1) && (m_clicked_label_col == -1))
        {
            wxMessageBox( wxT("Wrong header"), wxT("Invalid Header"), wxICON_ERROR);
            return;
        }

        PopupMenu(label_context_menu);
    }
    else
        wxMessageBox( wxT("Header editing is allowed\nonly in CURRENT VIEW"), wxT("Invalid View"), wxICON_ERROR);
}

void td5mapeditorGridPanel::OnModifyLabelValue(wxCommandEvent& WXUNUSED(event))
{
    td5mapeditorDoc *doc = (td5mapeditorDoc *) m_view->GetDocument();
    wxString label_value;

    if(m_clicked_label_row > -1)
    {
        label_value = m_grid->GetRowLabelValue(m_clicked_label_row);
    }
    if(m_clicked_label_col > -1)
    {
        label_value = m_grid->GetColLabelValue(m_clicked_label_col);
    }

    wxString sNewValue = wxGetTextFromUser(wxT("Enter the new value"), wxT("Modify Label Value"), label_value);
    if (sNewValue.Cmp(label_value) == 0)
        return;

    // check for invalid entries
    if (sNewValue.IsEmpty())
    {
        wxMessageBox( wxT("Empty cell is not accepted"), wxT("Invalid Entry"), wxICON_ERROR);
        return;
    }

    if(
        (m_clicked_label_row > -1) && (doc->GetSelectedMapTable()->m_rowlabelsized == false)
        ||
        (m_clicked_label_col > -1) && (doc->GetSelectedMapTable()->m_collabelsized == false)
       )
    {
        long value;
        if(!sNewValue.ToLong(&value))
        {
            wxMessageBox( wxT("Integer value expected"), wxT("Invalid Entry"), wxICON_ERROR);
            return;
        }
    }

    if(
        (m_clicked_label_row > -1) && (doc->GetSelectedMapTable()->m_rowlabelsized == true)
        ||
        (m_clicked_label_col > -1) && (doc->GetSelectedMapTable()->m_collabelsized == true)
       )
    {
        double value;
        if(!sNewValue.ToDouble(&value))
        {
            wxMessageBox( wxT("Float value expected"), wxT("Invalid Entry"), wxICON_ERROR);
            return;
        }
        sNewValue.Printf(wxT("%0.1f"), value);
    }


    // entry is valid, proceed saving in database
    if(m_clicked_label_row > -1)
    {
        m_grid->SetRowLabelValue(m_clicked_label_row, sNewValue);

        if(doc->GetSelectedMapTable()->m_rowlabelsized == false)
            doc->SetSelMapRowHeaderCurrentRawValue(m_clicked_label_row, m_grid->GetRowLabelIntValue(m_clicked_label_row));
        else
            doc->SetSelMapRowHeaderCurrentSizedValue(m_clicked_label_row, m_grid->GetRowLabelFloatValue(m_clicked_label_row));

        SetGridRowLabelTextColour(m_clicked_label_row, doc->GetSelMapRowLabelDiffRawValue(m_clicked_label_row));
    }

    if(m_clicked_label_col > -1)
    {
        m_grid->SetColLabelValue(m_clicked_label_col, sNewValue);

        if(doc->GetSelectedMapTable()->m_collabelsized == false)
            doc->SetSelMapColHeaderCurrentRawValue(m_clicked_label_col, m_grid->GetColLabelIntValue(m_clicked_label_col));
        else
            doc->SetSelMapColHeaderCurrentSizedValue(m_clicked_label_col, m_grid->GetColLabelFloatValue(m_clicked_label_col));


        SetGridColLabelTextColour(m_clicked_label_col, doc->GetSelMapColLabelDiffRawValue(m_clicked_label_col));
    }

    doc->Modify(true);
    doc->SetUpdateFlag(INFO_PANEL | GRAPH_PANEL);
    doc->Update(NULL);
}


void td5mapeditorGridPanel::OnKeyDown(wxKeyEvent& event)
{
    int keycode = event.GetKeyCode();

    switch (keycode)
    {
        case 67:
        case 99:
        {
            if (event.ControlDown())
            {
                m_grid->CopyToClipboard();
            }
            break;
        }

        case 86:
        case 118:
        {
            if (event.ControlDown())
            {
                m_grid->PasteFromClipboard();
            }
            break;
        }

        case 388: // keypad +
        {
            if (event.ControlDown())
            {
                OnAddOne();
            }
            break;
        }

        case 390: // keypad -
        {
            if (event.ControlDown())
            {
                OnSubtractOne();
            }
            break;
        }

        default:
            break;
    }

    event.Skip();
}

void td5mapeditorGridPanel::OnCellChange(wxGridEvent& event)
{
    int row = event.GetRow();
    int col = event.GetCol();

    td5mapeditorDoc *doc = (td5mapeditorDoc *) m_view->GetDocument();

    if(m_valueType == GRID_VALUE_CURRENT)
    {
        if(doc->GetSelectedMapTable()->m_datasized == false)
            doc->SetSelMapCurrentRawValue(col, row, m_grid->GetCellIntValue(row, col));
        else
            doc->SetSelMapCurrentSizedValue(col, row, m_grid->GetCellFloatValue(row, col));
        SetGridCellTextColour(row, col, doc->GetSelMapDiffRawValue(col, row));
        doc->Modify(true);
    }

    if(m_valueType == GRID_VALUE_DIFF)
    {
        if(doc->GetSelectedMapTable()->m_datasized == false)
            doc->SetSelMapCurrentRawValue(col, row, m_grid->GetCellIntValue(row, col) + doc->GetSelMapBaseRawValue(col, row));
        else
            doc->SetSelMapCurrentSizedValue(col, row, m_grid->GetCellFloatValue(row, col) + doc->GetSelMapBaseSizedValue(col, row));
        SetGridCellTextColour(row, col, doc->GetSelMapDiffRawValue(col, row));
        doc->Modify(true);
    }

    doc->SetUpdateFlag(INFO_PANEL | GRAPH_PANEL);
    doc->Update(NULL);
}

void td5mapeditorGridPanel::OnGridCopy(wxGridRangeSelectEvent& event)
{
    GetToolBar()->EnableTool( wxID_PASTE, true );
    GetMenuBar()->Enable( wxID_PASTE, true );
}

void td5mapeditorGridPanel::OnGridPaste(wxGridRangeSelectEvent& event)
{
    td5mapeditorDoc *doc = (td5mapeditorDoc *) m_view->GetDocument();

    for (int row = event.GetTopRow(); row <= event.GetBottomRow(); ++row)
    {
        for (int col = event.GetLeftCol(); col <= event.GetRightCol(); ++col)
        {
            if(m_valueType == GRID_VALUE_CURRENT)
            {
                if(doc->GetSelectedMapTable()->m_datasized == false)
                    doc->SetSelMapCurrentRawValue(col, row, m_grid->GetCellIntValue(row, col));
                else
                    doc->SetSelMapCurrentSizedValue(col, row, m_grid->GetCellFloatValue(row, col));
                doc->Modify(true);
            }

            if(m_valueType == GRID_VALUE_DIFF)
            {
                if(doc->GetSelectedMapTable()->m_datasized == false)
                    doc->SetSelMapCurrentRawValue(col, row, m_grid->GetCellIntValue(row, col) + doc->GetSelMapBaseRawValue(col, row));
                else
                    doc->SetSelMapCurrentSizedValue(col, row, m_grid->GetCellFloatValue(row, col) + doc->GetSelMapBaseSizedValue(col, row));
                doc->Modify(true);
            }

            SetGridCellTextColour(row, col, doc->GetSelMapDiffRawValue(col, row));
        }
    }

    doc->SetUpdateFlag(INFO_PANEL | GRAPH_PANEL);
    doc->Update(NULL);
}

void td5mapeditorGridPanel::OnRangeSelect(wxGridRangeSelectEvent& event)
{
    if(event.Selecting())
    {
        m_view->GetChildFrame()->SetSelectionRange(ewxRange(event.GetTopRow(),
                                                            event.GetLeftCol(),
                                                            event.GetBottomRow(),
                                                            event.GetRightCol()),
                                                   GetId());

        GetMenuBar()->Enable(ID_TOOLS_EDIT_RANGE_OF_VALUES, true);
    }

    event.Skip();
}

void td5mapeditorGridPanel::OnCellSelect(wxGridEvent& event)
{
    m_view->GetChildFrame()->SetSelection(event.GetCol(), event.GetRow(), GetId());

    GetMenuBar()->Enable(ID_TOOLS_EDIT_RANGE_OF_VALUES, false);
    event.Skip();
}

void td5mapeditorGridPanel::OnSumCellValue(int value)
{
    if(m_valueType == GRID_VALUE_BASE)
        return;

    wxString strdata;
    short rawdata = 0;
    float sizeddata = 0.0;

    td5mapeditorDoc *doc = (td5mapeditorDoc *) m_view->GetDocument();

    if (m_grid->IsSelection())
    {
        ewxRange range;
        m_grid->GetSelectionCoordinates(range);

        for(int row = range.topRow; row <= range.bottomRow; ++row)
        {
            for(int col = range.leftCol; col <= range.rightCol; ++col)
            {
                if(m_valueType == GRID_VALUE_CURRENT)
                {
                    if(doc->GetSelectedMapTable()->m_datasized == false)
                    {
                        rawdata = doc->GetSelMapCurrentRawValue(col, row) + value;
                        m_grid->SetCellIntValue( row, col, rawdata);
                        doc->SetSelMapCurrentRawValue(col, row, rawdata);
                    }
                    else
                    {
                        sizeddata = doc->GetSelMapCurrentSizedValue(col, row) + value;
                        m_grid->SetCellFloatValue( row, col, sizeddata);
                        doc->SetSelMapCurrentSizedValue(col, row, sizeddata);
                    }
                }

                if(m_valueType == GRID_VALUE_DIFF)
                {
                    if(doc->GetSelectedMapTable()->m_datasized == false)
                    {
                        rawdata = doc->GetSelMapDiffRawValue(col, row) + value;
                        m_grid->SetCellIntValue( row, col, rawdata);
                        doc->SetSelMapCurrentRawValue(col, row, doc->GetSelMapCurrentRawValue(col, row) + value);
                    }
                    else
                    {
                        sizeddata = doc->GetSelMapDiffSizedValue(col, row) + value;
                        m_grid->SetCellFloatValue( row, col, sizeddata);
                        doc->SetSelMapCurrentSizedValue(col, row, doc->GetSelMapCurrentSizedValue(col, row) + value);
                    }
                }

                SetGridCellTextColour(row, col, doc->GetSelMapDiffRawValue(col, row));
            }
        }
    }
    else
    {
        int row = m_grid->GetGridCursorRow();
        int col = m_grid->GetGridCursorCol();

        if(m_valueType == GRID_VALUE_CURRENT)
        {
            if(doc->GetSelectedMapTable()->m_datasized == false)
            {
                rawdata = doc->GetSelMapCurrentRawValue(col, row) + value;
                m_grid->SetCellIntValue( row, col, rawdata);
                doc->SetSelMapCurrentRawValue(col, row, rawdata);
            }
            else
            {
                sizeddata = doc->GetSelMapCurrentSizedValue(col, row) + value;
                m_grid->SetCellFloatValue( row, col, sizeddata);
                doc->SetSelMapCurrentSizedValue(col, row, sizeddata);
            }
        }

        if(m_valueType == GRID_VALUE_DIFF)
        {
            if(doc->GetSelectedMapTable()->m_datasized == false)
            {
                rawdata = doc->GetSelMapDiffRawValue(col, row) + value;
                m_grid->SetCellIntValue( row, col, rawdata);
                doc->SetSelMapCurrentRawValue(col, row, doc->GetSelMapCurrentRawValue(col, row) + value);
            }
            else
            {
                sizeddata = doc->GetSelMapDiffSizedValue(col, row) + value;
                m_grid->SetCellFloatValue( row, col, sizeddata);
                doc->SetSelMapCurrentSizedValue(col, row, doc->GetSelMapCurrentSizedValue(col, row) + value);
            }
        }

        SetGridCellTextColour(row, col, doc->GetSelMapDiffRawValue(col, row));
    }

    doc->Modify(true);
    doc->SetUpdateFlag(INFO_PANEL | GRAPH_PANEL);
    doc->Update(NULL);
}
