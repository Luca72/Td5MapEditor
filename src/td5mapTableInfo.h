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

		return *this;
	}

public:
	td5mapTableInfoItem();
	td5mapTableInfoItem(int iType, int iIndex, wxString sXUnit, wxString sYUnit, wxString sZUnit, wxString sName, wxString sComment);
	td5mapTableInfoItem(const td5mapTableInfoItem& pmti);

	virtual ~td5mapTableInfoItem();

public:
    int m_type;
    int m_index;
	wxString m_xunit;
	wxString m_yunit;
	wxString m_zunit;
	wxString m_name;
	wxString m_comment;
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

#define SCALAR_TWG_PWM_FREQUENCY       200
#define SCALAR_MAP_MAX_LIMIT           201
#define SCALAR_TURBO_MAX_LIMIT  	   202
#define SCALAR_TURBO_RECOVERY          203


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
