/***************************************************************
 * Name:      td5mapGraph.cpp
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2012-04-25
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

#include "td5mapGraph.h"
#include "td5mapeditorMainFrame.h"
#include "td5mapeditorChildFrame.h"

#define LEFT_RIGHT_MARGIN   60
#define TOP_BOTTOM_MARGIN   60

const double PI = 4*atan(1)  ;


//! td5mapGraph
td5mapGraph::td5mapGraph(wxScrolledWindow *parent)
{
    m_parent = parent;
    m_prepared = false;
    m_cursor = new td5mapGraphCursor;
    m_cursor->SetVisible(true);
    m_selecting = false;
    m_selectionBegin = wxPoint(0, 0);
    m_selectionEnd = wxPoint(0, 0);

    m_theta0 = -25 ;
	m_theta1 = 25 ;
	m_resXY = 0.0;
	m_resXZ = 0.0;
	m_resZY = 0.0;
	m_resX = 0.0;
	m_resY = 0.0;
}

td5mapGraph::td5mapGraph(wxDC& dc, wxScrolledWindow *parent, wxRect canvasrect, td5mapTable *maptable, int graphshow)
{
    m_parent = parent;

    m_cursor = new td5mapGraphCursor;
    m_cursor->SetVisible(true);
    m_selecting = false;
    m_selectionBegin = wxPoint(0, 0);
    m_selectionEnd = wxPoint(0, 0);

    m_theta0 = -25 ;
	m_theta1 = 25 ;
	m_resXY = 0.0;
	m_resXZ = 0.0;
	m_resZY = 0.0;
	m_resX = 0.0;
	m_resY = 0.0;

    Prepare(dc, canvasrect, maptable, graphshow);
}

td5mapGraph::~td5mapGraph()
{
    if(m_cursor)
        delete m_cursor;
}

bool td5mapGraph::Prepare(wxDC& dc, wxRect canvasrect, td5mapTable *maptable, int graphshow)
{
    if ((maptable->GetCols() == 0) || (maptable->GetRows() == 0))
    {
        m_prepared = false;
        return false;
    }

    m_dc = &dc;

    m_table = maptable;
    canvasrect.Deflate(25,25);
    m_axisRect = canvasrect;
    m_show = graphshow;

    int minY, maxY;
/*
    if(m_show == GDC_SHOW_DIFF)
        m_table->EvalDiffRange(minY, maxY);
    else
        m_table->EvalRange(minY, maxY);
*/
    m_table->EvalRange(minY, maxY);

    if(m_table->IsBidimensional())
    {
        if(!m_table->IsSingleValue())
        {
            m_yaxisTagsNum = 10;
            m_xaxisTagsNum = m_table->GetCols();
            m_zaxisTagsNum = 0;

            SetRange2D(0, m_table->GetCols(), minY, maxY);
        }
        else
        {
            m_yaxisTagsNum = 10;
            m_xaxisTagsNum = 2;
            m_zaxisTagsNum = 0;

            SetRange2D(0, 2, minY, maxY);
        }
    }
    if(m_table->IsTridimensional())
    {
        m_xaxisTagsNum = m_table->GetRows() - 1;
        m_yaxisTagsNum = 10;
        m_zaxisTagsNum = m_table->GetCols() - 1;

        SetRange3D(-(m_table->GetRows() - 1), (m_table->GetRows() - 1), minY, maxY, -(m_table->GetCols() - 1), (m_table->GetCols() - 1));
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

            m_points[c][0] = point3D((double)c, (double)data, 0.0);
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

                m_points[c][r] = point3D((double)r, (double)data, (double)(m_maxZAxis - c));
            }
    }

    m_prepared = true;
    return m_prepared;
}

void td5mapGraph::SetRange2D(double minX, double maxX, double minY, double maxY)
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

void td5mapGraph::DrawSelectedPointArea(double xAxis, double yAxis, double zAxis)
{

}

void td5mapGraph::DrawCartesianAxis()
{
#ifndef __WINDOWS__
    m_dc->SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
#else
    m_dc->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
#endif

    if(m_table->IsBidimensional())
    {
		m_dc->SetPen(wxPen(*wxBLACK, 2));

        DrawLine2D(m_orgX, m_orgY - 10, m_orgX, m_axisRect.GetBottom() - (TOP_BOTTOM_MARGIN - 10));
		DrawLine2D(m_orgX - 10,  m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN, m_axisRect.GetRight() - (LEFT_RIGHT_MARGIN - 10), m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN);

        DrawText2D(m_table->m_xunit, m_axisRect.GetRight() - (LEFT_RIGHT_MARGIN - 10), m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN);
        DrawText2D(m_table->m_yunit, m_orgX - 10, m_orgY - 30);
    }

    if(m_table->IsTridimensional())
    {
        // Draw X Y Z axis
        m_dc->SetPen(wxPen(*wxBLACK, 2));
        point3D orig(0.0, 0.0, 0.0);
        point3D xaxis((double)m_maxXAxis, 0.0, 0.0);
        point3D yaxis(0.0, (double)m_maxYAxis, 0.0);
        point3D zaxis(0.0, 0.0, (double)m_maxZAxis);
        DrawLine3D(orig, xaxis);
        DrawLine3D(orig, yaxis);
        DrawLine3D(orig, zaxis);

        wxSize xunitsize = m_dc->GetTextExtent( m_table->m_xunit );
        DrawText3D(m_table->m_xunit, (double)m_maxXAxis, 0.0, 0.0, 5.0, -xunitsize.GetHeight());     // right
        wxSize yunitsize = m_dc->GetTextExtent( m_table->m_yunit );
        DrawText3D(m_table->m_yunit, 0.0, 0.0, (double)m_maxZAxis, -(yunitsize.GetWidth() + 5), -yunitsize.GetHeight());   // left
        wxSize zunitsize = m_dc->GetTextExtent( m_table->m_zunit );
        DrawText3D(m_table->m_zunit, 0.0, (double) m_maxYAxis, 0.0, 0.0, -zunitsize.GetHeight());    // top
    }
}

void td5mapGraph::DrawTitle()
{
#ifndef __WINDOWS__
    m_dc->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
#else
    m_dc->SetFont(wxFont(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
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

void td5mapGraph::DrawTags()
{
#ifndef __WINDOWS__
    m_dc->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
#else
    m_dc->SetFont(wxFont(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
#endif

    if(m_table->IsBidimensional())
    {
        if(!m_table->IsSingleValue())
        {
            for(int c = 0; c < m_xaxisTagsNum; c++)
            {
                short data = m_table->m_headerCol[c].current;
                if (m_table->m_collabelsized == false)
                    DrawText2D(wxString::Format(_T("%d"), data), m_orgX + 5 + (double)c * m_xaxisTagRes, (m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN + 10));
                else
                    DrawText2D(wxString::Format(_T("%.1f"), m_table->ApplyColLabelSizer(data)), m_orgX + 5 + (double)c * m_xaxisTagRes, (m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN + 10));
            }
        }

        for(int r = 0; r <= m_yaxisTagsNum; r++)
        {
            int data = ((m_maxYAxis + abs(m_minYAxis)) / m_yaxisTagsNum) * r;
            if (m_minYAxis < 0)
                data = ((m_maxYAxis + abs(m_minYAxis)) / m_yaxisTagsNum) * r - abs(m_minYAxis);
            if (m_table->m_datasized == false)
                DrawText2D(wxString::Format(_T("%d"), data), m_orgX - 30, (m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN - 10) - (double)r * m_yaxisTagRes);
            else
                DrawText2D(wxString::Format(_T("%.1f"), m_table->ApplyDataSizer(data)), m_orgX - 30, (m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN - 10) - (double)r * m_yaxisTagRes);
        }
    }

    if(m_table->IsTridimensional())
    {
        #ifndef __WINDOWS__
        m_dc->SetFont(wxFont(7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
        #else
        m_dc->SetFont(wxFont(6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
        #endif

        for(int c = 1; c <= m_yaxisTagsNum; c++)
        {
            short data = (m_maxYAxis / m_yaxisTagsNum) * c;
            if (m_table->m_datasized == false)
                DrawText3D(wxString::Format(_T("%d"), data), (double)m_maxXAxis + ((double) m_maxXAxis * 0.05), data, 0.0);
            else
                DrawText3D(wxString::Format(_T("%.1f"), m_table->ApplyDataSizer(data)), (double)m_maxXAxis + ((double) m_maxXAxis * 0.05), data, 0.0);
        }

        for(int c = 0; c <= m_xaxisTagsNum; c++)
        {
            short data = m_table->m_headerRow[c].current;
            if (m_table->m_rowlabelsized == false)
                DrawText3D(wxString::Format(_T("%d"), data), (double) c, 0.0, (double)m_maxZAxis + ((double) m_maxZAxis * 0.10));
            else
                DrawText3D(wxString::Format(_T("%.1f"), m_table->ApplyRowLabelSizer(data)), (double) c, 0.0, (double)m_maxZAxis + ((double) m_maxZAxis * 0.10));
        }

        for(int c = 0; c <= m_zaxisTagsNum; c++)
        {
            short data = m_table->m_headerCol[c].current;
            if (m_table->m_collabelsized == false)
                DrawText3D(wxString::Format(_T("%d"), data), (double)m_maxXAxis + ((double) m_maxXAxis * 0.05), 0.0, (double)(m_maxZAxis - c));
            else
                DrawText3D(wxString::Format(_T("%.1f"), m_table->ApplyColLabelSizer(data)), (double)m_maxXAxis + ((double) m_maxXAxis * 0.05), 0.0, (double)(m_maxZAxis - c));
        }

        // Draw X Y Z helper axis
        m_dc->SetPen(wxPen(*wxBLACK, 1));
        point3D xaxis((double)m_maxXAxis, 0.0, 0.0);
        point3D yaxis(0.0, (double)m_maxYAxis, 0.0);
        point3D zaxis(0.0, 0.0, (double)m_maxZAxis);
        point3D xzhelpaxis((double)m_maxXAxis, 0.0, (double)m_maxZAxis);
        point3D yzhelpaxis(0.0, (double)m_maxYAxis, (double)m_maxZAxis);
        point3D xyhelpaxis((double)m_maxXAxis, (double)m_maxYAxis, 0.0);
        DrawLine3D(xaxis, xzhelpaxis);
        DrawLine3D(zaxis, xzhelpaxis);
        DrawLine3D(yaxis, yzhelpaxis);
        DrawLine3D(zaxis, yzhelpaxis);
        DrawLine3D(xaxis, xyhelpaxis);
        DrawLine3D(yaxis, xyhelpaxis);
    }
}

void td5mapGraph::DrawHelpingLines()
{
    if(m_table->IsBidimensional())
    {
        // plot Y axis helping lines
        m_dc->SetPen(wxPen(*wxLIGHT_GREY, 1, wxPENSTYLE_DOT));
        for(int c = 0; c < m_xaxisTagsNum; c++)
        {
            DrawLine2D(m_orgX + (double)c * m_xaxisTagRes, m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN, m_orgX + (double)c * m_xaxisTagRes, m_orgY);
        }

        // plot X axis helping lines
        for(int r = 0; r <= m_yaxisTagsNum; r++)
        {
            DrawLine2D(m_axisRect.GetRight() - LEFT_RIGHT_MARGIN , (m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN) - (double)r * m_yaxisTagRes,
                        m_orgX, (m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN) - (double)r * m_yaxisTagRes);
        }
    }

    if(m_table->IsTridimensional())
    {
        m_dc->SetPen(wxPen(*wxLIGHT_GREY, 1, wxPENSTYLE_DOT));

        // plot Y axis helping lines
        for(int c = 0; c < m_yaxisTagsNum; c++)
        {
            DrawLine3D(0.0, (double)c * ((double) m_maxYAxis / m_yaxisTagsNum), 0.0,
                0.0, (double)c * ((double) m_maxYAxis / m_yaxisTagsNum), (double)m_maxZAxis);

            DrawLine3D(0.0, (double)c * ((double) m_maxYAxis / m_yaxisTagsNum), 0.0,
                m_maxXAxis, (double)c * ((double) m_maxYAxis / m_yaxisTagsNum), 0.0);
        }

        // plot X axis helping lines
        for(int c = 0; c < m_xaxisTagsNum; c++)
        {
            DrawLine3D((double)c * ((double) m_maxXAxis / m_xaxisTagsNum), 0.0, 0.0,
                (double)c * ((double) m_maxXAxis / m_xaxisTagsNum), 0.0, (double)m_maxZAxis);

            DrawLine3D((double)c * ((double) m_maxXAxis / m_xaxisTagsNum), 0.0, 0.0,
                (double)c * ((double) m_maxXAxis / m_xaxisTagsNum), (double) m_maxYAxis, 0.0);
        }

        // plot Z axis helping lines
        for(int c = 0; c < m_zaxisTagsNum; c++)
        {
            DrawLine3D(0.0, 0.0, (double)c * ((double) m_maxZAxis / m_zaxisTagsNum),
                (double) m_maxXAxis, 0.0, (double)c * ((double) m_maxZAxis / m_zaxisTagsNum));

            DrawLine3D(0.0, 0.0, (double)c * ((double) m_maxZAxis / m_zaxisTagsNum),
                0.0, (double) m_maxYAxis, (double)c * ((double) m_maxXAxis / m_xaxisTagsNum));
        }
    }
}

void td5mapGraph::DrawGraph()
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

                wxPoint ptStart = TransformTo2D(c, data_a);
                wxPoint ptEnd = TransformTo2D(c + 1, data_b);
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

                    wxPoint ptStart = TransformTo2D(c, data_a);
                    wxPoint ptEnd = TransformTo2D(c + 1, data_b);
                    DrawLine2D(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
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

            wxPoint ptStart = TransformTo2D(0, data_a);
            wxPoint ptEnd = TransformTo2D(1, data_a);
            DrawLine2D(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);

            if((m_show == GDC_SHOW_BASE) || (m_show == GDC_SHOW_BASE_CURRENT))
            {
                // plot initial line
                m_dc->SetPen(wxPen(*wxBLACK, 1));
                short data_a = m_table->m_tableData[0][0].base;

                wxPoint ptStart = TransformTo2D(0, data_a);
                wxPoint ptEnd = TransformTo2D(1, data_a);
                DrawLine2D(ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
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

                    point3D points[4] = {
                        point3D((double)r, (double)data[0], (double)(m_maxZAxis - c)),
                        point3D((double)(r + 1), (double)data[1], (double)(m_maxZAxis - c)),
                        point3D((double)(r + 1), (double)data[3], (double)(m_maxZAxis - (c + 1))),
                        point3D((double)r, (double)data[2], (double)(m_maxZAxis - (c + 1))) };

                    DrawPolygon3D(4, points);

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

                    point3D points[4] = {
                        point3D((double)r, (double)data[0], (double)(m_maxZAxis - c)),
                        point3D((double)(r + 1), (double)data[1], (double)(m_maxZAxis - c)),
                        point3D((double)(r + 1), (double)data[3], (double)(m_maxZAxis - (c + 1))),
                        point3D((double)r, (double)data[2], (double)(m_maxZAxis - (c + 1))) };

                    DrawPolygon3D(4, points);
                }
        }

        m_dc->SetBrush(wxBrush(wxColor(*wxBLACK), wxBRUSHSTYLE_TRANSPARENT));
    }
}

void td5mapGraph::DrawCursor()
{
    if(m_cursor->IsVisible())
    {
        if(m_table->IsBidimensional())
        {
            wxPoint pt = TransformTo2D(m_cursor->Get2DPosition());
            m_dc->SetPen(wxPen(*wxRED, 2));
            m_dc->DrawCircle(pt, m_cursor->GetSize().GetX() / 2);
        }
        if(m_table->IsTridimensional())
        {
            wxPoint pt = TransformTo2D(m_cursor->Get3DPosition());
            m_dc->SetPen(wxPen(*wxRED, 2));
            m_dc->DrawCircle(pt, m_cursor->GetSize().GetX() / 2);
        }
    }
}

void td5mapGraph::DrawSelection()
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
                wxPoint(TransformTo2D(c, data_a)),
                wxPoint(TransformTo2D(c + 1, data_b)),
                wxPoint(TransformTo2D(c + 1, 0)),
                wxPoint(TransformTo2D(c, 0)) };
            DrawPolygon2D(4, points);
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

                point3D points[4] = {
                    point3D((double)r, (double)data[0], (double)(m_maxZAxis - c)),
                    point3D((double)(r + 1), (double)data[1], (double)(m_maxZAxis - c)),
                    point3D((double)(r + 1), (double)data[3], (double)(m_maxZAxis - (c + 1))),
                    point3D((double)r, (double)data[2], (double)(m_maxZAxis - (c + 1))) };

                DrawPolygon3D(4, points);
            }
    }
}

void td5mapGraph::Draw()
{
    DrawCartesianAxis();
    DrawHelpingLines();
    DrawGraph();
    DrawTags();
    DrawSelection();
    DrawCursor();
    DrawTitle();
}

bool td5mapGraph::PointIsNear(wxPoint mapPoint, wxPoint mousePoint, double precision)
{
    double distX, distY, precX, precY;

    distX = (double) abs(abs(mapPoint.x) - abs(mousePoint.x));
    distY = (double) abs(abs(mapPoint.y) - abs(mousePoint.y));
    precX = precision/* * m_resX */;
    precY = precision/* * m_resY */;

    return (distX < precX) && (distY < precY);
}

bool td5mapGraph::TransformTo3DGrid(int x, int y, point3D& ptRet)
{
    bool found = false;

    for(int c = 0; c < m_table->GetCols(); c++)
        for(int r = 0; r < m_table->GetRows(); r++)
        {
            point3D tmpPoint = m_points[c][r];
            wxPoint pt2D = TransformTo2D(tmpPoint);
            if(PointIsNear(pt2D, wxPoint(x,y), 5.0))
            {
                found = true;
                ptRet = tmpPoint;
                break;
            }
        }
    return found;
}

bool td5mapGraph::TransformTo2DGrid(int x, int y, wxPoint& ptRet)
{
    bool found = false;

    for(int c = 0; c < m_table->GetCols(); c++)
    {
        point3D tmpPoint = m_points[c][0];
        wxPoint pt2D = TransformTo2D((int)tmpPoint.x, (int)tmpPoint.y);

        if(PointIsNear(pt2D, wxPoint(x, pt2D.y), 3.0))
        {
            found = true;
            ptRet = wxPoint((int)tmpPoint.x, (int)tmpPoint.y);
            break;
        }
    }

    return found;
}

void td5mapGraph::SetRange3D(double minX, double maxX, double minY, double maxY, double minZ, double maxZ, double orgX, double orgY)
{
	double dRy , dRx ;

	dRy = maxY - minY ;
	dRx = maxX - minX ;

	m_rangeY[0] = minY ;
	m_rangeY[1] = maxY ;

	m_orgY = orgY;
	m_orgX = orgX;

	m_resXZ = 1.0 * fabs(((maxX - minX )*cos(m_theta0 * PI/180)) /
		((maxZ - minZ)*cos(m_theta1 * PI / 180))) ;

	m_rangeX[0] = minX;
	m_rangeX[1] = maxX;

	double dpixelx, dpixely ;

	dpixelx = (double)m_axisRect.width;
	dpixely = (double)m_axisRect.height ;
	m_resY = (m_rangeY[1] - m_rangeY[0]) / dpixely ;
	m_resX = (m_rangeX[1] - m_rangeX[0]) / dpixelx ;

	m_r3dX[0] = minX ;
	m_r3dX[1] = maxX ;

	m_r3dY[0] = minY ;
	m_r3dY[1] = maxY ;

	m_r3dZ[0] = minZ ;
	m_r3dZ[1] = maxZ ;

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


wxPoint td5mapGraph::TransformTo2D(int x, int y)
{
    return wxPoint( m_orgX + (double)x * m_xaxisRes,
                  ( m_axisRect.GetBottom() - TOP_BOTTOM_MARGIN) - ((double)y * m_yaxisRes) - (abs(m_minYAxis) * m_yaxisRes));
}

wxPoint td5mapGraph::TransformTo2D(point3D pt3d)
{
	return TransformTo2D((double) pt3d.x, (double) pt3d.y, (double) pt3d.z);
}


wxPoint td5mapGraph::TransformTo2D(double x, double y, double z)
{
	double dx, dy;

	// Map to 2d
	dx = (-z * m_resXZ  * cos(m_theta1 * PI / 180.0)) +
				( x * cos(m_theta0 * PI / 180))   ;

	dy = (m_resY/m_resX)* ((  m_resXZ * -z * sin(m_theta1 * PI / 180))
				+ (x *sin(m_theta0 * PI / 180)))  + y;

	// Add origin offset
	dx += m_orgX ;
	dy += m_orgY  ;

	double rx , ry ;
	int xPixel , yPixel ;
	wxPoint ptRet ;

	rx = dx - m_rangeX[0] ; // Calculate horizontal offset from origin
	ry = dy - m_rangeY[0] ; // Calculate vertical offset from origin .

	// Convert offset to be number of pixel on screen .
	xPixel = (int)(rx / m_resX) ;
	yPixel = (int)(ry / m_resY) ;

	//Calulate point to be drawn .
	ptRet.x= xPixel + m_axisRect.GetLeft() ;
	ptRet.y= m_axisRect.GetBottom() - yPixel;

	return ptRet ;
}

void td5mapGraph::OnMouseMove(int x, int y)
{
    td5mapeditorChildFrame *frame = (td5mapeditorChildFrame *) GetMainFrame()->GetActiveChild();

    if(m_table->IsBidimensional())
    {
        wxPoint newPoint;

        if(TransformTo2DGrid(x, y, newPoint))
        {
            MoveCursor2D(newPoint.x, 0);

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
        point3D newPoint(0,0,0);

        if(TransformTo3DGrid(x, y, newPoint))
        {
            MoveCursor2D(m_maxZAxis - newPoint.z, newPoint.x);

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


void td5mapGraph::OnMouseSelectBegin(int x, int y)
{
    td5mapeditorChildFrame *frame = (td5mapeditorChildFrame *) GetMainFrame()->GetActiveChild();

    if(m_table->IsBidimensional())
    {
        wxPoint newPoint = m_cursor->Get2DPosition();

        if(!IsSelecting())
        {
            frame->SetSelection(newPoint.x, 0, m_parent->GetId());
            m_selectionBegin = wxPoint(newPoint.x, 0);
        }
    }

    if(m_table->IsTridimensional())
    {
        point3D newPoint = m_cursor->Get3DPosition();

        if(!IsSelecting())
        {
            frame->SetSelection(m_maxZAxis - newPoint.z, newPoint.x, m_parent->GetId());
            m_selectionBegin = wxPoint(m_maxZAxis - newPoint.z, newPoint.x);
        }
    }

    m_selecting = true;
}

void td5mapGraph::OnMouseSelectEnd(int x, int y)
{
    td5mapeditorChildFrame *frame = (td5mapeditorChildFrame *) GetMainFrame()->GetActiveChild();

    if(m_table->IsBidimensional())
    {
        wxPoint newPoint = m_cursor->Get2DPosition();
        m_selectionEnd = wxPoint(newPoint.x, 0);
    }

    if(m_table->IsTridimensional())
    {
        point3D newPoint = m_cursor->Get3DPosition();
        m_selectionEnd = wxPoint(m_maxZAxis - newPoint.z, newPoint.x);
    }

    ewxRange range(m_selectionBegin.y, m_selectionBegin.x, m_selectionEnd.y, m_selectionEnd.x);
    frame->SetSelectionRange(range, m_parent->GetId());

    m_selecting = false;
}

void td5mapGraph::MoveCursor2D(int col, int row)
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

        MoveCursor3D(col, data, 0);
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

        MoveCursor3D(row, data, col);
    }
}

void td5mapGraph::MoveCursor3D(int x, int y, int z)
{
    //wxScrolledWindow *parent = (wxScrolledWindow *) GetParent();

    if(m_table->IsBidimensional())
    {
        int logicX, logicY;
        point3D newPoint(x, y, 0);
        point3D oldPoint = m_cursor->Move(newPoint);

        wxPoint pt2D = TransformTo2D(oldPoint.x, oldPoint.y);
        m_parent->CalcScrolledPosition(pt2D.x, pt2D.y, &logicX, &logicY);
        m_parent->RefreshRect(wxRect(wxPoint(logicX - m_cursor->GetSize().GetWidth() / 2,
                                           logicY - m_cursor->GetSize().GetHeight() / 2), m_cursor->GetSize()));

        pt2D = TransformTo2D(newPoint.x, newPoint.x);
        m_parent->CalcScrolledPosition(pt2D.x, pt2D.y, &logicX, &logicY);

        m_parent->RefreshRect(wxRect(wxPoint(logicX - m_cursor->GetSize().GetWidth() / 2,
                                           logicY - m_cursor->GetSize().GetHeight() / 2), m_cursor->GetSize()));
    }
    if(m_table->IsTridimensional())
    {
        int logicX, logicY;
        point3D newPoint(x, y, m_maxZAxis - z);
        point3D oldPoint = m_cursor->Move(newPoint);

        wxPoint pt2D = TransformTo2D(oldPoint);
        m_parent->CalcScrolledPosition(pt2D.x, pt2D.y, &logicX, &logicY);
        m_parent->RefreshRect(wxRect(wxPoint(logicX - m_cursor->GetSize().GetWidth() / 2,
                                           logicY - m_cursor->GetSize().GetHeight() / 2), m_cursor->GetSize()));

        pt2D = TransformTo2D(newPoint);
        m_parent->CalcScrolledPosition(pt2D.x, pt2D.y, &logicX, &logicY);

        m_parent->RefreshRect(wxRect(wxPoint(logicX - m_cursor->GetSize().GetWidth() / 2,
                                           logicY - m_cursor->GetSize().GetHeight() / 2), m_cursor->GetSize()));
    }
}

void td5mapGraph::SelectRange(ewxRange range)
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

    m_parent->Refresh();
}

void td5mapGraph::DrawLine2D(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
{
    m_dc->DrawLine(x1, y1, x2, y2);
}

point3D td5mapGraph::DrawLine3D(double xbegin, double ybegin, double zbegin, double xend, double yend, double zend)
{
    point3D pt3d(xend, yend, zend);
	wxPoint ptbegin, ptend;

	ptbegin = TransformTo2D(xbegin, ybegin, zbegin);
	ptend = TransformTo2D(xend, yend, zend);

	m_dc->DrawLine(ptbegin.x, ptbegin.y, ptend.x, ptend.y);

	return pt3d;
}

point3D td5mapGraph::DrawLine3D(point3D pt3dBegin, point3D pt3dEnd)
{
    point3D pt3d;

    pt3d = DrawLine3D(pt3dBegin.x, pt3dBegin.y, pt3dBegin.z, pt3dEnd.x, pt3dEnd.y, pt3dEnd.z);

	return pt3d;
}

void td5mapGraph::DrawText2D(const wxString& text, wxCoord x, wxCoord y)
{
    m_dc->DrawText(text, x, y);
}

void td5mapGraph::DrawText3D(const wxString& text, wxCoord x, wxCoord y, wxCoord z)
{
	wxPoint pt;

	pt = TransformTo2D(x, y, z);

    m_dc->DrawText(text, pt.x, pt.y);
}

void td5mapGraph::DrawText3D(const wxString& text, wxCoord x, wxCoord y, wxCoord z, wxCoord offsetx2D, wxCoord offsety2D)
{
	wxPoint pt;

	pt = TransformTo2D(x, y, z);

    m_dc->DrawText(text, pt.x + offsetx2D, pt.y + offsety2D);
}


void td5mapGraph::DrawPolygon2D(int n, wxPoint points[], wxCoord xoffset, wxCoord yoffset, wxPolygonFillMode fill_style)
{
    m_dc->DrawPolygon(n, points, xoffset, yoffset, fill_style);
}

void td5mapGraph::DrawPolygon3D(int n, point3D points3d[], wxCoord xoffset, wxCoord yoffset, wxPolygonFillMode fill_style)
{
    wxPoint *points2d = new wxPoint[n];

    for(int i = 0; i < n; i++)
    {
        points2d[i] = TransformTo2D(points3d[i]);
    }

    m_dc->DrawPolygon(n, points2d , xoffset, yoffset, fill_style);

    delete [] points2d;
}


//! td5mapGraphCursor
td5mapGraphCursor::td5mapGraphCursor(wxSize size, bool visible)
{
    m_xpos = 0.0;
    m_ypos = 0.0;
    m_zpos = 0.0;
    m_size = size;
    m_visible = visible;
}

td5mapGraphCursor::~td5mapGraphCursor()
{

}

point3D td5mapGraphCursor::Move(double xpos, double ypos, double zpos)
{
    point3D oldPoint(m_xpos, m_ypos, m_zpos);

    m_xpos = xpos;
    m_ypos = ypos;
    m_zpos = zpos;

    return oldPoint;
}

wxPoint td5mapGraphCursor::Move(int xpos, int ypos)
{
    wxPoint oldPoint(m_xpos, m_ypos);

    m_xpos = xpos;
    m_ypos = ypos;

    return oldPoint;
}

void td5mapGraphCursor::SetVisible(bool visible)
{
    m_visible = visible;
}


//! point3D
point3D::point3D()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

point3D::point3D(double cx, double cy, double cz)
{
	x = cx;
	y = cy;
	z = cz;
}

point3D::point3D(point3D& pt3d)
    :wxObject(pt3d),
	x(pt3d.x),
	y(pt3d.y),
	z(pt3d.z)
{

}

point3D::point3D(const point3D& pt3d)
    :wxObject(pt3d),
	x(pt3d.x),
	y(pt3d.y),
	z(pt3d.z)
{

}

point3D::~point3D()
{

}
