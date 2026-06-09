// Functions.cpp : implementation of global functions
//

#include "functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

static wxWord SumBE16RangeSkipping(const wxByte* data, size_t start, size_t endExclusive, size_t skipOffset)
{
    wxUint32 sum = 0;

    for (size_t off = start; off < endExclusive; off += 2)
    {
        if (off == skipOffset)
            continue;

        sum += ReadBE16AtByteOffset(data, off);
    }

    return static_cast<wxWord>(sum & 0xFFFF);
}

void FirmwareAndTablesChecksum(wxWord *pMapFileData)
{
    wxByte* bytes = reinterpret_cast<wxByte*>(pMapFileData);

    WriteBE16AtByteOffset(bytes, 0x01CFE8, 0x4D4D);

    wxWord fwSum = SumBE16RangeSkipping(bytes, 0x000401, 0x019011, 0x00041B);
    wxWord fwCorrection = static_cast<wxWord>((0xD343 - fwSum) & 0xFFFF);
    WriteBE16AtByteOffset(bytes, 0x00041B, fwCorrection);

    wxWord tbSum = SumBE16RangeSkipping(bytes, 0x019010, 0x01CFEA, 0x01CFE6);
    wxWord tbCorrection = static_cast<wxWord>((0x005E - tbSum) & 0xFFFF);
    WriteBE16AtByteOffset(bytes, 0x01CFE6, tbCorrection);
}


wxWord NanocomChecksum(wxWord *pMapFileData)
{
  int count = (MAP_FILE_LENGTH - 2) / sizeof(wxWord);

  union DATA {wxWord word; wxByte byte[2];};
  DATA data; data.word = 0; data.byte[0] = 0; data.byte[1] = 0;

  union SUM {wxUint32 dword; wxWord word[2];};
  SUM sum; sum.dword = 0; sum.word[0] = 0; sum.word[1] = 0;

  // Main summing loop
  while(count > 0)
  {
	data.word = *pMapFileData;
    sum.dword = sum.dword + data.byte[0] + data.byte[1];
	pMapFileData++;
    count = count - 1;
  }

  return(sum.word[0]);
}


wxString ExtractFileName(wxString path)
{
    return path.Right(path.Length() - (path.Find(wxChar(47), true) + 1));
}
