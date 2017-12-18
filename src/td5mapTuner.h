/***************************************************************
 * Name:      td5mapTuner.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2011-01-12
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef TD5MAPTUNER_H
#define TD5MAPTUNER_H

#include <vector>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "td5mapTable.h"
#include "ewxRange.h"
#include "ewxDynArray.h"

#define ewxDynIntArray ewxDyn2DArray<int>
#define ewxDynDoubleArray ewxDyn2DArray<double>

class tuneData;

class td5mapTuner
{
    public:
        td5mapTuner();
        td5mapTuner(td5mapTable *table);
        virtual ~td5mapTuner();
        td5mapTable GetTable() { return *m_table; }
        void SetTable(td5mapTable *table) { m_table = table; }

        bool DoPlaneTuning(ewxRange range, tuneData data);
        bool DoPercentTuning(ewxRange range, tuneData data);
        bool DoTableTuning(ewxRange range, ewxDynDoubleArray tuneTable);

    protected:
        ewxDynDoubleArray CreateTuneTable(ewxRange range, tuneData data);

    private:
        td5mapTable *m_table;
        int m_cols;
        int m_rows;
};

class tuneData : public wxObject
{
    public:
        tuneData()
        {
            constValue = 0;
            rtValue = 0;
            ltValue = 0;
            rbValue = 0;
            lbValue = 0;
        };
        tuneData(int constant, int lefttop = 0, int righttop = 0, int leftbottom = 0, int rightbottom = 0)
        {
            constValue = constant;
            rtValue = righttop;
            ltValue = lefttop;
            rbValue = rightbottom;
            lbValue = leftbottom;
        };
        virtual ~tuneData(){};
        tuneData(const tuneData& other)
        {
            constValue = other.constValue;
            rtValue = other.rtValue;
            ltValue = other.ltValue;
            rbValue = other.rbValue;
            lbValue = other.lbValue;
        };
        tuneData& operator=(const tuneData& other)
        {
            if (this == &other) return *this; // handle self assignment

            constValue = other.constValue;
            rtValue = other.rtValue;
            ltValue = other.ltValue;
            rbValue = other.rbValue;
            lbValue = other.lbValue;

            return *this;
        };

    public:
        int ltValue;
        int rtValue;
        int lbValue;
        int rbValue;
        int constValue;

    protected:
    private:
};


#endif // TD5MAPTUNER_H
