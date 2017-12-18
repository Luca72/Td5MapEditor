#ifndef TD5MAPEDITORPRINT_H
#define TD5MAPEDITORPRINT_H

#include <wx/print.h>

#include "td5mapeditorDoc.h"
#include "td5mapTable.h"
#include "ewxGrid.h"

class td5mapeditorPrint : public wxPrintout
{
public:
    //! constructor
        td5mapeditorPrint(td5mapeditorDoc *doc, wxChar *title = (wxChar*)_T(""));
        virtual ~td5mapeditorPrint();

    //! event handlers
    bool OnPrintPage (int page);
    bool OnBeginDocument (int startPage, int endPage);

    //! print functions
    bool HasPage (int page);
    void GetPageInfo (int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);
    bool DrawGrid(wxDC *dc, wxRect& gridRect, int page);

protected:
	static void DrawTextInRectangle(wxDC& dc,wxString& str,wxRect& rect,wxFont& font,wxBrush& fontbrush,int horizAlign=wxALIGN_LEFT,int vertAlign=wxALIGN_TOP,int textOrientation=wxHORIZONTAL );
	static void DrawTextRectangle(wxDC& dc,const wxArrayString& lines,const wxRect& rect,int horizAlign=wxALIGN_LEFT,int vertAlign=wxALIGN_TOP,int textOrientation=wxHORIZONTAL );
    static void StringToLines(const wxString& value, wxArrayString& lines );
    static wxArrayString GetTextLines(wxDC& dc,const wxString& data,wxFont& font,const wxRect& rect);
    static void GetTextBoxSize(wxDC& dc,const wxArrayString& lines,long *width, long *height );
private:
    td5mapeditorDoc *m_doc;
    td5mapTable *m_table;
    int m_printed;
    wxRect m_pageRect;
    wxRect m_printRect;

    bool PrintScaling (wxDC *dc);
};

#endif // TD5MAPEDITORPRINT_H
