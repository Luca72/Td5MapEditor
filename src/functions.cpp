// Functions.cpp : implementation of global functions
//

#include "functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAP_FILE_LENGTH 118798

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

wxWord Checksum(wxWord *pMapFileData)
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
