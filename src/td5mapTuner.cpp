/***************************************************************
 * Name:      td5mapTuner.cpp
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2011-01-12
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#include "td5mapTuner.h"
#include "functions.h"

td5mapTuner::td5mapTuner()
{
    //ctor
}

td5mapTuner::td5mapTuner(td5mapTable *table)
{
    m_table = table;
    m_cols = table->GetCols();
    m_rows = table->GetRows();
}

td5mapTuner::~td5mapTuner()
{
    //dtor
}

ewxDynDoubleArray td5mapTuner::CreateTuneTable(ewxRange range, tuneData data)
{
    ewxDynDoubleArray tuneTable(m_cols, m_rows);
    double factorTop = 0.0;
    double factorBottom = 0.0;
    double factorCol = 0.0;

    for (int c = range.leftCol; c <= min(range.rightCol, m_cols - 1); c++)
        for (int r = range.topRow; r <= min(range.bottomRow, m_rows - 1); r++)
        {
            tuneTable[c][r] = 0;
        }

    if((range.rightCol - range.leftCol) > 0)
    {
        for (int c = range.leftCol; c <= min(range.rightCol, m_cols - 1) ; c++)
        {
            factorTop = (double)(data.rtValue - data.ltValue) / (double) (range.rightCol - range.leftCol);
            tuneTable[c][range.topRow] = ((factorTop * (double) (c - range.leftCol)) + data.ltValue);

            if((range.bottomRow - range.topRow) > 0)
            {
                factorBottom = (double)(data.rbValue - data.lbValue) / (double) (range.rightCol - range.leftCol);
                tuneTable[c][range.bottomRow] = ((factorBottom * (double) (c - range.leftCol)) + data.lbValue);
            }
        }
    }
    else
    {
        tuneTable[range.leftCol][range.topRow] = (double)(data.ltValue);

        if((range.bottomRow - range.topRow) > 0)
        {
            tuneTable[range.leftCol][range.bottomRow] = (double)(data.lbValue);
        }
    }


    if((range.bottomRow - range.topRow) > 0)
    {
        for (int c = range.leftCol; c <= min(range.rightCol, m_cols - 1); c++)
            for (int r = range.topRow; r <= min(range.bottomRow, m_rows - 1); r++)
            {
                factorCol = (double)(tuneTable[c][range.bottomRow] - tuneTable[c][range.topRow]) / (double) (range.bottomRow - range.topRow);
                tuneTable[c][r] = (factorCol * (double) (r - range.topRow)) + tuneTable[c][range.topRow];
            }
    }

    for (int c = range.leftCol; c <= min(range.rightCol, m_cols - 1); c++)
        for (int r = range.topRow; r <= min(range.bottomRow, m_rows - 1); r++)
        {
            tuneTable[c][r] = tuneTable[c][r] + data.constValue;
        }

    return tuneTable;
}

bool td5mapTuner::DoPlaneTuning(ewxRange range, tuneData data)
{
    ewxDynDoubleArray tuneTable = CreateTuneTable(range, data);

    for (int c = range.leftCol; c <= min(range.rightCol, m_cols - 1); c++)
        for (int r = range.topRow; r <= min(range.bottomRow, m_rows - 1); r++)
        {
            m_table->SumCurrentValue(tuneTable[c][r], c, r);
        }

    return true;
}

bool td5mapTuner::DoPercentTuning(ewxRange range, tuneData data)
{
    ewxDynDoubleArray tuneTable = CreateTuneTable(range, data);

    for (int c = range.leftCol; c <= min(range.rightCol, m_cols - 1); c++)
        for (int r = range.topRow; r <= min(range.bottomRow, m_rows - 1); r++)
        {
            m_table->SumPercentCurrentValue(tuneTable[c][r], c, r);
        }

    return true;
}

bool td5mapTuner::DoTableTuning(ewxRange range, ewxDynDoubleArray tuneTable)
{
    for (int c = range.leftCol; c <= min(range.rightCol, m_cols - 1); c++)
        for (int r = range.topRow; r <= min(range.bottomRow, m_rows - 1); r++)
        {
            m_table->SumCurrentValue(tuneTable[c][r], c, r);
        }

    return true;
}
