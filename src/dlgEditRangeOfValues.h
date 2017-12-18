/***************************************************************
 * Name:      dlgEditRangeOfValues.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2011-03-18
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef DLGEDITRANGEOFVALUES_H
#define DLGEDITRANGEOFVALUES_H


#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/dialog.h>

#define RANGE_TABLE         1
#define RANGE_HORIZ_LINE    2
#define RANGE_VERT_LINE     3

class dlgEditRangeOfValues : public wxDialog
{
    public:
        dlgEditRangeOfValues(wxWindow *parent, int type = RANGE_TABLE);
        virtual ~dlgEditRangeOfValues();

        wxTextCtrl  *m_ltValue;
        wxTextCtrl  *m_rtValue;
        wxTextCtrl  *m_lbValue;
        wxTextCtrl  *m_rbValue;
        wxTextCtrl  *m_constValue;

    protected:

    private:

    DECLARE_EVENT_TABLE()
};

#endif // DLGEDITRANGEOFVALUES_H
