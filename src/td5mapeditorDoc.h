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
#define FUEL_PART_ADDRESS_BEGIN             102416
#define CHEKSUM_ADDRESS_BEGIN               (MAP_FILE_LENGTH - 2)

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

        bool LoadXDF(const wxString& ifileName);

        void Update(wxView* sender = NULL);

        void SetUpdateFlag(long panel);
        void ResetUpdateFlag(long panel);
        bool GetUpdateFlag(long panel);

        wxString GetMapTag(){return m_mapTag;};
        void SetMapTag(wxString tag){m_mapTag = tag;WriteTagToFile();};

        bool SelectTable(int index);
        int GetSelectedTable(){return m_selectedTable;};
        void ResetTableToBaseMap();

        td5mapTable *GetSelectedMapTable(){return &m_mapTable[m_selectedTable];};

        short GetSelMapCurrentRawValue(int col, int row) {return m_mapTable[m_selectedTable].m_tableData[col][row].current;};
        void SetSelMapCurrentRawValue(int col, int row, short value) {m_mapTable[m_selectedTable].m_tableData[col][row].current = value;};

        short GetSelMapBaseRawValue(int col, int row) {return m_mapTable[m_selectedTable].m_tableData[col][row].base;};
        void SetSelMapBaseRawValue(int col, int row, short value) {m_mapTable[m_selectedTable].m_tableData[col][row].base = value;};

        short GetSelMapDiffRawValue(int col, int row) {return GetSelMapCurrentRawValue(col, row) - GetSelMapBaseRawValue(col, row);};
        short GetSelMapRowLabelDiffRawValue(int row) {return m_mapTable[m_selectedTable].m_headerRow[row].current - m_mapTable[m_selectedTable].m_headerRow[row].base;};
        short GetSelMapColLabelDiffRawValue(int col) {return m_mapTable[m_selectedTable].m_headerCol[col].current - m_mapTable[m_selectedTable].m_headerCol[col].base;};

        float GetSelMapCurrentSizedValue(int col, int row){return m_mapTable[m_selectedTable].ApplyDataSizer(m_mapTable[m_selectedTable].m_tableData[col][row].current);};
        void SetSelMapCurrentSizedValue(int col, int row, float value){m_mapTable[m_selectedTable].m_tableData[col][row].current = m_mapTable[m_selectedTable].RestoreDataRaw(value);};

        float GetSelMapBaseSizedValue(int col, int row){return m_mapTable[m_selectedTable].ApplyDataSizer(m_mapTable[m_selectedTable].m_tableData[col][row].base);};
        void SetSelMapBaseSizedValue(int col, int row, float value){m_mapTable[m_selectedTable].m_tableData[col][row].base = m_mapTable[m_selectedTable].RestoreDataRaw(value);};

        float GetSelMapDiffSizedValue(int col, int row){return m_mapTable[m_selectedTable].ApplyDataSizer(GetSelMapCurrentRawValue(col, row) - GetSelMapBaseRawValue(col, row));};

        short GetSelMapRowHeaderCurrentRawValue(int row) {return m_mapTable[m_selectedTable].m_headerRow[row].current;};
        float GetSelMapRowHeaderCurrentSizedValue(int row){return m_mapTable[m_selectedTable].ApplyRowLabelSizer(m_mapTable[m_selectedTable].m_headerRow[row].current);};
        short GetSelMapRowHeaderBaseRawValue(int row) {return m_mapTable[m_selectedTable].m_headerRow[row].base;};
        float GetSelMapRowHeaderBaseSizedValue(int row){return m_mapTable[m_selectedTable].ApplyRowLabelSizer(m_mapTable[m_selectedTable].m_headerRow[row].base);};
        void SetSelMapRowHeaderCurrentRawValue(int row, short value) {m_mapTable[m_selectedTable].m_headerRow[row].current = value;};
        void SetSelMapRowHeaderCurrentSizedValue(int row, float value) {m_mapTable[m_selectedTable].m_headerRow[row].current = m_mapTable[m_selectedTable].RestoreRowLabelRaw(value);};

        short GetSelMapColHeaderCurrentRawValue(int col) {return m_mapTable[m_selectedTable].m_headerCol[col].current;};
        float GetSelMapColHeaderCurrentSizedValue(int col){return m_mapTable[m_selectedTable].ApplyColLabelSizer(m_mapTable[m_selectedTable].m_headerCol[col].current);};
        short GetSelMapColHeaderBaseRawValue(int col) {return m_mapTable[m_selectedTable].m_headerCol[col].base;};
        float GetSelMapColHeaderBaseSizedValue(int col){return m_mapTable[m_selectedTable].ApplyColLabelSizer(m_mapTable[m_selectedTable].m_headerCol[col].base);};
        void SetSelMapColHeaderCurrentRawValue(int col, short value) {m_mapTable[m_selectedTable].m_headerCol[col].current = value;};
        void SetSelMapColHeaderCurrentSizedValue(int col, float value) {m_mapTable[m_selectedTable].m_headerCol[col].current = m_mapTable[m_selectedTable].RestoreColLabelRaw(value);};


        int GetNumberOfTables(){return m_numberOfTables;}

        wxWord GetCurrentRawValue(int address);
        wxWord GetBaseRawValue(int address);

        void SetSelectionRange(ewxRange range) {m_selectionRange = range;};
        ewxRange GetSelectionRange() {return m_selectionRange;};

        td5mapTable *GetMapTable(int index) {return &m_mapTable[index];}
        wxString GetMapName(){return m_mapName;};
        int GetMapID(){return m_mapID;};
        wxWord GetMapAddressFromIndex(int index){ return m_mapAddresses[index]; };
        wxWord GetScalarAddressFromIndex(int index){ return m_scalarAddresses[index]; };
        short GetMapIdBegin(){return m_mapIdBegin;};
        short GetMapIdEnd(){return m_mapIdEnd;};
        short GetScalarIdBegin(){return m_scalarIdBegin;};
        short GetScalarIdEnd(){return m_scalarIdEnd;};


    protected:
        wxString ExtractMapNameFromFile();
        wxString ExtractTagFromFile();
        void WriteTagToFile();
        int ExtractMapIDFromFile();
        wxWord *ExtractMapResource(int nResourceId, bool& bOk);
        int ExtractTablesIndexAddress();
        wxWord* ExtractScalarsIndexAddress();

    protected:
        wxString m_mapName;
        wxString m_mapTag;
        wxString m_fileName;
        wxWord *m_mapBaseData;
        wxWord *m_mapExternalData;
        wxWord *m_scalarAddresses;
        wxWord m_mapAddresses[MAX_NUM_OF_TABLES];
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

        short m_mapIdBegin;
        short m_mapIdEnd;
        short m_scalarIdBegin;
        short m_scalarIdEnd;


        static wxWord scalarIndexAddressEU3[];
        static wxWord scalarIndexAddressDefEU2[];
        static wxWord scalarIndexAddressDiscoEU2[];
};



#endif
