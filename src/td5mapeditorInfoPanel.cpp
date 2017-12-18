/***************************************************************
 * Name:      td5mapeditorInfoPanel.cpp
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

#include "td5mapeditorInfoPanel.h"

IMPLEMENT_CLASS(td5mapeditorInfoPanel, wxPanel)

BEGIN_EVENT_TABLE(td5mapeditorInfoPanel, wxPanel)
    EVT_LIST_ITEM_SELECTED(IDR_INFO_PANEL_MAPLIST_LISTCTRL, td5mapeditorInfoPanel::OnListItemSelected)
    EVT_CHECKBOX(IDR_INFO_PANEL_ESOTICMAPSHIDE_CHECKBOX, td5mapeditorInfoPanel::OnHideEsoticMapsCheckBox)
    EVT_CHECKBOX(IDR_INFO_PANEL_UNMODIFIEDMAPSHIDE_CHECKBOX, td5mapeditorInfoPanel::OnHideUnmodifiedMapsCheckBox)
END_EVENT_TABLE()

td5mapeditorInfoPanel::td5mapeditorInfoPanel(wxWindow* parent, wxView *view, wxWindowID id, const wxPoint& pos, const wxSize& size, long style):
    wxPanel(parent, id, pos, size, style, _T("Info Panel"))
{
    m_view = view;

    m_hideEsoticMaps = true;
    m_hideUnmodifiedMaps = false;

    m_listTables = new wxListCtrl( this,
                               IDR_INFO_PANEL_MAPLIST_LISTCTRL,
                               wxPoint( 0, 0 ),
                               wxSize( 280, 200 ),
                               wxLC_REPORT  );

    wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL );
    wxBoxSizer *mapnamesizer = new wxBoxSizer( wxHORIZONTAL );

    mapnamesizer->Add( new wxStaticText(this, -1, _T("Base firmware:")),
                   0,
                   wxLEFT | wxRIGHT,
                   10 );

    m_baseMapName = new wxStaticText(this, IDR_INFO_PANEL_BASEMAPNAME_STATIC, _T("Unknown"));
    mapnamesizer->Add( m_baseMapName,
                   1,
                   wxLEFT | wxRIGHT,
                   10 );

    topsizer->Add( mapnamesizer,
                   0,
                   wxTOP | wxBOTTOM,
                   10 );         // set border width to 10

    wxBoxSizer *maptagsizer = new wxBoxSizer( wxHORIZONTAL );

    maptagsizer->Add( new wxStaticText(this, -1, _T("Map tag:")),
                   0,
                   wxLEFT | wxRIGHT,
                   10 );

    m_mapTagLabel = new wxStaticText(this, IDR_INFO_PANEL_MAPTAG_STATIC, _T(""));
    maptagsizer->Add( m_mapTagLabel,
                   1,
                   wxLEFT | wxRIGHT,
                   10 );

    topsizer->Add( maptagsizer,
                   0,
                   wxTOP | wxBOTTOM,
                   10 );         // set border width to 10

    topsizer->Add( m_listTables,
                   1,
                   wxEXPAND | wxALL,
                   5 );         // set border width to 5

    m_hideEsoticMapsLabel = new wxCheckBox(this, IDR_INFO_PANEL_ESOTICMAPSHIDE_CHECKBOX, _T("Hide 'esotic' maps"));
    topsizer->Add( m_hideEsoticMapsLabel,
                   0,
                   wxLEFT | wxRIGHT | wxTOP,
                   10 );         // set border width to 10

    m_hideUnmodifiedMapsLabel = new wxCheckBox(this, IDR_INFO_PANEL_UNMODIFIEDMAPSHIDE_CHECKBOX, _T("Hide unmodified maps"));
    topsizer->Add( m_hideUnmodifiedMapsLabel,
                   0,
                   wxLEFT | wxRIGHT | wxBOTTOM,
                   10 );         // set border width to 10

    topsizer->Add( new wxStaticText(this, -1, _T("Map role:")),
                   0,
                   wxLEFT | wxRIGHT | wxTOP,
                   10 );

    m_mapRoleLabel = new wxStaticText(this, IDR_INFO_PANEL_MAPROLE_STATIC, _T("Unknown"));
    topsizer->Add( m_mapRoleLabel,
                   0,
                   wxEXPAND | wxALL,
                   5 );


    wxBoxSizer *mapaddresssizer = new wxBoxSizer( wxHORIZONTAL );

    mapaddresssizer->Add( new wxStaticText(this, -1, _T("Map address:")),
                   0,
                   wxLEFT | wxRIGHT,
                   10 );

    m_mapAddressLabel = new wxStaticText(this, IDR_INFO_PANEL_MAPADDRESS_STATIC, _T("Unknown"));
    mapaddresssizer->Add( m_mapAddressLabel,
                   1,
                   wxLEFT | wxRIGHT,
                   10 );

    topsizer->Add( mapaddresssizer,
                   0,
                   wxTOP | wxBOTTOM,
                   10 );         // set border width to 10


    topsizer->Add( new wxStaticText(this, -1, _T("Map comment:")),
                   0,
                   wxLEFT | wxRIGHT | wxTOP,
                   10 );

    m_mapCommentText = new wxTextCtrl( this, IDR_INFO_PANEL_MAPCOMMENT_TEXTCTRL, _T("Unknown"), wxDefaultPosition, wxSize(180,100), wxTE_MULTILINE | wxTE_READONLY);
    topsizer->Add( m_mapCommentText,
                   1,            // make vertically stretchable
                   wxEXPAND |    // make horizontally stretchable
                   wxALL,        //   and make border all around
                   5 );         // set border width to 10


    SetSizer( topsizer );      // use the sizer for layout

    topsizer->SetSizeHints( this );   // set size hints to honour minimum size

    m_listTables->InsertColumn(0, wxString(_T("#")), wxLIST_FORMAT_LEFT, 45);
    m_listTables->InsertColumn(1, wxString(_T("Addr.")), wxLIST_FORMAT_LEFT, 65);
    m_listTables->InsertColumn(2, wxString(_T("Size")), wxLIST_FORMAT_LEFT, 60);
    m_listTables->InsertColumn(3, wxString(_T("Name")), wxLIST_FORMAT_LEFT, 300);

    m_hideEsoticMapsLabel->SetValue(m_hideEsoticMaps);
    m_hideUnmodifiedMapsLabel->SetValue(m_hideUnmodifiedMaps);

}

td5mapeditorInfoPanel::~td5mapeditorInfoPanel()
{
    //dtor
}

void td5mapeditorInfoPanel::SetFlags(bool hideEsoticMaps, bool hideUnmodifiedMaps)
{
    m_hideEsoticMaps = hideEsoticMaps;
    m_hideUnmodifiedMaps = hideUnmodifiedMaps;

    m_hideEsoticMapsLabel->SetValue(m_hideEsoticMaps);
    m_hideUnmodifiedMapsLabel->SetValue(m_hideUnmodifiedMaps);
}


void td5mapeditorInfoPanel::UpdateList(td5mapeditorDoc *doc)
{
    if(!doc->GetUpdateFlag(INFO_PANEL))
        return;

    int selectedTable = doc->GetSelectedTable();
    m_baseMapName->SetLabel(doc->GetMapName());

    m_listTables->DeleteAllItems();

    for (int i=0; i < doc->GetNumberOfTables(); i++)
    {
        if (((doc->GetMapTable(i)->m_recognized == TRUE) || (m_hideEsoticMaps == false)) &&
            ((doc->GetMapTable(i)->IsDifferentFromOriginal()) || (m_hideUnmodifiedMaps == false)))
        {
            long index = m_listTables->InsertItem(i, wxString::Format(_T("%03d"), i), 0);
            m_listTables->SetItemData(index, i);
            m_listTables->SetItem(index, 1, wxString::Format(_T("0x%04X"), doc->GetMapTable(i)->m_address));
            m_listTables->SetItem(index, 2, wxString::Format(_T("%02dx%02d"), doc->GetMapTable(i)->m_cols,doc->GetMapTable(i)->m_rows));
            m_listTables->SetItem(index, 3, doc->GetMapTable(i)->m_name);
            if (doc->GetMapTable(i)->IsDifferentFromOriginal())
                m_listTables->SetItemFont(index, wxFont(8, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD , false));
            else
                m_listTables->SetItemFont(index, wxFont(8, wxDEFAULT, wxNORMAL, wxNORMAL, false));
            if (doc->GetMapTable(i)->IsSingleValue())
                m_listTables->SetItemTextColour(index,  wxColour( 60, 100, 225 ));
            else
                m_listTables->SetItemTextColour(index,  wxColour( 0, 0, 0 ));
        }
    }

    if(m_listTables->GetItemCount() > 0)
    {
        wxListItem item;

        item.SetId(0);
        item.SetColumn(0);
        item.SetMask(wxLIST_MASK_TEXT );
        m_listTables->GetItem(item);

        doc->SelectTable(selectedTable);

        m_mapTagLabel->SetLabel(doc->GetMapTag());

        m_mapAddressLabel->SetLabel(wxString::Format(_T("0x%04X"), doc->GetSelectedMapTable()->m_address));
        m_mapRoleLabel->SetLabel(doc->GetSelectedMapTable()->m_name);
        m_mapCommentText->SetValue(doc->GetSelectedMapTable()->m_comment);
    }

    doc->ResetUpdateFlag(INFO_PANEL);
}

void td5mapeditorInfoPanel::OnListItemSelected(wxListEvent& event)
{
    td5mapeditorDoc *doc = (td5mapeditorDoc *) m_view->GetDocument();

    long selectedtable;
    event.GetLabel().ToLong(&selectedtable);
    doc->SelectTable(selectedtable);

    m_mapAddressLabel->SetLabel(wxString::Format(_T("0x%04X"), doc->GetSelectedMapTable()->m_address));
    m_mapRoleLabel->SetLabel(doc->GetSelectedMapTable()->m_name);
    m_mapCommentText->SetValue(doc->GetSelectedMapTable()->m_comment);

    doc->SetUpdateFlag(GRID_PANEL | GRAPH_PANEL);
    doc->Update();
}

void td5mapeditorInfoPanel::OnHideEsoticMapsCheckBox(wxCommandEvent& event)
{
    td5mapeditorDoc *doc = (td5mapeditorDoc *) m_view->GetDocument();

    m_hideEsoticMaps = m_hideEsoticMapsLabel->GetValue();

    doc->SetUpdateFlag(GRID_PANEL | INFO_PANEL | GRAPH_PANEL);

    doc->Update();
}

void td5mapeditorInfoPanel::OnHideUnmodifiedMapsCheckBox(wxCommandEvent& event)
{
    td5mapeditorDoc *doc = (td5mapeditorDoc *) m_view->GetDocument();

    m_hideUnmodifiedMaps = m_hideUnmodifiedMapsLabel->GetValue();

    doc->SetUpdateFlag(GRID_PANEL | INFO_PANEL | GRAPH_PANEL);

    doc->Update();
}

