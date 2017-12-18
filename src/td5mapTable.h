/***************************************************************
 * Name:      td5mapTable.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2011-01-12
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef TD5MAPTABLE_H
#define TD5MAPTABLE_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "td5mapTableInfo.h"
#include "ewxRange.h"
#include "ewxDynArray.h"

class td5mapTable : public wxObject
{
public:
	td5mapTable();
	virtual ~td5mapTable();

// Implementation
public:
	bool ReadTable(wxWord* pwMapFileData, int iIndex, wxWord* pwBaseMapFileData = NULL);
	bool WriteTable(wxWord* pwMapFileData);
	void EvalRange(int& min, int& max);
	void EvalDiffRange(int& min, int& max);
	int GetCols(){return m_cols;};
	int GetRows(){return m_rows;};
	int GetAddress(){return m_address;};
	wxString GetName(){return m_name;};
	int GetType(){return m_type;};
	int GetCurrentHeaderRowValue(int row) { return m_headerRow[row].current;};
	int GetCurrentHeaderColValue(int col) { return m_headerCol[col].current;};
	int GetCurrentValue(int col, int row) { return m_tableData[col][row].current;};
	int GetBaseValue(int col, int row) { return m_tableData[col][row].base;};
	int GetDiffValue(int col, int row) { return m_tableData[col][row].current - m_tableData[col][row].base;};
	void SetCurrentValue(int value, int col, int row) { m_tableData[col][row].current = value;};
	void SetBaseValue(int value, int col, int row) { m_tableData[col][row].base = value;};
	void SumCurrentValue(int sum, int col, int row);
	void SumPercentCurrentValue(double sumpercent, int col, int row);
    bool IsRecognized(){return m_recognized;};
	bool IsTridimensional(){return m_map3d;};
	bool IsBidimensional(){return !m_map3d;};
	bool IsSingleValue(){return m_singlevalue;};
	bool IsDifferentFromOriginal();

	void SetColLabelSizers(double mult, int off) {m_collabelmult = mult; m_collabeloff = off;};
	void SetRowLabelSizers(double mult, int off) {m_rowlabelmult = mult; m_rowlabeloff = off;};
	void SetDataSizers(double mult, int off) {m_datamult = mult; m_dataoff = off;};

    double ApplyColLabelSizer(int rawdata) {return apply_sizer(rawdata, m_collabelmult, m_collabeloff);};
    double ApplyRowLabelSizer(int rawdata) {return apply_sizer(rawdata, m_rowlabelmult, m_rowlabeloff);};
    double ApplyDataSizer(int rawdata) {return apply_sizer(rawdata, m_datamult, m_dataoff);};
	int RestoreColLabelRaw(double sizeddata) {return restore_raw(sizeddata, m_collabelmult, m_collabeloff);};
	int RestoreRowLabelRaw(double sizeddata) {return restore_raw(sizeddata, m_rowlabelmult, m_rowlabeloff);};
	int RestoreDataRaw(double sizeddata) {return restore_raw(sizeddata, m_datamult, m_dataoff);};

private:
    double apply_sizer(short raw, double mult, int offset) {return (double)((raw + offset) * mult);};
    short restore_raw(double sized, double mult, int offset);

public:
	wxUint32 m_index;
	wxUint32 m_mapID;
	bool m_map3d;
	bool m_recognized;
	int	m_cols;
	int	m_rows;
	wxWord m_address;

	int m_type;
	wxString m_name;
	wxString m_comment;
	wxString m_xunit;
	wxString m_yunit;
	wxString m_zunit;
	double m_collabelmult;
	int m_collabeloff;
	double m_rowlabelmult;
	int m_rowlabeloff;
   	double m_datamult;
	int m_dataoff;

	/*
	struct COLUMN_HEADERS {
		int base;
		int	current;
		int	external;
	};

	struct ROW_HEADERS {
		int base;
		int	current;
		int	external;
	};

	struct TABLE_DATA{
		int base;
		int	current;
		int	external;
	};
	*/
	struct COLUMN_HEADERS {
		wxWord base;
		wxWord current;
		wxWord external;
	};

	struct ROW_HEADERS {
		wxWord base;
		wxWord current;
		wxWord external;
	};

	struct TABLE_DATA{
		wxWord base;
		wxWord current;
		wxWord external;
	};

    ewxDynArray<COLUMN_HEADERS> m_headerCol;
    ewxDynArray<ROW_HEADERS> m_headerRow;
	ewxDyn2DArray<TABLE_DATA> m_tableData;

	bool m_collabelsized;
	bool m_rowlabelsized;
	bool m_datasized;
	bool m_singlevalue;
};

#define FUEL_MAP_BEGIN_ADDRESS 102416

#endif // TD5MAPTABLE_H
