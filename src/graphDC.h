/***************************************************************
 * Name:      graphDC.h
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-17
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef GRAPHDC_H
#define GRAPHDC_H

#include <wx/mdi.h>
#include <wx/docview.h>

#include "dc3d.h"

#include "td5mapTable.h"
#include "ewxDynArray.h"

#define GDC_TYPE_2D     2
#define GDC_TYPE_3D     3

#define GDC_SHOW_BASE           10
#define GDC_SHOW_CURRENT        11
#define GDC_SHOW_DIFF           12
#define GDC_SHOW_BASE_CURRENT   13

class graphCursor;

class graphDC : public dc3d
{
    public:
        graphDC(wxScrolledWindow *parent);
        graphDC(wxDC& DC, wxScrolledWindow *parent, wxRect canvasrect, td5mapTable *maptable = NULL, int graphshow = GDC_SHOW_CURRENT);
        virtual ~graphDC();

        bool Prepare(wxDC& DC, wxRect canvasrect, td5mapTable *maptable = NULL, int graphshow = GDC_SHOW_CURRENT);
        void SetRange(double minX, double maxX, double minY, double maxY, double minZ, double maxZ);
        void SetRange(double minX, double maxX, double minY, double maxY);
        void DrawSelectedPointArea(double xAxis, double yAxis, double zAxis);
        void OnMouseMove(int x, int y);
        void OnMouseSelectBegin(int x, int y);
        void OnMouseSelectEnd(int x, int y);
        void MoveCursor(point3d pt3d){MoveCursor(pt3d.x, pt3d.y, pt3d.z);};
        void MoveCursor(int x, int y, int z);
        bool IsPrepared(){return m_prepared;};
        wxScrolledWindow *GetParent(){return m_parent;};
        void Draw();
        void MoveCursor(int col, int row);
        void SelectRange(ewxRange range);
        void ResetRange(){ m_selectionBegin = wxPoint(0, 0); m_selectionEnd = wxPoint(0, 0); };
        bool IsSelecting(){return m_selecting;};

    protected:
        void DrawCartesianAxis();
        void DrawTags();
        void DrawHelpingLines();
        void DrawGraph();
        void DrawCursor();
        void DrawSelection();
        void DrawTitle();
        bool TransformTo3dGrid(wxPoint pt2d, point3d& ptRet){return TransformTo3dGrid(pt2d.x, pt2d.y, ptRet);};
        bool TransformTo3dGrid(int x, int y, point3d& ptRet);
        bool TransformTo2dGrid(wxPoint pt2d, wxPoint& ptRet){return TransformTo2dGrid(pt2d.x, pt2d.y, ptRet);};
        bool TransformTo2dGrid(int x, int y, wxPoint& ptRet);
        bool PointIsNear(wxPoint mapPoint, wxPoint mousePoint, double precision);
        wxPoint TransformTo2d(wxPoint pt){return TransformTo2d(pt.x, pt.y);};
        wxPoint TransformTo2d(int x, int y);

    private:
        int m_maxXAxis;
        int m_maxYAxis;
        int m_maxZAxis;
        int m_minXAxis;
        int m_minYAxis;
        int m_minZAxis;

        int m_xaxisTagsNum;
        int m_yaxisTagsNum;
        int m_zaxisTagsNum;

        double m_xaxisRes;
        double m_yaxisRes;
        double m_zaxisRes;

        double m_xaxisTagRes;
        double m_yaxisTagRes;
        double m_zaxisTagRes;

        int m_show;
        bool m_prepared;

        td5mapTable *m_table;
        wxScrolledWindow *m_parent;
        graphCursor *m_cursor;
        ewxDyn2DArray<point3d> m_points;
        bool m_selecting;
        wxPoint m_selectionBegin;
        wxPoint m_selectionEnd;
};

#define cursorDefaultSize wxSize(5, 5)

class graphCursor
{
    public:
        graphCursor();
        graphCursor(graphDC *DC, wxSize size = cursorDefaultSize, bool visible = false);
        virtual ~graphCursor();

        point3d Move(double xpos, double ypos, double zpos);
        point3d Move(point3d pt){return Move(pt.x, pt.y, pt.z);};
        wxPoint Move(int xpos, int ypos);
        wxPoint Move(wxPoint pt){return Move(pt.x, pt.y);};
        point3d Get3dPosition(){return point3d(m_xpos, m_ypos, m_zpos);};
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
        graphDC *m_dc;
        wxSize m_size;
};

#endif // GRAPHDC_H
