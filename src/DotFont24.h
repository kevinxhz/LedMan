//---------------------------------------------------------------------------

#ifndef DotFont24H
#define DotFont24H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

class TDotFont24
{
public:
	static BYTE ucAsciiFont[];
	static BYTE ucHanziFont[];
	static int nHanziDotFontDataSize;
	static int DrawChar(TCanvas* Canvas, int xPos, int yPos, DWORD crForeColor, DWORD crBackColor, wchar_t wc);
	static int GetTextWidth(UnicodeString ws, int nCount);
};

#endif
