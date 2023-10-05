//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SysConfig.h"
#include "TextEditForm.h"
#include "act.h"
#include "CustMsgDlg.h"
#include "DotFont16.h"
#include "DotFont24.h"
#include "DotFont32.h"
#include "math.h"
#define PI	3.1415826
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TwndTextEdit::TwndTextEdit(TComponent* Owner)
	: TForm(Owner)
{
	m_nAlignStyle = ALIGN_CENTER;
	m_bAlignVertCenter = TRUE;
	m_nRow = m_nCol = 0;
	m_nSelectStartRow = m_nSelectStartCol = m_nSelectEndRow = m_nSelectEndCol = 0;
	m_bSelect = false;
	m_bMouseStartSelect = FALSE;
	m_ucTextColor = COLOR_RED | 0x40;
	m_nEditScreenWidth = 128;
	m_nEditScreenHeight = 16;
	m_bDirty = false;
	m_bDotFont = false;
	pic = NULL;
	m_bRefreshEnabled = true;

	m_wsListText = new TStringList;
	m_wsListActive = new TStringList;
	m_wsListColor = new TStringList;
}
//---------------------------------------------------------------------------

void TwndTextEdit::ChangeLanguage()
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		mnuCut->Caption = "剪切(&T)";
		mnuCopy->Caption = "复制(&C)";
		mnuPaste->Caption = "粘贴(&P)";
		mnuDelete->Caption = "删除(&D)";
		mnuSelectAll->Caption = "选择全部(&A)";
	}
	else
	{
		mnuCut->Caption = "Cu&t";
		mnuCopy->Caption = "&Copy";
		mnuPaste->Caption = "&Paste";
		mnuDelete->Caption = "&Delete";
		mnuSelectAll->Caption = "Seletc &All";
	}
}
//---------------------------------------------------------------------------

void TwndTextEdit::DrawCicleClock()
{
	int x, y;
	int x1, y1;
	int r = (m_nEditScreenWidth < m_nEditScreenHeight) ? m_nEditScreenWidth / 2 : m_nEditScreenHeight / 2;
	int xCenter =  m_nEditScreenWidth / 2;
	int yCenter =  m_nEditScreenHeight / 2;

	Canvas->Pen->Color = clRed;
	Canvas->Pen->Width = 1;
	Canvas->Pen->Style = psSolid;
	Canvas->Brush->Color = clRed;
	Canvas->Brush->Style = bsSolid;

	for (int i = 0; i < 12; i++)
	{
		x = (r - 3) * sin(2 * PI * i / 12) + xCenter;
		y = (r - 3) * cos(2 * PI * i / 12) + yCenter;
		if (i % 3 == 0)
			Canvas->Ellipse(x - 3, y - 3, x + 3, y + 3);
		else
			Canvas->Ellipse(x - 2, y - 2, x + 2, y + 2);
	}

	SYSTEMTIME st;
	::GetLocalTime(&st);

	double secAng = 2.0 * PI * st.wSecond /60.0;
	double minAng = 2.0 * PI * (st.wMinute + st.wSecond / 60.0) / 60.0;
	double hourAng = 2.0 * PI * (st.wHour +st.wMinute / 60.0) / 12.0;

	int secHandLength = (int)(0.9 * r);
	int minHandLength = (int)(0.7 * r);
	int hourHandLength = (int)(0.5 * r);

	x = hourHandLength * sin(hourAng) + xCenter;
	y = -hourHandLength * cos(hourAng) + yCenter;
	x1 = -2 * sin(hourAng) + xCenter;
	y1 = 2 * cos(hourAng) + yCenter;
	Canvas->Pen->Width = 3;
	Canvas->MoveTo(x1, y1);
	Canvas->LineTo(x, y);

	x = minHandLength * sin(minAng) + xCenter;
	y = -minHandLength * cos(minAng) + yCenter;
	x1 = -2 * sin(minAng) + xCenter;
	y1 = 2 * cos(minAng) + yCenter;
	Canvas->Pen->Width = 2;
	Canvas->MoveTo(x1, y1);
	Canvas->LineTo(x, y);

	x = secHandLength * sin(secAng) + xCenter;
	y = -secHandLength * cos(secAng) + yCenter;
	x1 = -4 * sin(secAng) + xCenter;
	y1 = 4 * cos(secAng) + yCenter;
	Canvas->Pen->Width = 1;
	Canvas->MoveTo(x1, y1);
	Canvas->LineTo(x, y);
}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::FormPaint(TObject *Sender)
{
	int nRow, nCol;
	int nPicX, nPicY;
	bool bCircleClock = false;

	if (!m_bRefreshEnabled)
		return;

	if (m_wsListText->Count)
	{
		if (m_wsListText->Strings[0].Compare("A") == 0)
		{
			if (m_wsListActive->Strings[0].Compare("N") == 0)
				bCircleClock = true;
		}
	}

	if (bCircleClock)
		DrawCicleClock();
	else
	{
		if (!m_bDotFont)
			DrawWindowsText();
		else
			DrawDotFontText();

		try
		{
			nPicX = StrToInt(frmMain->editPicPosX->Text);
		}
		catch (Exception &ex)
		{
			nPicX = 0;
		}

		try
		{
			nPicY = StrToInt(frmMain->editPicPosY->Text);
		}
		catch (Exception &ex)
		{
			nPicY = 0;
		}

		if (pic != NULL)
			::BitBlt(
			Canvas->Handle,
			nPicX,
			nPicY,
			pic->Width,
			pic->Height,
			pic->Bitmap->Canvas->Handle,
			0,
			0,
			SRCCOPY);
	}

	Canvas->Pen->Color = clSilver;
	Canvas->Pen->Width = 1;
	Canvas->Pen->Style = psSolid;

	nRow = m_nEditScreenWidth;
	for (;;)
	{
		Canvas->MoveTo(nRow - 1, 0);
		Canvas->LineTo(nRow - 1, Height);
		nRow += m_nEditScreenWidth;
		if (nRow >= Width)
			break;
	}

	nCol = m_nEditScreenHeight;
	for (;;)
	{
		Canvas->MoveTo(0, nCol - 1);
		Canvas->LineTo(Width, nCol - 1);
		nCol += m_nEditScreenHeight;
		if (nCol >= Height)
			break;
	}
}
//---------------------------------------------------------------------------

void TwndTextEdit::DrawWindowsText()
{
	int nRow, x, y;
	int nTopSpace;
	int nRowsPerScreen;
	int nScreen = 0;
	int nOffsetX;
	wchar_t *ws, *wsColor, *wsActive;
	SIZE sz;
	int nCount;
	DWORD crFColor, crBColor;

	if (m_bAlignVertCenter)
		nRowsPerScreen = 1;
	else
		nRowsPerScreen = m_nEditScreenHeight / m_nFontHeight;
	nTopSpace = (m_nEditScreenHeight - nRowsPerScreen * m_nFontHeight) / (nRowsPerScreen + 1);
	Canvas->Pen->Style = psClear;

	y = nTopSpace;
	for (nRow = 0; nRow < m_wsListText->Count; nRow++)
	{
		nOffsetX = GetOffsetX(nRow);
		ws = m_wsListText->Strings[nRow].c_str();
		wsColor = m_wsListColor->Strings[nRow].c_str();
		wsActive = m_wsListActive->Strings[nRow].c_str();
		x = nOffsetX;

		Canvas->Font->Color = clWhite;
		Canvas->Brush->Color = clBlack;
		TextOutW(Canvas->Handle, nOffsetX, y + nScreen * m_nEditScreenHeight, ws, m_wsListText->Strings[nRow].Length());
		nCount = 1;
		while (*ws)
		{
			GetTextExtentPoint32W(Canvas->Handle, m_wsListText->Strings[nRow].c_str(), nCount, &sz);
			utils.GetRGBColor(*wsColor, *wsActive, &crFColor, &crBColor);
			Canvas->Brush->Style = bsSolid;
			Canvas->Brush->Color = TColor(crFColor ^ crBColor);
			SetROP2(Canvas->Handle, R2_MASKPEN);
			Canvas->Rectangle(x, y + nScreen * m_nEditScreenHeight, nOffsetX + sz.cx + 1, y + nScreen * m_nEditScreenHeight + sz.cy + 1);
			Canvas->Brush->Color = TColor(crBColor);
			SetROP2(Canvas->Handle, R2_XORPEN);
			Canvas->Rectangle(x, y + nScreen * m_nEditScreenHeight, nOffsetX + sz.cx + 1, y + nScreen * m_nEditScreenHeight + sz.cy + 1);

			x = sz.cx;
			ws++;
			wsActive++;
			wsColor++;
			nCount++;
		}
		SetROP2(Canvas->Handle, R2_COPYPEN);

		if (IsSelectedRow(nRow))
		{
			TRect rect;
			Canvas->Brush->Color = TColor(RGB(0xC0, 0xC0, 0xC0));

			if (m_nSelectStartRow == m_nSelectEndRow)
			{
				int nLeftCol = (m_nSelectStartCol < m_nSelectEndCol) ? m_nSelectStartCol : m_nSelectEndCol;
				int nRightCol = (m_nSelectStartCol > m_nSelectEndCol) ? m_nSelectStartCol : m_nSelectEndCol;
				int nLeft = GetTextWidth(m_wsListText->Strings[nRow], nLeftCol);
				int nRight = GetTextWidth(m_wsListText->Strings[nRow], nRightCol);
				rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX + nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
			}
			else if (m_nSelectStartRow < m_nSelectEndRow)
			{
				if (nRow == m_nSelectStartRow)
				{
					int nLeft = GetTextWidth(m_wsListText->Strings[nRow], m_nSelectStartCol);
					int nRight = GetTextWidth(m_wsListText->Strings[nRow], m_wsListText->Strings[nRow].Length());
					rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX + nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
				}
				else if (nRow == m_nSelectEndRow)
				{
					int nLeft = 0;
					int nRight = GetTextWidth(m_wsListText->Strings[nRow], m_nSelectEndCol);
					rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX + nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
				}
				else
				{
					int nLeft = 0;
					int nRight = GetTextWidth(m_wsListText->Strings[nRow], m_wsListText->Strings[nRow].Length());
					rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX + nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
				}
			}
			else
			{
				if (nRow == m_nSelectEndRow)
				{
					int nLeft = GetTextWidth(m_wsListText->Strings[nRow], m_nSelectEndCol);
					int nRight = GetTextWidth(m_wsListText->Strings[nRow], m_wsListText->Strings[nRow].Length());
					rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX + nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
				}
				else if (nRow == m_nSelectStartRow)
				{
					int nLeft = 0;
					int nRight = GetTextWidth(m_wsListText->Strings[nRow], m_nSelectStartCol);
					rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX +  nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
				}
				else
				{
					int nLeft = 0;
					//int nRight = Canvas->TextWidth(m_wsListText->Strings[nRow]);
					int nRight = GetTextWidth(m_wsListText->Strings[nRow], m_wsListText->Strings[nRow].Length());
					rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX + nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
				}
			}
			Canvas->Pen->Color = TColor(RGB(0xC0, 0xC0, 0xC0));
			SetROP2(Canvas->Handle, R2_MERGEPEN);
			Canvas->Rectangle(rect);
			Canvas->Brush->Color = clBlack;
		}

		y += nTopSpace + m_nFontHeight;
		if (y + m_nFontHeight > m_nEditScreenHeight)
		{
			y = nTopSpace;
			nScreen++;
		}
	}
}
//---------------------------------------------------------------------------

void TwndTextEdit::DrawDotFontText()
{
	int nRow, x, y;
	int nTopSpace;
	int nRowsPerScreen;
	int nScreen = 0;
	int nOffsetX;
	wchar_t *ws, *wsColor, *wsActive;
	DWORD crFColor, crBColor;

	if (m_bAlignVertCenter)
		nRowsPerScreen = 1;
	else
		nRowsPerScreen = m_nEditScreenHeight / m_nFontHeight;
	nTopSpace = (m_nEditScreenHeight - nRowsPerScreen * m_nFontHeight) / (nRowsPerScreen + 1);

	y = nTopSpace;
	for (nRow = 0; nRow < m_wsListText->Count; nRow++)
	{
		nOffsetX = GetOffsetX(nRow);
		ws = m_wsListText->Strings[nRow].c_str();
		wsColor = m_wsListColor->Strings[nRow].c_str();
		wsActive = m_wsListActive->Strings[nRow].c_str();
		x = nOffsetX;

		while (*ws)
		{
			utils.GetRGBColor(*wsColor, *wsActive, &crFColor, &crBColor);
			if (m_nDotFontSize == 16)
				x += TDotFont16::DrawChar(Canvas, x, y + nScreen * m_nEditScreenHeight, crFColor, crBColor, *ws);
			else if (m_nDotFontSize == 24)
				x += TDotFont24::DrawChar(Canvas, x, y + nScreen * m_nEditScreenHeight, crFColor, crBColor, *ws);
			else
				x += TDotFont32::DrawChar(Canvas, x, y + nScreen * m_nEditScreenHeight, crFColor, crBColor, *ws);
			ws++;
			wsActive++;
			wsColor++;
		}

		if (IsSelectedRow(nRow))
		{
			TRect rect;
			Canvas->Brush->Color = TColor(RGB(0xC0, 0xC0, 0xC0));

			if (m_nSelectStartRow == m_nSelectEndRow)
			{
				int nLeftCol = (m_nSelectStartCol < m_nSelectEndCol) ? m_nSelectStartCol : m_nSelectEndCol;
				int nRightCol = (m_nSelectStartCol > m_nSelectEndCol) ? m_nSelectStartCol : m_nSelectEndCol;
				int nLeft = GetTextWidth(m_wsListText->Strings[nRow], nLeftCol);
				int nRight = GetTextWidth(m_wsListText->Strings[nRow], nRightCol);
				rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX + nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
			}
			else if (m_nSelectStartRow < m_nSelectEndRow)
			{
				if (nRow == m_nSelectStartRow)
				{
					int nLeft = GetTextWidth(m_wsListText->Strings[nRow], m_nSelectStartCol);
					int nRight = GetTextWidth(m_wsListText->Strings[nRow], m_wsListText->Strings[nRow].Length());
					rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX + nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
				}
				else if (nRow == m_nSelectEndRow)
				{
					int nLeft = 0;
					int nRight = GetTextWidth(m_wsListText->Strings[nRow], m_nSelectEndCol);
					rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX + nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
				}
				else
				{
					int nLeft = 0;
					int nRight = GetTextWidth(m_wsListText->Strings[nRow], m_wsListText->Strings[nRow].Length());
					rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX + nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
				}
			}
			else
			{
				if (nRow == m_nSelectEndRow)
				{
					int nLeft = GetTextWidth(m_wsListText->Strings[nRow], m_nSelectEndCol);
					int nRight = GetTextWidth(m_wsListText->Strings[nRow], m_wsListText->Strings[nRow].Length());
					rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX + nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
				}
				else if (nRow == m_nSelectStartRow)
				{
					int nLeft = 0;
					int nRight = GetTextWidth(m_wsListText->Strings[nRow], m_nSelectStartCol);
					rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX +  nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
				}
				else
				{
					int nLeft = 0;
					int nRight = GetTextWidth(m_wsListText->Strings[nRow], m_wsListText->Strings[nRow].Length());
					rect = TRect(nOffsetX + nLeft, y + nScreen * m_nEditScreenHeight, nOffsetX + nRight, y + nScreen * m_nEditScreenHeight + m_nFontHeight);
				}
			}
			Canvas->Pen->Color = TColor(RGB(0xC0, 0xC0, 0xC0));
			SetROP2(Canvas->Handle, R2_MERGEPEN);
			Canvas->Rectangle(rect);
			Canvas->Brush->Color = clBlack;
		}

		y += nTopSpace + m_nFontHeight;
		if (y + m_nFontHeight > m_nEditScreenHeight)
		{
			y = nTopSpace;
			nScreen++;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall  TwndTextEdit::WinMsgProc(TMessage &msg)
{
	BOOL bShift;

	if (msg.Msg == 0xBD00)
	{
		switch (msg.WParam)
		{
		case VK_LEFT:
			bShift = (GetKeyState(VK_LSHIFT) >> 8) || (GetKeyState(VK_RSHIFT) >> 8);
			if (bShift)
			{
				if (!m_bSelect)
				{
					m_nSelectStartRow = m_nRow;
					m_nSelectStartCol = m_nCol;
					m_bSelect = true;
				}
				if (m_nCol > 0)
				{
					int nOldPos = GetTextWidth(m_wsListText->Strings[m_nRow], m_nCol);
					while (m_nCol > 0)
					{
						m_nCol--;
						InvalidateChar(m_nRow, m_nCol);
						int nPos = GetTextWidth(m_wsListText->Strings[m_nRow], m_nCol);
						if (nPos != nOldPos)
							break;
					}
					SetCaret();
					m_nSelectEndRow = m_nRow;
					m_nSelectEndCol = m_nCol;
				}
				else if (m_nRow > 0)
				{
					m_nRow--;
					m_nCol = m_wsListText->Strings[m_nRow].Length();
					SetCaret();
					m_nSelectEndRow = m_nRow;
					m_nSelectEndCol = m_nCol;
					InvalidateChar(m_nRow, m_nCol);
				}
			}
			else
			{
				BOOL bSelectExist = (m_nSelectStartRow != m_nSelectEndRow) || (m_nSelectStartCol != m_nSelectEndCol);
				if (m_bSelect || bSelectExist)
				{
					InvalidateSelectedArea();
					m_bSelect = false;
					m_nSelectStartRow = m_nSelectStartCol = m_nSelectEndRow = m_nSelectEndCol = 0;
				}
				if (m_nCol > 0)
				{
					int nOldPos = GetTextWidth(m_wsListText->Strings[m_nRow], m_nCol);
					while (m_nCol > 0)
					{
						m_nCol--;
						int nPos = GetTextWidth(m_wsListText->Strings[m_nRow], m_nCol);
						if (nPos != nOldPos)
							break;
					}
					SetCaret();
				}
				else if (m_nRow > 0)
				{
					m_nRow--;
					m_nCol = m_wsListText->Strings[m_nRow].Length();
					SetCaret();
				}
			}
			break;
		case VK_RIGHT:
			bShift = (GetKeyState(VK_LSHIFT) >> 8) || (GetKeyState(VK_RSHIFT) >> 8);
			if (bShift)
			{
				if (!m_bSelect)
				{
					m_nSelectStartRow = m_nRow;
					m_nSelectStartCol = m_nCol;
					m_bSelect = true;
				}
				if (m_nCol < m_wsListText->Strings[m_nRow].Length())
				{
					InvalidateChar(m_nRow, m_nCol);

					int nOldPos = GetTextWidth(m_wsListText->Strings[m_nRow], m_nCol);
					while (m_nCol < m_wsListText->Strings[m_nRow].Length())
					{
						m_nCol++;
						InvalidateChar(m_nRow, m_nCol);
						int nPos = GetTextWidth(m_wsListText->Strings[m_nRow], m_nCol);
						if (nPos != nOldPos)
							break;
					}
					SetCaret();
					m_nSelectEndRow = m_nRow;
					m_nSelectEndCol = m_nCol;
				}
				else if (m_nRow < m_wsListText->Count - 1)
				{
					m_nRow++;
					m_nCol = 0;
					SetCaret();
					m_nSelectEndRow = m_nRow;
					m_nSelectEndCol = m_nCol;
					InvalidateChar(m_nRow, m_nCol);
				}
			}
			else
			{
				BOOL bSelectExist = (m_nSelectStartRow != m_nSelectEndRow) || (m_nSelectStartCol != m_nSelectEndCol);
				if (m_bSelect || bSelectExist)
				{
					InvalidateSelectedArea();
					m_bSelect = false;
					m_nSelectStartRow = m_nSelectStartCol = m_nSelectEndRow = m_nSelectEndCol = 0;
				}
				if (m_nCol < m_wsListText->Strings[m_nRow].Length())
				{
					int nOldPos = GetTextWidth(m_wsListText->Strings[m_nRow], m_nCol);
					while (m_nCol < m_wsListText->Strings[m_nRow].Length())
					{
						m_nCol++;
						int nPos = GetTextWidth(m_wsListText->Strings[m_nRow], m_nCol);
						if (nPos != nOldPos)
							break;
					}
					SetCaret();
				}
				else if (m_nRow < m_wsListText->Count - 1)
				{
					m_nRow++;
					m_nCol = 0;
					SetCaret();
				}
			}
			break;
		case VK_UP:
			bShift = (GetKeyState(VK_LSHIFT) >> 8) || (GetKeyState(VK_RSHIFT) >> 8);
			if (bShift)
			{
				if (!m_bSelect)
				{
					m_nSelectStartRow = m_nRow;
					m_nSelectStartCol = m_nCol;
					m_bSelect = true;
				}
				if (m_nRow > 0)
				{
					InvalidateRow(m_nRow);
					m_nRow--;
					if (m_nCol > m_wsListText->Strings[m_nRow].Length())
						m_nCol = m_wsListText->Strings[m_nRow].Length();
					SetCaret();
					m_nSelectEndRow = m_nRow;
					m_nSelectEndCol = m_nCol;
					InvalidateRow(m_nRow);
				}
			}
			else
			{
				BOOL bSelectExist = (m_nSelectStartRow != m_nSelectEndRow) || (m_nSelectStartCol != m_nSelectEndCol);
				if (m_bSelect || bSelectExist)
				{
					InvalidateSelectedArea();
					m_bSelect = false;
					m_nSelectStartRow = m_nSelectStartCol = m_nSelectEndRow = m_nSelectEndCol = 0;
				}
				if (m_nRow > 0)
				{
					m_nRow--;
					if (m_nCol > m_wsListText->Strings[m_nRow].Length())
						m_nCol = m_wsListText->Strings[m_nRow].Length();
					SetCaret();
				}
			}
			break;
		case VK_DOWN:
			bShift = (GetKeyState(VK_LSHIFT) >> 8) || (GetKeyState(VK_RSHIFT) >> 8);
			if (bShift)
			{
				if (!m_bSelect)
				{
					m_nSelectStartRow = m_nRow;
					m_nSelectStartCol = m_nCol;
					m_bSelect = true;
				}
				if (m_nRow < m_wsListText->Count - 1)
				{
					InvalidateRow(m_nRow);
					m_nRow++;
					if (m_nCol > m_wsListText->Strings[m_nRow].Length())
						m_nCol = m_wsListText->Strings[m_nRow].Length();
					SetCaret();
					m_nSelectEndRow = m_nRow;
					m_nSelectEndCol = m_nCol;
					InvalidateRow(m_nRow);
				}
			}
			else
			{
				BOOL bSelectExist = (m_nSelectStartRow != m_nSelectEndRow) || (m_nSelectStartCol != m_nSelectEndCol);
				if (m_bSelect || bSelectExist)
				{
					InvalidateSelectedArea();
					m_bSelect = false;
					m_nSelectStartRow = m_nSelectStartCol = m_nSelectEndRow = m_nSelectEndCol = 0;
				}
				if (m_nRow < m_wsListText->Count - 1)
				{
					m_nRow++;
					if (m_nCol > m_wsListText->Strings[m_nRow].Length())
						m_nCol = m_wsListText->Strings[m_nRow].Length();
					SetCaret();
				}
			}
			break;
		case VK_HOME:
			bShift = (GetKeyState(VK_LSHIFT) >> 8) || (GetKeyState(VK_RSHIFT) >> 8);
			if (bShift)
			{
				if (!m_bSelect)
				{
					m_nSelectStartRow = m_nRow;
					m_nSelectStartCol = m_nCol;
					m_bSelect = true;
				}
				InvalidateRow(m_nRow);
				m_nCol = 0;
				SetCaret();
				m_nSelectEndRow = m_nRow;
				m_nSelectEndCol = m_nCol;
			}
			else
			{
				BOOL bSelectExist = (m_nSelectStartRow != m_nSelectEndRow) || (m_nSelectStartCol != m_nSelectEndCol);
				if (m_bSelect || bSelectExist)
				{
					InvalidateSelectedArea();
					m_bSelect = false;
					m_nSelectStartRow = m_nSelectStartCol = m_nSelectEndRow = m_nSelectEndCol = 0;
				}
				m_nCol = 0;
				SetCaret();
			}
			break;
		case VK_END:
			bShift = (GetKeyState(VK_LSHIFT) >> 8) || (GetKeyState(VK_RSHIFT) >> 8);
			if (bShift)
			{
				if (!m_bSelect)
				{
					m_nSelectStartRow = m_nRow;
					m_nSelectStartCol = m_nCol;
					m_bSelect = true;
				}
				InvalidateRow(m_nRow);
				m_nCol = m_wsListText->Strings[m_nRow].Length();
				SetCaret();
				m_nSelectEndRow = m_nRow;
				m_nSelectEndCol = m_nCol;
			}
			else
			{
				BOOL bSelectExist = (m_nSelectStartRow != m_nSelectEndRow) || (m_nSelectStartCol != m_nSelectEndCol);
				if (m_bSelect || bSelectExist)
				{
					InvalidateSelectedArea();
					m_bSelect = false;
					m_nSelectStartRow = m_nSelectStartCol = m_nSelectEndRow = m_nSelectEndCol = 0;
				}
				m_nCol = m_wsListText->Strings[m_nRow].Length();
				SetCaret();
			}
			break;
		case VK_DELETE:
			m_bDirty = true;
			if (DeleteSelectedText())
			{
				SetCaret();
				break;
			}

			if (m_nCol < m_wsListText->Strings[m_nRow].Length())
			{
				InvalidateRow(m_nRow);
				m_wsListText->Strings[m_nRow] = m_wsListText->Strings[m_nRow].Delete(m_nCol + 1, 1);
				m_wsListActive->Strings[m_nRow] = m_wsListActive->Strings[m_nRow].Delete(m_nCol + 1, 1);
				m_wsListColor->Strings[m_nRow] = m_wsListColor->Strings[m_nRow].Delete(m_nCol + 1, 1);
			}
			else if (m_nRow < m_wsListText->Count - 1)
			{
				UnicodeString wsTemp = m_wsListText->Strings[m_nRow + 1];
				m_wsListText->Delete(m_nRow + 1);
				m_wsListText->Strings[m_nRow] = m_wsListText->Strings[m_nRow] + wsTemp;

				wsTemp = m_wsListActive->Strings[m_nRow + 1];
				m_wsListActive->Delete(m_nRow + 1);
				m_wsListActive->Strings[m_nRow] = m_wsListActive->Strings[m_nRow] + wsTemp;

				wsTemp = m_wsListColor->Strings[m_nRow + 1];
				m_wsListColor->Delete(m_nRow + 1);
				m_wsListColor->Strings[m_nRow] = m_wsListColor->Strings[m_nRow] + wsTemp;

				InvalidateRect(Handle, NULL, TRUE);
			}

			SetCaret();
			break;
		}
	}

	WndProc(msg);
}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::FormCreate(TObject *Sender)
{
	WindowProc = WinMsgProc;
	m_ClipBoard = new TClipboard;
}
//---------------------------------------------------------------------------

void TwndTextEdit::SetCaret()
{
	int x, y;
	int nOldPos;
	TScrollBox *scrParent;
	int nOffsetX = GetOffsetX(m_nRow);
	int nTextWidth;

	if (m_nRow < m_wsListText->Count)
	{
		nTextWidth = GetTextWidth(m_wsListText->Strings[m_nRow], m_nCol);
		x = nTextWidth + nOffsetX;
	}
	else
		x = nOffsetX;
	y = GetRowPosInWindow(m_nRow);
	SetCaretPos(x , y);

	scrParent = (TScrollBox *)Parent;
	while (x >= scrParent->HorzScrollBar->Position + scrParent->Width - ::GetSystemMetrics(SM_CXVSCROLL)- m_nFontHeight)
	{
		nOldPos = scrParent->HorzScrollBar->Position;
		scrParent->HorzScrollBar->Position += m_nFontHeight * 4;
		if (nOldPos == scrParent->HorzScrollBar->Position)
			break;
	}
	while ((x < scrParent->HorzScrollBar->Position + m_nFontHeight) && (scrParent->HorzScrollBar->Position > 0))
		scrParent->HorzScrollBar->Position -= m_nFontHeight * 4;
	while (y >= scrParent->VertScrollBar->Position + scrParent->Height - ::GetSystemMetrics(SM_CYHSCROLL) - m_nFontHeight)
	{
		nOldPos = scrParent->VertScrollBar->Position;
		scrParent->VertScrollBar->Position += m_nFontHeight;
		if (nOldPos == scrParent->VertScrollBar->Position)
			break;
	}
	while ((y < scrParent->VertScrollBar->Position + m_nFontHeight) && (scrParent->VertScrollBar->Position > 0))
		scrParent->VertScrollBar->Position -= m_nFontHeight;
}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::FormMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
#ifdef TWO_SOFT_DOG
	if (!frmMain->bDogOk)
		return;
#endif
	if (Button == mbLeft)
	{
		if (!Focused())
		{
			SetFocus();
			CreateCaret(Handle, (void*)1, 2, m_nFontHeight);
			ShowCaret(Handle);
			SetCaret();
		}

		GetCharMatrixPos(X, Y);
		SetCaret();

		BOOL bSelectExist = (m_nSelectStartRow != m_nSelectEndRow) || (m_nSelectStartCol != m_nSelectEndCol);
		if (bSelectExist)
		{
			InvalidateSelectedArea();
			m_nSelectStartRow = m_nSelectStartCol = m_nSelectEndRow = m_nSelectEndCol = 0;
		}
		if (!m_bSelect)
		{
			m_bMouseStartSelect = true;
			m_nSelectStartRow = m_nRow;
			m_nSelectStartCol = m_nCol;
			m_nSelectEndRow = m_nRow;
			m_nSelectEndCol = m_nCol;
			m_nOldMoveRow = m_nRow;
			m_nOldMoveCol = m_nCol;
			GetCapture();
		}
	}
}
//---------------------------------------------------------------------------

void TwndTextEdit::AdjustSelectArea()
{
	if (m_bSelect)
	{
		m_nSelectEndRow = m_nRow;
		m_nSelectEndCol = m_nCol;
	}
	else
	{
		m_nSelectStartRow = m_nSelectStartCol = m_nSelectEndRow = m_nSelectEndCol = 0;
	}
}
//---------------------------------------------------------------------------

bool TwndTextEdit::CheckInSelectArea(int nRow, int nCol)
{
	int nStartRow, nStartCol, nEndRow, nEndCol;

	if (m_nSelectStartRow < m_nSelectEndRow)
	{
		nStartRow = m_nSelectStartRow;
		nStartCol = m_nSelectStartCol;
		nEndRow = m_nSelectEndRow;
		nEndCol = m_nSelectEndCol;
	}
	else if (m_nSelectStartRow > m_nSelectEndRow)
	{
		nEndRow = m_nSelectStartRow;
		nEndCol = m_nSelectStartCol;
		nStartRow = m_nSelectEndRow;
		nStartCol = m_nSelectEndCol;
	}
	else
	{
		nStartRow = m_nSelectStartRow;
		nEndRow = m_nSelectEndRow;
		if (m_nSelectStartCol <= m_nSelectEndCol)
		{
			nStartCol = m_nSelectStartCol;
			nEndCol = m_nSelectEndCol;
		}
		else
		{
			nEndCol = m_nSelectStartCol;
			nStartCol = m_nSelectEndCol;
		}
	}

	if (nStartRow == nEndRow)
	{
		if (nRow == nStartRow && nCol >= nStartCol && nCol < nEndCol)
			return true;
	}
	else if (nRow == nStartRow)
	{
		if (nCol >= nStartCol)
			return true;
	}
	else if (nRow == nEndRow)
	{
		if (nCol < nEndCol)
			return true;
	}
	else if (nRow > nStartRow && nRow < nEndRow)
		return true;


	return false;
}
//---------------------------------------------------------------------------

bool TwndTextEdit::	DeleteSelectedText()
{
	int nStartRow, nStartCol, nEndRow, nEndCol;

	if ((m_nSelectStartRow == m_nSelectEndRow) && (m_nSelectStartCol == m_nSelectEndCol))
		return false;

	if (m_nSelectStartRow < m_nSelectEndRow)
	{
		nStartRow = m_nSelectStartRow;
		nStartCol = m_nSelectStartCol;
		nEndRow = m_nSelectEndRow;
		nEndCol = m_nSelectEndCol;
	}
	else if (m_nSelectStartRow > m_nSelectEndRow)
	{
		nEndRow = m_nSelectStartRow;
		nEndCol = m_nSelectStartCol;
		nStartRow = m_nSelectEndRow;
		nStartCol = m_nSelectEndCol;
	}
	else
	{
		nStartRow = m_nSelectStartRow;
		nEndRow = m_nSelectEndRow;
		if (m_nSelectStartCol <= m_nSelectEndCol)
		{
			nStartCol = m_nSelectStartCol;
			nEndCol = m_nSelectEndCol;
		}
		else
		{
			nEndCol = m_nSelectStartCol;
			nStartCol = m_nSelectEndCol;
		}
	}

	if (nStartRow == nEndRow)
	{
		m_wsListText->Strings[nStartRow] = m_wsListText->Strings[nStartRow].Delete(nStartCol + 1, nEndCol - nStartCol);
		m_wsListActive->Strings[nStartRow] = m_wsListActive->Strings[nStartRow].Delete(nStartCol + 1, nEndCol - nStartCol);
		m_wsListColor->Strings[nStartRow] = m_wsListColor->Strings[nStartRow].Delete(nStartCol + 1, nEndCol - nStartCol);
	}
	else
	{
		while (nEndRow != nStartRow + 1)
		{
			m_wsListText->Delete(nEndRow - 1);
			m_wsListActive->Delete(nEndRow - 1);
			m_wsListColor->Delete(nEndRow - 1);
			nEndRow--;
		}

		if (nEndRow < m_wsListText->Count)
		{
			m_wsListText->Strings[nEndRow] = m_wsListText->Strings[nEndRow].Delete(1, nEndCol);
			m_wsListActive->Strings[nEndRow] = m_wsListActive->Strings[nEndRow].Delete(1, nEndCol);
			m_wsListColor->Strings[nEndRow] = m_wsListColor->Strings[nEndRow].Delete(1, nEndCol);
		}

		m_wsListText->Strings[nStartRow] = m_wsListText->Strings[nStartRow].Delete(nStartCol + 1, m_wsListText->Strings[nStartRow].Length() - nStartCol);
		m_wsListActive->Strings[nStartRow] = m_wsListActive->Strings[nStartRow].Delete(nStartCol + 1, m_wsListText->Strings[nStartRow].Length() - nStartCol);
		m_wsListColor->Strings[nStartRow] = m_wsListColor->Strings[nStartRow].Delete(nStartCol + 1, m_wsListText->Strings[nStartRow].Length() - nStartCol);

		if (nEndRow < m_wsListText->Count)
		{
			if (m_wsListText->Strings[nEndRow].Length() == 0 )
			{
				m_wsListText->Delete(nEndRow);
				m_wsListActive->Delete(nEndRow);
				m_wsListColor->Delete(nEndRow);
			}
		}
		if (m_wsListText->Strings[nStartRow].Length() == 0 )
		{
			m_wsListText->Delete(nStartRow);
			m_wsListActive->Delete(nStartRow);
			m_wsListColor->Delete(nStartRow);
		}
	}

	m_nRow = nStartRow;
	m_nCol = nStartCol;
	m_nSelectStartRow = m_nSelectStartCol = m_nSelectEndRow = m_nSelectEndCol = 0;
	SetCaret();
	InvalidateRect(Handle, NULL, TRUE);

	return true;
}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::mnuPopupEditPopup(TObject *Sender)
{
	BOOL bSelectExist = (m_nSelectStartRow != m_nSelectEndRow) || (m_nSelectStartCol != m_nSelectEndCol);
	mnuPaste->Enabled = m_ClipBoard->HasFormat(CF_TEXT);
	mnuCut->Enabled = bSelectExist;
	mnuCopy->Enabled = bSelectExist;
	mnuDelete->Enabled = bSelectExist;

}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::mnuDeleteClick(TObject *Sender)
{
	m_bDirty = true;
	DeleteSelectedText();
}
//---------------------------------------------------------------------------

void TwndTextEdit::FormatSelctedArea()
{
	int nRow, nCol;

	if (m_nSelectStartRow > m_nSelectEndRow)
	{
		nRow = m_nSelectStartRow;
		nCol = m_nSelectStartCol;
		m_nSelectStartRow = m_nSelectEndRow;
		m_nSelectStartCol = m_nSelectEndCol;
		m_nSelectEndRow = nRow;
		m_nSelectEndCol = nCol;
	}
	else if (m_nSelectStartRow == m_nSelectEndRow)
	{
		if (m_nSelectStartCol > m_nSelectEndCol)
		{
			nCol = m_nSelectStartCol;
			m_nSelectStartCol = m_nSelectEndCol;
			m_nSelectEndCol = nCol;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::mnuCopyClick(TObject *Sender)
{
	UnicodeString wsTemp;
	int nRow;

	FormatSelctedArea();

	if (m_nSelectStartRow != m_nSelectEndRow)
	{
		wsTemp = m_wsListText->Strings[m_nSelectStartRow].SubString(m_nSelectStartCol + 1, m_wsListText->Strings[m_nSelectStartRow].Length() - m_nSelectStartCol);
		wsTemp = wsTemp + "\r\n";
		for (nRow = m_nSelectStartRow + 1; nRow < m_nSelectEndRow; nRow++)
		{
			wsTemp = wsTemp + m_wsListText->Strings[nRow];
			wsTemp = wsTemp + "\r\n";
		}
		wsTemp = wsTemp + m_wsListText->Strings[m_nSelectEndRow].SubString(1, m_nSelectEndCol);
	}
	else
		wsTemp = m_wsListText->Strings[m_nSelectStartRow].SubString(m_nSelectStartCol + 1, m_nSelectEndCol - m_nSelectStartCol);

	m_ClipBoard->SetTextBuf(wsTemp.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::mnuCutClick(TObject *Sender)
{
	mnuCopyClick(Sender);
	m_bDirty = true;
	DeleteSelectedText();
}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::mnuPasteClick(TObject *Sender)
{
	BOOL bSelectExist = (m_nSelectStartRow != m_nSelectEndRow) || (m_nSelectStartCol != m_nSelectEndCol);
	UnicodeString wsText = UnicodeString(m_ClipBoard->AsText);
	wchar_t* ws = wsText.c_str();

	m_bDirty = true;
	if (bSelectExist)
		DeleteSelectedText();

	while (ws && *ws)
		KeyPress(*ws++);
}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::mnuSelectAllClick(TObject *Sender)
{
	m_nSelectStartRow = m_nSelectStartCol = 0;
	if (m_wsListText->Count)
	{
		m_nSelectEndRow = m_wsListText->Count - 1;
		m_nSelectEndCol = m_wsListText->Strings[m_nSelectEndRow].Length();
	}
	else
		m_nSelectEndRow = m_nSelectEndCol = 0;

	InvalidateRect(Handle, NULL, TRUE);
}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::FormMouseMove(TObject *Sender, TShiftState Shift,
	  int X, int Y)
{
	if (m_bMouseStartSelect)
	{
		GetCharMatrixPos(X, Y);

		SetCaret();
		if ((m_nOldMoveRow != m_nRow) || (m_nOldMoveCol != m_nCol))
		{
			InvalidateSelectedArea();
			m_nSelectEndRow = m_nRow;
			m_nSelectEndCol = m_nCol;
			InvalidateSelectedArea();
			m_nOldMoveRow = m_nRow;
			m_nOldMoveCol = m_nCol;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::FormMouseUp(TObject *Sender, TMouseButton Button,
	  TShiftState Shift, int X, int Y)
{
	if (Button == mbLeft)
	{
		if (m_bMouseStartSelect)
		{
			m_bMouseStartSelect = false;
			InvalidateSelectedArea();
			if ((m_nSelectStartRow == m_nSelectEndRow) && (m_nSelectStartCol == m_nSelectEndCol))
			{
				m_nSelectStartRow = m_nSelectEndRow = m_nSelectStartCol = m_nSelectEndCol = 0;
			}
		}
		ReleaseCapture();
		m_bSelect = false;
	}
}
//---------------------------------------------------------------------------

void TwndTextEdit::InvalidateChar(int nRow, int nCol)
{
	TRect rect;
	int	nOffsetX = GetOffsetX(nRow);

	int nLeft = GetTextWidth(m_wsListText->Strings[m_nRow], m_nCol) + nOffsetX;
	int nRight = GetTextWidth(m_wsListText->Strings[m_nRow], m_nCol + 1) + nOffsetX;
	int nTop = GetRowPosInWindow(nRow);
	int nBottom = nTop + m_nFontHeight;
	rect = TRect(nLeft - 1, nTop, nRight, nBottom);

	InvalidateRect(Handle, &rect, true);
}
//---------------------------------------------------------------------------

void TwndTextEdit::InvalidateRow(int nRow)
{
	if ((nRow >= m_wsListText->Count))
		return;

	int nOffsetX = GetOffsetX(nRow);
	int nTop = GetRowPosInWindow(nRow);
	TRect rect = TRect(nOffsetX, nTop, GetTextWidth(m_wsListText->Strings[nRow], m_wsListText->Strings[nRow].Length()) + nOffsetX + 1, nTop + /*sz.cy*/ m_nFontHeight);
	InvalidateRect(Handle, &rect, true);
}
//---------------------------------------------------------------------------

void TwndTextEdit::InvalidateSelectedArea()
{
	TRect rect;

	if (m_nSelectStartRow < m_nSelectEndRow)
	{
		rect = TRect(0,
				  GetRowPosInWindow(m_nSelectStartRow),
				  this->Width,
				  GetRowPosInWindow(m_nSelectEndRow) + m_nFontHeight);
	}
	else if (m_nSelectStartRow > m_nSelectEndRow)
	{
		rect = TRect(0,
				  GetRowPosInWindow(m_nSelectEndRow),
				  this->Width,
				  GetRowPosInWindow(m_nSelectStartRow) + m_nFontHeight);
	}
	else
	{
		InvalidateRow(m_nSelectStartRow);
		return;
	}

	InvalidateRect(Handle, NULL, true);
}
//---------------------------------------------------------------------------

bool TwndTextEdit::IsSelectedRow(int nRow)
{
	if ((m_nSelectStartRow == m_nSelectEndRow) && (m_nSelectStartCol == m_nSelectEndCol))
		return false;

	if (m_nSelectStartRow <= m_nSelectEndRow)
		return ((nRow >= m_nSelectStartRow) && (nRow <= m_nSelectEndRow));
	else
		return ((nRow >= m_nSelectEndRow) && (nRow <= m_nSelectStartRow));
}
//---------------------------------------------------------------------------

int TwndTextEdit::GetRowPosInWindow(int nRow)
{
	int nTopSpace;
	int nRowsPerScreen;

	if (m_bAlignVertCenter)
		nRowsPerScreen = 1;
	else
		nRowsPerScreen = m_nEditScreenHeight / m_nFontHeight;
	if (nRowsPerScreen < 1)
		nRowsPerScreen = 1;
	nTopSpace = (m_nEditScreenHeight - nRowsPerScreen * m_nFontHeight) / (nRowsPerScreen + 1);

	return (nRow / nRowsPerScreen) * m_nEditScreenHeight + (nRow % nRowsPerScreen) * (nTopSpace + m_nFontHeight) + nTopSpace;
}
//---------------------------------------------------------------------------

void TwndTextEdit::GetCharMatrixPos(int X, int Y)
{
	int nRow;
	int nCol;
	int nOffsetX;
	SIZE sz;

	for (nRow = 0; nRow < m_wsListText->Count; nRow++)
	{
		if (GetRowPosInWindow(nRow + 1) > Y)
			break;
	}

	if (nRow >= m_wsListText->Count)
		nCol = 1;
	else
	{
		nOffsetX = GetOffsetX(nRow);
		for (nCol = 1; nCol <= m_wsListText->Strings[nRow].Length(); nCol++)
		{
			sz.cx = GetTextWidth(m_wsListText->Strings[nRow], nCol);
			if (sz.cx + nOffsetX >= X )
				break;
		}
	}

	m_nRow = nRow;
	m_nCol = nCol - 1;
}
//---------------------------------------------------------------------------

UnicodeString TwndTextEdit::GetText()
{
	return TrimRightEmptyLines(UnicodeString(m_wsListText->GetText()));
}
//---------------------------------------------------------------------------

int TwndTextEdit::GetOffsetX(int nRow)
{
	int nOffsetX = 0;
	int nTextWidth = 0;

	if (nRow < m_wsListText->Count)
		nTextWidth = GetTextWidth(m_wsListText->Strings[nRow], m_wsListText->Strings[nRow].Length());

	if (m_nAlignStyle == ALIGN_CENTER)
	{
		if (nTextWidth < m_nEditScreenWidth)
			nOffsetX = (m_nEditScreenWidth - nTextWidth) / 2;
	}
	else if (m_nAlignStyle == ALIGN_RIGHT)
	{
		if (nTextWidth < m_nEditScreenWidth)
			nOffsetX = m_nEditScreenWidth - nTextWidth;
	}

	return nOffsetX;
}
//---------------------------------------------------------------------------

void TwndTextEdit::ChangeTextColor(BYTE ucColor)
{
	int nStartRow, nStartCol, nEndRow, nEndCol, nRow, nCol;
	wchar_t* ws;

	m_ucTextColor &= (COLOR_WHITE ^ 0xFF);
	m_ucTextColor |= ucColor;

	if ((m_nSelectStartRow == m_nSelectEndRow) && (m_nSelectStartCol == m_nSelectEndCol))
		return;

	m_bDirty = true;
	if (m_nSelectStartRow < m_nSelectEndRow)
	{
		nStartRow = m_nSelectStartRow;
		nStartCol = m_nSelectStartCol;
		nEndRow = m_nSelectEndRow;
		nEndCol = m_nSelectEndCol;
	}
	else if (m_nSelectStartRow > m_nSelectEndRow)
	{
		nEndRow = m_nSelectStartRow;
		nEndCol = m_nSelectStartCol;
		nStartRow = m_nSelectEndRow;
		nStartCol = m_nSelectEndCol;
	}
	else
	{
		nStartRow = m_nSelectStartRow;
		nEndRow = m_nSelectEndRow;
		if (m_nSelectStartCol <= m_nSelectEndCol)
		{
			nStartCol = m_nSelectStartCol;
			nEndCol = m_nSelectEndCol;
		}
		else
		{
			nEndCol = m_nSelectStartCol;
			nStartCol = m_nSelectEndCol;
		}
	}

	if (nStartRow == nEndRow)
	{
		ws = m_wsListColor->Strings[nStartRow].c_str();
		ws += nStartCol;
		for (nCol = nStartCol; nCol < nEndCol; nCol++)
		{
			*ws &= (COLOR_WHITE ^ 0xFF);
			*ws |= ucColor;
			ws++;
		}
	}
	else
	{
		ws = m_wsListColor->Strings[nStartRow].c_str();
		ws += nStartCol;
		for (nCol = nStartCol; nCol < m_wsListColor->Strings[nStartRow].Length(); nCol++)
		{
			*ws &= (COLOR_WHITE ^ 0xFF);
			*ws |= ucColor;
			ws++;
		}

		for (nRow = nStartRow + 1; nRow < nEndRow - 1; nRow++)
		{
			ws = m_wsListColor->Strings[nRow].c_str();
			for (nCol = 0; nCol < m_wsListColor->Strings[nRow].Length(); nCol++)
			{
				*ws &= (COLOR_WHITE ^ 0xFF);
				*ws |= ucColor;
				ws++;
			}
		}

		ws = m_wsListColor->Strings[nEndRow].c_str();
		for (nCol = 0; nCol < nEndCol; nCol++)
		{
			*ws &= (COLOR_WHITE ^ 0xFF);
			*ws |= ucColor;
			ws++;
		}
	}

	InvalidateRect(Handle, NULL, TRUE);
}
//---------------------------------------------------------------------------

void TwndTextEdit::ChangeBackColor(BYTE ucColor)
{
	int nStartRow, nStartCol, nEndRow, nEndCol, nRow, nCol;
	wchar_t *ws;

	m_ucTextColor &= (BACKGROUND_COLOR_WHITE ^ 0xFF);
	m_ucTextColor |= ucColor;

	if ((m_nSelectStartRow == m_nSelectEndRow) && (m_nSelectStartCol == m_nSelectEndCol))
		return;

	m_bDirty = true;
	if (m_nSelectStartRow < m_nSelectEndRow)
	{
		nStartRow = m_nSelectStartRow;
		nStartCol = m_nSelectStartCol;
		nEndRow = m_nSelectEndRow;
		nEndCol = m_nSelectEndCol;
	}
	else if (m_nSelectStartRow > m_nSelectEndRow)
	{
		nEndRow = m_nSelectStartRow;
		nEndCol = m_nSelectStartCol;
		nStartRow = m_nSelectEndRow;
		nStartCol = m_nSelectEndCol;
	}
	else
	{
		nStartRow = m_nSelectStartRow;
		nEndRow = m_nSelectEndRow;
		if (m_nSelectStartCol <= m_nSelectEndCol)
		{
			nStartCol = m_nSelectStartCol;
			nEndCol = m_nSelectEndCol;
		}
		else
		{
			nEndCol = m_nSelectStartCol;
			nStartCol = m_nSelectEndCol;
		}
	}

	if (nStartRow == nEndRow)
	{
		ws = m_wsListColor->Strings[nStartRow].c_str();
		ws += nStartCol;
		for (nCol = nStartCol; nCol < nEndCol; nCol++)
		{
			*ws &= (BACKGROUND_COLOR_WHITE ^ 0xFF);
			*ws |= ucColor;
			ws++;
		}
	}
	else
	{
		ws = m_wsListColor->Strings[nStartRow].c_str();
		ws += nStartCol;
		for (nCol = nStartCol; nCol < m_wsListColor->Strings[nStartRow].Length(); nCol++)
		{
			*ws &= (BACKGROUND_COLOR_WHITE ^ 0xFF);
			*ws |= ucColor;
			ws++;
		}

		for (nRow = nStartRow + 1; nRow < nEndRow - 1; nRow++)
		{
			ws = m_wsListColor->Strings[nRow].c_str();
			for (nCol = 0; nCol < m_wsListColor->Strings[nRow].Length(); nCol++)
			{
				*ws &= (BACKGROUND_COLOR_WHITE ^ 0xFF);
				*ws |= ucColor;
				ws++;
			}
		}

		ws = m_wsListColor->Strings[nEndRow].c_str();
		for (nCol = 0; nCol < nEndCol; nCol++)
		{
			*ws &= (BACKGROUND_COLOR_WHITE ^ 0xFF);
			*ws |= ucColor;
			ws++;
		}
	}

	InvalidateRect(Handle, NULL, TRUE);
}
//---------------------------------------------------------------------------

void TwndTextEdit::Clear()
{
	m_wsListText->Clear();
	m_wsListActive->Clear();
	m_wsListColor->Clear();

	if (pic != NULL)
	{
		delete pic;
		pic = NULL;
	}
}
//---------------------------------------------------------------------------

void TwndTextEdit::SetText(UnicodeString wsText)
{
	m_wsListText->SetText(wsText.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (pic != NULL)
		delete pic;

	delete m_wsListText;
	delete m_wsListActive;
	delete m_wsListColor;
}
//---------------------------------------------------------------------------

bool TwndTextEdit::IsAnyTextSelected()
{
	return (m_nSelectStartRow != m_nSelectEndRow) || (m_nSelectStartCol != m_nSelectEndCol);
}
//---------------------------------------------------------------------------

UnicodeString TwndTextEdit::GetSelectedText()
{
	UnicodeString ws;

	FormatSelctedArea();

	if (m_nSelectStartRow != m_nSelectEndRow)
		return ws;

	if (m_nSelectStartCol >= m_nSelectEndCol)
		return ws;

	if ((m_nSelectEndCol - m_nSelectStartCol) > 128)
	{
		::MessageBox(this->Handle, L"选择字数过多!请重新选择。", NULL, MB_OK | MB_ICONWARNING);
		return ws;
	}

	wchar_t* wsSrc = m_wsListText->Strings[m_nSelectStartRow].c_str();
	wsSrc += m_nSelectStartCol;
	for (int nCol = m_nSelectStartCol; nCol < m_nSelectEndCol; nCol++)
		ws += *wsSrc++;

	return ws;
}
//---------------------------------------------------------------------------

void TwndTextEdit::SetTextActive(int nActiveType)
{
	wchar_t* ws = m_wsListActive->Strings[m_nSelectStartRow].c_str();

	for (int nCol = m_nSelectStartCol; nCol < m_nSelectEndCol; nCol++)
		ws[nCol] = nActiveType;

	m_wsListActive->Strings[m_nSelectStartRow] = ws;

	InvalidateRow(m_nSelectStartRow);
}
//---------------------------------------------------------------------------

UnicodeString TwndTextEdit::GetActiveText()
{
	return TrimRightEmptyLines(UnicodeString(m_wsListActive->GetText()));
}
//---------------------------------------------------------------------------

void TwndTextEdit::SetActiveText(UnicodeString wsTextActive)
{
	m_wsListActive->SetText(wsTextActive.c_str());
}
//---------------------------------------------------------------------------

UnicodeString TwndTextEdit::GetTextColor()
{
	return TrimRightEmptyLines(UnicodeString(m_wsListColor->GetText()));
}
//---------------------------------------------------------------------------

void TwndTextEdit::SetTextColor(UnicodeString sTextColor)
{
	m_wsListColor->SetText(sTextColor.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TwndTextEdit::FormKeyPress(TObject *Sender, wchar_t &Key)
{
#ifdef TWO_SOFT_DOG
	if (!frmMain->bDogOk)
		return;
#endif
	if (Key >= 0x20)
	{
		m_bDirty = true;
		DeleteSelectedText();

		if (m_nRow >= m_wsListText->Count)
		{
			m_wsListText->Add(UnicodeString(Key));
			m_wsListActive->Add("$");
			m_wsListColor->Add(UnicodeString(wchar_t(m_ucTextColor)));
		}
		else
		{
			m_wsListText->Strings[m_nRow] =
				m_wsListText->Strings[m_nRow].SubString(1, m_nCol) +
				UnicodeString(Key) +
				m_wsListText->Strings[m_nRow].SubString(m_nCol + 1, m_wsListText->Strings[m_nRow].Length() - m_nCol);
			m_wsListActive->Strings[m_nRow] =
				m_wsListActive->Strings[m_nRow].SubString(1, m_nCol) +
				"$" +
				m_wsListActive->Strings[m_nRow].SubString(m_nCol + 1, m_wsListActive->Strings[m_nRow].Length() - m_nCol);
			m_wsListColor->Strings[m_nRow] =
				m_wsListColor->Strings[m_nRow].SubString(1, m_nCol) +
				UnicodeString(wchar_t(m_ucTextColor)) +
				m_wsListColor->Strings[m_nRow].SubString(m_nCol + 1, m_wsListColor->Strings[m_nRow].Length() - m_nCol);
		}
		m_nCol++;
		//CheckActiveText();
		SetCaret();
		InvalidateRow(m_nRow);
	}
	else
	{
		if (Key == '\b')
		{
			m_bDirty = true;
			if (DeleteSelectedText())
				return;

			if (m_nCol > 0)
			{
				InvalidateRow(m_nRow);
				m_wsListText->Strings[m_nRow] = m_wsListText->Strings[m_nRow].Delete(m_nCol, 1);
				m_wsListActive->Strings[m_nRow] = m_wsListActive->Strings[m_nRow].Delete(m_nCol, 1);
				m_wsListColor->Strings[m_nRow] = m_wsListColor->Strings[m_nRow].Delete(m_nCol, 1);
				m_nCol--;
				SetCaret();
			}
			else if (m_nRow > 0 && m_nRow < m_wsListText->Count)
			{
				UnicodeString wsTemp = m_wsListText->Strings[m_nRow];
				m_wsListText->Delete(m_nRow);
				m_nCol = m_wsListText->Strings[m_nRow - 1].Length();
				m_wsListText->Strings[m_nRow - 1] = m_wsListText->Strings[m_nRow - 1] + wsTemp;

				wsTemp = m_wsListActive->Strings[m_nRow];
				m_wsListActive->Delete(m_nRow);
				m_wsListActive->Strings[m_nRow - 1] = m_wsListActive->Strings[m_nRow - 1] + wsTemp;

				wsTemp = m_wsListColor->Strings[m_nRow];
				m_wsListColor->Delete(m_nRow);
				m_wsListColor->Strings[m_nRow - 1] = m_wsListColor->Strings[m_nRow - 1] + wsTemp;

				m_nRow--;
				InvalidateRect(Handle, NULL, TRUE);
				SetCaret();
			}
		}
		else if (Key == '\r')
		{
			m_bDirty = true;
			DeleteSelectedText();

			if (m_nCol == 0)
			{
				m_wsListText->Insert(m_nRow, "");
				m_wsListActive->Insert(m_nRow, "");
				m_wsListColor->Insert(m_nRow, "");
				m_nRow++;
			}
			else
			{
				UnicodeString wsTemp;

				wsTemp = m_wsListText->Strings[m_nRow].SubString(m_nCol + 1, m_wsListText->Strings[m_nRow].Length() - m_nCol);
				m_wsListText->Strings[m_nRow] = m_wsListText->Strings[m_nRow].Delete(m_nCol + 1, m_wsListText->Strings[m_nRow].Length() - m_nCol);
				m_wsListText->Insert(m_nRow + 1, wsTemp);

				wsTemp = m_wsListActive->Strings[m_nRow].SubString(m_nCol + 1, m_wsListActive->Strings[m_nRow].Length() - m_nCol);
				m_wsListActive->Strings[m_nRow] = m_wsListActive->Strings[m_nRow].Delete(m_nCol + 1, m_wsListActive->Strings[m_nRow].Length() - m_nCol);
				m_wsListActive->Insert(m_nRow + 1, wsTemp);

				wsTemp = m_wsListColor->Strings[m_nRow].SubString(m_nCol + 1, m_wsListColor->Strings[m_nRow].Length() - m_nCol);
				m_wsListColor->Strings[m_nRow] = m_wsListColor->Strings[m_nRow].Delete(m_nCol + 1, m_wsListColor->Strings[m_nRow].Length() - m_nCol);
				m_wsListColor->Insert(m_nRow + 1, wsTemp);

				m_nRow++;
				m_nCol = 0;
			}

			SetCaret();
			InvalidateRect(Handle, NULL, TRUE);
		}
	}
}
//---------------------------------------------------------------------------

void TwndTextEdit::SetFont(TFont* pFont, int nDisplayAreaHeight)
{
	TEXTMETRIC tm;

	if (nDisplayAreaHeight < 8) nDisplayAreaHeight = 8;

	m_bDotFont = false;
	for (;;)
	{
		Canvas->Font = pFont;
		GetTextMetrics(Canvas->Handle, &tm);
		m_nFontHeight = tm.tmHeight;
		if (m_nFontHeight <= nDisplayAreaHeight)
			break;
		if (pFont->Height < 0)
			pFont->Height++;
		else
			pFont->Height--;
	}
}
//---------------------------------------------------------------------------

void TwndTextEdit::SetDotFont(int nFontSize)
{
	m_bDotFont = true;
	m_nDotFontSize = nFontSize;
	m_nFontHeight = nFontSize;
}
//---------------------------------------------------------------------------

UnicodeString TwndTextEdit::TrimRightEmptyLines(UnicodeString& wsSrc)
{
	UnicodeString ws = wsSrc;
	while (ws.Length() > 1  && ws.SubString(ws.Length(), 1) == "\r")
		ws = ws.SubString(1, ws.Length() - 1);
	while (ws.Length() > 1  && ws.SubString(ws.Length() - 1, 2) == "\r\n")
		ws = ws.SubString(1, ws.Length() - 2);
	return ws;
}
//---------------------------------------------------------------------------

int TwndTextEdit::GetTextWidth(UnicodeString ws, int nCount)
{
	SIZE sz;

	if (m_bDotFont)
	{
		if (m_nDotFontSize == 16)
			return TDotFont16::GetTextWidth(ws, nCount);
		else if (m_nDotFontSize == 24)
			return TDotFont24::GetTextWidth(ws, nCount);
		else
			return TDotFont32::GetTextWidth(ws, nCount);
	}
	else
	{
		GetTextExtentPoint32W(Canvas->Handle, ws.c_str(), nCount, &sz);
		return sz.cx;
	}
}
//---------------------------------------------------------------------------

void TwndTextEdit::EnabledRefresh(bool bEnable)
{
	m_bRefreshEnabled = bEnable;
}
//---------------------------------------------------------------------------

void TwndTextEdit::ResetCursor()
{
	CreateCaret(Handle, (void*)1, 2, m_nFontHeight);
	ShowCaret(Handle);
	GetCharMatrixPos(0, 0);
	SetCaret();

	m_nSelectStartRow = m_nSelectEndRow = m_nSelectStartCol = m_nSelectEndCol = 0;
	m_bMouseStartSelect = false;
}
//---------------------------------------------------------------------------

#ifdef AUTO_SIZE_TEXT
void TwndTextEdit::AutoSizeText()
{
	if (m_bDotFont)
	{
		if (m_nEditScreenHeight == 16)
			m_nDotFontSize = 16;
		else if (m_nEditScreenHeight == 24)
			m_nDotFontSize = 24;
		else if (m_nEditScreenHeight == 32)
			m_nDotFontSize = 32;
		else
		{
			m_bDotFont = false;
			Canvas->Font->Name = "宋体";
			Canvas->Font->Size = -m_nEditScreenHeight * 72 / GetDeviceCaps(Canvas->Handle, LOGPIXELSY);
			Canvas->Font->Style = TFontStyles();
		}
	}
	else
	{
		Canvas->Font->Size = -m_nEditScreenHeight * 72 / GetDeviceCaps(Canvas->Handle, LOGPIXELSY);
		m_nFontHeight = m_nEditScreenHeight;
	}
}
#endif
//---------------------------------------------------------------------------

