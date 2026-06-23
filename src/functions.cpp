// Functions.cpp : implementation of global functions
//

#include "functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
    constexpr size_t NANOCOM_FILE_LENGTH              = 0x01D00E;   // 118798

    constexpr size_t NANOCOM_HEADER_SIZE              = 0x000009;
    constexpr size_t NANOCOM_TABLES_ADDRESS_OFF       = 0x000006;

    constexpr size_t ECU_FIRMWARE_SIZE                = 0x02A000;
    constexpr size_t FIRMWARE_VERIFY_WORD_RAW_OFF     = 0x000410;
    constexpr size_t FIRMWARE_CORRECTION_WORD_RAW_OFF = 0x000412;

    constexpr size_t ECU_TABLES_SIZE                  = 0x004000;
    constexpr size_t TABLES_VERIFY_WORD_RAW_OFF       = 0x000012;

    constexpr wxWord ECU_FIRMWARE_TARGET_SUM          = 0xAA55;
    constexpr wxWord ECU_TABLES_TARGET_SUM            = 0xA5A5;

    constexpr size_t FIRMWARE_VERIFY_WORD_FILE_OFF =
        NANOCOM_HEADER_SIZE + FIRMWARE_VERIFY_WORD_RAW_OFF;

    constexpr size_t FIRMWARE_CORRECTION_WORD_FILE_OFF =
        NANOCOM_HEADER_SIZE + FIRMWARE_CORRECTION_WORD_RAW_OFF;
}

wxWord LoHi2HiLo(wxWord word)
{
	union ADDRESS {wxWord word; wxByte byte[2];};

	ADDRESS addressLoHi, addressHiLo;

	addressLoHi.word = word;

	addressHiLo.byte[0] = addressLoHi.byte[1];
	addressHiLo.byte[1] = addressLoHi.byte[0];

	return addressHiLo.word;
}

wxWord HiLo2LoHi(wxWord word)
{
	union ADDRESS {wxWord word; wxByte byte[2];};

	ADDRESS addressLoHi, addressHiLo;

	addressLoHi.word = word;

	addressHiLo.byte[0] = addressLoHi.byte[1];
	addressHiLo.byte[1] = addressLoHi.byte[0];

	return addressHiLo.word;
}

static wxWord ReadBE16AtByteOffset(const wxByte* data, size_t off)
{
    return static_cast<wxWord>((data[off] << 8) | data[off + 1]);
}

static void WriteBE16AtByteOffset(wxByte* data, size_t off, wxWord value)
{
    data[off]     = static_cast<wxByte>((value >> 8) & 0xFF);
    data[off + 1] = static_cast<wxByte>(value & 0xFF);
}

static size_t ReadBE24AtByteOffset(const wxByte* data, size_t off)
{
    return (static_cast<size_t>(data[off])     << 16) |
           (static_cast<size_t>(data[off + 1]) << 8)  |
            static_cast<size_t>(data[off + 2]);
}

static wxWord ComputeFirmwareCorrectionFromMapFile(const wxByte* bytes)
{
    const size_t firmwareStart = 
        ReadBE24AtByteOffset(bytes, NANOCOM_TABLES_ADDRESS_OFF); // Firmware start address

    // Nanocom variant block:
    // file 0x000009 -> raw firmware offset 0x000000
    const size_t variantSize = firmwareStart - NANOCOM_HEADER_SIZE;

    wxUint32 sum = 0;

    for (size_t rawOff = 0; rawOff < ECU_FIRMWARE_SIZE; rawOff += 2)
    {
        if (rawOff == FIRMWARE_CORRECTION_WORD_RAW_OFF)   // Skip correction WORD from checksum
            continue;

        wxWord word;
                
        if (rawOff == FIRMWARE_VERIFY_WORD_RAW_OFF)   // Verify WORD assume 0xFFFF (ECU will switch it to 0x5AA5)
        {
            // ECU/programming state
            word = 0xFFFF;
        }
        else
        {
            wxByte hi =
                rawOff < variantSize
                    ? bytes[NANOCOM_HEADER_SIZE + rawOff]
                    : 0xFF;

            wxByte lo =
                (rawOff + 1) < variantSize
                    ? bytes[NANOCOM_HEADER_SIZE + rawOff + 1]
                    : 0xFF;

            word = static_cast<wxWord>((hi << 8) | lo);
        }

        sum += word;
    }

    return static_cast<wxWord>((ECU_FIRMWARE_TARGET_SUM - (sum & 0xFFFF)) & 0xFFFF);
}

static wxWord ComputeTablesCorrectionFromMapFile(const wxByte* bytes)
{
    const size_t tablesStart = ReadBE24AtByteOffset(bytes, NANOCOM_TABLES_ADDRESS_OFF);   // Firmware start address
    const size_t tablesSize  = ReadBE16AtByteOffset(bytes, tablesStart);    // Firmware size in .map file

    const size_t tablesCorrWordRawOff = tablesSize - 2;         // Offset of the CORRECTION WORD

    wxUint32 sum = 0;

    for (size_t rawOff = 0; rawOff < ECU_TABLES_SIZE; rawOff += 2)
    {
        if (rawOff == tablesCorrWordRawOff)    
            continue;       // Skip correction WORD from checksum

        wxWord word;

        if (rawOff == TABLES_VERIFY_WORD_RAW_OFF)
        {
            word = 0xFFFF;  // Verification WORD is always treated as "to be checked"
        }
        else if (rawOff <= tablesSize)
        {
            word = ReadBE16AtByteOffset(bytes, tablesStart + rawOff);   // Use actual file contents for tail
        }
        else
        {            
            word = 0xFFFF;  // Missing WORDs up to full 0x4000 ECU map area
        }

        sum += word;
    }

    return static_cast<wxWord>((ECU_TABLES_TARGET_SUM - (sum & 0xFFFF)) & 0xFFFF);
}


void WriteFirmwareAndTablesCorrection(wxWord *pMapFileData)
{
    wxByte* bytes = reinterpret_cast<wxByte*>(pMapFileData);

    const size_t tablesStart = ReadBE24AtByteOffset(bytes, NANOCOM_TABLES_ADDRESS_OFF);
    const size_t tablesSize  = ReadBE16AtByteOffset(bytes, tablesStart);

    const size_t tablesCorrWordRawOff = 
        tablesStart + tablesSize - 2;                       // Offset of the tables CORRECTION WORD

    const wxWord firmwareCorrection = ComputeFirmwareCorrectionFromMapFile(bytes);
    const wxWord tablesCorrection = ComputeTablesCorrectionFromMapFile(bytes);

    WriteBE16AtByteOffset(bytes, FIRMWARE_CORRECTION_WORD_FILE_OFF, firmwareCorrection);
    WriteBE16AtByteOffset(bytes, tablesCorrWordRawOff, tablesCorrection);
}

void WriteNanocomChecksum(wxWord *pMapFileData)
{
    wxByte* bytes = reinterpret_cast<wxByte*>(pMapFileData);
    const wxWord checksum = ComputeNanocomChecksum(pMapFileData);

    WriteBE16AtByteOffset(bytes, NANOCOM_FILE_LENGTH - sizeof(wxWord), checksum);
}


wxWord ComputeFirmwareCorrection(const wxWord *pMapFileData)
{
    const wxByte* bytes = reinterpret_cast<const wxByte*>(pMapFileData);
    return ComputeFirmwareCorrectionFromMapFile(bytes);
}

wxWord GetFirmwareCorrection(const wxWord *pMapFileData)
{
    const wxByte* bytes = reinterpret_cast<const wxByte*>(pMapFileData);

    return ReadBE16AtByteOffset(bytes, FIRMWARE_CORRECTION_WORD_FILE_OFF);
}


wxWord ComputeTablesCorrection(const wxWord *pMapFileData)
{
    const wxByte* bytes = reinterpret_cast<const wxByte*>(pMapFileData);

    return ComputeTablesCorrectionFromMapFile(bytes);
}

wxWord GetTablesCorrection(const wxWord *pMapFileData)
{
    const wxByte* bytes = reinterpret_cast<const wxByte*>(pMapFileData);

    const size_t tablesStart = ReadBE24AtByteOffset(bytes, NANOCOM_TABLES_ADDRESS_OFF);
    const size_t tablesSize  = ReadBE16AtByteOffset(bytes, tablesStart);

    const size_t mapCorrWordFileOff = tablesStart + tablesSize - 2;

    return ReadBE16AtByteOffset(bytes, mapCorrWordFileOff);
}


wxWord ComputeNanocomChecksum(const wxWord *pMapFileData)
{
    wxUint32 sum = 0;
    const wxByte* bytes = reinterpret_cast<const wxByte*>(pMapFileData);

    for (size_t i = 0; i < NANOCOM_FILE_LENGTH - 2; ++i)
        sum += bytes[i];

    return static_cast<wxWord>(sum & 0xFFFF);
}

wxWord GetNanocomChecksum(const wxWord *pMapFileData)
{
    const wxByte* bytes = reinterpret_cast<const wxByte*>(pMapFileData);

    return ReadBE16AtByteOffset(bytes, NANOCOM_FILE_LENGTH - sizeof(wxWord));
}

wxWord GetFirmwareVerificationWord(const wxWord *pMapFileData)
{
    const wxByte* bytes = reinterpret_cast<const wxByte*>(pMapFileData);

    return ReadBE16AtByteOffset(bytes, FIRMWARE_VERIFY_WORD_FILE_OFF);
}

wxWord GetTablesVerificationWord(const wxWord *pMapFileData)
{
    const wxByte* bytes = reinterpret_cast<const wxByte*>(pMapFileData);

    const size_t tablesStart = ReadBE24AtByteOffset(bytes, NANOCOM_TABLES_ADDRESS_OFF);

    return ReadBE16AtByteOffset(bytes, tablesStart + TABLES_VERIFY_WORD_RAW_OFF);
}

bool SetVerificationWordsToUnchecked(wxWord *pMapFileData)
{
    if (pMapFileData == nullptr)
        return false;

    wxByte* bytes = reinterpret_cast<wxByte*>(pMapFileData);

    const size_t tablesStart = ReadBE24AtByteOffset(bytes, NANOCOM_TABLES_ADDRESS_OFF);

    // Firmware verification word
    WriteBE16AtByteOffset(bytes, FIRMWARE_VERIFY_WORD_FILE_OFF, 0xFFFF);

    // Map verification word
    WriteBE16AtByteOffset(bytes, tablesStart + TABLES_VERIFY_WORD_RAW_OFF, 0xFFFF);

    return true;
}

bool SetVerificationWordsToChecked(wxWord *pMapFileData)
{
    if (pMapFileData == nullptr)
        return false;

    wxByte* bytes = reinterpret_cast<wxByte*>(pMapFileData);

    const size_t tablesStart = ReadBE24AtByteOffset(bytes, NANOCOM_TABLES_ADDRESS_OFF);

    // Firmware verification word
    WriteBE16AtByteOffset(bytes, FIRMWARE_VERIFY_WORD_FILE_OFF, 0x5AA5);

    // Map verification word
    WriteBE16AtByteOffset(bytes, tablesStart + TABLES_VERIFY_WORD_RAW_OFF, 0x5AA5);

    return true;
}

/*
wxString ExtractFileName(wxString path)
{
    return path.Right(path.Length() - (path.Find(wxChar(47), true) + 1));
}
*/
#include <wx/filename.h>
wxString ExtractFileName(wxString path)
{
    return wxFileName(path).GetFullName();
}
