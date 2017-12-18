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
    //dtor
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
    m_axisRect = canvasrect;
    m_show = graphshow;
    m_points.resize(m_table->GetCols(), m_table->GetRows());

    int minY, maxY;
    if(m_show == GDC_SHOW_DIFF)
        m_table->EvalDiffRange(minY, maxY);
    else
        m_table->EvalRange(minY, maxY);


    if (m_table->IsBidimensional())
    {
        m_type = GDC_TYPE_2D;

		m_yaxisTagsNum = 10;
		m_xaxisTagsNum = m_table->GetCols();
		m_zaxisTagsNum = 0;

		SetRange2D(0, m_table->GetCols(), minY, maxY);

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

    if (m_table->IsTridimensional())
    {
        m_type = GDC_TYPE_3D;

        m_xaxisTagsNum = m_table->GetRows() - 1;
        m_yaxisTagsNum = 10;
        m_zaxisTagsNum = m_table->GetCols() - 1;

        SetRange3D(-(m_table->GetRows() - 1), (m_table->GetRows() - 1), minY, maxY, -(m_table->GetCols() - 1), (m_table->GetCols() - 1));

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

void td5mapGraph::Draw()
{
//    DrawCartesianAxis();
//    DrawHelpingLines();
//    DrawGraph();
//    DrawTags();
//    DrawSelection();
//    DrawCursor();
//    DrawTitle();
}


void td5mapGraph::SetRange2D(double minX, double maxX, double minY, double maxY)
{
    m_maxXAxis = maxX;
    m_maxYAxis = maxY;
    m_minXAxis = minX;
    m_minYAxis = minY;
    m_minZAxis = 0.0;
    m_maxZAxis = 0.0;
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
{
	x = pt3d.x;
	y = pt3d.y;
	z = pt3d.z;
}

point3D::point3D(const point3D& pt3d)
{
	x = pt3d.x;
	y = pt3d.y;
	z = pt3d.z;
}

point3D::~point3D()
{

}
