/***************************************************************
 * Name:      td5mapVariants.cpp
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-09
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "td5mapVariants.h"
#include "resource.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(arrayofVariants);


// td5mapVariant
IMPLEMENT_CLASS(td5mapVariant, wxObject)

td5mapVariant::td5mapVariant()
{
    m_id = 0;
    m_mapName = wxT("");
    m_variant = wxT("");
}

td5mapVariant::td5mapVariant(wxUint32 id, wxString mapname, wxString variant)
{
    m_id = id;
    m_mapName = mapname;
    m_variant = variant;
}

td5mapVariant::td5mapVariant(const td5mapVariant& pmm)
{
    m_id = pmm.m_id;
    m_mapName = pmm.m_mapName;
    m_variant = pmm.m_variant;
}

td5mapVariant::~td5mapVariant()
{

}

// td5mapVariants

td5mapVariants::td5mapVariants()
{
    td5mapVariant mmDisco_Man_Eu_sthde021_sttdp009(
        IDR_STHDE021STTDP009_MAP,
        _T("sthde021sttdp009"),
        _T("Disco, Manual, Europe, EU2, NNN000120"));

    td5mapVariant mmDisco_Man_Eu_sthde021_sttdp010(
        IDR_STHDE021STTDP010_MAP,
        _T("sthde021sttdp010"),
        _T("Disco, Manual, Europe, EU2, NNN500020"));

    td5mapVariant mmDisco_Man_Eu_sthdr009_sttdp009(
        IDR_STHDR009STTDP009_MAP,
        _T("sthdr009sttdp009"),
        _T("Disco, Manual, ROW, EU2, NNN000120"));

    td5mapVariant mmDisco_Man_Eu_sthdr009_sttdp010(
        IDR_STHDR009STTDP010_MAP,
        _T("sthdr009sttdp010"),
        _T("Disco, Manual, ROW, EU2, NNN500020"));

    td5mapVariant mmDef_Man_Eu_sthle022_sttlp009(
        IDR_STHLE022STTLP009_MAP,
        _T("sthle022sttlp009"),
        _T("Defender, Manual, Europe, EU2, NNN000120"));

    td5mapVariant mmDef_Man_Eu_sthle022_sttlp010(
        IDR_STHLE022STTLP010_MAP,
        _T("sthle022sttlp010"),
        _T("Defender, Manual, Europe, EU2, NNN500020"));

    td5mapVariant mmDisco_Auto_Eu_suhde036_sutdp012(
        IDR_SUHDE036SUTDP012_MAP,
        _T("suhde036sutdp012"),
        _T("Disco, Auto, Europe, EU2, NNN000130"));

    td5mapVariant mmDisco_Auto_Eu_suhde036_sutdp014(
        IDR_SUHDE036SUTDP014_MAP,
        _T("suhde036sutdp014"),
        _T("Disco, Auto, Europe, EU2, NNN500030"));

    td5mapVariant mmDisco_Auto_ROW_suhdr009_sutzp004(
        IDR_SUHDR009SUTZP004_MAP,
        _T("suhdr009sutzp004"),
        _T("Disco, Auto, ROW, EU2, NNN000130"));

    td5mapVariant mmDisco_Auto_ROW_suhdr009_sutzp005(
        IDR_SUHDR009SUTZP005_MAP,
        _T("suhdr009sutzp005"),
        _T("Disco, Auto, ROW, EU2, NNN500030"));

    td5mapVariant mmDisco_Auto_Korea_surdk004_sutzp004(
        IDR_SURDK004SUTZP004_MAP,
        _T("surdk004sutzp004"),
        _T("Disco, Auto, Korea, EU2, NNN000130"));

    td5mapVariant mmDisco_Auto_Korea_surdk004_sutzp005(
        IDR_SURDK004SUTZP005_MAP,
        _T("surdk004sutzp005"),
        _T("Disco, Auto, Korea, EU2, NNN500030"));

    td5mapVariant mmDisco_Man_Eu_svdxe003_svtnp003(
        IDR_SVDXE003SVTNP003_MAP,
        _T("svdxe003svtnp003"),
        _T("Discovery, Manual, Europe, EU3, NNN000120"));

    td5mapVariant mmDisco_Man_Eu_svdxe004_svtnp003(
        IDR_SVDXE004SVTNP003_MAP,
        _T("svdxe004svtnp003"),
        _T("Discovery, Manual, Europe, EU3, NNN000120"));

    td5mapVariant mmDisco_Man_Eu_svdxe006_svtnp003(
        IDR_SVDXE006SVTNP003_MAP,
        _T("svdxe006svtnp003"),
        _T("Discovery, Manual, Europe, EU3, NNN000120"));

    td5mapVariant mmDisco_Man_Eu_svdxe008_svtnp005(
        IDR_SVDXE008SVTNP005_MAP,
        _T("svdxe008svtnp005"),
        _T("Discovery, Manual, Europe, EU3, NNN000120"));

    td5mapVariant mmDisco_Man_Eu_svdxe008_svtnp006(
        IDR_SVDXE008SVTNP006_MAP,
        _T("svdxe008svtnp006"),
        _T("Discovery, Manual, Europe, EU3, NNN500020"));

    td5mapVariant mmDisco_Man_Unkn_svdxg003_svtnp006(
        IDR_SVDXG003SVTNP006_MAP,
        _T("svdxg003svtnp006"),
        _T("Discovery, Manual, Unknown, EU3, NNN500250"));

    td5mapVariant mmDisco_Man_ROW_svdxr002_svtnp003(
        IDR_SVDXR002SVTNP003_MAP,
        _T("svdxr002svtnp003"),
        _T("Discovery, Manual, ROW, EU3, NNN000120"));

    td5mapVariant mmDisco_Man_ROW_svdxr005_svtnp003(
        IDR_SVDXR005SVTNP003_MAP,
        _T("svdxr005svtnp003"),
        _T("Discovery, Manual, ROW, EU3, NNN000120"));

    td5mapVariant mmDisco_Man_ROW_svdxr007_svtnp005(
        IDR_SVDXR007SVTNP005_MAP,
        _T("svdxr007svtnp005"),
        _T("Discovery, Manual, ROW, EU3, NNN000120"));

    td5mapVariant mmDisco_Man_ROW_svdxr007_svtnp006(
        IDR_SVDXR007SVTNP006_MAP,
        _T("svdxr007svtnp006"),
        _T("Discovery, Manual, ROW, EU3, NNN500020"));

    td5mapVariant mmDef90_Man_Eu_svlne004_svtnp003(
        IDR_SVLNE004SVTNP003_MAP,
        _T("svlne004svtnp003"),
        _T("Defender 90, Manual, Europe, EU3, NNN000120"));

    td5mapVariant mmDef90_Man_Eu_svlne006_svtnp003(
        IDR_SVLNE006SVTNP003_MAP,
        _T("svlne006svtnp003"),
        _T("Defender 90, Manual, Europe, EU3, NNN000120"));

    td5mapVariant mmDef90_Man_Eu_svlne007_svtnp005(
        IDR_SVLNE007SVTNP005_MAP,
        _T("svlne007svtnp005"),
        _T("Defender 90, Manual, Europe, EU3, NNN000120"));

    td5mapVariant mmDef90_Man_Eu_svlne007_svtnp006(
        IDR_SVLNE007SVTNP006_MAP,
        _T("svlne007svtnp006"),
        _T("Defender 90, Manual, Europe, EU3, NNN500020"));

    td5mapVariant mmDef90_Man_ROW_svlnr002_svtnp003(
        IDR_SVLNR002SVTNP003_MAP,
        _T("svlnr002svtnp003"),
        _T("Defender 90, Manual, ROW, EU3, NNN000120"));

    td5mapVariant mmDef90_Man_ROW_svlnr004_svtnp003(
        IDR_SVLNR004SVTNP003_MAP,
        _T("svlnr004svtnp003"),
        _T("Defender 90, Manual, ROW, EU3, NNN000120"));

    td5mapVariant mmDef90_Man_ROW_svlnr005_svtnp005(
        IDR_SVLNR005SVTNP005_MAP,
        _T("svlnr005svtnp005"),
        _T("Defender 90, Manual, ROW, EU3, NNN000120"));

    td5mapVariant mmDef90_Man_ROW_svlnr005_svtnp006(
        IDR_SVLNR005SVTNP006_MAP,
        _T("svlnr005svtnp006"),
        _T("Defender 90, Manual, ROW, EU3, NNN500020"));

    td5mapVariant mmDef110_Man_Eu_svloe002_svtnp003(
        IDR_SVLOE002SVTNP003_MAP,
        _T("svloe002svtnp003"),
        _T("Defender 110, Manual, Europe, EU3, NNN000120"));

    td5mapVariant mmDef110_Man_Eu_svloe004_svtnp003(
        IDR_SVLOE004SVTNP003_MAP,
        _T("svloe004svtnp003"),
        _T("Defender 110, Manual, Europe, EU3, NNN000120"));

    td5mapVariant mmDef110_Man_Eu_svloe005_svtnp005(
        IDR_SVLOE005SVTNP005_MAP,
        _T("svloe005svtnp005"),
        _T("Defender 110, Manual, Europe, EU3, NNN000120"));

    td5mapVariant mmDef110_Man_Eu_svloe005_svtnp006(
        IDR_SVLOE005SVTNP006_MAP,
        _T("svloe005svtnp006"),
        _T("Defender 110, Manual, Europe, EU3, NNN500020"));

    td5mapVariant mmDef110_Man_Eu_svloj002_svtnp003(
        IDR_SVLOJ002SVTNP003_MAP,
        _T("svloj002svtnp003"),
        _T("Defender 110, Manual, Japan, EU3, NNN000120"));

    td5mapVariant mmDef110_Man_Eu_svloj002_svtnp006(
        IDR_SVLOJ002SVTNP006_MAP,
        _T("svloj002svtnp006"),
        _T("Defender 110, Manual, Japan, EU3, NNN500020"));

    td5mapVariant mmDef110_Man_ROW_svlor002_svtnp003(
        IDR_SVLOR002SVTNP003_MAP,
        _T("svlor002svtnp003"),
        _T("Defender 110, Manual, ROW, EU3, NNN000120"));

    td5mapVariant mmDef110_Man_ROW_svlor004_svtnp003(
        IDR_SVLOR004SVTNP003_MAP,
        _T("svlor004svtnp003"),
        _T("Defender 110, Manual, ROW, EU3, NNN000120"));

    td5mapVariant mmDef110_Man_ROW_svlor005_svtnp005(
        IDR_SVLOR005SVTNP005_MAP,
        _T("svlor005svtnp005"),
        _T("Defender 110, Manual, ROW, EU3, NNN000120"));

    td5mapVariant mmDef110_Man_ROW_svlor005_svtnp006(
        IDR_SVLOR005SVTNP006_MAP,
        _T("svlor005svtnp006"),
        _T("Defender 110, Manual, ROW, EU3, NNN500020"));

    td5mapVariant mmDisco_Auto_Eu_swdxe004_swtnp004(
        IDR_SWDXE004SWTNP004_MAP,
        _T("swdxe004swtnp004"),
        _T("Discovery, Automatic, Europe, EU3, NNN000130"));

    td5mapVariant mmDisco_Auto_Eu_swdxe007_swtnp004(
        IDR_SWDXE007SWTNP004_MAP,
        _T("swdxe007swtnp004"),
        _T("Discovery, Automatic, Europe, EU3, NNN000130"));

    td5mapVariant mmDisco_Auto_Eu_swdxe007_swtnp006(
        IDR_SWDXE007SWTNP006_MAP,
        _T("swdxe007swtnp006"),
        _T("Discovery, Automatic, Europe, EU3, NNN500030"));

    td5mapVariant mmDisco_Auto_Korea_swdxk001_swtnp004(
        IDR_SWDXK001SWTNP004_MAP,
        _T("swdxk001swtnp004"),
        _T("Discovery, Automatic, Korea, EU3, NNN000130"));

    td5mapVariant mmDisco_Auto_Korea_swdxk003_swtnp004(
        IDR_SWDXK003SWTNP004_MAP,
        _T("swdxk003swtnp004"),
        _T("Discovery, Automatic, Korea, EU3, NNN000130"));

    td5mapVariant mmDisco_Auto_Korea_swdxk003_swtnp006(
        IDR_SWDXK003SWTNP006_MAP,
        _T("swdxk003swtnp006"),
        _T("Discovery, Automatic, Korea, EU3, NNN500030"));

    td5mapVariant mmDisco_Auto_ROW_swdxr002_swtnp004(
        IDR_SWDXR002SWTNP004_MAP,
        _T("swdxr001swtnp004"),
        _T("Discovery, Automatic, ROW, EU3, NNN000130"));

    td5mapVariant mmDisco_Auto_ROW_swdxr004_swtnp004(
        IDR_SWDXR004SWTNP004_MAP,
        _T("swdxr003swtnp004"),
        _T("Discovery, Automatic, ROW, EU3, NNN000130"));

    td5mapVariant mmDisco_Auto_ROW_swdxr004_swtnp006(
        IDR_SWDXR004SWTNP006_MAP,
        _T("swdxr003swtnp006"),
        _T("Discovery, Automatic, ROW, EU3, NNN500030"));

    m_variants.Add(mmDisco_Man_Eu_sthde021_sttdp009);
    m_variants.Add(mmDisco_Man_Eu_sthde021_sttdp010);
    m_variants.Add(mmDisco_Man_Eu_sthdr009_sttdp009);
    m_variants.Add(mmDisco_Man_Eu_sthdr009_sttdp010);
    m_variants.Add(mmDef_Man_Eu_sthle022_sttlp009);
    m_variants.Add(mmDef_Man_Eu_sthle022_sttlp010);
    m_variants.Add(mmDisco_Auto_Eu_suhde036_sutdp012);
    m_variants.Add(mmDisco_Auto_Eu_suhde036_sutdp014);
    m_variants.Add(mmDisco_Auto_ROW_suhdr009_sutzp004);
    m_variants.Add(mmDisco_Auto_ROW_suhdr009_sutzp005);
    m_variants.Add(mmDisco_Auto_Korea_surdk004_sutzp004);
    m_variants.Add(mmDisco_Auto_Korea_surdk004_sutzp005);
    m_variants.Add(mmDisco_Man_Eu_svdxe003_svtnp003);
    m_variants.Add(mmDisco_Man_Eu_svdxe004_svtnp003);
    m_variants.Add(mmDisco_Man_Eu_svdxe006_svtnp003);
    m_variants.Add(mmDisco_Man_Eu_svdxe008_svtnp005);
    m_variants.Add(mmDisco_Man_Eu_svdxe008_svtnp006);
    m_variants.Add(mmDisco_Man_Unkn_svdxg003_svtnp006);
    m_variants.Add(mmDisco_Man_ROW_svdxr002_svtnp003);
    m_variants.Add(mmDisco_Man_ROW_svdxr005_svtnp003);
    m_variants.Add(mmDisco_Man_ROW_svdxr007_svtnp005);
    m_variants.Add(mmDisco_Man_ROW_svdxr007_svtnp006);
    m_variants.Add(mmDef90_Man_Eu_svlne004_svtnp003);
    m_variants.Add(mmDef90_Man_Eu_svlne006_svtnp003);
    m_variants.Add(mmDef90_Man_Eu_svlne007_svtnp005);
    m_variants.Add(mmDef90_Man_Eu_svlne007_svtnp006);
    m_variants.Add(mmDef90_Man_ROW_svlnr002_svtnp003);
    m_variants.Add(mmDef90_Man_ROW_svlnr004_svtnp003);
    m_variants.Add(mmDef90_Man_ROW_svlnr005_svtnp005);
    m_variants.Add(mmDef90_Man_ROW_svlnr005_svtnp006);
    m_variants.Add(mmDef110_Man_Eu_svloe002_svtnp003);
    m_variants.Add(mmDef110_Man_Eu_svloe004_svtnp003);
    m_variants.Add(mmDef110_Man_Eu_svloe005_svtnp005);
    m_variants.Add(mmDef110_Man_Eu_svloe005_svtnp006);
    m_variants.Add(mmDef110_Man_Eu_svloj002_svtnp003);
    m_variants.Add(mmDef110_Man_Eu_svloj002_svtnp006);
    m_variants.Add(mmDef110_Man_ROW_svlor002_svtnp003);
    m_variants.Add(mmDef110_Man_ROW_svlor004_svtnp003);
    m_variants.Add(mmDef110_Man_ROW_svlor005_svtnp005);
    m_variants.Add(mmDef110_Man_ROW_svlor005_svtnp006);
    m_variants.Add(mmDisco_Auto_Eu_swdxe004_swtnp004);
    m_variants.Add(mmDisco_Auto_Eu_swdxe007_swtnp004);
    m_variants.Add(mmDisco_Auto_Eu_swdxe007_swtnp006);
    m_variants.Add(mmDisco_Auto_Korea_swdxk001_swtnp004);
    m_variants.Add(mmDisco_Auto_Korea_swdxk003_swtnp004);
    m_variants.Add(mmDisco_Auto_Korea_swdxk003_swtnp006);
    m_variants.Add(mmDisco_Auto_ROW_swdxr002_swtnp004);
    m_variants.Add(mmDisco_Auto_ROW_swdxr004_swtnp004);
    m_variants.Add(mmDisco_Auto_ROW_swdxr004_swtnp006);
}

td5mapVariants::~td5mapVariants()
{

}

wxString td5mapVariants::GetMapName(int index)
{
    wxString str;
    str = m_variants.Item(index).m_mapName;
    return str;
}


bool td5mapVariants::GetVariantInfoFromID(td5mapVariant& mm, wxUint32 id)
{
    bool bFound = FALSE;

    for(int i = 0; i < (int) m_variants.GetCount(); i++)
    {
        td5mapVariant variant = m_variants.Item(i);

        if(variant.m_id == id)
        {
            bFound = TRUE;
            mm = variant;
        }
    }

    return bFound;
}

