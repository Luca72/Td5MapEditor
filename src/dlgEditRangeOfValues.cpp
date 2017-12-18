/***************************************************************
 * Name:      dlgEditRangeOfValues.cpp
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2011-03-18
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#include "dlgEditRangeOfValues.h"

BEGIN_EVENT_TABLE(dlgEditRangeOfValues, wxDialog)
END_EVENT_TABLE()

dlgEditRangeOfValues::dlgEditRangeOfValues(wxWindow *parent, int type)
             : wxDialog(parent, wxID_ANY, wxString(_T("Edit Range Of Values dialog")),
                        wxDefaultPosition, wxSize(400, 300))
{
    // Sizers automatically ensure a workable layout.
    wxBoxSizer *sizerMain = new wxBoxSizer( wxVERTICAL );
    sizerMain->SetMinSize(wxSize(400, 300));

    // data sizer
    wxBoxSizer *sizerData = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *sizerDataTop = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer *sizerDataTopLeft = new wxBoxSizer( wxVERTICAL );
    sizerDataTopLeft->Add(new wxStaticText(this, wxID_ANY, _T("Top left Data")),
                   0, wxLEFT | wxALIGN_LEFT, 5 );

    m_ltValue = new wxTextCtrl( this, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(60,20), wxTE_RIGHT);
    sizerDataTopLeft->Add(m_ltValue, 0, wxLEFT | wxRIGHT | wxALIGN_LEFT, 15 );

    sizerDataTop->Add(sizerDataTopLeft, 1, wxEXPAND | wxALIGN_TOP | wxALL, 5);

    sizerDataTop->AddStretchSpacer();

    wxBoxSizer *sizerDataTopRight = new wxBoxSizer( wxVERTICAL );
    sizerDataTopRight->Add(new wxStaticText(this, wxID_ANY, _T("Top right Data")),
                   0, wxRIGHT | wxALIGN_RIGHT, 5 );

    m_rtValue = new wxTextCtrl( this, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(60,20), wxTE_RIGHT);
    sizerDataTopRight->Add(m_rtValue, 0, wxRIGHT | wxRIGHT | wxALIGN_RIGHT, 15 );

    sizerDataTop->Add(sizerDataTopRight, 1, wxEXPAND | wxALIGN_TOP | wxALL, 5);

    sizerData->Add(sizerDataTop, 1, wxEXPAND | wxALIGN_TOP | wxALL, 5);

    wxBoxSizer *sizerDataCenter = new wxBoxSizer( wxVERTICAL );

    sizerDataCenter->Add(new wxStaticText(this, wxID_ANY, _T("Constant Data")),
                   0, wxALIGN_CENTER, 5 );

    m_constValue = new wxTextCtrl( this, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(60,20), wxTE_RIGHT);
    sizerDataCenter->Add(m_constValue, 0, wxALIGN_CENTER, 15 );

    sizerData->Add(sizerDataCenter, 1, wxEXPAND | wxALIGN_TOP | wxALL, 5);

    wxBoxSizer *sizerDataBottom = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer *sizerDataBottomLeft = new wxBoxSizer( wxVERTICAL );

    sizerDataBottomLeft->Add(new wxStaticText(this, wxID_ANY, _T("Bottom left Data")),
                   0, wxLEFT | wxALIGN_LEFT, 5 );

    m_lbValue = new wxTextCtrl( this, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(60,20), wxTE_RIGHT);
    sizerDataBottomLeft->Add(m_lbValue, 0, wxLEFT | wxRIGHT | wxALIGN_LEFT, 15 );

    sizerDataBottom->Add(sizerDataBottomLeft, 1, wxEXPAND | wxALIGN_BOTTOM | wxALL, 5);

    sizerDataBottom->AddStretchSpacer();

    wxBoxSizer *sizerDataBottomRight = new wxBoxSizer( wxVERTICAL );

    sizerDataBottomRight->Add(new wxStaticText(this, wxID_ANY, _T("Bottom right Data")),
                   0, wxRIGHT | wxALIGN_RIGHT, 5 );

    m_rbValue = new wxTextCtrl( this, wxID_ANY, _T("0"), wxDefaultPosition, wxSize(60,20), wxTE_RIGHT);
    sizerDataBottomRight->Add(m_rbValue, 0, wxRIGHT | wxRIGHT | wxALIGN_RIGHT, 15 );

    sizerDataBottom->Add(sizerDataBottomRight, 1, wxEXPAND | wxALIGN_BOTTOM | wxALL, 5);

    sizerData->Add(sizerDataBottom, 1, wxEXPAND | wxALIGN_BOTTOM | wxALL, 5);
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

    if (type == RANGE_HORIZ_LINE)
    {
        m_lbValue->Disable();
        m_rbValue->Disable();
    }

    if (type == RANGE_VERT_LINE)
    {
        m_rtValue->Disable();
        m_rbValue->Disable();
    }
}

dlgEditRangeOfValues::~dlgEditRangeOfValues()
{
    //dtor
}
