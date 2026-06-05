/***************************************************************
 * Name:      td5mapGraph.h
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2012-04-25
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef TD5MAPGRAPH_H
#define TD5MAPGRAPH_H

#include <wx/mdi.h>
#include <wx/docview.h>

#include "math.h"

#include "td5mapTable.h"
#include "ewxDynArray.h"

#define GDC_TYPE_2D              2
#define GDC_TYPE_3D              3

#define GDC_SHOW_BASE           10
#define GDC_SHOW_CURRENT        11
#define GDC_SHOW_DIFF           12
#define GDC_SHOW_BASE_CURRENT   13

class td5mapGraphCursor;
class point3D;

class td5mapGraph
{
    public:
        td5mapGraph(wxScrolledWindow *parent);
        td5mapGraph(wxDC& dc, wxScrolledWindow *parent, wxRect canvasrect, td5mapTable *maptable = NULL, int graphshow = GDC_SHOW_CURRENT);
        virtual ~td5mapGraph();

        bool Prepare(wxDC& dc, wxRect canvasrect, td5mapTable *maptable = NULL, int graphshow = GDC_SHOW_CURRENT);
        bool IsPrepared(){return m_prepared;};
        void Draw();

        void SetRange2D(double minX, double maxX, double minY, double maxY);
        void DrawLine2D(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2);

        void DrawPolygon2D(int n, wxPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0, wxPolygonFillMode fill_style = wxODDEVEN_RULE);
        void DrawText2D(const wxString& text, wxCoord x, wxCoord y);

        void SetRange3D(double minX, double maxX, double minY, double maxY, double minZ, double maxZ, double orgX = 0.0, double orgY = 0.0);
        point3D DrawLine3D(double xbegin, double ybegin, double zbegin, double xend, double yend, double zend);
        point3D DrawLine3D(point3D pt3dBegin, point3D pt3dEnd);
        void DrawPolygon3D(int n, point3D points3d[], wxCoord xoffset = 0, wxCoord yoffset = 0, wxPolygonFillMode fill_style = wxODDEVEN_RULE);
        void DrawText3D(const wxString& text, wxCoord x, wxCoord y, wxCoord z);
    	wxPoint TransformTo2D(point3D pt3d);
        wxPoint TransformTo2D(double x, double y, double z);

    protected:

    private:
    	wxDC* m_dc;
        wxScrolledWindow *m_parent;

        int m_show;
        int m_graphtype;
        bool m_prepared;

        td5mapTable *m_table;
        ewxDyn2DArray<point3D> m_points;

        td5mapGraphCursor *m_cursor;
        bool m_selecting;
        wxPoint m_selectionBegin;
        wxPoint m_selectionEnd;

        wxRect m_axisRect;
        double m_theta0 ;
        double m_theta1 ;
        double m_resXZ , m_resXY , m_resZY;
        double m_orgY;
        double m_orgX;
        double m_r3dZ[2], m_r3dX[2], m_r3dY[2] ;
        double m_resY, m_resX ;
        double m_rangeY[2], m_rangeX[2];

        int m_maxXAxis;
        int m_maxYAxis;
        int m_maxZAxis;
        int m_minXAxis;
        int m_minYAxis;
        int m_minZAxis;
        int m_xaxisTagsNum;
        int m_yaxisTagsNum;
        int m_zaxisTagsNum;

};

class point3D : public wxObject
{
public:
	point3D& operator=( const point3D &s )  // assignment operator
	{
		x = s.x;
		y = s.y;
		z = s.z;

		return *this;
	}

public:
	point3D();
	point3D(double cx, double cy, double cz);
	point3D(point3D& pt3d);
	point3D(const point3D& pt3d);

	virtual ~point3D();

public:
	double x, y, z;
};


#define cursorDefaultSize wxSize(5, 5)

class td5mapGraphCursor
{
    public:
        td5mapGraphCursor(wxSize size = cursorDefaultSize, bool visible = false);
        virtual ~td5mapGraphCursor();

        point3D Move(double xpos, double ypos, double zpos);
        point3D Move(point3D pt){return Move(pt.x, pt.y, pt.z);};
        wxPoint Move(int xpos, int ypos);
        wxPoint Move(wxPoint pt){return Move(pt.x, pt.y);};
        point3D Get3dPosition(){return point3D(m_xpos, m_ypos, m_zpos);};
        wxPoint Get2dPosition(){return wxPoint(m_xpos, m_ypos);};
        int GetX(){return m_xpos;};
        int GetY(){return m_ypos;};
        int GetZ(){return m_zpos;};
        wxSize GetSize(){return m_size;};
        void SetVisible(bool visible);
        bool IsVisible(){return m_visible;};

    protected:
    private:
        double m_xpos;
        double m_ypos;
        double m_zpos;
        bool m_visible;
        wxSize m_size;
};


#endif // TD5MAPGRAPH_H
