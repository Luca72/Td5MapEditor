/***************************************************************
 * Name:      ewxRange.cpp
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2011-01-01
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#include "ewxRange.h"

ewxRange::ewxRange()
{
    topRow = 0;
    bottomRow = 0;
    leftCol = 0;
    rightCol = 0;
}

ewxRange::ewxRange(int toprow, int leftcol, int bottomrow, int rightcol)
{
    topRow = toprow;
    bottomRow = bottomrow;
    leftCol = leftcol;
    rightCol = rightcol;
}

ewxRange::~ewxRange()
{
    //dtor
}

ewxRange::ewxRange(const ewxRange& other)
{
    topRow = other.topRow;
    bottomRow = other.bottomRow;
    leftCol = other.leftCol;
    rightCol = other.rightCol;
}

ewxRange& ewxRange::operator=(const ewxRange& rhs)
{
    if (this == &rhs) return *this; // handle self assignment

    topRow = rhs.topRow;
    bottomRow = rhs.bottomRow;
    leftCol = rhs.leftCol;
    rightCol = rhs.rightCol;

    return *this;
}

bool ewxRange::IsInRange(int col, int row)
{
    if ((col >= leftCol) && (col <= rightCol) &&
        (row >= topRow) && (row <= bottomRow))
        return true;
    else
        return false;
}
