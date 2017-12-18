/***************************************************************
 * Name:      td5mapeditorView.cpp
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-07
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef __TD5MAPEDITORVIEWH__
#define __TD5MAPEDITORVIEWH__

#include "wx/docview.h"

#include "td5mapeditorCanvas.h"
#include "td5mapeditorChildFrame.h"
#include "td5mapeditorChildFrame.h"
#include "td5mapeditorInfoPanel.h"
#include "td5mapeditorGridPanel.h"

class td5mapeditorChildFrame;

class td5mapeditorView: public wxView
{
public:
    td5mapeditorChildFrame *frame;

    td5mapeditorView();
    ~td5mapeditorView();

    bool OnCreate(wxDocument *doc, long flags);
    void OnDraw(wxDC *dc);
    void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
    bool OnClose(bool deleteWindow = true);
    void SetTitle(wxString title);
    void SetInfoPanelFlags(bool hideEsoticMaps, bool hideUnmodifiedMaps);

    td5mapeditorCanvas *GetMainGraphCanvas();
    td5mapeditorCanvas *GetDiffGraphCanvas();
    td5mapeditorInfoPanel *GetInfoPanel();
    td5mapeditorGridPanel *GetGridPanel();

    td5mapeditorDoc *GetDoc();
    td5mapeditorChildFrame *GetChildFrame() {return frame;};

private:
    DECLARE_DYNAMIC_CLASS(td5mapeditorView)
    DECLARE_EVENT_TABLE()
};

#endif
