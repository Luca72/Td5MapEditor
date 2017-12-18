/***************************************************************
 * Name:      td5mapeditorDoc.h
 * Purpose:   Code for Application Class
 * Author:    Luca Veronesi (luca72@libero.it)
 * Created:   2010-12-07
 * Copyright: Luca Veronesi ()
 * License:
 **************************************************************/

#ifndef __DOCSAMPLEH__
#define __DOCSAMPLEH__

#define MAP_FILE_LENGTH                     118798
#define TABLES_ADDRESS_AREA_BEGIN           118526
#define TABLES_ADDRESS_AREA_BEGIN_DISCO_EU2 113032
#define TABLES_ADDRESS_AREA_BEGIN_DEF_EU2   112882
#define MAPNAME_ADDRESS_BEGIN               102418
#define TAG_ADDRESS_BEGIN                   118448

#define MAX_NUM_OF_TABLES                   /*128*/ 2024
#define MAP_FILE_LENGTH_WORD                (MAP_FILE_LENGTH / sizeof(wxWord))
#define MAP_TAG_ADDRESS_BEGIN_WORD          (TAG_ADDRESS_BEGIN / sizeof(wxWord))
#define MAP_MAPNAME_ADDRESS_BEGIN_WORD      (MAPNAME_ADDRESS_BEGIN / sizeof(wxWord))

#define GRID_PANEL      0x0100
#define INFO_PANEL      0x0200
#define GRAPH_PANEL     0x0400

#include "wx/docview.h"
#include "wx/cmdproc.h"

#include "td5mapTable.h"
#include "wizNewMap.h"

class td5mapeditorDoc: public wxDocument
{
    DECLARE_DYNAMIC_CLASS(td5mapeditorDoc)
    private:
    public:
        td5mapeditorDoc(void);
        ~td5mapeditorDoc(void);

    #if wxUSE_STD_IOSTREAM
        wxSTD ostream& SaveObject(wxSTD ostream& text_stream);
        wxSTD istream& LoadObject(wxSTD istream& text_stream);
    #else
        wxOutputStream& SaveObject(wxOutputStream& stream);
        wxInputStream& LoadObject(wxInputStream& stream);
    #endif

        virtual bool OnNewDocument();
        virtual bool OnOpenDocument(const wxString& filename);
        virtual bool OnSaveDocument(const wxString& filename);
        virtual bool OnCloseDocument();

        void ExportTuning(const wxString& ifileName);
        bool ImportTuning(const wxString& ifileName);

        void Update(wxView* sender = NULL);

        void SetUpdateFlag(long panel);
        void ResetUpdateFlag(long panel);
        bool GetUpdateFlag(long panel);

        wxString GetMapTag(){return m_mapTag;};
        void SetMapTag(wxString tag){m_mapTag = tag;WriteTagToFile();};

        bool SelectTable(int index);
        int GetSelectedTable(){return m_selectedTable;};

        td5mapTable *GetSelectedMapTable(){return &m_mapTable[m_selectedTable];};

        short GetSelMapCurrentValue(int col, int row) {return m_mapTable[m_selectedTable].m_tableData[col][row].current;};
        void SetSelMapCurrentValue(int col, int row, short value) {m_mapTable[m_selectedTable].m_tableData[col][row].current = value;};
        short GetSelMapBaseValue(int col, int row) {return m_mapTable[m_selectedTable].m_tableData[col][row].base;};
        void SetSelMapBaseValue(int col, int row, short value) {m_mapTable[m_selectedTable].m_tableData[col][row].base = value;};
        short GetSelMapDiffValue(int col, int row) {return GetSelMapCurrentValue(col, row) - GetSelMapBaseValue(col, row);};
        short GetSelMapColHeaderCurrentValue(int col) {return m_mapTable[m_selectedTable].m_headerCol[col].current;};
        short GetSelMapRowHeaderCurrentValue(int row) {return m_mapTable[m_selectedTable].m_headerRow[row].current;};
        short GetSelMapRowLabelDiffRawValue(int row) {return m_mapTable[m_selectedTable].m_headerRow[row].current - m_mapTable[m_selectedTable].m_headerRow[row].base;};
        short GetSelMapColLabelDiffRawValue(int col) {return m_mapTable[m_selectedTable].m_headerCol[col].current - m_mapTable[m_selectedTable].m_headerCol[col].base;};
        short GetSelMapRowHeaderBaseRawValue(int row) {return m_mapTable[m_selectedTable].m_headerRow[row].base;};
        short GetSelMapColHeaderBaseRawValue(int col) {return m_mapTable[m_selectedTable].m_headerCol[col].base;};

        int GetNumberOfTables(){return m_numberOfTables;}

        void SetSelectionRange(ewxRange range) {m_selectionRange = range;};
        ewxRange GetSelectionRange() {return m_selectionRange;};

        td5mapTable *GetMapTable(int index) {return &m_mapTable[index];}
        wxString GetMapName(){return m_mapName;};
        int GetMapID(){return m_mapID;};


    protected:
        wxString ExtractMapNameFromFile();
        wxString ExtractTagFromFile();
        void WriteTagToFile();
        int ExtractMapIDFromFile();
        wxWord *ExtractMapResource(int nResourceId, bool& bOk);
        int ExtractTablesIndexAddress();

    protected:
        wxString m_mapName;
        wxString m_mapTag;
        wxString m_fileName;
        wxWord *m_mapBaseData;
        wxWord *m_mapExternalData;
        short m_mapType;
        wxWord m_mapFileData[MAP_FILE_LENGTH_WORD];
        td5mapTable m_mapTable[MAX_NUM_OF_TABLES];
        int m_numberOfTables;
        int m_selectedTable;
        bool m_updateGridPanel;
        bool m_updateInfoPanel;
        bool m_updateGraphCanvas;
        bool m_mapFound;
        int  m_mapID;
        ewxRange m_selectionRange;
};

#endif
