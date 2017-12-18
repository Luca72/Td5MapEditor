/***************************************************************
 * Name:      td5mapeditorCanvas.h
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-07
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef TD5MAPEDITORCANVAS_H
#define TD5MAPEDITORCANVAS_H

#include <wx/mdi.h>
#include <wx/docview.h>

#include "resource.h"

#include "td5mapeditorDoc.h"
#include "graphDC.h"

class td5mapeditorCanvas: public wxScrolledWindow
{
    public:
        td5mapeditorCanvas(wxWindow *parent, wxView *v, int canvastype, wxWindowID id);
        ~td5mapeditorCanvas();
        void SetGraphShow(int canvasshow);
        void SetZoomLevel(int level);
        void DecreaseZoomLevel(int dec = 10);
        void IncreaseZoomLevel(int inc = 10);
        int GetZoomLevel() {return m_zoom;};
        void OnSize(wxSizeEvent& event);
        virtual void OnDraw(wxDC& dc);
        void OnMouseMove(wxMouseEvent& event);
        void OnMouseLeftDown(wxMouseEvent& event);
        void OnMouseLeftUp(wxMouseEvent& event);
        void GetScrollbarsPos(int &xpos, int &ypos);
        void SetSelection(int x, int y);
        void SetSelectionRange(int x1, int y1, int x2, int y2);
        void ResetSelectionRange(){ m_graphDC->ResetRange(); };

        wxView *m_view;

    private:
        td5mapeditorDoc *GetDocument(){return (td5mapeditorDoc *) m_view->GetDocument();};
        DECLARE_EVENT_TABLE()

    protected:
        int m_canvastype;
        int m_show;
        int m_zoom;

        int m_horizScrollBase;
        int m_vertScrollBase;

        graphDC *m_graphDC;
};

#define CANVAS_MAIN 1
#define CANVAS_DIFF 2

#endif // TD5MAPEDITORCANVAS_H
