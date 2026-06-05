// Td5MapInfo.h: interface for the td5mapTableInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TD5MAPINFO_H__8640B121_D04E_488F_9609_A9ADF478C930__INCLUDED_)
#define AFX_TD5MAPINFO_H__8640B121_D04E_488F_9609_A9ADF478C930__INCLUDED_

#include <wx/dynarray.h>


class td5mapTableInfoItem : public wxObject
{
	DECLARE_DYNAMIC_CLASS(td5mapTableInfoItem)
public:
	td5mapTableInfoItem& operator=( const td5mapTableInfoItem &s )  // assignment operator
	{
	    m_type = s.m_type;
	    m_index = s.m_index;
		m_xunit = s.m_xunit;
		m_yunit = s.m_yunit;
		m_zunit = s.m_zunit;
		m_name = s.m_name;
		m_comment = s.m_comment;
        m_collabelmult = s.m_collabelmult;
        m_collabeloff = s.m_collabeloff;
        m_rowlabelmult = s.m_rowlabelmult;
        m_rowlabeloff = s.m_rowlabeloff;
        m_datamult = s.m_datamult;
        m_dataoff = s.m_dataoff;
		return *this;
	}

public:
	td5mapTableInfoItem();
	td5mapTableInfoItem(int iType, int iIndex, wxString sXUnit, wxString sYUnit, wxString sZUnit, wxString sName, wxString sComment);
	td5mapTableInfoItem(const td5mapTableInfoItem& pmti);
	void SetColLabelSizers(double mult, int off) {m_collabelmult = mult; m_collabeloff = off;};
	void SetRowLabelSizers(double mult, int off) {m_rowlabelmult = mult; m_rowlabeloff = off;};
	void SetDataSizers(double mult, int off) {m_datamult = mult; m_dataoff = off;};
	virtual ~td5mapTableInfoItem();

public:
    int m_type;
    int m_index;
	wxString m_xunit;
	wxString m_yunit;
	wxString m_zunit;
	wxString m_name;
	wxString m_comment;
	double m_collabelmult;
	int m_collabeloff;
	double m_rowlabelmult;
	int m_rowlabeloff;
   	double m_datamult;
	int m_dataoff;
};

#define TORQUE_LIM_HIGH_RANGE          100
#define TORQUE_LIM_LOW_RANGE           101
#define DRIVER_DEMAND_LOW_RANGE        102
#define DRIVER_DEMAND_HIGH_RANGE       103
#define SMOKE_LIM_LOW_RANGE            104
#define SMOKE_LIM_HIGH_RANGE_A         105
#define SMOKE_LIM_HIGH_RANGE_B         106
#define FUEL_MAP_1                     107
#define FUEL_MAP_2                     108
#define FUEL_MAP_3                     109
#define FUEL_MAP_4                     110
#define IDLE_SPEED_1                   111
#define IDLE_SPEED_2                   112
#define EGR_MAP                        113
#define FUEL_DENSITY_COMP_LOWER        114
#define FUEL_DENSITY_COMP_UPPER        115
#define COOLANT_TEMPERATURE            116
#define TIMINGS_MAP                    117
#define TWG_DUTY_CYCLE                 118
#define TWG_INTEGRAL_GAIN              119
#define TWG_PRESSURE_MAP               120
#define XDF_MAP                        199

#define SCALAR_TWG_PWM_FREQUENCY       200
#define SCALAR_MAP_MAX_LIMIT           201
#define SCALAR_TURBO_MAX_LIMIT  	   202
#define SCALAR_TURBO_RECOVERY          203
#define SCALAR_CC_MIN_SPEED            204
#define SCALAR_CC_MAX_SPEED            205
#define SCALAR_CC_INC_FOR_TAP          206
#define SCALAR_CC_MAX_ACC_RATE         207
#define SCALAR_CC_MAX_DEC_RATE         208
#define SCALAR_CC_TAPS_LIMIT           209
#define XDF_SCALAR                     299

#define UNKNOWN_MAP_1                  900
#define UNKNOWN_MAP_2                  901
#define UNKNOWN_MAP_3                  902
#define UNKNOWN_MAP_4                  903
#define UNKNOWN_MAP_5                  904



WX_DECLARE_OBJARRAY(td5mapTableInfoItem, arrayofTableInfoItem);

class td5mapTableInfo : public wxObject
{
public:
	td5mapTableInfo(wxUint32 nID);
	virtual ~td5mapTableInfo();

// Implementation
public:
	bool GetInfoFromIndex(td5mapTableInfoItem& ti, int iIndex);

	arrayofTableInfoItem m_mapInfo;
};

#endif // !defined(AFX_TD5MAPINFO_H__8640B121_D04E_488F_9609_A9ADF478C930__INCLUDED_)
