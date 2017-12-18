/***************************************************************
 * Name:      dlgEditTag.cpp
 * Purpose:   Defines Edit Tag Dialog
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2012-05-29
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#include "dlgEditTag.h"

BEGIN_EVENT_TABLE(dlgEditTag, wxDialog)
END_EVENT_TABLE()

dlgEditTag::dlgEditTag(wxWindow *parent)
             : wxDialog(parent, wxID_ANY, wxString(_T("Edit Tag dialog")),
                        wxDefaultPosition, wxSize(200, 80))
{
    // Sizers automatically ensure a workable layout.
    wxBoxSizer *sizerMain = new wxBoxSizer( wxVERTICAL );
    sizerMain->SetMinSize(wxSize(300, 80));

    // data sizer
    wxBoxSizer *sizerData = new wxBoxSizer( wxVERTICAL );

    // tag sizer
    wxBoxSizer *sizerTag = new wxBoxSizer( wxVERTICAL );

    m_tag = new wxTextCtrl( this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_LEFT);
    sizerTag->Add(m_tag, 0, wxEXPAND | wxLEFT | wxRIGHT | wxALIGN_LEFT, 15 );

    sizerData->Add(sizerTag, 1, wxEXPAND | wxALIGN_BOTTOM | wxALL, 5);

    sizerMain->Add(sizerData, 1, wxEXPAND | wxALIGN_TOP | wxALL, 5);

    // buttons sizer
    wxBoxSizer *sizerButtons = new wxBoxSizer( wxHORIZONTAL );

    wxButton *btnOk = new wxButton(this, wxID_OK, _T("&Ok"));
    sizerButtons->Add(btnOk, 0, wxALIGN_CENTER | wxALL, 5);

    wxButton *btnCancel = new wxButton(this, wxID_CANCEL, _T("&Cancel"));
    sizerButtons->Add(btnCancel, 0, wxALIGN_CENTER | wxALL, 5);

    sizerMain->Add(sizerButtons, 0, wxALIGN_BOTTOM | wxALIGN_RIGHT | wxALL, 5);

    SetSizer(sizerMain);
    sizerMain->SetSizeHints(this);
    sizerMain->Fit(this);
}

dlgEditTag::~dlgEditTag()
{
    //dtor
}
