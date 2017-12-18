/***************************************************************
 * Name:      td5mapeditorCanvas.cpp
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-07
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#include "resource.h"

#include "td5mapeditorCanvas.h"

#include "td5mapeditorMainFrame.h"
#include "td5mapTable.h"


BEGIN_EVENT_TABLE(td5mapeditorCanvas, wxScrolledWindow)
    EVT_MOTION(td5mapeditorCanvas::OnMouseMove)
    EVT_LEFT_DOWN(td5mapeditorCanvas::OnMouseLeftDown)
    EVT_LEFT_UP(td5mapeditorCanvas::OnMouseLeftUp)
    EVT_SIZE(td5mapeditorCanvas::OnSize)
END_EVENT_TABLE()

// Define a constructor for canvas
td5mapeditorCanvas::td5mapeditorCanvas(wxWindow *parent, wxView *v, int canvastype, wxWindowID id):
    wxScrolledWindow(parent, id, wxDefaultPosition, wxDefaultSize)
{
    m_view = v;
    m_type = canvastype;
    m_show = GDC_SHOW_CURRENT;
    m_zoom = 100;
    m_horizScrollBase = 20;
    m_vertScrollBase = 20;

    m_graphDC = new graphDC(this);
}

td5mapeditorCanvas::~td5mapeditorCanvas()
{
    if(m_graphDC)
        delete m_graphDC;
}

void td5mapeditorCanvas::GetScrollbarsPos(int &xpos, int &ypos)
{
    xpos = m_xScrollPosition;
    ypos = m_yScrollPosition;
}

void td5mapeditorCanvas::SetGraphShow(int canvasshow)
{
    m_show = canvasshow;
    Refresh();
}

void td5mapeditorCanvas::IncreaseZoomLevel(int inc)
{
    if ((m_zoom + inc) <= 400)
        m_zoom = m_zoom + inc;

    SetZoomLevel(m_zoom);
}

void td5mapeditorCanvas::DecreaseZoomLevel(int dec)
{
    if ((m_zoom - dec) >= 100)
        m_zoom = m_zoom - dec;

    SetZoomLevel(m_zoom);

}

void td5mapeditorCanvas::SetZoomLevel(int level)
{
    m_zoom = level;
    double ratio = (double) m_zoom / 100.0;

    int xpos, ypos;
    GetScrollbarsPos(xpos, ypos);

    SetScrollbars((int) (20.0  * ratio), (int) (20.0 * ratio), m_horizScrollBase, m_vertScrollBase, xpos, ypos);
    Refresh();

    GetMainFrame()->SetStatusBarText(wxString::Format(_T("Zoom: %d%s"), level, _T("%")), 6);
}

// Define the repainting behaviour
void td5mapeditorCanvas::OnDraw(wxDC& dc)
{
    if(m_graphDC->Prepare(dc, wxRect(GetVirtualSize()), GetDocument()->GetSelectedMapTable(), m_show))
    {
        m_graphDC->Draw();
    }
}

void td5mapeditorCanvas::OnMouseMove(wxMouseEvent& event)
{
    if (m_view)
    {
        if(m_graphDC->IsPrepared())
        {
            int logicX, logicY;
            CalcUnscrolledPosition(event.GetX(), event.GetY(), &logicX, &logicY);
            m_graphDC->OnMouseMove(logicX, logicY);
        }
    }

    event.Skip();
}


void td5mapeditorCanvas::OnMouseLeftDown(wxMouseEvent& event)
{
    if (m_view)
    {
        if(m_graphDC->IsPrepared())
        {
            if(! m_graphDC->IsSelecting())
            {
                int logicX, logicY;
                CalcUnscrolledPosition(event.GetX(), event.GetY(), &logicX, &logicY);
                m_graphDC->OnMouseSelectBegin(logicX, logicY);
            }
        }
    }

    event.Skip();
}

void td5mapeditorCanvas::OnMouseLeftUp(wxMouseEvent& event)
{
    if (m_view)
    {
        if(m_graphDC->IsPrepared())
        {
            if(m_graphDC->IsSelecting())
            {
                int logicX, logicY;
                CalcUnscrolledPosition(event.GetX(), event.GetY(), &logicX, &logicY);
                m_graphDC->OnMouseSelectEnd(logicX, logicY);
            }
        }
    }

    event.Skip();
}


void td5mapeditorCanvas::OnSize(wxSizeEvent&
                                  #ifdef __WXUNIVERSAL__
                                  event
                                  #else
                                  WXUNUSED(event)
                                  #endif
                                  )
{
    int w, h;
    GetClientSize(&w, &h);

    m_horizScrollBase = (double) w / 20.0;
    m_vertScrollBase = (double) h / 20.0;

    SetZoomLevel(m_zoom);
}

void td5mapeditorCanvas::SetSelection(int x, int y)
{
    if(m_graphDC->IsPrepared())
        m_graphDC->MoveCursor(x, y);
}

void td5mapeditorCanvas::SetSelectionRange(int x1, int y1, int x2, int y2)
{
    if(m_graphDC->IsPrepared())
        m_graphDC->SelectRange(ewxRange(y1, x1, y2, x2));
}
