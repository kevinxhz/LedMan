//---------------------------------------------------------------------------
#ifndef DotFont32H
#define DotFont32H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

class TDotFont32
{
public:
	static BYTE ucHanziFont[];
	static int nHanziDotFontDataSize;
	static int DrawChar(TCanvas* Canvas, int xPos, int yPos, DWORD crForeColor, DWORD crBackColor, wchar_t wc);
	static int GetTextWidth(UnicodeString ws, int nCount);
};

#endif
