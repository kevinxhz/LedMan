//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "About.h"
#include "SysConfig.h"
#include "DotFont24.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
__fastcall TfrmAbout::TfrmAbout(TComponent* Owner)
	: TForm(Owner)
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
#ifdef AUTO_SIZE_TEXT
		Caption = "关于 YledMan LED 显示屏管理 V3.951E";
#else
		Caption = "关于 YledMan LED 显示屏管理 V3.953";
#endif
		Label1->Caption = "本程序当前配置适用于:";
		btnOk->Caption = "确定";
	}
	else
	{
#ifdef AUTO_SIZE_TEXT
		Caption = "About Yuepoch LED Panle Manager V3.951E";
#else
		Caption = "About Yuepoch LED Panle Manager V3.953";
#endif
		Label1->Caption = "Current config is suitable for:";
		btnOk->Caption = "OK";
	}

	switch (sysConfig.m_nCardType)
	{
	case CARD_TYPE_GS7020BF_HEB:
		editCardType->Text = "GS7020EF-H";
		break;
	case CARD_TYPE_GS7020BF:
	case CARD_TYPE_GS7020BF_W:
		editCardType->Text = "GS7020BF GS7020EF";
		break;
	case CARD_TYPE_GS9026:
		editCardType->Text = "GS9026";
		break;
	case CARD_TYPE_GS2012A:
		editCardType->Text = "GS2012A CS2012A";
		break;
	case CARD_TYPE_GS7030B:
		editCardType->Text = "GS7030B CS7030B";
		break;
	case CARD_TYPE_GS7030BV2:
		editCardType->Text = "GS7030BV2";
		break;
	case CARD_TYPE_YL2010AR:
		editCardType->Text = "YL2010AR";
		break;
	case CARD_TYPE_GS7020BX:
		editCardType->Text = "GS7020BX GS7020EX";
		break;
	case CARD_TYPE_GS7022B:
		editCardType->Text = "GS7022B(E)(H) CS7022B(H)";
		break;
	case CARD_TYPE_GS7020BQ:
		editCardType->Text = "GS7020B/EQ CS7020BQ";
		break;
	case CARD_TYPE_PD102B:
		editCardType->Text = "PD102B";
		break;
	case CARD_TYPE_GS7020F:
		editCardType->Text = "GS7020F";
		break;
	case CARD_TYPE_GS7020BP:
		editCardType->Text = "GS7020B/EP CS7020BP";
		break;
	case CARD_TYPE_GS2012A_S:
		editCardType->Text = "GS2012A-S";
		break;
	case CARD_TYPE_GS7024E:
		editCardType->Text = "GS7024E";
		break;
	case CARD_TYPE_WF2012B:
		editCardType->Text = "WF2012B";
		break;
	case CARD_TYPE_GS2012AS_64:
		editCardType->Text = "GS2012A/BS-64";
		break;
	case CARD_TYPE_GS9020S:
		editCardType->Text = "GS9020S";
		break;
	case CARD_TYPE_GS7020HS:
		editCardType->Text = "GS7020HS GS7020IS";
		break;
	case CARD_TYPE_GS2012B:
		editCardType->Text = "GS2012B CS2012B";
		break;
	case CARD_TYPE_GS9020:
		editCardType->Text = "GS9020";
		break;
	case CARD_TYPE_YL2010C:
		editCardType->Text = "YL2010C";
		break;
	case CARD_TYPE_YL2010A:
		editCardType->Text = "YL2010A";
		break;
	case CARD_TYPE_YL2010A_GPS_JTX:
		editCardType->Text = "YL2010A_GPS_JTX";
		break;
	case CARD_TYPE_GS7020H:
		editCardType->Text = "GS7020H CS7020H";
		break;
	case CARD_TYPE_GS9025:
		editCardType->Text = "GS9025";
		break;
	case CARD_TYPE_GS9023:
		editCardType->Text = "GS9023";
		break;
	case CARD_TYPE_GS9020F_GPS:
		editCardType->Text = "GS9020F-GPS";
		break;
	case CARD_TYPE_GS7020HFA:
		editCardType->Text = "GS7020HFA CS7020HFA";
		break;
	case CARD_TYPE_SN2P:
		editCardType->Text = "SN2P";
		break;
	default:
		editCardType->Text = "GS7020B/E CS7020B";
		break;
	}
}
//---------------------------------------------------------------------------

int TfrmAbout::OutputByte(int hFile, BYTE byte, int nCount)
{
/*	char sTemp[32];

	if (nCount != 11)
	{
		sprintf(sTemp, "0x%02X, ", byte);
		nCount++;
	}
	else
	{
		sprintf(sTemp, "0x%02X,\r\n", byte);
		nCount = 0;
	}
	FileWrite(hFile, sTemp, strlen(sTemp));
*/
	return nCount;
}

//---------------------------------------------------------------------------
void __fastcall TfrmAbout::Button1Click(TObject *Sender)
{
/*	char c = 0x20;
	int i, x, y;
	BYTE byte;
	int hFile;
	char szHanzi[3];
	int nQu, nWei;
	int nCount = 0;

	szHanzi[2] = 0;
	Image1->Width = 24;
	Image1->Height = 24;
	Image1->Canvas->Brush->Color = clBlack;
	Image1->Canvas->Brush->Style = bsSolid;
	Image1->Canvas->Font->Color = clRed;

	//////////////////////////////
	//6*12宋体 ASCII
	hFile = FileCreate(ExtractFilePath(Application->ExeName) + "ASC12.c", fmOpenWrite);
	Image1->Canvas->Font->Name = "宋体";
	Image1->Canvas->Font->Size = 10;
	Image1->Canvas->Font->Style = TFontStyles();

	for (i = 0; i < 96; i++)
	{
		Image1->Canvas->TextOutW(0, 0, UnicodeString(c++));
		for (y = 0; y < 12; y++)
		{
			byte = 0;
			for (x = 0; x < 6; x++)
			{
				byte <<= 1;
				if (Image1->Canvas->Pixels[x][y] & 0xE0E0E0)
					byte |= 1;
			}
			byte <<= 2;
			nCount = OutputByte(hFile, byte, nCount);
		}
	}

	FileClose(hFile);

	//////////////////////////////
	//12*12宋体汉字
	hFile = FileCreate(ExtractFilePath(Application->ExeName) + "HZK12_S.c", fmOpenWrite);
	for (nQu = 0xA1; nQu <= (0xA1 + 87); nQu++)
	{
		for (nWei = 0xA1; nWei <= 0xFE; nWei++)
		{
			szHanzi[0] = nQu;
			szHanzi[1] = nWei;
			TextOutA(Image1->Canvas->Handle, 0, 0, szHanzi, 2);
			for (y = 0; y < 12; y++)
			{
				byte = 0;
				for (x = 0; x < 6; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x][y] & 0xE0E0E0)
						byte |= 1;
				}
				byte <<= 2;
				nCount = OutputByte(hFile, byte, nCount);

				byte = 0;
				for (x = 0; x < 6; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 6][y] & 0xE0E0E0)
						byte |= 1;
				}
				byte <<= 2;
				nCount = OutputByte(hFile, byte, nCount);
			}
		}
	}
	FileClose(hFile);
*/
}
//---------------------------------------------------------------------------

/*	char c = 0x20;
	int i, x, y;
	BYTE byte;
	int hFile;
	char szHanzi[3];
	int nQu, nWei;

	szHanzi[2] = 0;
	Image1->Width = 24;
	Image1->Height = 24;
	Image1->Canvas->Brush->Color = clBlack;
	Image1->Canvas->Brush->Style = bsSolid;
	Image1->Canvas->Font->Color = clRed;

	//////////////////////////////
	//8*16宋体 ASCII
	hFile = FileCreate(ExtractFilePath(Application->ExeName) + "GS9025_ASC16_S.fon", fmOpenWrite);
	Image1->Canvas->Font->Name = "宋体";
	Image1->Canvas->Font->Size = 12;
	Image1->Canvas->Font->Style = TFontStyles();

	for (i = 0; i < 96; i++)
	{
		Image1->Canvas->TextOutW(0, 0, UnicodeString(c++));
		for (y = 0; y < 16; y++)
		{
			byte = 0;
			for (x = 0; x < 8; x++)
			{
				byte <<= 1;
				if (Image1->Canvas->Pixels[x][y] & 0xE0E0E0)
					byte |= 1;
			}
			FileWrite(hFile, &byte, 1);
		}
	}

	FileClose(hFile);

	//////////////////////////////
	//16*16宋体汉字
	hFile = FileCreate(ExtractFilePath(Application->ExeName) + "GS9025_HZK16_S.fon", fmOpenWrite);
	for (nQu = 0xA1; nQu <= 0xA3; nQu++)
	{
		for (nWei = 0xA1; nWei <= 0xFE; nWei++)
		{
			szHanzi[0] = nQu;
			szHanzi[1] = nWei;
			TextOutA(Image1->Canvas->Handle, 0, 0, szHanzi, 2);
			for (y = 0; y < 16; y++)
			{
				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);
			}
		}
	}
	for (nQu = 0xB0; nQu <= (0xA1 + 87); nQu++)
	{
		for (nWei = 0xA1; nWei <= 0xFE; nWei++)
		{
			szHanzi[0] = nQu;
			szHanzi[1] = nWei;
			TextOutA(Image1->Canvas->Handle, 0, 0, szHanzi, 2);
			for (y = 0; y < 16; y++)
			{
				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);
			}
		}
	}
	FileClose(hFile);

	//////////////////////////////
	//8*16黑体 ASCII
	c = 0x20;
	hFile = FileCreate(ExtractFilePath(Application->ExeName) + "GS9025_ASC16_H.fon", fmOpenWrite);
	Image1->Canvas->Font->Name = "黑体";
	Image1->Canvas->Font->Size = 12;
	Image1->Canvas->Font->Style = TFontStyles();

	for (i = 0; i < 96; i++)
	{
		Image1->Canvas->TextOutW(0, 0, UnicodeString(c));
		for (y = 0; y < 16; y++)
		{
			byte = 0;
			for (x = 0; x < 8; x++)
			{
				byte <<= 1;
				if (Image1->Canvas->Pixels[x][y] & 0xE0E0E0)
					byte |= 1;
			}
			FileWrite(hFile, &byte, 1);
		}
		c++;
		if (c == '0')
			Sleep(5000);
	}

	FileClose(hFile);

	//////////////////////////////
	//16*16黑体汉字
	hFile = FileCreate(ExtractFilePath(Application->ExeName) + "GS9025_HZK16_H.fon", fmOpenWrite);
	for (nQu = 0xA1; nQu <= 0xA3; nQu++)
	{
		for (nWei = 0xA1; nWei <= 0xFE; nWei++)
		{
			szHanzi[0] = nQu;
			szHanzi[1] = nWei;
			TextOutA(Image1->Canvas->Handle, 0, 0, szHanzi, 2);
			for (y = 0; y < 16; y++)
			{
				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);
			}
		}
	}
	for (nQu = 0xB0; nQu <= (0xA1 + 87); nQu++)
	{
		for (nWei = 0xA1; nWei <= 0xFE; nWei++)
		{
			szHanzi[0] = nQu;
			szHanzi[1] = nWei;
			TextOutA(Image1->Canvas->Handle, 0, 0, szHanzi, 2);
			for (y = 0; y < 16; y++)
			{
				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);
			}
		}
	}
	FileClose(hFile);

	//////////////////////////////
	//12*24宋体 ASCII
	hFile = FileCreate(ExtractFilePath(Application->ExeName) + "GS9025_ASC24_S.fon", fmOpenWrite);
	FileWrite(hFile, TDotFont24::ucAsciiFont + 0x20 * 48, 96 * 48);
	FileClose(hFile);

	//////////////////////////////
	//24*24宋体 汉字
	hFile = FileCreate(ExtractFilePath(Application->ExeName) + "GS9025_HZK24_S.fon", fmOpenWrite);
	FileWrite(hFile, TDotFont24::ucHanziFont, 3 * 94 * 72);
	FileWrite(hFile, TDotFont24::ucHanziFont + 9 * 94 * 72, 72 * 94 * 72);
	FileClose(hFile);

	//////////////////////////////
	//12*24黑体 ASCII
	c = 0x20;
	hFile = FileCreate(ExtractFilePath(Application->ExeName) + "GS9025_ASC24_H.fon", fmOpenWrite);
	Image1->Canvas->Font->Name = "黑体";
	Image1->Canvas->Font->Size = 18;
	Image1->Canvas->Font->Style = TFontStyles();

	for (i = 0; i < 96; i++)
	{
		Image1->Canvas->TextOutW(0, 0, UnicodeString(c));
		for (y = 0; y < 24; y++)
		{
			byte = 0;
			for (x = 0; x < 8; x++)
			{
				byte <<= 1;
				if (Image1->Canvas->Pixels[x][y] & 0xE0E0E0)
					byte |= 1;
			}
			FileWrite(hFile, &byte, 1);

			byte = 0;
			for (x = 0; x < 4; x++)
			{
				byte <<= 1;
				if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
					byte |= 1;
			}
			byte <<= 4;
			FileWrite(hFile, &byte, 1);
		}
		c++;
	}

	FileClose(hFile);

	//////////////////////////////
	//24*24黑体汉字
	hFile = FileCreate(ExtractFilePath(Application->ExeName) + "GS9025_HZK24_H.fon", fmOpenWrite);
	for (nQu = 0xA1; nQu <= 0xA3; nQu++)
	{
		for (nWei = 0xA1; nWei <= 0xFE; nWei++)
		{
			szHanzi[0] = nQu;
			szHanzi[1] = nWei;
			TextOutA(Image1->Canvas->Handle, 0, 0, szHanzi, 2);
			for (y = 0; y < 24; y++)
			{
				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 16][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);
			}
		}
	}
	for (nQu = 0xB0; nQu <= (0xA1 + 87); nQu++)
	{
		for (nWei = 0xA1; nWei <= 0xFE; nWei++)
		{
			szHanzi[0] = nQu;
			szHanzi[1] = nWei;
			TextOutA(Image1->Canvas->Handle, 0, 0, szHanzi, 2);
			for (y = 0; y < 24; y++)
			{
				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 16][y] & 0xE0E0E0)
						byte |= 1;
				}
				FileWrite(hFile, &byte, 1);
			}
		}
	}
	FileClose(hFile);
*/

/*	AnsiString s = "01234567890";
	int x, y;
	char* p = s.c_str();
	BYTE* pDot;
	BYTE c;
	char buf[128];

	m_hFile = FileCreate(ExtractFilePath(Application->ExeName) + "1234.c", fmOpenWrite);
	if ((*p & 0x80) == 0)
	{
		if (*p < 0x20)
			pDot = ucHanziFont + (82 * 94) * 128;
		else
			pDot = ucHanziFont + (82 * 94) * 128 + (*p - 0x20) * 64;

		for (y = 0; y < 32; y++)
		{
			for (x = 0; x < 8; x++)
				dwColors[31 * 32 - y * 32 + x] = (*pDot & (0x80 >> x)) ? dwFColor : dwBColor;
			pDot++;
			for (x = 0; x < 8; x++)
				dwColors[31 * 32 - y * 32 + x + 8] = (*pDot & (0x80 >> x)) ? dwFColor : dwBColor;
			pDot++;
		}

		nWidth = 16;
	}
	FileWrite(m_hFile, "[ACT]\r\n", 7);
	else
	{
		BYTE ucQu = *p - 0xA1;
		if (ucQu >= 15)
			ucQu -= 6;
		BYTE ucWei = *(p + 1) - 0xA1;
		int nPos = ucQu * 94;
		nPos += ucWei;
		nPos *= 128;
		if (nPos >= nHanziDotFontDataSize - 96 * 64)
			nPos = 0;
		pDot = ucHanziFont + nPos;

		for (y = 0; y < 32; y++)
		{
			for (x = 0; x < 8; x++)
				dwColors[31 * 32 - y * 32 + x] = (*pDot & (0x80 >> x)) ? dwFColor : dwBColor;
			pDot++;
			for (x = 0; x < 8; x++)
				dwColors[31 * 32 - y * 32 + x + 8] = (*pDot & (0x80 >> x)) ? dwFColor : dwBColor;
			pDot++;
			for (x = 0; x < 8; x++)
				dwColors[31 * 32 - y * 32 + x + 16] = (*pDot & (0x80 >> x)) ? dwFColor : dwBColor;
			pDot++;
			for (x = 0; x < 8; x++)
				dwColors[31 * 32 - y * 32 + x + 24] = (*pDot & (0x80 >> x)) ? dwFColor : dwBColor;
			pDot++;
		}

		nWidth = 32;
	}

	SetDIBits(hMemDC, hBitmap, 0, 32, dwColors, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS);
	BitBlt(Canvas->Handle, xPos, yPos, nWidth, 32, hMemDC, 0, 0, SRCCOPY);
	DeleteObject(hBitmap);
	DeleteObject(hMemDC);
*/

