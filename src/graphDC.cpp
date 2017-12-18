/***************************************************************
 * Name:      graphDC.cpp
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-17
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

#include "graphDC.h"
#include "td5mapeditorMainFrame.h"
#include "td5mapeditorChildFrame.h"

#define LEFT_RIGHT_MARGIN   60
#define TOP_BOTTOM_MARGIN   60

// graphCursor
graphCursor::graphCursor(graphDC *pDC, wxSize size, bool visible)
{
    m_dc = pDC;
    m_xpos = 0.0;
    m_ypos = 0.0;
    m_zpos = 0.0;
    m_size = size;
    m_visible = visible;
}

graphCursor::~graphCursor()
{

}

point3d graphCursor::Move(double xpos, double ypos, double zpos)
{
    point3d oldPoint(m_xpos, m_ypos, m_zpos);

    m_xpos = xpos;
    m_ypos = ypos;
    m_zpos = zpos;

    return oldPoint;
}

wxPoint graphCursor::Move(int xpos, int ypos)
{
    wxPoint oldPoint(m_xpos, m_ypos);

    m_xpos = xpos;
    m_ypos = ypos;

    return oldPoint;
}

void graphCursor::SetVisible(bool visible)
{
    m_visible = visible;
}


// graphDC
graphDC::graphDC(wxScrolledWindow *parent)
{
    m_parent = parent;
    m_prepared = false;
    m_cursor = new graphCursor(this);
    m_cursor->SetVisible(true);
    m_selecting = false;
    m_selectionBegin = wxPoint(0, 0);
    m_selectionEnd = wxPoint(0, 0);
}

graphDC::~graphDC()
{
    if(m_cursor)
        delete m_cursor;
}

graphDC::graphDC(wxDC& DC, wxScrolledWindow *parent, wxRect canvasrect, td5mapTable *maptable, int graphshow)
{
    m_parent = parent;

    Prepare(DC, canvasrect, maptable, graphshow);

    m_cursor = new graphCursor(this);
    m_cursor->SetVisible(true);
    m_selecting = false;
    m_selectionBegin = wxPoint(0, 0);
    m_selectionEnd = wxPoint(0, 0);
}

bool graphDC::Prepare(wxDC& DC, wxRect canvasrect, td5mapTable *maptable, int graphshow)
{
    if ((maptable->GetCols() == 0) || (maptable->GetRows() == 0))
    {
        m_prepared = false;
        return false;
    }

    SetDC(&DC);

    m_table = maptable;
    m_axisRect = canvasrect;
    m_show = graphshow;

    int minY, maxY;

    if(m_show == GDC_SHOW_DIFF)
        m_table->EvalDiffRange(minY, maxY);
    else
        m_table->EvalRange(minY, maxY);

    if(m_table->IsBidimensional())
    {
        if(!m_table->IsSingleValue())
        {
            m_yaxisTagsNum = 10;
            m_xaxisTagsNum = m_table->GetCols();
            m_zaxisTagsNum = 0;

            SetRange(0, m_table->GetCols(), minY, maxY);
        }
        else
        {
            m_yaxisTagsNum = 10;
            m_xaxisTagsNum = 2;
            m_zaxisTagsNum = 0;

            SetRange(0, 2, minY, maxY);
        }
    }
    if(m_table->IsTridimensional())
    {
        m_xaxisTagsNum = m_table->GetRows() - 1;
        m_yaxisTagsNum = 10;
        m_zaxisTagsNum = m_table->GetCols() - 1;

        SetRange(-(m_table->GetRows() - 1), (m_table->GetRows() - 1), minY, maxY, -(m_table->GetCols() - 1), (m_table->GetCols() - 1));
    }

    m_points.resize(m_table->GetCols(), m_table->GetRows());

    if(m_table->IsBidimensional())
    {
        for(int c = 0; c < m_table->GetCols(); c++)
        {
            int data = 0;

            if((m_show == GDC_SHOW_CURRENT) || (m_show == GDC_SHOW_BASE_CURRENT))
            {
                data = m_table->m_tableData[c][0].current;
            }
            if(m_show == GDC_SHOW_DIFF)
            {
                data = m_table->m_tableData[c][0].current - m_table->m_tableData[c][0].base;
            }
            if(m_show == GDC_SHOW_BASE)
            {
                data = m_table->m_tableData[c][0].base;
            }

            m_points[c][0] = point3d((double)c, (double)data, 0.0);
        }
    }

    if(m_table->IsTridimensional())
    {
        for(int c = 0; c < m_table->GetCols(); c++)
            for(int r = 0; r < m_table->GetRows(); r++)
            {
                int data = 0;

                if((m_show == GDC_SHOW_CURRENT) || (m_show == GDC_SHOW_BASE_CURRENT))
                {
                    data = m_table->m_tableData[c][r].current;
                }
                if(m_show == GDC_SHOW_DIFF)
                {
                    data = m_table->m_tableData[c][r].current - m_table->m_tableData[c][r].base;
                }
                if(m_show == GDC_SHOW_BASE)
                {
                    data = m_table->m_tableData[c][r].base;
                }

                m_points[c][r] = point3d((double)r, (double)data, (double)(m_maxZAxis - c));
            }
    }

    m_prepared = true;

    return m_prepared;
}

void graphDC::SetRange(double minX, double maxX, double minY, double maxY, double minZ, double maxZ)
{
    dc3d::SetRange(minX, maxX, minY, maxY, minZ, maxZ);

    m_maxXAxis = maxX;
    m_maxYAxis = maxY;
    m_maxZAxis = maxZ;
    m_minXAxis = minX;
    m_minYAxis = minY;
    m_minZAxis = minZ;

    m_yaxisRes = 0.0;
    m_xaxisRes = 0.0;
    m_zaxisRes = 0.0;

    m_yaxisTagRes = 0.0;
    m_xaxisTagRes = 0.0;
    m_zaxisTagRes = 0.0;
}

void graphDC::SetRange(double minX, double maxX, double minY, double maxY)
{
    m_maxXAxis = maxX;
    m_maxYAxis = maxY;
    m_minXAxis = minX;
    m_minYAxis = minY;
    m_minZAxis = 0.0;
    m_maxZAxis = 0.0;

    m_yaxisRes = (double)(m_axisRect.GetHeight() - 120) / ((double)maxY + (double)abs(minY));
    m_xaxisRes = (double)(m_axisRect.GetWidth() - 120) / (double)(m_xaxisTagsNum - 1);
    m_zaxisRes = 0.0;

    m_yaxisTagRes = (double)(m_axisRect.GetHeight() - 120) / (double)(m_yaxisTagsNum);
    m_xaxisTagRes = m_xaxisRes;
    m_zaxisTagRes = 0.0;

    m_orgX = LEFT_RIGHT_MARGIN + m_axisRect.GetX();
    m_orgY = TOP_BOTTOM_MARGIN + m_axisRect.GetY();
}

void graphDC::DrawSelectedPointArea(double xAxis, double yAxis, double zAxis)
{

}

void graphDC::DrawCartesianAxis()
{
#ifndef __WINDOWS__
    m_dc->SetFont(wxFont(8, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false));
#else
    m_dc->SetFont(wxFont(7, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false));
#endif

    if(m_table->IsBidimensional())
    {
		m_dc->SetPen(wxPen(*wxBLACK, 2));

        m_dc->DrawLine(m_orgX, m_orgY - 10, m_orgX, m_axisRect.GetBottom() - (TOP_BOTTOM_MARGIN - 10));
		m_dc->DrawLine(m_orgX - 10,  m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN, m_axisRect.GetRight() - (LEFT_RIGHT_MARGIN - 10), m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN);

        m_dc->DrawText(m_table->m_tableInfo.m_xunit, m_axisRect.GetRight() - (LEFT_RIGHT_MARGIN - 10), m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN);
        m_dc->DrawText(m_table->m_tableInfo.m_yunit, m_orgX - 10, m_orgY - 20);
    }

    if(m_table->IsTridimensional())
    {
        // Draw X Y Z axis
        m_dc->SetPen(wxPen(*wxBLACK, 2));
        point3d orig(0.0, 0.0, 0.0);
        point3d xaxis((double)m_maxXAxis, 0.0, 0.0);
        point3d yaxis(0.0, (double)m_maxYAxis, 0.0);
        point3d zaxis(0.0, 0.0, (double)m_maxZAxis);
        DrawLine(orig, xaxis);
        DrawLine(orig, yaxis);
        DrawLine(orig, zaxis);

        DrawText(m_table->m_tableInfo.m_xunit, (double)m_maxXAxis + ((double) m_maxXAxis * 0.1), 0.0, 0.0);
        DrawText(m_table->m_tableInfo.m_yunit, 0.0, 0.0, (double)m_maxZAxis + ((double) m_maxZAxis * 0.10));
        DrawText(m_table->m_tableInfo.m_zunit, 0.0, (double) m_maxYAxis, 0.0);
    }
}

void graphDC::DrawTitle()
{
#ifndef __WINDOWS__
    m_dc->SetFont(wxFont(7, wxDEFAULT, wxNORMAL, wxNORMAL, false));
#else
    m_dc->SetFont(wxFont(6, wxDEFAULT, wxNORMAL, wxNORMAL, false));
#endif

    if(m_show == GDC_SHOW_CURRENT)
        m_dc->DrawText(_T("Current Map View"), 30, m_axisRect.GetBottom() - 30);
    if(m_show == GDC_SHOW_BASE_CURRENT)
        m_dc->DrawText(_T("Original Map plus Current Map View"), 30, m_axisRect.GetBottom() - 30);
    if(m_show == GDC_SHOW_DIFF)
        m_dc->DrawText(_T("Differences between Maps View"), 30, m_axisRect.GetBottom() - 30);
    if(m_show == GDC_SHOW_BASE)
        m_dc->DrawText(_T("Original Map View"), 30, m_axisRect.GetBottom() - 30);
}

void graphDC::DrawTags()
{
#ifndef __WINDOWS__
    m_dc->SetFont(wxFont(7, wxDEFAULT, wxNORMAL, wxNORMAL, false));
#else
    m_dc->SetFont(wxFont(6, wxDEFAULT, wxNORMAL, wxNORMAL, false));
#endif

    if(m_table->IsBidimensional())
    {
        if(!m_table->IsSingleValue())
        {
            for(int c = 0; c < m_xaxisTagsNum; c++)
            {
                short data = m_table->m_headerCol[c].current;
                    m_dc->DrawText(wxString::Format(_T("%d"), data), m_orgX + 5 + (double)c * m_xaxisTagRes, (m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN + 10));
            }
        }

        for(int r = 0; r <= m_yaxisTagsNum; r++)
        {
            int data = ((m_maxYAxis + abs(m_minYAxis)) / m_yaxisTagsNum) * r;
            if (m_minYAxis < 0)
                data = ((m_maxYAxis + abs(m_minYAxis)) / m_yaxisTagsNum) * r - abs(m_minYAxis);
                m_dc->DrawText(wxString::Format(_T("%d"), data), m_orgX - 30, (m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN - 10) - (double)r * m_yaxisTagRes);
        }
    }

    if(m_table->IsTridimensional())
    {
#ifndef __WINDOWS__
        m_dc->SetFont(wxFont(7, wxDEFAULT, wxNORMAL, wxNORMAL, false));
#else
        m_dc->SetFont(wxFont(6, wxDEFAULT, wxNORMAL, wxNORMAL, false));
#endif

        for(int c = 0; c < m_yaxisTagsNum; c++)
        {
            short data = (m_maxYAxis / m_yaxisTagsNum) * c;
                DrawText(wxString::Format(_T("%d"), data), (double)m_maxXAxis + ((double) m_maxXAxis * 0.05), data, 0.0);
        }

        for(int c = 0; c < m_xaxisTagsNum; c++)
        {
            short data = m_table->m_headerRow[c].current;
                DrawText(wxString::Format(_T("%d"), data), (double) c, 0.0, (double)m_maxZAxis + ((double) m_maxZAxis * 0.10));
        }

        for(int c = 0; c < m_zaxisTagsNum; c++)
        {
            short data = m_table->m_headerCol[c].current;
                DrawText(wxString::Format(_T("%d"), data), (double)m_maxXAxis + ((double) m_maxXAxis * 0.05), 0.0, (double)(m_maxZAxis - c));
        }

        // Draw X Y Z helper axis
        m_dc->SetPen(wxPen(*wxBLACK, 1));
        point3d xaxis((double)m_maxXAxis, 0.0, 0.0);
        point3d yaxis(0.0, (double)m_maxYAxis, 0.0);
        point3d zaxis(0.0, 0.0, (double)m_maxZAxis);
        point3d xzhelpaxis((double)m_maxXAxis, 0.0, (double)m_maxZAxis);
        point3d yzhelpaxis(0.0, (double)m_maxYAxis, (double)m_maxZAxis);
        point3d xyhelpaxis((double)m_maxXAxis, (double)m_maxYAxis, 0.0);
        DrawLine(xaxis, xzhelpaxis);
        DrawLine(zaxis, xzhelpaxis);
        DrawLine(yaxis, yzhelpaxis);
        DrawLine(zaxis, yzhelpaxis);
        DrawLine(xaxis, xyhelpaxis);
        DrawLine(yaxis, xyhelpaxis);
    }
}

void graphDC::DrawHelpingLines()
{
    if(m_table->IsBidimensional())
    {
        // plot Y axis helping lines
        m_dc->SetPen(wxPen(*wxLIGHT_GREY, 1, wxDOT));
        for(int c = 0; c < m_xaxisTagsNum; c++)
        {
            m_dc->DrawLine(m_orgX + (double)c * m_xaxisTagRes, m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN, m_orgX + (double)c * m_xaxisTagRes, m_orgY);
        }

        // plot X axis helping lines
        for(int r = 0; r <= m_yaxisTagsNum; r++)
        {
            m_dc->DrawLine(m_axisRect.GetRight() - LEFT_RIGHT_MARGIN , (m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN) - (double)r * m_yaxisTagRes,
                        m_orgX, (m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN) - (double)r * m_yaxisTagRes);
        }
    }

    if(m_table->IsTridimensional())
    {
        m_dc->SetPen(wxPen(*wxLIGHT_GREY, 1, wxDOT));

        // plot Y axis helping lines
        for(int c = 0; c < m_yaxisTagsNum; c++)
        {
            DrawLine(0.0, (double)c * ((double) m_maxYAxis / m_yaxisTagsNum), 0.0,
                0.0, (double)c * ((double) m_maxYAxis / m_yaxisTagsNum), (double)m_maxZAxis);

            DrawLine(0.0, (double)c * ((double) m_maxYAxis / m_yaxisTagsNum), 0.0,
                m_maxXAxis, (double)c * ((double) m_maxYAxis / m_yaxisTagsNum), 0.0);
        }

        // plot X axis helping lines
        for(int c = 0; c < m_xaxisTagsNum; c++)
        {
            DrawLine((double)c * ((double) m_maxXAxis / m_xaxisTagsNum), 0.0, 0.0,
                (double)c * ((double) m_maxXAxis / m_xaxisTagsNum), 0.0, (double)m_maxZAxis);

            DrawLine((double)c * ((double) m_maxXAxis / m_xaxisTagsNum), 0.0, 0.0,
                (double)c * ((double) m_maxXAxis / m_xaxisTagsNum), (double) m_maxYAxis, 0.0);
        }

        // plot Z axis helping lines
        for(int c = 0; c < m_zaxisTagsNum; c++)
        {
            DrawLine(0.0, 0.0, (double)c * ((double) m_maxZAxis / m_zaxisTagsNum),
                (double) m_maxXAxis, 0.0, (double)c * ((double) m_maxZAxis / m_zaxisTagsNum));

            DrawLine(0.0, 0.0, (double)c * ((double) m_maxZAxis / m_zaxisTagsNum),
                0.0, (double) m_maxYAxis, (double)c * ((double) m_maxXAxis / m_xaxisTagsNum));
        }
    }
}

void graphDC::DrawGraph()
{
    if(m_table->IsBidimensional())
    {
        if(!m_table->IsSingleValue())
        {
            for(int c = 0; c < m_table->GetCols() - 1; c++)
            {
                short diff_a = m_table->m_tableData[c][0].current - m_table->m_tableData[c][0].base;
                short diff_b = m_table->m_tableData[c + 1][0].current - m_table->m_tableData[c + 1][0].base;

                if((diff_a > 0) || (diff_b > 0))
                    m_dc->SetPen(wxPen(*wxRED, 1));
                else if((diff_a < 0) || (diff_b < 0))
                    m_dc->SetPen(wxPen(*wxBLUE, 1));
                else
                    m_dc->SetPen(wxPen(*wxBLACK, 1));

                short data_a, data_b; data_a = data_b = 0;

                if((m_show == GDC_SHOW_CURRENT) || (m_show == GDC_SHOW_BASE_CURRENT))
                {
                    data_a = m_table->m_tableData[c][0].current;
                    data_b = m_table->m_tableData[c + 1][0].current;
                }
                if (m_show == GDC_SHOW_DIFF)
                {
                    data_a = m_table->m_tableData[c][0].current - m_table->m_tableData[c][0].base;
                    data_b = m_table->m_tableData[c + 1][0].current - m_table->m_tableData[c + 1][0].base;
                }
                if (m_show == GDC_SHOW_BASE)
                {
                    data_a = m_table->m_tableData[c][0].base;
                    data_b = m_table->m_tableData[c + 1][0].base;
                }

                wxPoint ptStart = TransformTo2d(c, data_a);
                wxPoint ptEnd = TransformTo2d(c + 1, data_b);
                m_dc->DrawLine(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
            }

            if((m_show == GDC_SHOW_BASE) || (m_show == GDC_SHOW_BASE_CURRENT))
            {
                // plot initial line
                m_dc->SetPen(wxPen(*wxBLACK, 1));
                for(int c = 0; c < m_table->GetCols() - 1; c++)
                {
                    short data_a = m_table->m_tableData[c][0].base;
                    short data_b = m_table->m_tableData[c + 1][0].base;

                    wxPoint ptStart = TransformTo2d(c, data_a);
                    wxPoint ptEnd = TransformTo2d(c + 1, data_b);
                    m_dc->DrawLine(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
                }
            }
        }
        else
        {
            short diff_a = m_table->m_tableData[0][0].current - m_table->m_tableData[0][0].base;

            if(diff_a > 0)
                m_dc->SetPen(wxPen(*wxRED, 1));
            else if(diff_a < 0)
                m_dc->SetPen(wxPen(*wxBLUE, 1));
            else
                m_dc->SetPen(wxPen(*wxBLACK, 1));

            short data_a;

            if((m_show == GDC_SHOW_CURRENT) || (m_show == GDC_SHOW_BASE_CURRENT))
            {
                data_a = m_table->m_tableData[0][0].current;
            }
            if (m_show == GDC_SHOW_DIFF)
            {
                data_a = m_table->m_tableData[0][0].current - m_table->m_tableData[0][0].base;
            }
            if (m_show == GDC_SHOW_BASE)
            {
                data_a = m_table->m_tableData[0][0].base;
            }

            wxPoint ptStart = TransformTo2d(0, data_a);
            wxPoint ptEnd = TransformTo2d(1, data_a);
            m_dc->DrawLine(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);

            if((m_show == GDC_SHOW_BASE) || (m_show == GDC_SHOW_BASE_CURRENT))
            {
                // plot initial line
                m_dc->SetPen(wxPen(*wxBLACK, 1));
                short data_a = m_table->m_tableData[0][0].base;

                wxPoint ptStart = TransformTo2d(0, data_a);
                wxPoint ptEnd = TransformTo2d(1, data_a);
                m_dc->DrawLine(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
            }
        }
    }

    if(m_table->IsTridimensional())
    {
        short data[4]; data[0] = data[1] = data[2] = data[3] = 0;
        // plot differences line
        m_dc->SetBrush(wxBrush(*wxLIGHT_GREY_BRUSH));

        if((m_show == GDC_SHOW_BASE) || (m_show == GDC_SHOW_BASE_CURRENT))
        {
            // plot initial line
            m_dc->SetPen(wxPen(*wxBLACK, 1));
            for(int c = m_table->GetCols() - 2; c >= 0 ; c--)
                for(int r = m_table->GetRows() - 2; r >= 0 ; r--)
                {
                    data[0] = m_table->GetBaseValue(c, r);
                    data[1] = m_table->GetBaseValue(c, r + 1);
                    data[2] = m_table->GetBaseValue(c + 1, r);
                    data[3] = m_table->GetBaseValue(c + 1, r + 1);

                    point3d points[4] = {
                        point3d((double)r, (double)data[0], (double)(m_maxZAxis - c)),
                        point3d((double)(r + 1), (double)data[1], (double)(m_maxZAxis - c)),
                        point3d((double)(r + 1), (double)data[3], (double)(m_maxZAxis - (c + 1))),
                        point3d((double)r, (double)data[2], (double)(m_maxZAxis - (c + 1))) };

                    DrawPolygon(4, points);

                }
        }

        if((m_show == GDC_SHOW_CURRENT) || (m_show == GDC_SHOW_BASE_CURRENT) || (m_show == GDC_SHOW_DIFF))
        {
            for(int c = m_table->GetCols() - 2; c >= 0 ; c--)
                for(int r = m_table->GetRows() - 2; r >= 0 ; r--)
                {
                    if((m_show == GDC_SHOW_CURRENT) || (m_show == GDC_SHOW_BASE_CURRENT))
                    {
                        data[0] = m_table->GetCurrentValue(c, r);
                        data[1] = m_table->GetCurrentValue(c, r + 1);
                        data[2] = m_table->GetCurrentValue(c + 1, r);
                        data[3] = m_table->GetCurrentValue(c + 1, r + 1);
                    }
                    if(m_show == GDC_SHOW_DIFF)
                    {
                        data[0] = m_table->GetDiffValue(c, r);
                        data[1] = m_table->GetDiffValue(c, r + 1);
                        data[2] = m_table->GetDiffValue(c + 1, r);
                        data[3] = m_table->GetDiffValue(c + 1, r + 1);
                    }

                    if ((m_table->GetDiffValue(c, r + 1) > 0) || (m_table->GetDiffValue(c + 1, r) > 0))
                        m_dc->SetPen(wxPen(*wxRED, 1));
                    else if ((m_table->GetDiffValue(c, r + 1) < 0) || (m_table->GetDiffValue(c + 1, r) < 0))
                        m_dc->SetPen(wxPen(*wxBLUE, 1));
                    else
                        m_dc->SetPen(wxPen(*wxBLACK, 1));

                    point3d points[4] = {
                        point3d((double)r, (double)data[0], (double)(m_maxZAxis - c)),
                        point3d((double)(r + 1), (double)data[1], (double)(m_maxZAxis - c)),
                        point3d((double)(r + 1), (double)data[3], (double)(m_maxZAxis - (c + 1))),
                        point3d((double)r, (double)data[2], (double)(m_maxZAxis - (c + 1))) };

                    DrawPolygon(4, points);
                }
        }

        m_dc->SetBrush(wxBrush(wxColor(*wxBLACK), wxTRANSPARENT));
    }
}

void graphDC::DrawCursor()
{
    if(m_cursor->IsVisible())
    {
        if(m_table->IsBidimensional())
        {
            wxPoint pt = TransformTo2d(m_cursor->Get2dPosition());
            m_dc->SetPen(wxPen(*wxRED, 2));
            m_dc->DrawCircle(pt, m_cursor->GetSize().GetX() / 2);
        }
        if(m_table->IsTridimensional())
        {
            wxPoint pt = dc3d::TransformTo2d(m_cursor->Get3dPosition());
            m_dc->SetPen(wxPen(*wxRED, 2));
            m_dc->DrawCircle(pt, m_cursor->GetSize().GetX() / 2);
        }
    }
}

void graphDC::DrawSelection()
{
    if(m_table->IsBidimensional())
    {
        for(int c = min(m_selectionBegin.x, m_selectionEnd.x) ; c < max(m_selectionBegin.x, m_selectionEnd.x) ; c++)
        {
            short data_a, data_b; data_a = data_b = 0;

            if((m_show == GDC_SHOW_CURRENT) || (m_show == GDC_SHOW_BASE_CURRENT))
            {
                data_a = m_table->m_tableData[c][0].current;
                data_b = m_table->m_tableData[c + 1][0].current;
            }
            if (m_show == GDC_SHOW_DIFF)
            {
                data_a = m_table->m_tableData[c][0].current - m_table->m_tableData[c][0].base;
                data_b = m_table->m_tableData[c + 1][0].current - m_table->m_tableData[c + 1][0].base;
            }
            if (m_show == GDC_SHOW_BASE)
            {
                data_a = m_table->m_tableData[c][0].base;
                data_b = m_table->m_tableData[c + 1][0].base;
            }

            m_dc->SetBrush(wxBrush(*wxGREY_BRUSH));
            m_dc->SetPen(wxPen(*wxBLACK, 1));

            wxPoint points[4] = {
                wxPoint(TransformTo2d(c, data_a)),
                wxPoint(TransformTo2d(c + 1, data_b)),
                wxPoint(TransformTo2d(c + 1, 0)),
                wxPoint(TransformTo2d(c, 0)) };
            m_dc->DrawPolygon(4, points);
        }
    }

    if(m_table->IsTridimensional())
    {
        short data[4] = {0,0,0,0};

        for(int c = min(m_selectionBegin.x, m_selectionEnd.x) ; c < max(m_selectionBegin.x, m_selectionEnd.x) ; c++)
            for(int r = min(m_selectionBegin.y, m_selectionEnd.y); r < max(m_selectionBegin.y, m_selectionEnd.y) ; r++)
            {
                if((m_show == GDC_SHOW_CURRENT) || (m_show == GDC_SHOW_BASE_CURRENT))
                {
                    data[0] = m_table->GetCurrentValue(c, r);
                    data[1] = m_table->GetCurrentValue(c, r + 1);
                    data[2] = m_table->GetCurrentValue(c + 1, r);
                    data[3] = m_table->GetCurrentValue(c + 1, r + 1);
                }
                if(m_show == GDC_SHOW_DIFF)
                {
                    data[0] = m_table->GetDiffValue(c, r);
                    data[1] = m_table->GetDiffValue(c, r + 1);
                    data[2] = m_table->GetDiffValue(c + 1, r);
                    data[3] = m_table->GetDiffValue(c + 1, r + 1);
                }
                if(m_show == GDC_SHOW_BASE)
                {
                    data[0] = m_table->GetBaseValue(c, r);
                    data[1] = m_table->GetBaseValue(c, r + 1);
                    data[2] = m_table->GetBaseValue(c + 1, r);
                    data[3] = m_table->GetBaseValue(c + 1, r + 1);
                }

                m_dc->SetBrush(wxBrush(*wxGREY_BRUSH));
                m_dc->SetPen(wxPen(*wxBLACK, 1));

                point3d points[4] = {
                    point3d((double)r, (double)data[0], (double)(m_maxZAxis - c)),
                    point3d((double)(r + 1), (double)data[1], (double)(m_maxZAxis - c)),
                    point3d((double)(r + 1), (double)data[3], (double)(m_maxZAxis - (c + 1))),
                    point3d((double)r, (double)data[2], (double)(m_maxZAxis - (c + 1))) };

                DrawPolygon(4, points);
            }
    }
}

void graphDC::Draw()
{
    DrawCartesianAxis();
    DrawHelpingLines();
    DrawGraph();
    DrawTags();
    DrawSelection();
    DrawCursor();
    DrawTitle();
}

bool graphDC::PointIsNear(wxPoint mapPoint, wxPoint mousePoint, double precision)
{
    double distX, distY, precX, precY;

    distX = (double) abs(abs(mapPoint.x) - abs(mousePoint.x));
    distY = (double) abs(abs(mapPoint.y) - abs(mousePoint.y));
    precX = precision/* * m_resX */;
    precY = precision/* * m_resY */;

    return (distX < precX) && (distY < precY);
}

bool graphDC::TransformTo3dGrid(int x, int y, point3d& ptRet)
{
    bool found = false;

    for(int c = 0; c < m_table->GetCols(); c++)
        for(int r = 0; r < m_table->GetRows(); r++)
        {
            point3d tmpPoint = m_points[c][r];
            wxPoint pt2d = dc3d::TransformTo2d(tmpPoint);
            if(PointIsNear(pt2d, wxPoint(x,y), 5.0))
            {
                found = true;
                ptRet = tmpPoint;
                break;
            }
        }
    return found;
}

bool graphDC::TransformTo2dGrid(int x, int y, wxPoint& ptRet)
{
    bool found = false;

    for(int c = 0; c < m_table->GetCols(); c++)
    {
        point3d tmpPoint = m_points[c][0];
        wxPoint pt2d = TransformTo2d((int)tmpPoint.x, (int)tmpPoint.y);

        if(PointIsNear(pt2d, wxPoint(x, pt2d.y), 3.0))
        {
            found = true;
            ptRet = wxPoint((int)tmpPoint.x, (int)tmpPoint.y);
            break;
        }
    }

    return found;
}

wxPoint graphDC::TransformTo2d(int x, int y)
{
    return wxPoint( m_orgX + (double)x * m_xaxisRes,
                  ( m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN) - ((double)y * m_yaxisRes) - (abs(m_minYAxis) * m_yaxisRes));
}

void graphDC::OnMouseMove(int x, int y)
{
    td5mapeditorChildFrame *frame = (td5mapeditorChildFrame *) GetMainFrame()->GetActiveChild();

    if(m_table->IsBidimensional())
    {
        wxPoint newPoint;

        if(TransformTo2dGrid(x, y, newPoint))
        {
            MoveCursor(newPoint.x, 0);

            if(m_selecting)
            {
                if(m_selectionEnd != wxPoint(newPoint.x, 0))
                {
                    m_selectionEnd = wxPoint(newPoint.x, 0);

                    ewxRange range(m_selectionBegin.y, m_selectionBegin.x, m_selectionEnd.y, m_selectionEnd.x);
                    frame->SetSelectionRange(range, m_parent->GetId());
                }
            }
        }
    }

    if(m_table->IsTridimensional())
    {
        point3d newPoint(0,0,0);

        if(TransformTo3dGrid(x, y, newPoint))
        {
            MoveCursor(m_maxZAxis - newPoint.z, newPoint.x);

            if(IsSelecting() &&
              ((m_selectionEnd.x != m_maxZAxis - newPoint.z) ||
               (m_selectionEnd.y != newPoint.x)))
            {
                m_selectionEnd = wxPoint(m_maxZAxis - newPoint.z, newPoint.x);

                ewxRange range(m_selectionBegin.y, m_selectionBegin.x, m_selectionEnd.y, m_selectionEnd.x);
                frame->SetSelectionRange(range, m_parent->GetId());
            }
        }
    }
}


void graphDC::OnMouseSelectBegin(int x, int y)
{
    td5mapeditorChildFrame *frame = (td5mapeditorChildFrame *) GetMainFrame()->GetActiveChild();

    if(m_table->IsBidimensional())
    {
        wxPoint newPoint = m_cursor->Get2dPosition();

        if(!IsSelecting())
        {
            frame->SetSelection(newPoint.x, 0, m_parent->GetId());
            m_selectionBegin = wxPoint(newPoint.x, 0);
        }
    }

    if(m_table->IsTridimensional())
    {
        point3d newPoint = m_cursor->Get3dPosition();

        if(!IsSelecting())
        {
            frame->SetSelection(m_maxZAxis - newPoint.z, newPoint.x, m_parent->GetId());
            m_selectionBegin = wxPoint(m_maxZAxis - newPoint.z, newPoint.x);
        }
    }

    m_selecting = true;
}

void graphDC::OnMouseSelectEnd(int x, int y)
{
    td5mapeditorChildFrame *frame = (td5mapeditorChildFrame *) GetMainFrame()->GetActiveChild();

    if(m_table->IsBidimensional())
    {
        wxPoint newPoint = m_cursor->Get2dPosition();
        m_selectionEnd = wxPoint(newPoint.x, 0);
    }

    if(m_table->IsTridimensional())
    {
        point3d newPoint = m_cursor->Get3dPosition();
        m_selectionEnd = wxPoint(m_maxZAxis - newPoint.z, newPoint.x);
    }

    ewxRange range(m_selectionBegin.y, m_selectionBegin.x, m_selectionEnd.y, m_selectionEnd.x);
    frame->SetSelectionRange(range, m_parent->GetId());

    m_selecting = false;
}


void graphDC::MoveCursor(int col, int row)
{
    if(m_table->IsBidimensional())
    {
        int data = 0;

        if((m_show == GDC_SHOW_CURRENT) || (m_show == GDC_SHOW_BASE_CURRENT))
        {
            data = m_table->m_tableData[col][0].current;
        }
        if(m_show == GDC_SHOW_DIFF)
        {
            data = m_table->m_tableData[col][0].current - m_table->m_tableData[col][row].base;
        }
        if(m_show == GDC_SHOW_BASE)
        {
            data = m_table->m_tableData[col][0].base;
        }

        MoveCursor(col, data, 0);
    }

    if(m_table->IsTridimensional())
    {
        int data = 0;

        if((m_show == GDC_SHOW_CURRENT) || (m_show == GDC_SHOW_BASE_CURRENT))
        {
            data = m_table->m_tableData[col][row].current;
        }
        if(m_show == GDC_SHOW_DIFF)
        {
            data = m_table->m_tableData[col][row].current - m_table->m_tableData[col][row].base;
        }
        if(m_show == GDC_SHOW_BASE)
        {
            data = m_table->m_tableData[col][row].base;
        }

        MoveCursor(row, data, col);
    }
}

void graphDC::SelectRange(ewxRange range)
{
    m_selectionBegin.y = range.topRow;
    m_selectionBegin.x = range.leftCol;
    m_selectionEnd.y = range.bottomRow;
    m_selectionEnd.x = range.rightCol;

    if(m_table->IsBidimensional())
    {
    }

    if(m_table->IsBidimensional())
    {
    }

/*
    wxPoint pt2d = TransformTo2d(oldPoint.x, oldPoint.y);
    m_parent->CalcScrolledPosition(pt2d.x, pt2d.y, &logicX, &logicY);
    m_parent->RefreshRect(wxRect(wxPoint(logicX - m_cursor->GetSize().GetWidth() / 2,
                                       logicY - m_cursor->GetSize().GetHeight() / 2), m_cursor->GetSize()));
*/

    m_parent->Refresh();
}

void graphDC::MoveCursor(int x, int y, int z)
{
    //wxScrolledWindow *parent = (wxScrolledWindow *) GetParent();

    if(m_table->IsBidimensional())
    {
        int logicX, logicY;
        point3d newPoint(x, y, 0);
        point3d oldPoint = m_cursor->Move(newPoint);

        wxPoint pt2d = TransformTo2d(oldPoint.x, oldPoint.y);
        m_parent->CalcScrolledPosition(pt2d.x, pt2d.y, &logicX, &logicY);
        m_parent->RefreshRect(wxRect(wxPoint(logicX - m_cursor->GetSize().GetWidth() / 2,
                                           logicY - m_cursor->GetSize().GetHeight() / 2), m_cursor->GetSize()));

        pt2d = TransformTo2d(newPoint.x, newPoint.x);
        m_parent->CalcScrolledPosition(pt2d.x, pt2d.y, &logicX, &logicY);

        m_parent->RefreshRect(wxRect(wxPoint(logicX - m_cursor->GetSize().GetWidth() / 2,
                                           logicY - m_cursor->GetSize().GetHeight() / 2), m_cursor->GetSize()));
    }
    if(m_table->IsTridimensional())
    {
        int logicX, logicY;
        point3d newPoint(x, y, m_maxZAxis - z);
        point3d oldPoint = m_cursor->Move(newPoint);

        wxPoint pt2d = dc3d::TransformTo2d(oldPoint);
        m_parent->CalcScrolledPosition(pt2d.x, pt2d.y, &logicX, &logicY);
        m_parent->RefreshRect(wxRect(wxPoint(logicX - m_cursor->GetSize().GetWidth() / 2,
                                           logicY - m_cursor->GetSize().GetHeight() / 2), m_cursor->GetSize()));

        pt2d = dc3d::TransformTo2d(newPoint);
        m_parent->CalcScrolledPosition(pt2d.x, pt2d.y, &logicX, &logicY);

        m_parent->RefreshRect(wxRect(wxPoint(logicX - m_cursor->GetSize().GetWidth() / 2,
                                           logicY - m_cursor->GetSize().GetHeight() / 2), m_cursor->GetSize()));
    }
}
