/***************************************************************
 * Name:      ewxRange.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2011-01-01
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef EWXRANGE_H
#define EWXRANGE_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class ewxRange : public wxObject
{
    public:
        ewxRange();
        ewxRange(int toprow, int leftcol, int bottomrow, int rightcol);
        virtual ~ewxRange();
        ewxRange(const ewxRange& other);
        ewxRange& operator=(const ewxRange& other);
        bool IsInRange(int col, int row);
        int GetRows(){return ((bottomRow - topRow) + 1);};
        int GetCols(){return ((rightCol - leftCol) + 1);};

        int topRow;
        int bottomRow;
        int leftCol;
        int rightCol;

    protected:
    private:
};

#endif // EWXRANGE_H
