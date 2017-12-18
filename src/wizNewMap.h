/***************************************************************
 * Name:      wizNewMap.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-29
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef WIZNEWMAP_H
#define WIZNEWMAP_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/docmdi.h>
#include <wx/wizard.h>

#include "wizNewMapPage1.h"

struct WIZ_NEW_MAP_DATA
{
    long mapID;
    wxString mapName;
};

class wizNewMap : public wxWizard
{
    public:
        wizNewMap(wxFrame *frame, bool useSizer = true);
        virtual ~wizNewMap();
        wxWizardPage *GetFirstPage() const { return m_page1; }
        WIZ_NEW_MAP_DATA TransferDataFromPages();

    protected:
    private:
        wizNewMapPage1 *m_page1;
};

#endif // WIZNEWMAP_H
