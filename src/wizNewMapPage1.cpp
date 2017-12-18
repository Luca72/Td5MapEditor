/***************************************************************
 * Name:      wizNewMapPage1.cpp
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-29
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#include "wizNewMapPage1.h"

BEGIN_EVENT_TABLE(wizNewMapPage1, wxWizardPageSimple)
    EVT_LIST_ITEM_SELECTED(IDR_MODEL_LIST_LISTCTRL, wizNewMapPage1::OnListItemSelected)
END_EVENT_TABLE()


wizNewMapPage1::wizNewMapPage1(wxWizard* parent)
              : wxWizardPageSimple(parent, NULL, NULL, wxNullBitmap)
{
    selectedVariant = -1;

    SetMinSize(wxSize(550, 400));

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    m_listVariants = new wxListCtrl( this,
                               IDR_MODEL_LIST_LISTCTRL,
                               wxPoint( 0, 0 ),
                               wxSize( 400, 300 ),
                               wxLC_REPORT  );

    mainSizer->Add(
        m_listVariants,
        1, // No vertical stretching
        wxEXPAND | wxALL,
        10 // Border width
    );

    mainSizer->Add(
        new wxStaticText(this, wxID_ANY, _T("Select the Base Map")),
        0, // No vertical stretching
        wxALL,
        10 // Border width
    );

    SetSizer(mainSizer);
    mainSizer->Fit(this);

    m_listVariants->InsertColumn(0, wxString(_T("Base Map Name")), wxLIST_FORMAT_LEFT, 150);
    m_listVariants->InsertColumn(1, wxString(_T("Variant Name")), wxLIST_FORMAT_LEFT, 380);

    td5mapVariants *variants = new td5mapVariants;

    for (int i=0; i < variants->GetCount(); i++)
    {
        long index = m_listVariants->InsertItem(i, variants->GetMapName(i), 0);
        m_listVariants->SetItemData(index, i);
        m_listVariants->SetItem(index, 1, variants->GetVariant(i));
    }

    delete variants;
}

wizNewMapPage1::~wizNewMapPage1()
{
    delete m_listVariants;
}

bool wizNewMapPage1::TransferDataFromWindow()
{
    if ( selectedVariant == -1 )
    {
        wxMessageBox(_T("Select a Base Map first!"), _T("Not complete"), wxICON_WARNING | wxOK, this);

        return false;
    }

    return true;
}

void wizNewMapPage1::OnListItemSelected(wxListEvent& event)
{
    selectedVariant = event.GetIndex();
}

