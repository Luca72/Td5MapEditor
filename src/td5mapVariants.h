/***************************************************************
 * Name:      td5mapVariants.h
 * Purpose:   Defines Application Frame
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-09
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef TD5MAPVARIANTS_H
#define TD5MAPVARIANTS_H


#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/dynarray.h>

class td5mapVariant : public wxObject
{
	DECLARE_DYNAMIC_CLASS(td5mapVariant)
public:
	td5mapVariant& operator=( const td5mapVariant &s )  // assignment operator
	{
	    m_id = s.m_id;
		m_mapName = s.m_mapName;
		m_variant = s.m_variant;

		return *this;
	}

public:
	td5mapVariant();
	td5mapVariant(wxUint32 id, wxString mapname, wxString variant);
	td5mapVariant(const td5mapVariant& pmm);

	virtual ~td5mapVariant();

public:

	wxUint32 m_id;
	wxString m_mapName;
	wxString m_variant;
};


WX_DECLARE_OBJARRAY(td5mapVariant, arrayofVariants);

class td5mapVariants : public wxObject
{
public:
	td5mapVariants();
	virtual ~td5mapVariants();

// Implementation
public:
	bool GetVariantInfoFromID(td5mapVariant& mm, wxUint32 id);
	wxString GetVariant(int index) {return m_variants.Item(index).m_variant;};
	wxString GetMapName(int index);
	int GetID(int index) {return (int) m_variants.Item(index).m_id;};
	int  GetCount() {return m_variants.GetCount();};

	arrayofVariants m_variants;
};

#endif // TD5MAPVARIANTS_H
