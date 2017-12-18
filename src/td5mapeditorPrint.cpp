
#include "td5mapeditorPrint.h"
#include "td5mapeditorApp.h"
#include "td5mapeditorView.h"
#include "td5mapeditorMainFrame.h"

#include <wx/textfile.h>
#include <wx/tokenzr.h>

#if wxUSE_PRINTING_ARCHITECTURE
td5mapeditorPrint::td5mapeditorPrint(td5mapeditorDoc *doc, wxChar *title): wxPrintout(title)
{
    m_doc = doc;
    m_table = doc->GetSelectedMapTable();
    m_printed = 0;
}

td5mapeditorPrint::~td5mapeditorPrint()
{
    //dtor
}


bool td5mapeditorPrint::OnPrintPage (int page) {

    wxDC *dc = GetDC();
    if (!dc) return false;

    // scale DC
    PrintScaling (dc);

    // print page
    if (page == 1) m_printed = 0;

    /*
    m_printed = m_edit->FormatRange (1, m_printed, m_edit->GetLength(),
                                     dc, dc, m_printRect, m_pageRect);
    */

    dc->SetFont(wxFont(7, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false));

    int col[2] = {m_printRect.GetLeft() + 10, m_printRect.GetLeft() + 200 }; int row = m_printRect.GetTop() + 20;
    dc->DrawText(_T("Base firmware:"), col[0], row); dc->DrawText(m_doc->GetMapName(), col[1], row); row += 20;
    dc->DrawText(_T("Map address:"), col[0], row); dc->DrawText(wxString::Format(_T("0x%04X"), m_table->m_address), col[1], row); row += 20;
    dc->DrawText(_T("Map role:"), col[0], row); dc->DrawText(m_table->m_name, col[1], row); row += 20;

    row = m_printRect.GetTop() + 100;
    dc->DrawText(_T("Map table:"), col[0], row); row += 20;

    wxRect gridRect(m_printRect.GetX(), m_printRect.GetY()+m_printRect.GetHeight() / 8, m_printRect.GetWidth(), (m_printRect.GetHeight() / 4)) ;
    DrawGrid(dc, gridRect, 1);

    row = (m_printRect.GetHeight() / 2);
    dc->DrawText(_T("Map graph:"), col[0], row); row += 20;

    graphDC *gDC;

    wxRect rect(m_printRect.GetX()+m_printRect.GetWidth() / 8, m_printRect.GetY()+m_printRect.GetHeight() / 2, (m_printRect.GetWidth() / 4) * 3, (m_printRect.GetHeight() / 2)) ;
    gDC = new graphDC(*dc, NULL, /*m_printRect*/ rect, m_table, GDC_SHOW_BASE_CURRENT);
    gDC->Draw();

    //delete gDC;

    m_printed = 1;

    return true;
}

bool td5mapeditorPrint::OnBeginDocument (int startPage, int endPage) {

    if (!wxPrintout::OnBeginDocument (startPage, endPage)) {
        return false;
    }

    return true;
}

void td5mapeditorPrint::GetPageInfo (int *minPage, int *maxPage, int *selPageFrom, int *selPageTo) {

    // initialize values
    *minPage = 0;
    *maxPage = 0;
    *selPageFrom = 0;
    *selPageTo = 0;

    // scale DC if possible
    wxDC *dc = GetDC();
    if (!dc) return;
    PrintScaling (dc);

    // get print page informations and convert to printer pixels
    wxSize ppiScr;
    GetPPIScreen (&ppiScr.x, &ppiScr.y);
    wxSize page = g_pageSetupData->GetPaperSize();
    page.x = static_cast<int> (page.x * ppiScr.x / 25.4);
    page.y = static_cast<int> (page.y * ppiScr.y / 25.4);
    m_pageRect = wxRect (0,
                         0,
                         page.x,
                         page.y);

    // get margins informations and convert to printer pixels
    wxPoint pt = g_pageSetupData->GetMarginTopLeft();
    int left = pt.x;
    int top = pt.y;
    pt = g_pageSetupData->GetMarginBottomRight();
    int right = pt.x;
    int bottom = pt.y;

    top = static_cast<int> (top * ppiScr.y / 25.4);
    bottom = static_cast<int> (bottom * ppiScr.y / 25.4);
    left = static_cast<int> (left * ppiScr.x / 25.4);
    right = static_cast<int> (right * ppiScr.x / 25.4);

    m_printRect = wxRect (left,
                          top,
                          page.x - (left + right),
                          page.y - (top + bottom));


    m_printed = 1;
    *maxPage = 1;

    if (*maxPage > 0) *minPage = 1;
    *selPageFrom = *minPage;
    *selPageTo = *maxPage;
}

bool td5mapeditorPrint::HasPage (int WXUNUSED(page))
{
    //return (m_printed < m_edit->GetLength());
    return true;
}

bool td5mapeditorPrint::PrintScaling (wxDC *dc){

    // check for dc, return if none
    if (!dc) return false;

    // get printer and screen sizing values
    wxSize ppiScr;
    GetPPIScreen (&ppiScr.x, &ppiScr.y);
    if (ppiScr.x == 0) { // most possible guess 96 dpi
        ppiScr.x = 96;
        ppiScr.y = 96;
    }
    wxSize ppiPrt;
    GetPPIPrinter (&ppiPrt.x, &ppiPrt.y);
    if (ppiPrt.x == 0) { // scaling factor to 1
        ppiPrt.x = ppiScr.x;
        ppiPrt.y = ppiScr.y;
    }
    wxSize dcSize = dc->GetSize();
    wxSize pageSize;
    GetPageSizePixels (&pageSize.x, &pageSize.y);

    // set user scale
    float scale_x = (float)(ppiPrt.x * dcSize.x) /
                    (float)(ppiScr.x * pageSize.x);
    float scale_y = (float)(ppiPrt.y * dcSize.y) /
                    (float)(ppiScr.y * pageSize.y);
    dc->SetUserScale (scale_x, scale_y);

    return true;
}


bool td5mapeditorPrint::DrawGrid(wxDC *dc, wxRect& gridRect, int page)
{
    int origX = gridRect.GetX();
    int origY = gridRect.GetY();

    wxFont font = wxFont(5, wxDEFAULT, wxNORMAL, wxFONTWEIGHT_BOLD, false);
    wxBrush brush = *wxBLACK_BRUSH;

    int colIndex = 0; int rowIndex = 0;

	int countWidth=0;
	int countHeight=0;
	int cellInitialH=0;
	int cellInitialW=0;
	int colSize=gridRect.GetWidth()/22;
	int rowSize=gridRect.GetHeight()/20;

	//draw column headers if requested
	wxString colLabel;
    for(int i=0;i<m_table->GetCols();i++)
    {
        colLabel.Printf(_T("%d"), (wxInt16) m_table->GetCurrentHeaderColValue(i));
        wxRect rect = wxRect(countWidth+origX,countHeight+origY,colSize,rowSize);

        dc->SetBrush(*wxLIGHT_GREY_BRUSH);
        DrawTextInRectangle(*dc,colLabel,rect,font,brush,wxALIGN_CENTER,wxALIGN_CENTER);
        countWidth+=colSize;
    }
    cellInitialH=rowSize;

	//draw row headers if requested
	countHeight=cellInitialH;
    wxString rowLabel;
    for(int i=0;i<m_table->GetRows();i++)
    {
        rowLabel.Printf(_T("%d"), (wxInt16) m_table->GetCurrentHeaderRowValue(i));
        wxRect rect = wxRect(countWidth+origX,countHeight+origY,colSize,rowSize);

        dc->SetBrush(*wxLIGHT_GREY_BRUSH);
        DrawTextInRectangle(*dc,rowLabel,rect,font,brush,wxALIGN_CENTER,wxALIGN_CENTER);
        countHeight+=rowSize;
    }
    cellInitialW=0;

	// Draw cell content
	countHeight=cellInitialH;
	for(int j=0;j<m_table->GetRows();j++)
	{
		countWidth=cellInitialW;
		wxString cellValue;
		for(int k=0;k<m_table->GetCols();k++)
		{
			//dc->SetPen(wxNullPen);
			dc->SetBrush(wxNullBrush);
			wxBrush bgBrush = *wxTRANSPARENT_BRUSH;
			dc->SetBrush(bgBrush);

            cellValue.Printf(_T("%d"), (wxInt16) m_table->GetCurrentValue(k,j));
            wxRect rect = wxRect(countWidth+origX,countHeight+origY,colSize,rowSize);

			//drawTextInRect(dc,wxString(wxT("")),wxRect(countWidth,cellInitialH,grid->GetColSize(k),grid->GetRowSize(j)),grid->GetCellFont(j,k),*wxBLACK_BRUSH,LEFT);
			DrawTextInRectangle(*dc,cellValue,rect,font,brush);
			countWidth+=colSize;
		}
		countHeight+=rowSize;
	}

	return true;
}

void td5mapeditorPrint::DrawTextInRectangle(wxDC& dc,wxString& str,wxRect& rect,wxFont& font,wxBrush& fontbrush,int horizAlign,int vertAlign,int textOrientation)
{
	dc.DrawRectangle(rect.x,rect.y,rect.width,rect.height);

	dc.SetFont(font);
	dc.SetBrush(fontbrush);

	wxArrayString lines;

	wxArrayString naturalLines;
	StringToLines(str,naturalLines);

	rect.x+=2;
	rect.width-=2;
	for(int i=0;i<(int)naturalLines.Count();i++){
		wxArrayString wrappedLines=GetTextLines(dc,naturalLines.Item(i),font,rect);
		for(int j=0;j<(int)wrappedLines.Count();j++)
			lines.Add(wrappedLines.Item(j));
	}

	DrawTextRectangle(dc,lines,rect,horizAlign,vertAlign,textOrientation);

}

void td5mapeditorPrint::DrawTextRectangle(wxDC& dc,const wxArrayString& lines,const wxRect& rect,int horizAlign,int vertAlign,int textOrientation )
{
    long textWidth, textHeight;
    long lineWidth, lineHeight;
    int nLines;

    dc.SetClippingRegion( rect );

    nLines = lines.GetCount();
    if( nLines > 0 )
    {
        int l;
        float x = 0.0, y = 0.0;

        if( textOrientation == wxHORIZONTAL )
            GetTextBoxSize(dc, lines, &textWidth, &textHeight);
        else
            GetTextBoxSize( dc, lines, &textHeight, &textWidth );

        switch( vertAlign )
        {
        case wxALIGN_BOTTOM:
            if( textOrientation == wxHORIZONTAL )
                y = rect.y + (rect.height - textHeight - 1);
            else
                x = rect.x + rect.width - textWidth;
            break;

        case wxALIGN_CENTRE:
            if( textOrientation == wxHORIZONTAL )
                y = rect.y + ((rect.height - textHeight)/2);
            else
                x = rect.x + ((rect.width - textWidth)/2);
            break;

        case wxALIGN_TOP:
        default:
            if( textOrientation == wxHORIZONTAL )
                y = rect.y + 1;
            else
                x = rect.x + 1;
            break;
        }

        // Align each line of a multi-line label
        for( l = 0; l < nLines; l++ )
        {
            dc.GetTextExtent(lines[l], &lineWidth, &lineHeight);

            switch( horizAlign )
            {
            case wxALIGN_RIGHT:
                if( textOrientation == wxHORIZONTAL )
                    x = rect.x + (rect.width - lineWidth - 1);
                else
                    y = rect.y + lineWidth + 1;
                break;

            case wxALIGN_CENTRE:
                if( textOrientation == wxHORIZONTAL )
                    x = rect.x + ((rect.width - lineWidth)/2);
                else
                    y = rect.y + rect.height - ((rect.height - lineWidth)/2);
                break;

            case wxALIGN_LEFT:
            default:
                if( textOrientation == wxHORIZONTAL )
                    x = rect.x + 1;
                else
                    y = rect.y + rect.height - 1;
                break;
            }

            if( textOrientation == wxHORIZONTAL )
            {
                dc.DrawText( lines[l], (int)x, (int)y );
                y += lineHeight;
            }
            else
            {
                dc.DrawRotatedText( lines[l], (int)x, (int)y, 90.0 );
                x += lineHeight;
            }
        }
    }
    dc.DestroyClippingRegion();
}

void td5mapeditorPrint::StringToLines( const wxString& value, wxArrayString& lines )
{
    int startPos = 0;
    int pos;
    wxString eol = wxTextFile::GetEOL( wxTextFileType_Unix );
    wxString tVal = wxTextFile::Translate( value, wxTextFileType_Unix );

    while ( startPos < (int)tVal.Length() )
    {
        pos = tVal.Mid(startPos).Find( eol );
        if ( pos < 0 )
        {
            break;
        }
        else if ( pos == 0 )
        {
            lines.Add( wxEmptyString );
        }
        else
        {
            lines.Add( value.Mid(startPos, pos) );
        }
        startPos += pos+1;
    }
    if ( startPos < (int)value.Length() )
    {
        lines.Add( value.Mid( startPos ) );
    }
}

wxArrayString td5mapeditorPrint::GetTextLines(wxDC& dc,const wxString& data,wxFont& font,const wxRect& rect)
{
	wxArrayString lines;

	wxCoord x = 0, y = 0, curr_x = 0;
    wxCoord max_x = rect.GetWidth();

    dc.SetFont(font);
    wxStringTokenizer tk(data , _T(" \n\t\r"));
    wxString thisline = wxEmptyString;

    while ( tk.HasMoreTokens() )
    {
        wxString tok = tk.GetNextToken();
        //FIXME: this causes us to print an extra unnecesary
        //       space at the end of the line. But it
        //       is invisible , simplifies the size calculation
        //       and ensures tokens are separated in the display
        tok += _T(" ");

        dc.GetTextExtent(tok, &x, &y);
        if ( curr_x + x > max_x)
        {
            lines.Add( wxString(thisline) );
            thisline = tok;
            curr_x=x;
        }
        else
        {
            thisline+= tok;
            curr_x += x;
        }
    }
    //Add last line
    lines.Add( wxString(thisline) );

    return lines;
}

void td5mapeditorPrint::GetTextBoxSize( wxDC& dc,const wxArrayString& lines,long *width, long *height )
{
    long w = 0;
    long h = 0;
    long lineW, lineH;

    size_t i;
    for ( i = 0;  i < lines.GetCount();  i++ )
    {
        dc.GetTextExtent( lines[i], &lineW, &lineH );
        w = wxMax( w, lineW );
        h += lineH;
    }

    *width = w;
    *height = h;
}
#endif // wxUSE_PRINTING_ARCHITECTURE
