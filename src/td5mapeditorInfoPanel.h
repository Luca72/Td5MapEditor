/***************************************************************
 * Name:      td5mapeditorInfoPanel.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-09
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef TD5MAPEDITORINFOPANEL_H
#define TD5MAPEDITORINFOPANEL_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "resource.h"

#include "td5mapeditorApp.h"
#include "td5mapeditorDoc.h"

#include <wx/listctrl.h>

class td5mapeditorView;

class td5mapeditorInfoPanel : public wxPanel
{
    DECLARE_CLASS(td5mapeditorInfoPanel)

    public:
        td5mapeditorInfoPanel(wxWindow* parent, wxView *view, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);
        virtual ~td5mapeditorInfoPanel();

        void UpdateList(td5mapeditorDoc *doc);
        void OnListItemSelected(wxListEvent& event);
        void OnHideEsoticMapsCheckBox(wxCommandEvent& event);
        void OnHideUnmodifiedMapsCheckBox(wxCommandEvent& event);
        void OnResetToBaseMapButton(wxCommandEvent& event);

        void SetFlags(bool hideEsoticMaps, bool hideUnmodifiedMaps);

    protected:
    private:
        wxView *m_view;

        bool m_hideEsoticMaps;
        bool m_hideUnmodifiedMaps;
        wxStaticText *m_baseMapName;
        wxListCtrl *m_listTables;
        wxCheckBox *m_hideEsoticMapsLabel;
        wxCheckBox *m_hideUnmodifiedMapsLabel;
        wxStaticText *m_mapRoleLabel;
        wxStaticText *m_mapAddressLabel;
        wxStaticText *m_mapTagLabel;
        wxTextCtrl *m_mapCommentText;
        wxButton *m_mapResetToBase;

public:
    DECLARE_EVENT_TABLE()
};

#endif // TD5MAPEDITORINFOPANEL_H
