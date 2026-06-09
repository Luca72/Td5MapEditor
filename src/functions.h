// Functions.h : Global functions
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTIONS__INCLUDED_)
#define AFX_FUNCTIONS__INCLUDED_

#include <wx/wx.h>

#define MAP_FILE_LENGTH 	118798

wxWord LoHi2HiLo(wxWord word);
wxWord HiLo2LoHi(wxWord word);
wxWord SumRange(wxWord *pMapFileData, wxUint32 start, wxUint32 end, wxUint32 correctionOffset);
void FirmwareAndTablesChecksum(wxWord *pMapFileData);
wxWord NanocomChecksum(wxWord *pMapFileData);
wxString ExtractFileName(wxString path);
template <class T> const T& max ( const T& a, const T& b ) {return (b<a)?a:b;};
template <class T> const T& min ( const T& a, const T& b ) {return (b>a)?a:b;};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNCTIONS__INCLUDED_)
