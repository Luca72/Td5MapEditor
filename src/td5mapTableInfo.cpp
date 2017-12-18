// Td5MapInfo.cpp: implementation of the td5mapTableInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "td5mapeditorApp.h"
#include "td5mapTableInfo.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(arrayofTableInfoItem);

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// td5mapTableInfoItem

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_CLASS(td5mapTableInfoItem, wxObject)

td5mapTableInfoItem::td5mapTableInfoItem()
{
    m_type = 0;
    m_index = 0;
	m_xunit = wxT("");
	m_yunit = wxT("");
	m_zunit = wxT("");
	m_name = wxT("");
	m_comment = wxT("");
    m_collabelmult = 1.0;
    m_collabeloff = 0;
    m_rowlabelmult = 1.0;
    m_rowlabeloff = 0;
    m_datamult = 1.0;
    m_dataoff = 0;
}

td5mapTableInfoItem::td5mapTableInfoItem(int iType, int iIndex, wxString sXUnit, wxString sYUnit, wxString sZUnit, wxString sName, wxString sComment)
{
    m_type = iType;
    m_index = iIndex;
	m_xunit = sXUnit;
	m_yunit = sYUnit;
	m_zunit = sZUnit;
	m_name = sName;
	m_comment = sComment;
    m_collabelmult = 1.0;
    m_collabeloff = 0;
    m_rowlabelmult = 1.0;
    m_rowlabeloff = 0;
    m_datamult = 1.0;
    m_dataoff = 0;
}

td5mapTableInfoItem::td5mapTableInfoItem(const td5mapTableInfoItem& pmti)
{
    m_type = pmti.m_type;
    m_index = pmti.m_index;
	m_xunit = pmti.m_xunit;
	m_yunit = pmti.m_yunit;
	m_zunit = pmti.m_zunit;
	m_name = pmti.m_name;
	m_comment = pmti.m_comment;
    m_collabelmult = pmti.m_collabelmult;
    m_collabeloff = pmti.m_collabeloff;
    m_rowlabelmult = pmti.m_rowlabelmult;
    m_rowlabeloff = pmti.m_rowlabeloff;
    m_datamult = pmti.m_datamult;
    m_dataoff = pmti.m_dataoff;
}

td5mapTableInfoItem::~td5mapTableInfoItem()
{

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

td5mapTableInfo::td5mapTableInfo(wxUint32 nID)
{
	td5mapTableInfoItem mtiTorqueLimHighRange(
        TORQUE_LIM_HIGH_RANGE,
		17,
		_T("RPM"),
		_T("Inject Quantity mg/stroke"),
		_T(""),
		_T("TORQUE LIMITER HIGH RANGE"),
		_T("Torque limiter table for HIGH range.\n\ndemand->smoke lim.->TORQUE LIM.->inj. duration\n\nX Axis: RPM\nY Axis: mg/stroke\n"));
    mtiTorqueLimHighRange.SetDataSizers(0.01, 0);


	td5mapTableInfoItem mtiTorqueLimLowRange(
        TORQUE_LIM_LOW_RANGE,
		18,
		_T("RPM"),
		_T("Inject Quantity mg/stroke"),
		_T(""),
		_T("TORQUE LIMITER LOW RANGE"),
		_T("Torque limiter table for LOW range.\n\ndemand->smoke lim.->TORQUE LIM.->inj. duration\n\nX Axis: RPM\nY Axis: mg/stroke\n"));
    mtiTorqueLimLowRange.SetDataSizers(0.01, 0);

	td5mapTableInfoItem mtiDriverDemandHighRange(
		DRIVER_DEMAND_HIGH_RANGE,
		69,
		_T("Throttle ref. %"),
		_T("RPM"),
		_T("Inject Quantity (mg/stroke)"),
		_T("DRIVER DEMAND HIGH RANGE"),
		_T("Driver demand table for HIGH range.\n\nDEMAND->smoke lim.->torque lim.->inj. duration\n\nX Axis: RPM\nY Axis: mg/stroke\nZ Axis: %"));
    mtiDriverDemandHighRange.SetColLabelSizers(0.01, 0);
    mtiDriverDemandHighRange.SetDataSizers(0.01, 0);

	td5mapTableInfoItem mtiDriverDemandLowRange(
		DRIVER_DEMAND_LOW_RANGE,
		70,
		_T("Throttle ref. %"),
		_T("RPM"),
		_T("Inject Quantity (mg/stroke)"),
		_T("DRIVER DEMAND LOW RANGE"),
		_T("Driver demand table for LOW range.\n\nDEMAND->smoke lim.->torque lim.->inj. duration\n\nX Axis: RPM\nY Axis: mg/stroke\nZ Axis: %"));
    mtiDriverDemandLowRange.SetColLabelSizers(0.01, 0);
    mtiDriverDemandLowRange.SetDataSizers(0.01, 0);

	td5mapTableInfoItem mtiSmokeLimLowRange(
		SMOKE_LIM_LOW_RANGE,
		0,
		_T("Airmass (mg/stroke)"),
		_T("RPM"),
		_T("Limited Inj. (mg/stroke)"),
		_T("SMOKE LIMITER MAP LOW RANGE"),
		_T("Smoke limiter map for low range.\n\ndemand->SMOKE LIM.->torque lim.->inj. duration\n\nX Axis: Airmass (mg/stroke)\nY Axis: RPM\nZ Axis: Limited Inj. (mg/stroke)"));
    mtiSmokeLimLowRange.SetColLabelSizers(0.1, 0);
    mtiSmokeLimLowRange.SetDataSizers(0.01, 0);

	td5mapTableInfoItem mtiSmokeLimHighRangeA(
		SMOKE_LIM_HIGH_RANGE_A,
		60,
		_T("Airmass (mg/stroke)"),
		_T("RPM"),
		_T("Limited Inj. (mg/stroke)"),
		_T("SMOKE LIM. MAP HIGH RANGE"),
		_T("Smoke limiter map for high range.\n\ndemand->SMOKE LIM.->torque lim.->inj. duration\n\nX Axis: Airmass (mg/stroke)\nY Axis: RPM\nZ Axis: Limited Inj. (mg/stroke)"));
    mtiSmokeLimHighRangeA.SetColLabelSizers(0.1, 0);
    mtiSmokeLimHighRangeA.SetDataSizers(0.01, 0);

	td5mapTableInfoItem mtiSmokeLimHighRangeB(
		SMOKE_LIM_HIGH_RANGE_B,
		0,
		_T("Airmass (mg/stroke)"),
		_T("RPM"),
		_T("Limited Inj. (mg/stroke)"),
		_T("SMOKE LIM. MAP HIGH RANGE (3 sec.)"),
		_T("Smoke limiter map for high range (temp. - 3 sec. - after full throttle).\n\ndemand->SMOKE LIM.->torque lim.->inj. duration\n\nX Axis: Airmass (mg/stroke)\nY Axis: RPM\nZ Axis: Limited Inj. (mg/stroke)"));
    mtiSmokeLimHighRangeB.SetColLabelSizers(0.1, 0);
    mtiSmokeLimHighRangeB.SetDataSizers(0.01, 0);

	td5mapTableInfoItem mtiFuelMap1(
		FUEL_MAP_1,
		0,
		_T("Requested IQ (mg/stroke)"),
		_T("RPM"),
		_T("microsec."),
		_T("FUEL MAP (0 deg. advance)"),
		_T("Fuel map. Manages injection timing in function of engine speed and reference from throttle.\n\ndemand->smoke lim.->torque lim.->INJ. DURATION\n\nX Axis: Demand (mg/stroke)\nY Axis: RPMs\nZ Axis: Time (microsec.)"));
    mtiFuelMap1.SetColLabelSizers(0.01, 0);

	td5mapTableInfoItem mtiFuelMap2(
		FUEL_MAP_2,
		0,
		_T("Requested IQ (mg/stroke)"),
		_T("RPM"),
		_T("microsec."),
		_T("FUEL MAP (5 deg. advance)"),
		_T("Fuel map. Manages injection timing in function of engine speed and reference from throttle.\n\ndemand->smoke lim.->torque lim.->INJ. DURATION\n\nX Axis: Demand (mg/stroke)\nY Axis: RPMs\nZ Axis: Time (microsec.)"));
    mtiFuelMap2.SetColLabelSizers(0.01, 0);

	td5mapTableInfoItem mtiFuelMap3(
		FUEL_MAP_3,
		0,
		_T("Requested IQ (mg/stroke)"),
		_T("RPM"),
		_T("microsec."),
		_T("FUEL MAP (10 deg. advance)"),
		_T("Fuel map. Manages injection timing in function of engine speed and reference from throttle.\n\ndemand->smoke lim.->torque lim.->INJ. DURATION\n\nX Axis: Demand (mg/stroke)\nY Axis: RPMs\nZ Axis: Time (microsec.)"));
    mtiFuelMap3.SetColLabelSizers(0.01, 0);

	td5mapTableInfoItem mtiFuelMap4(
		FUEL_MAP_4,
		0,
		_T("Requested IQ (mg/stroke)"),
		_T("RPM"),
		_T("microsec."),
		_T("FUEL MAP (25 deg. advance)"),
		_T("Fuel map. Manages injection timing in function of engine speed and reference from throttle.\n\ndemand->smoke lim.->torque lim.->INJ. DURATION\n\nX Axis: Demand (mg/stroke)\nY Axis: RPMs\nZ Axis: Time (microsec.)"));
    mtiFuelMap4.SetColLabelSizers(0.01, 0);

	td5mapTableInfoItem mtiFuelDensityCompLower(
		FUEL_DENSITY_COMP_LOWER,
		0,
		_T("?"),
		_T("Fuel Temp"),
		_T("Inject Quantity (mg/stroke)"),
		_T("FUEL DENSITY COMPENSATION LOWER"),
		_T("Fuel density compensation lower table\n\nX Axis: ?\nY Axis: Fuel temp.\nZ Axis: mg/stroke"));
    mtiFuelDensityCompLower.SetColLabelSizers(0.01, 0);
    mtiFuelDensityCompLower.SetRowLabelSizers(0.1, -2732);
    mtiFuelDensityCompLower.SetDataSizers(0.01, 0);

	td5mapTableInfoItem mtiFuelDensityCompUpper(
		FUEL_DENSITY_COMP_UPPER,
		0,
		_T("?"),
		_T("Fuel Temp"),
		_T("Inject Quantity (mg/stroke)"),
		_T("FUEL DENSITY COMPENSATION UPPER"),
		_T("Fuel density compensation upper table\n\nX Axis: ?\nY Axis: Fuel temp.\nZ Axis: mg/stroke"));
    mtiFuelDensityCompUpper.SetColLabelSizers(0.01, 0);
    mtiFuelDensityCompUpper.SetRowLabelSizers(0.1, -2732);
    mtiFuelDensityCompUpper.SetDataSizers(0.01, 0);

	td5mapTableInfoItem mtiIdleSpeed1(
		IDLE_SPEED_1,
		0,
		_T("Ambient Temp (deg.)"),
		_T("RPM"),
		_T(""),
		_T("IDLE SPEED 1"),
		_T("Idle speed map. Manages the idle speed. Both Idle Speed tables must be modified.\n\nX Axis: Ambient Temp.(deg.)\nY Axis: RPMs"));
    mtiIdleSpeed1.SetColLabelSizers(0.1, -2732);

	td5mapTableInfoItem mtiIdleSpeed2(
		IDLE_SPEED_2,
		0,
		_T("Ambient Temp (deg.)"),
		_T("RPM"),
		_T(""),
		_T("IDLE SPEED 2"),
		_T("Idle speed map. Manages the idle speed. Both Idle Speed tables must be modified.\n\nX Axis: Ambient Temp.(deg.)\nY Axis: RPMs"));
    mtiIdleSpeed2.SetColLabelSizers(0.1, -2732);

	td5mapTableInfoItem mtiEgrMap(
		EGR_MAP,
		0,
		_T("RPM"),
		_T("?"),
		_T("?"),
		_T("EGR MAP"),
		_T("EGR map. Manages the EGR valve. To exclude it you must set all the values to 16000 (decimal).\n\nX Axis: ?\nY Axis: ?\nZ Axis: ?"));

    td5mapTableInfoItem mtiCoolantTemp(
		COOLANT_TEMPERATURE,
		0,
		_T("Sensor (mV)"),
		_T("Battery (V)"),
		_T("Temp (deg)"),
		_T("COOLANT TEMPERATURE SENSOR"),
		_T("Coolant temperature sensor table.\n\nX Axis: Temperature (Celsius deg.)\nY Axis: Battery (v)\nZ Axis: Sensor ref. (mV)"));
    mtiCoolantTemp.SetColLabelSizers(0.1, -2732);
    mtiCoolantTemp.SetRowLabelSizers(0.001, 0);

    td5mapTableInfoItem mtiTimingsMap(
		TIMINGS_MAP,
		0,
		_T("RPM"),
		_T("Requested IQ (mg/stroke)"),
		_T("Timing (deg)"),
		_T("TIMINGS MAP"),
		_T("Timings table.\n\nX Axis: RPM\nY Axis: mg/stroke\nZ Axis: deg"));

    td5mapTableInfoItem mtiTwgDutyCycle(
		TWG_DUTY_CYCLE,
		0,
		_T("RPM"),
		_T("Boost (kPa)"),
		_T("Duty Cycle"),
		_T("TWG DUTY CYCLE"),
		_T("TWG Duty Cycle.\n\nX Axis: RPM\nY Axis: kPa\nZ Axis: D.C."));

	td5mapTableInfoItem mtiTwgIntegralGain(
		TWG_INTEGRAL_GAIN,
		0,
		_T("RPM"),
		_T("RPM"),
		_T("Integral Gain"),
		_T("TWG INTEGRAL GAIN"),
		_T("TWG Integral gain.\n\nX Axis: Boost\nY Axis: RPM\nZ Axis: I.G."));

    td5mapTableInfoItem mtiTwgPressureMap(
		TWG_PRESSURE_MAP,
		0,
		_T("RPM"),
		_T("Requested IQ (mg/stroke)"),
		_T("Boost (kPa)"),
		_T("TWG PRESSURE MAP"),
		_T("Twg pressure table.\n\nX Axis: RPM\nY Axis: mg/stroke\nZ Axis: kPa"));


	td5mapTableInfoItem mtiUnknownMap1(
		UNKNOWN_MAP_1,
		0,
		_T("?"),
		_T("?"),
		_T("?"),
		_T("UNKNOWN MAP 1"),
		_T("Unknown.\n\nX Axis: ?\nY Axis: ?\nZ Axis: ?"));

	td5mapTableInfoItem mtiUnknownMap2(
		UNKNOWN_MAP_2,
		0,
		_T("?"),
		_T("?"),
		_T("?"),
		_T("UNKNOWN MAP 2"),
		_T("Unknown.\n\nX Axis: ?\nY Axis: ?\nZ Axis: ?"));

	td5mapTableInfoItem mtiUnknownMap3(
        UNKNOWN_MAP_3,
		54,
		_T("?"),
		_T("?"),
		_T(""),
		_T("UNKNOWN MAP 3"),
		_T("Unknown (Increments improve response).\n\nX Axis: ?\nY Axis: ?\nZ Axis: ?"));
    mtiUnknownMap3.SetRowLabelSizers(0.1, -2732);

	td5mapTableInfoItem mtiUnknownMap4(
		UNKNOWN_MAP_4,
		0,
		_T("?"),
		_T("?"),
		_T("?"),
		_T("UNKNOWN MAP 4"),
		_T("Unknown.\n\nX Axis: ?\nY Axis: ?\nZ Axis: ?"));

	td5mapTableInfoItem mtiUnknownMap5(
		UNKNOWN_MAP_5,
		0,
		_T("?"),
		_T("?"),
		_T("?"),
		_T("UNKNOWN MAP 5"),
		_T("Unknown.\n\nX Axis: ?\nY Axis: ?\nZ Axis: ?"));


    td5mapTableInfoItem mtiScalarTwgPwmFreq(
		SCALAR_TWG_PWM_FREQUENCY,
		129,
		_T(""),
		_T(""),
		_T(""),
		_T("TWG PWM FREQUENCY SCALER"),
		_T("Frequency scaler (16=ENABLE/0=DISABLE)"));

    td5mapTableInfoItem mtiScalarMapMaxLimit(
		SCALAR_MAP_MAX_LIMIT,
		145,
		_T(""),
		_T(""),
		_T(""),
		_T("MAP MAX LIMIT"),
		_T("MAP sensor scaler (max limit)"));

    td5mapTableInfoItem mtiScalarTurboOverboostMax(
		SCALAR_TURBO_MAX_LIMIT,
		133,
		_T("kPa"),
		_T(""),
		_T(""),
		_T("TURBO OVERBOOST MAX"),
		_T("Turbo overboost max pressure (kPa)"));
    mtiScalarTurboOverboostMax.SetDataSizers(0.01, 0);

    td5mapTableInfoItem mtiScalarTurboOverboostRecovery(
		SCALAR_TURBO_RECOVERY,
		134,
		_T("kPa"),
		_T(""),
		_T(""),
		_T("TURBO OVERBOOST RECOVERY"),
		_T("Turbo overboost recovery pressure (kPa)"));
    mtiScalarTurboOverboostRecovery.SetDataSizers(0.01, 0);


    td5mapTableInfoItem mtiScalarCruiseControlMinSpeed(
		SCALAR_CC_MIN_SPEED,
		168,
		_T("Km/h"),
		_T(""),
		_T(""),
		_T("CRUISE CONTROL MIN SPEED"),
		_T("Cruise Control lower speed limit (Km/h)"));
    mtiScalarCruiseControlMinSpeed.SetDataSizers(0.1, 0);

    td5mapTableInfoItem mtiScalarCruiseControlMaxSpeed(
		SCALAR_CC_MAX_SPEED,
		169,
		_T("Km/h"),
		_T(""),
		_T(""),
		_T("CRUISE CONTROL MAX SPEED"),
		_T("Cruise Control upper speed limit (Km/h)"));
    mtiScalarCruiseControlMaxSpeed.SetDataSizers(0.1, 0);

    td5mapTableInfoItem mtiScalarCruiseControlIncrementForTap(
		SCALAR_CC_INC_FOR_TAP,
		170,
		_T("Km/h"),
		_T(""),
		_T(""),
		_T("CRUISE CONTROL INC. FOR TAP"),
		_T("Cruise Control speed increment for each tap (Km/h)"));
    mtiScalarCruiseControlIncrementForTap.SetDataSizers(0.1, 0);

    td5mapTableInfoItem mtiScalarCruiseControlMaxAccelerationRate(
		SCALAR_CC_MAX_ACC_RATE,
		172,
		_T("Km/h/sec"),
		_T(""),
		_T(""),
		_T("CRUISE CONTROL MAX ACCELERATION RATE"),
		_T("Cruise Control max acceleration rate (Km/h/sec)"));
    mtiScalarCruiseControlMaxAccelerationRate.SetDataSizers(0.01, 0);

    td5mapTableInfoItem mtiScalarCruiseControlMaxDecelerationRate(
		SCALAR_CC_MAX_DEC_RATE,
		173,
		_T("Km/h/sec"),
		_T(""),
		_T(""),
		_T("CRUISE CONTROL MAX DECELERATION RATE"),
		_T("Cruise Control max deceleration rate (Km/h/sec)"));
    mtiScalarCruiseControlMaxDecelerationRate.SetDataSizers(0.01, 0);

    td5mapTableInfoItem mtiScalarCruiseControlTapsLimit(
		SCALAR_CC_TAPS_LIMIT,
		174,
		_T("taps"),
		_T(""),
		_T(""),
		_T("CRUISE CONTROL MAX UP TAPS"),
		_T("Cruise Control number of max taps for up speed (taps)"));




	switch(nID)
	{
		case IDR_SVDXE003SVTNP003_MAP:
		case IDR_SVDXE004SVTNP003_MAP:
		case IDR_SVDXE006SVTNP003_MAP:
		case IDR_SVDXE008SVTNP005_MAP:
		case IDR_SVDXE008SVTNP006_MAP:
		case IDR_SVDXG003SVTNP006_MAP:
		case IDR_SVDXR002SVTNP003_MAP:
		case IDR_SVDXR005SVTNP003_MAP:
		case IDR_SVDXR007SVTNP005_MAP:
		case IDR_SVDXR007SVTNP006_MAP:
		case IDR_SVLNE004SVTNP003_MAP:
		case IDR_SVLNE006SVTNP003_MAP:
		case IDR_SVLNE007SVTNP005_MAP:
		case IDR_SVLNE007SVTNP006_MAP:
		case IDR_SVLNR002SVTNP003_MAP:
		case IDR_SVLNR004SVTNP003_MAP:
		case IDR_SVLNR005SVTNP005_MAP:
		case IDR_SVLNR005SVTNP006_MAP:
		case IDR_SVLOE002SVTNP003_MAP:
		case IDR_SVLOE004SVTNP003_MAP:
		case IDR_SVLOE005SVTNP005_MAP:
		case IDR_SVLOE005SVTNP006_MAP:
		case IDR_SVLOJ002SVTNP003_MAP:
		case IDR_SVLOJ002SVTNP006_MAP:
		case IDR_SVLOR002SVTNP003_MAP:
		case IDR_SVLOR004SVTNP003_MAP:
		case IDR_SVLOR005SVTNP005_MAP:
		case IDR_SVLOR005SVTNP006_MAP:
		case IDR_SWDXE004SWTNP004_MAP:
		case IDR_SWDXE007SWTNP004_MAP:
		case IDR_SWDXE007SWTNP006_MAP:
		case IDR_SWDXK001SWTNP004_MAP:
		case IDR_SWDXK003SWTNP004_MAP:
		case IDR_SWDXK003SWTNP006_MAP:
		case IDR_SWDXR002SWTNP004_MAP:
		case IDR_SWDXR004SWTNP004_MAP:
		case IDR_SWDXR004SWTNP006_MAP:
            mtiTorqueLimHighRange.m_index = 17;
            mtiTorqueLimLowRange.m_index = 18;
            mtiUnknownMap3.m_index = 54;
            mtiSmokeLimHighRangeA.m_index = 60;
            mtiSmokeLimHighRangeB.m_index = 61;
            mtiDriverDemandHighRange.m_index = 69;
            mtiDriverDemandLowRange.m_index = 70;
            mtiFuelDensityCompLower.m_index = 96;
            mtiFuelDensityCompUpper.m_index = 97;
            mtiFuelMap1.m_index = 98;
            mtiFuelMap2.m_index = 99;
            mtiFuelMap3.m_index = 100;
            mtiFuelMap4.m_index = 101;
            mtiIdleSpeed1.m_index = 40;
            mtiIdleSpeed2.m_index = 41;
            mtiEgrMap.m_index = 73;
            mtiCoolantTemp.m_index = 112;
            mtiUnknownMap1.m_index = 16;
            mtiUnknownMap2.m_index = 57;
            mtiSmokeLimLowRange.m_index = 59;
            mtiUnknownMap4.m_index = 90;
            mtiUnknownMap5.m_index = 94;
            mtiTimingsMap.m_index = 64;
            mtiTwgDutyCycle.m_index = 110;
            mtiTwgIntegralGain.m_index = 111;
            mtiTwgPressureMap.m_index = 115;

            mtiScalarTwgPwmFreq.m_index = 129;
            mtiScalarMapMaxLimit.m_index = 145;
            mtiScalarTurboOverboostMax.m_index = 133;
            mtiScalarTurboOverboostRecovery.m_index = 134;
            mtiScalarCruiseControlMinSpeed.m_index = 168;
            mtiScalarCruiseControlMaxSpeed.m_index = 169;
            mtiScalarCruiseControlIncrementForTap.m_index = 170;
            mtiScalarCruiseControlMaxAccelerationRate.m_index = 172;
            mtiScalarCruiseControlMaxDecelerationRate.m_index = 173;
            mtiScalarCruiseControlTapsLimit.m_index = 174;
			break;

        case IDR_STHDE021STTDP009_MAP:
        case IDR_STHDE021STTDP010_MAP:
        case IDR_STHDR009STTDP009_MAP:
        case IDR_STHDR009STTDP010_MAP:
        case IDR_SUHDE036SUTDP012_MAP:
        case IDR_SUHDE036SUTDP014_MAP:
        case IDR_SUHDR009SUTZP004_MAP:
        case IDR_SUHDR009SUTZP005_MAP:
        case IDR_SURDK004SUTZP004_MAP:
        case IDR_SURDK004SUTZP005_MAP:
            mtiTorqueLimHighRange.m_index = 255; // fake
            mtiTorqueLimLowRange.m_index = 8;
			mtiUnknownMap3.m_index = 255; // fake
			mtiSmokeLimHighRangeA.m_index = 42;
			mtiSmokeLimHighRangeB.m_index = 255; // fake
			mtiDriverDemandHighRange.m_index = 53;
			mtiDriverDemandLowRange.m_index = 54;
            mtiFuelDensityCompLower.m_index = 62;
            mtiFuelDensityCompUpper.m_index = 63;
			mtiFuelMap1.m_index = 64;
			mtiFuelMap2.m_index = 65;
			mtiFuelMap3.m_index = 66;
			mtiFuelMap4.m_index = 255; // fake
			mtiIdleSpeed1.m_index = 23;
			mtiIdleSpeed2.m_index = 24;
			mtiEgrMap.m_index = 56;
			mtiCoolantTemp.m_index = 75; // fake
            mtiUnknownMap1.m_index = 255; // fake
            mtiUnknownMap2.m_index = 255; // fake
            mtiSmokeLimLowRange.m_index = 255; // fake
            mtiUnknownMap4.m_index = 255; // fake
            mtiUnknownMap5.m_index = 255; // fake
            mtiTimingsMap.m_index = 255; // fake
            mtiTwgDutyCycle.m_index = 73;
            mtiTwgIntegralGain.m_index = 74;
            mtiTwgPressureMap.m_index = 78;

            mtiScalarTwgPwmFreq.m_index = 92;
            mtiScalarMapMaxLimit.m_index = 108;
            mtiScalarTurboOverboostMax.m_index = 96;
            mtiScalarTurboOverboostRecovery.m_index = 97;
            mtiScalarCruiseControlMinSpeed.m_index = 255; // fake
            mtiScalarCruiseControlMaxSpeed.m_index = 255; // fake
            mtiScalarCruiseControlIncrementForTap.m_index = 255; // fake
            mtiScalarCruiseControlMaxAccelerationRate.m_index = 255; // fake
            mtiScalarCruiseControlMaxDecelerationRate.m_index = 255; // fake
            mtiScalarCruiseControlTapsLimit.m_index = 255; // fake
			break;

		case IDR_STHLE022STTLP009_MAP:
		case IDR_STHLE022STTLP010_MAP:
            mtiTorqueLimHighRange.m_index = 255; // fake
            mtiTorqueLimLowRange.m_index = 9;
			mtiUnknownMap3.m_index = 255; // fake
			mtiSmokeLimHighRangeA.m_index = 41;
			mtiSmokeLimHighRangeB.m_index = 255; // fake
			mtiDriverDemandHighRange.m_index = 52;
			mtiDriverDemandLowRange.m_index = 53;
            mtiFuelDensityCompLower.m_index = 62;
            mtiFuelDensityCompUpper.m_index = 63;
			mtiFuelMap1.m_index = 63;
			mtiFuelMap2.m_index = 64;
			mtiFuelMap3.m_index = 65;
			mtiFuelMap4.m_index = 255; // fake
			mtiIdleSpeed1.m_index = 24;
			mtiIdleSpeed2.m_index = 26;
			mtiEgrMap.m_index = 55;
			mtiCoolantTemp.m_index = 74; // fake
            mtiUnknownMap1.m_index = 255; // fake
            mtiUnknownMap2.m_index = 255; // fake
            mtiSmokeLimLowRange.m_index = 255; // fake
            mtiUnknownMap4.m_index = 255; // fake
            mtiUnknownMap5.m_index = 255; // fake
            mtiTimingsMap.m_index = 255; // fake
            mtiTwgDutyCycle.m_index = 72;
            mtiTwgIntegralGain.m_index = 255; // fake
            mtiTwgPressureMap.m_index = 77;

            mtiScalarTwgPwmFreq.m_index = 91;
            mtiScalarMapMaxLimit.m_index = 107;
            mtiScalarTurboOverboostMax.m_index = 95;
            mtiScalarTurboOverboostRecovery.m_index = 96;
            mtiScalarCruiseControlMinSpeed.m_index = 255; // fake
            mtiScalarCruiseControlMaxSpeed.m_index = 255; // fake
            mtiScalarCruiseControlIncrementForTap.m_index = 255; // fake
            mtiScalarCruiseControlMaxAccelerationRate.m_index = 255; // fake
            mtiScalarCruiseControlMaxDecelerationRate.m_index = 255; // fake
            mtiScalarCruiseControlTapsLimit.m_index = 255; // fake
			break;

        default:
            mtiTorqueLimHighRange.m_index = 255; // fake
            mtiTorqueLimLowRange.m_index = 255; // fake
			mtiUnknownMap3.m_index = 255; // fake
			mtiSmokeLimHighRangeA.m_index = 255; // fake
			mtiSmokeLimHighRangeB.m_index = 255; // fake
			mtiDriverDemandHighRange.m_index = 255; // fake
			mtiDriverDemandLowRange.m_index = 255; // fake
            mtiFuelDensityCompLower.m_index = 255; // fake
            mtiFuelDensityCompUpper.m_index = 255; // fake
			mtiFuelMap1.m_index = 255; // fake
			mtiFuelMap2.m_index = 255; // fake
			mtiFuelMap3.m_index = 255; // fake
			mtiFuelMap4.m_index = 255; // fake
			mtiIdleSpeed1.m_index = 255; // fake
			mtiIdleSpeed2.m_index = 255; // fake
			mtiEgrMap.m_index = 255; // fake
            mtiUnknownMap1.m_index = 255; // fake
            mtiUnknownMap2.m_index = 255; // fake
            mtiSmokeLimLowRange.m_index = 255; // fake
            mtiUnknownMap4.m_index = 255; // fake
            mtiUnknownMap5.m_index = 255; // fake
            mtiTimingsMap.m_index = 255; // fake
            mtiTwgDutyCycle.m_index = 255; // fake
            mtiTwgIntegralGain.m_index = 255; // fake
            mtiTwgPressureMap.m_index = 255; // fake

            mtiScalarTwgPwmFreq.m_index = 255; // fake
            mtiScalarMapMaxLimit.m_index = 255; // fake
            mtiScalarTurboOverboostMax.m_index = 255; // fake
            mtiScalarTurboOverboostRecovery.m_index = 255; // fake
            mtiScalarCruiseControlMinSpeed.m_index = 255; // fake
            mtiScalarCruiseControlMaxSpeed.m_index = 255; // fake
            mtiScalarCruiseControlIncrementForTap.m_index = 255; // fake
            mtiScalarCruiseControlMaxAccelerationRate.m_index = 255; // fake
            mtiScalarCruiseControlMaxDecelerationRate.m_index = 255; // fake
            mtiScalarCruiseControlTapsLimit.m_index = 255; // fake
			break;
	}


	m_mapInfo.Add(mtiTorqueLimHighRange);
	m_mapInfo.Add(mtiTorqueLimLowRange);
	m_mapInfo.Add(mtiDriverDemandHighRange);
	m_mapInfo.Add(mtiDriverDemandLowRange);
    m_mapInfo.Add(mtiSmokeLimLowRange);
	m_mapInfo.Add(mtiSmokeLimHighRangeA);
	m_mapInfo.Add(mtiSmokeLimHighRangeB);
	m_mapInfo.Add(mtiFuelMap1);
	m_mapInfo.Add(mtiFuelMap2);
	m_mapInfo.Add(mtiFuelMap3);
	m_mapInfo.Add(mtiFuelMap4);
	m_mapInfo.Add(mtiFuelDensityCompLower);
	m_mapInfo.Add(mtiFuelDensityCompUpper);
	m_mapInfo.Add(mtiIdleSpeed1);
	m_mapInfo.Add(mtiIdleSpeed2);
	m_mapInfo.Add(mtiEgrMap);
	m_mapInfo.Add(mtiCoolantTemp);
	m_mapInfo.Add(mtiTimingsMap);
    m_mapInfo.Add(mtiUnknownMap1);
    m_mapInfo.Add(mtiUnknownMap2);
	m_mapInfo.Add(mtiUnknownMap3);
    m_mapInfo.Add(mtiUnknownMap4);
	m_mapInfo.Add(mtiUnknownMap5);
	m_mapInfo.Add(mtiTwgDutyCycle);
	m_mapInfo.Add(mtiTwgIntegralGain);
	m_mapInfo.Add(mtiTwgPressureMap);

	m_mapInfo.Add(mtiScalarTwgPwmFreq);
	m_mapInfo.Add(mtiScalarMapMaxLimit);
	m_mapInfo.Add(mtiScalarTurboOverboostMax);
	m_mapInfo.Add(mtiScalarTurboOverboostRecovery);
	m_mapInfo.Add(mtiScalarCruiseControlMinSpeed);
	m_mapInfo.Add(mtiScalarCruiseControlMaxSpeed);
	m_mapInfo.Add(mtiScalarCruiseControlIncrementForTap);
	m_mapInfo.Add(mtiScalarCruiseControlMaxAccelerationRate);
	m_mapInfo.Add(mtiScalarCruiseControlMaxDecelerationRate);
	m_mapInfo.Add(mtiScalarCruiseControlTapsLimit);

}

td5mapTableInfo::~td5mapTableInfo()
{

}

bool td5mapTableInfo::GetInfoFromIndex(td5mapTableInfoItem& ti, int iIndex)
{
	bool bFound = FALSE;

	for(int i = 0; i < (int) m_mapInfo.GetCount(); i++)
	{
		td5mapTableInfoItem tableinfo = m_mapInfo.Item(i);

		if(tableinfo.m_index == iIndex)
		{
			bFound = TRUE;
			ti = tableinfo;
		}
	}

	return bFound;
}
