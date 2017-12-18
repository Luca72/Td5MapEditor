/***************************************************************
 * Name:      dlgEditTag.cpp
 * Purpose:   Defines Edit Tag Dialog
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2012-05-29
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef DLGEDITTAG_H
#define DLGEDITTAG_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/dialog.h>

class dlgEditTag : public wxDialog
{
    public:
        dlgEditTag(wxWindow *parent);
        virtual ~dlgEditTag();

        void SetTag(wxString tag){m_tag->SetValue(tag);}
        wxString GetTag(){return m_tag->GetValue();}


    protected:
        wxTextCtrl  *m_tag;
    private:

    DECLARE_EVENT_TABLE()
};

#endif // DLGEDITTAG_H
