/***************************************************************
 * Name:      td5mapeditorMainFrame.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-07
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef TD5MAPEDITORMAIN_H
#define TD5MAPEDITORMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "td5mapeditorApp.h"

#ifdef __WXGTK__
    /*#include "ewxMDIClientWindow.h"*/
#endif

// Define a new frame
class td5mapeditorCanvas;

class td5mapeditorMainFrame: public wxDocMDIParentFrame
{
    DECLARE_CLASS(td5mapeditorMainFrame)

    public:
        td5mapeditorMainFrame();
        td5mapeditorMainFrame(wxDocManager *manager, wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size, long type);
        bool Create(wxDocManager *manager, wxFrame *frame, const wxString& title, const wxPoint& pos, const wxSize& size, long type);
        void OnAbout(wxCommandEvent& event);

        void OnCopy(wxCommandEvent& event);
        void OnPaste(wxCommandEvent& event);
        void OnZoomMinus(wxCommandEvent& event);
        void OnZoomPlus(wxCommandEvent& event);
        void OnCurrentView(wxCommandEvent& event);
        void OnOriginalMapView(wxCommandEvent& event);
        void OnDifferencesView(wxCommandEvent& event);
        void OnAddOne(wxCommandEvent& event);
        void OnSubtractOne(wxCommandEvent& event);
        void OnEditRangeOfValues(wxCommandEvent& event);
        void OnFollowLocale(wxCommandEvent& event);
        void OnEditChecksum(wxCommandEvent& event);
        
        void EnableChildButtons();
        void DisableChildButtons();

        void SetStatusBarText(const wxString& text, int i);
        void SetStatusBarCellColours(int i, const wxColour& backgroundColour, const wxColour& foregroundColour = *wxBLACK);
#ifdef __WXGTK__
        /*virtual wxMDIClientWindow* OnCreateClient();*/
#endif

    public:
        wxToolBar *toolBar;
        wxStatusBar *statusBar;
        wxStaticText *statusBarCell[9];
        wxColor *statusBarForegroundColor;
        wxColor *statusBarBackgroundColor;
    private:
        void CreateLayout();
        //wxTimer m_cyclicTimer;
        //void OnCyclicTimerEvent(wxTimerEvent& event);
        void CreateStatusBarCell(int number);
        void PositionStatusBarCell(int number);
        void PositionStatusBarCells();
        void OnStatusBarSize(wxSizeEvent& event);


    DECLARE_EVENT_TABLE()
};

extern td5mapeditorMainFrame *GetMainFrame(void);

#define TD5MAPEDITOR_ABOUT   wxID_ABOUT
#define ID_TOOLBAR   500

extern td5mapeditorMainFrame *mainframe;

#endif // TD5MAPEDITORMAIN_H
