/***************************************************************
 * Name:      dc3d.cpp
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-07
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "dc3d.h"

const double PI = 4*atan(1)  ;

//////////////////////////////////////////////////////////////////////
// point3d

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

point3d::point3d()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

point3d::point3d(double cx, double cy, double cz)
{
	x = cx;
	y = cy;
	z = cz;
}

point3d::point3d(point3d& pt3d)
{
	x = pt3d.x;
	y = pt3d.y;
	z = pt3d.z;
}

point3d::point3d(const point3d& pt3d)
{
	x = pt3d.x;
	y = pt3d.y;
	z = pt3d.z;
}

point3d::~point3d()
{

}

//////////////////////////////////////////////////////////////////////
// dc3d

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

dc3d::dc3d()
{
	m_dc = NULL;
	m_theta0 = -25 ;
	m_theta1 = 25 ;

	m_resXY = 0.0;
	m_resXZ = 0.0;
	m_resZY = 0.0;
	m_resX = 0.0;
	m_resY = 0.0;
}

dc3d::dc3d(wxDC& DC)
{
	m_dc = &DC;
	m_theta0 = -25 ;
	m_theta1 = 25 ;

	m_resXY = 0.0;
	m_resXZ = 0.0;
	m_resZY = 0.0;
	m_resX = 0.0;
	m_resY = 0.0;
}


dc3d::~dc3d()
{

}

void dc3d::SetDC(wxDC *pDC)
{
	m_dc = pDC;
}

wxDC *dc3d::GetDC()
{
	return m_dc;
}

void dc3d::SetRange(double minX, double maxX, double minY, double maxY, double minZ, double maxZ, double orgX, double orgY)
{
	double dRy , dRx ;

	dRy = maxY - minY ;
	dRx = maxX - minX ;

//	miny =  minY - (dRy) ;
//	maxy =  maxY + (dRy) ;
//	maxy = maxY + fabs(2 * dRy * sin(m_theta0 * PI/180)) ;
//	miny = minY ;
	m_rangeY[0] = minY ;
	m_rangeY[1] = maxY ;

//	m_orgY = minY;
//	m_orgX = minX;
	m_orgY = orgY;
	m_orgX = orgX;


	m_resXZ = 1.0 * fabs(((maxX - minX )*cos(m_theta0 * PI/180)) /
		((maxZ - minZ)*cos(m_theta1 * PI / 180))) ;


//	maxx = fabs (2 * dRx * cos(m_theta0 * PI /180)) ;
//	minx = minX ;
//	rangeX[0] = minX ;
//	rangeX[1] = maxX ;
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
}

void dc3d::SetRect(wxRect rect)
{
	m_axisRect = rect;
}

wxPoint dc3d::TransformTo2d(point3d pt3d)
{
	return TransformTo2d((double) pt3d.x, (double) pt3d.y, (double) pt3d.z);
}


wxPoint dc3d::TransformTo2d(double x, double y, double z)
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

point3d dc3d::DrawLine(double xbegin, double ybegin, double zbegin, double xend, double yend, double zend)
{
    point3d pt3d(xend, yend, zend);
	wxPoint ptbegin, ptend;

	ptbegin = TransformTo2d(xbegin, ybegin, zbegin);
	ptend = TransformTo2d(xend, yend, zend);

	m_dc->DrawLine(ptbegin.x, ptbegin.y, ptend.x, ptend.y);

	return pt3d;
}

point3d dc3d::DrawLine(point3d pt3dBegin, point3d pt3dEnd)
{
    point3d pt3d;

    pt3d = DrawLine(pt3dBegin.x, pt3dBegin.y, pt3dBegin.z, pt3dEnd.x, pt3dEnd.y, pt3dEnd.z);

	return pt3d;
}

void dc3d::DrawText(const wxString& text, wxCoord x, wxCoord y, wxCoord z)
{
	wxPoint pt;

	pt = TransformTo2d(x, y, z);

    m_dc->DrawText(text, pt.x, pt.y);
}

void dc3d::DrawPolygon(int n, point3d points3d[], wxCoord xoffset, wxCoord yoffset, /*int*/wxPolygonFillMode fill_style)
{
    wxPoint *points2d = new wxPoint[n];

    for(int i = 0; i < n; i++)
    {
        points2d[i] = TransformTo2d(points3d[i]);
    }

    m_dc->DrawPolygon(n, points2d , xoffset, yoffset, fill_style);

    delete [] points2d;
}



/*
bool dc3d::TransformTo3d(int x, int y, point3d& ptRet)
{
    double dx, dy;
	double rx , ry ;
	int xPixel , yPixel ;

	xPixel = x - m_axisRect.GetLeft();
	yPixel = m_axisRect.GetBottom() - y;

	rx = xPixel * m_resX;
	ry = yPixel * m_resY;

	dx = rx + rangeX[0];
	dy = ry + rangeY[0];

	dx -= m_orgX ;
	dy -= m_orgY  ;

    bool found = false;
    ptRet = point3d(0.0, 0.0, 0.0);
	for(double ix = m_r3dX[0]; ix < m_r3dX[1]; ix += 0.1 )
    {
        double iy = dy - (m_resY/m_resX) * ((-((ix * cos(m_theta0 * PI / 180.0)) - dx) / cos(m_theta1 * PI / 180.0)) * sin(m_theta1 * PI / 180)) + (ix * sin(m_theta0 * PI / 180));

        if ((iy <  m_r3dY[1]) && (iy >  m_r3dY[0]))
        {
            double iz = ((ix * cos(m_theta0 * PI / 180.0)) - dx) / (m_resXZ * cos(m_theta1 * PI / 180.0));

            if ((iz <  m_r3dZ[1]) && (iz >  m_r3dZ[0]))
            {
                point3d ptCandidate = point3d(ix, iy, iz);

                wxPoint ptConverted = TransformTo2d(ptCandidate);
                if((ptConverted.x == x) &&  (ptConverted.y == y))
                {
                    ptRet = ptCandidate;
                    found = true;
                    break;
                }

            }
        }
    }


	return found ;
}
*/
