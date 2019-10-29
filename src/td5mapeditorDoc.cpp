/***************************************************************
 * Name:      td5mapeditorDoc.cpp
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-07
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#if wxUSE_STD_IOSTREAM
    #include "wx/ioswrap.h"
#else
    #include "wx/txtstrm.h"
#endif

#include <wx/tokenzr.h>
#include <wx/filedlg.h>
#include <wx/file.h>

#include "td5mapeditorDoc.h"
#include "td5mapeditorView.h"

#include "baseMaps.h"
#include "ewxRange.h"
#include "td5mapTuner.h"
#include "tinyxml/tinyxml.h"


IMPLEMENT_DYNAMIC_CLASS(td5mapeditorDoc, wxDocument)

td5mapeditorDoc::td5mapeditorDoc(void)
{
	m_numberOfTables = 0;
	m_selectedTable = 0;
    m_fileName = _T("");
	m_mapBaseData = NULL;
	m_mapExternalData = NULL;
	m_scalarAddresses = NULL;
	m_fileName = _T("");

	m_mapName = _T("");
	m_mapTag = _T("");

	m_mapType = 0;

    m_mapFound = false;
    m_mapID = 0;
	m_selectionRange = ewxRange(0, 0, 0, 0);

    m_updateGridPanel = false;
    m_updateInfoPanel = false;
    m_updateGraphCanvas = false;

    m_mapIdBegin = 0;
    m_mapIdEnd = 0;
    m_scalarIdBegin = 0;
    m_scalarIdEnd = 0;
}

td5mapeditorDoc::~td5mapeditorDoc(void)
{
    if(m_mapBaseData && m_mapFound)
        delete m_mapBaseData;
    if(m_mapExternalData)
        delete m_mapExternalData;
}

#if wxUSE_STD_IOSTREAM
wxSTD ostream& td5mapeditorDoc::SaveObject(wxSTD ostream& stream)
{
    wxDocument::SaveObject(stream);

    wxInt32 n = MAP_FILE_LENGTH_WORD;

    union DATAWORD {wxWord word; char chr[2];};
    DATAWORD buff;

    for(int i = 0; i < n; i++)
    {
        buff.word = m_mapFileData[i];
        stream.write(buff.chr, 2);
    }


    return stream;
}
#else
wxOutputStream& td5mapeditorDoc::SaveObject(wxOutputStream& stream)
{
    wxDocument::SaveObject(stream);

    wxInt32 n = MAP_FILE_LENGTH_WORD;

    for(int i = 0; i < n; i++)
        stream.Write(&m_mapFileData[i], 2);

    return stream;
}
#endif

#if wxUSE_STD_IOSTREAM
wxSTD istream& td5mapeditorDoc::LoadObject(wxSTD istream& stream)
{
    wxDocument::LoadObject(stream);

    wxInt32 n = MAP_FILE_LENGTH_WORD;

    union DATAWORD {wxWord word; char chr[2];};
    DATAWORD buff;

    for(int i = 0; i < n; i++)
    {
        stream.read(buff.chr, 2);
        m_mapFileData[i] = buff.word;
    }


    return stream;
}
#else
wxInputStream& td5mapeditorDoc::LoadObject(wxInputStream& stream)
{
    wxDocument::LoadObject(stream);

    wxInt32 n = MAP_FILE_LENGTH_WORD;

    for(int i = 0; i < n; i++)
        stream.Read(&m_mapFileData[i], 2);

    return stream;
}
#endif

bool td5mapeditorDoc::OnNewDocument()
{
    wizNewMap *wizard = new wizNewMap(GetMainFrame());

    if (!wizard->RunWizard(wizard->GetFirstPage()))
    {
        wizard->Destroy();
        return false;
    }

    WIZ_NEW_MAP_DATA wiznewmapdata = wizard->TransferDataFromPages();
    wizard->Destroy();

	if (!wxDocument::OnNewDocument())
		return false;

	m_mapName = wiznewmapdata.mapName;
    m_fileName = m_mapName; //ExtractFileName(GetFilename()); /*GetFilename() + _T(".map");*/
    m_mapTag = _T("");
	m_mapID = wiznewmapdata.mapID;

	// extract Base map
	m_mapBaseData = ExtractMapResource(wiznewmapdata.mapID, m_mapFound);

	if(!m_mapFound)
        m_mapBaseData = m_mapFileData;

    // create new map
    wxWord *m_mapBasePointer = m_mapBaseData;
    for(int i = 0; i < (int) MAP_FILE_LENGTH_WORD; i++)
    {
        m_mapFileData[i] = *m_mapBasePointer;
        m_mapBasePointer++;
    }

 	// Build tables structures
	int i = 0;
	wxWord tableAddress = 0;
	wxWord address = 0;
	int indexStartAddress = ExtractTablesIndexAddress();

    m_mapIdBegin = i;
    while(tableAddress < 32767/*65535*/)
    {
        address = (indexStartAddress / sizeof(wxWord)) + i;
        tableAddress = LoHi2HiLo(m_mapFileData[address]);
        m_mapTable[i].m_index = i;
        m_mapTable[i].m_address = tableAddress;
        m_mapAddresses[i] = tableAddress;
        m_mapTable[i].m_singlevalue = false;
        i++;
    }
	//i = i - 3; // To be verified why "- 3" ????
    i = i - 1;
    m_mapIdEnd = i;

	// Build scalar structures
	wxWord scalarAddress = 0;
	int scalar_index = 0;
	m_scalarAddresses = ExtractScalarsIndexAddress();

    m_scalarIdBegin = i;
    while(scalarAddress < 65535)
    {
        scalarAddress = m_scalarAddresses[scalar_index];
        m_mapTable[i].m_index = i;
        m_mapTable[i].m_address = scalarAddress;
        m_mapTable[i].m_singlevalue = true;
        scalar_index++;
        i++;
    }
    i = i - 1;
    m_scalarIdEnd = i;

    m_numberOfTables = i;


    // read tables
    bool recognized = false;
    int firstRecognized = 0;
	for(int m = 0; m < m_numberOfTables; m++)
	{
		m_mapTable[m].m_mapID = wiznewmapdata.mapID;
		m_mapTable[m].ReadTable(m_mapFileData, m, m_mapBaseData);

		if (m_mapTable[m].m_recognized)
		{
		    recognized = true;
		    if (firstRecognized == 0)
                firstRecognized = m;
		}

    }

    SelectTable(firstRecognized);

    //td5mapeditorView *view = (td5mapeditorView *) GetFirstView();
    //view->SetTitle(m_fileName);
    //SetTitle(m_fileName);
    //SetFilename(m_fileName);

    td5mapeditorView *view = (td5mapeditorView *) GetFirstView();
    if (recognized)
        view->SetInfoPanelFlags(true, false);
    else
        view->SetInfoPanelFlags(false, false);

    Modify(true);
    SetUpdateFlag(GRID_PANEL | INFO_PANEL |GRAPH_PANEL);
    Update();

    /*
    GetMainFrame()->EnableChildButtons();
    */

    return true;
}

bool td5mapeditorDoc::OnOpenDocument(const wxString& filename)
{
	if (!wxDocument::OnOpenDocument(filename))
		return true;

    //m_fileName = filename.Right(filename.Length() - (filename.Find(wxChar(47), true) + 1));
    m_fileName = ExtractFileName(filename);

	// Get base map name
    m_mapName = ExtractMapNameFromFile();

    // Get map tag
	m_mapTag = ExtractTagFromFile();

    // Get base map ID
	int m_mapID = ExtractMapIDFromFile();

	// Build tables structures
	int i = 0;
	wxWord tableAddress = 0;
	wxWord address = 0;
	int indexStartAddress = ExtractTablesIndexAddress();

    m_mapIdBegin = i;
    while(tableAddress < 32767/*65535*/)
    {
        address = (indexStartAddress / sizeof(wxWord)) + i;
        tableAddress = LoHi2HiLo(m_mapFileData[address]);
        m_mapTable[i].m_index = i;
        m_mapTable[i].m_address = tableAddress;
        m_mapAddresses[i] = tableAddress;
        m_mapTable[i].m_singlevalue = false;
        i++;
    }

    //i = i - 3; // To be verified why "- 3" ????
    i = i - 1;
    m_mapIdEnd = i;


	// Build scalar structures
	wxWord scalarAddress = 0;
	int scalar_index = 0;
	m_scalarAddresses = ExtractScalarsIndexAddress();

    m_scalarIdBegin = i;
    while(scalarAddress < 65535)
    {
        scalarAddress = m_scalarAddresses[scalar_index];
        m_mapTable[i].m_index = i;
        m_mapTable[i].m_address = scalarAddress;
        m_mapTable[i].m_singlevalue = true;
        scalar_index++;
        i++;
    }
    i = i - 1;
    m_scalarIdEnd = i;

    m_numberOfTables = i;

    // Extract base map from resources
	m_mapBaseData = ExtractMapResource(m_mapID, m_mapFound);

	if(!m_mapFound)
        m_mapBaseData = m_mapFileData;

    // read tables
    bool recognized = false;
    int firstRecognized = 0;
    for(int m = 0; m < m_numberOfTables; m++)
	{
		m_mapTable[m].m_mapID = m_mapID;
		m_mapTable[m].ReadTable(m_mapFileData, m, m_mapBaseData);

		if (m_mapTable[m].m_recognized)
		{
		    recognized = true;
		    if (firstRecognized == 0)
                firstRecognized = m;
		}

    }

    SelectTable(firstRecognized);

    //td5mapeditorView *view = (td5mapeditorView *) GetFirstView();
    //view->SetTitle(m_fileName);
    //SetTitle(m_fileName);
    //SetFilename(m_fileName);

    td5mapeditorView *view = (td5mapeditorView *) GetFirstView();
    if (recognized)
        view->SetInfoPanelFlags(true, false);
    else
        view->SetInfoPanelFlags(false, false);

    SetUpdateFlag(GRID_PANEL | INFO_PANEL |GRAPH_PANEL);
    Update();

/*
    GetMainFrame()->EnableChildButtons();
*/
    return true;
}

bool td5mapeditorDoc::OnSaveDocument(const wxString& filename)
{
    for(int m = 0; m < m_numberOfTables; m++)
	{
		m_mapTable[m].WriteTable(m_mapFileData);
	}

	m_mapFileData[(MAP_FILE_LENGTH / sizeof(wxWord)) - 1] = LoHi2HiLo(Checksum(m_mapFileData));

    //td5mapeditorView *view = (td5mapeditorView *) GetFirstView();
    //m_fileName = ExtractFileName(filename);
    //view->SetTitle(m_fileName);

    return wxDocument::OnSaveDocument(filename);
}

bool td5mapeditorDoc::OnCloseDocument()
{
/*
    if(GetDocumentManager()->GetDocuments().GetCount() == 1)
        GetMainFrame()->DisableChildButtons();
*/
    return wxDocument::OnCloseDocument();
}

void td5mapeditorDoc::ExportTuning(const wxString& ifileName)
{
    wxFile file (ifileName, wxFile::write);

    if (!file.IsOpened())
        return;

    short numModTables = 0;
    for(int m = 0; m < m_numberOfTables; m+=1)
    {
        if(m_mapTable[m].IsDifferentFromOriginal())
            numModTables += 1;
    }

    // number of modified tables
    wxWord buff = numModTables;
    file.Write (&buff, 2);

    for(int m = 0; m < m_numberOfTables; m+=1)
    {
        if(m_mapTable[m].IsDifferentFromOriginal())
        {
            // index of the table
            buff = m;
            file.Write (&buff, 2);

            // columns of the table
            buff = m_mapTable[m].GetCols();
            file.Write (&buff, 2);

            // rows of the table
            buff = m_mapTable[m].GetRows();
            file.Write (&buff, 2);

            for (int c = 0; c < m_mapTable[m].GetCols(); c++)
                for (int r = 0; r < m_mapTable[m].GetRows(); r++)
                {
                    buff = m_mapTable[m].GetCurrentValue(c, r) - m_mapTable[m].GetBaseValue(c, r);
                    file.Write (&buff, 2);
                }
        }
    }


    file.Close();
}

bool td5mapeditorDoc::ImportTuning(const wxString& ifileName)
{
    wxFile file (ifileName, wxFile::read);

    if (!file.IsOpened())
        return true;

    wxWord buff = 0;

    // number of modified tables
    file.Read (&buff, 2);
    short numModTables = buff;

    short indexImportedTable, cols, rows, value;
    for(int i = 0; i < numModTables; i+=1)
    {
        // index of the table
        file.Read (&buff, 2);
        indexImportedTable= buff;

        for(int m = 0; m < m_numberOfTables; m++)
        {
            if(m == indexImportedTable)
            {
                // columns of the table
                file.Read (&buff, 2);
                cols = buff;

                // rows of the table
                file.Read (&buff, 2);
                rows = buff;

                bool writeOK = false;
                if ((m_mapTable[indexImportedTable].GetCols() == cols) && (m_mapTable[indexImportedTable].GetRows() == rows))
                    writeOK = true;
                else
                {
                    wxString msg = _("Table  ") + m_mapTable[indexImportedTable].GetName() + _(" has a different size.\n Do you want to import it anyway?" );
                    wxMessageDialog dlg(GetMainFrame(), msg, _("Import Warning"), wxYES_NO);

                    if (dlg.ShowModal() == wxID_YES)
                        writeOK = true;
                    else
                        writeOK = false;
                }

                for (int c = 0; c < cols; c++)
                    for (int r = 0; r < rows; r++)
                    {
                        file.Read (&buff, 2);
                        value = buff;

                        if (writeOK && (c < m_mapTable[indexImportedTable].GetCols()) && (r < m_mapTable[indexImportedTable].GetRows()))
                        {
                            m_mapTable[indexImportedTable].SetCurrentValue((m_mapTable[indexImportedTable].GetBaseValue(c, r) + value), c, r);
                        }
                    }
            }
        }
    }


    file.Close();

    Modify(true);
    SetUpdateFlag(GRID_PANEL | INFO_PANEL |GRAPH_PANEL);
    Update();

    return false;
}

void td5mapeditorDoc::Update(wxView* sender)
{
    for(int m = 0; m < m_numberOfTables; m++)
	{
		m_mapTable[m].WriteTable(m_mapFileData);
	}

	m_mapFileData[(MAP_FILE_LENGTH / sizeof(wxWord)) - 1] = LoHi2HiLo(Checksum(m_mapFileData));

    GetMainFrame()->SetStatusBarText(wxString::Format(_T("Cks: 0x%04X"), Checksum(m_mapFileData)), 2);

    UpdateAllViews(sender);
}

void td5mapeditorDoc::ResetTableToBaseMap()
{
    for (int c = 0; c < m_mapTable[m_selectedTable].GetCols(); c++)
        for (int r = 0; r < m_mapTable[m_selectedTable].GetRows(); r++)
        {
            m_mapTable[m_selectedTable].SetCurrentValue(m_mapTable[m_selectedTable].GetBaseValue(c, r), c, r);
        }

    Modify(true);
    SetUpdateFlag(GRID_PANEL | INFO_PANEL |GRAPH_PANEL);
    Update();
}

wxWord *td5mapeditorDoc::ExtractMapResource(int nResourceId, bool& bOk)
{
	wxWord *baseMap; // pointer to resource data
	baseMap = new wxWord[MAP_FILE_LENGTH_WORD];
    bOk = true;

    wxString m_fileName = _T(":") + m_mapName + _T(".map");

#if wxUSE_STD_IOSTREAM
    std::string mapFileName = std::string(m_fileName.mb_str());
    std::istream *stream;

    baseMaps baseMapArchive(mapFileName);
    stream = baseMapArchive.get(mapFileName);

    if(stream == 0)
    {
        bOk = false;
        return baseMap;
    }

    union DATAWORD {wxWord word; char chr[2];};
    DATAWORD buff;

    for(int i = 0; i <(int) MAP_FILE_LENGTH_WORD; i++)
    {
        stream->read(buff.chr, 2);
        baseMap[i] = buff.word;
    }
#else
    wxInputStream* stream;
    baseMaps baseMapArchive(m_fileName);
    stream = baseMapArchive.Get(m_fileName);

    if(stream == 0)
    {
        bOk = false;
        return baseMap;
    }

    for(int i = 0; i < (int) MAP_FILE_LENGTH_WORD; i++)
        stream->Read(&baseMap[i], 2);
#endif

	return baseMap;
}

void td5mapeditorDoc::SetUpdateFlag(long panel)
{
    if (panel & GRID_PANEL)
        m_updateGridPanel = true;
    if (panel & INFO_PANEL)
        m_updateInfoPanel = true;
    if (panel & GRAPH_PANEL)
        m_updateGraphCanvas = true;
}

void td5mapeditorDoc::ResetUpdateFlag(long panel)
{
    if (panel & GRID_PANEL)
        m_updateGridPanel = false;
    if (panel & INFO_PANEL)
        m_updateInfoPanel = false;
    if (panel & GRAPH_PANEL)
        m_updateGraphCanvas = false;
}

bool td5mapeditorDoc::GetUpdateFlag(long panel)
{
    if (((panel & GRID_PANEL) && m_updateGridPanel) ||
        ((panel & INFO_PANEL) && m_updateInfoPanel) ||
        ((panel & GRAPH_PANEL) && m_updateGraphCanvas))
        return true;

    return false;
}

wxWord td5mapeditorDoc::GetCurrentRawValue(int address)
{
    return LoHi2HiLo(m_mapFileData[address]);
}

wxWord td5mapeditorDoc::GetBaseRawValue(int address)
{
    return LoHi2HiLo(m_mapBaseData[address]);
}

bool td5mapeditorDoc::SelectTable(int index)
{
    if (index > m_numberOfTables)
        return false;

    m_selectedTable = index;

    return true;
}

void td5mapeditorDoc::WriteTagToFile()
{
	union TAG {wxWord word[32]; char strtag[64];};
	TAG tag;

    for(int n = 0; n < 64; n++)
    {
        if (n < (int)m_mapTag.Length())
            tag.strtag[n] = m_mapTag.GetChar(n);
        else
            tag.strtag[n] = 0xFF;
    }

	for(int m = MAP_TAG_ADDRESS_BEGIN_WORD; m < (int)(MAP_TAG_ADDRESS_BEGIN_WORD + 32); m++)
        m_mapFileData[m] = tag.word[m - MAP_TAG_ADDRESS_BEGIN_WORD];
}


wxString td5mapeditorDoc::ExtractTagFromFile()
{
	union TAG {wxWord word[32]; char strtag[65];};
	TAG tag;

	for(int m = MAP_TAG_ADDRESS_BEGIN_WORD; m < (int)(MAP_TAG_ADDRESS_BEGIN_WORD + 32); m++)
        tag.word[m - MAP_TAG_ADDRESS_BEGIN_WORD] = m_mapFileData[m];

    for(int n = 0; n < 64; n++)
    {
        if (tag.strtag[n] < 0)
            tag.strtag[n] = 0;
    }

	tag.strtag[64] = 0;

	wxString appstr(tag.strtag, wxConvUTF8);

	return appstr;
}

wxString td5mapeditorDoc::ExtractMapNameFromFile()
{
	union MAPNAME {wxWord word[8]; char strname[17];};
	MAPNAME mapname;

	for(int m = MAP_MAPNAME_ADDRESS_BEGIN_WORD; m < (int)(MAP_MAPNAME_ADDRESS_BEGIN_WORD + 8); m++)
	{
		mapname.word[m - MAP_MAPNAME_ADDRESS_BEGIN_WORD] = m_mapFileData[m];
	}

	mapname.strname[16] = 0;
	wxString appstr(mapname.strname, wxConvUTF8);

	return appstr;
}

bool td5mapeditorDoc::LoadXDF(const wxString& ifileName)
{
    const char* title;
    const char* description;
    const char* address;
    const char* x_equation;
    const char* x_units;
    const char* y_units;
    const char* y_equation;
    const char* d_units;
    const char* d_equation;

    bool recognizedtable;
    wxWord tableAddress = 0x0000;
    double collabelmult;
    int collabeloff;
    double rowlabelmult;
    int rowlabeloff;
    double datamult;
    int dataoff;

    int counter = 0;

	TiXmlDocument document( ifileName );
	document.LoadFile();

	TiXmlElement* root = document.FirstChildElement( "XDFFORMAT" );
    if ( root )
    {
     	TiXmlElement* table = root->FirstChildElement( "XDFTABLE" );

        while( table )
        {
            recognizedtable = false;
            collabelmult = 1.0;
            collabeloff = 0;
            rowlabelmult = 1.0;
            rowlabeloff = 0;
            datamult = 1.0;
            dataoff = 0;

            title = "";
            description = "";
            address = "";
            x_equation = "";
            x_units = "";
            y_units = "";
            y_equation = "";
            d_units = "";
            d_equation = "";

            TiXmlElement* table_title = table->FirstChildElement( "title" );
            if(table_title)
            {
                title = table_title->GetText();
                wxString strtitle(title);
                if(strtitle.StartsWith(_T("Map")))
                    recognizedtable = false;
                else
                    recognizedtable = true;
            }

            TiXmlElement* table_desc = table->FirstChildElement( "description" );
            if(table_desc)
            {
                description = table_desc->GetText();
            }

            TiXmlElement* xaxis = table->FirstChildElement( "XDFAXIS" );
            if(xaxis)
            {
                TiXmlElement* xaxis_address = xaxis->FirstChildElement( "EMBEDDEDDATA" );
                if(xaxis_address)
                {
                    address = xaxis_address->Attribute("mmedaddress");
                    wxString straddress(address);
                    unsigned long ulongaddress;
                    straddress.ToULong(&ulongaddress, 16);
                    tableAddress = (wxWord)ulongaddress - 4;
                }

                TiXmlElement* xaxis_units = xaxis->FirstChildElement( "units" );
                if(xaxis_units)
                {
                    x_units = xaxis_units->GetText();
                }

                TiXmlElement* xaxis_math = xaxis->FirstChildElement( "MATH" );
                if(xaxis_math)
                {
                    x_equation = xaxis_math->Attribute("equation");
                    wxString strmath(x_equation);
                    if(strmath.IsSameAs(_T("X")))
                    {
                        collabelmult = 1.0;
                        collabeloff = 0;
                    }
                    if(strmath.IsSameAs(_T("(X-2732)/10")))
                    {
                        collabelmult = 0.1;
                        collabeloff = -2732;
                    }
                    if(strmath.IsSameAs(_T("X/10")))
                    {
                        collabelmult = 0.1;
                        collabeloff = 0;
                    }
                    if(strmath.IsSameAs(_T("X/100")))
                    {
                        collabelmult = 0.01;
                        collabeloff = 0;
                    }
                }
            }

            TiXmlElement* yaxis = xaxis->NextSiblingElement( "XDFAXIS" );
            if(yaxis)
            {
                TiXmlElement* yaxis_units = yaxis->FirstChildElement( "units" );
                if(yaxis_units)
                {
                    y_units = yaxis_units->GetText();
                }

                TiXmlElement* yaxis_math = yaxis->FirstChildElement( "MATH" );
                if(yaxis_math)
                {
                    y_equation = yaxis_math->Attribute("equation");
                    wxString strmath(y_equation);
                    if(strmath.IsSameAs(_T("X")))
                    {
                        rowlabelmult = 1.0;
                        rowlabeloff = 0;
                    }
                    if(strmath.IsSameAs(_T("(X-2732)/10")))
                    {
                        rowlabelmult = 0.1;
                        rowlabeloff = -2732;
                    }
                    if(strmath.IsSameAs(_T("X/10")))
                    {
                        rowlabelmult = 0.1;
                        rowlabeloff = 0;
                    }
                    if(strmath.IsSameAs(_T("X/100")))
                    {
                        rowlabelmult = 0.01;
                        rowlabeloff = 0;
                    }
                }
            }

            TiXmlElement* data = yaxis->NextSiblingElement( "XDFAXIS" );
            if(data)
            {
                TiXmlElement* data_units = data->FirstChildElement( "units" );
                if(data_units)
                {
                    d_units = data_units->GetText();
                }

                TiXmlElement* data_math = data->FirstChildElement( "MATH" );
                if(data_math)
                {
                    d_equation = data_math->Attribute("equation");
                    wxString strmath(d_equation);
                    if(strmath.IsSameAs(_T("X")))
                    {
                        datamult = 1.0;
                        dataoff = 0;
                    }
                    if(strmath.IsSameAs(_T("(X-2732)/10")))
                    {
                        datamult = 0.1;
                        dataoff = -2732;
                    }
                    if(strmath.IsSameAs(_T("X/10")))
                    {
                        datamult = 0.1;
                        dataoff = 0;
                    }
                    if(strmath.IsSameAs(_T("X/100")))
                    {
                        datamult = 0.01;
                        dataoff = 0;
                    }
                }
            }

            for(int m = m_mapIdBegin; m <= m_mapIdEnd; m++)
            {
                if(m_mapTable[m].m_address == tableAddress)
                {
                    if(recognizedtable)
                    {
                        m_mapTable[m].m_recognized = true;
                        m_mapTable[m].m_type = XDF_MAP;
                        m_mapTable[m].m_name = title;
                        m_mapTable[m].m_comment = description;
                        m_mapTable[m].m_xunit = x_units;
                        m_mapTable[m].m_yunit = y_units;
                        m_mapTable[m].m_zunit = d_units;
                        m_mapTable[m].m_collabelmult = collabelmult;
                        m_mapTable[m].m_collabeloff = collabeloff;
                        m_mapTable[m].m_rowlabelmult = rowlabelmult;
                        m_mapTable[m].m_rowlabeloff = rowlabeloff;
                        m_mapTable[m].m_datamult = datamult;
                        m_mapTable[m].m_dataoff = dataoff;

                        if ((collabelmult != 1.0) || (collabeloff != 0))
                            m_mapTable[m].m_collabelsized = true;
                        else
                            m_mapTable[m].m_collabelsized = false;

                        if ((rowlabelmult != 1.0) || (rowlabeloff != 0))
                            m_mapTable[m].m_rowlabelsized = true;
                        else
                            m_mapTable[m].m_rowlabelsized = false;

                        if ((datamult != 1.0) || (dataoff != 0))
                            m_mapTable[m].m_datasized = true;
                        else
                            m_mapTable[m].m_datasized = false;
                    }
                    else
                    {
                        m_mapTable[m].m_recognized = false;
                    }

                    break;
                }
            }

            table = table->NextSiblingElement( "XDFTABLE" );
            counter++;
        }

     	TiXmlElement* constant = root->FirstChildElement( "XDFCONSTANT" );

        while( constant )
        {
            recognizedtable = false;
            datamult = 1.0;
            dataoff = 0;

            title = "";
            description = "";
            address = "";
            d_units = "";
            d_equation = "";

            TiXmlElement* constant_title = constant->FirstChildElement( "title" );
            if(constant_title)
            {
                title = constant_title->GetText();
                recognizedtable = true;
            }

            TiXmlElement* constant_address = constant->FirstChildElement( "EMBEDDEDDATA" );
            if(constant_address)
            {
                address = constant_address->Attribute("mmedaddress");
                    wxString straddress(address);
                    unsigned long ulongaddress;
                    straddress.ToULong(&ulongaddress, 16);
                    tableAddress = (wxWord)ulongaddress;
            }

            TiXmlElement* constant_math = constant->FirstChildElement( "MATH" );
            if(constant_math)
            {
                d_equation = constant_math->Attribute("equation");
                wxString strmath(d_equation);
                if(strmath.IsSameAs(_T("X")))
                {
                    datamult = 1.0;
                    dataoff = 0;
                }
                if(strmath.IsSameAs(_T("(X-2732)/10")))
                {
                    datamult = 0.1;
                    dataoff = -2732;
                }
                if(strmath.IsSameAs(_T("X/10")))
                {
                    datamult = 0.1;
                    dataoff = 0;
                }
                if(strmath.IsSameAs(_T("X/100")))
                {
                    datamult = 0.01;
                    dataoff = 0;
                }
            }

           for(int s = m_scalarIdBegin; s <= m_scalarIdEnd; s++)
            {
                if(m_mapTable[s].m_address == tableAddress)
                {
                    if(recognizedtable)
                    {
                        m_mapTable[s].m_recognized = true;
                        m_mapTable[s].m_type = XDF_SCALAR;
                        m_mapTable[s].m_name = title;
                        m_mapTable[s].m_comment = description;
                        m_mapTable[s].m_xunit = _T("");
                        m_mapTable[s].m_yunit = _T("");
                        m_mapTable[s].m_zunit = d_units;
                        m_mapTable[s].m_collabelmult = 1.0;
                        m_mapTable[s].m_collabeloff = 0;
                        m_mapTable[s].m_rowlabelmult = 1.0;
                        m_mapTable[s].m_rowlabeloff = 0;
                        m_mapTable[s].m_datamult = datamult;
                        m_mapTable[s].m_dataoff = dataoff;
                        m_mapTable[s].m_collabelsized = false;
                        m_mapTable[s].m_rowlabelsized = false;

                        if ((datamult != 1.0) || (dataoff != 0))
                            m_mapTable[s].m_datasized = true;
                        else
                            m_mapTable[s].m_datasized = false;

                        break;
                    }
                }
            }

            constant = constant->NextSiblingElement( "XDFCONSTANT" );
            counter++;
        }

        int firstRecognized = 0;
        for(int m = 0; m < m_numberOfTables; m++)
        {
            if (m_mapTable[m].m_recognized)
            {
                if (firstRecognized == 0)
                    firstRecognized = m;
            }
        }

        SelectTable(firstRecognized);

        td5mapeditorView *view = (td5mapeditorView *) GetFirstView();
        view->SetInfoPanelFlags(true, false);
        SetUpdateFlag(GRID_PANEL | INFO_PANEL |GRAPH_PANEL);
        Update();
    }

    return true;
}


int td5mapeditorDoc::ExtractMapIDFromFile()
{
	int mapID = -1;

	if (m_mapName == _T("svlne007svtnp006"))
		mapID = IDR_SVLNE007SVTNP006_MAP;
	if (m_mapName == _T("svdxe003svtnp003"))
		mapID = IDR_SVDXE003SVTNP003_MAP;
	if (m_mapName == _T("svdxe008svtnp006"))
		mapID = IDR_SVDXE008SVTNP006_MAP;
	if (m_mapName == _T("svdxr007svtnp006"))
		mapID = IDR_SVDXR007SVTNP006_MAP;
	if (m_mapName == _T("svlne004svtnp003"))
		mapID = IDR_SVLNE004SVTNP003_MAP;
	if (m_mapName == _T("svlnr005svtnp006"))
		mapID = IDR_SVLNR005SVTNP006_MAP;
	if (m_mapName == _T("svloe005svtnp005"))
		mapID = IDR_SVLOE005SVTNP005_MAP;
	if (m_mapName == _T("svloe005svtnp006"))
		mapID = IDR_SVLOE005SVTNP006_MAP;
	if (m_mapName == _T("swdxe007swtnp006"))
		mapID = IDR_SWDXE007SWTNP006_MAP;
	if (m_mapName == _T("svlne007svtnp005"))
		mapID = IDR_SVLNE007SVTNP005_MAP;
    if (m_mapName == _T("svdxe004svtnp003"))
		mapID = IDR_SVDXE004SVTNP003_MAP;
	if (m_mapName == _T("svdxe006svtnp003"))
		mapID = IDR_SVDXE006SVTNP003_MAP;
	if (m_mapName == _T("svdxe008svtnp005"))
		mapID = IDR_SVDXE008SVTNP005_MAP;
	if (m_mapName == _T("svdxg003svtnp006"))
		mapID = IDR_SVDXG003SVTNP006_MAP;
	if (m_mapName == _T("svlne006svtnp003"))
		mapID = IDR_SVLNE006SVTNP003_MAP;
	if (m_mapName == _T("sthle022sttlp009"))
		mapID = IDR_STHLE022STTLP009_MAP;
	if (m_mapName == _T("sthle022sttlp010"))
		mapID = IDR_STHLE022STTLP010_MAP;
    if (m_mapName == _T("sthde021sttdp009"))
        mapID = IDR_STHDE021STTDP009_MAP;
    if (m_mapName == _T("sthde021sttdp010"))
        mapID = IDR_STHDE021STTDP010_MAP;
    if (m_mapName == _T("sthdr009sttdp009"))
        mapID = IDR_STHDR009STTDP009_MAP;
    if (m_mapName == _T("sthdr009sttdp010"))
        mapID = IDR_STHDR009STTDP010_MAP;
    if (m_mapName == _T("suhde036sutdp012"))
        mapID = IDR_SUHDE036SUTDP012_MAP;
    if (m_mapName == _T("suhde036sutdp014"))
        mapID = IDR_SUHDE036SUTDP014_MAP;
    if (m_mapName == _T("suhdr009sutzp004"))
        mapID = IDR_SUHDR009SUTZP004_MAP;
    if (m_mapName == _T("suhdr009sutzp005"))
        mapID = IDR_SUHDR009SUTZP005_MAP;
    if (m_mapName == _T("surdk004sutzp004"))
        mapID = IDR_SURDK004SUTZP004_MAP;
    if (m_mapName == _T("surdk004sutzp005"))
        mapID = IDR_SURDK004SUTZP005_MAP;
    if (m_mapName == _T("svdxr002svtnp003"))
        mapID = IDR_SVDXR002SVTNP003_MAP;
    if (m_mapName == _T("svdxr005svtnp003"))
        mapID = IDR_SVDXR005SVTNP003_MAP;
    if (m_mapName == _T("svdxr007svtnp005"))
        mapID = IDR_SVDXR007SVTNP005_MAP;
    if (m_mapName == _T("svlnr002svtnp003"))
        mapID = IDR_SVLNR002SVTNP003_MAP;
    if (m_mapName == _T("svlnr004svtnp003"))
        mapID = IDR_SVLNR004SVTNP003_MAP;
    if (m_mapName == _T("svlnr005svtnp005"))
        mapID = IDR_SVLNR005SVTNP005_MAP;
    if (m_mapName == _T("svloe002svtnp003"))
        mapID = IDR_SVLOE002SVTNP003_MAP;
    if (m_mapName == _T("svloe004svtnp003"))
        mapID = IDR_SVLOE004SVTNP003_MAP;
    if (m_mapName == _T("svloj002svtnp003"))
        mapID = IDR_SVLOJ002SVTNP003_MAP;
    if (m_mapName == _T("svloj002svtnp006"))
        mapID = IDR_SVLOJ002SVTNP006_MAP;
    if (m_mapName == _T("svlor002svtnp003"))
        mapID = IDR_SVLOR002SVTNP003_MAP;
    if (m_mapName == _T("svlor004svtnp003"))
        mapID = IDR_SVLOR004SVTNP003_MAP;
    if (m_mapName == _T("svlor005svtnp005"))
        mapID = IDR_SVLOR005SVTNP005_MAP;
    if (m_mapName == _T("svlor005svtnp006"))
        mapID = IDR_SVLOR005SVTNP006_MAP;
    if (m_mapName == _T("swdxe004swtnp004"))
        mapID = IDR_SWDXE004SWTNP004_MAP;
    if (m_mapName == _T("swdxe007swtnp004"))
        mapID = IDR_SWDXE007SWTNP004_MAP;
    if (m_mapName == _T("swdxk001swtnp004"))
        mapID = IDR_SWDXK001SWTNP004_MAP;
    if (m_mapName == _T("swdxk003swtnp004"))
        mapID = IDR_SWDXK003SWTNP004_MAP;
    if (m_mapName == _T("swdxk003swtnp006"))
        mapID = IDR_SWDXK003SWTNP006_MAP;
    if (m_mapName == _T("swdxr002swtnp004"))
        mapID = IDR_SWDXR002SWTNP004_MAP;
    if (m_mapName == _T("swdxr004swtnp004"))
        mapID = IDR_SWDXR004SWTNP004_MAP;
    if (m_mapName == _T("swdxr004swtnp006"))
        mapID = IDR_SWDXR004SWTNP006_MAP;


    return mapID;
}

int td5mapeditorDoc::ExtractTablesIndexAddress()
{
	if ((m_mapName == _T("sthde021sttdp009")) ||
        (m_mapName == _T("sthde021sttdp010")) ||
        (m_mapName == _T("sthdr009sttdp009")) ||
        (m_mapName == _T("sthdr009sttdp010")) ||
        (m_mapName == _T("suhde036sutdp012")) ||
        (m_mapName == _T("suhde036sutdp014")) ||
        (m_mapName == _T("suhdr009sutzp004")) ||
        (m_mapName == _T("suhdr009sutzp005")) ||
        (m_mapName == _T("surdk004sutzp004")) ||
        (m_mapName == _T("surdk004sutzp005")))
    {
        m_mapType = IDR_MAP_TYPE_DISCO_EU2;
        return TABLES_ADDRESS_AREA_BEGIN_DISCO_EU2;
    }
    else
    if ((m_mapName == _T("sthle022sttlp009")) ||
        (m_mapName == _T("sthle022sttlp010")))
    {
        m_mapType = IDR_MAP_TYPE_DEF_EU2;
        return TABLES_ADDRESS_AREA_BEGIN_DEF_EU2;
    }
    else
    {
        m_mapType = IDR_MAP_TYPE_EU3;
        return TABLES_ADDRESS_AREA_BEGIN;
    }

    return 0;
}


wxWord* td5mapeditorDoc::ExtractScalarsIndexAddress()
{
	if (m_mapType == IDR_MAP_TYPE_DISCO_EU2)
    {
        return scalarIndexAddressDiscoEU2;
    }
    else
    if (m_mapType == IDR_MAP_TYPE_DEF_EU2)
    {
        return scalarIndexAddressDefEU2;
    }
    else
    if (m_mapType == IDR_MAP_TYPE_EU3)
    {
        return scalarIndexAddressEU3;
    }

    return NULL;
}


// Scalars Addresses for all Models EU3
wxWord td5mapeditorDoc::scalarIndexAddressEU3[] =
{
    0x06DC, // tb_intgl_deriv_rate      116
    0x06DE, // tb_error_filt_const      117
    0x06E0, // tb_intgl_enbl            118
    0x06E2, // tb_clamp_intgl_hi        119
    0x06E4, // tb_clamp_intgl_lo        120
    0x06E6, // tb_deriv_enbi            121
    0x06E8, // tb_clamp_deriv_hi        122
    0x06EA, // tb_clamp_deriv_lo        123
    0x06EC, // tb_gain_deriv_hi         124
    0x06EE, // tb_gain_deriv_lo         125
    0x06F0, // tb_max_err_intgl         126
    0x06F2, // tb_max_duty_ratio        127
    0x06F4, // tb_min_duty_ratio        128
    0x06F6, // tb_turbo_pwm_freq        129
    0x0746, // tb_under_rev_enbl        130
    0x0748, // tb_under_pres_disbl      131
    0x074A, // tb_under_pres_enbl       132
    0x074C, // tb_over_pres_enbl        133
    0x074E, // tb_over_pres_disbl       134
    0x0750, // tb_over_rev_enbl         135
    0x0752, // tb_engine_speed_disbl    136
    0x0754, // tb_fuel_mass_disbl       137
    0x0756, // tb_engine_speed_enbl     138
    0x0758, // tb_fuel_mass_enbl        139
    0x075A, // tb_pwm_default           140
    0x077C, // ai_limit_min_aap         141
    0x0786, // ai_limit_min_map         142
    0x078A, // ai_limit_min_maf         143
    0x079C, // ai_limit_max_aap         144
    0x07A6, // ai_limit_max_map         145
    0x07AA, // ai_limit_max_maf         146
    0x07BC, // ai_anlg_multip_aap       147
    0x07C6, // ai_anlg_multip_map       148
    0x07CA, // ai_anlg_multip_maf       149
    0x07DC, // ai_anlg_divisor_aap      150
    0x07E6, // ai_anlg_divisor_map      151
    0x07EA, // ai_anlg_divisor_maf      152
    0x07FC, // ai_anlg_offset_aap       153
    0x0806, // ai_anlg_offset_map       154
    0x080A, // ai_anlg_offset_maf       155
	0x0330, // ? (	cc_resume_sw_time )	156
	0x0332, // ? (	cc_set_sw_time )	157
	0x0334, // ? (	cc_sw_stuck_time )	158
	0x0336, // ? (	cc_spd_chk_period )	159
	0x0338, // cc_eng_spd_disi			160
	0x033A, // cc_eng_spd_disd			161
	0x033C, // cc_veh_spd_rate_disi		162
	0x033E, // cc_veh_spd_ctrl_disd		163
	0x0340, // ? (	cc_pedal_max_time )	164
	0x0342, // ? (	cc_pedal_enbi )	    165
	0x0344, // ? (	cc_veh_accel_filt )	166
	0x0346, // ? (	cc_tap_up_interval )167
	0x0348, // cc_vehicle_spd_disd		168
	0x034A, // cc_vehicle_spd_disi		169
	0x034C, // cc_vehicle_spd_inc		170
	0x034E, // cc_vehicle_spd_dec		171
	0x0350, // cc_max_accel_rate		172
	0x0352, // cc_max_decel_rate		173
	0x0354, // cc_tap_up_limit			174
	0x0356, // ? (	cc_error_filt_rate )175
	0x0358, // ? (	cc_int_der_lock_rt )176
	0x035A, // ? (	cc_int_der_open_rt )177
	0x035C, // ? (	cc_error_filt_const 178
	0x035E, // ? (	cc_intgl_deadband )	179
	0x0360, // ? (	cc_max_err_intgl )	180
	0x0362, // cc_clamp_intgl_hi		181
	0x0364, // cc_clamp_intgl_lo		182
	0x0366, // ? (	cc_deriv_enbd )	    183
	0x0368, // cc_clamp_deriv_hi		184
	0x036A, // cc_clamp_deriv_lo		185
	0x036C, // ? (	cc_gain_deriv_hi )	186
	0x036E, // ? (	cc_gain_deriv_lo )	187
	0x0370, // cc_dr_holding			188
	0x0372, // ? (	cc_initial_I_mult )	189
	0x0374, // ? (	cc_init_accel_rate )190
	0x0376, // ? (	cc_demand_filter )	191
	0x0378, // ? (	cc_open_intgl_mult )192
	0x037A, // ? (	cc_lock_intgl_mult )193
	0x037C, // cc_5_prptn_mult			194
	0x037E, // cc_4_prptn_mult			195
	0x0380, // cc_3_prptn_mult			196

    0xFFFF  // END MARKER
};

// Scalars Addresses for Defender EU2
wxWord td5mapeditorDoc::scalarIndexAddressDefEU2[] =
{
    0x05F2, // 	tb_intgl_deriv_rate	    	78
    0x05F4, // 	tb_error_filt_const	    	79
    0x05F6, // 	tb_intgl_enbl	    		80
    0x05F8, // 	tb_clamp_intgl_hi	    	81
    0x05FA, // 	tb_clamp_intgl_lo	    	82
    0x05FC, // 	tb_deriv_enbi	    		83
    0x05FE, // 	tb_clamp_deriv_hi	    	84
    0x0600, // 	tb_clamp_deriv_lo	    	85
    0x0602, // 	tb_gain_deriv_hi	    	86
    0x0604, // 	tb_gain_deriv_lo	    	87
    0x0606, // 	tb_max_err_intgl	    	88
    0x0608, // 	tb_max_duty_ratio	    	89
    0x060A, // 	tb_min_duty_ratio	    	90
    0x060C, // 	tb_turbo_pwm_freq	    	91
    0x0638, // 	tb_under_rev_enbl	    	92
    0x063A, // 	tb_under_pres_disbl	    	93
    0x063C, // 	tb_under_pres_enbl	    	94
    0x063E, // 	tb_over_pres_enbl	    	95
    0x0640, // 	tb_over_pres_disbl	    	96
    0x0642, // 	tb_over_rev_enbl	    	97
    0x0644, // 	tb_engine_speed_disbl	    98
    0x0646, // 	tb_fuel_mass_disbl	    	99
    0x0648, // 	tb_engine_speed_enbl	    100
    0x064A, // 	tb_fuel_mass_enbl	    	101
    0x064C, // 	tb_pwm_default	    		102
    0x065C, // 	ai_limit_min_aap	    	103
    0x0666, // 	ai_limit_min_map	    	104
    0x066A, // 	ai_limit_min_maf	    	105
    0x067C, // 	ai_limit_max_aap	    	106
    0x0686, // 	ai_limit_max_map	    	107
    0x068A, // 	ai_limit_max_maf	    	108
    0x069C, // 	ai_anlg_multip_aap	    	109
    0x06A6, // 	ai_anlg_multip_map	    	110
    0x06AA, // 	ai_anlg_multip_maf	    	111
    0x06BC, // 	ai_anlg_divisor_aap	    	112
    0x06C6, // 	ai_anlg_divisor_map	    	113
    0x06CA, // 	ai_anlg_divisor_maf	    	114
    0x06DC, // 	ai_anlg_offset_aap	    	115
    0x06E6, // 	ai_anlg_offset_map	    	116
    0x06EA, // 	ai_anlg_offset_maf	    	117

    0xFFFF  // END MARKER
};

// Scalars Addresses for Discovery EU2
wxWord td5mapeditorDoc::scalarIndexAddressDiscoEU2[] =
{
    0x05FA, // 	tb_intgl_deriv_rate		79
    0x05FC, // 	tb_error_filt_const		80
    0x05FE, // 	tb_intgl_enbl			81
    0x0600, // 	tb_clamp_intgl_hi		82
    0x0602, // 	tb_clamp_intgl_lo		83
    0x0604, // 	tb_deriv_enbi			84
    0x0606, // 	tb_clamp_deriv_hi		85
    0x0608, // 	tb_clamp_deriv_lo		86
    0x060A, // 	tb_gain_deriv_hi		87
    0x060C, // 	tb_gain_deriv_lo		88
    0x060E, // 	tb_max_err_intgl		89
    0x0610, // 	tb_max_duty_ratio		90
    0x0612, // 	tb_min_duty_ratio		91
    0x0614, // 	tb_turbo_pwm_freq		92
    0x0640, // 	tb_under_rev_enbl		93
    0x0642, // 	tb_under_pres_disbl		94
    0x0644, // 	tb_under_pres_enbl		95
    0x0646, // 	tb_over_pres_enbl		96
    0x0648, // 	tb_over_pres_disbl		97
    0x064A, // 	tb_over_rev_enbl		98
    0x064C, // 	tb_engine_speed_disbl	99
    0x064E, // 	tb_fuel_mass_disbl		100
    0x0650, // 	tb_engine_speed_enbl	101
    0x0652, // 	tb_fuel_mass_enbl		102
    0x0654, // 	tb_pwm_default			103
    0x0664, // 	ai_limit_min_aap		104
    0x066E, // 	ai_limit_min_map		105
    0x0672, // 	ai_limit_min_maf		106
    0x0684, // 	ai_limit_max_aap		107
    0x068E, // 	ai_limit_max_map		108
    0x0692, // 	ai_limit_max_maf		109
    0x06A4, // 	ai_anlg_multip_aap		110
    0x06AE, // 	ai_anlg_multip_map		111
    0x06B2, // 	ai_anlg_multip_maf		112
    0x06C4, // 	ai_anlg_divisor_aap		113
    0x06CE, // 	ai_anlg_divisor_map		114
    0x06D2, // 	ai_anlg_divisor_maf		115
    0x06E4, // 	ai_anlg_offset_aap		116
    0x06EE, // 	ai_anlg_offset_map		117
    0x06F2, // 	ai_anlg_offset_maf		118

    0xFFFF  // END MARKER
};
