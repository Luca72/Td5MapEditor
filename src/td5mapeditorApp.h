/***************************************************************
 * Name:      td5mapeditorApp.h
 * Purpose:   Defines Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-07
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef TD5MAPEDITORAPP_H
#define TD5MAPEDITORAPP_H

#include <wx/app.h>
#include <wx/mdi.h>
#include <wx/docview.h>
#include <wx/docmdi.h>

#include "resource.h"
#include "functions.h"

class wxDocManager;

class td5mapeditorApp : public wxApp
{
  public:
    td5mapeditorApp(void);
    bool OnInit(void);
    int OnExit(void);

    wxMDIChildFrame *CreateChildFrame(wxDocument *doc, wxView *view, bool isCanvas);

  protected:
    wxDocManager* m_docManager;
};

DECLARE_APP(td5mapeditorApp)

#if wxUSE_PRINTING_ARCHITECTURE
//! global print data, to remember settings during the session
extern wxPrintData *g_printData;
extern wxPageSetupData *g_pageSetupData;
#endif // wxUSE_PRINTING_ARCHITECTURE

#endif // TD5MAPEDITORAPP_H
