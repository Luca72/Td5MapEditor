/***************************************************************
 * Name:      wizNewMapPage1.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-29
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef WIZNEWMAPPAGE1_H
#define WIZNEWMAPPAGE1_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/wizard.h>
#include <wx/listctrl.h>
#include "td5mapVariants.h"

class wizNewMapPage1 : public wxWizardPageSimple
{
    public:
        wizNewMapPage1(wxWizard* parent);
        virtual ~wizNewMapPage1();
        virtual bool TransferDataFromWindow();
        void OnListItemSelected(wxListEvent& event);

        wxListCtrl *m_listVariants;
        long selectedVariant;

        DECLARE_EVENT_TABLE()
};

#define IDR_MODEL_LIST_LISTCTRL     100

#endif // WIZNEWMAPPAGE1_H
