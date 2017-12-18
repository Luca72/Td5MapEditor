/***************************************************************
 * Name:      DC3D.cpp
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-07
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef __DC3DH__
#define __DC3DH__

#include "wx/wx.h"
#include "math.h"

class point3d : public wxObject
{
public:
	point3d& operator=( const point3d &s )  // assignment operator
	{
		x = s.x;
		y = s.y;
		z = s.z;

		return *this;
	}

public:
	point3d();
	point3d(double cx, double cy, double cz);
	point3d(point3d& pt3d);
	point3d(const point3d& pt3d);

	virtual ~point3d();

public:
	double x, y, z;
};


class dc3d : public wxObject
{
public:
	point3d DrawLine(double xbegin, double ybegin, double zbegin, double xend, double yend, double zend);
	point3d DrawLine(point3d pt3dBegin, point3d pt3dEnd);
	void DrawPolygon(int n, point3d points3d[], wxCoord xoffset = 0, wxCoord yoffset = 0, /*int*/wxPolygonFillMode fill_style = wxODDEVEN_RULE);
	void DrawText(const wxString& text, wxCoord x, wxCoord y, wxCoord z);

	void SetDC(wxDC* pDC);
	void SetRange(double minX, double maxX, double minY, double maxY, double minZ, double maxZ, double orgX = 0.0, double orgY = 0.0);
	void SetRect(wxRect rect);
    wxDC *GetDC();

	dc3d();
	dc3d(wxDC& dc);
	virtual ~dc3d();


/*protected:*/
public:
	wxPoint TransformTo2d(point3d pt3d);
	wxPoint TransformTo2d(double x, double y, double z);
	//bool TransformTo3d(wxPoint pt2d, point3d& ptRet){return TransformTo3d(pt2d.x, pt2d.y, ptRet);};
	//bool TransformTo3d(int x, int y, point3d& ptRet);

protected:
	wxDC* m_dc;
	wxRect m_axisRect;
	double m_theta0 ;
	double m_theta1 ;
	double m_resXZ , m_resXY , m_resZY;
	double m_orgY;
	double m_orgX;
	double m_r3dZ[2], m_r3dX[2], m_r3dY[2] ;
	double m_resY, m_resX ;
	double m_rangeY[2], m_rangeX[2];
};

#endif
