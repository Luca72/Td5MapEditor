/***************************************************************
 * Name:      wizNewMap.cpp
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-29
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#include "wizNewMap.h"
#include "td5mapVariants.h"


wizNewMap::wizNewMap(wxFrame *frame, bool useSizer)
         : wxWizard(frame,wxID_ANY,_T("New Map Wizard"),
                   wxNullBitmap,wxDefaultPosition,
                   wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    // a wizard page may be either an object of predefined class
    m_page1 = new wizNewMapPage1(this);

    if ( useSizer )
    {
        // allow the wizard to size itself around the pages
        GetPageAreaSizer()->Add(m_page1);
    }
}

wizNewMap::~wizNewMap()
{
    //dtor
}

WIZ_NEW_MAP_DATA wizNewMap::TransferDataFromPages()
{
    td5mapVariants variants;
    WIZ_NEW_MAP_DATA wnmd;

    wnmd.mapID = variants.GetID(m_page1->selectedVariant);
    wnmd.mapName = variants.GetMapName(m_page1->selectedVariant);

    return wnmd;
}
