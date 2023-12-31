//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <process.h>
#include <inifiles.hpp>
#include <registry.hpp>
#include <jpeg.hpp>
#include <FileCtrl.hpp>
#include "global.h"
#include "SerialPort.h"
#include "sysConfig.h"
#include "Password.h"
#include "TextEditForm.h"
#include "CustMsgDlg.h"
#include "dtu.h"
#include "CardList.h"
#include "About.h"
#include "SearchCard.h"
#include "SearchAct.h"
#include "ActiveType.h"
#include "DialogReTx.h"
#include "TxLogView.h"
#include "ConfigForm.h"
#include "SelectFont.h"
#include "DotFont16.h"
#include "DotFont24.h"
#include "DotFont32.h"
#include "RemoteControl.h"    s
#include "Weather.h"
#include "FlashTest.h"
#include "AdjustClockSetup.h"
#include "act.h"
#include "ReTxSetup.h"
#include "CommConfig.h"
#include "ParkDigit.h"
#include "CardType.h"
#include "Weather_16x16_C1.h"
#include "RootName.h"
#include "Warnning.h"
#include "SignCheck.h"
#include "ActDefine.h"
#include "gsmh.h"
#include "GpsMap.h"
#include "AdjustLightness.h"
#include "BrightnessSetup.h"
#include "LedDigit.h"
#include <Mshtml.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma link "IdUDPServer"
#pragma link "SHDocVw_OCX"
#pragma link "IdHTTP"
#pragma link "IdTCPClient"
#pragma link "IdTCPConnection"
#pragma resource "*.dfm"
TfrmMain *frmMain;

//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
  : TForm(Owner)
{
	m_nLanguage = sysConfig.m_nLanguage;
	m_nLastAutoWriteClockMinute = -1;
	m_nAutoWriteClockHourCount = 0;

	for (int i = 0; i < MAX_ACT_COUNT; i++)
		m_picAct[i] = new TPicture;

	for (int i = 0; i < 8; i++)
	{
		m_nParkValue[i] = 0;
		m_clParkColor[i] = clYellow;
	}
	m_bParkDigit_DoubleColor = true;
	m_nParkDigit_Lightvalue = 7;

	m_bPreview = false;
	m_bDirty = false;
	m_nActIndex = 0;

	m_dtu = NULL;
	m_Node = NULL;
	m_lstAllRootNodeName = new TStringList;
	m_sLedDigit = "0";
}
//---------------------------------------------------------------------------

void TfrmMain::LoadCardTree(UnicodeString sSection, PVirtualNode NodeRoot, CDtu* dtuParent)
{
	TCustomIniFile* ini = new TRegistryIniFile(sSection);
	int nIndex = 0;

	for (;;)
	{
		UnicodeString sName = ini->ReadString("List", UnicodeString(nIndex), "");
		if (sName == "")
			break;

		PVirtualNode Node = vt->AddChild(NodeRoot);
		Node->CheckType = ctTriStateCheckBox;
		Node->CheckState = csUncheckedNormal;
		CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
		dtu->m_sName = sName;
		dtu->m_sSection = sSection + "\\LedPanels";
		dtu->m_dtuParent = dtuParent;
		dtu->LoadConfig();
		if (dtu->m_nNodeType == NODE_GROUP)
			LoadCardTree(dtu->m_sSection + "\\" + dtu->m_sName, Node, dtu);

		nIndex++;
	}

	delete ini;
}
//---------------------------------------------------------------------------

void TfrmMain::LoadAllCard()
{
	PVirtualNode NodeRoot = vt->AddChild(NULL);
	CDtu* dtu = (CDtu*)(vt->GetNodeData(NodeRoot));
	dtu->m_sName = sysConfig.m_sRootName;
	dtu->m_sSection = "Software\\Yuepoch\\LedPanelManV3";
	dtu->LoadConfig();
	dtu->m_nNodeType = NODE_ALL_CARD_ROOT;
	dtu->m_dtuParent = NULL;
	NodeRoot->CheckType = ctTriStateCheckBox;
	NodeRoot->CheckState = csUncheckedNormal;
	LoadCardTree(dtu->m_sSection + "\\" + dtu->m_sName, NodeRoot, dtu);
	vt->FullExpand(NodeRoot);
	vt->Refresh();

	PVirtualNode Node = vt->GetFirst();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
		{
			if (m_dtu == NULL)
			{
				vt->Selected[Node] = true;
				vt->FocusedNode = Node;
				break;
			}
		}
		Node = vt->GetNext(Node);
	}

	if (m_dtu == NULL)
	{
		PVirtualNode Node = vt->GetFirst();
		vt->Selected[Node] = true;
		vt->FocusedNode = Node;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
#ifdef AUTO_SIZE_TEXT
	mnuActDefine->Visible = false;
#endif
#ifdef _DEBUG
	btnTest->Visible = true;
#endif
	ChangeLanguage(sysConfig.m_nLanguage);
	vt->NodeDataSize = sizeof(CDtu);

	LoadAllRootNodeName();
	LoadAllCardName();

	UnicodeString sPath = ExtractFilePath(Application->ExeName) + "log";
	CreateDirectory(sPath.c_str(), NULL);

	timerAutoWriteClock->Enabled = (sysConfig.m_nAutoClockInterval != 0);
	mnuReTxSetup->Checked = (sysConfig.m_nAutoReTxActInterval != 0);

	wndTextEdit = new TwndTextEdit(this);
	wndTextEdit->Parent = scrollboxEdit;
	wndTextEdit->ChangeLanguage();
	wndTextEdit->Visible = true;

	scrollboxTime->VertScrollBar->Range = ACT_TIME_PERIOD_COUNT_24 * 24 + 8;
	for (int i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
	{
		m_lbPeriod[i] = new TLabel(scrollboxTime);
		m_lbPeriod[i]->Parent = scrollboxTime;
		m_lbPeriod[i]->Caption = AnsiString(i + 1) + ":";
		m_lbPeriod[i]->Top = 4 + 24 * i;
		m_lbPeriod[i]->Left = 0;

		m_timeStart[i] = new TDateTimePicker(scrollboxTime);
		m_timeStart[i]->Parent = scrollboxTime;
		m_timeStart[i]->Kind = dtkTime;
		m_timeStart[i]->Format = "HH:mm:ss";
		m_timeStart[i]->Left = 44;
		m_timeStart[i]->Top = 1 + 24 * i;

		m_lbTo[i] = new TLabel(scrollboxTime);
		m_lbTo[i]->Parent = scrollboxTime;
		m_lbTo[i]->Caption = "-";
		m_lbTo[i]->Top = 4 + 24 * i;
		m_lbTo[i]->Left = 116;

		m_timeEnd[i] = new TDateTimePicker(scrollboxTime);
		m_timeEnd[i]->Parent = scrollboxTime;
		m_timeEnd[i]->Kind = dtkTime;
		m_timeEnd[i]->Format = "HH:mm:ss";
		m_timeEnd[i]->Left = 134;
		m_timeEnd[i]->Top = 1 + 24 * i;
	}

	LoadAllCard();
	InitActInputWindow();

	PVirtualNode Node = vt->GetFirstSelected();
	CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
	LoadDynamicActText(dtu);

	Node = vt->GetFirst();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
			dtu->ReadTxActStatus();
		Node = vt->GetNext(Node);
	}

	mnuManualReTxAct->Enabled = CheckIfAnyActTxFailed();
	if (mnuManualReTxAct->Enabled)
	{
		if (sysConfig.m_nAutoReTxActInterval != 0)
		{
			m_nWaitReTxTimerCount = sysConfig.m_nAutoReTxActInterval * 60 - 1;
			timer_AutoWaitReTxAct->Enabled = true;
		}
	}

#ifdef XINJIANG_AUTO_WEB_DATA
	TWebDataThread *thread = new TWebDataThread();
	thread->FreeOnTerminate = true;
	thread->Priority = tpLower;
	thread->Resume();
#endif
}
//---------------------------------------------------------------------------

__fastcall TfrmMain::~TfrmMain()
{
	for (int i = 0; i < MAX_ACT_COUNT; i++)
		delete m_picAct[i];

	delete wndTextEdit;
	delete m_lstAllRootNodeName;

	ClearDtuList();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormShow(TObject *Sender)
{
#ifdef AUTO_SIZE_TEXT
	m_dtu->ApplyCurrentScreenConfig();
#endif
	wndTextEdit->Height = sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] * MAX_EDIT_SCREEN_Y_COUNT;
	wndTextEdit->Width = sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea] * MAX_EDIT_SCREEN_X_COUNT;
	vt->ScrollBarOptions->AlwaysVisible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnLoadPicureClick(TObject *Sender)
{
	try
	{
		if (OpenPictureDialog->Execute())
		{
			if (wndTextEdit->pic != NULL)
				delete wndTextEdit->pic;
			wndTextEdit->pic = new TPicture;
			if (ExtractFileExt(OpenPictureDialog->FileName) == ".jpg")
			{
				TJPEGImage  *JpegImage = new  TJPEGImage();
				Graphics::TBitmap *bitmap = new Graphics::TBitmap();
				JpegImage->LoadFromFile(OpenPictureDialog->FileName);
				bitmap->Width = JpegImage->Width;
				bitmap->Height = JpegImage->Height;
				bitmap->Canvas->StretchDraw(Rect(0, 0, bitmap->Width, bitmap->Height), JpegImage);
				wndTextEdit->pic->Graphic = bitmap;
				delete JpegImage;
				delete bitmap;
			}
			else
				wndTextEdit->pic->LoadFromFile(OpenPictureDialog->FileName);

			wndTextEdit->Refresh();
		}
	}
	catch (Exception &ex)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			::MessageBox(this->Handle, L"图片文件打开失败!", NULL, MB_OK | MB_ICONWARNING);
		else
			::MessageBox(this->Handle, L"Picture File Open Failed!", NULL, MB_OK | MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------

void TfrmMain::CreateActImage()
{
	int nAreaIndex = sysConfig.m_nActArea;
	TStrings *strListText = new TStringList;
	TStrings *strListColor = new TStringList;
	TStrings *strListActive = new TStringList;
	TEXTMETRIC tm;
	int nFontHeight;
	int nRowsPerScreen;
	int nImageWidth, nImageHeight, nLineWidth;
	int nTopSpace;
	int nOffsetX;
	int x, y, nRow, nCount;
	int nVertScreen;
	wchar_t *ws, *wsColor, *wsActive;
	SIZE sz;

	strListText->SetText(sysConfig.m_sActText.c_str());
	strListColor->SetText(sysConfig.m_sActTextColor.c_str());
	strListActive->SetText(sysConfig.m_sActActiveText.c_str());

	imgScreen->Picture->Bitmap->PixelFormat = pf4bit;
	imgScreen->Picture->Bitmap->Canvas->Brush->Color = clBlack;
	imgScreen->Picture->Bitmap->Canvas->Font->Name = sysConfig.m_sActFontName;
	imgScreen->Picture->Bitmap->Canvas->Font->Size = sysConfig.m_nActFontSize;
	imgScreen->Picture->Bitmap->Canvas->Font->Style = TFontStyles(sysConfig.m_nActFontStyle);
	imgScreen->Picture->Bitmap->Canvas->Font->Color = clRed;
	GetTextMetrics(imgScreen->Picture->Bitmap->Canvas->Handle, &tm);
	if (sysConfig.m_bDotFont)
		nFontHeight = sysConfig.m_nDotFontSize;
	else
		nFontHeight = tm.tmHeight;

	if (sysConfig.m_nActVertAlignment)
		nRowsPerScreen = 1;
	else
		nRowsPerScreen = sysConfig.m_nSubAreaHeight[nAreaIndex] / nFontHeight;
	if (nRowsPerScreen < 1)
		nRowsPerScreen = 1;
	nTopSpace = (sysConfig.m_nSubAreaHeight[nAreaIndex] - nRowsPerScreen * nFontHeight) / (nRowsPerScreen + 1);

	nImageWidth = 0;
	nImageHeight = 0;
	y = nTopSpace;
	for (nRow = 0; nRow < strListText->Count; nRow++)
	{
		nLineWidth = GetTextWidth(imgScreen->Picture->Bitmap->Canvas, sysConfig.m_bDotFont, sysConfig.m_nDotFontSize, strListText->Strings[nRow]);
		if ((sysConfig.m_nActHorzAlignment == ALIGN_CENTER) ||
			(sysConfig.m_nActHorzAlignment == ALIGN_RIGHT))
		{
			if (nLineWidth < sysConfig.m_nSubAreaWidth[nAreaIndex])
				nLineWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];
		}

		if (nLineWidth > nImageWidth)
			nImageWidth = nLineWidth;

		y += nTopSpace + nFontHeight;
		if (y + nFontHeight > sysConfig.m_nSubAreaHeight[nAreaIndex])
		{
			y = nTopSpace;
			nImageHeight += sysConfig.m_nSubAreaHeight[nAreaIndex];
		}
	}

	if (y > nTopSpace)
		nImageHeight += sysConfig.m_nSubAreaHeight[nAreaIndex];

	if (sysConfig.m_picAct != NULL)
	{
		int nPicWidth = (sysConfig.m_picAct->Width + sysConfig.m_nActImagePosX + sysConfig.m_nSubAreaWidth[nAreaIndex] - 1) / sysConfig.m_nSubAreaWidth[nAreaIndex];
		nPicWidth *= sysConfig.m_nSubAreaWidth[nAreaIndex];

		int nPicHeight = (sysConfig.m_picAct->Height + sysConfig.m_nActImagePosY + sysConfig.m_nSubAreaHeight[nAreaIndex] - 1) / sysConfig.m_nSubAreaHeight[nAreaIndex];
		nPicHeight *= sysConfig.m_nSubAreaHeight[nAreaIndex];

		if (nImageWidth < nPicWidth)
			nImageWidth = nPicWidth;

		if (nImageHeight < nPicHeight)
			nImageHeight = nPicHeight;
	}

	imgScreen->Picture->Bitmap->SetSize(nImageWidth, nImageHeight);
	imgScreen->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, nImageWidth, nImageHeight));

	nVertScreen = 0;
	imgScreen->Picture->Bitmap->Canvas->Pen->Style = psClear;
	y = nTopSpace;
	for (nRow = 0; nRow < strListText->Count; nRow++)
	{
		nOffsetX = 0;
		nLineWidth = GetTextWidth(imgScreen->Picture->Bitmap->Canvas, sysConfig.m_bDotFont, sysConfig.m_nDotFontSize, strListText->Strings[nRow]);
		if (sysConfig.m_nActHorzAlignment == ALIGN_CENTER)
		{
			if (nLineWidth < sysConfig.m_nSubAreaWidth[nAreaIndex])
				nOffsetX = (sysConfig.m_nSubAreaWidth[nAreaIndex] - nLineWidth) / 2;
		}
		else if (sysConfig.m_nActHorzAlignment == ALIGN_RIGHT)
		{
			if (nLineWidth < sysConfig.m_nSubAreaWidth[nAreaIndex])
				nOffsetX = sysConfig.m_nSubAreaWidth[nAreaIndex] - nLineWidth;
		}

		ws = strListText->Strings[nRow].c_str();
		wsColor = strListColor->Strings[nRow].c_str();
		wsActive = strListActive->Strings[nRow].c_str();
		x = nOffsetX;

		if (sysConfig.m_bDotFont)
		{
			DWORD crFColor, crBColor;
			while (*ws)
			{
				if (*wsActive == '$')
					utils.GetRGBColor(*wsColor, '$', &crFColor, &crBColor);
				else
				{
					crFColor = clBlack;
					crBColor = clBlack;
				}

				if (sysConfig.m_nDotFontSize == 16)
					x += TDotFont16::DrawChar(imgScreen->Picture->Bitmap->Canvas, x, y + nVertScreen * sysConfig.m_nSubAreaHeight[nAreaIndex], crFColor, crBColor, *ws);
				else if (sysConfig.m_nDotFontSize == 24)
					x += TDotFont24::DrawChar(imgScreen->Picture->Bitmap->Canvas, x, y + nVertScreen * sysConfig.m_nSubAreaHeight[nAreaIndex], crFColor, crBColor, *ws);
				else
					x += TDotFont32::DrawChar(imgScreen->Picture->Bitmap->Canvas, x, y + nVertScreen * sysConfig.m_nSubAreaHeight[nAreaIndex], crFColor, crBColor, *ws);
				ws++;
				wsColor++;
				wsActive++;
			}
		}
		else
		{
			imgScreen->Picture->Bitmap->Canvas->Font->Color = clWhite;
			imgScreen->Picture->Bitmap->Canvas->Brush->Color = clBlack;
			TextOutW(imgScreen->Picture->Bitmap->Canvas->Handle, nOffsetX, y + nVertScreen * sysConfig.m_nSubAreaHeight[nAreaIndex], ws, strListText->Strings[nRow].Length());
			nCount = 1;
			DWORD crFColor, crBColor;
			while (*ws)
			{
				GetTextExtentPoint32W(imgScreen->Picture->Bitmap->Canvas->Handle, strListText->Strings[nRow].c_str(), nCount, &sz);
				if (*wsActive == '$')
					utils.GetRGBColor(*wsColor, '$', &crFColor, &crBColor);
				else
				{
					crFColor = clBlack;
					crBColor = clBlack;
				}
				imgScreen->Picture->Bitmap->Canvas->Brush->Style = bsSolid;
				imgScreen->Picture->Bitmap->Canvas->Brush->Color = TColor(crFColor ^ crBColor);
				SetROP2(imgScreen->Picture->Bitmap->Canvas->Handle, R2_MASKPEN);
				imgScreen->Picture->Bitmap->Canvas->Rectangle(x, y + nVertScreen * sysConfig.m_nSubAreaHeight[nAreaIndex], nOffsetX + sz.cx + 1, y + nVertScreen * sysConfig.m_nSubAreaHeight[nAreaIndex] + sz.cy + 1);
				imgScreen->Picture->Bitmap->Canvas->Brush->Color = TColor(crBColor);
				SetROP2(imgScreen->Picture->Bitmap->Canvas->Handle, R2_XORPEN);
				imgScreen->Picture->Bitmap->Canvas->Rectangle(x, y + nVertScreen * sysConfig.m_nSubAreaHeight[nAreaIndex], nOffsetX + sz.cx + 1, y + nVertScreen * sysConfig.m_nSubAreaHeight[nAreaIndex] + sz.cy + 1);

				x = sz.cx;
				ws++;
				wsColor++;
				nCount++;
				wsActive++;
			}
			SetROP2(imgScreen->Picture->Bitmap->Canvas->Handle, R2_COPYPEN);
		}

		y += nTopSpace + nFontHeight;
		if (y + nFontHeight > sysConfig.m_nSubAreaHeight[nAreaIndex])
		{
			y = nTopSpace;
			nVertScreen++;
		}
	}

	if (sysConfig.m_picAct != NULL)
		::BitBlt(
		imgScreen->Picture->Bitmap->Canvas->Handle,
		sysConfig.m_nActImagePosX,
		sysConfig.m_nActImagePosY,
		sysConfig.m_picAct->Width,
		sysConfig.m_picAct->Height,
		sysConfig.m_picAct->Bitmap->Canvas->Handle,
		0,
		0,
		SRCCOPY);

	delete strListText;
	delete strListColor;
	delete strListActive;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetColorCount()
{
	if (sysConfig.m_bFullColor)
		return 3;
	return (sysConfig.m_bDoubleColor ? 2 : 1);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_ScrollLeft(int nAreaIndex)
{
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;
	return nByteHeight * imgScreen->Picture->Bitmap->Width * GetColorCount() + sizeof(IMAGE_OBJECT_HEAD) * 2;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_MoveLeft(int nAreaIndex)
{
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;
	return nByteHeight * sysConfig.m_nSubAreaWidth[nAreaIndex] * GetColorCount() + sizeof(IMAGE_OBJECT_HEAD) * 2;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_MoveLeft(int nAreaIndex)
{
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;
	return sizeof(IMAGE_OBJECT_HEAD) + nByteHeight * sysConfig.m_nSubAreaWidth[nAreaIndex] * GetColorCount();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_ScrollRight(int nAreaIndex)
{
	return GetFlashImageLen_ScrollLeft(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_MoveRight(int nAreaIndex)
{
	return GetFlashImageLen_MoveLeft(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_MoveRight(int nAreaIndex)
{
	return GetFlashImageLen_Exit_MoveLeft(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_ScrollUp(int nAreaIndex)
{
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;
	return nByteWidth * imgScreen->Picture->Bitmap->Height * GetColorCount() + sizeof(IMAGE_OBJECT_HEAD) * 2;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_MoveUp(int nAreaIndex)
{
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;
	return nByteWidth * sysConfig.m_nSubAreaHeight[nAreaIndex] * GetColorCount() + sizeof(IMAGE_OBJECT_HEAD) * 2;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_MoveUp(int nAreaIndex)
{
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;
	return sizeof(IMAGE_OBJECT_HEAD) + nByteWidth * sysConfig.m_nSubAreaHeight[nAreaIndex] * GetColorCount();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_ScrollDown(int nAreaIndex)
{
	return GetFlashImageLen_ScrollUp(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_MoveDown(int nAreaIndex)
{
	return GetFlashImageLen_MoveUp(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_MoveDown(int nAreaIndex)
{
	return GetFlashImageLen_Exit_MoveUp(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_LeftToRightSpread(int nAreaIndex)
{
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;
	return (nByteHeight * GetColorCount() + sizeof(IMAGE_OBJECT_HEAD)) * sysConfig.m_nSubAreaWidth[nAreaIndex];
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_LeftToRightErase(int nAreaIndex)
{
	return sizeof(IMAGE_OBJECT_HEAD) * sysConfig.m_nSubAreaWidth[nAreaIndex];
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_RightToLeftSpread(int nAreaIndex)
{
	return  GetFlashImageLen_LeftToRightSpread(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_RightToLeftErase(int nAreaIndex)
{
	return  GetFlashImageLen_Exit_LeftToRightErase(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_TopToBottomSpread(int nAreaIndex)
{
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;
	return (nByteWidth * GetColorCount() + sizeof(IMAGE_OBJECT_HEAD)) * sysConfig.m_nSubAreaHeight[nAreaIndex];
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_TopToBottomErase(int nAreaIndex)
{
	return sizeof(IMAGE_OBJECT_HEAD) * sysConfig.m_nSubAreaHeight[nAreaIndex];
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_BottomToTopSpread(int nAreaIndex)
{
	return  GetFlashImageLen_TopToBottomSpread(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_BottomToTopErase(int nAreaIndex)
{
	return  GetFlashImageLen_TopToBottomSpread(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_H_ToCenterSpread(int nAreaIndex)
{
	return  GetFlashImageLen_LeftToRightSpread(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_H_ToCenterErase(int nAreaIndex)
{
	return  GetFlashImageLen_Exit_LeftToRightErase(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_H_FromCenterSpread(int nAreaIndex)
{
	return  GetFlashImageLen_LeftToRightSpread(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_H_FromCenterErase(int nAreaIndex)
{
	return  GetFlashImageLen_Exit_LeftToRightErase(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_H_Shutter(int nAreaIndex)
{
	return  GetFlashImageLen_LeftToRightSpread(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_H_Shutter(int nAreaIndex)
{
	return  GetFlashImageLen_Exit_LeftToRightErase(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_V_Shutter(int nAreaIndex)
{
	return  GetFlashImageLen_TopToBottomSpread(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_V_Shutter(int nAreaIndex)
{
	return  GetFlashImageLen_Exit_TopToBottomErase(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_V_ToCenterSpread(int nAreaIndex)
{
	return  GetFlashImageLen_TopToBottomSpread(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_V_ToCenterErase(int nAreaIndex)
{
	return  GetFlashImageLen_Exit_TopToBottomErase(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_V_FromCenterSpread(int nAreaIndex)
{
	return  GetFlashImageLen_TopToBottomSpread(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_V_FromCenterErase(int nAreaIndex)
{
	return  GetFlashImageLen_Exit_TopToBottomErase(nAreaIndex);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Direct(int nAreaIndex)
{
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;
	return nByteWidth * sysConfig.m_nSubAreaHeight[nAreaIndex] * GetColorCount() + sizeof(IMAGE_OBJECT_HEAD);
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Flash(int nAreaIndex)
{
	return 3 * (GetFlashImageLen_Direct(nAreaIndex) + GetFlashImageLen_Exit_Direct(nAreaIndex));
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetFlashImageLen_Exit_Direct(int nAreaIndex)
{
	return sizeof(IMAGE_OBJECT_HEAD);
}
//---------------------------------------------------------------------------

BYTE TfrmMain::GetBit(BYTE c, int x, int y, DWORD dwColorMask)
{
	c <<= 1;
	if ((x  < imgScreen->Picture->Bitmap->Width) && (y < imgScreen->Picture->Bitmap->Height))
	{
		if (imgScreen->Picture->Bitmap->Canvas->Pixels[x][y] & 0xE0E0E0 & dwColorMask)
			c |= 1;
	}

	return c;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Direct_YL2010C(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
//Color			R	 G    B			R(4) G(4) B(4)	Value
//黑：          0,	 0，  0 		0	 0    0		0
//兰：          0，  0，  128  		0	 0    2		1
//绿：          0，	 128，0  		0	 2    0		2
//青：          0，	 128，128  		0	 2    2		3
//红：         	128，0，  0  		2	 0    0		4
//品红：     	128, 0，  128  		2	 0    2		5
//棕：         	128，128，0  		2	 2    0		6
//灰白：     	128，128，128  		2	 2    2		7
//灰：         	64， 64， 64  		1	 1    1		8
//浅兰：     	128，128，255  		0	 0    4		9
//浅绿：     	128，255，128  		0	 4    0		A
//浅青：     	128，255，255  		1	 4    4		B
//浅红：     	255，128，128  		4	 0    0		C
//浅品红：		255，128，255  		4	 1    4		D
//黄：         	255，255，128  		4	 4    0		E
//白：         	255，255，255 		4	 4    4		F
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;

	pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
	p = (BYTE*)(pFlashImageHead + 1);

	for (y = 0; y < sysConfig.m_nSubAreaHeight[nAreaIndex]; y++)
	{
		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c >>= 4;
			if ((x  < imgScreen->Picture->Bitmap->Width) && (y < imgScreen->Picture->Bitmap->Height))
			{
				DWORD ulColor = imgScreen->Picture->Bitmap->Canvas->Pixels[x][y];

				if (ulColor == 0x000000)
					c |= 0x00;
				else if (ulColor == 0x800000)
					c |= 0x10;
				else if (ulColor == 0x008000)
					c |= 0x20;
				else if (ulColor == 0x808000)
					c |= 0x30;
				else if (ulColor == 0x000080)
					c |= 0x40;
				else if (ulColor == 0x800080)
					c |= 0x50;
				else if (ulColor == 0x008080)
					c |= 0x60;
				else if (ulColor == 0x808080)
					c |= 0x70;
				else if (ulColor == 0x404040)
					c |= 0x80;
				else if (ulColor == 0xFF0000)
					c |= 0x90;
				else if (ulColor == 0x00FF00)
					c |= 0xA0;
				else if (ulColor == 0xFFFF00)
					c |= 0xB0;
				else if (ulColor == 0x0000FF)
					c |= 0xC0;
				else if (ulColor == 0xFF00FF)
					c |= 0xD0;
				else if (ulColor == 0x00FFFF)
					c |= 0xE0;
				else //if (ulColor == 0x00FFFF)
					c |= 0xF0;
			}

			i++;
			if (i >= 2)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}
	}

	pFlashImageHead->ucObjectType = OBJ_TYPE_4BIT_COLOR_IMAGE;
	pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
	pFlashImageHead->usRepeatCount = 1;
	if (IsCurrentActActive())
		pFlashImageHead->ulPara = 0;   //显示时间us
	else
		pFlashImageHead->ulPara = sysConfig.m_nActShowTime * 1000;	//显示时间us
	pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
	pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
	pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
	pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
	pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
	pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

	return p;
}
//---------------------------------------------------------------------------

int TfrmMain::GetImageType()
{
	if (sysConfig.m_bFullColor)
		return  OBJ_TYPE_THREE_COLOR_IMAGE;

	return (sysConfig.m_bDoubleColor) ? OBJ_TYPE_DOUBLE_COLOR_IMAGE : OBJ_TYPE_SINGLE_COLOR_IMAGE;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Direct(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;

	pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
	pFlashImageHead->ucObjectType = GetImageType();
	pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
	pFlashImageHead->usRepeatCount = 1;
	if (IsCurrentActActive())
		pFlashImageHead->ulPara = 0;   //显示时间us
	else
		pFlashImageHead->ulPara = sysConfig.m_nActShowTime * 1000;	//显示时间us
	pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
	pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
	pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
	pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
	pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
	pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

	p = (BYTE*)(pFlashImageHead + 1);

	for (y = 0; y < sysConfig.m_nSubAreaHeight[nAreaIndex]; y++)
	{
		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}
	}

	if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
	{
		for (y = 0; y < sysConfig.m_nSubAreaHeight[nAreaIndex]; y++)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	if (sysConfig.m_bFullColor)
	{
		for (y = 0; y < sysConfig.m_nSubAreaHeight[nAreaIndex]; y++)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Flash(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	for (int i = 0; i < 3; i++)
	{
		p = CreateFlashImage_Flash_Dark(nActIndex, nAreaIndex, p);
		p = CreateFlashImage_Direct(nActIndex, nAreaIndex, xStart, yStart, p);
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Flash_Dark(int nActIndex, int nAreaIndex, BYTE* p)
{
	IMAGE_OBJECT_HEAD* pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;

	pFlashImageHead->ucObjectType = GetImageType();
	pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
	pFlashImageHead->usRepeatCount = 1;
	pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;
	pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
	pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
	pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
	pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
	pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
	pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

	return p + sizeof(IMAGE_OBJECT_HEAD);
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_Direct(int nActIndex, int nAreaIndex, BYTE* p)
{
	IMAGE_OBJECT_HEAD* pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;

	pFlashImageHead->ucObjectType = GetImageType();
	pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
	pFlashImageHead->usRepeatCount = 1;
	pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;
	pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
	pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
	pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
	pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
	pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
	pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

	return p + sizeof(IMAGE_OBJECT_HEAD);
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_ScrollLeft(int nActIndex, int nAreaIndex, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;

	for (x = 0; x < imgScreen->Picture->Bitmap->Width; x++)
	{
		if ((x == 0) || (x == imgScreen->Picture->Bitmap->Width - 1))
		{
			pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
			pFlashImageHead->ucObjectType = GetImageType();
			pFlashImageHead->ucActType = ACT_TYPE_MOVE_LEFT;
			if (x == 0)
			{
				pFlashImageHead->usRepeatCount = imgScreen->Picture->Bitmap->Width - 1;
				pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
			}
			else
			{
				pFlashImageHead->usRepeatCount = 1;
				pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
			}
			pFlashImageHead->usDisplayPosX = sysConfig.m_nSubAreaWidth[nAreaIndex] - 1;	//显示区域坐上角X坐标
			pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
			pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
			pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
			pFlashImageHead->usImageWidth = 1;		//图像宽度
			pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

			p = (BYTE*)(pFlashImageHead + 1);
		}

		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c = GetBit(c, x, y, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x, y, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x, y, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_MoveLeft(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;

	for (x = 0; x < sysConfig.m_nSubAreaWidth[nAreaIndex]; x++)
	{
		if ((x == 0) || (x == sysConfig.m_nSubAreaWidth[nAreaIndex] - 1))
		{
			pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
			pFlashImageHead->ucObjectType = GetImageType();
			pFlashImageHead->ucActType = ACT_TYPE_MOVE_LEFT;
			if (x == 0)
			{
				pFlashImageHead->usRepeatCount = sysConfig.m_nSubAreaWidth[nAreaIndex] - 1;
				pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
			}
			else
			{
				pFlashImageHead->usRepeatCount = 1;
				pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
			}
			pFlashImageHead->usDisplayPosX = sysConfig.m_nSubAreaWidth[nAreaIndex] - 1;	//显示区域坐上角X坐标
			pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
			pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
			pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
			pFlashImageHead->usImageWidth = 1;		//图像宽度
			pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

			p = (BYTE*)(pFlashImageHead + 1);
		}

		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_MoveLeft(int nActIndex, int nAreaIndex, BYTE* p)
{
	IMAGE_OBJECT_HEAD* pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;

	p = (BYTE*)(pFlashImageHead + 1);
	memset(p, 0, nByteHeight * sysConfig.m_nSubAreaWidth[nAreaIndex] * GetColorCount());

	pFlashImageHead->ucObjectType = GetImageType();
	pFlashImageHead->ucActType = ACT_TYPE_MOVE_LEFT;
	pFlashImageHead->usRepeatCount = sysConfig.m_nSubAreaWidth[nAreaIndex];
	pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
	pFlashImageHead->usDisplayPosX = sysConfig.m_nSubAreaWidth[nAreaIndex] - 1;	//显示区域坐上角X坐标
	pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
	pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
	pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
	pFlashImageHead->usImageWidth = 1;		//图像宽度
	pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

	return p + nByteHeight * sysConfig.m_nSubAreaWidth[nAreaIndex] * GetColorCount();
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_ScrollRight(int nActIndex, int nAreaIndex, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;

	for (x = imgScreen->Picture->Bitmap->Width - 1; x >= 0; x--)
	{
		if ((x == 0) || (x == imgScreen->Picture->Bitmap->Width - 1))
		{
			pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
			pFlashImageHead->ucObjectType = GetImageType();
			pFlashImageHead->ucActType = ACT_TYPE_MOVE_RIGHT;
			if (x == 0)
			{
				pFlashImageHead->usRepeatCount = 1;
				pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
			}
			else
			{
				pFlashImageHead->usRepeatCount = imgScreen->Picture->Bitmap->Width - 1;
				pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
			}
			pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
			pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
			pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
			pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
			pFlashImageHead->usImageWidth = 1;		//图像宽度
			pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

			p = (BYTE*)(pFlashImageHead + 1);
		}

		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c = GetBit(c, x, y, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x, y, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x, y, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_MoveRight(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;

	for (x = sysConfig.m_nSubAreaWidth[nAreaIndex] - 1; x >= 0; x--)
	{
		if ((x == 0) || (x == sysConfig.m_nSubAreaWidth[nAreaIndex] - 1))
		{
			pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
			pFlashImageHead->ucObjectType = GetImageType();
			pFlashImageHead->ucActType = ACT_TYPE_MOVE_RIGHT;
			if (x == 0)
			{
				pFlashImageHead->usRepeatCount = 1;
				pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
			}
			else
			{
				pFlashImageHead->usRepeatCount = sysConfig.m_nSubAreaWidth[nAreaIndex] - 1;
				pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
			}
			pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
			pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
			pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
			pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
			pFlashImageHead->usImageWidth = 1;		//图像宽度
			pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

			p = (BYTE*)(pFlashImageHead + 1);
		}

		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_MoveRight(int nActIndex, int nAreaIndex, BYTE* p)
{
	IMAGE_OBJECT_HEAD* pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;

	p = (BYTE*)(pFlashImageHead + 1);
	memset(p, 0, nByteHeight * sysConfig.m_nSubAreaWidth[nAreaIndex] * GetColorCount());

	pFlashImageHead->ucObjectType = GetImageType();
	pFlashImageHead->ucActType = ACT_TYPE_MOVE_RIGHT;
	pFlashImageHead->usRepeatCount = sysConfig.m_nSubAreaWidth[nAreaIndex];
	pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
	pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
	pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
	pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
	pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
	pFlashImageHead->usImageWidth = 1;		//图像宽度
	pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

	return p + nByteHeight * sysConfig.m_nSubAreaWidth[nAreaIndex] * GetColorCount();
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_ScrollUp(int nActIndex, int nAreaIndex, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;

	for (y = 0; y < imgScreen->Picture->Bitmap->Height; y++)
	{
		if ((y == 0) || (y == imgScreen->Picture->Bitmap->Height - 1))
		{
			pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
			pFlashImageHead->ucObjectType = GetImageType();
			pFlashImageHead->ucActType = ACT_TYPE_MOVE_UP;
			if (y == 0)
			{
				pFlashImageHead->usRepeatCount = imgScreen->Picture->Bitmap->Height - 1;
				pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
			}
			else
			{
				pFlashImageHead->usRepeatCount = 1;
				pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
			}
			pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
			pFlashImageHead->usDisplayPosY = sysConfig.m_nSubAreaHeight[nAreaIndex] - 1;	//显示区域坐上角Y坐标
			pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
			pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
			pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
			pFlashImageHead->usImageHeight = 1;		//图像高度

			p = (BYTE*)(pFlashImageHead + 1);
		}

		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x, y, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x, y, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x, y, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_MoveUp(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;

	for (y = 0; y < sysConfig.m_nSubAreaHeight[nAreaIndex]; y++)
	{
		if ((y == 0) || (y == sysConfig.m_nSubAreaHeight[nAreaIndex] - 1))
		{
			pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
			pFlashImageHead->ucObjectType = GetImageType();
			pFlashImageHead->ucActType = ACT_TYPE_MOVE_UP;
			if (y == 0)
			{
				pFlashImageHead->usRepeatCount = sysConfig.m_nSubAreaHeight[nAreaIndex] - 1;
				pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
			}
			else
			{
				pFlashImageHead->usRepeatCount = 1;
				pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
			}
			pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
			pFlashImageHead->usDisplayPosY = sysConfig.m_nSubAreaHeight[nAreaIndex] - 1;	//显示区域坐上角Y坐标
			pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
			pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
			pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
			pFlashImageHead->usImageHeight = 1;		//图像高度

			p = (BYTE*)(pFlashImageHead + 1);
		}

		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_MoveUp(int nActIndex, int nAreaIndex, BYTE* p)
{
	IMAGE_OBJECT_HEAD* pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;

	p = (BYTE*)(pFlashImageHead + 1);
	memset(p, 0, nByteWidth * sysConfig.m_nSubAreaHeight[nAreaIndex] * GetColorCount());

	pFlashImageHead->ucObjectType = GetImageType();
	pFlashImageHead->ucActType = ACT_TYPE_MOVE_UP;
	pFlashImageHead->usRepeatCount = sysConfig.m_nSubAreaHeight[nAreaIndex];
	pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
	pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
	pFlashImageHead->usDisplayPosY = sysConfig.m_nSubAreaHeight[nAreaIndex] - 1;	//显示区域坐上角Y坐标
	pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
	pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
	pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
	pFlashImageHead->usImageHeight = 1;		//图像高度

	return p + nByteWidth * sysConfig.m_nSubAreaHeight[nAreaIndex] * GetColorCount();
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_ScrollDown(int nActIndex, int nAreaIndex, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;

	for (y = imgScreen->Picture->Bitmap->Height - 1; y >= 0; y--)
	{
		if ((y == 0) || (y == imgScreen->Picture->Bitmap->Height - 1))
		{
			pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
			pFlashImageHead->ucObjectType = GetImageType();
			pFlashImageHead->ucActType = ACT_TYPE_MOVE_DOWN;
			if (y == 0)
			{
				pFlashImageHead->usRepeatCount = 1;
				pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
			}
			else
			{
				pFlashImageHead->usRepeatCount = imgScreen->Picture->Bitmap->Height - 1;
				pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
			}
			pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
			pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
			pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
			pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
			pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
			pFlashImageHead->usImageHeight = 1;		//图像高度

			p = (BYTE*)(pFlashImageHead + 1);
		}

		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x, y, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x, y, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x, y, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_MoveDown(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;

	for (y = sysConfig.m_nSubAreaHeight[nAreaIndex] - 1; y >= 0; y--)
	{
		if ((y == 0) || (y == sysConfig.m_nSubAreaHeight[nAreaIndex] - 1))
		{
			pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
			pFlashImageHead->ucObjectType = GetImageType();
			pFlashImageHead->ucActType = ACT_TYPE_MOVE_DOWN;
			if (y == 0)
			{
				pFlashImageHead->usRepeatCount = 1;
				pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
			}
			else
			{
				pFlashImageHead->usRepeatCount = sysConfig.m_nSubAreaHeight[nAreaIndex] - 1;
				pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
			}
			pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
			pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
			pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
			pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
			pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
			pFlashImageHead->usImageHeight = 1;		//图像高度

			p = (BYTE*)(pFlashImageHead + 1);
		}

		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_MoveDown(int nActIndex, int nAreaIndex, BYTE* p)
{
	IMAGE_OBJECT_HEAD* pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;

	p = (BYTE*)(pFlashImageHead + 1);
	memset(p, 0, nByteWidth * sysConfig.m_nSubAreaHeight[nAreaIndex] * GetColorCount());

	pFlashImageHead->ucObjectType = GetImageType();
	pFlashImageHead->ucActType = ACT_TYPE_MOVE_DOWN;
	pFlashImageHead->usRepeatCount = sysConfig.m_nSubAreaHeight[nAreaIndex];
	pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
	pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
	pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
	pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
	pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
	pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
	pFlashImageHead->usImageHeight = 1;		//图像高度

	return p + nByteWidth * sysConfig.m_nSubAreaHeight[nAreaIndex] * GetColorCount();
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_LeftToRightSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;

	for (x = 0; x < sysConfig.m_nSubAreaWidth[nAreaIndex]; x++)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		if (x != sysConfig.m_nSubAreaWidth[nAreaIndex] - 1)
			pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
		else
			pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
		pFlashImageHead->usDisplayPosX = x;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_LeftToRightErase(int nActIndex, int nAreaIndex, BYTE* p)
{
	int x;
	IMAGE_OBJECT_HEAD* pFlashImageHead;

	for (x = 0; x < sysConfig.m_nSubAreaWidth[nAreaIndex]; x++)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
		pFlashImageHead->usDisplayPosX = x;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_RightToLeftSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;

	for (x = sysConfig.m_nSubAreaWidth[nAreaIndex] - 1; x >= 0; x--)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		if (x != 0)
			pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
		else
			pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
		pFlashImageHead->usDisplayPosX = x;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_RightToLeftErase(int nActIndex, int nAreaIndex, BYTE* p)
{
	int x;
	IMAGE_OBJECT_HEAD* pFlashImageHead;

	for (x = sysConfig.m_nSubAreaWidth[nAreaIndex] - 1; x >= 0; x--)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
		pFlashImageHead->usDisplayPosX = x;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_TopToBottomSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;

	for (y = 0; y < sysConfig.m_nSubAreaHeight[nAreaIndex]; y++)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		if (y != sysConfig.m_nSubAreaHeight[nAreaIndex] - 1)
			pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
		else
			pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = y;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_TopToBottomErase(int nActIndex, int nAreaIndex, BYTE* p)
{
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int y;

	for (y = 0; y < sysConfig.m_nSubAreaHeight[nAreaIndex]; y++)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = y;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_BottomToTopSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;

	for (y = sysConfig.m_nSubAreaHeight[nAreaIndex] - 1; y >= 0; y--)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		if (y != 0)
			pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
		else
			pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = y;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_BottomToTopErase(int nActIndex, int nAreaIndex, BYTE* p)
{
	int y;
	IMAGE_OBJECT_HEAD* pFlashImageHead;

	for (y = sysConfig.m_nSubAreaHeight[nAreaIndex] - 1; y >= 0; y--)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = y;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_H_ToCenterSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;
	int nLeftBit = sysConfig.m_nSubAreaWidth[nAreaIndex] & 1;

	for (x = 0; x < sysConfig.m_nSubAreaWidth[nAreaIndex] / 2; x++)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = 0;
		pFlashImageHead->usDisplayPosX = x;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		if (x != sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 - 1)
			pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
		else
		{
			if (nLeftBit)
				pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
			else
				pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
		}
		pFlashImageHead->usDisplayPosX = sysConfig.m_nSubAreaWidth[nAreaIndex] - x - 1;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c = GetBit(c, xStart + sysConfig.m_nSubAreaWidth[nAreaIndex] - x - 1, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, xStart + sysConfig.m_nSubAreaWidth[nAreaIndex] - x - 1, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, xStart + sysConfig.m_nSubAreaWidth[nAreaIndex] - x - 1, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	if (nLeftBit)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
		pFlashImageHead->usDisplayPosX = sysConfig.m_nSubAreaWidth[nAreaIndex] - x - 1;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_H_ToCenterErase(int nActIndex, int nAreaIndex, BYTE* p)
{
	int x;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nLeftBit = sysConfig.m_nSubAreaWidth[nAreaIndex] & 1;

	for (x = 0; x < sysConfig.m_nSubAreaWidth[nAreaIndex] / 2; x++)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = 0;
		pFlashImageHead->usDisplayPosX = x;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p += sizeof(IMAGE_OBJECT_HEAD);

		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
		pFlashImageHead->usDisplayPosX = sysConfig.m_nSubAreaWidth[nAreaIndex] - x - 1;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p += sizeof(IMAGE_OBJECT_HEAD);
	}

	if (nLeftBit)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = 0;
		pFlashImageHead->usDisplayPosX = x;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p += sizeof(IMAGE_OBJECT_HEAD);
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_H_FromCenterSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;
	int nLeftBit = sysConfig.m_nSubAreaWidth[nAreaIndex] & 1;

	if (nLeftBit)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		if (sysConfig.m_nSubAreaWidth[nAreaIndex] != 1)
			pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
		else
			pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
		pFlashImageHead->usDisplayPosX = sysConfig.m_nSubAreaWidth[nAreaIndex] / 2;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c = GetBit(c, sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	for (x = 0; x < sysConfig.m_nSubAreaWidth[nAreaIndex] / 2; x++)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = 0;
		pFlashImageHead->usDisplayPosX = sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 - x - 1;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c = GetBit(c, xStart + sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 - x - 1, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, xStart + sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 - x - 1, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, xStart + sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 - x - 1, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		if (x != sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 - 1)
			pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
		else
			pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
		pFlashImageHead->usDisplayPosX = sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 + x + nLeftBit;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c = GetBit(c, sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 + x + nLeftBit + xStart, y + yStart, RGB(255, 0, 0));

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 + x + nLeftBit + xStart, y + yStart, RGB(0, 255, 0));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (y = 0; y < nByteHeight * 8; y++)
			{
				c = GetBit(c, sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 + x + nLeftBit + xStart, y + yStart, RGB(0, 0, 255));

				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_H_FromCenterErase(int nActIndex, int nAreaIndex, BYTE* p)
{
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int x;
	int nLeftBit = sysConfig.m_nSubAreaWidth[nAreaIndex] & 1;

	if (nLeftBit)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
		pFlashImageHead->usDisplayPosX = sysConfig.m_nSubAreaWidth[nAreaIndex] / 2;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);
	}

	for (x = 0; x < sysConfig.m_nSubAreaWidth[nAreaIndex] / 2; x++)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = 0;
		pFlashImageHead->usDisplayPosX = sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 - x - 1;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
		pFlashImageHead->usDisplayPosX = sysConfig.m_nSubAreaWidth[nAreaIndex] / 2 + x + nLeftBit;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
		pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
		pFlashImageHead->usImageWidth = 1;		//图像宽度
		pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_V_ToCenterSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;
	int nLeftBit = sysConfig.m_nSubAreaHeight[nAreaIndex] & 1;

	for (y = 0; y < sysConfig.m_nSubAreaHeight[nAreaIndex] / 2; y++)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = 0;
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = y;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));
			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		if (y != sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 - 1)
			pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
		else
		{
			if (nLeftBit)
				pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
			else
				pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
		}
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = sysConfig.m_nSubAreaHeight[nAreaIndex] - y - 1;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x + xStart, yStart + sysConfig.m_nSubAreaHeight[nAreaIndex] - y - 1, RGB(255, 0, 0));
			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, yStart + sysConfig.m_nSubAreaHeight[nAreaIndex] - y - 1, RGB(0, 255, 0));
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, yStart + sysConfig.m_nSubAreaHeight[nAreaIndex] - y - 1, RGB(0, 0, 255));
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	if (nLeftBit)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = y;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x + xStart, y + yStart, RGB(255, 0, 0));
			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 255, 0));
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, y + yStart, RGB(0, 0, 255));
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_V_ToCenterErase(int nActIndex, int nAreaIndex, BYTE* p)
{
	int y;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nLeftBit = sysConfig.m_nSubAreaHeight[nAreaIndex] & 1;

	for (y = 0; y < sysConfig.m_nSubAreaHeight[nAreaIndex] / 2; y++)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = 0;
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = y;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p += sizeof(IMAGE_OBJECT_HEAD);

		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = sysConfig.m_nSubAreaHeight[nAreaIndex] - y - 1;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p += sizeof(IMAGE_OBJECT_HEAD);
	}

	if (nLeftBit)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = y;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p += sizeof(IMAGE_OBJECT_HEAD);
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_V_FromCenterSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;
	int nLeftBit = sysConfig.m_nSubAreaHeight[nAreaIndex] & 1;

	if (nLeftBit)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		if (sysConfig.m_nSubAreaHeight[nAreaIndex] != 1)
			pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
		else
			pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = sysConfig.m_nSubAreaHeight[nAreaIndex] / 2;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x + xStart, yStart + sysConfig.m_nSubAreaHeight[nAreaIndex] / 2, RGB(255, 0, 0));
			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, yStart + sysConfig.m_nSubAreaHeight[nAreaIndex] / 2, RGB(0, 255, 0));
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, yStart + sysConfig.m_nSubAreaHeight[nAreaIndex] / 2, RGB(0, 0, 255));
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	for (y = 0; y < sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 ; y++)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = 0;
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 - y - 1;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x + xStart, yStart + sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 - y - 1, RGB(255, 0, 0));
			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, yStart + sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 - y - 1, RGB(0, 255, 0));
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, yStart + sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 - y - 1, RGB(0, 0, 255));
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
		pFlashImageHead->usRepeatCount = 1;
		if (y != sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 - 1)
			pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
		else
			pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 + y + nLeftBit;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p = (BYTE*)(pFlashImageHead + 1);

		c = 0;
		i = 0;
		for (x = 0; x < nByteWidth * 8; x++)
		{
			c = GetBit(c, x + xStart, yStart + sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 + y + nLeftBit, RGB(255, 0, 0));
			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}

		if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, yStart + sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 + y + nLeftBit, RGB(0, 255, 0));
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}

		if (sysConfig.m_bFullColor)
		{
			c = 0;
			i = 0;
			for (x = 0; x < nByteWidth * 8; x++)
			{
				c = GetBit(c, x + xStart, yStart + sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 + y + nLeftBit, RGB(0, 0, 255));
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_V_FromCenterErase(int nActIndex, int nAreaIndex, BYTE* p)
{
	int y;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nLeftBit = sysConfig.m_nSubAreaHeight[nAreaIndex] & 1;

	if (nLeftBit)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = 0;
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = sysConfig.m_nSubAreaHeight[nAreaIndex] / 2;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p += sizeof(IMAGE_OBJECT_HEAD);
	}

	for (y = 0; y < sysConfig.m_nSubAreaHeight[nAreaIndex] / 2; y++)
	{
		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = 0;
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 - 1 - y;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p += sizeof(IMAGE_OBJECT_HEAD);

		pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
		pFlashImageHead->ucObjectType = GetImageType();
		pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
		pFlashImageHead->usRepeatCount = 1;
		pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
		pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
		pFlashImageHead->usDisplayPosY = sysConfig.m_nSubAreaHeight[nAreaIndex] / 2 + y + nLeftBit;	//显示区域坐上角Y坐标
		pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
		pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
		pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
		pFlashImageHead->usImageHeight = 1;		//图像高度

		p += sizeof(IMAGE_OBJECT_HEAD);
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_H_Shutter(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i, n;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteHeight = (sysConfig.m_nSubAreaHeight[nAreaIndex] + 7) >> 3;
	int nShutterCount = (sysConfig.m_nSubAreaWidth[nAreaIndex] + sysConfig.m_nActShutterWidth - 1) / sysConfig.m_nActShutterWidth;

	for (x = 0; x < sysConfig.m_nActShutterWidth; x++)
	{
		for (n = 0; n < nShutterCount; n++)
		{
			if (n * sysConfig.m_nActShutterWidth + x < sysConfig.m_nSubAreaWidth[nAreaIndex])
			{
				pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
				pFlashImageHead->ucObjectType = GetImageType();
				pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
				pFlashImageHead->usRepeatCount = 1;
				pFlashImageHead->ulPara = 0;
				pFlashImageHead->usDisplayPosX = n * sysConfig.m_nActShutterWidth + x;	//显示区域坐上角X坐标
				pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
				pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
				pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
				pFlashImageHead->usImageWidth = 1;		//图像宽度
				pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度

				p = (BYTE*)(pFlashImageHead + 1);

				c = 0;
				i = 0;
				for (y = 0; y < nByteHeight * 8; y++)
				{
					c = GetBit(c, n * sysConfig.m_nActShutterWidth + x + xStart, y + yStart, RGB(255, 0, 0));
					i++;
					if (i >= 8)
					{
						*p++ = c;
						i = 0;
						c =0;
					}
				}

				if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
				{
					c = 0;
					i = 0;
					for (y = 0; y < nByteHeight * 8; y++)
					{
						c = GetBit(c, n * sysConfig.m_nActShutterWidth + x + xStart, y + yStart, RGB(0, 255, 0));
						i++;
						if (i >= 8)
						{
							*p++ = c;
							i = 0;
							c =0;
						}
					}
				}

				if (sysConfig.m_bFullColor)
				{
					c = 0;
					i = 0;
					for (y = 0; y < nByteHeight * 8; y++)
					{
						c = GetBit(c, n * sysConfig.m_nActShutterWidth + x + xStart, y + yStart, RGB(0, 0, 255));
						i++;
						if (i >= 8)
						{
							*p++ = c;
							i = 0;
							c =0;
						}
					}
				}
			}
		}
		pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
	}

	pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_H_ShutterErase(int nActIndex, int nAreaIndex, BYTE* p)
{
	int x, n;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nShutterCount = (sysConfig.m_nSubAreaWidth[nAreaIndex] + sysConfig.m_nActShutterWidth - 1) / sysConfig.m_nActShutterWidth;

	for (x = 0; x < sysConfig.m_nActShutterWidth; x++)
	{
		for (n = 0; n < nShutterCount; n++)
		{
			if (n * sysConfig.m_nActShutterWidth + x < sysConfig.m_nSubAreaWidth[nAreaIndex])
			{
				pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
				pFlashImageHead->ucObjectType = GetImageType();
				pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
				pFlashImageHead->usRepeatCount = 1;
				pFlashImageHead->ulPara = 0;
				pFlashImageHead->usDisplayPosX = n * sysConfig.m_nActShutterWidth + x;	//显示区域坐上角X坐标
				pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
				pFlashImageHead->usDisplayWidth = 1;		//显示区域宽度
				pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域高度
				pFlashImageHead->usImageWidth = 1;		//图像宽度
				pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];		//图像高度
				p = (BYTE*)(pFlashImageHead + 1);
			}
		}

		pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
	}

	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_V_Shutter(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	int x, y, i, n;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteWidth = (sysConfig.m_nSubAreaWidth[nAreaIndex] + 7) >> 3;
	int nShutterCount = (sysConfig.m_nSubAreaHeight[nAreaIndex] +  sysConfig.m_nActShutterWidth - 1) / sysConfig.m_nActShutterWidth;

	for (y = 0; y < sysConfig.m_nActShutterWidth; y++)
	{
		for (n = 0; n < nShutterCount; n++)
		{
			if (n * sysConfig.m_nActShutterWidth + y < sysConfig.m_nSubAreaHeight[nAreaIndex])
			{
				pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
				pFlashImageHead->ucObjectType = GetImageType();
				pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
				pFlashImageHead->usRepeatCount = 1;
				pFlashImageHead->ulPara = 0;
				pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
				pFlashImageHead->usDisplayPosY = n * sysConfig.m_nActShutterWidth + y;	//显示区域坐上角Y坐标
				pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
				pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
				pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
				pFlashImageHead->usImageHeight = 1;		//图像高度

				p = (BYTE*)(pFlashImageHead + 1);

				c = 0;
				i = 0;
				for (x = 0; x < nByteWidth * 8; x++)
				{
					c = GetBit(c, x + xStart, n * sysConfig.m_nActShutterWidth + y + yStart, RGB(255, 0, 0));
					i++;
					if (i >= 8)
					{
						*p++ = c;
						i = 0;
						c =0;
					}
				}

				if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
				{
					c = 0;
					i = 0;
					for (x = 0; x < nByteWidth * 8; x++)
					{
						c = GetBit(c, x + xStart, n * sysConfig.m_nActShutterWidth + y + yStart, RGB(0, 255, 0));
						i++;
						if (i >= 8)
						{
							*p++ = c;
							i = 0;
							c =0;
						}
					}
				}

				if (sysConfig.m_bFullColor)
				{
					c = 0;
					i = 0;
					for (x = 0; x < nByteWidth * 8; x++)
					{
						c = GetBit(c, x + xStart, n * sysConfig.m_nActShutterWidth + y + yStart, RGB(0, 0, 255));
						i++;
						if (i >= 8)
						{
							*p++ = c;
							i = 0;
							c =0;
						}
					}
				}
			}
		}

		pFlashImageHead->ulPara = sysConfig.m_nActEnterSpeed * 1000;	//速度
	}

	pFlashImageHead->ulPara = (sysConfig.m_nActShowTime + sysConfig.m_nActEnterSpeed) * 1000;	//停留时间
	return p;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateFlashImage_Exit_V_ShutterErase(int nActIndex, int nAreaIndex, BYTE* p)
{
	int y, n;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nShutterCount = (sysConfig.m_nSubAreaHeight[nAreaIndex] +  sysConfig.m_nActShutterWidth - 1) / sysConfig.m_nActShutterWidth;

	for (y = 0; y < sysConfig.m_nActShutterWidth; y++)
	{
		for (n = 0; n < nShutterCount; n++)
		{
			if (n * sysConfig.m_nActShutterWidth + y < sysConfig.m_nSubAreaHeight[nAreaIndex])
			{
				pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
				pFlashImageHead->ucObjectType = GetImageType();
				pFlashImageHead->ucActType = ACT_TYPE_CLEAR;
				pFlashImageHead->usRepeatCount = 1;
				pFlashImageHead->ulPara = 0;
				pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
				pFlashImageHead->usDisplayPosY = n * sysConfig.m_nActShutterWidth + y;	//显示区域坐上角Y坐标
				pFlashImageHead->usDisplayWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//显示区域宽度
				pFlashImageHead->usDisplayHeight = 1;	//显示区域高度
				pFlashImageHead->usImageWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];		//图像宽度
				pFlashImageHead->usImageHeight = 1;		//图像高度

				p += sizeof(IMAGE_OBJECT_HEAD);
			}
		}

		pFlashImageHead->ulPara = sysConfig.m_nActExitSpeed * 1000;	//速度
	}

	return p;
}
//---------------------------------------------------------------------------

void TfrmMain::MakeGS9025TextActData()
{
#pragma pack(1)
typedef struct
{
	BYTE ucActIndex;		//节目编号(0-15)
	BYTE ucAreaIndex;		//分区编号(0-7)
	BYTE ucEnterStyle;		//进入方式(0-?)
	BYTE ucEnterSpeed;		//进入速度(ms)
	DWORD ulShowTime;		//停留时间(ms)
	BYTE ucExitStyle;		//退出方式(0-?)
	BYTE ucExitSpeed;		//退出速度(ms)
	BYTE ucAlignFlags;		//文字排列
	BYTE ucShutterWidth;	//叶片宽度
	BYTE ucRes[2];
	BYTE ucRepeatCount;		//重复播放次数
	BYTE ucStartYear; 		//起始播放时间_年
	BYTE ucStartMonth; 		//起始播放时间_月
	BYTE ucStartDay; 		//起始播放时间_日
	BYTE ucStartHour; 		//起始播放时间_时
	BYTE ucStartMinute; 	//起始播放时间_分
	BYTE ucEndYear; 		//终止播放时间_年
	BYTE ucEndMonth; 		//终止播放时间_月
	BYTE ucEndDay; 			//终止播放时间_日
	BYTE ucEndHour; 		//终止播放时间_时
	BYTE ucEndMinute; 		//终止播放时间_分
	WORD usStart1;			//每日播放时段开始时间 分钟计数
	WORD usEnd1;			//每日播放时段结束时间 分钟计数
	WORD usStart2;			//每日播放时段开始时间 分钟计数
	WORD usEnd2;			//每日播放时段结束时间 分钟计数
	WORD usStart3;			//每日播放时段开始时间 分钟计数
	WORD usEnd3;			//每日播放时段结束时间 分钟计数
	WORD usStart4;			//每日播放时段开始时间 分钟计数
	WORD usEnd4;			//每日播放时段结束时间 分钟计数
	BYTE ucInsertFlag;
	BYTE ucTextSizeIndex;
} GS9025_TEXT_ACT_DEF;
#pragma pack(1)

	GS9025_TEXT_ACT_DEF* pActDef;
	BYTE *pucDataBuf;
	DWORD dwTxDataLen = sizeof(GS9025_TEXT_ACT_DEF);
	TStrings *strListText = new TStringList;
	AnsiString sText;

	strListText->SetText(sysConfig.m_sActText.c_str());
	for (int nRow = 0; nRow < strListText->Count; nRow++)
	{
		if (nRow != 0)
			sText += "\r";
		sText += strListText->Strings[nRow];
	}
	delete strListText;
	if (!sText.IsEmpty())
	{
		sText = sText.SubString(0, 255);
		dwTxDataLen += (sText.Length() + 1);
	}
	else
		dwTxDataLen += 1;
	pucDataBuf = (BYTE *)::HeapAlloc(::GetProcessHeap(), 0, dwTxDataLen);
	if (!sText.IsEmpty())
		strcpy((char*)(pucDataBuf + sizeof(GS9025_TEXT_ACT_DEF)), sText.c_str());
	else
		strcpy((char*)(pucDataBuf + sizeof(GS9025_TEXT_ACT_DEF)), "");
	pActDef = (GS9025_TEXT_ACT_DEF*)pucDataBuf;

	SYSTEMTIME st;
	GetLocalTime(&st);

	bool bZeroStart = false;
	if (sysConfig.m_nActStartYear < st.wYear)
		bZeroStart = true;
	else if (sysConfig.m_nActStartYear == st.wYear)
	{
		if (sysConfig.m_nActStartMonth < st.wMonth)
			bZeroStart = true;
		else if (sysConfig.m_nActStartMonth == st.wMonth)
		{
			if (sysConfig.m_nActStartDay < st.wDay)
				bZeroStart = true;
			else if (sysConfig.m_nActStartDay == st.wDay)
			{
				if (sysConfig.m_nActStartHour < st.wHour)
					bZeroStart = true;
				else if (sysConfig.m_nActStartHour == st.wHour)
				{
					if (sysConfig.m_nActStartMin <= st.wMinute)
						bZeroStart = true;
				}
			}
		}
	}

	bool bForever = false;
	if (sysConfig.m_nActEndYear >= st.wYear + 10)
		bForever = true;

	pActDef->ucActIndex = m_nActIndex;
	pActDef->ucAreaIndex = sysConfig.m_nActArea;
	pActDef->ucEnterStyle = sysConfig.m_nActEnterStyle;
	pActDef->ucEnterSpeed = sysConfig.m_nActEnterSpeed;
	pActDef->ulShowTime = sysConfig.m_nActShowTime;
	pActDef->ucExitStyle = sysConfig.m_nActExitStyle;
	pActDef->ucExitSpeed = sysConfig.m_nActExitSpeed;
	pActDef->ucAlignFlags = 0;
	pActDef->ucShutterWidth = sysConfig.m_nActShutterWidth;
	pActDef->ucRes[0] = 0;
	pActDef->ucRes[1] = 0;
	pActDef->ucRepeatCount = sysConfig.m_nActRepeatCount;
	if (bZeroStart)
	{
		pActDef->ucStartYear = 0;
		pActDef->ucStartMonth = 1;
		pActDef->ucStartDay = 1;
		pActDef->ucStartHour = 0;
		pActDef->ucStartMinute = 0;
	}
	else
	{
		pActDef->ucStartYear = sysConfig.m_nActStartYear % 100;
		pActDef->ucStartMonth = sysConfig.m_nActStartMonth;
		pActDef->ucStartDay = sysConfig.m_nActStartDay;
		pActDef->ucStartHour = sysConfig.m_nActStartHour;
		pActDef->ucStartMinute = sysConfig.m_nActStartMin;
	}
	if (bForever)
	{
		pActDef->ucEndYear = 99;
		pActDef->ucEndMonth = 12;
		pActDef->ucEndDay = 31;
		pActDef->ucEndHour = 23;
		pActDef->ucEndMinute = 59;
	}
	else
	{
		pActDef->ucEndYear = sysConfig.m_nActEndYear % 100;
		pActDef->ucEndMonth = sysConfig.m_nActEndMonth;
		pActDef->ucEndDay = sysConfig.m_nActEndDay;
		pActDef->ucEndHour = sysConfig.m_nActEndHour;
		pActDef->ucEndMinute = sysConfig.m_nActEndMin;
	}
	pActDef->usStart1 = sysConfig.m_nActStartMinuteInDay[0];
	pActDef->usEnd1 = sysConfig.m_nActEndMinuteInDay[0];
	pActDef->usStart2 = sysConfig.m_nActStartMinuteInDay[1];
	pActDef->usEnd2 = sysConfig.m_nActEndMinuteInDay[1];
	pActDef->usStart3 = sysConfig.m_nActStartMinuteInDay[2];
	pActDef->usEnd3 = sysConfig.m_nActEndMinuteInDay[2];
	pActDef->usStart4 = sysConfig.m_nActStartMinuteInDay[3];
	pActDef->usEnd4 = sysConfig.m_nActEndMinuteInDay[3];
	pActDef->ucInsertFlag = false;
	if (sysConfig.m_bDotFont)
		pActDef->ucTextSizeIndex = (sysConfig.m_nDotFontSize == 16) ? 0 : 2;
	else
		pActDef->ucTextSizeIndex = (abs(sysConfig.m_nActFontSize) <= 12) ? 1 : 3;

	sysConfig.SaveActData(m_dtu, m_nActIndex, pucDataBuf, dwTxDataLen);
	::HeapFree(::GetProcessHeap(), 0, pucDataBuf);
}
//---------------------------------------------------------------------------

int TfrmMain::MakeGS9025NullTextActData(int nActIndex, BYTE* p)
{
#pragma pack(1)
typedef struct
{
	BYTE ucActIndex;		//节目编号(0-15)
	BYTE ucAreaIndex;		//分区编号(0-7)
	BYTE ucEnterStyle;		//进入方式(0-?)
	BYTE ucEnterSpeed;		//进入速度(ms)
	DWORD ulShowTime;		//停留时间(ms)
	BYTE ucExitStyle;		//退出方式(0-?)
	BYTE ucExitSpeed;		//退出速度(ms)
	BYTE ucAlignFlags;		//文字排列
	BYTE ucShutterWidth;	//叶片宽度
	BYTE ucRes[2];
	BYTE ucRepeatCount;		//重复播放次数
	BYTE ucStartYear; 		//起始播放时间_年
	BYTE ucStartMonth; 		//起始播放时间_月
	BYTE ucStartDay; 		//起始播放时间_日
	BYTE ucStartHour; 		//起始播放时间_时
	BYTE ucStartMinute; 	//起始播放时间_分
	BYTE ucEndYear; 		//终止播放时间_年
	BYTE ucEndMonth; 		//终止播放时间_月
	BYTE ucEndDay; 			//终止播放时间_日
	BYTE ucEndHour; 		//终止播放时间_时
	BYTE ucEndMinute; 		//终止播放时间_分
	WORD usStart1;			//每日播放时段开始时间 分钟计数
	WORD usEnd1;			//每日播放时段结束时间 分钟计数
	WORD usStart2;			//每日播放时段开始时间 分钟计数
	WORD usEnd2;			//每日播放时段结束时间 分钟计数
	WORD usStart3;			//每日播放时段开始时间 分钟计数
	WORD usEnd3;			//每日播放时段结束时间 分钟计数
	WORD usStart4;			//每日播放时段开始时间 分钟计数
	WORD usEnd4;			//每日播放时段结束时间 分钟计数
	BYTE ucInsertFlag;
	BYTE ucTextSizeIndex;
} GS9025_TEXT_ACT_DEF;
#pragma pack(1)

	GS9025_TEXT_ACT_DEF* pActDef = (GS9025_TEXT_ACT_DEF*)p;;
	p[sizeof(GS9025_TEXT_ACT_DEF)] = 0;

	pActDef->ucActIndex = nActIndex;
	pActDef->ucAreaIndex = 0;
	pActDef->ucEnterStyle = 0;
	pActDef->ucEnterSpeed = 16;
	pActDef->ulShowTime = 3000;
	pActDef->ucExitStyle = 0;
	pActDef->ucExitSpeed = 16;
	pActDef->ucAlignFlags = 0;
	pActDef->ucShutterWidth = 8;
	pActDef->ucRes[0] = 0;
	pActDef->ucRes[1] = 0;
	pActDef->ucRepeatCount = 1;
	pActDef->ucStartYear = 0;
	pActDef->ucStartMonth = 1;
	pActDef->ucStartDay = 1;
	pActDef->ucStartHour = 0;
	pActDef->ucStartMinute = 0;
	pActDef->ucEndYear = 99;
	pActDef->ucEndMonth = 12;
	pActDef->ucEndDay = 31;
	pActDef->ucEndHour = 23;
	pActDef->ucEndMinute = 59;
	pActDef->usStart1 = 0;
	pActDef->usEnd1 = 1439;
	pActDef->usStart2 = 0;
	pActDef->usEnd2 = 1439;
	pActDef->usStart3 = 0;
	pActDef->usEnd3 = 1439;
	pActDef->usStart4 = 0;
	pActDef->usEnd4 = 1439;
	pActDef->ucInsertFlag = false;
	pActDef->ucTextSizeIndex = 0;

	return (sizeof(GS9025_TEXT_ACT_DEF) + 1);
}
//---------------------------------------------------------------------------

void TfrmMain::MakeGS9025RamTextActData()
{
#pragma pack(1)
typedef struct
{
	BYTE ucActIndex;		//节目编号(0-15)
	BYTE ucAreaIndex;		//分区编号(0-7)
	BYTE ucEnterStyle;		//进入方式(0-?)
	BYTE ucEnterSpeed;		//进入速度(ms)
	DWORD ulShowTime;		//停留时间(ms)
	BYTE ucExitStyle;		//退出方式(0-?)
	BYTE ucExitSpeed;		//退出速度(ms)
	BYTE ucAlignFlags;		//文字排列
	BYTE ucShutterWidth;	//叶片宽度
	BYTE ucRes[2];
	BYTE ucRepeatCount;		//重复播放次数
	BYTE ucTextSizeIndex;
} GS9025_RAM_TEXT_ACT_DEF;
#pragma pack(1)

	GS9025_RAM_TEXT_ACT_DEF* pActDef;
	BYTE *pucDataBuf;
	DWORD dwTxDataLen = sizeof(GS9025_RAM_TEXT_ACT_DEF);
	TStrings *strListText = new TStringList;
	AnsiString sText;

	strListText->SetText(sysConfig.m_sActText.c_str());
	for (int nRow = 0; nRow < strListText->Count; nRow++)
	{
		if (nRow != 0)
			sText += "\r";
		sText += strListText->Strings[nRow];
	}
	delete strListText;
	if (!sText.IsEmpty())
	{
		sText = sText.SubString(0, 255);
		dwTxDataLen += (sText.Length() + 1);
	}
	else
		dwTxDataLen += 1;
	pucDataBuf = (BYTE *)::HeapAlloc(::GetProcessHeap(), 0, dwTxDataLen);
	if (!sText.IsEmpty())
		strcpy((char*)(pucDataBuf + sizeof(GS9025_RAM_TEXT_ACT_DEF)), sText.c_str());
	else
		strcpy((char*)(pucDataBuf + sizeof(GS9025_RAM_TEXT_ACT_DEF)), "");
	pActDef = (GS9025_RAM_TEXT_ACT_DEF*)pucDataBuf;

	pActDef->ucActIndex = m_nActIndex;
	pActDef->ucAreaIndex = sysConfig.m_nActArea;
	pActDef->ucEnterStyle = sysConfig.m_nActEnterStyle;
	pActDef->ucEnterSpeed = sysConfig.m_nActEnterSpeed;
	pActDef->ulShowTime = sysConfig.m_nActShowTime;
	pActDef->ucExitStyle = sysConfig.m_nActExitStyle;
	pActDef->ucExitSpeed = sysConfig.m_nActExitSpeed;
	pActDef->ucAlignFlags = 0;
	pActDef->ucShutterWidth = sysConfig.m_nActShutterWidth;
	pActDef->ucRes[0] = 0;
	pActDef->ucRes[1] = 0;
	pActDef->ucRepeatCount = sysConfig.m_nActRepeatCount;
	if (sysConfig.m_bDotFont)
		pActDef->ucTextSizeIndex = (sysConfig.m_nDotFontSize == 16) ? 0 : 2;
	else
		pActDef->ucTextSizeIndex = (abs(sysConfig.m_nActFontSize) <= 12) ? 1 : 3;

	sysConfig.SaveActData(m_dtu, m_nActIndex, pucDataBuf, dwTxDataLen);
	::HeapFree(::GetProcessHeap(), 0, pucDataBuf);
}
//---------------------------------------------------------------------------

void TfrmMain::CreateTxData()
{
	BYTE *pucDataBuf;
	BYTE* p;
	DWORD dwTxDataLen;
	int nXScreenCount, nYScreenCount;
	int xScreen, yScreen;
	int xStart, yStart;
	boolean bExit;
	int i;

	if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
	{
		//if (sysConfig.m_bDotFont)
		//{
			//if (sysConfig.m_nDotFontSize == 16 || sysConfig.m_nDotFontSize == 24)
			//{
				if (mnuRamText->Checked)
					MakeGS9025RamTextActData();
				else
					MakeGS9025TextActData();
				return;
			//}
		//}
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9023)
	{
		//if (sysConfig.m_bDotFont)
		//{
		 //	if (sysConfig.m_nDotFontSize == 16)
		   //	{
				MakeGS9025TextActData();
				return;
			//}
		//}
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_GS7020BP)
	{
		CreateActImage();

		dwTxDataLen = imgScreen->Picture->Bitmap->Width * imgScreen->Picture->Bitmap->Height;
		if (dwTxDataLen == 0)
		{
			DWORD dwEmptyAct = 0xFFFFFFFF;
			sysConfig.SaveActData(m_dtu, m_nActIndex, (BYTE*)&dwEmptyAct, sizeof(dwEmptyAct));
			return;
		}

		pucDataBuf = (BYTE*)::HeapAlloc(::GetProcessHeap(), 0, dwTxDataLen);
		p = pucDataBuf;
		for (int y = 0; y < imgScreen->Picture->Bitmap->Height; y++)
		{
			for (int x = 0; x < imgScreen->Picture->Bitmap->Width; x++)
			{
				if (imgScreen->Picture->Bitmap->Canvas->Pixels[x][y] & 0xF0F0F0)
					*p++ = 1;
				else
					*p++ = 0;
			}
		}

		sysConfig.SaveActData(m_dtu, m_nActIndex, pucDataBuf, dwTxDataLen);
		::HeapFree(::GetProcessHeap(), 0, pucDataBuf);
		return;
	}

	CreateActImage();
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BX) && sysConfig.m_bSupportCycleTimeHint)
	{
		if (m_nActIndex == sysConfig.m_nSycleTimeActIndex)
			CreateAutoTimeHintImage();
	}

	nXScreenCount = (imgScreen->Picture->Bitmap->Width + sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea] - 1) / sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea];
	nYScreenCount = (imgScreen->Picture->Bitmap->Height + sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] - 1) / sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea];
	if (nXScreenCount > MAX_EDIT_SCREEN_X_COUNT)
		nXScreenCount = MAX_EDIT_SCREEN_X_COUNT;
	if (nYScreenCount > MAX_EDIT_SCREEN_Y_COUNT)
		nYScreenCount = MAX_EDIT_SCREEN_Y_COUNT;

	dwTxDataLen = 0;
	for (yScreen = 0; yScreen < nYScreenCount; yScreen++)
	{
		for (xScreen = 0; xScreen < nXScreenCount; xScreen++)
		{
			switch (sysConfig.m_nActEnterStyle)
			{
			case ENTER_STYLE_SCROLL_LEFT:
				if ((xScreen == 0) && (yScreen == 0))
				{
					dwTxDataLen += GetFlashImageLen_ScrollLeft(sysConfig.m_nActArea);
					if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BX) && sysConfig.m_bSupportCycleTimeHint)
					{
						if (m_nActIndex == sysConfig.m_nSycleTimeActIndex)
							dwTxDataLen += ((sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] + 7) / 8) * imgActive->Picture->Bitmap->Width + sizeof(IMAGE_OBJECT_HEAD);
					}
				}
				break;
			case ENTER_STYLE_SCROLL_RIGHT:
				if ((xScreen == 0) && (yScreen == 0))
					dwTxDataLen += GetFlashImageLen_ScrollRight(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_SCROLL_UP:
				if ((xScreen == 0) && (yScreen == 0))
					dwTxDataLen += GetFlashImageLen_ScrollUp(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_SCROLL_DOWN:
				if ((xScreen == 0) && (yScreen == 0))
					dwTxDataLen += GetFlashImageLen_ScrollDown(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_MOVE_LEFT:
				dwTxDataLen += GetFlashImageLen_MoveLeft(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_MOVE_RIGHT:
				dwTxDataLen += GetFlashImageLen_MoveRight(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_MOVE_UP:
				dwTxDataLen += GetFlashImageLen_MoveUp(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_MOVE_DOWN:
				dwTxDataLen += GetFlashImageLen_MoveDown(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_LEFT_TO_RIGHT_SPREAD:
				dwTxDataLen += GetFlashImageLen_LeftToRightSpread(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_RIGHT_TO_LEFT_SPREAD:
				dwTxDataLen += GetFlashImageLen_RightToLeftSpread(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_TOP_TO_BOTTOM_SPREAD:
				dwTxDataLen += GetFlashImageLen_TopToBottomSpread(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_BOTTOM_TO_TOP_SPREAD:
				dwTxDataLen += GetFlashImageLen_BottomToTopSpread(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_H_TO_CENTER_SPREAD:
				dwTxDataLen += GetFlashImageLen_H_ToCenterSpread(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_H_FROM_CENTER_SPREAD:
				dwTxDataLen += GetFlashImageLen_H_FromCenterSpread(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_V_TO_CENTER_SPREAD:
				dwTxDataLen += GetFlashImageLen_V_ToCenterSpread(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_V_FROM_CENTER_SPREAD:
				dwTxDataLen += GetFlashImageLen_V_FromCenterSpread(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_H_SHUTTER:
				dwTxDataLen += GetFlashImageLen_H_Shutter(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_V_SHUTTER:
				dwTxDataLen += GetFlashImageLen_V_Shutter(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_FLASH:
				dwTxDataLen += GetFlashImageLen_Flash(sysConfig.m_nActArea);
				break;
			case ENTER_STYLE_DIRECT:
			default:
				dwTxDataLen += GetFlashImageLen_Direct(sysConfig.m_nActArea);
				if (IsCurrentActActive())
				{
					if (nYScreenCount == 1 && nXScreenCount == 1)
						dwTxDataLen += GetActiveFlashImageLen_Direct(sysConfig.m_nActArea);
				}
				break;
			}

			bExit = true;
			switch (sysConfig.m_nActEnterStyle)
			{
			case ENTER_STYLE_SCROLL_LEFT:
			case ENTER_STYLE_SCROLL_RIGHT:
			case ENTER_STYLE_SCROLL_UP:
			case ENTER_STYLE_SCROLL_DOWN:
				if ((xScreen != 0) || (yScreen != 0))
					bExit = false;
				break;
			}

			if (bExit)
			{
				switch (sysConfig.m_nActExitStyle)
				{
				case EXIT_STYLE_MOVE_LEFT:
					dwTxDataLen += GetFlashImageLen_Exit_MoveLeft(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_MOVE_RIGHT:
					dwTxDataLen += GetFlashImageLen_Exit_MoveRight(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_MOVE_UP:
					dwTxDataLen += GetFlashImageLen_Exit_MoveUp(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_MOVE_DOWN:
					dwTxDataLen += GetFlashImageLen_Exit_MoveDown(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_LEFT_TO_RIGHT_ERASE:
					dwTxDataLen += GetFlashImageLen_Exit_LeftToRightErase(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_RIGHT_TO_LEFT_ERASE:
					dwTxDataLen += GetFlashImageLen_Exit_RightToLeftErase(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_TOP_TO_BOTTOM_ERASE:
					dwTxDataLen += GetFlashImageLen_Exit_TopToBottomErase(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_BOTTOM_TO_TOP_ERASE:
					dwTxDataLen += GetFlashImageLen_Exit_BottomToTopErase(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_H_TO_CENTER_ERASE:
					dwTxDataLen += GetFlashImageLen_Exit_H_ToCenterErase(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_H_FROM_CENTER_ERASE:
					dwTxDataLen += GetFlashImageLen_Exit_H_FromCenterErase(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_V_TO_CENTER_ERASE:
					dwTxDataLen += GetFlashImageLen_Exit_V_ToCenterErase(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_V_FROM_CENTER_ERASE:
					dwTxDataLen += GetFlashImageLen_Exit_V_FromCenterErase(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_H_SHUTTER:
					dwTxDataLen += GetFlashImageLen_Exit_H_Shutter(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_V_SHUTTER:
					dwTxDataLen += GetFlashImageLen_Exit_V_Shutter(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_FLASH:
					dwTxDataLen += GetFlashImageLen_Exit_Direct(sysConfig.m_nActArea);
					break;
				case EXIT_STYLE_NULL:
					break;
				case EXIT_STYLE_DIRECT:
				default:
					dwTxDataLen += GetFlashImageLen_Exit_Direct(sysConfig.m_nActArea);
					break;
				}
			}
		}
	}

	if (dwTxDataLen == 0)
	{
		DWORD dwEmptyAct = 0xFFFFFFFF;
		sysConfig.SaveActData(m_dtu, m_nActIndex, (BYTE*)&dwEmptyAct, sizeof(dwEmptyAct));
		return;
	}

	switch (sysConfig.m_nCardType)
	{
	case CARD_TYPE_GS7020BX:
	case CARD_TYPE_GS7024E:
	case CARD_TYPE_GS7020BF:
	case CARD_TYPE_GS7020BF_HEB:
	case CARD_TYPE_GS7020BF_W:
	case CARD_TYPE_GS9026:
	case CARD_TYPE_YL2010AF:
	case CARD_TYPE_GS9020F_GPS:
	case CARD_TYPE_GS7020HFA:
		dwTxDataLen += sizeof(ACT_HEAD_GS7020BF) + sizeof(DWORD);	//节目头部有效标记和结束标记
		break;
	case CARD_TYPE_GS2012AS_64:
		dwTxDataLen += sizeof(ACT_HEAD_GS2012AS_64) + sizeof(DWORD);	//节目头部有效标记和结束标记
		break;
	case CARD_TYPE_GS9020S:
		dwTxDataLen += sizeof(ACT_HEAD_GS2012AS_64) + sizeof(DWORD);	//节目头部有效标记和结束标记
		break;
	case CARD_TYPE_GS7020HS:
		dwTxDataLen += sizeof(ACT_HEAD_GS2012AS_64) + sizeof(DWORD);	//节目头部有效标记和结束标记
		break;
	//case CARD_TYPE_GS7024E:
	//	dwTxDataLen += sizeof(ACT_HEAD_GS7024E) + sizeof(DWORD);	//节目头部有效标记和结束标记
	 //	if (sysConfig.m_bActSecond)
	 //		dwTxDataLen += 24 * 2;
	//	break;
	default:
		dwTxDataLen += sizeof(ACT_HEAD_STANDARD) + sizeof(DWORD);	//节目头部有效标记和结束标记
		if (sysConfig.m_bActSecond)
			dwTxDataLen += 8;
		break;
	}

	if (sysConfig.m_nCardType == CARD_TYPE_GS2012A_S)
	{
		if (sysConfig.m_nVoiceOutputAreaIndex)
		   dwTxDataLen += 512;
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64 || sysConfig.m_nCardType == CARD_TYPE_GS9020S || sysConfig.m_nCardType == CARD_TYPE_GS7020HS)
		dwTxDataLen += 512;

	pucDataBuf = (BYTE *)::HeapAlloc(::GetProcessHeap(), 0, dwTxDataLen);

	SYSTEMTIME st;
	GetLocalTime(&st);

	bool bZeroStart = false;
	if (sysConfig.m_nActStartYear < st.wYear)
		bZeroStart = true;
	else if (sysConfig.m_nActStartYear == st.wYear)
	{
		if (sysConfig.m_nActStartMonth < st.wMonth)
			bZeroStart = true;
		else if (sysConfig.m_nActStartMonth == st.wMonth)
		{
			if (sysConfig.m_nActStartDay < st.wDay)
				bZeroStart = true;
			else if (sysConfig.m_nActStartDay == st.wDay)
			{
				if (sysConfig.m_nActStartHour < st.wHour)
					bZeroStart = true;
				else if (sysConfig.m_nActStartHour == st.wHour)
				{
					if (sysConfig.m_nActStartMin <= st.wMinute)
						bZeroStart = true;
				}
			}
		}
	}

	bool bForever = false;
	if (sysConfig.m_nActEndYear >= st.wYear + 10)
		bForever = true;

	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BX) || (sysConfig.m_nCardType == CARD_TYPE_GS7020BF) || (sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS7020BF_W) || (sysConfig.m_nCardType == CARD_TYPE_GS9026) || (sysConfig.m_nCardType == CARD_TYPE_YL2010AF) || (sysConfig.m_nCardType == CARD_TYPE_GS9020F_GPS) || (sysConfig.m_nCardType == CARD_TYPE_GS7020HFA))
	{
		ACT_HEAD_GS7020BF* pActHead = (ACT_HEAD_GS7020BF*)pucDataBuf;

		pActHead->ulTag = 0x9ABCDEF0 | sysConfig.m_nActArea;
		if (bZeroStart)
		{
			pActHead->ucStartYear = 0;
			pActHead->ucStartMonth = 1;
			pActHead->ucStartDay = 1;
			pActHead->ucStartHour = 0;
			pActHead->ucStartMin = 0;
		}
		else
		{
			pActHead->ucStartYear = sysConfig.m_nActStartYear % 100;
			pActHead->ucStartMonth = sysConfig.m_nActStartMonth;
			pActHead->ucStartDay = sysConfig.m_nActStartDay;
			pActHead->ucStartHour = sysConfig.m_nActStartHour;
			pActHead->ucStartMin = sysConfig.m_nActStartMin;
		}

		if (bForever)
		{
			pActHead->ucEndYear = 99;
			pActHead->ucEndMonth = 12;
			pActHead->ucEndDay = 31;
			pActHead->ucEndHour = 23;
			pActHead->ucEndMin = 59;
		}
		else
		{
			pActHead->ucEndYear = sysConfig.m_nActEndYear % 100;
			pActHead->ucEndMonth = sysConfig.m_nActEndMonth;
			pActHead->ucEndDay = sysConfig.m_nActEndDay;
			pActHead->ucEndHour = sysConfig.m_nActEndHour;
			pActHead->ucEndMin = sysConfig.m_nActEndMin;
		}

		pActHead->ucRepeatCount = sysConfig.m_nActRepeatCount;
		pActHead->ucRefActIndex = sysConfig.m_nActRefIndex;

		for (i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
		{
			pActHead->ulStartSecondEveryDay[i] = sysConfig.m_nActStartMinuteInDay[i] * 60 + sysConfig.m_nActStartSecondInDay[i];
			pActHead->ulEndSecondEveryDay[i] = sysConfig.m_nActEndMinuteInDay[i] * 60 + sysConfig.m_nActEndSecondInDay[i];
		}
		pActHead->ulPlayTime = CalcPlayTime(m_nActIndex);
		p = (BYTE*)(pActHead + 1);
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64 || sysConfig.m_nCardType == CARD_TYPE_GS9020S || sysConfig.m_nCardType == CARD_TYPE_GS7020HS)
	{
		ACT_HEAD_GS2012AS_64* pActHead = (ACT_HEAD_GS2012AS_64*)pucDataBuf;

		pActHead->ulTag = 0x9ABCDEF0 | sysConfig.m_nActArea;
		if (bZeroStart)
		{
			pActHead->usStartYear = 0;
			pActHead->ucStartMonth = 1;
			pActHead->ucStartDay = 1;
			pActHead->ucStartHour = 0;
			pActHead->ucStartMinute = 0;
		}
		else
		{
			pActHead->usStartYear = sysConfig.m_nActStartYear;
			pActHead->ucStartMonth = sysConfig.m_nActStartMonth;
			pActHead->ucStartDay = sysConfig.m_nActStartDay;
			pActHead->ucStartHour = sysConfig.m_nActStartHour;
			pActHead->ucStartMinute = sysConfig.m_nActStartMin;
		}

		if (bForever)
		{
			pActHead->usEndYear = 2099;
			pActHead->ucEndMonth = 12;
			pActHead->ucEndDay = 31;
			pActHead->ucEndHour = 23;
			pActHead->ucEndMinute = 59;
		}
		else
		{
			pActHead->usEndYear = sysConfig.m_nActEndYear;
			pActHead->ucEndMonth = sysConfig.m_nActEndMonth;
			pActHead->ucEndDay = sysConfig.m_nActEndDay;
			pActHead->ucEndHour = sysConfig.m_nActEndHour;
			pActHead->ucEndMinute = sysConfig.m_nActEndMin;
		}

		pActHead->ucRepeatCount = sysConfig.m_nActRepeatCount;

		for (i = 0; i < ACT_TIME_PERIOD_COUNT_4; i++)
		{
			pActHead->ulStartSecondInDay[i] = sysConfig.m_nActStartMinuteInDay[i] * 60 + sysConfig.m_nActStartSecondInDay[i];
			pActHead->ulEndSecondInDay[i] = sysConfig.m_nActEndMinuteInDay[i] * 60 + sysConfig.m_nActEndSecondInDay[i];
		}
		p = (BYTE*)(pActHead + 1);
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_PD102B)
	{
		DWORD* pulTag = (DWORD*)pucDataBuf;
		*pulTag = 0x9ABCDEF0;
		p = (BYTE*)(pulTag + 1);
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_GS7024E)
	{
		ACT_HEAD_GS7024E* pActHead = (ACT_HEAD_GS7024E*)pucDataBuf;

		pActHead->ulTag = 0x9ABCDEF0 | sysConfig.m_nActArea;
		if (bZeroStart)
		{
			pActHead->ucStartYear = 0;
			pActHead->ucStartMonth = 1;
			pActHead->ucStartDay = 1;
			pActHead->ucStartHour = 0;
			pActHead->ucStartMin = 0;
		}
		else
		{
			pActHead->ucStartYear = sysConfig.m_nActStartYear % 100;
			pActHead->ucStartMonth = sysConfig.m_nActStartMonth;
			pActHead->ucStartDay = sysConfig.m_nActStartDay;
			pActHead->ucStartHour = sysConfig.m_nActStartHour;
			pActHead->ucStartMin = sysConfig.m_nActStartMin;
		}

		if (bForever)
		{
			pActHead->ucEndYear = 99;
			pActHead->ucEndMonth = 12;
			pActHead->ucEndDay = 31;
			pActHead->ucEndHour = 23;
			pActHead->ucEndMin = 59;
		}
		else
		{
			pActHead->ucEndYear = sysConfig.m_nActEndYear % 100;
			pActHead->ucEndMonth = sysConfig.m_nActEndMonth;
			pActHead->ucEndDay = sysConfig.m_nActEndDay;
			pActHead->ucEndHour = sysConfig.m_nActEndHour;
			pActHead->ucEndMin = sysConfig.m_nActEndMin;
		}
		pActHead->ucRepeatCount = sysConfig.m_nActRepeatCount;
		pActHead->ucRefActIndex = sysConfig.m_nActRefIndex;
		for (i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
		{
			pActHead->usStartMinuteEveryDay[i] = sysConfig.m_nActStartMinuteInDay[i];
			pActHead->usEndMinuteEveryDay[i] = sysConfig.m_nActEndMinuteInDay[i];
		}
		p = (BYTE*)(pActHead + 1);

		if (sysConfig.m_bActSecond)
		{
			for (i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
				*p++ = sysConfig.m_nActStartSecondInDay[i];
			for (i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
				*p++ = sysConfig.m_nActEndSecondInDay[i];
		}
	}
	else
	{
		ACT_HEAD_STANDARD* pActHead = (ACT_HEAD_STANDARD*)pucDataBuf;

		pActHead->ulTag = 0x9ABCDEF0 | sysConfig.m_nActArea;
		if (bZeroStart)
		{
			pActHead->ucStartYear = 0;
			pActHead->ucStartMonth = 1;
			pActHead->ucStartDay = 1;
			pActHead->ucStartHour = 0;
			pActHead->ucStartMin = 0;
		}
		else
		{
			pActHead->ucStartYear = sysConfig.m_nActStartYear % 100;
			pActHead->ucStartMonth = sysConfig.m_nActStartMonth;
			pActHead->ucStartDay = sysConfig.m_nActStartDay;
			pActHead->ucStartHour = sysConfig.m_nActStartHour;
			pActHead->ucStartMin = sysConfig.m_nActStartMin;
		}

		if (bForever)
		{
			pActHead->ucEndYear = 99;
			pActHead->ucEndMonth = 12;
			pActHead->ucEndDay = 31;
			pActHead->ucEndHour = 23;
			pActHead->ucEndMin = 59;
		}
		else
		{
			pActHead->ucEndYear = sysConfig.m_nActEndYear % 100;
			pActHead->ucEndMonth = sysConfig.m_nActEndMonth;
			pActHead->ucEndDay = sysConfig.m_nActEndDay;
			pActHead->ucEndHour = sysConfig.m_nActEndHour;
			pActHead->ucEndMin = sysConfig.m_nActEndMin;
		}
		pActHead->ucRepeatCount = sysConfig.m_nActRepeatCount;
		pActHead->ucRefActIndex = sysConfig.m_nActRefIndex;
		for (i = 0; i < ACT_TIME_PERIOD_COUNT_4; i++)
		{
			pActHead->usStartMinuteEveryDay[i] = sysConfig.m_nActStartMinuteInDay[i];
			pActHead->usEndMinuteEveryDay[i] = sysConfig.m_nActEndMinuteInDay[i];
		}
		p = (BYTE*)(pActHead + 1);

		if (sysConfig.m_bActSecond)
		{
			for (i = 0; i < ACT_TIME_PERIOD_COUNT_4; i++)
				*p++ = sysConfig.m_nActStartSecondInDay[i];
			for (i = 0; i < ACT_TIME_PERIOD_COUNT_4; i++)
				*p++ = sysConfig.m_nActEndSecondInDay[i];
		}
	}

	if (sysConfig.m_nCardType == CARD_TYPE_GS2012A_S)
	{
		if (sysConfig.m_nVoiceOutputAreaIndex)
		{
			wchar_t ws[256];
			wcsncpy(ws, sysConfig.m_sActDesc.c_str(), 255);
			memcpy(p, ws, 512);
			p += 512;
		}
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64 || sysConfig.m_nCardType == CARD_TYPE_GS9020S || sysConfig.m_nCardType == CARD_TYPE_GS7020HS)
	{
		wchar_t ws[256];
		wcsncpy(ws, sysConfig.m_sActDesc.c_str(), 255);
		memcpy(p, ws, 512);
		p += 512;
	}

	yStart = 0;
	for (yScreen = 0; yScreen < nYScreenCount; yScreen++)
	{
		xStart = 0;
		for (xScreen = 0; xScreen < nXScreenCount; xScreen++)
		{
			switch (sysConfig.m_nActEnterStyle)
			{
			case ENTER_STYLE_SCROLL_LEFT:
				if ((xScreen == 0) && (yScreen == 0))
				{
					if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BX) && sysConfig.m_bSupportCycleTimeHint)
					{
						if (m_nActIndex == sysConfig.m_nSycleTimeActIndex)
							p = CreateAutoHintFlashImage(p);
					}
					p = CreateFlashImage_ScrollLeft(m_nActIndex, sysConfig.m_nActArea, p);
				}
				break;
			case ENTER_STYLE_SCROLL_RIGHT:
				if ((xScreen == 0) && (yScreen == 0))
					p = CreateFlashImage_ScrollRight(m_nActIndex, sysConfig.m_nActArea, p);
				break;
			case ENTER_STYLE_SCROLL_UP:
				if ((xScreen == 0) && (yScreen == 0))
					p = CreateFlashImage_ScrollUp(m_nActIndex, sysConfig.m_nActArea, p);
				break;
			case ENTER_STYLE_SCROLL_DOWN:
				if ((xScreen == 0) && (yScreen == 0))
					p = CreateFlashImage_ScrollDown(m_nActIndex, sysConfig.m_nActArea, p);
				break;
			case ENTER_STYLE_MOVE_LEFT:
				p = CreateFlashImage_MoveLeft(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_MOVE_RIGHT:
				p = CreateFlashImage_MoveRight(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_MOVE_UP:
				p = CreateFlashImage_MoveUp(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_MOVE_DOWN:
				p = CreateFlashImage_MoveDown(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_LEFT_TO_RIGHT_SPREAD:
				p = CreateFlashImage_LeftToRightSpread(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_RIGHT_TO_LEFT_SPREAD:
				p = CreateFlashImage_RightToLeftSpread(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_TOP_TO_BOTTOM_SPREAD:
				p = CreateFlashImage_TopToBottomSpread(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_BOTTOM_TO_TOP_SPREAD:
				p = CreateFlashImage_BottomToTopSpread(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_H_TO_CENTER_SPREAD:
				p = CreateFlashImage_H_ToCenterSpread(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_H_FROM_CENTER_SPREAD:
				p = CreateFlashImage_H_FromCenterSpread(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_V_TO_CENTER_SPREAD:
				p = CreateFlashImage_V_ToCenterSpread(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_V_FROM_CENTER_SPREAD:
				p = CreateFlashImage_V_FromCenterSpread(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_H_SHUTTER:
				p = CreateFlashImage_H_Shutter(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_V_SHUTTER:
				p = CreateFlashImage_V_Shutter(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_FLASH:
				p = CreateFlashImage_Flash(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			case ENTER_STYLE_DIRECT:
			default:
				p = CreateFlashImage_Direct(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				if (nYScreenCount == 1 && nXScreenCount == 1 && IsCurrentActActive())
					p = CreateActiveFlashImage_Direct(m_nActIndex, sysConfig.m_nActArea, xStart, yStart, p);
				break;
			}

			bExit = true;
			switch (sysConfig.m_nActEnterStyle)
			{
			case ENTER_STYLE_SCROLL_LEFT:
			case ENTER_STYLE_SCROLL_RIGHT:
			case ENTER_STYLE_SCROLL_UP:
			case ENTER_STYLE_SCROLL_DOWN:
				if ((xScreen != 0) || (yScreen != 0))
					bExit = false;
				break;
			}

			if (bExit)
			{
				switch (sysConfig.m_nActExitStyle)
				{
				case EXIT_STYLE_MOVE_LEFT:
					p = CreateFlashImage_Exit_MoveLeft(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_MOVE_RIGHT:
					p = CreateFlashImage_Exit_MoveRight(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_MOVE_UP:
					p = CreateFlashImage_Exit_MoveUp(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_MOVE_DOWN:
					p = CreateFlashImage_Exit_MoveDown(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_LEFT_TO_RIGHT_ERASE:
					p = CreateFlashImage_Exit_LeftToRightErase(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_RIGHT_TO_LEFT_ERASE:
					p = CreateFlashImage_Exit_RightToLeftErase(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_TOP_TO_BOTTOM_ERASE:
					p = CreateFlashImage_Exit_TopToBottomErase(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_BOTTOM_TO_TOP_ERASE:
					p = CreateFlashImage_Exit_BottomToTopErase(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_H_TO_CENTER_ERASE:
					p = CreateFlashImage_Exit_H_ToCenterErase(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_H_FROM_CENTER_ERASE:
					p = CreateFlashImage_Exit_H_FromCenterErase(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_V_TO_CENTER_ERASE:
					p = CreateFlashImage_Exit_V_ToCenterErase(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_V_FROM_CENTER_ERASE:
					p = CreateFlashImage_Exit_V_FromCenterErase(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_H_SHUTTER:
					p = CreateFlashImage_Exit_H_ShutterErase(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_V_SHUTTER:
					p = CreateFlashImage_Exit_V_ShutterErase(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				case EXIT_STYLE_FLASH:
					//dwTxDataLen += GetFlashImageLen_Exit_Direct();
					break;
				case EXIT_STYLE_NULL:
					break;
				case EXIT_STYLE_DIRECT:
				default:
					p = CreateFlashImage_Exit_Direct(m_nActIndex, sysConfig.m_nActArea, p);
					break;
				}
			}

			xStart += sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea];
		}

		yStart += sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea];
	}

	DWORD* pTag = (DWORD*)p;
	*pTag = 0;

	sysConfig.SaveActData(m_dtu, m_nActIndex, pucDataBuf, dwTxDataLen);
	::HeapFree(::GetProcessHeap(), 0, pucDataBuf);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerTxActTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_WriteFlash(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (dtu->m_nTxStep == 0)
				{
					if (sysConfig.m_nCardType == CARD_TYPE_GS9025 || sysConfig.m_nCardType == CARD_TYPE_GS9023)
					{
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 1;
						m_ComPort.Close();
					}
					else
					{
						BYTE ucCmd;
						switch (sysConfig.m_nCardType)
						{
						case CARD_TYPE_GS7020BF_W: ucCmd = MMI_CMD_STOP_ACT_PROC; break;
						default: ucCmd = MMI_CMD_STOP_DISPLAY; break;
						}
						if (WaitForCmdEcho(ucCmd, dtu, 0))
						{
							dtu->m_nRetries = 0;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus= "已暂停显示,准备发送节目" + UnicodeString(dtu->m_nTxAct + 1);
							else
								dtu->m_sStatus= "Act Playing Stopped, Preparing Sending Act " + UnicodeString(dtu->m_nTxAct + 1);
							vt->InvalidateNode(Node);
							dtu->m_nTxStatus = TX_STATUS_READY;
							dtu->m_nTxStep = 1;
							m_ComPort.Close();
						}
					}
				}
				else if (dtu->m_nTxStep == 1)
				{
					BYTE ucCmd;
					switch (sysConfig.m_nCardType)
					{
					case CARD_TYPE_GS7020F: ucCmd = MMI_CMD_WRITE_FLASH_F; break;
					case CARD_TYPE_GS9025: ucCmd = (mnuRamText->Checked) ? MMI_CMD_RAM_TEXT_ACT : MMI_CMD_TEXT_ACT; break;
					case CARD_TYPE_GS9023: ucCmd = MMI_CMD_TEXT_ACT; break;
					default: ucCmd = MMI_CMD_WRITE_FLASH; break;
					}
					if (WaitForCmdEcho(ucCmd, dtu, 0))
					{
						m_ComPort.Close();
						dtu->m_nRetries = 0;
						if (dtu->m_bWantVerifyAct)
						{
							dtu->m_nTxStatus = TX_STATUS_READY;
							dtu->m_nTxStep = 2;
						}
						else
						{
							if (dtu->m_nDataSent + sysConfig.m_nMaxPackSize >= sysConfig.GetActDataSize(dtu, dtu->m_nTxAct))
							{
								if (sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) > sysConfig.m_nMaxPackSize)
								{
									dtu->m_nTxStatus = TX_STATUS_READY;
									dtu->m_nTxStep = 4;
								}
								else
								{
									dtu->m_bActTxOk[dtu->m_nTxAct] = TRUE;

									bool bFound = false;
									TListItem* itemAct;
									while ((dtu->m_nTxAct < sysConfig.m_nMaxActCount - 1) && (dtu->m_nTxAct < sysConfig.m_nMaxActCount - 1) && (dtu->m_nTxAct < listActs->Items->Count - 1))
									{
										dtu->m_nTxAct++;
										if (m_bReTx)
										{
											bFound = (!dtu->m_bActTxOk[dtu->m_nTxAct]);
										}
										else
										{
											itemAct = listActs->Items->Item[dtu->m_nTxAct];
											bFound = itemAct->Checked;
										}
										if (bFound)
										{
											dtu->m_nDataSent = 0;
											dtu->m_nTxStatus = TX_STATUS_READY;
											if (m_nLanguage == LANGUAGE_CHINESE)
												dtu->m_sStatus = "开始发送节目" + UnicodeString(dtu->m_nTxAct + 1);
											else
												dtu->m_sStatus = "Sending Act " + UnicodeString(dtu->m_nTxAct + 1);
											vt->InvalidateNode(Node);
											break;
										}
									}

									if (!bFound)
									{
										if (sysConfig.m_nCardType == CARD_TYPE_GS9025 || sysConfig.m_nCardType == CARD_TYPE_GS9023)
										{
										}
										else
										{
											if (m_nLanguage == LANGUAGE_CHINESE)
												dtu->m_sStatus = "准备重新开始节目显示";
											else
												dtu->m_sStatus = "Restarting Playing Acts";
											vt->InvalidateNode(Node);
										}
										dtu->m_nTxStatus = TX_STATUS_READY;
										dtu->m_nTxStep = 3;
									}
								}
							}
							else
							{
								dtu->m_nTxStatus = TX_STATUS_READY;
								dtu->m_nDataSent += sysConfig.m_nMaxPackSize;
								int nPercent = dtu->m_nDataSent * 100 / (sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) + sysConfig.m_nMaxPackSize);
								if (m_nLanguage == LANGUAGE_CHINESE)
									dtu->m_sStatus = "节目" + UnicodeString(dtu->m_nTxAct + 1) + ":" + UnicodeString(nPercent) + "%";
								else
									dtu->m_sStatus = UnicodeString(nPercent) + "% of act " + UnicodeString(dtu->m_nTxAct + 1) + " has been sent";
								vt->InvalidateNode(Node);
							}
						}
					}
				}
				else if (dtu->m_nTxStep == 2)
				{
					int nLeftDataLen = sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) - dtu->m_nDataSent;
					int nDataLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize : nLeftDataLen;
					if (WaitForCmdEcho(MMI_CMD_READ_FLASH, dtu, nDataLen + 6))
					{
						m_ComPort.Close();
						dtu->m_nRetries = 0;
						if (VerifyAct(dtu))
						{
							if (dtu->m_nDataSent + sysConfig.m_nMaxPackSize >= sysConfig.GetActDataSize(dtu, dtu->m_nTxAct))
							{
								if (sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) > sysConfig.m_nMaxPackSize)
								{
									dtu->m_nTxStatus = TX_STATUS_READY;
									dtu->m_nTxStep = 4;
								}
								else
								{
									dtu->m_bActTxOk[dtu->m_nTxAct] = TRUE;

									bool bFound = false;
									TListItem* itemAct;
									while ((dtu->m_nTxAct < sysConfig.m_nMaxActCount - 1) && (dtu->m_nTxAct < sysConfig.m_nMaxActCount - 1) && (dtu->m_nTxAct < listActs->Items->Count - 1))
									{
										dtu->m_nTxAct++;
										if (m_bReTx)
										{
											bFound = (!dtu->m_bActTxOk[dtu->m_nTxAct]);
										}
										else
										{
											itemAct = listActs->Items->Item[dtu->m_nTxAct];
											bFound = itemAct->Checked;
										}
										if (bFound)
										{
											dtu->m_nDataSent = 0;
											dtu->m_nTxStatus = TX_STATUS_READY;
											if (m_nLanguage == LANGUAGE_CHINESE)
												dtu->m_sStatus = "开始发送节目" + UnicodeString(dtu->m_nTxAct + 1);
											else
												dtu->m_sStatus = "Sending Act " + UnicodeString(dtu->m_nTxAct + 1);
											vt->InvalidateNode(Node);
											break;
										}
									}

									if (!bFound)
									{
										if (m_nLanguage == LANGUAGE_CHINESE)
											dtu->m_sStatus = "准备重新开始节目显示";
										else
											dtu->m_sStatus = "Restarting Playing Acts";
										vt->InvalidateNode(Node);
										dtu->m_nTxStatus = TX_STATUS_READY;
										dtu->m_nTxStep = 3;
									}
								}
							}
							else
							{
								dtu->m_nTxStep = 1;
								dtu->m_nTxStatus = TX_STATUS_READY;
								dtu->m_nDataSent += sysConfig.m_nMaxPackSize;
								int nPercent = dtu->m_nDataSent * 100 / (sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) + sysConfig.m_nMaxPackSize);
								if (m_nLanguage == LANGUAGE_CHINESE)
									dtu->m_sStatus = "节目" + UnicodeString(dtu->m_nTxAct + 1) + ":" + UnicodeString(nPercent) + "%";
								else
									dtu->m_sStatus = UnicodeString(nPercent) + "% of act " + UnicodeString(dtu->m_nTxAct + 1) + " has been sent";
								vt->InvalidateNode(Node);
							}
						}
						else
						{
							dtu->m_nDataSent &= 0xFFFFF000;		//4096
							dtu->m_nTxStatus = TX_STATUS_READY;
							dtu->m_nTxStep = 1;
						}
					}
				}
				else if (dtu->m_nTxStep == 4)
				{
					if (WaitForCmdEcho((sysConfig.m_nCardType != CARD_TYPE_GS7020F) ? MMI_CMD_WRITE_FLASH : MMI_CMD_WRITE_FLASH_F, dtu, 0))
					{
						m_ComPort.Close();
						dtu->m_nRetries = 0;
						dtu->m_bActTxOk[dtu->m_nTxAct] = TRUE;
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 1;

						bool bFound = false;
						TListItem* itemAct;
						while ((dtu->m_nTxAct < sysConfig.m_nMaxActCount - 1) && (dtu->m_nTxAct < sysConfig.m_nMaxActCount - 1) && (dtu->m_nTxAct < listActs->Items->Count - 1))
						{
							dtu->m_nTxAct++;
							if (m_bReTx)
							{
								bFound = (!dtu->m_bActTxOk[dtu->m_nTxAct]);
							}
							else
							{
								itemAct = listActs->Items->Item[dtu->m_nTxAct];
								bFound = itemAct->Checked;
							}
							if (bFound)
							{
								dtu->m_nDataSent = 0;
								dtu->m_nTxStatus = TX_STATUS_READY;
								if (m_nLanguage == LANGUAGE_CHINESE)
									dtu->m_sStatus = "开始发送节目" + UnicodeString(dtu->m_nTxAct + 1);
								else
									dtu->m_sStatus = "Sending Act " + UnicodeString(dtu->m_nTxAct + 1);
								vt->InvalidateNode(Node);
								break;
							}
						}

						if (!bFound)
						{
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "准备重新开始节目显示";
							else
								dtu->m_sStatus = "Restarting Playing Acts";
							vt->InvalidateNode(Node);
							dtu->m_nTxStatus = TX_STATUS_READY;
							dtu->m_nTxStep = 3;
						}
					}
				}
				else if (dtu->m_nTxStep == 3)
				{
					if (sysConfig.m_nCardType == CARD_TYPE_GS9025 || sysConfig.m_nCardType == CARD_TYPE_GS9023)
					{
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
						log.LogTx(dtu, true);
						m_ComPort.Close();
					}
					else if (WaitForCmdEcho(MMI_CMD_RESTART_DISPLAY, dtu, 0))
					{
						dtu->m_nRetries = 0;
#ifdef SUPPORT_PLAYTIME_TX				//深圳苓贯 YL2010AF 节目发送结束后发送总播放时间
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 5;
						dtu->m_sStatus = "发送播放时间";
						vt->InvalidateNode(Node);
#else
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
						log.LogTx(dtu, true);
						m_ComPort.Close();
#endif
					}
				}
				else //if (dtu->m_nTxStep == 5)
				{
					if (WaitForCmdEcho(MMI_CMD_PLAYTIME, dtu, 0))
					{
						dtu->m_nRetries = 0;
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
						log.LogTx(dtu, true);
						m_ComPort.Close();
					}
				}

				if (dtu->m_nTxStatus == TX_STATUS_WAIT_ECHO)
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							log.LogTx(dtu, false);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_WriteFlash(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else	//GPRS
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_WriteFlash(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (dtu->m_nTxStep == 0)
				{
					if (sysConfig.m_nCardType == CARD_TYPE_GS9025 || sysConfig.m_nCardType == CARD_TYPE_GS9023)
					{
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 1;
						m_ComPort.Close();
					}
					else
					{
						if (WaitForCmdEcho((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_W) ? MMI_CMD_STOP_ACT_PROC : MMI_CMD_STOP_DISPLAY, dtu, 0))
						{
							dtu->m_nRetries = 0;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus= "已暂停显示,准备发送节目" + UnicodeString(dtu->m_nTxAct + 1);
							else
								dtu->m_sStatus= "Act Playing Stopped, Preparing Sending Act " + UnicodeString(dtu->m_nTxAct + 1);
							vt->InvalidateNode(Node);
							dtu->m_nTxStatus = TX_STATUS_READY;
							dtu->m_nTxStep = 1;
						}
					}
				}
				else if (dtu->m_nTxStep == 1)
				{
					BYTE ucCmd;
					switch (sysConfig.m_nCardType)
					{
					case CARD_TYPE_GS7020F: ucCmd = MMI_CMD_WRITE_FLASH_F; break;
					case CARD_TYPE_GS9025: ucCmd = (mnuRamText->Checked) ? MMI_CMD_RAM_TEXT_ACT : MMI_CMD_TEXT_ACT; break;
					case CARD_TYPE_GS9023: MMI_CMD_TEXT_ACT; break;
					default: ucCmd = MMI_CMD_WRITE_FLASH; break;
					}
					if (WaitForCmdEcho(ucCmd, dtu, 0))
					{
						dtu->m_nRetries = 0;
						if (dtu->m_bWantVerifyAct)
						{
							dtu->m_nTxStatus = TX_STATUS_READY;
							dtu->m_nTxStep = 2;
						}
						else
						{
							if (dtu->m_nDataSent + sysConfig.m_nMaxPackSize >= sysConfig.GetActDataSize(dtu, dtu->m_nTxAct))
							{
								if (sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) > sysConfig.m_nMaxPackSize)
								{
									dtu->m_nTxStatus = TX_STATUS_READY;
									dtu->m_nTxStep = 4;
								}
								else
								{
									dtu->m_bActTxOk[dtu->m_nTxAct] = TRUE;

									bool bFound = false;
									TListItem* itemAct;
									while ((dtu->m_nTxAct < sysConfig.m_nMaxActCount - 1) && (dtu->m_nTxAct < sysConfig.m_nMaxActCount - 1) && (dtu->m_nTxAct < listActs->Items->Count - 1))
									{
										dtu->m_nTxAct++;
										if (m_bReTx)
										{
											bFound = (!dtu->m_bActTxOk[dtu->m_nTxAct]);
										}
										else
										{
											itemAct = listActs->Items->Item[dtu->m_nTxAct];
											bFound = itemAct->Checked;
										}
										if (bFound)
										{
											dtu->m_nDataSent = 0;
											dtu->m_nTxStatus = TX_STATUS_READY;
											if (m_nLanguage == LANGUAGE_CHINESE)
												dtu->m_sStatus = "开始发送节目" + UnicodeString(dtu->m_nTxAct + 1);
											else
												dtu->m_sStatus = "Sending Act " + UnicodeString(dtu->m_nTxAct + 1);
											vt->InvalidateNode(Node);
											break;
										}
									}

									if (!bFound)
									{
										if (m_nLanguage == LANGUAGE_CHINESE)
											dtu->m_sStatus = "准备重新开始节目显示";
										else
											dtu->m_sStatus = "Restarting Playing Acts";
										vt->InvalidateNode(Node);
										dtu->m_nTxStatus = TX_STATUS_READY;
										dtu->m_nTxStep = 3;
									}
								}
							}
							else
							{
								dtu->m_nTxStatus = TX_STATUS_READY;
								dtu->m_nDataSent += sysConfig.m_nMaxPackSize;
								int nPercent = dtu->m_nDataSent * 100 / (sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) + sysConfig.m_nMaxPackSize);
								if (m_nLanguage == LANGUAGE_CHINESE)
									dtu->m_sStatus = "节目" + UnicodeString(dtu->m_nTxAct + 1) + ":" + UnicodeString(nPercent) + "%";
								else
									dtu->m_sStatus = UnicodeString(nPercent) + "% of act " + UnicodeString(dtu->m_nTxAct + 1) + " has been sent";
								vt->InvalidateNode(Node);
							}
						}
					}
				}
				else if (dtu->m_nTxStep == 2)
				{
					int nLeftDataLen = sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) - dtu->m_nDataSent;
					int nDataLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize : nLeftDataLen;
					if (WaitForCmdEcho(MMI_CMD_READ_FLASH, dtu, nDataLen + 6))
					{
						dtu->m_nRetries = 0;
						if (VerifyAct(dtu))
						{
							if (dtu->m_nDataSent + sysConfig.m_nMaxPackSize >= sysConfig.GetActDataSize(dtu, dtu->m_nTxAct))
							{
								if (sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) > sysConfig.m_nMaxPackSize)
								{
									dtu->m_nTxStatus = TX_STATUS_READY;
									dtu->m_nTxStep = 4;
								}
								else
								{
									dtu->m_bActTxOk[dtu->m_nTxAct] = TRUE;

									bool bFound = false;
									TListItem* itemAct;
									while ((dtu->m_nTxAct < sysConfig.m_nMaxActCount - 1) && (dtu->m_nTxAct < sysConfig.m_nMaxActCount - 1) && (dtu->m_nTxAct < listActs->Items->Count - 1))
									{
										dtu->m_nTxAct++;
										if (m_bReTx)
										{
											bFound = (!dtu->m_bActTxOk[dtu->m_nTxAct]);
										}
										else
										{
											itemAct = listActs->Items->Item[dtu->m_nTxAct];
											bFound = itemAct->Checked;
										}
										if (bFound)
										{
											dtu->m_nDataSent = 0;
											dtu->m_nTxStatus = TX_STATUS_READY;
											if (m_nLanguage == LANGUAGE_CHINESE)
												dtu->m_sStatus = "开始发送节目" + UnicodeString(dtu->m_nTxAct + 1);
											else
												dtu->m_sStatus = "Sending Act " + UnicodeString(dtu->m_nTxAct + 1);
											vt->InvalidateNode(Node);
											break;
										}
									}

									if (!bFound)
									{
										if (m_nLanguage == LANGUAGE_CHINESE)
											dtu->m_sStatus = "准备重新开始节目显示";
										else
											dtu->m_sStatus = "Restarting Playing Acts";
										vt->InvalidateNode(Node);
										dtu->m_nTxStatus = TX_STATUS_READY;
										dtu->m_nTxStep = 3;
									}
								}
							}
							else
							{
								dtu->m_nTxStep = 1;
								dtu->m_nTxStatus = TX_STATUS_READY;
								dtu->m_nDataSent += sysConfig.m_nMaxPackSize;
								int nPercent = dtu->m_nDataSent * 100 / (sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) + sysConfig.m_nMaxPackSize);
								if (m_nLanguage == LANGUAGE_CHINESE)
									dtu->m_sStatus = "节目" + UnicodeString(dtu->m_nTxAct + 1) + ":" + UnicodeString(nPercent) + "%";
								else
									dtu->m_sStatus = UnicodeString(nPercent) + "% of act " + UnicodeString(dtu->m_nTxAct + 1) + " has been sent";
								vt->InvalidateNode(Node);
							}
						}
						else
						{
							dtu->m_nDataSent &= 0xFFFFF000;		//4096
							dtu->m_nTxStatus = TX_STATUS_READY;
							dtu->m_nTxStep = 1;
						}
					}
				}
				else if (dtu->m_nTxStep == 4)
				{
					if (WaitForCmdEcho((sysConfig.m_nCardType != CARD_TYPE_GS7020F) ? MMI_CMD_WRITE_FLASH : MMI_CMD_WRITE_FLASH_F, dtu, 0))
					{
						dtu->m_nRetries = 0;
						dtu->m_bActTxOk[dtu->m_nTxAct] = TRUE;
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 1;

						bool bFound = false;
						TListItem* itemAct;
						while ((dtu->m_nTxAct < sysConfig.m_nMaxActCount - 1) && (dtu->m_nTxAct < sysConfig.m_nMaxActCount - 1) && (dtu->m_nTxAct < listActs->Items->Count - 1))
						{
							dtu->m_nTxAct++;
							if (m_bReTx)
							{
								bFound = (!dtu->m_bActTxOk[dtu->m_nTxAct]);
							}
							else
							{
								itemAct = listActs->Items->Item[dtu->m_nTxAct];
								bFound = itemAct->Checked;
							}
							if (bFound)
							{
								dtu->m_nDataSent = 0;
								dtu->m_nTxStatus = TX_STATUS_READY;
								if (m_nLanguage == LANGUAGE_CHINESE)
									dtu->m_sStatus = "开始发送节目" + UnicodeString(dtu->m_nTxAct + 1);
								else
									dtu->m_sStatus = "Sending Act " + UnicodeString(dtu->m_nTxAct + 1);
								vt->InvalidateNode(Node);
								break;
							}
						}

						if (!bFound)
						{
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "准备重新开始节目显示";
							else
								dtu->m_sStatus = "Restarting Playing Acts";
							vt->InvalidateNode(Node);
							dtu->m_nTxStatus = TX_STATUS_READY;
							dtu->m_nTxStep = 3;
						}
					}
				}
				else if (dtu->m_nTxStep == 3)
				{
					if (sysConfig.m_nCardType == CARD_TYPE_GS9025 || sysConfig.m_nCardType == CARD_TYPE_GS9023)
					{
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
						log.LogTx(dtu, true);
					}
					else if (WaitForCmdEcho(MMI_CMD_RESTART_DISPLAY, dtu, 0))
					{
						dtu->m_nRetries = 0;
#ifdef SUPPORT_PLAYTIME_TX				//深圳苓贯 YL2010AF 节目发送结束后发送总播放时间
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 5;
						dtu->m_sStatus = "发送播放时间";
						vt->InvalidateNode(Node);
#else
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
						log.LogTx(dtu, true);
#endif
					}
				}
				else //if (dtu->m_nTxStep == 5)
				{
					if (WaitForCmdEcho(MMI_CMD_PLAYTIME, dtu, 0))
					{
						dtu->m_nRetries = 0;
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
						log.LogTx(dtu, true);
					}
				}

				if (dtu->m_nTxStatus == TX_STATUS_WAIT_ECHO)
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							log.LogTx(dtu, false);
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_WriteFlash(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
							nSendDtuCount++;
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();
		timerTxAct->Enabled = false;
		log.EndLogTx();
		SaveCardTxStatus();
		EndOperation();
		mnuManualReTxAct->Enabled = CheckIfAnyActTxFailed();
		m_nWaitReTxTimerCount = 0;
		timer_AutoWaitReTxAct->Enabled = (mnuManualReTxAct->Enabled) && (sysConfig.m_nAutoReTxActInterval != 0);
	}
}
//---------------------------------------------------------------------------

bool TfrmMain::WaitForCmdEcho(BYTE ucCmd, CDtu* dtu, int nDataLen)
{
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		return WaitForCmdEcho_HEB(ucCmd, dtu, nDataLen);

	if (sysConfig.m_nCommMethod == 1)
	{
		if (dtu->m_nRxLen >= (int)(sizeof(MMI_CMD_HEAD) + nDataLen + 1))
		{
			dtu->m_nRxLen = 0;
			MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)dtu->m_ucRxBuf;

			if (ucCmd == MMI_CMD_WRITE_FLASH || ucCmd == MMI_CMD_WRITE_FLASH_F)
			{
				if ((pHead->usHeadTag == MMI_START_ID) &&
					(pHead->ucCardId == dtu->GetCardId()) &&
					(pHead->ucCmd == ucCmd))
				{
#ifndef SUPPORT_PLAYTIME_TX
					if ((pHead->usLen == 0) &&
						(CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, dtu->m_ucRxBuf) == dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD) + pHead->usLen]))
					{
						dtu->m_bWantVerifyAct = true;
						return true;
					}
					else
#endif
					if (pHead->usLen == 4)
					{
						DWORD* pdw = (DWORD*)&dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD)];
						if (timer_UpdateProg->Enabled)
						{
							DWORD dwWant;
#ifndef GS7025H
							if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
								dwWant = (DWORD)(256 * 1024 + dtu->m_nDataSent);
							else
#endif
								dwWant = (DWORD)(1024 * 1024 + dtu->m_nDataSent);
							if ((*pdw == dwWant) &&
								(CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, dtu->m_ucRxBuf) == dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD) + pHead->usLen]))
							{
								dtu->m_bWantVerifyAct = false;
								return true;
							}
						}
						else
						{
							if ((*pdw == (DWORD)(dtu->m_nTxAct * sysConfig.GetOneActVolMaxLen() + dtu->m_nDataSent)) &&
								(CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, dtu->m_ucRxBuf) == dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD) + pHead->usLen]))
							{
								dtu->m_bWantVerifyAct = false;
								return true;
							}
						}
					}
				}
			}
			else
			{
				if (pHead->usHeadTag != MMI_START_ID)
					return false;
				if (pHead->ucCardId != dtu->GetCardId())
					return false;
				if (pHead->ucCmd != ucCmd)
					return false;
				if (pHead->usLen != nDataLen)
					return false;
				if (CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, dtu->m_ucRxBuf) != dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD) + pHead->usLen])
					return false;
				return true;
			}
		}

		return false;
	}

	while (m_ComPort.GetInQueue())
	{
		switch (dtu->m_nComPortRxStep)
		{
		case 0:
			m_ComPort.Read(1, dtu->m_ucRxBuf);
			if (dtu->m_ucRxBuf[0] == MMI_START_ID1)
				dtu->m_nComPortRxStep = 1;
			break;
		case 1:
			m_ComPort.Read(1, dtu->m_ucRxBuf + 1);
			if (dtu->m_ucRxBuf[1] == MMI_START_ID2)
			{
				dtu->m_nComPortRxStep = 2;
				dtu->m_nRxLen = 2;
			}
			else
				dtu->m_nComPortRxStep = 0;
			break;
		default:
			while (m_ComPort.GetInQueue() && dtu->m_nRxLen < (int)(sizeof(MMI_CMD_HEAD) + nDataLen + 5))
			{
				m_ComPort.Read(1, dtu->m_ucRxBuf + dtu->m_nRxLen);
				dtu->m_nRxLen++;
			}
			MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)dtu->m_ucRxBuf;

#ifndef SUPPORT_PLAYTIME_TX
			if (dtu->m_nRxLen >= (int)(sizeof(MMI_CMD_HEAD) + nDataLen + 1))
			{

				if ((pHead->usHeadTag == MMI_START_ID) &&
					 (pHead->ucCardId == dtu->GetCardId()) &&
					 (pHead->ucCmd == ucCmd) &&
					 (pHead->usLen == nDataLen) &&
					 (CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, dtu->m_ucRxBuf) == dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD) + pHead->usLen]))
				{
					dtu->m_nComPortRxStep = 0;
					if ((ucCmd == MMI_CMD_WRITE_FLASH || ucCmd == MMI_CMD_WRITE_FLASH_F) && (sysConfig.m_nCardType != CARD_TYPE_PD102B))
						dtu->m_bWantVerifyAct = true;
					else
						dtu->m_bWantVerifyAct = false;
					return true;
				}

				if (ucCmd != MMI_CMD_WRITE_FLASH && ucCmd != MMI_CMD_WRITE_FLASH_F)
					dtu->m_nComPortRxStep = 0;
			}
#else
			if (ucCmd != MMI_CMD_WRITE_FLASH && ucCmd != MMI_CMD_WRITE_FLASH_F)
			{
				if (dtu->m_nRxLen >= (int)(sizeof(MMI_CMD_HEAD) + nDataLen + 1))
				{
					if ((pHead->usHeadTag == MMI_START_ID) &&
						 (pHead->ucCardId == dtu->GetCardId()) &&
						 (pHead->ucCmd == ucCmd) &&
						 (pHead->usLen == nDataLen) &&
						 (CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, dtu->m_ucRxBuf) == dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD) + pHead->usLen]))
					{
						dtu->m_nComPortRxStep = 0;
						return true;
					}
					dtu->m_nComPortRxStep = 0;
				}
			}
#endif
			if (dtu->m_nRxLen >= (int)(sizeof(MMI_CMD_HEAD) + nDataLen + 5))
			{
				dtu->m_nComPortRxStep = 0;

				if (pHead->usHeadTag != MMI_START_ID)
					return false;
				if (pHead->ucCardId != dtu->GetCardId())
					return false;
				if (pHead->ucCmd != ucCmd)
					return false;
				if (pHead->ucCmd != MMI_CMD_WRITE_FLASH && pHead->ucCmd != MMI_CMD_WRITE_FLASH_F)
					return false;
				if (pHead->usLen != nDataLen + 4)
					return false;
				if (CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, dtu->m_ucRxBuf) != dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD) + pHead->usLen])
					return false;

				DWORD* pdw = (DWORD*)&dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD)];
				if (timer_UpdateProg->Enabled)
				{
					DWORD dwWant;
#ifndef GS7025H
					if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
						dwWant = (DWORD)(256 * 1024 + dtu->m_nDataSent);
					else
#endif
						dwWant = (DWORD)(1024 * 1024 + dtu->m_nDataSent);
					if (*pdw == dwWant)
					{
						dtu->m_bWantVerifyAct = false;
						return true;
					}
				}
				else
				{
					if (*pdw == (DWORD)(dtu->m_nTxAct * sysConfig.GetOneActVolMaxLen() + dtu->m_nDataSent))
					{
						dtu->m_bWantVerifyAct = false;
						return true;
					}
				}
			}
			return false;
		}
	}

	return false;
}
//---------------------------------------------------------------------------

bool TfrmMain::WaitForCmdEcho_HEB(BYTE ucCmd, CDtu* dtu, int nDataLen)
{
	if (sysConfig.m_nCommMethod == 1)
	{
		if (dtu->m_nRxLen >= (int)(sizeof(MMI_CMD_HEAD_HEB) + nDataLen + 1))
		{
			dtu->m_nRxLen = 0;
			MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)dtu->m_ucRxBuf;

			if (ucCmd == MMI_CMD_WRITE_FLASH)
			{
				if ((pHead->usHeadTag == MMI_START_ID) &&
					(pHead->ucCardId == dtu->GetCardId()) &&
					(pHead->ucKey1 == dtu->m_ucKey1) &&
					(pHead->ucKey2 == dtu->m_ucKey2) &&
					(pHead->ucCmd == ucCmd))
				{
					Encrypt(dtu, pHead->usLen + 1, &dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD_HEB)]);

					if ((pHead->usLen == 0) &&
						(CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, dtu->m_ucRxBuf) == dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen]))
					{
						dtu->m_bWantVerifyAct = true;
						return true;
					}
					else if (pHead->usLen == 4)
					{
						DWORD* pdw = (DWORD*)&dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD_HEB)];
						if (timer_UpdateProg->Enabled)
						{
							if ((*pdw == (DWORD)(1024 * 1024 + dtu->m_nDataSent)) &&
								(CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, dtu->m_ucRxBuf) == dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen]))
							{
								dtu->m_bWantVerifyAct = false;
								return true;
							}
						}
						else
						{
							if ((*pdw == (DWORD)(dtu->m_nTxAct * sysConfig.GetOneActVolMaxLen() + dtu->m_nDataSent)) &&
								(CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, dtu->m_ucRxBuf) == dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen]))
							{
								dtu->m_bWantVerifyAct = false;
								return true;
							}
						}
					}
				}
			}
			else if ((pHead->usHeadTag == MMI_START_ID) &&
				(pHead->ucCardId == dtu->GetCardId()) &&
				(pHead->ucKey1 == dtu->m_ucKey1) &&
				(pHead->ucKey2 == dtu->m_ucKey2) &&
				(pHead->ucCmd == ucCmd) &&
				(pHead->usLen == nDataLen))
			{
				Encrypt(dtu, pHead->usLen + 1, &dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD_HEB)]);
				if (CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, dtu->m_ucRxBuf) == dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen])
				return true;
			}
		}

		return false;
	}

	while (m_ComPort.GetInQueue())
	{
		switch (dtu->m_nComPortRxStep)
		{
		case 0:
			m_ComPort.Read(1, dtu->m_ucRxBuf);
			if (dtu->m_ucRxBuf[0] == MMI_START_ID1)
				dtu->m_nComPortRxStep = 1;
			break;
		case 1:
			m_ComPort.Read(1, dtu->m_ucRxBuf + 1);
			if (dtu->m_ucRxBuf[1] == MMI_START_ID2)
			{
				dtu->m_nComPortRxStep = 2;
				dtu->m_nRxLen = 2;
			}
			else
				dtu->m_nComPortRxStep = 0;
			break;
		default:
			while (m_ComPort.GetInQueue() && dtu->m_nRxLen < (int)(sizeof(MMI_CMD_HEAD_HEB) + nDataLen + 5))
			{
				m_ComPort.Read(1, dtu->m_ucRxBuf + dtu->m_nRxLen);
				dtu->m_nRxLen++;
			}
			MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)dtu->m_ucRxBuf;

			if (dtu->m_nRxLen >= (int)(sizeof(MMI_CMD_HEAD_HEB) + nDataLen + 1))
			{

				if ((pHead->usHeadTag == MMI_START_ID) &&
					 (pHead->ucCardId == dtu->GetCardId()) &&
					 (pHead->ucKey1 == dtu->m_ucKey1) &&
					 (pHead->ucKey2 == dtu->m_ucKey2) &&
					 (pHead->ucCmd == ucCmd) &&
					 (pHead->usLen == nDataLen))
				{
					Encrypt(dtu, pHead->usLen + 1, &dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD_HEB)]);
					if (CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, dtu->m_ucRxBuf) == dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen])
					{
						dtu->m_nComPortRxStep = 0;
						if (ucCmd == MMI_CMD_WRITE_FLASH)
							dtu->m_bWantVerifyAct = true;
						return true;
					}
				}

				if (ucCmd != MMI_CMD_WRITE_FLASH)
					dtu->m_nComPortRxStep = 0;
			}

			if (dtu->m_nRxLen >= (int)(sizeof(MMI_CMD_HEAD_HEB) + nDataLen + 5))
			{
				dtu->m_nComPortRxStep = 0;

				if (pHead->usHeadTag != MMI_START_ID)
					return false;
				if (pHead->ucCardId != dtu->GetCardId())
					return false;
				if (pHead->ucKey1 != dtu->m_ucKey1)
					return false;
				if (pHead->ucKey2 != dtu->m_ucKey2)
					return false;
				if (pHead->ucCmd != ucCmd)
					return false;
				if (pHead->ucCmd != MMI_CMD_WRITE_FLASH)
					return false;
				if (pHead->usLen != nDataLen + 4)
					return false;
				Encrypt(dtu, pHead->usLen + 1, &dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD_HEB)]);

				if (CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, dtu->m_ucRxBuf) != dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen])
					return false;

				DWORD* pdw = (DWORD*)&dtu->m_ucRxBuf[sizeof(MMI_CMD_HEAD_HEB)];
				if (timer_UpdateProg->Enabled)
				{
					if (*pdw == (DWORD)(1024 * 1024 + dtu->m_nDataSent))
					{
						dtu->m_bWantVerifyAct = false;
						return true;
					}
				}
				else if (timerFlashTest->Enabled)
					return true;
				else
				{
					if (*pdw == (DWORD)(dtu->m_nTxAct * sysConfig.GetOneActVolMaxLen() + dtu->m_nDataSent))
					{
						dtu->m_bWantVerifyAct = false;
						return true;
					}
				}
			}
			return false;
		}
	}

	return false;
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WriteFlash(CDtu* dtu)
{
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		SendCommand_WriteFlash_HEB(dtu);
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9025 || sysConfig.m_nCardType == CARD_TYPE_GS9023)
		SendCommand_TextAct(dtu);
	else
		SendCommand_WriteFlash_Normal(dtu);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_TextAct(CDtu* dtu)
{
	BYTE buf[2048];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	if (dtu->m_nTxStep == 1)
	{
		pHead->ucCmd = (mnuRamText->Checked) ? MMI_CMD_RAM_TEXT_ACT : MMI_CMD_TEXT_ACT;
		pHead->usLen = sysConfig.GetActDataSize(dtu, dtu->m_nTxAct);
		if (pHead->usLen < 16)
			pHead->usLen = MakeGS9025NullTextActData(dtu->m_nTxAct, &buf[sizeof(MMI_CMD_HEAD)]);
		else
			sysConfig.ReadActData(dtu, dtu->m_nTxAct, dtu->m_nDataSent, pHead->usLen, &buf[sizeof(MMI_CMD_HEAD)]);
		dtu->m_bWantVerifyAct = false;
		buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
		if (sysConfig.m_nCommMethod == 0)
			m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
		else
			SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	}
}

//---------------------------------------------------------------------------
void TfrmMain::SendCommand_WriteFlash_Normal(CDtu* dtu)
{
	BYTE buf[2048];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	DWORD* pStartPos = (DWORD*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	if (dtu->m_nTxStep == 0)
	{
		pHead->ucCmd = (sysConfig.m_nCardType == CARD_TYPE_GS7020BF_W) ? MMI_CMD_STOP_ACT_PROC : MMI_CMD_STOP_DISPLAY;
		pHead->usLen = 0;
	}
	else if (dtu->m_nTxStep == 1)
	{
		int nLeftDataLen = sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) - dtu->m_nDataSent;
		pHead->ucCmd = (sysConfig.m_nCardType != CARD_TYPE_GS7020F) ? MMI_CMD_WRITE_FLASH : MMI_CMD_WRITE_FLASH_F;
		pHead->usLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize + 4 : nLeftDataLen + 4;
		*pStartPos = (dtu->m_nTxAct) * sysConfig.GetOneActVolMaxLen() + dtu->m_nDataSent;
		sysConfig.ReadActData(dtu, dtu->m_nTxAct, dtu->m_nDataSent, pHead->usLen - 4, &buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)]);

		if (sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) > sysConfig.m_nMaxPackSize)
		{
			if (dtu->m_nDataSent == 0)
				buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD) + 1] = 0xFF;
		}
	}
	else if (dtu->m_nTxStep == 2)	//回读节目内容
	{
		int nLeftDataLen = sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) - dtu->m_nDataSent;
		pHead->ucCmd = MMI_CMD_READ_FLASH;
		pHead->usLen = 6;
		*pStartPos = (dtu->m_nTxAct) * sysConfig.GetOneActVolMaxLen() + dtu->m_nDataSent;
		WORD* pusLen = (WORD*)(pStartPos + 1);
		*pusLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize : nLeftDataLen;
	}
	else if (dtu->m_nTxStep == 3)
	{
		pHead->ucCmd = MMI_CMD_RESTART_DISPLAY;
		pHead->usLen = 0;
	}
	else if (dtu->m_nTxStep == 4)
	{
		dtu->m_nDataSent = 1;
		int nLeftDataLen = 1;
		pHead->ucCmd = (sysConfig.m_nCardType != CARD_TYPE_GS7020F) ? MMI_CMD_WRITE_FLASH : MMI_CMD_WRITE_FLASH_F;
		pHead->usLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize + 4 : nLeftDataLen + 4;
		*pStartPos = (dtu->m_nTxAct) * sysConfig.GetOneActVolMaxLen() + dtu->m_nDataSent;
		sysConfig.ReadActData(dtu, dtu->m_nTxAct, dtu->m_nDataSent, pHead->usLen - 4, &buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)]);
	}
	else if (dtu->m_nTxStep == 5)	//深圳苓贯 YL2010AF 节目发送结束后发送总播放时间
	{
		pHead->ucCmd = MMI_CMD_PLAYTIME;
		pHead->usLen = 4;
		*pStartPos = CalcAllPlayTime();
	}

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WriteFlash_HEB(CDtu* dtu)
{
	BYTE buf[2048];
	MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;
	DWORD* pStartPos = (DWORD*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	if (dtu->m_nTxStep == 0)
	{
		pHead->ucCmd = MMI_CMD_STOP_DISPLAY;
		pHead->usLen = 0;
	}
	else if (dtu->m_nTxStep == 1)
	{
		int nLeftDataLen = sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) - dtu->m_nDataSent;
		pHead->ucCmd = MMI_CMD_WRITE_FLASH;
		pHead->usLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize + 4 : nLeftDataLen + 4;
		*pStartPos = (dtu->m_nTxAct) * sysConfig.GetOneActVolMaxLen() + dtu->m_nDataSent;
		sysConfig.ReadActData(dtu, dtu->m_nTxAct, dtu->m_nDataSent, pHead->usLen - 4, &buf[sizeof(MMI_CMD_HEAD_HEB) + sizeof(DWORD)]);

		if (sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) > sysConfig.m_nMaxPackSize)
		{
			if (dtu->m_nDataSent == 0)
				buf[sizeof(MMI_CMD_HEAD_HEB) + sizeof(DWORD) + 1] = 0xFF;
		}
	}
	else if (dtu->m_nTxStep == 2)	//回读节目内容
	{
		int nLeftDataLen = sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) - dtu->m_nDataSent;
		pHead->ucCmd = MMI_CMD_READ_FLASH;
		pHead->usLen = 6;
		*pStartPos = (dtu->m_nTxAct) * sysConfig.GetOneActVolMaxLen() + dtu->m_nDataSent;
		WORD* pusLen = (WORD*)(pStartPos + 1);
		*pusLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize : nLeftDataLen;
	}
	else if (dtu->m_nTxStep == 3)
	{
		pHead->ucCmd = MMI_CMD_RESTART_DISPLAY;
		pHead->usLen = 0;
	}
	else if (dtu->m_nTxStep == 4)
	{
		dtu->m_nDataSent = 1;
		int nLeftDataLen = 1;
		pHead->ucCmd = MMI_CMD_WRITE_FLASH;
		pHead->usLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize + 4 : nLeftDataLen + 4;
		*pStartPos = (dtu->m_nTxAct) * sysConfig.GetOneActVolMaxLen() + dtu->m_nDataSent;
		sysConfig.ReadActData(dtu, dtu->m_nTxAct, dtu->m_nDataSent, pHead->usLen - 4, &buf[sizeof(MMI_CMD_HEAD_HEB) + sizeof(DWORD)]);
	}

	pHead->ucKey1 = dtu->m_ucKey1 = GetTickCount();
	pHead->ucKey2 = dtu->m_ucKey2 = GetTickCount() >> 8;
	buf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, buf);
	Encrypt(dtu, pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);

	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnFontClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

#ifndef AUTO_SIZE_TEXT
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		TfrmFontSelect* frmFontSelect = new TfrmFontSelect(this);
		if (wndTextEdit->m_bDotFont)
		{
			if (wndTextEdit->m_nDotFontSize == 16)
				frmFontSelect->rbtn16->Checked = true;
			else if (wndTextEdit->m_nDotFontSize == 24)
				frmFontSelect->rbtn24->Checked = true;
			else
				frmFontSelect->rbtn32->Checked = true;
		}
		else
			frmFontSelect->rbtnWindows->Checked = true;

		if (frmFontSelect->ShowModal() != mrOk)
		{
			delete frmFontSelect;
			return;
		}

		if (frmFontSelect->rbtnWindows->Checked)
		{
			delete frmFontSelect;
			dlgFont->Font = wndTextEdit->Canvas->Font;
			if (dlgFont->Execute())
			{
				sysConfig.m_sActFontName = dlgFont->Font->Name;
				sysConfig.m_nActFontSize = dlgFont->Font->Size;
				sysConfig.m_nActFontStyle = dlgFont->Font->Style.ToInt();
				sysConfig.m_bDotFont = false;

				wndTextEdit->m_bDotFont = false;
				wndTextEdit->Font->Name = sysConfig.m_sActFontName;
				wndTextEdit->Font->Size = sysConfig.m_nActFontSize;
				wndTextEdit->Font->Style = TFontStyles(sysConfig.m_nActFontStyle);
				wndTextEdit->SetFont(wndTextEdit->Font, sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea]);
				wndTextEdit->Refresh();
				wndTextEdit->m_bDirty = true;
			}
		}
		else if (frmFontSelect->rbtn16->Checked)
		{
			delete frmFontSelect;
			sysConfig.m_bDotFont = true;
			sysConfig.m_nDotFontSize = 16;
			if (sysConfig.m_bDotFont)
				wndTextEdit->SetDotFont(sysConfig.m_nDotFontSize);
			else
			{
				wndTextEdit->Font->Name = sysConfig.m_sActFontName;
				wndTextEdit->Font->Size = sysConfig.m_nActFontSize;
				wndTextEdit->Font->Style = TFontStyles(sysConfig.m_nActFontStyle);
			}
			wndTextEdit->Refresh();
			wndTextEdit->m_bDirty = true;
		}
		else if (frmFontSelect->rbtn24->Checked)
		{
			delete frmFontSelect;
			sysConfig.m_bDotFont = true;
			sysConfig.m_nDotFontSize = 24;
			if (sysConfig.m_bDotFont)
				wndTextEdit->SetDotFont(sysConfig.m_nDotFontSize);
			else
			{
				wndTextEdit->Font->Name = sysConfig.m_sActFontName;
				wndTextEdit->Font->Size = sysConfig.m_nActFontSize;
				wndTextEdit->Font->Style = TFontStyles(sysConfig.m_nActFontStyle);
			}
			wndTextEdit->Refresh();
			wndTextEdit->m_bDirty = true;
		}
		else
		{
			delete frmFontSelect;
			sysConfig.m_bDotFont = true;
			sysConfig.m_nDotFontSize = 32;
			if (sysConfig.m_bDotFont)
				wndTextEdit->SetDotFont(sysConfig.m_nDotFontSize);
			else
			{
				wndTextEdit->Font->Name = sysConfig.m_sActFontName;
				wndTextEdit->Font->Size = sysConfig.m_nActFontSize;
				wndTextEdit->Font->Style = TFontStyles(sysConfig.m_nActFontStyle);
			}
			wndTextEdit->Refresh();
			wndTextEdit->m_bDirty = true;
		}
	}
	else
#endif
	{
		dlgFont->Font = wndTextEdit->Canvas->Font;
		dlgFont->Font->Color = clRed;
		if (dlgFont->Execute())
		{
			sysConfig.m_sActFontName = dlgFont->Font->Name;
			sysConfig.m_nActFontSize = dlgFont->Font->Size;
			sysConfig.m_nActFontStyle = dlgFont->Font->Style.ToInt();
			sysConfig.m_bDotFont = false;

			wndTextEdit->m_bDotFont = false;
			wndTextEdit->Font->Name = sysConfig.m_sActFontName;
			wndTextEdit->Font->Size = sysConfig.m_nActFontSize;
			wndTextEdit->Font->Style = TFontStyles(sysConfig.m_nActFontStyle);
			wndTextEdit->SetFont(wndTextEdit->Font, sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea]);
			wndTextEdit->Refresh();
			wndTextEdit->m_bDirty = true;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnAlignClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	int nAlign;

	if (btnAlignLeft->Down)
		nAlign = ALIGN_LEFT;
	else if (btnAlignRight->Down)
		nAlign = ALIGN_RIGHT;
	else
		nAlign = ALIGN_CENTER;

	if (nAlign != wndTextEdit->m_nAlignStyle)
	{
		wndTextEdit->m_nAlignStyle = nAlign;
		wndTextEdit->m_bDirty = true;
		wndTextEdit->SetCaret();
		wndTextEdit->Refresh();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnAlignVertCenterClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	if (wndTextEdit->m_bAlignVertCenter != btnAlignVertCenter->Down)
	{
		wndTextEdit->m_bDirty = true;
		wndTextEdit->m_bAlignVertCenter = btnAlignVertCenter->Down;
		wndTextEdit->SetCaret();
		wndTextEdit->Refresh();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnRedClick(TObject *Sender)
{
	wndTextEdit->ChangeTextColor(COLOR_RED);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnGreenClick(TObject *Sender)
{
	wndTextEdit->ChangeTextColor(COLOR_GREEN);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnYellowClick(TObject *Sender)
{
	wndTextEdit->ChangeTextColor(COLOR_YELLOW);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnBlackClick(TObject *Sender)
{
	wndTextEdit->ChangeTextColor(COLOR_BLACK);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnRed_BackClick(TObject *Sender)
{
	wndTextEdit->ChangeBackColor(BACKGROUND_COLOR_RED);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnGreen_BackClick(TObject *Sender)
{
	wndTextEdit->ChangeBackColor(BACKGROUND_COLOR_GREEN);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnWhite_BackClick(TObject *Sender)
{
	wndTextEdit->ChangeBackColor(BACKGROUND_COLOR_WHITE);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnBlack_BackClick(TObject *Sender)
{
	wndTextEdit->ChangeBackColor(BACKGROUND_COLOR_BLACK);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuCardConfigClick(TObject *Sender)
{
	int mr;
	int nSelected = GetCheckedCardCount();

#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	if (nSelected > 1)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"配置操作每次只能选择一个显示屏。", L"错误", MB_OK | MB_ICONEXCLAMATION);
		else
			Application->MessageBox(L"Please select only one LED panel.", L"Error", MB_OK | MB_ICONWARNING);
		return;
	}

	if (nSelected <= 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请先选择要操作的显示屏(复选框打勾)", L"错误", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select one LED panel.", L"Error", MB_OK | MB_ICONWARNING);
		return;
	}

	SaveAct();

	PVirtualNode Node = GetFirstCheckedCard();
	CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
	TfrmConfig *frmConfig = new TfrmConfig(this, dtu);
	mr = frmConfig->ShowModal();
	delete frmConfig;
	if (mr != mrOk)
		return;

	sysConfig.SaveConfig();
	InitActInputWindow();
	vt->Refresh();
}
//---------------------------------------------------------------------------

UnicodeString TfrmMain::GetActIndexText(int nIndex)
{
	UnicodeString sResult = nIndex + 1;

	if (sysConfig.m_nCardType == CARD_TYPE_GS7020BF || sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB || sysConfig.m_nCardType == CARD_TYPE_GS9020F_GPS || sysConfig.m_nCardType == CARD_TYPE_GS7020HFA)
	{
		if (nIndex >= sysConfig.m_nMaxActCount - 6)
			sResult = "插" + UnicodeString(nIndex + 1 - sysConfig.m_nMaxActCount + 6);

		if (!sysConfig.m_bActSuitForAllCard[nIndex])
			sResult += "*";

		return sResult;
	}

	if ((sysConfig.m_bSupportWarning) && (nIndex == sysConfig.m_nMaxActCount - 1))
		sResult = "报警";
	else
	{
		if (sysConfig.m_nCardType == CARD_TYPE_GS7020BX)
		{
			if (sysConfig.m_bSupportWarning)
			{
				if (sysConfig.m_bSupportInsertPlay)
				{
					if (nIndex >= sysConfig.m_nMaxActCount - 6)
						sResult = "插" + UnicodeString(nIndex + 1 - sysConfig.m_nMaxActCount + 6);
				}
			}
			else
			{
				if (sysConfig.m_bSupportInsertPlay )
				{
					if (nIndex >= sysConfig.m_nMaxActCount - 5)
						sResult = "插" + UnicodeString(nIndex + 1 - sysConfig.m_nMaxActCount + 5);
				}
			}

			if (sysConfig.m_bSupportCycleTimeHint && (nIndex == sysConfig.m_nSycleTimeActIndex))
				sResult = "报时";
		}
		else if (sysConfig.m_nCardType == CARD_TYPE_GS7020BQ)
		{
			switch (nIndex)
			{
			case 100:
				sResult = "红色预警";
				break;
			case 101:
				sResult = "橙色预警";
				break;
			case 102:
				sResult = "黄色预警";
				break;
			case 103:
				sResult = "蓝色预警";
				break;
			}
		}
		else if (sysConfig.m_nCardType == CARD_TYPE_GS7020BF_W)
		{
			if ((nIndex >= sysConfig.m_nMaxActCount - 64 - 6) && (nIndex < sysConfig.m_nMaxActCount - 64))
				sResult = "插" + UnicodeString(nIndex + 1 - (sysConfig.m_nMaxActCount - 64 - 6));
		}
		else
		{
			if (sysConfig.m_bSupportWarning)
			{
				if (sysConfig.m_bSupportInsertPlay)
				{
					if (nIndex >= sysConfig.m_nMaxActCount - 7)
						sResult = "插" + UnicodeString(nIndex + 1 - sysConfig.m_nMaxActCount + 7);
				}
			}
			else
			{
				if (sysConfig.m_bSupportInsertPlay )
				{
					if (nIndex >= sysConfig.m_nMaxActCount - 6)
						sResult = "插" + UnicodeString(nIndex + 1 - sysConfig.m_nMaxActCount + 6);
				}
			}
		}
	}

	if (!sysConfig.m_bActSuitForAllCard[nIndex])
		sResult += "*";

	return sResult;
}
//---------------------------------------------------------------------------

void TfrmMain::InitActInputWindow()
{
#ifdef AUTO_SIZE_TEXT
	m_dtu->ApplyCurrentScreenConfig();
#endif
	mnuDownloadDotFont->Visible = (sysConfig.m_nCardType == CARD_TYPE_GS7022B ||
								   sysConfig.m_nCardType == CARD_TYPE_GS2012A_S ||
								   sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64 ||
								   sysConfig.m_nCardType == CARD_TYPE_GS9025 ||
								   sysConfig.m_nCardType == CARD_TYPE_GS9023 ||
								   sysConfig.m_nCardType == CARD_TYPE_GS7020HS ||
								   sysConfig.m_nCardType == CARD_TYPE_GS9020S);
	mnuQueryCheckResult->Visible = ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026));
	mnuParkDigit->Visible = (sysConfig.m_nCardType == CARD_TYPE_GS7020BP || sysConfig.m_nCardType == CARD_TYPE_SN2P);
	mnuRamText->Visible = (sysConfig.m_nCardType == CARD_TYPE_GS9025);
	btnGps->Visible = (sysConfig.m_nCardType == CARD_TYPE_GS9020F_GPS);
	mnuAdjustLightness->Visible = (sysConfig.m_nCardType == CARD_TYPE_GS9020F_GPS);
	mnuBrightnessSetup->Visible = (sysConfig.m_nCardType == CARD_TYPE_GS9020F_GPS);

	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF) || (sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS7020BF_W) || (sysConfig.m_nCardType == CARD_TYPE_GS9026) || (sysConfig.m_nCardType == CARD_TYPE_YL2010AF) || (sysConfig.m_nCardType == CARD_TYPE_GS9020F_GPS) || (sysConfig.m_nCardType == CARD_TYPE_GS7020HFA))
		cbRefActIndex->Enabled = true;
	else
		cbRefActIndex->Enabled = false;

	//if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BQ) && (sysConfig.m_nMaxActCount == 127) && (sysConfig.m_nSubAreaCount >= 5))
	{
		btnTxWeatherWarnning->Visible = (sysConfig.m_nCardType != CARD_TYPE_SN2P);
		mnuTxWeatherWarnning->Visible = (sysConfig.m_nCardType != CARD_TYPE_SN2P);
		btnSendAct->Visible = (sysConfig.m_nCardType != CARD_TYPE_SN2P);
		mnuSendAct->Visible = (sysConfig.m_nCardType != CARD_TYPE_SN2P);
		mnuManualReTxAct->Visible = (sysConfig.m_nCardType != CARD_TYPE_SN2P);
		mnuEraseAllAct->Visible = (sysConfig.m_nCardType != CARD_TYPE_SN2P);
	}
	/*else
	{
		btnTxWeatherWarnning->Visible = false;
		mnuTxWeatherWarnning->Visible = false;
	}*/

	panelScreen->Visible = true;
	panelMisc->Visible = true;

	btnRed->Enabled = true;
	btnGreen->Enabled = sysConfig.m_bDoubleColor || sysConfig.m_bFullColor;
	btnYellow->Enabled = sysConfig.m_bDoubleColor || sysConfig.m_bFullColor;
	btnBlack->Enabled = true;
	btnRed_Back->Enabled = true;
	btnGreen_Back->Enabled = sysConfig.m_bDoubleColor || sysConfig.m_bFullColor;
	btnYellow_Back->Enabled = sysConfig.m_bDoubleColor || sysConfig.m_bFullColor;
	btnBlack_Back->Enabled = true;
	btnBlue->Enabled = sysConfig.m_bFullColor;
	btnPink->Enabled = sysConfig.m_bFullColor;
	btnAqua->Enabled = sysConfig.m_bFullColor;
	btnWhite->Enabled = sysConfig.m_bFullColor;
	btnBlue_Back->Enabled = sysConfig.m_bFullColor;
	btnPink_Back->Enabled = sysConfig.m_bFullColor;
	btnAqua_Back->Enabled = sysConfig.m_bFullColor;
	btnWhite_Back->Enabled = sysConfig.m_bFullColor;

	comboArea->Items->Clear();
	for (int i = 0; i < sysConfig.m_nSubAreaCount; i++)
		comboArea->Items->Add(i + 1);
	comboArea->ItemIndex = 0;

	bool bCheckedAct[MAX_ACT_COUNT];
	memset(bCheckedAct, 0, sizeof(bCheckedAct));
	for (int nIndex = 0; nIndex < listActs->Items->Count; nIndex++)
		bCheckedAct[nIndex] = listActs->Items->Item[nIndex]->Checked;

	lbAreaOfAct->Visible = true;
	comboArea->Visible = true;
	lbStart->Visible = true;
	dateActStart->Visible = true;
	timeActStart->Visible = true;
	lbEnd->Visible = true;
	dateActEnd->Visible = true;
	timeActEnd->Visible = true;
	groupBoxPeriod->Visible = true;

	if (sysConfig.m_nSubAreaCount != comboArea->Items->Count)
	{
		if (sysConfig.m_nSubAreaCount > comboArea->Items->Count)
		{
			for (int i = comboArea->Items->Count; i < sysConfig.m_nSubAreaCount; i++)
				comboArea->Items->Add(i + 1);
		}
		else
		{
			while (sysConfig.m_nSubAreaCount != comboArea->Items->Count)
				comboArea->Items->Delete(comboArea->Items->Count - 1);
		}
		comboAreaChange(this);
	}

	if (sysConfig.m_bActSecond)
	{
		for (int i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
		{
			m_timeStart[i]->Format = "HH:mm:ss";
			m_timeEnd[i]->Format = "HH:mm:ss";
		}
	}
	else
	{
		for (int i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
		{
			m_timeStart[i]->Format = "HH:mm";
			m_timeEnd[i]->Format = "HH:mm";
		}
	}

	cbStartWeek->Enabled = false;
	cbEndWeek->Enabled = false;

	comboEditAct->Items->Clear();
	for (int i = 0; i < sysConfig.m_nMaxActCount; i++)
		comboEditAct->Items->Add(GetActIndexText(i));

	cbRefActIndex->Items->Clear();
	for (int i = 0; i < sysConfig.m_nMaxActCount; i++)
		cbRefActIndex->Items->Add(i + 1);

	TListItem *item;
	listActs->Items->Clear();
	for (int nIndex = 0; nIndex < sysConfig.m_nMaxActCount; nIndex++)
	{
		item = listActs->Items->Add();
		item->Caption = GetActIndexText(nIndex);
		item->SubItems->Add(sysConfig.GetActArea(m_dtu, nIndex) + 1);
		item->SubItems->Add(sysConfig.GetActDesc(m_dtu, nIndex));
		item->Checked = bCheckedAct[nIndex];
	}

	int nAreaIndex = comboArea->ItemIndex;
	wndTextEdit->m_nEditScreenWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];
	wndTextEdit->m_nEditScreenHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];
	wndTextEdit->Height = sysConfig.m_nSubAreaHeight[nAreaIndex] * MAX_EDIT_SCREEN_Y_COUNT;
	wndTextEdit->Width = sysConfig.m_nSubAreaWidth[nAreaIndex] * MAX_EDIT_SCREEN_X_COUNT;

	if (m_nActIndex >= sysConfig.m_nMaxActCount)
	{
		m_nActIndex = 0;
		comboEditAct->ItemIndex = 0;
		listActs->ItemIndex = 0;
	}
	else
	{
		comboEditAct->ItemIndex = m_nActIndex;
		listActs->ItemIndex = m_nActIndex;
	}

	sysConfig.LoadAct(m_dtu, m_nActIndex);
	UpdateActVol();
	wndTextEdit->Refresh();
	DrawAreaImage();

	mnuCardConfig->Enabled = true;
	btnCardConfig->Enabled = true;
	listActs->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuAdminPasswordClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	TfrmPassword * frmPassword = new TfrmPassword(this);
	frmPassword->ShowModal();
	delete frmPassword;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuCardListClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	SaveAct();

	TfrmCardList *frmCardList = new TfrmCardList(this);
	frmCardList->ShowModal();
	if (!frmCardList->m_bChanged)
	{
		delete frmCardList;
		return;
	}
	delete frmCardList;

	vt->Clear();
	m_dtu = NULL;
	m_Node = NULL;
	LoadAllCard();
	InitActInputWindow();

	LoadAllRootNodeName();
	LoadAllCardName();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::panelScreenResize(TObject *Sender)
{
	scrollboxEdit->Width = panelScreen->Width - scrollboxEdit->Left * 2;
	scrollboxEdit->Height = panelScreen->Height - scrollboxEdit->Top - 8;

	if (wndTextEdit->Width < scrollboxEdit->Width)
		wndTextEdit->Left = (scrollboxEdit->Width - wndTextEdit->Width) / 2;
	else
		wndTextEdit->Left = 0;
	if (wndTextEdit->Height < scrollboxEdit->Height)
		wndTextEdit->Top = (scrollboxEdit->Height - wndTextEdit->Height) / 2;
	else
		wndTextEdit->Top = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::comboAreaChange(TObject *Sender)
{
	int nAreaIndex = comboArea->ItemIndex;

#ifdef AUTO_SIZE_TEXT
	m_dtu->ApplyCurrentScreenConfig();
#endif
	wndTextEdit->Height = sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] * MAX_EDIT_SCREEN_Y_COUNT;
	wndTextEdit->Width = sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea] * MAX_EDIT_SCREEN_X_COUNT;
	wndTextEdit->m_nEditScreenWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];
	wndTextEdit->m_nEditScreenHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];
	panelScreenResize(this);
	wndTextEdit->Refresh();
	listActs->Items->Item[m_nActIndex]->SubItems->Strings[0] = nAreaIndex + 1;
	DrawAreaImage();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuSendActClick(TObject *Sender)
{
	int nSelected = 0;
	int i;
	TListItem *itemAct;
	int nFirstAct;

#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	SaveAct();

	for (i = 0; i < listActs->Items->Count; i++)
	{
		itemAct = listActs->Items->Item[i];
		if (itemAct->Checked)
		{
			nSelected++;
			nFirstAct = i;
			break;
		}
	}

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要发送的节目。", L"错误", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select the acts to be sent.", L"Error", MB_OK | MB_ICONWARNING);
		return;
	}

	if (sysConfig.m_bAutoReTx_OnlyLast)
		ClearAllTxOkStatus();

	nSelected = 0;
	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
		dtu->m_nTxAct = nFirstAct;
		dtu->m_nDataSent = 0;
		dtu->m_nTxStep = 0;
		dtu->m_nTxStatus = TX_STATUS_READY;
		for (i = 0; i < listActs->Items->Count; i++)
		{
#ifdef AUTO_SIZE_TEXT
			dtu->m_sStatus = "正在生成节目" + UnicodeString(i) + "数据";
			vt->InvalidateNode(Node);
			//CreateActTxData(dtu, i);
#endif
			itemAct = listActs->Items->Item[i];
			if (itemAct->Checked)
			{
				if (sysConfig.GetActDataSize(dtu, i) > sysConfig.GetOneActVolMaxLen())
				{
					UnicodeString s = "显示屏<" + dtu->m_sName + ">中的节目";
					s += UnicodeString(i + 1);
					s += "内容过大，请适当减少文字内容、缩小打开的图片文件或减少控制卡的节目容量。";
					Application->MessageBox(s.c_str(), L"错误", MB_OK | MB_ICONWARNING);
					return;
				}
			}
		}
		for (i = 0; i < listActs->Items->Count; i++)
		{
			itemAct = listActs->Items->Item[i];
			if (itemAct->Checked)
				dtu->m_bActTxOk[i] = false;
		}

		nSelected++;
		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要发送的显示屏", L"发送节目内容", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Send Acts", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenCommPort(nFirstAct, (m_nLanguage == LANGUAGE_CHINESE) ? "发送节目内容" : "Send Acts"))
		return;

	Node = vt->GetFirst();
	CDtu* dtuRoot = (CDtu*)(vt->GetNodeData(Node));

	log.StartLogTx();
	for (i = 0; i < listActs->Items->Count; i++)
	{
		itemAct = listActs->Items->Item[i];
		if (itemAct->Checked)
			log.LogAct(dtuRoot, i);
	}
	log.StartLogCard();

	StartOperation();
	m_bReTx = false;
	timerTxAct->Enabled = true;
}
//---------------------------------------------------------------------------

BYTE TfrmMain::CalcChecksum(int nLen, BYTE* p)
{
	BYTE uc = 0;
	int i;

	for (i = 0; i < nLen; i++)
		uc += *p++;
	uc ^= 0xFF;

	return uc;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::comboEditActChange(TObject *Sender)
{
	if (m_nActIndex == comboEditAct->ItemIndex)
		return;

	wndTextEdit->EnabledRefresh(false);
	SaveAct();
	m_nActIndex = comboEditAct->ItemIndex;
	listActs->ItemIndex = m_nActIndex;
	sysConfig.LoadAct(m_dtu, m_nActIndex);
#ifdef AUTO_SIZE_TEXT
	m_dtu->ApplyCurrentScreenConfig();
#endif
	UpdateActVol();
	wndTextEdit->ResetCursor();
	wndTextEdit->EnabledRefresh(true);
	wndTextEdit->Refresh();
	wndTextEdit->m_bDirty = false;
	DrawAreaImage();
}
//---------------------------------------------------------------------------

void TfrmMain::UpdateActVol()
{
	wndTextEdit->Clear();
	wndTextEdit->m_nAlignStyle = sysConfig.m_nActHorzAlignment;
	wndTextEdit->m_bAlignVertCenter = (sysConfig.m_nActVertAlignment != 0);
	wndTextEdit->Font->Name = sysConfig.m_sActFontName;
	wndTextEdit->Font->Size = sysConfig.m_nActFontSize;
	wndTextEdit->Font->Style = TFontStyles(sysConfig.m_nActFontStyle);
	wndTextEdit->SetText(sysConfig.m_sActText);
	wndTextEdit->SetTextColor(sysConfig.m_sActTextColor);
	wndTextEdit->SetActiveText(sysConfig.m_sActActiveText);

	switch (sysConfig.m_nActHorzAlignment)
	{
	case ALIGN_LEFT:
		btnAlignLeft->Down = true;
		break;
	case ALIGN_RIGHT:
		btnAlignRight->Down = true;
		break;
	default:
		btnAlignCenter->Down = true;
		break;
	}

	try
	{
		btnAlignVertCenter->Down = (sysConfig.m_nActVertAlignment != 0);
	}
	catch (Exception &ex)
	{
	}

	editActDesc->Text = sysConfig.m_sActDesc;
	comboArea->ItemIndex = sysConfig.m_nActArea;
	comboEnterStyle->ItemIndex = sysConfig.m_nActEnterStyle;
	comboExitStyle->ItemIndex = sysConfig.m_nActExitStyle;
	editEnterSpeed->Text = sysConfig.m_nActEnterSpeed;
	editExitSpeed->Text = sysConfig.m_nActExitSpeed;
	editShowTime->Text = sysConfig.m_nActShowTime;
	editRepeatCount->Text = sysConfig.m_nActRepeatCount;
	editShutterWidth->Text = sysConfig.m_nActShutterWidth;
	editPicPosX->Text = sysConfig.m_nActImagePosX;
	editPicPosY->Text = sysConfig.m_nActImagePosY;
	if (sysConfig.m_picAct != NULL)
	{
		wndTextEdit->pic = new TPicture;
		wndTextEdit->pic->Bitmap->Assign(sysConfig.m_picAct->Bitmap);
	}

	SYSTEMTIME st;
	::GetLocalTime(&st);

	st.wYear = sysConfig.m_nActStartYear;
	st.wMonth = sysConfig.m_nActStartMonth;
	st.wDay = sysConfig.m_nActStartDay;
	st.wHour = sysConfig.m_nActStartHour;
	st.wMinute = sysConfig.m_nActStartMin;
	dateActStart->DateTime = SystemTimeToDateTime(st);
	timeActStart->DateTime = SystemTimeToDateTime(st);

	st.wYear = sysConfig.m_nActEndYear;
	st.wMonth = sysConfig.m_nActEndMonth;
	st.wDay = sysConfig.m_nActEndDay;
	st.wHour = sysConfig.m_nActEndHour;
	st.wMinute = sysConfig.m_nActEndMin;
	dateActEnd->DateTime = SystemTimeToDateTime(st);
	timeActEnd->DateTime = SystemTimeToDateTime(st);

	for (int i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
	{
		st.wHour = sysConfig.m_nActStartMinuteInDay[i] / 60;
		st.wMinute = sysConfig.m_nActStartMinuteInDay[i] % 60;
		st.wSecond = sysConfig.m_nActStartSecondInDay[i];
		m_timeStart[i]->DateTime = SystemTimeToDateTime(st);
		st.wHour = sysConfig.m_nActEndMinuteInDay[i] / 60;
		st.wMinute = sysConfig.m_nActEndMinuteInDay[i] % 60;
		st.wSecond = sysConfig.m_nActEndSecondInDay[i];
		m_timeEnd[i]->DateTime = SystemTimeToDateTime(st);
	}

	cbStartWeek->ItemIndex = sysConfig.m_nActStartWeek;
	cbEndWeek->ItemIndex = sysConfig.m_nActEndWeek;
	if (sysConfig.m_nActRefIndex >= sysConfig.m_nMaxActCount || sysConfig.m_nActRefIndex < 0)
		sysConfig.m_nActRefIndex = comboEditAct->ItemIndex;
	cbRefActIndex->ItemIndex = sysConfig.m_nActRefIndex;
	wndTextEdit->m_nEditScreenWidth = sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea];
	wndTextEdit->m_nEditScreenHeight = sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea];
	lbDisplayTime->Caption = UnicodeString(CalcPlayTime(m_nActIndex) * sysConfig.GetActRepeatCount(m_dtu, m_nActIndex)) + ((sysConfig.m_nLanguage == LANGUAGE_CHINESE)? " 毫秒" : " ms");
	lbAllDisplayTime->Caption = UnicodeString(CalcAllPlayTime()) + ((sysConfig.m_nLanguage == LANGUAGE_CHINESE) ? " 毫秒" : " ms");

	wndTextEdit->SetFont(wndTextEdit->Font, sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea]);
	if (sysConfig.m_bDotFont)
		wndTextEdit->SetDotFont(sysConfig.m_nDotFontSize);
#ifdef AUTO_SIZE_TEXT
	wndTextEdit->AutoSizeText();
#endif
}
//---------------------------------------------------------------------------

void TfrmMain::SaveAct()
{
	if (comboArea->ItemIndex < 0)
		return;

	sysConfig.m_nActHorzAlignment = wndTextEdit->m_nAlignStyle;
	sysConfig.m_nActVertAlignment = wndTextEdit->m_bAlignVertCenter;
	sysConfig.m_bDotFont = wndTextEdit->m_bDotFont;
	sysConfig.m_nDotFontSize = wndTextEdit->m_nDotFontSize;
	sysConfig.m_sActFontName = wndTextEdit->Canvas->Font->Name;
	sysConfig.m_nActFontSize = wndTextEdit->Canvas->Font->Size;
	sysConfig.m_nActFontStyle = wndTextEdit->Canvas->Font->Style.ToInt();
	sysConfig.m_sActText = wndTextEdit->GetText();
	sysConfig.m_sActTextColor = wndTextEdit->GetTextColor();
	sysConfig.m_sActActiveText = wndTextEdit->GetActiveText();
	sysConfig.m_sActDesc = editActDesc->Text;
	sysConfig.m_nActArea = comboArea->ItemIndex;
	sysConfig.m_nActEnterStyle = comboEnterStyle->ItemIndex;
	sysConfig.m_nActExitStyle = comboExitStyle->ItemIndex;
	if (sysConfig.m_nCardType == CARD_TYPE_GS7020BX)
	{
		sysConfig.m_nActEnterSpeed = (editEnterSpeed->Text.ToInt() / 4) * 4;
		sysConfig.m_nActExitSpeed = (editExitSpeed->Text.ToInt() / 4) * 4;
		sysConfig.m_nActShowTime = (editShowTime->Text.ToInt() / 4) * 4;
	}
	else
	{
		sysConfig.m_nActEnterSpeed = editEnterSpeed->Text.ToInt();
		sysConfig.m_nActExitSpeed = editExitSpeed->Text.ToInt();
		sysConfig.m_nActShowTime = editShowTime->Text.ToInt();
	}
	sysConfig.m_nActRepeatCount = editRepeatCount->Text.ToInt();
	sysConfig.m_nActShutterWidth = editShutterWidth->Text.ToInt();
	if (sysConfig.m_nActShutterWidth <= 0)
		sysConfig.m_nActShutterWidth = 8;
	sysConfig.m_nActImagePosX = editPicPosX->Text.ToInt();
	sysConfig.m_nActImagePosY = editPicPosY->Text.ToInt();
	if (wndTextEdit->pic == NULL)
	{
		if (sysConfig.m_picAct != NULL)
		{
			delete sysConfig.m_picAct;
			sysConfig.m_picAct = NULL;
		}
	}
	else
	{
		if (sysConfig.m_picAct == NULL)
			sysConfig.m_picAct = new TPicture;
		sysConfig.m_picAct->Bitmap->Assign(wndTextEdit->pic->Bitmap);
	}

	SYSTEMTIME st;

	DateTimeToSystemTime(dateActStart->DateTime, st);
	sysConfig.m_nActStartYear = st.wYear;
	sysConfig.m_nActStartMonth = st.wMonth;
	sysConfig.m_nActStartDay = st.wDay;

	DateTimeToSystemTime(timeActStart->DateTime, st);
	sysConfig.m_nActStartHour = st.wHour;
	sysConfig.m_nActStartMin = st.wMinute;

	DateTimeToSystemTime(dateActEnd->DateTime, st);
	sysConfig.m_nActEndYear = st.wYear;
	sysConfig.m_nActEndMonth = st.wMonth;
	sysConfig.m_nActEndDay = st.wDay;

	DateTimeToSystemTime(timeActEnd->DateTime, st);
	sysConfig.m_nActEndHour = st.wHour;
	sysConfig.m_nActEndMin = st.wMinute;

	for (int i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
	{
		DateTimeToSystemTime(m_timeStart[i]->DateTime, st);
		sysConfig.m_nActStartMinuteInDay[i] = st.wHour * 60 + st.wMinute;
		sysConfig.m_nActStartSecondInDay[i] = st.wSecond;
		DateTimeToSystemTime(m_timeEnd[i]->DateTime, st);
		sysConfig.m_nActEndMinuteInDay[i] = st.wHour * 60 + st.wMinute;
		sysConfig.m_nActEndSecondInDay[i] = st.wSecond;
	}

	sysConfig.m_nActStartWeek = cbStartWeek->ItemIndex;
	sysConfig.m_nActEndWeek = cbEndWeek->ItemIndex;
	sysConfig.m_nActRefIndex = cbRefActIndex->ItemIndex;

	sysConfig.SaveAct(m_dtu, m_nActIndex);
	CreateTxData1();
}
//---------------------------------------------------------------------------

void TfrmMain::CreateTxData1()
{
#ifndef AUTO_SIZE_TEXT
	CreateTxData();
#else
	CDtu* m_dtuOld = m_dtu;

	PVirtualNode Node = vt->GetFirst();
	while (Node != NULL)
	{
		m_dtu = (CDtu*)(vt->GetNodeData(Node));
		if (m_dtu->m_nNodeType == NODE_CARD)
			m_dtu->m_bTxDataCreated = false;
		Node = vt->GetNext(Node);
	}

	for (;;)
	{
		bool bFound = false;
		Node = vt->GetFirst();
		while (Node != NULL)
		{
			m_dtu = (CDtu*)(vt->GetNodeData(Node));
			if (m_dtu->m_nNodeType == NODE_CARD)
			{
				if (!m_dtu->m_bTxDataCreated)
				{
					bFound = true;
					sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea]  = m_dtu->m_nSubAreaWidth[sysConfig.m_nActArea];
					sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] = m_dtu->m_nSubAreaHeight[sysConfig.m_nActArea];
					break;
				}
			}
			Node = vt->GetNext(Node);
		}

		if (!bFound)
			break;

		if (sysConfig.m_bDotFont)
		{
			if (sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] == 16)
				sysConfig.m_nDotFontSize = 16;
			else if (sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] == 24)
				sysConfig.m_nDotFontSize = 24;
			else if (sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] == 32)
				sysConfig.m_nDotFontSize = 32;
			else
			{
				sysConfig.m_bDotFont = false;
				sysConfig.m_sActFontName = "宋体";
				sysConfig.m_nActFontSize = -sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] * 72 / GetDeviceCaps(Canvas->Handle, LOGPIXELSY);
				sysConfig.m_nActFontStyle = 0;
			}
		}
		else
			sysConfig.m_nActFontSize = -sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] * 72 / GetDeviceCaps(Canvas->Handle, LOGPIXELSY);

		CreateTxData();
		while (Node != NULL)
		{
			m_dtu = (CDtu*)(vt->GetNodeData(Node));
			if (m_dtu->m_nNodeType == NODE_CARD)
			{
				if ((!m_dtu->m_bTxDataCreated) &&
					(sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea] == m_dtu->m_nSubAreaWidth[sysConfig.m_nActArea]) &&
					(sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] == m_dtu->m_nSubAreaHeight[sysConfig.m_nActArea]))
				{
					m_dtu->CopyTxData(m_nActIndex);
					m_dtu->m_bTxDataCreated = true;
				}
			}
			Node = vt->GetNext(Node);
		}
	}
	m_dtu = m_dtuOld;
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuSelectAllActClick(TObject *Sender)
{
	for (int i = 0; i < listActs->Items->Count; i++)
	{
		TListItem* itemAct = listActs->Items->Item[i];
		itemAct->Checked = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuCancelAllActClick(TObject *Sender)
{
	for (int i = 0; i < listActs->Items->Count; i++)
	{
		TListItem* itemAct = listActs->Items->Item[i];
		itemAct->Checked = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuSelectAllCardClick(TObject *Sender)
{
	PVirtualNode Node = vt->GetFirst();
	while (Node != NULL)
	{
		Node->CheckState = csCheckedNormal;
		vt->InvalidateNode(Node);
		Node = vt->GetNext(Node);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuCancelAllCardClick(TObject *Sender)
{
	PVirtualNode Node = vt->GetFirst();
	while (Node != NULL)
	{
		Node->CheckState = csUncheckedNormal;
		vt->InvalidateNode(Node);
		Node = vt->GetNext(Node);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::editActDescChange(TObject *Sender)
{
	if (m_dtu == NULL)
		return;

	if (listActs->Items->Count == sysConfig.m_nMaxActCount)	//防止初始化错误
	{
		listActs->Items->Item[m_nActIndex]->SubItems->Strings[1] = editActDesc->Text;
		wndTextEdit->m_bDirty = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuAboutClick(TObject *Sender)
{
	TfrmAbout *frmAbout = new TfrmAbout(this);
	frmAbout->ShowModal();
	delete frmAbout;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::listActsSelectItem(TObject *Sender,
	  TListItem *Item, bool Selected)
{
	if (Selected)
	{
		comboEditAct->ItemIndex = Item->Index;
		comboEditActChange(this);
		comboAreaChange(this);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuUpdateProgClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	if (prog == NULL)
	{
		Application->MessageBox(L"暂不支持该类型控制卡的软件升级。请与生产厂家取得联系。", L"软件升级", MB_OK | MB_ICONWARNING);
		return;
	}

	if (sysConfig.m_nCardType == CARD_TYPE_GS7020BQ)
	{
		if (Application->MessageBox(L"本程序仅支持对GS7020HQ升级。请确认要升级的控制卡类型。您确定继续吗?", L"软件升级", MB_YESNO | MB_ICONQUESTION) != IDYES)
		return;
	}

	if (m_nLanguage == LANGUAGE_CHINESE)
	{
		if (mnuUpdateProg->Caption == "停止升级(&T)")
		{
			m_bStopUpdateProg = true;
			return;
		}
	}
	else
	{
		if (mnuUpdateProg->Caption == "Stop &Updating Software")
		{
			m_bStopUpdateProg = true;
			return;
		}
	}

	int nSelected = GetCheckedCardCount();
	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请先选择要进行软件升级的显示屏(左侧复选框打勾)", L"软件升级", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Update Software", MB_OK | MB_ICONWARNING);
		return;
	}

	m_bStopUpdateProg = false;
	if (m_nLanguage == LANGUAGE_CHINESE)
	{
		if (Application->MessageBox(L"您确定在软件升级前已经与生产厂家取得联系,以确认您的控制卡硬件与使用的软件是兼容的. 升级不兼容的硬件将会导致控制卡不能工作。您确定继续吗?", L"软件升级", MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;

		if (Application->MessageBox(L"软件升级后部分或全部节目的显示内容可能会丢失。您确定继续吗?", L"软件升级", MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;

		if (!OpenCommPort(0, "升级控制卡软件"))
			return;

		if (sysConfig.m_nCardType == CARD_TYPE_GS2012B)
			sysConfig.m_nMaxPackSize = 512;
		mnuUpdateProg->Caption = "停止升级(&T)";
		StartOperation();
		timer_UpdateProg->Enabled = true;
	}
	else
	{
		if (Application->MessageBox(L"Please contact the product vendor to confirm your software if the conrrent version. Incorrect software version will make the card dead. Do you want to continue?", L"Update Software", MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;

		if (Application->MessageBox(L"After updating software, some acts may be absent. Do you want to continue?", L"Update Software", MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;

		if (!OpenCommPort(0, "Update Software"))
			return;

		if (sysConfig.m_nCardType == CARD_TYPE_GS2012B)
			sysConfig.m_nMaxPackSize = 512;
		mnuUpdateProg->Caption = "Stop &Updating Software";
		StartOperation();
		timer_UpdateProg->Enabled = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timer_UpdateProgTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (m_bStopUpdateProg)
		{
			if (m_nLanguage == LANGUAGE_CHINESE)
				dtu->m_sStatus= "升级中止";
			else
				dtu->m_sStatus= "Updating Stoppped";
			dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
		}

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_UpdateProg(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (dtu->m_nTxStep == 0)
				{
					if (WaitForCmdEcho(MMI_CMD_STOP_DISPLAY, dtu, 0))
					{
						dtu->m_nRetries = 0;
						if (m_nLanguage == LANGUAGE_CHINESE)
							dtu->m_sStatus= "已暂停显示,准备发送升级代码";
						else
							dtu->m_sStatus= "Act Playing Stopped, Preparing Sending Software Code";
						vt->InvalidateNode(Node);
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 1;
						m_ComPort.Close();
					}
				}
				else if (dtu->m_nTxStep == 1)
				{
					if (WaitForCmdEcho((sysConfig.m_nCardType != CARD_TYPE_GS7020F) ? MMI_CMD_WRITE_FLASH : MMI_CMD_WRITE_FLASH_F, dtu, 0))
					{
						dtu->m_nRetries = 0;
						dtu->m_nTxStatus = TX_STATUS_READY;
						m_ComPort.Close();
						if (dtu->m_bWantVerifyAct)
							dtu->m_nTxStep = 2;
						else
						{
							if (dtu->m_nDataSent + sysConfig.m_nMaxPackSize >= g_nSizeofProg)
							{
								if (m_nLanguage == LANGUAGE_CHINESE)
									dtu->m_sStatus = "准备重新复位控制卡";
								else
									dtu->m_sStatus = "Preparing to Reset the Card";
								vt->InvalidateNode(Node);
								dtu->m_nTxStatus = TX_STATUS_READY;
								dtu->m_nTxStep = 3;
							}
							else
							{
								dtu->m_nTxStatus = TX_STATUS_READY;
								dtu->m_nDataSent += sysConfig.m_nMaxPackSize;
								int nPercent = dtu->m_nDataSent * 100 / (g_nSizeofProg + sysConfig.m_nMaxPackSize);
								dtu->m_sStatus = UnicodeString(nPercent) + "%";
								vt->InvalidateNode(Node);
								dtu->m_nTxStep = 1;
							}
						}
					}
				}
				else if (dtu->m_nTxStep == 2)
				{
					int nLen = ((g_nSizeofProg - dtu->m_nDataSent) > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize : (g_nSizeofProg - dtu->m_nDataSent);
					if (WaitForCmdEcho(MMI_CMD_READ_FLASH, dtu, nLen + 6))
					{
						m_ComPort.Close();
						dtu->m_nRetries = 0;
						if (dtu->VerifyProg())
						{
							if (dtu->m_nDataSent + sysConfig.m_nMaxPackSize >= g_nSizeofProg)
							{
								if (m_nLanguage == LANGUAGE_CHINESE)
									dtu->m_sStatus = "准备重新复位控制卡";
								else
									dtu->m_sStatus = "Preparing to Reset the Card";
								vt->InvalidateNode(Node);
								dtu->m_nTxStatus = TX_STATUS_READY;
								dtu->m_nTxStep = 3;
							}
							else
							{
								dtu->m_nTxStatus = TX_STATUS_READY;
								dtu->m_nDataSent += sysConfig.m_nMaxPackSize;
								int nPercent = dtu->m_nDataSent * 100 / (g_nSizeofProg + sysConfig.m_nMaxPackSize);
								dtu->m_sStatus = UnicodeString(nPercent) + "%";
								vt->InvalidateNode(Node);
								dtu->m_nTxStep = 1;
							}
						}
						else
						{
							dtu->m_nDataSent &= 0xFFFFF000;		//4096
							dtu->m_nTxStatus = TX_STATUS_READY;
							dtu->m_nTxStep = 1;
						}
					}
				}
				else
				{
					if (WaitForCmdEcho((sysConfig.m_nCardType != CARD_TYPE_GS7020F) ? MMI_CMD_IAP_START : MMI_CMD_IAP_START_F, dtu, 0))
					{
						dtu->m_nRetries = 0;
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
						m_ComPort.Close();
					}
				}

				if (dtu->m_nTxStatus == TX_STATUS_WAIT_ECHO)
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_UpdateProg(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_UpdateProg(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (dtu->m_nTxStep == 0)
				{
					if (WaitForCmdEcho(MMI_CMD_STOP_DISPLAY, dtu, 0))
					{
						dtu->m_nRetries = 0;
						if (m_nLanguage == LANGUAGE_CHINESE)
							dtu->m_sStatus= "已暂停显示,准备发送升级代码";
						else
							dtu->m_sStatus= "Act Playing Stopped, Preparing Sending Software Code";
						vt->InvalidateNode(Node);
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 1;
					}
				}
				else if (dtu->m_nTxStep == 1)
				{
					if (WaitForCmdEcho((sysConfig.m_nCardType != CARD_TYPE_GS7020F) ? MMI_CMD_WRITE_FLASH : MMI_CMD_WRITE_FLASH_F, dtu, 0))
					{
						dtu->m_nRetries = 0;
						dtu->m_nTxStatus = TX_STATUS_READY;
						if (dtu->m_bWantVerifyAct)
							dtu->m_nTxStep = 2;
						else
						{
							if (dtu->m_nDataSent + sysConfig.m_nMaxPackSize >= g_nSizeofProg)
							{
								if (m_nLanguage == LANGUAGE_CHINESE)
									dtu->m_sStatus = "准备重新复位控制卡";
								else
									dtu->m_sStatus = "Preparing to Reset the Card";
								vt->InvalidateNode(Node);
								dtu->m_nTxStatus = TX_STATUS_READY;
								dtu->m_nTxStep = 3;
							}
							else
							{
								dtu->m_nTxStatus = TX_STATUS_READY;
								dtu->m_nDataSent += sysConfig.m_nMaxPackSize;
								int nPercent = dtu->m_nDataSent * 100 / (g_nSizeofProg + sysConfig.m_nMaxPackSize);
								dtu->m_sStatus = UnicodeString(nPercent) + "%";
								vt->InvalidateNode(Node);
								dtu->m_nTxStep = 1;
							}
						}
					}
				}
				else if (dtu->m_nTxStep == 2)
				{
					int nLen = ((g_nSizeofProg - dtu->m_nDataSent) > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize : (g_nSizeofProg - dtu->m_nDataSent);
					if (WaitForCmdEcho(MMI_CMD_READ_FLASH, dtu, nLen + 6))
					{
						dtu->m_nRetries = 0;
						if (dtu->VerifyProg())
						{
							if (dtu->m_nDataSent + sysConfig.m_nMaxPackSize >= g_nSizeofProg)
							{
								if (m_nLanguage == LANGUAGE_CHINESE)
									dtu->m_sStatus = "准备重新复位控制卡";
								else
									dtu->m_sStatus = "Preparing to Reset the Card";
								vt->InvalidateNode(Node);
								dtu->m_nTxStatus = TX_STATUS_READY;
								dtu->m_nTxStep = 3;
							}
							else
							{
								dtu->m_nTxStatus = TX_STATUS_READY;
								dtu->m_nDataSent += sysConfig.m_nMaxPackSize;
								int nPercent = dtu->m_nDataSent * 100 / (g_nSizeofProg + sysConfig.m_nMaxPackSize);
								dtu->m_sStatus = UnicodeString(nPercent) + "%";
								vt->InvalidateNode(Node);
								dtu->m_nTxStep = 1;
							}
						}
						else
						{
							dtu->m_nDataSent &= 0xFFFFF000;		//4096
							dtu->m_nTxStatus = TX_STATUS_READY;
							dtu->m_nTxStep = 1;
						}
					}
				}
				else
				{
					if (WaitForCmdEcho(MMI_CMD_IAP_START, dtu, 0))
					{
						dtu->m_nRetries = 0;
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
					}
				}

				if (dtu->m_nTxStatus == TX_STATUS_WAIT_ECHO)
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_UpdateProg(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
							nSendDtuCount++;
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timer_UpdateProg->Enabled = false;
		EndOperation();

		if (sysConfig.m_nCardType == CARD_TYPE_GS2012B)
			sysConfig.m_nMaxPackSize = 1024;
		if (m_nLanguage == LANGUAGE_CHINESE)
			mnuUpdateProg->Caption = "控制卡软件升级(&U)";
		else
			mnuUpdateProg->Caption = "&Update Software";
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_UpdateProg(CDtu* dtu)
{
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		SendCommand_UpdateProg_HEB(dtu);
	else
		SendCommand_UpdateProg_Normal(dtu);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_UpdateProg_Normal(CDtu* dtu)
{
	BYTE buf[2048];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	DWORD* pStartPos = (DWORD*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	if (dtu->m_nTxStep == 0)
	{
		pHead->ucCmd = MMI_CMD_STOP_DISPLAY;
		pHead->usLen = 0;
	}
	else if (dtu->m_nTxStep == 1)
	{
		int nLeftDataLen = g_nSizeofProg - dtu->m_nDataSent;
		pHead->ucCmd =  (sysConfig.m_nCardType != CARD_TYPE_GS7020F) ? MMI_CMD_WRITE_FLASH : MMI_CMD_WRITE_FLASH_F;
		pHead->usLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize + 4 : nLeftDataLen + 4;
#ifndef GS7025H
		if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
			*pStartPos = 256 * 1024 + dtu->m_nDataSent;
		else
#endif
			*pStartPos = 1024 * 1024 + dtu->m_nDataSent;
		memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], prog + dtu->m_nDataSent, pHead->usLen - 4);
	}
	else if (dtu->m_nTxStep == 2)
	{
		int nLeftDataLen = g_nSizeofProg - dtu->m_nDataSent;
		pHead->ucCmd = MMI_CMD_READ_FLASH;
		pHead->usLen = 6;
#ifndef GS7025H
		if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
			*pStartPos = 256 * 1024 + dtu->m_nDataSent;
		else
#endif
			*pStartPos = 1024 * 1024 + dtu->m_nDataSent;
		WORD* pusLen = (WORD*)(pStartPos + 1);
		*pusLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize : nLeftDataLen;
	}
	else
	{
		pHead->ucCmd = (sysConfig.m_nCardType != CARD_TYPE_GS7020F) ? MMI_CMD_IAP_START : MMI_CMD_IAP_START_F;
		pHead->usLen = 0;
	}

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_UpdateProg_HEB(CDtu* dtu)
{
	BYTE buf[2048];
	MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;
	DWORD* pStartPos = (DWORD*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	if (dtu->m_nTxStep == 0)
	{
		pHead->ucCmd = MMI_CMD_STOP_DISPLAY;
		pHead->usLen = 0;
	}
	else if (dtu->m_nTxStep == 1)
	{
		int nLeftDataLen = g_nSizeofProg - dtu->m_nDataSent;
		pHead->ucCmd =  MMI_CMD_WRITE_FLASH;
		pHead->usLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize + 4 : nLeftDataLen + 4;
		*pStartPos = 1024 * 1024 + dtu->m_nDataSent;
		memcpy(&buf[sizeof(MMI_CMD_HEAD_HEB) + sizeof(DWORD)], prog + dtu->m_nDataSent, pHead->usLen - 4);
	}
	else if (dtu->m_nTxStep == 2)
	{
		int nLeftDataLen = g_nSizeofProg - dtu->m_nDataSent;
		pHead->ucCmd = MMI_CMD_READ_FLASH;
		pHead->usLen = 6;
		*pStartPos = 1024 * 1024 + dtu->m_nDataSent;
		WORD* pusLen = (WORD*)(pStartPos + 1);
		*pusLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize : nLeftDataLen;
	}
	else
	{
		pHead->ucCmd = MMI_CMD_IAP_START;
		pHead->usLen = 0;
	}

	pHead->ucKey1 = dtu->m_ucKey1 = GetTickCount();
	pHead->ucKey2 = dtu->m_ucKey2 = GetTickCount() >> 8;
	buf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, buf);
	Encrypt(dtu, pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);

	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
}
//---------------------------------------------------------------------------

void TfrmMain::SendData_UDP(CDtu* dtu, int nLen, BYTE* p)
{
	if (sysConfig.m_nCardType == CARD_TYPE_YL2010A_GPS_JTX)
		SendData_UDP_JTX(dtu, nLen, p);
	else
		SendData_UDP_Yuepoch(dtu, nLen, p);
}
//---------------------------------------------------------------------------

void TfrmMain::SendData_UDP_Yuepoch(CDtu* dtu, int nLen, BYTE* p)
{
#pragma pack(1)
	typedef struct
	{
		char szUserName[12];
		BYTE ucFrameType;
		char szPassword[16];
		char szDtuCode[12];
	}TX_DATA_HEADER;
#pragma pack()
	BYTE buf[2000];
	size_t i;

	if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
	{
		TIdBytes ar;
		ar.set_length(nLen);
		for (i = 0; i < nLen; i++)
			ar[i] = p[i];
		udpSocket->SendBuffer(dtu->m_sLocalIp, 3700, ar);
	}
	else
	{
		TX_DATA_HEADER* pHeader = (TX_DATA_HEADER*)buf;
		::strcpy(pHeader->szUserName, AnsiString(sysConfig.m_sUserName).c_str());
		for (i = 0; i < ::strlen(pHeader->szUserName); i++)
			pHeader->szUserName[i] ^= 0xAA;
		::strcpy(pHeader->szPassword, AnsiString(sysConfig.m_sPassword).c_str());
		for (i = 0; i < ::strlen(pHeader->szPassword); i++)
			pHeader->szPassword[i] ^= 0xAA;
		::strncpy(pHeader->szDtuCode, dtu->m_sCode.c_str(), 11);
		pHeader->szDtuCode[11] = 0;
		pHeader->ucFrameType = FT_DATA;
		memcpy(buf + sizeof(TX_DATA_HEADER), p, nLen);

		TIdBytes ar;
		ar.set_length(nLen + sizeof(TX_DATA_HEADER));
		for (i = 0; i < nLen + sizeof(TX_DATA_HEADER); i++)
			ar[i] = buf[i];
		udpSocket->SendBuffer(ar);
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendData_UDP_JTX(CDtu* dtu, int nLen, BYTE* p)
{
#pragma pack(1)
	typedef struct
	{
		BYTE ucHead[2];
		BYTE ucMainCmd;
		BYTE ucPackLen_H;
		BYTE ucPackLen_L;
		BYTE ucPseudoIp[4];
		BYTE ucComPort;
	}TX_DATA_HEADER;
#pragma pack()
	BYTE buf[2000];
	size_t i;

	TX_DATA_HEADER* pHeader = (TX_DATA_HEADER*)buf;
	pHeader->ucHead[0] = 0x26;
	pHeader->ucHead[1] = 0x26;
	pHeader->ucMainCmd = 0x69;
	pHeader->ucPackLen_H = (((WORD)(nLen + 7)) >> 8);
	pHeader->ucPackLen_L = (((WORD)(nLen + 7)) & 0xFF);
	GetPseudoIp(dtu->m_sCode, pHeader->ucPseudoIp);
	pHeader->ucComPort = 5;
	memcpy(buf + sizeof(TX_DATA_HEADER), p, nLen);
	buf[sizeof(TX_DATA_HEADER) + nLen] = CalcCheckSum_JTX(buf, sizeof(TX_DATA_HEADER) + nLen);
	buf[sizeof(TX_DATA_HEADER) + nLen + 1] = 0x0A;

	TIdBytes ar;
	ar.set_length(nLen + sizeof(TX_DATA_HEADER) + 2);
	for (i = 0; i < nLen + sizeof(TX_DATA_HEADER) + 2; i++)
		ar[i] = buf[i];
	udpSocket->SendBuffer(ar);
}
//---------------------------------------------------------------------------

BYTE TfrmMain::CalcCheckSum_JTX(BYTE* p, int nLen)
{
	BYTE uc = 0;

	while (nLen--)
		uc ^= *p++;

	return uc;
}
//---------------------------------------------------------------------------

void TfrmMain::ReceiveUdpData()
{
#pragma pack(1)
	struct
	{
		char  szGsmCode[12];
		BYTE  ucFrameType;
	}stHead;
#pragma pack()
	TIdBytes buf;
	BYTE* p;
	int nLen, i, j;
	CDtu* dtu;
	buf.set_length(2048);

	if (!udpSocket->Active)
		return;

	//if (!udpSocket->Connected())
	//	return;

	if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
	{
		UnicodeString sPeerIp;
		WORD usPeerPort;

		for (;;)
		{
			try
			{
				nLen = udpSocket->ReceiveBuffer(buf, sPeerIp, usPeerPort, 0);
			}
			catch (Exception &ex)
			{
				break;
			}

			if (nLen <= 0)
				break;

			PVirtualNode Node = vt->GetFirstChecked();
			while (Node != NULL)
			{
				dtu = (CDtu*)(vt->GetNodeData(Node));
				if (dtu->m_nNodeType == NODE_CARD)
				{
					if ((sPeerIp == dtu->m_sLocalIp))
					{
						if (nLen <= (int)sizeof(dtu->m_ucRxBuf))
						{
							dtu->m_nRxLen = nLen;
							for (j = 0; j < nLen; j++)
								dtu->m_ucRxBuf[j] = buf[j];
						}
						break;
					}
				}
				Node = vt->GetNextChecked(Node);
			}
		}
	}
	else
	{
		for (;;)
		{
			try
			{
				nLen = udpSocket->ReceiveBuffer(buf, 0);
			}
			catch (Exception &ex)
			{
				break;
			}

			if (nLen <= 0)
				break;

			if (nLen <= (int)sizeof(stHead))
				continue;

			p = (BYTE*)&stHead;
			for (i = 0; i < (int)sizeof(stHead); i++)
				*p++ = buf[i];

			if (stHead.ucFrameType != FT_DATA)
				continue;

			size_t nStrLen = 0;
			for (i = 0; i < 12; i++)
			{
				if (stHead.szGsmCode[i])
					nStrLen++;
				else
					break;
			}
			if (nStrLen > sizeof(stHead.szGsmCode) - 1)
				continue;

			PVirtualNode Node = vt->GetFirstChecked();
			while (Node != NULL)
			{
				dtu = (CDtu*)(vt->GetNodeData(Node));
				if (dtu->m_nNodeType == NODE_CARD)
				{
					if (::strcmp(dtu->m_sCode.c_str(), stHead.szGsmCode) == 0)
					{
						nLen -=  sizeof(stHead);

						if (nLen <= (int)sizeof(dtu->m_ucRxBuf))
						{
							dtu->m_nRxLen = nLen;
							for (j = 0; j < nLen; j++)
								dtu->m_ucRxBuf[j] = buf[j + sizeof(stHead)];
						}
						break;
					}
				}
				Node = vt->GetNextChecked(Node);
			}
		}
	}
}

void __fastcall TfrmMain::mnuCheckCardOnlineClick(TObject *Sender)
{
	int nSelected = GetCheckedCardCount();

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏(复选框打勾)", L"检查在线状态", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Check Online Status", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "检查在线状态" : "Check Online Status"))
		return;

	::Sleep(20);

	StartOperation();

	timerCheckOnline->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerCheckOnlineTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
	{
		if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
			ReceiveUdpData();
		else
			ReceiveUdpStatusPack();
	}

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_CheckOnline(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_ECHO, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
					m_ComPort.Close();
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
						dtu->m_sStatus = "-";
						vt->InvalidateNode(Node);
						m_ComPort.Close();
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME / 2)
				{
					SendCommand_CheckOnline(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
				{
					if (WaitForCmdEcho(MMI_CMD_ECHO, dtu, 0))
					{
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
						nEnd++;
					}
					else
					{
						dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
						if (dwTime >= 100)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							dtu->m_sStatus = "-";
							//vt->InvalidateNode(Node);
							nEnd++;
						}
					}
				}
				else
				{
					if (dtu->m_nTxStep == 1)	//在线
					{
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
						nEnd++;
					}
					else if (dtu->m_nTxStep == 2) //不在线
					{
						dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
						dtu->m_sStatus = "-";
						vt->InvalidateNode(Node);
						nEnd++;
					}
					else
					{
						dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
						//if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
						if (dwTime > (DWORD)1000)
						{
							if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
							{
								dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
								dtu->m_sStatus = "-";
								vt->InvalidateNode(Node);
								nEnd++;
							}
							else
							{
								if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
								{
									dtu->m_nRetries++;
									SendCommand_CheckOnline(dtu);
									dtu->m_dwTxStartTime = ::GetTickCount();
									nSendDtuCount++;
								}
							}
						}
					}
				}
				break;
//			case TX_STATUS_END_OK:
//				nEnd++;
//				break;
//			case TX_STATUS_END_TIME_OUT:
//				nEnd++;
//				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timerCheckOnline->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_CheckOnline(CDtu* dtu)
{
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		SendCommand_CheckOnline_HEB(dtu);
	else
		SendCommand_CheckOnline_Normal(dtu);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_CheckOnline_Normal(CDtu* dtu)
{
	if (sysConfig.m_nCommMethod == 0)
	{
		BYTE buf[32];
		MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

		pHead->usHeadTag = MMI_START_ID;
		pHead->ucCardId = dtu->GetCardId();
		pHead->ucCmd = MMI_CMD_ECHO;
		pHead->usLen = 0;

		buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	}
	else
	{
		if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
		{
			BYTE buf[32];
			MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

			pHead->usHeadTag = MMI_START_ID;
			pHead->ucCardId = dtu->GetCardId();
			pHead->ucCmd = MMI_CMD_ECHO;
			pHead->usLen = 0;

			buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);

			int nLen = pHead->usLen + sizeof(MMI_CMD_HEAD) + 1;
			TIdBytes ar;
			ar.set_length(nLen);
			for (int i = 0; i < nLen; i++)
				ar[i] = buf[i];

			try
			{
				udpSocket->SendBuffer(dtu->m_sLocalIp, 3700, ar);
			}
			catch (Exception &ex)
			{
			}
			return;
		}

#pragma pack(1)
		struct
		{
			char szUserName[12];
			BYTE ucFrameType;
			char szPassword[16];
			char szDtuCode[12];
		}stData;
#pragma pack()
		size_t i;

		// 发送请求状态报文
		::strcpy(stData.szUserName, AnsiString(sysConfig.m_sUserName).c_str());
		for (i = 0; i < strlen(stData.szUserName); i++)
			stData.szUserName[i] ^= 0xAA;
		::strcpy(stData.szPassword, AnsiString(sysConfig.m_sPassword).c_str());
		for (i = 0; i < strlen(stData.szPassword); i++)
			stData.szPassword[i] ^= 0xAA;
		stData.ucFrameType = FT_STATUS;
		::strcpy(stData.szDtuCode, dtu->m_sCode.c_str());

		TIdBytes ar;
		ar.set_length(sizeof(stData));
		BYTE* p = (BYTE*)&stData;
		for (i = 0; i < sizeof(stData); i++)
			ar[i] = *p++;
		try
		{
			udpSocket->SendBuffer(ar);
		}
		catch (Exception &ex)
		{
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_CheckOnline_HEB(CDtu* dtu)
{
	if (sysConfig.m_nCommMethod == 0)
	{
		BYTE buf[32];
		MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;

		pHead->usHeadTag = MMI_START_ID;
		pHead->ucCardId = dtu->GetCardId();
		pHead->ucCmd = MMI_CMD_ECHO;
		pHead->usLen = 0;
		pHead->ucKey1 = dtu->m_ucKey1 = GetTickCount();
		pHead->ucKey2 = dtu->m_ucKey2 = GetTickCount() >> 8;
		buf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, buf);
		Encrypt(dtu, pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
	}
	else
	{
#pragma pack(1)
		struct
		{
			char szUserName[12];
			BYTE ucFrameType;
			char szPassword[16];
			char szDtuCode[12];
		}stData;
#pragma pack()
		size_t i;

		// 发送请求状态报文
		::strcpy(stData.szUserName, AnsiString(sysConfig.m_sUserName).c_str());
		for (i = 0; i < strlen(stData.szUserName); i++)
			stData.szUserName[i] ^= 0xAA;
		::strcpy(stData.szPassword, AnsiString(sysConfig.m_sPassword).c_str());
		for (i = 0; i < strlen(stData.szPassword); i++)
			stData.szPassword[i] ^= 0xAA;
		stData.ucFrameType = FT_STATUS;
		::strcpy(stData.szDtuCode, dtu->m_sCode.c_str());

		TIdBytes ar;
		ar.set_length(sizeof(stData));
		BYTE* p = (BYTE*)&stData;
		for (i = 0; i < sizeof(stData); i++)
			ar[i] = *p++;
		try
		{
			udpSocket->SendBuffer(ar);
		}
		catch (Exception &ex)
		{
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::ReceiveUdpStatusPack()
{
#pragma pack(1)
	struct
	{
		char szGsmCode[12];
		BYTE ucFrameType;
		char cStatus;
	}stHead;
#pragma pack()
	TIdBytes buf;
	BYTE* p;
	int nLen, i;
	CDtu* dtu;
	buf.set_length(2048);

	if (!udpSocket->Active)
		return;

//	if (!udpSocket->Connected())
//		return;

	for (;;)
	{
		try
		{
			nLen = udpSocket->ReceiveBuffer(buf, 0);
		}
		catch (Exception &ex)
		{
			break;
		}

		if (nLen <= 0)
			break;

		if (nLen < (int)sizeof(stHead))
			continue;

		p = (BYTE*)&stHead;
		for (i = 0; i < (int)sizeof(stHead); i++)
			*p++ = buf[i];

		if (stHead.ucFrameType != FT_STATUS && stHead.ucFrameType != FT_GPS)
			continue;

		size_t nStrLen = 0;
		for (i = 0; i < 12; i++)
		{
			if (stHead.szGsmCode[i])
				nStrLen++;
			else
				break;
		}
		if (nStrLen > sizeof(stHead.szGsmCode) - 1)
			continue;

		PVirtualNode Node = vt->GetFirst();
		while (Node != NULL)
		{
			dtu = (CDtu*)(vt->GetNodeData(Node));
			if (dtu->m_nNodeType == NODE_CARD)
			{
				if (dtu->m_nTxStatus == TX_STATUS_WAIT_ECHO)
				{
					if (::strcmp(dtu->m_sCode.c_str(), stHead.szGsmCode) == 0)
					{
						dtu->m_nTxStep = stHead.cStatus ? 1 : 2;
						if (stHead.ucFrameType == FT_GPS)
						{
							for (i = 0; i < 18; i++)
								dtu->m_ucRxBuf[i] = buf[i + (int)sizeof(stHead)];
						}
						break;
					}
				}
			}
			Node = vt->GetNext(Node);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuAdjustCardClockClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	int nSelected = GetCheckedCardCount();

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"校对控制卡时钟", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Adjust Card's Clock", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "校对控制卡时钟" : "Adjust Card's Clock"))
		return;

	::Sleep(20);

	StartOperation();

	timerWriteClock->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerWriteClockTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_WriteClock(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_WRITE_CLOCK, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
					m_ComPort.Close();
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_WriteClock(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_WriteClock(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_WRITE_CLOCK, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
							{
								dtu->m_nRetries++;
								SendCommand_WriteClock(dtu);
								dtu->m_dwTxStartTime = ::GetTickCount();
								nSendDtuCount++;
							}
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timerWriteClock->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WriteClock(CDtu* dtu)
{
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		SendCommand_WriteClock_HEB(dtu);
	else
		SendCommand_WriteClock_Normal(dtu);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WriteClock_Normal(CDtu* dtu)
{
	BYTE buf[64];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	BYTE* p = (BYTE*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_CMD_WRITE_CLOCK;
	pHead->usLen = 9;

	SYSTEMTIME st;
	for (;;)
	{
		::GetLocalTime(&st);
		if (st.wMilliseconds < 50)
			break;
	}

	*((WORD*)p) = st.wYear;
	p++; p++;
	*p++ = st.wMonth;
	*p++ = st.wDay;
	*p++ = st.wHour;
	*p++ = st.wMinute;
	*p++ = st.wSecond;
	*p++ = st.wDayOfWeek;
	*p = 0;

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WriteClock_HEB(CDtu* dtu)
{
	BYTE buf[64];
	MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;
	BYTE* p = (BYTE*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_CMD_WRITE_CLOCK;
	pHead->usLen = 9;

	SYSTEMTIME st;
	for (;;)
	{
		::GetLocalTime(&st);
		if (st.wMilliseconds < 50)
			break;
	}

	*((WORD*)p) = st.wYear;
	p++; p++;
	*p++ = st.wMonth;
	*p++ = st.wDay;
	*p++ = st.wHour;
	*p++ = st.wMinute;
	*p++ = st.wSecond;
	*p++ = st.wDayOfWeek;
	*p = 0;

	pHead->ucKey1 = dtu->m_ucKey1 = GetTickCount();
	pHead->ucKey2 = dtu->m_ucKey2 = GetTickCount() >> 8;
	buf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, buf);
	Encrypt(dtu, pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);

	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuReadCardClockClick(TObject *Sender)
{
	int nSelected = GetCheckedCardCount();

#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"读取控制卡时钟", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Query Clock of Card", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "读取控制卡时钟" : "Query Clock of Card"))
		return;

	::Sleep(20);

	StartOperation();
	timerReadClock->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerReadClockTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_ReadClock(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_READ_CLOCK, dtu, 9))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					UpdateCardClock(dtu);
					vt->InvalidateNode(Node);
					m_ComPort.Close();
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_ReadClock(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_ReadClock(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_READ_CLOCK, dtu, 9))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					UpdateCardClock(dtu);
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					DWORD nMaxTime = (DWORD)sysConfig.m_nTimeout_Network * 1000;
					if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
						nMaxTime = 100;
					if (dwTime > nMaxTime)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
							{
								dtu->m_nRetries++;
								SendCommand_ReadClock(dtu);
								dtu->m_dwTxStartTime = ::GetTickCount();
								nSendDtuCount++;
							}
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timerReadClock->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::UpdateCardClock(CDtu* dtu)
{
	BYTE* p;

	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		 p = dtu->m_ucRxBuf + sizeof(MMI_CMD_HEAD_HEB);
	else
		 p = dtu->m_ucRxBuf + sizeof(MMI_CMD_HEAD);

	WORD usYear = *((WORD*)p); p++; p++;
	BYTE ucMonth = *p++;
	BYTE ucDay = *p++;
	BYTE ucHour = *p++;
	BYTE ucMinute = *p++;
	BYTE ucSecond = *p++;
	BYTE ucWeek = *p;

	char s[128];
	if (m_nLanguage == LANGUAGE_CHINESE)
	{
		sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d 星期", usYear, ucMonth, ucDay,ucHour, ucMinute, ucSecond);
		char sWeek[] = "日一二三四五六";
		char s1[3];
		if (ucWeek < 7)
		{
			s1[0] = sWeek[ucWeek * 2];
			s1[1] = sWeek[ucWeek * 2 + 1];
			s1[2] = 0;
			strcat(s, s1);
		}
	}
	else
	{
		sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d ", usYear, ucMonth, ucDay,ucHour, ucMinute, ucSecond);
		char sWeek[][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
		if (ucWeek < 7)
			strcat(s, sWeek[ucWeek]);
	}

	dtu->m_sStatus = s;
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_ReadClock(CDtu* dtu)
{
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		SendCommand_ReadClock_HEB(dtu);
	else
		SendCommand_ReadClock_Normal(dtu);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_ReadClock_Normal(CDtu* dtu)
{
	BYTE buf[64];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_CMD_READ_CLOCK;
	pHead->usLen = 0;
	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);

	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_ReadClock_HEB(CDtu* dtu)
{
	BYTE buf[64];
	MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_CMD_READ_CLOCK;
	pHead->usLen = 0;
	pHead->ucKey1 = dtu->m_ucKey1 = GetTickCount();
	pHead->ucKey2 = dtu->m_ucKey2 = GetTickCount() >> 8;
	buf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, buf);
	Encrypt(dtu, pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);

	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuManualCloseScreenClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	int nSelected = GetCheckedCardCount();

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"手动关闭显示屏" , MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"please select at least one LED panel.", L"Close Display" , MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "手动关闭显示屏" : "Close Display"))
		return;

	::Sleep(20);

	StartOperation();
	timer_Execute->Tag = MMI_CMD_STOP_DISPLAY;
	timer_Execute->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuManualOpenScreenClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	int nSelected = GetCheckedCardCount();

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"手动打开显示屏", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"please select at least one LED panel.", L"Open Display", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "手动打开显示屏" : "Open Display"))
		return;

	::Sleep(20);

	StartOperation();
	timer_Execute->Tag = MMI_CMD_RESTART_DISPLAY;
	timer_Execute->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timer_ExecuteTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_OnOffScreen(timer_Execute->Tag, dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(timer_Execute->Tag, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
					m_ComPort.Close();
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_OnOffScreen(timer_Execute->Tag, dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_OnOffScreen(timer_Execute->Tag, dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(timer_Execute->Tag, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
							{
								dtu->m_nRetries++;
								SendCommand_OnOffScreen(timer_Execute->Tag, dtu);
								dtu->m_dwTxStartTime = ::GetTickCount();
								nSendDtuCount++;
							}
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timer_Execute->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_OnOffScreen(BYTE ucCmd, CDtu* dtu)
{
	BYTE buf[64];

	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
	{
		MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;

		pHead->usHeadTag = MMI_START_ID;
		pHead->ucCardId = dtu->GetCardId();
		pHead->ucCmd = ucCmd;
		pHead->usLen = 0;
		pHead->ucKey1 = dtu->m_ucKey1 = GetTickCount();
		pHead->ucKey2 = dtu->m_ucKey2 = GetTickCount() >> 8;
		buf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, buf);
		Encrypt(dtu, pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);

		if (sysConfig.m_nCommMethod == 0)
			m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
		else
			SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
	}
	else
	{
		MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

		pHead->usHeadTag = MMI_START_ID;
		pHead->ucCardId = dtu->GetCardId();
		pHead->ucCmd = ucCmd;
		pHead->usLen = 0;
		buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);

		if (sysConfig.m_nCommMethod == 0)
			m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
		else
			SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuSearchCardClick(TObject *Sender)
{
	TfrmSearchCard *frmSearchCard = new TfrmSearchCard(Application);
	frmSearchCard->editId->Text = m_sSearchCardId;
	frmSearchCard->editCode->Text = m_sSearchCardCode;
	frmSearchCard->editDesc->Text = m_sSearchCardDesc;

	if (frmSearchCard->ShowModal() != mrOk)
	{
		delete frmSearchCard;
		return;
	}

	m_sSearchCardId = frmSearchCard->editId->Text;
	m_sSearchCardCode = frmSearchCard->editCode->Text;
	m_sSearchCardDesc = frmSearchCard->editDesc->Text;

	FindNextCard();
	delete frmSearchCard;
}
//---------------------------------------------------------------------------

void TfrmMain::FindNextCard()
{
	PVirtualNode Node = vt->GetFirstSelected();
	if (Node == NULL)
		Node = vt->GetFirst();
	PVirtualNode NodeStart = Node;

	if (Node == NULL)
		return;

	while (Node != NULL)
	{
		Node = vt->GetNext(Node);
		if (Node == NULL)
			Node = vt->GetFirst();
		CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
		{
			if (m_sSearchCardId.Length())
			{
				if (dtu->m_nCardId == m_sSearchCardId.ToInt())
				{
					vt->IsVisible[Node] = true;
					vt->Selected[Node] = true;
					vt->FocusedNode = Node;
					break;
				}
			}

			if (m_sSearchCardCode.Length())
			{
				if (strstr(dtu->m_sCode.c_str(), m_sSearchCardCode.c_str()))
				{
					vt->IsVisible[Node] = true;
					vt->Selected[Node] = true;
					vt->FocusedNode = Node;
					break;
				}
			}

			if (m_sSearchCardDesc.Length())
			{
				if (wcsstr(dtu->m_sName.c_str(), m_sSearchCardDesc.c_str()))
				{
					vt->IsVisible[Node] = true;
					vt->Selected[Node] = true;
					vt->FocusedNode = Node;
					break;
				}
			}
		}

		if (Node == NodeStart)
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuSearchActClick(TObject *Sender)
{
	TfrmSearchAct *frmSearchAct = new TfrmSearchAct(Application);
	frmSearchAct->editActText->Text = m_sSearchActText;
	frmSearchAct->editActDesc->Text = m_sSearchActDesc;

	if (frmSearchAct->ShowModal() != mrOk)
	{
		delete frmSearchAct;
		return;
	}

	m_sSearchActText = frmSearchAct->editActText->Text;
	m_sSearchActDesc = frmSearchAct->editActDesc->Text;

	FindNextAct();
	delete frmSearchAct;
}
//---------------------------------------------------------------------------

void TfrmMain::FindNextAct()
{
	TListItem* itemAct;
	int n = 0;

	for (int i = 0; i < listActs->Items->Count; i++)
	{
		itemAct = listActs->Items->Item[i];
		if (itemAct->Selected)
		{
			n = i;
			break;
		}
	}

	n++;
	if (n >= listActs->Items->Count)
		n = 0;

	for (int i = 0; i < listActs->Items->Count; i++)
	{
		itemAct = listActs->Items->Item[n];
		if (m_sSearchActText.Length())
		{
			if (m_sSearchActText == "*")
			{
				if (sysConfig.GetActText(m_dtu, n).Length() || sysConfig.CheckActHasPicture(m_dtu, n))
				{
					itemAct->Selected = true;
					itemAct->MakeVisible(false);
					break;
				}
			}
			else
			{
				if (sysConfig.GetActText(m_dtu, n).Pos(m_sSearchActText) > 0)
				{
					itemAct->Selected = true;
					itemAct->MakeVisible(false);
					break;
				}
			}
		}

		if (m_sSearchActDesc.Length())
		{
			if (m_sSearchActDesc == "*")
			{
				if (sysConfig.GetActDesc(m_dtu, n).Length())
				{
					itemAct->Selected = true;
					itemAct->MakeVisible(false);
					break;
				}
			}
			else
			{
				if (sysConfig.GetActDesc(m_dtu, n).Pos(m_sSearchActDesc) > 0)
				{
					itemAct->Selected = true;
					itemAct->MakeVisible(false);
					break;
				}
			}
		}

		n++;
		if (n >= listActs->Items->Count)
			n = 0;
	}
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Direct()
{
	return sysConfig.m_nActShowTime;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_Direct()
{
	return sysConfig.m_nActExitSpeed;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_ScrollLeft()
{
	return sysConfig.m_nActEnterSpeed * imgScreen->Picture->Bitmap->Width + sysConfig.m_nActShowTime;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_MoveLeft()
{
	return sysConfig.m_nActEnterSpeed * sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea] + sysConfig.m_nActShowTime;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_MoveLeft()
{
	return sysConfig.m_nActExitSpeed * sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea];
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_ScrollRight()
{
	return GetDisplayTime_ScrollLeft();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_MoveRight()
{
	return GetDisplayTime_MoveLeft();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_MoveRight()
{
	return GetDisplayTime_Exit_MoveLeft();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_ScrollUp()
{
	return sysConfig.m_nActEnterSpeed * imgScreen->Picture->Bitmap->Height + sysConfig.m_nActShowTime;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_MoveUp()
{
	return sysConfig.m_nActEnterSpeed * sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] + sysConfig.m_nActShowTime;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_MoveUp()
{
	return sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] * sysConfig.m_nActExitSpeed;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_ScrollDown()
{
	return GetDisplayTime_ScrollUp();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_MoveDown()
{
	return GetDisplayTime_MoveUp();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_MoveDown()
{
	return GetDisplayTime_Exit_MoveUp();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_LeftToRightSpread()
{
	return sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea] * sysConfig.m_nActEnterSpeed + sysConfig.m_nActShowTime;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_LeftToRightErase()
{
	return sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea] * sysConfig.m_nActExitSpeed;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_RightToLeftSpread()
{
	return GetDisplayTime_LeftToRightSpread();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_RightToLeftErase()
{
	return GetDisplayTime_Exit_LeftToRightErase();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_TopToBottomSpread()
{
	return sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] * sysConfig.m_nActEnterSpeed + sysConfig.m_nActShowTime;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_TopToBottomErase()
{
	return sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] * sysConfig.m_nActExitSpeed;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_BottomToTopSpread()
{
	return GetDisplayTime_TopToBottomSpread();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_BottomToTopErase()
{
	return GetDisplayTime_Exit_TopToBottomErase();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_H_ToCenterSpread()
{
	int nLeftBit = sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea] & 1;
	return (sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea] / 2 + nLeftBit) * sysConfig.m_nActEnterSpeed +
		   sysConfig.m_nActShowTime;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_H_ToCenterErase()
{
	int nLeftBit = sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea] & 1;
	return (sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea] / 2 + nLeftBit) * sysConfig.m_nActExitSpeed;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_H_FromCenterSpread()
{
	return GetDisplayTime_H_ToCenterSpread();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_H_FromCenterErase()
{
	return GetDisplayTime_Exit_H_ToCenterErase();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_V_ToCenterSpread()
{
	int nLeftBit = sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] & 1;
	return (sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] / 2 + nLeftBit) * sysConfig.m_nActEnterSpeed +
		   sysConfig.m_nActShowTime;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_V_ToCenterErase()
{
	int nLeftBit = sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] & 1;
	return (sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] / 2 + nLeftBit) * sysConfig.m_nActExitSpeed;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_V_FromCenterSpread()
{
	return GetDisplayTime_V_ToCenterSpread();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_V_FromCenterErase()
{
	return GetDisplayTime_Exit_V_ToCenterErase();
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_H_Shutter()
{
	return sysConfig.m_nActShutterWidth * sysConfig.m_nActEnterSpeed + sysConfig.m_nActShowTime;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_H_ShutterErase()
{
	return sysConfig.m_nActShutterWidth * sysConfig.m_nActExitSpeed;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_V_Shutter()
{
	return sysConfig.m_nActShutterWidth * sysConfig.m_nActEnterSpeed + sysConfig.m_nActShowTime;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_Exit_V_ShutterErase()
{
	return sysConfig.m_nActShutterWidth * sysConfig.m_nActExitSpeed;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::CalcPlayTime(int nActIndex)
{
	sysConfig.LoadAct(m_dtu, nActIndex);
	int nAreaIndex = sysConfig.GetActArea(m_dtu, nActIndex);
	int nXScreenCount, nYScreenCount;
	int xScreen, yScreen;
	boolean bExit;
	DWORD dwTime = 0;

	if (sysConfig.m_sActText.Length() == 0 && sysConfig.m_picAct == NULL)
	{
		sysConfig.LoadAct(m_dtu, m_nActIndex);
		return 0;
	}

	CreateActImage();

	nXScreenCount = (imgScreen->Picture->Bitmap->Width + sysConfig.m_nSubAreaWidth[nAreaIndex] - 1) / sysConfig.m_nSubAreaWidth[nAreaIndex];
	nYScreenCount = (imgScreen->Picture->Bitmap->Height + sysConfig.m_nSubAreaHeight[nAreaIndex] - 1) / sysConfig.m_nSubAreaHeight[nAreaIndex];

	for (yScreen = 0; yScreen < nYScreenCount; yScreen++)
	{
		for (xScreen = 0; xScreen < nXScreenCount; xScreen++)
		{
			switch (sysConfig.m_nActEnterStyle)
			{
			case ENTER_STYLE_SCROLL_LEFT:
				if ((xScreen == 0) && (yScreen == 0))
				{
					dwTime += GetDisplayTime_ScrollLeft();
					if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BX) && sysConfig.m_bSupportCycleTimeHint)
					{
						if (nActIndex == sysConfig.m_nSycleTimeActIndex)
							dwTime += GetDisplayTime_AutoTimeHint();
					}
				}
				break;
			case ENTER_STYLE_SCROLL_RIGHT:
				if ((xScreen == 0) && (yScreen == 0))
					dwTime += GetDisplayTime_ScrollRight();
				break;
			case ENTER_STYLE_SCROLL_UP:
				if ((xScreen == 0) && (yScreen == 0))
					dwTime += GetDisplayTime_ScrollUp();
				break;
			case ENTER_STYLE_SCROLL_DOWN:
				if ((xScreen == 0) && (yScreen == 0))
					dwTime += GetDisplayTime_ScrollDown();
				break;
			case ENTER_STYLE_MOVE_LEFT:
				dwTime += GetDisplayTime_MoveLeft();
				break;
			case ENTER_STYLE_MOVE_RIGHT:
				dwTime += GetDisplayTime_MoveRight();
				break;
			case ENTER_STYLE_MOVE_UP:
				dwTime += GetDisplayTime_MoveUp();
				break;
			case ENTER_STYLE_MOVE_DOWN:
				dwTime += GetDisplayTime_MoveDown();
				break;
			case ENTER_STYLE_LEFT_TO_RIGHT_SPREAD:
				dwTime += GetDisplayTime_LeftToRightSpread();
				break;
			case ENTER_STYLE_RIGHT_TO_LEFT_SPREAD:
				dwTime += GetDisplayTime_RightToLeftSpread();
				break;
			case ENTER_STYLE_TOP_TO_BOTTOM_SPREAD:
				dwTime += GetDisplayTime_TopToBottomSpread();
				break;
			case ENTER_STYLE_BOTTOM_TO_TOP_SPREAD:
				dwTime += GetDisplayTime_BottomToTopSpread();
				break;
			case ENTER_STYLE_H_TO_CENTER_SPREAD:
				dwTime += GetDisplayTime_H_ToCenterSpread();
				break;
			case ENTER_STYLE_H_FROM_CENTER_SPREAD:
				dwTime += GetDisplayTime_H_FromCenterSpread();
				break;
			case ENTER_STYLE_V_TO_CENTER_SPREAD:
				dwTime += GetDisplayTime_V_ToCenterSpread();
				break;
			case ENTER_STYLE_V_FROM_CENTER_SPREAD:
				dwTime += GetDisplayTime_V_FromCenterSpread();
				break;
			case ENTER_STYLE_H_SHUTTER:
				dwTime += GetDisplayTime_H_Shutter();
				break;
			case ENTER_STYLE_V_SHUTTER:
				dwTime += GetDisplayTime_V_Shutter();
				break;
			case ENTER_STYLE_FLASH:
				//break;
			case ENTER_STYLE_DIRECT:
			default:
				dwTime += GetDisplayTime_Direct();
				break;
			}

			bExit = true;
			switch (sysConfig.m_nActEnterStyle)
			{
			case ENTER_STYLE_SCROLL_LEFT:
			case ENTER_STYLE_SCROLL_RIGHT:
			case ENTER_STYLE_SCROLL_UP:
			case ENTER_STYLE_SCROLL_DOWN:
				if ((xScreen != 0) || (yScreen != 0))
					bExit = false;
				break;
			}

			if (bExit)
			{
				switch (sysConfig.m_nActExitStyle)
				{
				case EXIT_STYLE_MOVE_LEFT:
					dwTime += GetDisplayTime_Exit_MoveLeft();
					break;
				case EXIT_STYLE_MOVE_RIGHT:
					dwTime += GetDisplayTime_Exit_MoveRight();
					break;
				case EXIT_STYLE_MOVE_UP:
					dwTime += GetDisplayTime_Exit_MoveUp();
					break;
				case EXIT_STYLE_MOVE_DOWN:
					dwTime += GetDisplayTime_Exit_MoveDown();
					break;
				case EXIT_STYLE_LEFT_TO_RIGHT_ERASE:
					dwTime += GetDisplayTime_Exit_LeftToRightErase();
					break;
				case EXIT_STYLE_RIGHT_TO_LEFT_ERASE:
					dwTime += GetDisplayTime_Exit_RightToLeftErase();
					break;
				case EXIT_STYLE_TOP_TO_BOTTOM_ERASE:
					dwTime += GetDisplayTime_Exit_TopToBottomErase();
					break;
				case EXIT_STYLE_BOTTOM_TO_TOP_ERASE:
					dwTime += GetDisplayTime_Exit_BottomToTopErase();
					break;
				case EXIT_STYLE_H_TO_CENTER_ERASE:
					dwTime += GetDisplayTime_Exit_H_ToCenterErase();
					break;
				case EXIT_STYLE_H_FROM_CENTER_ERASE:
					dwTime += GetDisplayTime_Exit_H_FromCenterErase();
					break;
				case EXIT_STYLE_V_TO_CENTER_ERASE:
					dwTime += GetDisplayTime_Exit_V_ToCenterErase();
					break;
				case EXIT_STYLE_V_FROM_CENTER_ERASE:
					dwTime += GetDisplayTime_Exit_V_FromCenterErase();
					break;
				case EXIT_STYLE_H_SHUTTER:
					dwTime += GetDisplayTime_Exit_H_ShutterErase();
					break;
				case EXIT_STYLE_V_SHUTTER:
					dwTime += GetDisplayTime_Exit_V_ShutterErase();
					break;
				case EXIT_STYLE_FLASH:
					//dwTxDataLen += GetFlashImageLen_Exit_Direct();
					break;
				case EXIT_STYLE_NULL:
					break;
				case EXIT_STYLE_DIRECT:
				default:
					dwTime += GetDisplayTime_Exit_Direct();
					break;
				}
			}
		}
	}

	sysConfig.LoadAct(m_dtu, m_nActIndex);
	return dwTime;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnCalcDisplayTimeClick(TObject *Sender)
{
	SaveAct();
	lbDisplayTime->Caption = UnicodeString(CalcPlayTime(m_nActIndex) * sysConfig.GetActRepeatCount(m_dtu, m_nActIndex)) + ((sysConfig.m_nLanguage == LANGUAGE_CHINESE) ? " 毫秒" : " ms");
	lbAllDisplayTime->Caption = UnicodeString(CalcAllPlayTime()) + ((sysConfig.m_nLanguage == LANGUAGE_CHINESE) ? " 毫秒" : " ms");
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuDeleteActClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	SaveAct();

	if (Application->MessageBox(L"你确认要删除所选节目的所有内容？", L"清空节目内容", MB_YESNO | MB_ICONQUESTION) != IDYES)
		return;

	for (int i = 0; i < listActs->Items->Count; i++)
	{
		if (listActs->Items->Item[i]->Checked)
		{
			listActs->Items->Item[i]->SubItems->Strings[1] = "";
			sysConfig.DeleteAct(m_dtu, i);
		}
	}

	sysConfig.LoadAct(m_dtu, m_nActIndex);
	UpdateActVol();
	wndTextEdit->Refresh();
	btnCalcDisplayTimeClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::editPicPosChange(TObject *Sender)
{
	wndTextEdit->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuPreviewCurrentActClick(TObject *Sender)
{
	if (m_bPreview)
	{
		m_bPreview = false;
		return;
	}

	SaveAct();

	if (sysConfig.m_sActText.Length() == 0 && sysConfig.m_picAct == NULL)
		return;

#ifdef AUTO_SIZE_TEXT
	m_dtu->ApplyCurrentScreenConfig();
	sysConfig.LoadAct(m_dtu, m_nActIndex);
	UpdateActVol();
#endif

	CreateActImage();
	m_picAct[m_nActIndex]->Bitmap->Assign(imgScreen->Picture->Bitmap);

	int nAreaIndex = sysConfig.m_nActArea;
	m_nPreviewXScreenCount[m_nActIndex] = (imgScreen->Picture->Bitmap->Width + sysConfig.m_nSubAreaWidth[nAreaIndex] - 1) / sysConfig.m_nSubAreaWidth[nAreaIndex];
	m_nPreviewYScreenCount[m_nActIndex] = (imgScreen->Picture->Bitmap->Height + sysConfig.m_nSubAreaHeight[nAreaIndex] - 1) / sysConfig.m_nSubAreaHeight[nAreaIndex];
	m_nPreviewXScreen[m_nActIndex] = 0;
	m_nPreviewYScreen[m_nActIndex] = 0;
	m_nPreviewPos[m_nActIndex] = 0;

	m_bPreview = true;
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
		mnuPreviewCurrentAct->Caption = "停止预览当前编辑的节目(&P)";
	else
		mnuPreviewCurrentAct->Caption = "Stop &Previewing Current Act";
	mnuPreviewAllAct->Enabled = false;
	wndTextEdit->Visible = false;
	imgPreview->Picture->Bitmap->PixelFormat = pf4bit;
	imgPreview->Picture->Bitmap->Canvas->Brush->Color = clBlack;
	imgPreview->Picture->Bitmap->SetSize(sysConfig.m_nScreenWidth, sysConfig.m_nScreenHeight);
	imgPreview->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, sysConfig.m_nScreenWidth, sysConfig.m_nScreenHeight));
	imgPreview->Visible = true;

	PreviewAct(m_nActIndex);

	wndTextEdit->Visible = true;
	imgPreview->Visible = false;
	m_bPreview = false;
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
		mnuPreviewCurrentAct->Caption = "预览当前编辑的节目(&P)";
	else
		mnuPreviewCurrentAct->Caption = "&Preview Current Act";
	mnuPreviewAllAct->Enabled = true;
}
//---------------------------------------------------------------------------

void TfrmMain::PreviewAct(int nActIndex)
{
	LARGE_INTEGER int64SysFreq;
	LARGE_INTEGER int64CounterStart;
	LARGE_INTEGER int64CounterNow;
	int nAreaIndex = sysConfig.GetActArea(m_dtu, nActIndex);

	::QueryPerformanceFrequency(&int64SysFreq);
	::QueryPerformanceCounter(&int64CounterStart);
	m_nPreviewStaus[nActIndex] = 0;
	for (;;)
	{
		::QueryPerformanceCounter(&int64CounterNow);
		int64CounterNow.QuadPart = int64CounterNow.QuadPart - int64CounterStart.QuadPart;
		if (int64CounterNow.QuadPart >= (int64SysFreq.QuadPart / 1000))
		{
			int64CounterStart.QuadPart += (int64SysFreq.QuadPart / 1000);
			ProcPreview(nActIndex, nAreaIndex);
		}

		if (m_nPreviewStaus[nActIndex] == -1)
			break;

		Application->ProcessMessages();
		if (!m_bPreview)
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::scrollboxEditResize(TObject *Sender)
{
	imgPreview->Width = sysConfig.m_nScreenWidth;
	imgPreview->Height = sysConfig.m_nScreenHeight;
	if (imgPreview->Width < scrollboxEdit->Width)
		imgPreview->Left = (scrollboxEdit->Width - imgPreview->Width) / 2;
	else
		imgPreview->Left = 0;
	if (imgPreview->Height < scrollboxEdit->Height)
		imgPreview->Top = (scrollboxEdit->Height - imgPreview->Height) / 2;
	else
		imgPreview->Top = 0;
}
//---------------------------------------------------------------------------

void TfrmMain::ProcPreview(int nActIndex, int nAreaIndex)
{
	do
	{
		switch (sysConfig.GetActEnterStyle(m_dtu, nActIndex))
		{
		case ENTER_STYLE_SCROLL_LEFT:
			Preview_ScrollLeft(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_SCROLL_RIGHT:
			Preview_ScrollRight(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_SCROLL_UP:
			Preview_ScrollUp(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_SCROLL_DOWN:
			Preview_ScrollDown(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_MOVE_LEFT:
			Preview_MoveLeft(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_MOVE_RIGHT:
			Preview_MoveRight(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_MOVE_UP:
			Preview_MoveUp(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_MOVE_DOWN:
			Preview_MoveDown(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_LEFT_TO_RIGHT_SPREAD:
			Preview_LeftToRightSpread(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_RIGHT_TO_LEFT_SPREAD:
			Preview_RightToLeftSpread(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_TOP_TO_BOTTOM_SPREAD:
			Preview_TopToBottomSpread(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_BOTTOM_TO_TOP_SPREAD:
			Preview_BottomToTopSpread(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_H_TO_CENTER_SPREAD:
			Preview_H_ToCenterSpread(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_H_FROM_CENTER_SPREAD:
			Preview_H_FromCenterSpread(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_V_TO_CENTER_SPREAD:
			Preview_V_ToCenterSpread(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_V_FROM_CENTER_SPREAD:
			Preview_V_FromCenterSpread(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_H_SHUTTER:
			Preview_H_Shutter(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_V_SHUTTER:
			Preview_V_Shutter(nActIndex, nAreaIndex);
			break;
		case ENTER_STYLE_FLASH:
			//Preview_Flash(nActIndex, nAreaIndex);
			//break;
		case ENTER_STYLE_DIRECT:
		default:
			Preview_Direct(nActIndex, nAreaIndex);
			break;
		}

		switch (sysConfig.GetActExitStyle(m_dtu, nActIndex))
		{
		case EXIT_STYLE_MOVE_LEFT:
			Preview_Exit_MoveLeft(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_MOVE_RIGHT:
			Preview_Exit_MoveRight(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_MOVE_UP:
			Preview_Exit_MoveUp(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_MOVE_DOWN:
			Preview_Exit_MoveDown(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_LEFT_TO_RIGHT_ERASE:
			Preview_Exit_LeftToRightErase(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_RIGHT_TO_LEFT_ERASE:
			Preview_Exit_RightToLeftErase(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_TOP_TO_BOTTOM_ERASE:
			Preview_Exit_TopToBottomErase(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_BOTTOM_TO_TOP_ERASE:
			Preview_Exit_BottomToTopErase(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_H_TO_CENTER_ERASE:
			Preview_Exit_H_ToCenterErase(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_H_FROM_CENTER_ERASE:
			Preview_Exit_H_FromCenterErase(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_V_TO_CENTER_ERASE:
			Preview_Exit_V_ToCenterErase(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_V_FROM_CENTER_ERASE:
			Preview_Exit_V_FromCenterErase(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_H_SHUTTER:
			Preview_Exit_H_ShutterErase(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_V_SHUTTER:
			Preview_Exit_V_ShutterErase(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_FLASH:
			//break;
		case EXIT_STYLE_NULL:
			Preview_Exit_Null(nActIndex, nAreaIndex);
			break;
		case EXIT_STYLE_DIRECT:
		default:
			Preview_Exit_Direct(nActIndex, nAreaIndex);
			break;
		}
	}
	while (m_nPreviewStaus[nActIndex] == 0);
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Direct(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
			m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
			SRCCOPY);

		imgPreview->Refresh();
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nPreviewXScreen[nActIndex]++;
			if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
			{
				m_nPreviewXScreen[nActIndex] = 0;
				m_nPreviewYScreen[nActIndex]++;
			}

			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_Null(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
			m_nPreviewStaus[nActIndex] = -1;
		else
		{
			m_nPreviewStaus[nActIndex] = 0;
			m_nActTime[nActIndex] = 0;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_Direct(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();

		if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
			m_nPreviewStaus[nActIndex] = -1;
		else
		{
			m_nPreviewStaus[nActIndex] = 0;
			m_nActTime[nActIndex] = 0;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_ScrollLeft(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewPos[nActIndex],
			0,
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				SRCCOPY);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewPos[nActIndex],
				0,
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= m_picAct[nActIndex]->Bitmap->Width)
				m_nPreviewStaus[nActIndex] = 2;
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
			m_nPreviewYScreen[nActIndex] = m_nPreviewYScreenCount[nActIndex];
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_MoveLeft(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] + m_nPreviewPos[nActIndex],
			0,
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				SRCCOPY);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] + m_nPreviewPos[nActIndex],
				0,
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaWidth[nAreaIndex])
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex] = m_nPreviewYScreenCount[nActIndex];
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_MoveLeft(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaWidth[nAreaIndex])
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_ScrollRight(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_picAct[nActIndex]->Bitmap->Width - m_nPreviewPos[nActIndex] - 1,
			0,
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				SRCCOPY);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_picAct[nActIndex]->Bitmap->Width - m_nPreviewPos[nActIndex] - 1,
				0,
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= m_picAct[nActIndex]->Bitmap->Width)
				m_nPreviewStaus[nActIndex] = 2;
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
			m_nPreviewYScreen[nActIndex] = m_nPreviewYScreenCount[nActIndex];
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_MoveRight(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaWidth[nAreaIndex])
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_ScrollUp(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + 1,
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			0,
			m_nPreviewPos[nActIndex],
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + 1,
				SRCCOPY);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				0,
				m_nPreviewPos[nActIndex],
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= m_picAct[nActIndex]->Bitmap->Height)
				m_nPreviewStaus[nActIndex] = 2;
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
			m_nPreviewYScreen[nActIndex] = m_nPreviewYScreenCount[nActIndex];
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_ScrollDown(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + 1,
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			0,
			m_picAct[nActIndex]->Bitmap->Height - 1,
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + 1,
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				SRCCOPY);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				0,
				m_picAct[nActIndex]->Bitmap->Height - m_nPreviewPos[nActIndex] - 1,
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= m_picAct[nActIndex]->Bitmap->Height)
				m_nPreviewStaus[nActIndex] = 2;
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
			m_nPreviewYScreen[nActIndex] = m_nPreviewYScreenCount[nActIndex];
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_MoveUp(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + 1,
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			1,
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + 1,
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaHeight[nAreaIndex])
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_MoveDown(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + 1,
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + 1,
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaHeight[nAreaIndex])
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_MoveRight(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_picAct[nActIndex]->Bitmap->Width - m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			0,
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex] - 1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				SRCCOPY);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_picAct[nActIndex]->Bitmap->Width - m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				0,
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaWidth[nAreaIndex])
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex] = m_nPreviewYScreenCount[nActIndex];
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_MoveUp(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + 1,
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			0,
			m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] + m_nPreviewPos[nActIndex],
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + 1,
				SRCCOPY);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] + m_nPreviewPos[nActIndex],
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaHeight[nAreaIndex])
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex]++;
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_MoveDown(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + 1,
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			0,
			m_picAct[nActIndex]->Bitmap->Height - m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + 1,
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				sysConfig.m_nSubAreaHeight[nAreaIndex] - 1,
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				SRCCOPY);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
				m_picAct[nActIndex]->Bitmap->Height - m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaHeight[nAreaIndex])
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex]++;
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_LeftToRightSpread(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] + m_nPreviewPos[nActIndex],
			m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] + m_nPreviewPos[nActIndex],
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaWidth[nAreaIndex])
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex]++;
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_RightToLeftSpread(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			(m_nPreviewXScreen[nActIndex] + 1) * sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				(m_nPreviewXScreen[nActIndex] + 1) * sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaWidth[nAreaIndex])
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex]++;
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_TopToBottomSpread(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + m_nPreviewPos[nActIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
			m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] + m_nPreviewPos[nActIndex],
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] + m_nPreviewPos[nActIndex],
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaHeight[nAreaIndex])
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex]++;
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_BottomToTopSpread(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
			(m_nPreviewYScreen[nActIndex] + 1) * sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
				(m_nPreviewYScreen[nActIndex] + 1) * sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaHeight[nAreaIndex])
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex]++;
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_H_ToCenterSpread(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + m_nPreviewPos[nActIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] + m_nPreviewPos[nActIndex],
			m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			(m_nPreviewXScreen[nActIndex] + 1) * sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] + m_nPreviewPos[nActIndex],
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
				SRCCOPY);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				(m_nPreviewXScreen[nActIndex] + 1) * sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2)
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex]++;
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_H_FromCenterSpread(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2 + m_nPreviewPos[nActIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] + (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2 + m_nPreviewPos[nActIndex],
			m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2 - m_nPreviewPos[nActIndex] - 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] + (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2 - m_nPreviewPos[nActIndex] - 1,
			m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2 + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] + (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2 + m_nPreviewPos[nActIndex],
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
				SRCCOPY);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2 - m_nPreviewPos[nActIndex] - 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] + (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2 - m_nPreviewPos[nActIndex] - 1,
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2)
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex]++;
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_V_ToCenterSpread(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + m_nPreviewPos[nActIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
			m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] + m_nPreviewPos[nActIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
			(m_nPreviewYScreen[nActIndex] + 1) * sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] + m_nPreviewPos[nActIndex],
				SRCCOPY);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
				(m_nPreviewYScreen[nActIndex] + 1) * sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2)
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex]++;
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_V_FromCenterSpread(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2 + m_nPreviewPos[nActIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
			m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] + (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2 + m_nPreviewPos[nActIndex],
			SRCCOPY);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2 - m_nPreviewPos[nActIndex] - 1,
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			m_picAct[nActIndex]->Bitmap->Canvas->Handle,
			m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
			m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] + (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2 - m_nPreviewPos[nActIndex] - 1,
			SRCCOPY);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2 + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] + (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2 + m_nPreviewPos[nActIndex],
				SRCCOPY);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2 - m_nPreviewPos[nActIndex] - 1,
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] + (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2 - m_nPreviewPos[nActIndex] - 1,
				SRCCOPY);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2)
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex]++;
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_H_Shutter(int nActIndex, int nAreaIndex)
{
	int nActShutterWidth = sysConfig.GetActShutterWidth(m_dtu, nActIndex);
	int nShutterCount = (sysConfig.m_nSubAreaWidth[nAreaIndex] + nActShutterWidth - 1) / nActShutterWidth;

	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		for (int i = 0; i < nShutterCount; i++)
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + i * nActShutterWidth + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] + i * nActShutterWidth + m_nPreviewPos[nActIndex],
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
				SRCCOPY);
		}
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			for (int i = 0; i < nShutterCount; i++)
			{
				if ((i * nActShutterWidth + m_nPreviewPos[nActIndex]) < sysConfig.m_nSubAreaWidth[nAreaIndex])
					::BitBlt(
						imgPreview->Picture->Bitmap->Canvas->Handle,
						sysConfig.m_nSubAreaLeft[nAreaIndex] + i * nActShutterWidth + m_nPreviewPos[nActIndex],
						sysConfig.m_nSubAreaTop[nAreaIndex],
						1,
						sysConfig.m_nSubAreaHeight[nAreaIndex],
						m_picAct[nActIndex]->Bitmap->Canvas->Handle,
						m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex] + i * nActShutterWidth + m_nPreviewPos[nActIndex],
						m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex],
						SRCCOPY);
			}
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= nActShutterWidth)
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex]++;
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_V_Shutter(int nActIndex, int nAreaIndex)
{
	int nActShutterWidth = sysConfig.GetActShutterWidth(m_dtu, nActIndex);
	int nShutterCount = (sysConfig.m_nSubAreaHeight[nAreaIndex] + nActShutterWidth - 1) / nActShutterWidth;

	if (m_nPreviewStaus[nActIndex] == 0)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 1;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		for (int i = 0; i < nShutterCount; i++)
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + i * nActShutterWidth + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				m_picAct[nActIndex]->Bitmap->Canvas->Handle,
				m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
				m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] + i * nActShutterWidth + m_nPreviewPos[nActIndex],
				SRCCOPY);
		}
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 1)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 1)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActEnterSpeed(m_dtu, nActIndex))
		{
			for (int i = 0; i < nShutterCount; i++)
			{
				if ((i * nActShutterWidth + m_nPreviewPos[nActIndex]) < sysConfig.m_nSubAreaHeight[nAreaIndex])
					::BitBlt(
						imgPreview->Picture->Bitmap->Canvas->Handle,
						sysConfig.m_nSubAreaLeft[nAreaIndex],
						sysConfig.m_nSubAreaTop[nAreaIndex] + i * nActShutterWidth + m_nPreviewPos[nActIndex],
						sysConfig.m_nSubAreaWidth[nAreaIndex],
						1,
						m_picAct[nActIndex]->Bitmap->Canvas->Handle,
						m_nPreviewXScreen[nActIndex] * sysConfig.m_nSubAreaWidth[nAreaIndex],
						m_nPreviewYScreen[nActIndex] * sysConfig.m_nSubAreaHeight[nAreaIndex] + i * nActShutterWidth + m_nPreviewPos[nActIndex],
						SRCCOPY);
			}
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= nActShutterWidth)
			{
				m_nPreviewXScreen[nActIndex]++;
				if (m_nPreviewXScreen[nActIndex] >= m_nPreviewXScreenCount[nActIndex])
				{
					m_nPreviewXScreen[nActIndex] = 0;
					m_nPreviewYScreen[nActIndex]++;
				}
				m_nPreviewStaus[nActIndex] = 2;
			}
		}
	}
	else if (m_nPreviewStaus[nActIndex] == 2)
	{
		m_nActTime[nActIndex]++;
		if (m_nActTime[nActIndex] >= sysConfig.GetActShowTime(m_dtu, nActIndex))
		{
			m_nActTime[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 100;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_LeftToRightErase(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + m_nPreviewPos[nActIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				NULL,
				0,
				0,
				BLACKNESS);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaWidth[nAreaIndex])
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_RightToLeftErase(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				NULL,
				0,
				0,
				BLACKNESS);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaWidth[nAreaIndex])
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_TopToBottomErase(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + m_nPreviewPos[nActIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				NULL,
				0,
				0,
				BLACKNESS);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaHeight[nAreaIndex])
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_BottomToTopErase(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				NULL,
				0,
				0,
				BLACKNESS);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= sysConfig.m_nSubAreaHeight[nAreaIndex])
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_H_ToCenterErase(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + m_nPreviewPos[nActIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				NULL,
				0,
				0,
				BLACKNESS);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + sysConfig.m_nSubAreaWidth[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				NULL,
				0,
				0,
				BLACKNESS);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2)
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_H_FromCenterErase(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2 + m_nPreviewPos[nActIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex] + (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2 - m_nPreviewPos[nActIndex] - 1,
			sysConfig.m_nSubAreaTop[nAreaIndex],
			1,
			sysConfig.m_nSubAreaHeight[nAreaIndex],
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2 + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				NULL,
				0,
				0,
				BLACKNESS);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2 - m_nPreviewPos[nActIndex] - 1,
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				NULL,
				0,
				0,
				BLACKNESS);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= (sysConfig.m_nSubAreaWidth[nAreaIndex] + 1) / 2)
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_V_ToCenterErase(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + m_nPreviewPos[nActIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				NULL,
				0,
				0,
				BLACKNESS);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + sysConfig.m_nSubAreaHeight[nAreaIndex] - m_nPreviewPos[nActIndex] - 1,
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				NULL,
				0,
				0,
				BLACKNESS);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2)
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_V_FromCenterErase(int nActIndex, int nAreaIndex)
{
	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2 + m_nPreviewPos[nActIndex],
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			NULL,
			0,
			0,
			BLACKNESS);

		::BitBlt(
			imgPreview->Picture->Bitmap->Canvas->Handle,
			sysConfig.m_nSubAreaLeft[nAreaIndex],
			sysConfig.m_nSubAreaTop[nAreaIndex] + (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2 - m_nPreviewPos[nActIndex] - 1,
			sysConfig.m_nSubAreaWidth[nAreaIndex],
			1,
			NULL,
			0,
			0,
			BLACKNESS);
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2 + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				NULL,
				0,
				0,
				BLACKNESS);

			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2 - m_nPreviewPos[nActIndex] - 1,
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				NULL,
				0,
				0,
				BLACKNESS);
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= (sysConfig.m_nSubAreaHeight[nAreaIndex] + 1) / 2)
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_H_ShutterErase(int nActIndex, int nAreaIndex)
{
	int nActShutterWidth = sysConfig.GetActShutterWidth(m_dtu, nActIndex);
	int nShutterCount = (sysConfig.m_nSubAreaWidth[nAreaIndex] + nActShutterWidth - 1) / nActShutterWidth;

	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		for (int i = 0; i < nShutterCount; i++)
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex] + i * nActShutterWidth + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex],
				1,
				sysConfig.m_nSubAreaHeight[nAreaIndex],
				NULL,
				0,
				0,
				BLACKNESS);
		}
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			for (int i = 0; i < nShutterCount; i++)
			{
				if ((i * nActShutterWidth + m_nPreviewPos[nActIndex]) < sysConfig.m_nSubAreaWidth[nAreaIndex])
					::BitBlt(
						imgPreview->Picture->Bitmap->Canvas->Handle,
						sysConfig.m_nSubAreaLeft[nAreaIndex] + i * nActShutterWidth + m_nPreviewPos[nActIndex],
						sysConfig.m_nSubAreaTop[nAreaIndex],
						1,
						sysConfig.m_nSubAreaHeight[nAreaIndex],
						NULL,
						0,
						0,
						BLACKNESS);
			}
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= nActShutterWidth)
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Preview_Exit_V_ShutterErase(int nActIndex, int nAreaIndex)
{
	int nActShutterWidth = sysConfig.GetActShutterWidth(m_dtu, nActIndex);
	int nShutterCount = (sysConfig.m_nSubAreaHeight[nAreaIndex] + nActShutterWidth - 1) / nActShutterWidth;

	if (m_nPreviewStaus[nActIndex] == 100)
	{
		m_nActTime[nActIndex] = 0;
		m_nPreviewStaus[nActIndex] = 101;
		m_nPreviewPos[nActIndex] = 0;

		for (int i = 0; i < nShutterCount; i++)
		{
			::BitBlt(
				imgPreview->Picture->Bitmap->Canvas->Handle,
				sysConfig.m_nSubAreaLeft[nAreaIndex],
				sysConfig.m_nSubAreaTop[nAreaIndex] + i * nActShutterWidth + m_nPreviewPos[nActIndex],
				sysConfig.m_nSubAreaWidth[nAreaIndex],
				1,
				NULL,
				0,
				0,
				BLACKNESS);
		}
		imgPreview->Refresh();
	}
	else if (m_nPreviewStaus[nActIndex] == 101)
	{
		m_nActTime[nActIndex]++;
	}

	if (m_nPreviewStaus[nActIndex] == 101)
	{
		if (m_nActTime[nActIndex] >= sysConfig.GetActExitSpeed(m_dtu, nActIndex))
		{
			for (int i = 0; i < nShutterCount; i++)
			{
				if ((i * nActShutterWidth + m_nPreviewPos[nActIndex]) < sysConfig.m_nSubAreaHeight[nAreaIndex])
					::BitBlt(
						imgPreview->Picture->Bitmap->Canvas->Handle,
						sysConfig.m_nSubAreaLeft[nAreaIndex],
						sysConfig.m_nSubAreaTop[nAreaIndex] + i * nActShutterWidth + m_nPreviewPos[nActIndex],
						sysConfig.m_nSubAreaWidth[nAreaIndex],
						1,
						NULL,
						0,
						0,
						BLACKNESS);
			}
			imgPreview->Refresh();

			m_nPreviewPos[nActIndex]++;
			m_nActTime[nActIndex] = 0;
			if (m_nPreviewPos[nActIndex] >= nActShutterWidth)
			{
				m_nPreviewPos[nActIndex] = 0;
				if (m_nPreviewYScreen[nActIndex] >= m_nPreviewYScreenCount[nActIndex])
					m_nPreviewStaus[nActIndex] = -1;
				else
					m_nPreviewStaus[nActIndex] = 0;
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuPreviewAllActClick(TObject *Sender)
{
	if (m_bPreview)
	{
		m_bPreview = false;
		return;
	}

	SaveAct();

	for (int nActIndex = 0; nActIndex < sysConfig.m_nMaxActCount; nActIndex++)
	{
		sysConfig.LoadAct(m_dtu, nActIndex);
		if (sysConfig.m_sActText.Length() == 0 && sysConfig.m_picAct == NULL)
			m_picAct[nActIndex]->Bitmap->SetSize(1, 1);
		else
		{
			CreateActImage();
			m_picAct[nActIndex]->Bitmap->Assign(imgScreen->Picture->Bitmap);
			int nAreaIndex = sysConfig.m_nActArea;
			m_nPreviewXScreenCount[nActIndex] = (imgScreen->Picture->Bitmap->Width + sysConfig.m_nSubAreaWidth[nAreaIndex] - 1) / sysConfig.m_nSubAreaWidth[nAreaIndex];
			m_nPreviewYScreenCount[nActIndex] = (imgScreen->Picture->Bitmap->Height + sysConfig.m_nSubAreaHeight[nAreaIndex] - 1) / sysConfig.m_nSubAreaHeight[nAreaIndex];
			m_nPreviewXScreen[nActIndex] = 0;
			m_nPreviewYScreen[nActIndex] = 0;
			m_nPreviewPos[nActIndex] = 0;
			m_nPreviewStaus[nActIndex] = 0;
		}
	}
	sysConfig.LoadAct(m_dtu, m_nActIndex);

	m_bPreview = true;
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
		mnuPreviewAllAct->Caption = "停止预览所有节目(&V)";
	else
		mnuPreviewAllAct->Caption = "Stop Pre&view All Acts";
	mnuPreviewCurrentAct->Enabled = false;
	wndTextEdit->Visible = false;
	imgPreview->Picture->Bitmap->PixelFormat = pf4bit;
	imgPreview->Picture->Bitmap->Canvas->Brush->Color = clBlack;
	imgPreview->Picture->Bitmap->SetSize(sysConfig.m_nScreenWidth, sysConfig.m_nScreenHeight);
	imgPreview->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, sysConfig.m_nScreenWidth, sysConfig.m_nScreenHeight));
	imgPreview->Visible = true;

	PreviewAllAct();

	wndTextEdit->Visible = true;
	imgPreview->Visible = false;
	m_bPreview = false;
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
		mnuPreviewAllAct->Caption = "预览所有节目(&V)";
	else
		mnuPreviewAllAct->Caption = "Pre&view All Acts";
	mnuPreviewCurrentAct->Enabled = true;
}
//---------------------------------------------------------------------------

void TfrmMain::PreviewAllAct()
{
	LARGE_INTEGER int64SysFreq;
	LARGE_INTEGER int64CounterStart;
	LARGE_INTEGER int64CounterNow;
	int nAreaIndex, nActIndex;
	int nActOfSubArea[MAX_AREA_COUNT];

	for (nAreaIndex = 0; nAreaIndex < MAX_AREA_COUNT; nAreaIndex++)
		nActOfSubArea[nAreaIndex] = -1;

	for (nAreaIndex = 0; nAreaIndex < MAX_AREA_COUNT; nAreaIndex++)
	{
		for (nActIndex = 0; nActIndex < sysConfig.m_nMaxActCount; nActIndex++)
		{
			if (sysConfig.GetActArea(m_dtu, nActIndex) == nAreaIndex)
			{
				if (m_picAct[nActIndex]->Bitmap->Width > 1)
				{
					nActOfSubArea[nAreaIndex] = nActIndex;
					break;
				}
			}
		}
	}

	::QueryPerformanceFrequency(&int64SysFreq);
	::QueryPerformanceCounter(&int64CounterStart);
	m_nPreviewStaus[nActIndex] = 0;
	for (;;)
	{
		::QueryPerformanceCounter(&int64CounterNow);
		int64CounterNow.QuadPart = int64CounterNow.QuadPart - int64CounterStart.QuadPart;
		if (int64CounterNow.QuadPart >= (int64SysFreq.QuadPart / 1000))
		{
			int64CounterStart.QuadPart += (int64SysFreq.QuadPart / 1000);
			for (nAreaIndex = 0; nAreaIndex < MAX_AREA_COUNT; nAreaIndex++)
			{
				nActIndex = nActOfSubArea[nAreaIndex];
				if (nActIndex >= 0)
				{
					ProcPreview(nActIndex, nAreaIndex);
					if (m_nPreviewStaus[nActIndex] == -1)
					{
						m_nPreviewStaus[nActIndex] = 0;
						m_nPreviewXScreen[nActIndex] = 0;
						m_nPreviewYScreen[nActIndex] = 0;
						m_nPreviewPos[nActIndex] = 0;
						nActOfSubArea[nAreaIndex] = FindNextAct(nActIndex, nAreaIndex);
					 }
				 }
			}
		}

		Application->ProcessMessages();
		if (!m_bPreview)
			break;
	}
}
//---------------------------------------------------------------------------

int TfrmMain::FindNextAct(int nActIndex, int nAreaIndex)
{
	for (int i = 0; i < sysConfig.m_nMaxActCount; i++)
	{
		nActIndex++;
		if (nActIndex >= sysConfig.m_nMaxActCount)
			nActIndex = 0;
		if (sysConfig.GetActArea(m_dtu, nActIndex) == nAreaIndex)
		{
			if (m_picAct[nActIndex]->Bitmap->Width > 1)
				return nActIndex;
		}
	}

	return -1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuDeletePictureClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif
	SaveAct();

	int nActIndex = m_nActIndex;
	for (int i = 0; i < listActs->Items->Count; i++)
	{
		if (listActs->Items->Item[i]->Checked)
		{
			m_nActIndex = i;
			sysConfig.LoadAct(m_dtu, m_nActIndex);
			if (wndTextEdit->pic != NULL)
				delete wndTextEdit->pic;
			wndTextEdit->pic = NULL;
			SaveAct();
		}
	}

	m_nActIndex = nActIndex;
	sysConfig.LoadAct(m_dtu, m_nActIndex);
	UpdateActVol();
	wndTextEdit->Refresh();
	btnCalcDisplayTimeClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuDeleteTextClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	SaveAct();

	int nActIndex = m_nActIndex;
	for (int i = 0; i < listActs->Items->Count; i++)
	{
		if (listActs->Items->Item[i]->Checked)
		{
			m_nActIndex = i;
			sysConfig.LoadAct(m_dtu, m_nActIndex);
			wndTextEdit->m_wsListText->Clear();
			wndTextEdit->m_wsListActive->Clear();
			wndTextEdit->m_wsListColor->Clear();
			SaveAct();
		}
	}

	m_nActIndex = nActIndex;
	sysConfig.LoadAct(m_dtu, m_nActIndex);
	UpdateActVol();
	wndTextEdit->Refresh();
	btnCalcDisplayTimeClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuSetTextActiveClick(TObject *Sender)
{
	int nActiveType;
	int nLen;

	TfrmActiveType *frmActiveType = new TfrmActiveType(this);
	SYSTEMTIME st;
	::GetLocalTime(&st);
	st.wYear = sysConfig.m_nActDownCounterBaseYear;
	st.wMonth = sysConfig.m_nActDownCounterBaseMonth;
	st.wDay = sysConfig.m_nActDownCounterBaseDay;
	frmActiveType->dateCounterBy->DateTime = SystemTimeToDateTime(st);
	if (frmActiveType->ShowModal() != mrOk)
	{
		delete frmActiveType;
		return;
	}

	nActiveType = frmActiveType->comboActiveType->ItemIndex;
	DateTimeToSystemTime(frmActiveType->dateCounterBy->DateTime, st);
	int nExtDataId = frmActiveType->cbExtDataId->ItemIndex;
	switch (nActiveType)
	{
	case 0:	//4位数字年(2010)
		nLen = 4;
		break;
	case 1:	//2位数字年(10)
	case 2:	//2位数字月
	case 3:	//2位数字日
	case 4:	//2位数字小时
	case 5:	//2位数字分钟
	case 6:	//2位数字秒
		nLen = 2;
		break;
	case 7:	//1个汉字星期(日~六)
		nLen = 1;
		break;
	case 8:		//2个汉字农历年
	case 9:		//2个汉字农历月份
	case 10:	//2个汉字农历日期
		nLen = 2;
		break;
	case 11:	//3位数字倒计时天数
		nLen = 3;
		break;
	case 12:	//2位数字倒计时天数
		nLen = 2;
		break;
	case 13:	//1位数字倒计时天数
		nLen = 1;
		break;
	case 14:	//2位数字温度(-9~99)
		nLen = 2;
		break;
	case 15:	//2位数字湿度(00~99)
		nLen = 2;
		break;
	case 16:	//3位数字温度(-99~99)
		nLen = 3;
		break;
	case 17:	//4位数字温度(-99~99.9")
		nLen = 4;
		break;
	case 18:	//4位数字湿度(0~99.9)
		nLen = 4;
		break;
	case 19:	//2汉字风向
		nLen = 2;
		break;
	case 20:	//1字符外部数据
		nLen = 1;
		break;
	case 21:	//2字符外部数据
		nLen = 2;
		break;
	case 22:	//3字符外部数据
		nLen = 3;
		break;
	case 23:	//4字符外部数据
		nLen = 4;
		break;
	case 24:	//5字符外部数据
		nLen = 5;
		break;
	case 25:	//6字符外部数据
		nLen = 6;
		break;
	case 26:	//7字符外部数据
		nLen = 7;
		break;
	default:
		nLen = 0;
	}

	UnicodeString ws = wndTextEdit->GetSelectedText();
	if (nLen != ws.Length())
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			::MessageBox(this->Handle, L"选择字数错误!请重新选择。", NULL, MB_OK | MB_ICONWARNING);
		else
			::MessageBox(this->Handle, L"Selected character count incorrect! please reselect again.", NULL, MB_OK | MB_ICONWARNING);
		delete frmActiveType;
		return;
	}

	sysConfig.m_nActDownCounterBaseYear = st.wYear;
	sysConfig.m_nActDownCounterBaseMonth = st.wMonth;
	sysConfig.m_nActDownCounterBaseDay = st.wDay;
	if (nActiveType >= 20 && nActiveType <= 26)
	{
		nActiveType -= 20;
		nActiveType *= 8;
		nActiveType += nExtDataId;
		nActiveType += 20;
	}
	wndTextEdit->SetTextActive('0' + nActiveType);
	wndTextEdit->m_bDirty = true;

	delete frmActiveType;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuEditClick(TObject *Sender)
{
	mnuSetTextActive->Enabled =
		((wndTextEdit->m_nSelectStartRow == wndTextEdit->m_nSelectEndRow) &&
		(wndTextEdit->m_nSelectStartCol != wndTextEdit->m_nSelectEndCol));
	mnuSetTextNormal->Enabled = mnuSetTextActive->Enabled;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetActiveFlashImageLen_Direct(int nAreaIndex)
{
	DWORD dwLen = 0;
	TEXTMETRIC tm;
	int nFontHeight;

	imgActive->Picture->Bitmap->PixelFormat = pf4bit;
	imgActive->Picture->Bitmap->Canvas->Brush->Color = clBlack;
	imgActive->Picture->Bitmap->Canvas->Font->Name = sysConfig.m_sActFontName;
	imgActive->Picture->Bitmap->Canvas->Font->Size = sysConfig.m_nActFontSize;
	imgActive->Picture->Bitmap->Canvas->Font->Style = TFontStyles(sysConfig.m_nActFontStyle);
	imgActive->Picture->Bitmap->Canvas->Font->Color = clRed;
	GetTextMetrics(imgActive->Picture->Bitmap->Canvas->Handle, &tm);
	if (sysConfig.m_bDotFont)
		nFontHeight = sysConfig.m_nDotFontSize;
	else
		nFontHeight = tm.tmHeight;

	int nHanziWidth = GetTextWidth(imgActive->Picture->Bitmap->Canvas, sysConfig.m_bDotFont, sysConfig.m_nDotFontSize, "六");
	int nDigitWidth = GetTextWidth(imgActive->Picture->Bitmap->Canvas, sysConfig.m_bDotFont, sysConfig.m_nDotFontSize, "8");
	int nHanziByteWidth =  (nHanziWidth + 7) / 8;
	int nDigitByteWidth =  (nDigitWidth + 7) / 8;

	int nCountInItem = 0;
	wchar_t wcCurrentActiveChar = '$';
	wchar_t* ws = sysConfig.m_sActActiveText.c_str();
	for (;;)
	{
		switch (*ws)
		{
		case '0':		//4位数字年(2010)
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 4)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 10 * nDigitByteWidth * nFontHeight * GetColorCount());
			}
			break;
		case '1':		//2位数字年(10)
		case '2':		//2位数字月
		case '3':		//2位数字日
		case '4':		//2位数字小时
		case '5':		//2位数字分钟
		case '6':		//2位数字秒
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 2)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 10 * nDigitByteWidth * nFontHeight * GetColorCount());
			}
			break;
		case '7':		//1个汉字星期(日~六)
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 1)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 7 * nHanziByteWidth * nFontHeight * GetColorCount());
			}
			break;
		case '8':		//2个汉字农历年
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 2)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 22 * nHanziByteWidth * nFontHeight * GetColorCount());
			}
			break;
		case '9':		//2个汉字农历月份
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 2)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 12 * nHanziByteWidth * nFontHeight * GetColorCount());
			}
			break;
		case '0' + 10:	//2个汉字农历日期
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 2)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 12 * nHanziByteWidth * nFontHeight * GetColorCount());
			}
			break;
		case '0' + 11:	//3位数字倒计时天数
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 3)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 10 * nDigitByteWidth * nFontHeight * GetColorCount());
				dwLen += 4;	//基准时间
			}
			break;
		case '0' + 12:	//2位数字倒计时天数
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 2)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 10 * nDigitByteWidth * nFontHeight * GetColorCount());
				dwLen += 4;	//基准时间
			}
			break;
		case '0' + 13:	//1位数字倒计时天数
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 1)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 10 * nDigitByteWidth * nFontHeight * GetColorCount());
				dwLen += 4;	//基准时间
			}
			break;
		case '0' + 14:		//2位数字温度(-9~99)
		case '0' + 15:		//2位数字湿度(00~99)
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 2)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 13 * nDigitByteWidth * nFontHeight * GetColorCount());
			}
			break;
		case '0' + 16:		///3位数字温度(-99~99)
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 3)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 13 * nDigitByteWidth * nFontHeight * GetColorCount());
			}
			break;
		case '0' + 17:		//4位数字温度(-99~99.9")
		case '0' + 18:		//4位数字湿度(0~99.9)
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 4)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 13 * nDigitByteWidth * nFontHeight * GetColorCount());
			}
			break;
		case '0' + 19:		//2个汉字风向
			if (wcCurrentActiveChar != *ws)
			{
				wcCurrentActiveChar = *ws;
				nCountInItem = 1;
			}
			else
				nCountInItem++;
			if (nCountInItem >= 2)
			{
				wcCurrentActiveChar = '$';
				nCountInItem = 0;
				dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 5 * nHanziByteWidth * nFontHeight * GetColorCount());
			}
			break;
		default:
			if ((*ws >= '0' + 20) &&(*ws < '0' + 20 + 56))
			{
				if (wcCurrentActiveChar != *ws)
				{
					wcCurrentActiveChar = *ws;
					nCountInItem = 1;
				}
				else
					nCountInItem++;
				int nItemLen = wcCurrentActiveChar - '0' - 20;
				nItemLen /= 8;
				nItemLen += 1;
				if (nCountInItem >= nItemLen)
				{
					wcCurrentActiveChar = '$';
					nCountInItem = 0;
					dwLen += (sizeof(IMAGE_OBJECT_HEAD) + 14 * nDigitByteWidth * nFontHeight * GetColorCount());
				}
		}
			break;
		}

		if (*ws == 0)
			break;
		ws++;
	}

	if (sysConfig.m_nActShowTime > 1000)
	{
		if (sysConfig.m_nActShowTime % 1000)
			dwLen *= 2;
		dwLen += (sizeof(IMAGE_OBJECT_HEAD) * 2);
	}

	return dwLen;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateActiveFlashImage_Direct(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p)
{
	IMAGE_OBJECT_HEAD* pFlashImageHead;

	if (sysConfig.m_nActShowTime <= 1000)
	{
		p = CreateActiveFlashImage_Direct_1(nActIndex, nAreaIndex, xStart, yStart, p, sysConfig.m_nActShowTime);
		return p;
	}

	pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
	pFlashImageHead->ucObjectType = OBJ_TYPE_START_REPEAT;
	pFlashImageHead->usRepeatCount = sysConfig.m_nActShowTime / 1000;

	p = CreateActiveFlashImage_Direct_1(nActIndex, nAreaIndex, xStart, yStart, (BYTE*)(pFlashImageHead + 1), 1000);

	pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
	pFlashImageHead->ucObjectType = OBJ_TYPE_END_REPEAT;

	p = (BYTE*)(pFlashImageHead + 1);
	if (sysConfig.m_nActShowTime % 1000)
		p = CreateActiveFlashImage_Direct_1(nActIndex, nAreaIndex, xStart, yStart, (BYTE*)(pFlashImageHead + 1), sysConfig.m_nActShowTime % 1000);

	return p;
}

BYTE* TfrmMain::CreateActiveFlashImage_Direct_1(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p, int nShowTime)
{
	IMAGE_OBJECT_HEAD* pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
	IMAGE_OBJECT_HEAD* pFlashImageHead_Last = NULL;
	TStrings *strListText = new TStringList;
	TStrings *strListColor = new TStringList;
	TStrings *strListActive = new TStringList;
	int nRowsPerScreen;
	int nTopSpace, nLineWidth;
	int nOffsetX;
	int y;
	int nVertScreen;
	SIZE sz;
	TEXTMETRIC tm;
	int nFontHeight;

	strListText->SetText(sysConfig.m_sActText.c_str());
	strListColor->SetText(sysConfig.m_sActTextColor.c_str());
	strListActive->SetText(sysConfig.m_sActActiveText.c_str());

	p = (BYTE*)(pFlashImageHead + 1);

	imgActive->Picture->Bitmap->PixelFormat = pf4bit;
	imgActive->Picture->Bitmap->Canvas->Brush->Color = clBlack;
	imgActive->Picture->Bitmap->Canvas->Font->Name = sysConfig.m_sActFontName;
	imgActive->Picture->Bitmap->Canvas->Font->Size = sysConfig.m_nActFontSize;
	imgActive->Picture->Bitmap->Canvas->Font->Style = TFontStyles(sysConfig.m_nActFontStyle);
	imgActive->Picture->Bitmap->Canvas->Font->Color = clRed;
	GetTextMetrics(imgActive->Picture->Bitmap->Canvas->Handle, &tm);
	if (sysConfig.m_bDotFont)
		nFontHeight = sysConfig.m_nDotFontSize;
	else
		nFontHeight = tm.tmHeight;

	int nHanziWidth = GetTextWidth(imgActive->Picture->Bitmap->Canvas, sysConfig.m_bDotFont, sysConfig.m_nDotFontSize, "六");
	int nDigitWidth = GetTextWidth(imgActive->Picture->Bitmap->Canvas, sysConfig.m_bDotFont, sysConfig.m_nDotFontSize, "8");
	int nHanziByteWidth =  (nHanziWidth + 7) / 8;
	int nDigitByteWidth =  (nDigitWidth + 7) / 8;

	if (sysConfig.m_nActVertAlignment)
		nRowsPerScreen = 1;
	else
		nRowsPerScreen = sysConfig.m_nSubAreaHeight[nAreaIndex] / nFontHeight;
	nTopSpace = (sysConfig.m_nSubAreaHeight[nAreaIndex] - nRowsPerScreen * nFontHeight) / (nRowsPerScreen + 1);

	nVertScreen = 0;
	y = nTopSpace;
	for (int nRow = 0; nRow < strListText->Count; nRow++)
	{
		nOffsetX = 0;
		nLineWidth = GetTextWidth(imgActive->Picture->Bitmap->Canvas, sysConfig.m_bDotFont, sysConfig.m_nDotFontSize, strListText->Strings[nRow]);
		if (sysConfig.m_nActHorzAlignment == ALIGN_CENTER)
		{
			if (nLineWidth < sysConfig.m_nSubAreaWidth[nAreaIndex])
				nOffsetX = (sysConfig.m_nSubAreaWidth[nAreaIndex] - nLineWidth) / 2;
		}
		else if (sysConfig.m_nActHorzAlignment == ALIGN_RIGHT)
		{
			if (nLineWidth < sysConfig.m_nSubAreaWidth[nAreaIndex])
				nOffsetX = sysConfig.m_nSubAreaWidth[nAreaIndex] - nLineWidth;
		}

		wchar_t* wsActive = strListActive->Strings[nRow].c_str();
		wchar_t* wsColor = strListColor->Strings[nRow].c_str();
		int nCol = 0;
		while (*wsActive)
		{
			if (*wsActive >= '0' && *wsActive < '0' + 20 + 56)
			{
				p = CreateActiveFlashImage(nActIndex, *wsActive, *wsColor, p);
				int n = 0;

				if (sysConfig.m_bFullColor)
					pFlashImageHead->ucObjectType = OBJ_TYPE_THREE_COLOR_CLOCK;
				else if (sysConfig.m_bDoubleColor)
					pFlashImageHead->ucObjectType = OBJ_TYPE_DOUBLE_COLOR_CLOCK;
				else
					pFlashImageHead->ucObjectType = OBJ_TYPE_SINGLE_COLOR_CLOCK;
				pFlashImageHead->ucActType = *wsActive - '0';
				switch (*wsActive)
				{
				case '0':		//4位数字年(2010)
					pFlashImageHead->usDisplayWidth = nDigitWidth * 4;				//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nDigitByteWidth * 8 * 10;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					n = 4;
					break;
				case '1':		//2位数字年(10)
				case '2':		//2位数字月
				case '3':		//2位数字日
				case '4':		//2位数字小时
				case '5':		//2位数字分钟
				case '6':		//2位数字秒
					pFlashImageHead->usDisplayWidth = nDigitWidth * 2;				//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nDigitByteWidth * 8 * 10;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					n = 2;
					break;
				case '7':		//1个汉字星期(日~六)
					pFlashImageHead->usDisplayWidth = nHanziWidth;					//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nHanziByteWidth * 8 * 7;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					n = 1;
					break;
				case '8':		//2个汉字农历年
					pFlashImageHead->usDisplayWidth = nHanziWidth * 2;				//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nHanziByteWidth * 8 * 22;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					n = 2;
					break;
				case '9':		//2个汉字农历月份
					pFlashImageHead->usDisplayWidth = nHanziWidth * 2;				//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nHanziByteWidth * 8 * 12;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					n  = 2;
					break;
				case '0' + 10:	//2个汉字农历日期
					pFlashImageHead->usDisplayWidth = nHanziWidth * 2;				//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nHanziByteWidth * 8 * 12;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					n  = 2;
					break;
				case '0' + 11:	//3位数字倒计时天数
					pFlashImageHead->usDisplayWidth = nDigitWidth * 3;				//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nDigitByteWidth * 8 * 10;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					n  = 3;
					break;
				case '0' + 12:	//2位数字倒计时天数
					pFlashImageHead->usDisplayWidth = nDigitWidth * 2;				//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nDigitByteWidth * 8 * 10;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					n  = 2;
					break;
				case '0' + 13:	//1位数字倒计时天数
					pFlashImageHead->usDisplayWidth = nDigitWidth * 1;				//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nDigitByteWidth * 8 * 10;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;				//图像高度
					n  = 1;
					break;
				case '0' + 14:	//2位数字温度(-9~99)
				case '0' + 15:	//2位数字湿度(00~99)
					pFlashImageHead->usDisplayWidth = nDigitWidth * 2;				//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nDigitByteWidth * 8 * 13;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					n  = 2;
					break;
				case '0' + 16:	//3位数字温度(-99~99)
					pFlashImageHead->usDisplayWidth = nDigitWidth * 3;				//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nDigitByteWidth * 8 * 13;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					n  = 3;
					break;
				case '0' + 17:	//4位数字温度(-99~99.9")
				case '0' + 18:	//4位数字湿度(0~99.9)
					pFlashImageHead->usDisplayWidth = nDigitWidth * 4;				//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nDigitByteWidth * 8 * 13;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					n  = 4;
					break;
				case '0' + 19:		//2个汉字风向
					pFlashImageHead->usDisplayWidth = nHanziWidth;					//显示区域宽度
					pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
					pFlashImageHead->usImageWidth = nHanziByteWidth * 8 * 5;		//图像宽度
					pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					n = 2;
					break;
				default:
					if ((*wsActive >= '0' + 20) &&(*wsActive < '0' + 20 + 56))		//外部数据
					{
						n = *wsActive - '0' - 20;
						n /= 8;
						n += 1;
						pFlashImageHead->usDisplayWidth = nDigitWidth * n;				//显示区域宽度
						pFlashImageHead->usDisplayHeight = nFontHeight;					//显示区域高度
						pFlashImageHead->usImageWidth = nDigitByteWidth * 8 * 14;		//图像宽度
						pFlashImageHead->usImageHeight = nFontHeight;					//图像高度
					}
				}

				pFlashImageHead->ulPara = 0;   //显示时间us
				pFlashImageHead->usRepeatCount = 1;
				GetTextExtentPoint32W(imgActive->Picture->Bitmap->Canvas->Handle, strListText->Strings[nRow].c_str(), nCol, &sz);
				pFlashImageHead->usDisplayPosX = sz.cx + nOffsetX;	//显示区域坐上角X坐标
				pFlashImageHead->usDisplayPosY = y + nVertScreen * sysConfig.m_nSubAreaHeight[nAreaIndex];	//显示区域坐上角Y坐标
				pFlashImageHead_Last = pFlashImageHead;
				pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
				p = (BYTE*)(pFlashImageHead + 1);

				wsActive += n;
				wsColor += n;
				nCol += n;
			}
			else
			{
				wsActive++;
				wsColor++;
				nCol++;
			}
		}

		 y += nTopSpace + nFontHeight;
		 if (y + nFontHeight > sysConfig.m_nSubAreaHeight[nAreaIndex])
		 {
			y = nTopSpace;
			nVertScreen++;
		 }
	}

	if (pFlashImageHead_Last)
		pFlashImageHead_Last->ulPara = nShowTime * 1000;

	delete strListText;
	delete strListColor;
	delete strListActive;

	return (BYTE*)pFlashImageHead;
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateActiveFlashImage(int nActIndex, wchar_t wcActiveType, wchar_t wcColor, BYTE* p)
{
	int nFontHeight;
	TEXTMETRIC tm;
	GetTextMetrics(imgActive->Picture->Bitmap->Canvas->Handle, &tm);
	if (sysConfig.m_bDotFont)
		nFontHeight = sysConfig.m_nDotFontSize;
	else
		nFontHeight = tm.tmHeight;

	int nHanziWidth = GetTextWidth(imgActive->Picture->Bitmap->Canvas, sysConfig.m_bDotFont, sysConfig.m_nDotFontSize, "六");
	int nDigitWidth = GetTextWidth(imgActive->Picture->Bitmap->Canvas, sysConfig.m_bDotFont, sysConfig.m_nDotFontSize, "0");
	int nHanziByteWidth =  (nHanziWidth + 7) / 8;
	int nDigitByteWidth =  (nDigitWidth + 7) / 8;

	imgActive->Picture->Bitmap->Canvas->Font->Color = clWhite;
	imgActive->Picture->Bitmap->Canvas->Brush->Color = clBlack;

	switch (wcActiveType)
	{
	case '0':		//4位数字年(2010)
	case '1':		//2位数字年(10)
	case '2':		//2位数字月
	case '3':		//2位数字日
	case '4':		//2位数字小时
	case '5':		//2位数字分钟
	case '6':		//2位数字秒
		imgActive->Picture->Bitmap->Width = nDigitByteWidth * 10 * 8;
		imgActive->Picture->Bitmap->Height = nFontHeight;
		imgActive->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, nDigitByteWidth * 10 * 8, nFontHeight));
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 0, 0, '0');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 1, 0, '1');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 2, 0, '2');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 3, 0, '3');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 4, 0, '4');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 5, 0, '5');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 6, 0, '6');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 7, 0, '7');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 8, 0, '8');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 9, 0, '9');
		MakeActiveTextColor(wcColor);
		break;
	case '0' + 11:	//3位数字倒计时天数
	case '0' + 12:	//2位数字倒计时天数
	case '0' + 13:	//1位数字倒计时天数
		*((WORD*)p) = sysConfig.m_nActDownCounterBaseYear; p++; p++;
		*p = sysConfig.m_nActDownCounterBaseMonth; p++;
		*p = sysConfig.m_nActDownCounterBaseDay; p++;
		imgActive->Picture->Bitmap->Width = nDigitByteWidth * 10 * 8;
		imgActive->Picture->Bitmap->Height = nFontHeight;
		imgActive->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, nDigitByteWidth * 10 * 8, nFontHeight));
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 0, 0, '0');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 1, 0, '1');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 2, 0, '2');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 3, 0, '3');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 4, 0, '4');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 5, 0, '5');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 6, 0, '6');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 7, 0, '7');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 8, 0, '8');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 9, 0, '9');
		MakeActiveTextColor(wcColor);
		break;
	case '7':		//1个汉字星期(日~六)
		imgActive->Picture->Bitmap->Width = nHanziByteWidth * 7 * 8;
		imgActive->Picture->Bitmap->Height = nFontHeight;
		imgActive->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, nHanziByteWidth * 7 * 8, nFontHeight));
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 0, 0, L'日');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 1, 0, L'一');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 2, 0, L'二');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 3, 0, L'三');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 4, 0, L'四');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 5, 0, L'五');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 6, 0, L'六');
		MakeActiveTextColor(wcColor);
		break;
	case '8':		//2个汉字农历年
		imgActive->Picture->Bitmap->Width = nHanziByteWidth * 22 * 8;
		imgActive->Picture->Bitmap->Height = nFontHeight;
		imgActive->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, nHanziByteWidth * 22 * 8, nFontHeight));
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 0, 0, L'甲');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 1, 0, L'乙');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 2, 0, L'丙');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 3, 0, L'丁');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 4, 0, L'戊');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 5, 0, L'己');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 6, 0, L'庚');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 7, 0, L'辛');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 8, 0, L'壬');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 9, 0, L'癸');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 10, 0, L'子');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 11, 0, L'丑');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 12, 0, L'寅');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 13, 0, L'卯');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 14, 0, L'辰');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 15, 0, L'巳');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 16, 0, L'午');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 17, 0, L'未');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 18, 0, L'申');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 19, 0, L'酉');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 20, 0, L'戌');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 21, 0, L'亥');
		MakeActiveTextColor(wcColor);
		break;
	case '9':		//2个汉字农历月份
		imgActive->Picture->Bitmap->Width = nHanziByteWidth * 12 * 8;
		imgActive->Picture->Bitmap->Height = nFontHeight;
		imgActive->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, nHanziByteWidth * 12 * 8, nFontHeight));
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 0, 0, L'　');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 1, 0, L'正');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 2, 0, L'二');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 3, 0, L'三');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 4, 0, L'四');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 5, 0, L'五');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 6, 0, L'六');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 7, 0, L'七');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 8, 0, L'八');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 9, 0, L'九');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 10, 0, L'十');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 11, 0, L'一');
		MakeActiveTextColor(wcColor);
		break;
	case '0' + 10:	//2个汉字农历日期
		imgActive->Picture->Bitmap->Width = nHanziByteWidth * 12 * 8;
		imgActive->Picture->Bitmap->Height = nFontHeight;
		imgActive->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, nHanziByteWidth * 11 * 8, nFontHeight));
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 0, 0, L'一');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 1, 0, L'二');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 2, 0, L'三');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 3, 0, L'四');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 4, 0, L'五');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 5, 0, L'六');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 6, 0, L'七');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 7, 0, L'八');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 8, 0, L'九');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 9, 0, L'十');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 10, 0, L'廿');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 11, 0, L'初');
		MakeActiveTextColor(wcColor);
		break;
	case '0' + 14:	//2位数字温度(-9~99)
	case '0' + 15:	//2位数字湿度(00~99)
	case '0' + 16:	//3位数字温度(-99~99)
	case '0' + 17:	//4位数字温度(-99~99.9")
	case '0' + 18:	//4位数字湿度(0~99.9)
		imgActive->Picture->Bitmap->Width = nDigitByteWidth * 13 * 8;
		imgActive->Picture->Bitmap->Height = nFontHeight;
		imgActive->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, nDigitByteWidth * 13 * 8, nFontHeight));
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 0, 0, '0');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 1, 0, '1');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 2, 0, '2');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 3, 0, '3');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 4, 0, '4');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 5, 0, '5');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 6, 0, '6');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 7, 0, '7');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 8, 0, '8');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 9, 0, '9');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 10, 0, '+');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 11, 0, '-');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 12, 0, '.');
		MakeActiveTextColor(wcColor);
		break;
	case '0' + 19:	//2位汉字风向
		imgActive->Picture->Bitmap->Width = nHanziByteWidth * 5 * 8;
		imgActive->Picture->Bitmap->Height = nFontHeight;
		imgActive->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, nHanziByteWidth * 5 * 8, nFontHeight));
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 0, 0, L'东');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 1, 0, L'南');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 2, 0, L'西');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 3, 0, L'北');
		DrawChar(imgActive->Picture->Bitmap->Canvas, nHanziByteWidth * 8 * 4, 0, L'　');
		MakeActiveTextColor(wcColor);
		break;
	default:
		if ((wcActiveType >= '0' + 20) &&(wcActiveType < '0' + 20 + 56))		//外部数据
		{
			imgActive->Picture->Bitmap->Width = nDigitByteWidth * 14 * 8;
			imgActive->Picture->Bitmap->Height = nFontHeight;
			imgActive->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, nDigitByteWidth * 14 * 8, nFontHeight));
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 0, 0, '0');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 1, 0, '1');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 2, 0, '2');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 3, 0, '3');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 4, 0, '4');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 5, 0, '5');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 6, 0, '6');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 7, 0, '7');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 8, 0, '8');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 9, 0, '9');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 10, 0, '+');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 11, 0, '-');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 12, 0, '.');
			DrawChar(imgActive->Picture->Bitmap->Canvas, nDigitByteWidth * 8 * 13, 0, ' ');
			MakeActiveTextColor(wcColor);
		}
	}

	BYTE c;
	int x, y, i;
	for (y = 0; y < imgActive->Picture->Bitmap->Height; y++)
	{
		c = 0;
		i = 0;
		for (x = 0; x < imgActive->Picture->Bitmap->Width; x++)
		{
			c <<= 1;
			if (imgActive->Picture->Bitmap->Canvas->Pixels[x][y] & RGB(255, 0, 0))
				c |= 1;
			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}
	}

	if (sysConfig.m_bDoubleColor || sysConfig.m_bFullColor)
	{
		for (y = 0; y < imgActive->Picture->Bitmap->Height; y++)
		{
			c = 0;
			i = 0;
			for (x = 0; x < imgActive->Picture->Bitmap->Width; x++)
			{
				c <<= 1;
				if (imgActive->Picture->Bitmap->Canvas->Pixels[x][y] & RGB(0, 255, 0))
					c |= 1;
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	if (sysConfig.m_bFullColor)
	{
		for (y = 0; y < imgActive->Picture->Bitmap->Height; y++)
		{
			c = 0;
			i = 0;
			for (x = 0; x < imgActive->Picture->Bitmap->Width; x++)
			{
				c <<= 1;
				if (imgActive->Picture->Bitmap->Canvas->Pixels[x][y] & RGB(0, 0, 255))
					c |= 1;
				i++;
				if (i >= 8)
				{
					*p++ = c;
					i = 0;
					c =0;
				}
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

void TfrmMain::MakeActiveTextColor(wchar_t wcColor)
{
	DWORD crFColor, crBColor;
	utils.GetRGBColor(wcColor, '$', &crFColor, &crBColor);

	imgActive->Picture->Bitmap->Canvas->Brush->Style = bsSolid;
	imgActive->Picture->Bitmap->Canvas->Brush->Color = TColor(crFColor ^ crBColor);
	SetROP2(imgActive->Picture->Bitmap->Canvas->Handle, R2_MASKPEN);
	imgActive->Picture->Bitmap->Canvas->Rectangle(-1, -1, imgActive->Picture->Bitmap->Width + 1, imgActive->Picture->Bitmap->Height + 1);
	imgActive->Picture->Bitmap->Canvas->Brush->Color = TColor(crBColor);
	SetROP2(imgActive->Picture->Bitmap->Canvas->Handle, R2_XORPEN);
	imgActive->Picture->Bitmap->Canvas->Rectangle(-1, -1, imgActive->Picture->Bitmap->Width + 1, imgActive->Picture->Bitmap->Height + 1);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuSetTextNormalClick(TObject *Sender)
{
	wndTextEdit->SetTextActive('$');
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::vtGetNodeDataSize(TBaseVirtualTree *Sender,
	  int &NodeDataSize)
{
	NodeDataSize = sizeof(CDtu);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::vtGetText(TBaseVirtualTree *Sender,
	  PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
	  UnicodeString &CellText)
{
	CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));

	if (Column == 0)
	{
		if (TextType == ttNormal)
			CellText = dtu->m_sName;
		else
		{
			if (dtu->m_nNodeType == NODE_CARD)
				CellText = UnicodeString(dtu->m_sCode);
			else
				CellText = "";
		}
	}
	else if (Column == 2)
	{
		if (dtu->m_nNodeType == NODE_CARD)
		{
			if (TextType == ttNormal)
				CellText = dtu->m_sStatus;
			else
				CellText = "";
		}
		else
			CellText = "";
	}
	else
		CellText = "";
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::vtInitNode(TBaseVirtualTree *Sender,
	  PVirtualNode ParentNode, PVirtualNode Node,
	  TVirtualNodeInitStates &InitialStates)
{
	CDtu* pNodeData = (CDtu*)(vt->GetNodeData(Node));
	if ((pNodeData->m_nNodeType == NODE_GROUP) || (pNodeData->m_nNodeType == NODE_ALL_CARD_ROOT))
		InitialStates = InitialStates<<ivsHasChildren;
	else
		InitialStates = InitialStates>>ivsHasChildren;
	if ((Node->ChildCount == 0) || (pNodeData->m_nNodeType == NODE_CARD))
		InitialStates = InitialStates<<ivsExpanded;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::vtGetImageIndex(TBaseVirtualTree *Sender,
	  PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column, bool &Ghosted,
	  int &ImageIndex)
{
	if (Column == 0)
	{
		CDtu* pNodeData = (CDtu*)(vt->GetNodeData(Node));
		if (pNodeData->m_nNodeType == NODE_CARD)
			ImageIndex = 0;
		else
			ImageIndex = 1;
	}
	else if (Column == 1)
	{
		if (Sender->FocusedNode == Node)
			ImageIndex = 2;
		else
			ImageIndex = -1;
	}
	else
		ImageIndex = -1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::vtPaintText(TBaseVirtualTree *Sender,
	  const TCanvas *TargetCanvas, PVirtualNode Node, TColumnIndex Column,
	  TVSTTextType TextType)
{
	CDtu* pNodeData = (CDtu*)(vt->GetNodeData(Node));
	if (pNodeData->m_nNodeType == NODE_GROUP || pNodeData->m_nNodeType == NODE_ALL_CARD_ROOT)
		TargetCanvas->Font->Style = TargetCanvas->Font->Style << fsBold;
	else
		TargetCanvas->Font->Style = TargetCanvas->Font->Style >> fsBold;

	if (Node == Sender->HotNode)
		TargetCanvas->Font->Color = clRed;
	else
	{
		if (TextType == ttStatic)
			TargetCanvas->Font->Color = clBlue;
		else
			TargetCanvas->Font->Color = clWindowText;
	}
}
//---------------------------------------------------------------------------

int TfrmMain::GetCheckedCardCount()
{
	PVirtualNode Node;
	CDtu* dtu;
	int nSelected = 0;

	Node = vt->GetFirstChecked(csCheckedNormal);
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
			nSelected++;
		Node = vt->GetNextChecked(Node);
	}

	return nSelected;
}
//---------------------------------------------------------------------------

PVirtualNode TfrmMain::GetFirstCheckedCard()
{
	CDtu* dtu;
	PVirtualNode Node = vt->GetFirstChecked(csCheckedNormal);

	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
			break;
		Node = vt->GetNextChecked(Node);
	}

	return Node;
}

PVirtualNode TfrmMain::GetNextCheckedCard(PVirtualNode Node)
{
	CDtu* dtu;

	Node = vt->GetNextChecked(Node);
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
			break;
		Node = vt->GetNextChecked(Node);
	}

	return Node;
}
//---------------------------------------------------------------------------

void TfrmMain::StartOperation()
{
	vt->Enabled = false;
	listActs->Enabled = false;
	panelScreen->Enabled = false;
	panelMisc->Enabled = false;
	toolBar->Enabled = false;

	mnuSystem->Enabled = false;
	mnuSelect->Enabled = false;
	mnuEdit->Enabled = false;
	mnuEdit->Enabled = false;
	mnuCommunication->Enabled = false;
	mnuLog->Enabled = false;
	mnuLanguage->Enabled = false;
	mnuHelp->Enabled = false;
}
//---------------------------------------------------------------------------

void TfrmMain::EndOperation()
{
	vt->Enabled = true;
	listActs->Enabled = true;
	panelScreen->Enabled = true;
	panelMisc->Enabled = true;
	toolBar->Enabled = true;

	mnuSystem->Enabled = true;
	mnuSelect->Enabled = true;
	mnuEdit->Enabled = true;
	mnuEdit->Enabled = true;
	mnuCommunication->Enabled = true;
	mnuLog->Enabled = true;
	mnuLanguage->Enabled = true;
	mnuHelp->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnFindNextCardClick(TObject *Sender)
{
	FindNextCard();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnFindNextActClick(TObject *Sender)
{
	FindNextAct();
}
//---------------------------------------------------------------------------

bool TfrmMain::OpenCommPort(int nTxAct, UnicodeString sTitle)
{
	PVirtualNode Node;
	CDtu* dtu;
	int nTxStatus = TX_STATUS_READY;

	if (sysConfig.m_nCommMethod == 0)	//串口
	{
		m_ComPort.SetPortName(sysConfig.m_sComPort);
		m_ComPort.SetBaudRate(sysConfig.m_nBaudRate);
		if (!m_ComPort.Open())
		{
			if (m_nLanguage == LANGUAGE_CHINESE)
			{
				sTitle = "串口:";
				sTitle += sysConfig.m_sComPort;
				sTitle += " 打开失败";
			}
			else
			{
				sTitle = "Failed openning serial port: ";
				sTitle += sysConfig.m_sComPort;
			}
			nTxStatus = TX_STATUS_END_TIME_OUT;
		}
		else
		{
			m_ComPort.Close();
		}
	}
	else
	{
		try
		{
			udpSocket->Host = sysConfig.m_sServer;
			udpSocket->Port = sysConfig.m_nSocketPort;
			udpSocket->Active = true;
			nTxStatus = TX_STATUS_READY;
		}
		catch (Exception &ex)
		{
			if (m_nLanguage == LANGUAGE_CHINESE)
				sTitle = "网络连接失败";
			else
				sTitle = "Network Error";
		}
	}

	Node = vt->GetFirst();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
		{
			if (Node->CheckState != csCheckedNormal)
				dtu->m_sStatus = "";
			else
			{
				dtu->m_nRxLen = 0;
				dtu->m_nTxAct = nTxAct;
				dtu->m_nTxStep = 0;
				dtu->m_nDataSent = 0;
				dtu->m_nRetries = 0;
				dtu->m_nComPortRxStep = 0;
				dtu->m_nTxStatus = nTxStatus;
				dtu->m_sStatus = sTitle;
			}
		}

		Node = vt->GetNext(Node);
	}

	vt->Refresh();

	return (nTxStatus == TX_STATUS_READY);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuSaveActFileClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif
	SaveAct();

	try
	{
		if (ActFileSaveDialog->Execute())
		{
			sysConfig.SaveActToFile(ActFileSaveDialog->FileName);
		}
	}
	catch (Exception &ex)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			::MessageBox(this->Handle, L"节目文件导出失败!", NULL, MB_OK | MB_ICONWARNING);
		else
			::MessageBox(this->Handle, L"Export act file failed!", NULL, MB_OK | MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuOpenActFileClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif
	try
	{
		if (ActFileOpenDialog->Execute())
		{
			sysConfig.OpenActFile(ActFileOpenDialog->FileName);
			sysConfig.SaveAct(m_dtu, m_nActIndex);
			sysConfig.LoadAct(m_dtu, m_nActIndex);
			UpdateActVol();
			wndTextEdit->SetCaret();
			wndTextEdit->Refresh();
		}
	}
	catch (Exception &ex)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			::MessageBox(this->Handle, L"节目文件导入失败!", NULL, MB_OK | MB_ICONWARNING);
		else
			::MessageBox(this->Handle, L"Import act file failed!", NULL, MB_OK | MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuViewFailedTxActClick(TObject *Sender)
{
/*	PVirtualNode Node;
	CDtu* dtu;
	int i;

	Node = vt->GetFirstChecked(csCheckedNormal);
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
		{
			dtu->m_sStatus = "";
			for (i = 0; i < sysConfig.m_nMaxActCount; i++)
			{
				if (!dtu->m_bActTxOk[i])
				{
					dtu->m_sStatus += UnicodeString(i + 1);
					dtu->m_sStatus += " ";
				}
			}

			if (dtu->m_sStatus.IsEmpty())
				dtu->m_sStatus = "-";

			vt->InvalidateNode(Node);
		}
		Node = vt->GetNextChecked(Node);
	}*/
}
//---------------------------------------------------------------------------

void TfrmMain::SaveCardTxStatus()
{
	PVirtualNode Node;
	CDtu* dtu;

	Node = vt->GetFirstChecked(csCheckedNormal);

	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
			dtu->WriteTxActStatus();
		Node = vt->GetNextChecked(Node);
	}
}
//---------------------------------------------------------------------------

int TfrmMain::GetReTxCardCount()
{
	PVirtualNode Node;
	CDtu* dtu;
	int i;
	int nCount = 0;

	Node = vt->GetFirst();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
		{
			for (i = 0; i < sysConfig.m_nMaxActCount; i++)
			{
				if (!dtu->m_bActTxOk[i])
				{
					nCount++;
					break;
				}
			}
		}
		Node = vt->GetNext(Node);
	}

	return nCount;
}
//---------------------------------------------------------------------------

void TfrmMain::ClearAllTxOkStatus()
{
	PVirtualNode Node;
	CDtu* dtu;

	Node = vt->GetFirst();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
			dtu->ClearActTxOkStatus();
		Node = vt->GetNext(Node);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuManualReTxActClick(TObject *Sender)
{
	int nSelected;

	SaveAct();

	nSelected  = GetReTxCardCount();
	if (!nSelected)
		return;

	InitReTxCardStatus(FALSE, "");

	if (m_nLanguage == LANGUAGE_CHINESE)
	{
		if (!OpenCommPort(0, "发送节目内容"))
			return;
	}
	else
	{
		if (!OpenCommPort(0, "Send Acts"))
			return;
	}

	mnuCancelAllCardClick(this);
	if (m_nLanguage == LANGUAGE_CHINESE)
		InitReTxCardStatus(TRUE, "正在暂停显示");
	else
		InitReTxCardStatus(TRUE, "Stopping Display");
	StartOperation();
	m_bReTx = true;
	timerTxAct->Enabled = true;
}
//---------------------------------------------------------------------------

void TfrmMain::AutoReTxAct()
{
	int nSelected;

	SaveAct();

	nSelected  = GetReTxCardCount();
	if (!nSelected)
		return;

	InitReTxCardStatus(FALSE, "");

	if (m_nLanguage == LANGUAGE_CHINESE)
	{
		if (!OpenCommPort(0, "发送节目内容"))
			return;
	}
	else
	{
		if (!OpenCommPort(0, "Send Acts"))
			return;
	}

	mnuCancelAllCardClick(this);
	if (m_nLanguage == LANGUAGE_CHINESE)
		InitReTxCardStatus(TRUE, "正在暂停显示");
	else
		InitReTxCardStatus(TRUE, "Stopping Display");
	StartOperation();
	m_bReTx = true;
	timerTxAct->Enabled = true;
}
//---------------------------------------------------------------------------

bool TfrmMain::CheckIfAnyActTxFailed()
{
	PVirtualNode Node;
	CDtu* dtu;
	int i;

	Node = vt->GetFirst();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
		{
			for (i = 0; i < sysConfig.m_nMaxActCount; i++)
			{
				if (!dtu->m_bActTxOk[i])
					return true;
			}
		}
		Node = vt->GetNext(Node);
	}

	return false;
}
//---------------------------------------------------------------------------

void TfrmMain::InitReTxCardStatus(BOOL bLog, UnicodeString s)
{
	PVirtualNode Node = vt->GetFirst();
	int i;

	CDtu* dtuRoot = (CDtu*)(vt->GetNodeData(Node));
	if (bLog)
		log.StartLogTx();

	while (Node != NULL)
	{
		CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
		{
			bool bTx = false;
			for (i = 0; i < sysConfig.m_nMaxActCount; i++)
			{
				if (!dtu->m_bActTxOk[i])
				{
					if (!bTx)
					{
						dtu->m_nTxAct = i;
						dtu->m_nDataSent = 0;
						dtu->m_nTxStep = 0;
						dtu->m_nTxStatus = TX_STATUS_READY;
						Node->CheckState = csCheckedNormal;
						bTx = true;
					}
					if (bLog)
						log.LogAct(dtuRoot, i);
				}
			}
			if (bTx)
				dtu->m_sStatus = s;
			else
				dtu->m_sStatus = "";
			vt->InvalidateNode(Node);
		}
		Node = vt->GetNext(Node);
	}
	if (bLog)
		log.StartLogCard();
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::timer_AutoWaitReTxActTimer(TObject *Sender)
{
	if (!vt->Enabled)
		return;

	if (sysConfig.m_nAutoReTxActInterval == 0)
	{
		timer_AutoWaitReTxAct->Enabled = false;
		return;
	}

	m_nWaitReTxTimerCount++;
	if (m_nWaitReTxTimerCount >= sysConfig.m_nAutoReTxActInterval * 60)
	{
		timer_AutoWaitReTxAct->Enabled = false;

		TfrmAutoReTx *frmAutoReTx = new TfrmAutoReTx(this);
		int nResult = frmAutoReTx->ShowModal();
		delete frmAutoReTx;

		if (nResult == mrOk)
		{
			AutoReTxAct();
		}
		else if  (nResult == mrNo)
		{
			sysConfig.m_nAutoReTxActInterval = 0;
			sysConfig.SaveConfig();
		}
		else
		{
			m_nWaitReTxTimerCount = 0;
			timer_AutoWaitReTxAct->Enabled = true;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuQueryTxLogsClick(TObject *Sender)
{
	PVirtualNode Node = vt->GetFirst();
	CDtu* dtuRoot = (CDtu*)(vt->GetNodeData(Node));

	TfrmTxLogView *frmTxLogView = new TfrmTxLogView(this, dtuRoot);
	frmTxLogView->ShowModal();
	delete frmTxLogView;
}
//---------------------------------------------------------------------------

void TfrmMain::ChangeLanguage(int nLanguge)
{
	m_nLanguage = nLanguge;
	sysConfig.m_nLanguage = nLanguge;

	int nEnterStyle = comboEnterStyle->ItemIndex;
	int nExitStyle = comboExitStyle->ItemIndex;

	if (m_nLanguage == LANGUAGE_CHINESE)
	{
		mnuImportV3->Visible = true;
		mnuChinese->Checked = true;
		UpdateCaption();

		mnuCacelWarnning->Visible = true;
		mnuParkDigit->Visible = true;
		mnuAutoCheckActChange->Visible = true;
		mnuReTxSetup->Caption = "配置节目自动重发(&R)...";
		mnuAutoWriteClockSetup->Visible = true;

		mnuLanguage->Caption = "L&anguage";
		mnuSystem->Caption = "系统(&S)";
		mnuSelect->Caption = "选择(&L)";
		mnuEdit->Caption = "编辑(&E)";
		mnuLog->Caption = "日志(&G)";
		mnuHelp->Caption = "帮助(&H)";
		mnuCommunication->Caption = "通信(&C)";
		btnFont->Caption = "字体...";
		btnCalcDisplayTime->Caption = "刷新播放时间";
		btnCalcDisplayTime->Width = 112;
		lbPlaytime->Left = 136;
		lbDisplayTime->Left = 244;
		lbPlayTimeAll->Left = 348;
		lbAllDisplayTime->Left = 420;
		lbPlaytime->Caption = "当前节目播放时间:";
		lbPlayTimeAll->Caption = "总播放时间:";
		vt->Header->Columns[0].Items[0]->Text = "名称/描述/编码";
		vt->Header->Columns[0].Items[2]->Text = "通信状态/结果";
		listActs->Columns[0].Items[0]->Caption = "节目编号";
		listActs->Columns[0].Items[1]->Caption = "分区";
		listActs->Columns[0].Items[2]->Caption = "节目内容说明";
		lbEditingAct->Caption = "选择要编辑的节目";
		comboEditAct->Left = 127;
		lbAreaOfAct->Caption = "该节目所属分区";
		comboArea->Left = 323;
		lbEnterStyle->Caption = "进入方式";
		comboEnterStyle->Left = 79;
		lbExitStyle->Caption = "退出方式";
		comboExitStyle->Left = 79;
		comboEnterStyle->Items->Strings[0] = "直接显示";
		comboEnterStyle->Items->Strings[1] = "连续左移";
		comboEnterStyle->Items->Strings[2] = "连续右移";
		comboEnterStyle->Items->Strings[3] = "连续上移";
		comboEnterStyle->Items->Strings[4] = "连续下移";
		comboEnterStyle->Items->Strings[5] = "向左移入";
		comboEnterStyle->Items->Strings[6] = "向右移入";
		comboEnterStyle->Items->Strings[7] = "向上移入";
		comboEnterStyle->Items->Strings[8] = "向下移入";
		comboEnterStyle->Items->Strings[9] = "向右展开";
		comboEnterStyle->Items->Strings[10] = "向左展开";
		comboEnterStyle->Items->Strings[11] = "向下展开";
		comboEnterStyle->Items->Strings[12] = "向上展开";
		comboEnterStyle->Items->Strings[13] = "从左右向中间展开";
		comboEnterStyle->Items->Strings[14] = "从中间向左右展开";
		comboEnterStyle->Items->Strings[15] = "从上下向中间展开";
		comboEnterStyle->Items->Strings[16] = "从中间向上下展开";
		comboEnterStyle->Items->Strings[17] = "水平百叶窗";
		comboEnterStyle->Items->Strings[18] = "垂直百叶窗";
		comboEnterStyle->Items->Strings[19] = "闪烁";
		lbShutterWidth->Left = 297;
		lbShutterWidth->Caption = "百叶窗叶片宽度";
		editShutterWidth->Left = 387;
		comboExitStyle->Items->Strings[0] = "直接清除";
		comboExitStyle->Items->Strings[1] = "向左移出";
		comboExitStyle->Items->Strings[2] = "向右移出";
		comboExitStyle->Items->Strings[3] = "向上移出";
		comboExitStyle->Items->Strings[4] = "向下移出";
		comboExitStyle->Items->Strings[5] = "向右擦除";
		comboExitStyle->Items->Strings[6] = "向左擦除";
		comboExitStyle->Items->Strings[7] = "向下擦除";
		comboExitStyle->Items->Strings[8] = "向上擦除";
		comboExitStyle->Items->Strings[9] = "从左右向中间擦除";
		comboExitStyle->Items->Strings[10] = "从中间向左右擦除";
		comboExitStyle->Items->Strings[11] = "从上下向中间擦除";
		comboExitStyle->Items->Strings[12] = "从中间向上下擦除";
		comboExitStyle->Items->Strings[13] = "水平百叶窗";
		comboExitStyle->Items->Strings[14] = "垂直百叶窗";
		comboExitStyle->Items->Strings[15] = "其它";
		comboExitStyle->Items->Strings[16] = "无";
		lbEnterSpeed->Caption = "进入速度";
		editEnterSpeed->Left = 79;
		lbSpeed1->Left = 143;
		lbSpeed1->Caption = "毫秒/像素";
		lbExitSpeed->Caption = "退出速度";
		editExitSpeed->Left = 287;
		lbSpeed2->Left = 351;
		lbSpeed2->Caption = "毫秒/像素";
		lbStayTime->Caption = "停留时间";
		editShowTime->Left = 79;
		lbRepeatCount->Caption = "重复播放次数";
		lbRefHint1->Caption = "当节目";
		lbRefHint2->Caption = "播放完成后该节目停播";
		cbRefActIndex->Left = 411;
		lbRefHint2->Left = 489;
		lbStayTimeHint->Caption = "毫秒";
		lbStayTimeHint->Left = 143;
		lbStart->Caption = "起始时间";
		dateActStart->Left = 79;
		timeActStart->Left = 202;
		lbEnd->Caption = "终止时间";
		dateActEnd->Left = 336;
		timeActEnd->Left = 463;
		groupBoxPeriod->Caption = "每日播放时段";
		btnLoadPicture->Caption = "载入图片文件...";
		lbPictureX->Caption = "图片水平位置";
		lbPictureY->Caption = "图片垂直位置";
		editPicPosX->Left = 485;
		editPicPosY->Left = 485;
		editActDesc->EditLabel->Caption = "节目内容说明";
		groupWeekPeriod->Caption = "每周播放时段";
		lbStartWeek->Caption ="起始：";
		lbEndWeek->Caption ="终止：";
		cbStartWeek->Items->Clear();
		cbStartWeek->Items->Add("星期日");
		cbStartWeek->Items->Add("星期一");
		cbStartWeek->Items->Add("星期二");
		cbStartWeek->Items->Add("星期三");
		cbStartWeek->Items->Add("星期四");
		cbStartWeek->Items->Add("星期五");
		cbStartWeek->Items->Add("星期六");
		cbEndWeek->Items->Clear();
		cbEndWeek->Items->Add("星期日");
		cbEndWeek->Items->Add("星期一");
		cbEndWeek->Items->Add("星期二");
		cbEndWeek->Items->Add("星期三");
		cbEndWeek->Items->Add("星期四");
		cbEndWeek->Items->Add("星期五");
		cbEndWeek->Items->Add("星期六");

		mnuCardManage->Caption = "控制卡管理(&R)...";
		mnuCommConfig->Caption = "通信配置(&C)...";
		mnuCardConfig->Caption = "控制卡配置(&S)...";
		mnuOpen->Caption = "导入系统配置和所有节目内容(&O)...";
		mnuImportV3->Caption = "导入GS7020BManV3.X中的控制卡定义及节目内容(&I)...";
		mnuSave->Caption = "导出系统配置和所有节目内容(&A)...";
		mnuAdminPassword->Caption = "管理员密码(&P)...";
		mnuRootPath->Caption = "该软件管理的所有显示屏根节点名称(&N)...";
		mnuCardType->Caption = "控制卡型号选择(&T)...";
		mnuUpdateProg->Caption = "控制卡软件升级(&U)";

		mnuSelectAllAct->Caption = "选择全部节目(&S)";
		mnuCancelAllAct->Caption = "取消选择所有节目(&C)";
		mnuSelectAllCard->Caption = "选择所有显示屏(控制卡)(&R)";
		mnuCancelAllCard->Caption = "取消选择所有显示屏(控制卡)(&E)";
		mnuSelectAllSuccessed->Caption = "选择所有上次通信成功的显示屏(控制卡)(&U)";
		mnuSelectAllFailed->Caption = "选择所有上次通信失败的显示屏(控制卡)(&F)";

		mnuPreviewCurrentAct->Caption = "预览当前编辑的节目(&P)";
		mnuPreviewAllAct->Caption = "预览所有节目(&V)";
		mnuSearchCard->Caption = "查找显示屏(&C)...";
		mnuSearchAct->Caption = "查找节目(&A)...";
		mnuSetTextActive->Caption = "将所选文字设定为动态变化项(&E)... ";
		mnuSetTextNormal->Caption = "恢复所选文字为固定显示内容(&M)";
		mnuDeleteAct->Caption = "删除选择的节目的全部内容(&D)";
		mnuDeleteText->Caption = "删除选择的节目的文字内容(&T)";
		mnuDeletePicture->Caption = "删除选择的节目的图片内容(&R)";
		mnuSaveActFile->Caption = "导出当前节目(&O)...";
		mnuOpenActFile->Caption = "从节目文件导入到当前节目(&I)...";

		mnuSendAct->Caption = "发送节目内容(&T)";
		mnuManualReTxAct->Caption = "重新发送上次未成功发送的节目内容(&R)";
		mnuViewFailedTxAct->Caption = "查看所选显示屏未成功发送的节目(&V)";
		mnuCheckCardOnline->Caption = "查询控制卡在线状态(&L)";
		mnuReadCardClock->Caption = "查询控制卡时钟(&O)";
		mnuAdjustCardClock->Caption = "校对控制卡时钟(&C)";
		mnuManualCloseScreen->Caption = "手动关闭屏幕显示(&N)";
		mnuManualOpenScreen->Caption = "手动打开屏幕显示(&F)";
		mnuQueryTxLogs->Caption = "查看节目发送记录(&A)...";
		mnuAbout->Caption = "关于(&A)...";
		btnCardConfig->Hint = "控制卡配置";
		btnCardList->Hint = "控制卡管理";
		btnCommConfig->Hint = "通信配置";
		btnSelectAllCard->Hint = "选择所有显示屏";
		btnUnSelectAllCard->Hint = "取消选择所有显示屏";
		btnSelectAllAct->Hint = "选择所有节目";
		btnUnSelectAllAct->Hint = "取消选择所有节目";
		btnDeleteActContent->Hint = "删除选择的节目内容";
		btnPreview->Hint = "显示预览";
		btnFindNextCard->Hint = "查找下一个显示屏";
		btnFindNextAct->Hint = "查找下一个节目";
		btnSendAct->Hint = "发送节目";
		btnCheckOnline->Hint = "检查控制卡在线状态";
		btnReadCardClock->Hint = "查询控制卡时钟";
		btnWriteCardClock->Hint = "校对控制卡时钟";
	}
	else
	{
		mnuImportV3->Visible = false;
		mnuEnglish->Checked = true;
		UpdateCaption();

		mnuCacelWarnning->Visible = false;
		mnuParkDigit->Visible = false;
		mnuAutoCheckActChange->Visible = false;
		mnuReTxSetup->Caption = "Configure Re-sending...";
		mnuAutoWriteClockSetup->Visible = false;
		mnuCheckResult->Visible = false;

		mnuLanguage->Caption = "语言(&A)";
		mnuSystem->Caption = "&System";
		mnuSelect->Caption = "Se&lect";
		mnuEdit->Caption = "&Edit";
		mnuLog->Caption = "Lo&g";
		mnuHelp->Caption = "&Help";
		mnuCommunication->Caption = "&Communication";
		btnFont->Caption = "Font...";
		btnCalcDisplayTime->Caption = "Refresh Playing Time";
		btnCalcDisplayTime->Width = 140;
		lbPlaytime->Left = 180;
		lbDisplayTime->Left = 340;
		lbPlayTimeAll->Left = 410;
		lbAllDisplayTime->Left = 550;
		lbPlaytime->Caption = "Playing Time(Current Act):";
		lbPlayTimeAll->Caption = "Playing Time(All Acts):";
		vt->Header->Columns[0].Items[0]->Text = "Name/Descriptions/Code";
		vt->Header->Columns[0].Items[2]->Text = "Status/Result";
		listActs->Columns[0].Items[0]->Caption = "Act ID";
		listActs->Columns[0].Items[1]->Caption = "Area";
		listActs->Columns[0].Items[2]->Caption = "Content Descriptions";
		lbEditingAct->Caption = "Current Act ID";
		comboEditAct->Left = 120;
		lbAreaOfAct->Caption = "Area of Act";
		comboArea->Left = 308;
		lbEnterStyle->Caption = "Enter Style";
		comboEnterStyle->Left = 96;
		lbExitStyle->Caption = "Exit Style";
		comboExitStyle->Left = 90;
		comboEnterStyle->Items->Strings[0] = "Direct Display";
		comboEnterStyle->Items->Strings[1] = "Scroll Left";
		comboEnterStyle->Items->Strings[2] = "Scroll Right";
		comboEnterStyle->Items->Strings[3] = "Scroll Up";
		comboEnterStyle->Items->Strings[4] = "Scroll Down";
		comboEnterStyle->Items->Strings[5] = "Move Left";
		comboEnterStyle->Items->Strings[6] = "Move Right";
		comboEnterStyle->Items->Strings[7] = "Move Up";
		comboEnterStyle->Items->Strings[8] = "Move Down";
		comboEnterStyle->Items->Strings[9] = "Spread Right";
		comboEnterStyle->Items->Strings[10] = "Spread Left";
		comboEnterStyle->Items->Strings[11] = "Spread Down";
		comboEnterStyle->Items->Strings[12] = "Spread Up";
		comboEnterStyle->Items->Strings[13] = "Spread To Center Horizontal";
		comboEnterStyle->Items->Strings[14] = "Spread From Center Horizontal";
		comboEnterStyle->Items->Strings[15] = "Spread To Center Vertical";
		comboEnterStyle->Items->Strings[16] = "Spread From Center Vertical";
		comboEnterStyle->Items->Strings[17] = "Horizontal Shutters";
		comboEnterStyle->Items->Strings[18] = "Vertical Shutters";
		lbShutterWidth->Left = 312;
		lbShutterWidth->Caption = "Shutter Width";
		editShutterWidth->Left = 396;
		comboExitStyle->Items->Strings[0] = "Direct Erase";
		comboExitStyle->Items->Strings[1] = "Move Left Out";
		comboExitStyle->Items->Strings[2] = "Move Right Out";
		comboExitStyle->Items->Strings[3] = "Move Up Out";
		comboExitStyle->Items->Strings[4] = "Move Down Out";
		comboExitStyle->Items->Strings[5] = "Erase Right";
		comboExitStyle->Items->Strings[6] = "Erase Left";
		comboExitStyle->Items->Strings[7] = "Erase Down";
		comboExitStyle->Items->Strings[8] = "Erase Up";
		comboExitStyle->Items->Strings[9] = "Erase To Center Horizontal";
		comboExitStyle->Items->Strings[10] = "Erase From Center Horizontal";
		comboExitStyle->Items->Strings[11] = "Erase To Center Vertical";
		comboExitStyle->Items->Strings[12] = "Erase From Center Vertical";
		comboExitStyle->Items->Strings[13] = "Horizontal Shutters";
		comboExitStyle->Items->Strings[14] = "Vertical Shutters";
		comboExitStyle->Items->Strings[15] = "Reserved";
		comboExitStyle->Items->Strings[16] = "Null";
		lbEnterSpeed->Caption = "Enter Speed";
		editEnterSpeed->Left = 96;
		lbSpeed1->Left = 158;
		lbSpeed1->Caption = "ms/pixel";
		lbExitSpeed->Caption = "Exit Speed";
		editExitSpeed->Left = 296;
		lbSpeed2->Left = 360;
		lbSpeed2->Caption = "ms/pixel";
		lbStayTime->Caption = "Stay Time";
		editShowTime->Left = 88;
		lbStayTimeHint->Caption = "ms";
		lbStayTimeHint->Left = 150;
		lbRepeatCount->Caption = "Repeat Count";
		lbRefHint1->Caption = "Stop Playing when Act";
		lbRefHint2->Caption = "End";
		cbRefActIndex->Left = 500;
		lbRefHint2->Left = 580;
		lbStart->Caption = "Start Time";
		dateActStart->Left = 89;
		timeActStart->Left = 212;
		lbEnd->Caption = "End Time";
		dateActEnd->Left = 336;
		timeActEnd->Left = 463;
		groupBoxPeriod->Caption = "Playing Periods In a Day";
		btnLoadPicture->Caption = "Load Picture File...";
		lbPictureX->Caption = "Picture Position X";
		lbPictureY->Caption = "Picture Position Y";
		editPicPosX->Left = 520;
		editPicPosY->Left = 520;
		editActDesc->EditLabel->Caption = "Act Content Descriptions";
		groupWeekPeriod->Caption = "Valid in Week";
		lbStartWeek->Caption ="Start";
		lbEndWeek->Caption ="End";
		cbStartWeek->Items->Clear();
		cbStartWeek->Items->Add("Sunday");
		cbStartWeek->Items->Add("Monday");
		cbStartWeek->Items->Add("Tuesday");
		cbStartWeek->Items->Add("Wednesday");
		cbStartWeek->Items->Add("Thursday");
		cbStartWeek->Items->Add("Friday");
		cbStartWeek->Items->Add("Saturday ");
		cbEndWeek->Items->Clear();
		cbEndWeek->Items->Add("Sunday");
		cbEndWeek->Items->Add("Monday");
		cbEndWeek->Items->Add("Tuesday");
		cbEndWeek->Items->Add("Wednesday");
		cbEndWeek->Items->Add("Thursday");
		cbEndWeek->Items->Add("Friday");
		cbEndWeek->Items->Add("Saturday ");

		mnuCardManage->Caption = "O&rganize Cards...";
		mnuCommConfig->Caption = "&Communication Configurations...";
		mnuCardConfig->Caption = "Card Configuration&s...";
		mnuOpen->Caption = "Imp&ort System Configurations and All Acts...";
		mnuImportV3->Caption = "&Import System Configurations and All Acts in GS7020BManV3.X...";
		mnuSave->Caption = "Export &System Configurations and All Acts...";
		mnuAdminPassword->Caption = "Operating &Password...";
		mnuRootPath->Caption = "Select or Change the Root &Name of LED Signs...";
		mnuCardType->Caption = "Select Card &Type...";
		mnuUpdateProg->Caption = "&Update Software of Cards";

		mnuSelectAllAct->Caption = "&Select All Acts";
		mnuCancelAllAct->Caption = "&Unselect All Acts";
		mnuSelectAllCard->Caption = "Select All LED &Panels";
		mnuCancelAllCard->Caption = "Uns&elect All LED Panels";
		mnuSelectAllSuccessed->Caption = "Select All LED Panels Successed on Last Communication";
		mnuSelectAllFailed->Caption = "Select All LED Panels Failed on Last Communication";
		mnuPreviewCurrentAct->Caption = "&Preview Current Act";
		mnuPreviewAllAct->Caption = "Pre&view All Acts";
		mnuSearchCard->Caption = "&Search LED Panel...";
		mnuSearchAct->Caption = "Search &Act...";
		mnuSetTextActive->Caption = "Set Selected Te&xt to be Active... ";
		mnuSetTextNormal->Caption = "Set Selected Text to be &Normal";
		mnuDeleteAct->Caption = "&Delete All Contents of The Selected Acts";
		mnuDeleteText->Caption = "Delete &Text Contents  of The Selected Acts";
		mnuDeletePicture->Caption = "Delete Pictu&re Contents  of The Selected Acts";
		mnuSaveActFile->Caption = "Exp&ort Current Act to File...";
		mnuOpenActFile->Caption = "&Import Saved File to Current Act...";
		mnuSendAct->Caption = "&Send Acts";
		mnuManualReTxAct->Caption = "Send &Failed Acts Again";
		mnuViewFailedTxAct->Caption = "&View Act's ID Not Sent Successfully of The Selected Panels";
		mnuCheckCardOnline->Caption = "Query On&line Status of Cards";
		mnuReadCardClock->Caption = "Query Cloc&k of Cards";
		mnuAdjustCardClock->Caption = "&Adjust Clock of Cards";
		mnuManualCloseScreen->Caption = "&Close LED Panel's Display";
		mnuManualOpenScreen->Caption = "&Open LED Panel's Display";
		mnuQueryTxLogs->Caption = "&View Sending Logs of Acts...";
		mnuAbout->Caption = "&About...";
		btnCardConfig->Hint = "Card Configuration";
		btnCardList->Hint = "Organize Cards";
		btnCommConfig->Hint = "Communication Configuration";
		btnSelectAllCard->Hint = "Select All LED Panels";
		btnUnSelectAllCard->Hint = "Unselect All LED Panels";
		btnSelectAllAct->Hint = "Select All Acts";
		btnUnSelectAllAct->Hint = "Unselect All Acts";
		btnDeleteActContent->Hint = "Delete All Contents of The Selected Acts";
		btnPreview->Hint = "Preview All Acts";
		btnFindNextCard->Hint = "Search Next LED Panel";
		btnFindNextAct->Hint = "Search Next Act";
		btnSendAct->Hint = "Send Acts";
		btnCheckOnline->Hint = "Check Card Online Status";
		btnReadCardClock->Hint = "Query Clock of Cards";
		btnWriteCardClock->Hint = "Adjust Clock of Cards";
	}

	comboEnterStyle->ItemIndex = nEnterStyle;
	comboExitStyle->ItemIndex = nExitStyle;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuChineseClick(TObject *Sender)
{
	ChangeLanguage(LANGUAGE_CHINESE);
	wndTextEdit->ChangeLanguage();
	sysConfig.SaveConfig();
	lbDisplayTime->Caption = UnicodeString(CalcPlayTime(m_nActIndex) * sysConfig.GetActRepeatCount(m_dtu, m_nActIndex)) + ((sysConfig.m_nLanguage == LANGUAGE_CHINESE) ? " 毫秒" : " ms");
	lbAllDisplayTime->Caption = UnicodeString(CalcAllPlayTime()) + ((sysConfig.m_nLanguage == LANGUAGE_CHINESE) ? " 毫秒" : " ms");
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuEnglishClick(TObject *Sender)
{
	ChangeLanguage(LANGUAGE_ENGLISH);
	wndTextEdit->ChangeLanguage();
	sysConfig.SaveConfig();
	lbDisplayTime->Caption = UnicodeString(CalcPlayTime(m_nActIndex) * sysConfig.GetActRepeatCount(m_dtu, m_nActIndex)) + ((sysConfig.m_nLanguage == LANGUAGE_CHINESE) ? " 毫秒" : " ms");
	lbAllDisplayTime->Caption = UnicodeString(CalcAllPlayTime()) + ((sysConfig.m_nLanguage == LANGUAGE_CHINESE) ? " 毫秒" : " ms");
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::vtFocusChanged(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column)
{
	if (Node == m_Node)
		return;

	if (m_dtu != NULL)
		SaveAct();

#ifndef AUTO_SIZE_TEXT
	CDtu* dtuOldParent = NULL;
	if (m_dtu != NULL)
	{
		if (m_dtu->m_nNodeType == NODE_CARD)
			dtuOldParent = m_dtu->m_dtuParent;
		else
			dtuOldParent = m_dtu;
	}
#endif

	m_Node = Node;
	m_dtu = (CDtu*)(vt->GetNodeData(Node));

#ifndef AUTO_SIZE_TEXT
	CDtu* dtuNewParent;
	if (m_dtu->m_nNodeType == NODE_CARD)
		dtuNewParent = m_dtu->m_dtuParent;
	else
		dtuNewParent = m_dtu;

	if (dtuNewParent != dtuOldParent)
#endif
		InitActInputWindow();
}
//---------------------------------------------------------------------------

bool TfrmMain::IsCurrentActActive()
{
	wchar_t* ws = sysConfig.m_sActActiveText.c_str();

	while (ws && *ws)
	{
		if (*ws >= L'0' && *ws < (L'0' + 20 + 56))
			return true;
		ws++;
	}

	return false;
}
//---------------------------------------------------------------------------

void TfrmMain::LoadDynamicActText(CDtu* dtu)
{
	m_sDynamicActText = sysConfig.GetActText(dtu, 0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuAutoCheckActChangeClick(TObject *Sender)
{
	mnuAutoCheckActChange->Checked = !mnuAutoCheckActChange->Checked;
	timerCheckDynamicAct->Enabled = mnuAutoCheckActChange->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerCheckDynamicActTimer(TObject *Sender)
{
	if (!vt->Enabled)
		return;

	PVirtualNode Node = vt->GetFirst();
	CDtu* dtuRoot = (CDtu*)(vt->GetNodeData(Node));

	UnicodeString sNewActText = sysConfig.GetActText(dtuRoot, 0);

	if (sNewActText == m_sDynamicActText)
		return;
	m_sDynamicActText = sNewActText;

	if (m_nActIndex != 0)
		SaveAct();

	vt->FocusedNode = Node;
	m_Node = Node;
	m_dtu = dtuRoot;
	comboEditAct->ItemIndex = 0;
	m_nActIndex = comboEditAct->ItemIndex;
	sysConfig.LoadAct(m_dtu, m_nActIndex);
	UpdateActVol();
	wndTextEdit->Refresh();
	CreateTxData1();

	Node = vt->GetFirst();
	while (Node != NULL)
	{
		CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
			Node->CheckState = csCheckedNormal;
		Node = vt->GetNext(Node);
	}

	TListItem* itemAct;
	for (int nActIndex = 0; nActIndex < listActs->Items->Count; nActIndex++)
	{
		itemAct = listActs->Items->Item[nActIndex];
		if (nActIndex == 0)
			itemAct->Checked = true;
		else
			itemAct->Checked = false;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "发送节目内容" : "Send Acts"))
		return;

	StartOperation();
	m_bReTx = false;
	timerTxAct->Enabled = true;
}
//---------------------------------------------------------------------------

int TfrmMain::GetTextWidth(TCanvas* Canvas, bool bDotFont, int nDotFontSize, UnicodeString ws)
{
	if (bDotFont)
	{
		if (nDotFontSize == 16)
			return TDotFont16::GetTextWidth(ws, ws.Length());
		else if (nDotFontSize == 24)
			return TDotFont24::GetTextWidth(ws, ws.Length());
		else
			return TDotFont32::GetTextWidth(ws, ws.Length());
	}
	else
	{
		return Canvas->TextWidth(ws);
	}
}
//---------------------------------------------------------------------------

void TfrmMain::DrawChar(TCanvas* Canvas, int x, int y, wchar_t wc)
{
	if (sysConfig. m_bDotFont)
	{
		if (sysConfig.m_nDotFontSize == 16)
			TDotFont16::DrawChar(Canvas, x, y, clWhite, clBlack, wc);
		else if (sysConfig.m_nDotFontSize == 24)
			TDotFont24::DrawChar(Canvas, x, y, clWhite, clBlack, wc);
		else
			TDotFont32::DrawChar(Canvas, x, y, clWhite, clBlack, wc);
	}
	else
		Canvas->TextOut(x, y, UnicodeString(wc));
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::timerAutoWriteClockTimer(TObject *Sender)
{
	SYSTEMTIME tm;
	GetLocalTime(&tm);

	if (tm.wMinute != m_nLastAutoWriteClockMinute)
	{
		m_nLastAutoWriteClockMinute = tm.wMinute;
		if (m_nLastAutoWriteClockMinute == sysConfig.m_nAutoClockMinute)
		{
			m_nAutoWriteClockHourCount++;
			if (m_nAutoWriteClockHourCount >= sysConfig.m_nAutoClockInterval)
			{
				m_nAutoWriteClockHourCount = 0;

				PVirtualNode Node;
				Node = vt->GetFirst();
				while (Node != NULL)
				{
					Node->CheckState = csCheckedNormal;
					Node = vt->GetNext(Node);
				}
				mnuAdjustCardClockClick(this);
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::DrawAreaImage()
{
	int nAreaIndex = comboArea->ItemIndex;

	imageAreas->Picture->Bitmap->Width = sysConfig.m_nScreenWidth;
	imageAreas->Width = sysConfig.m_nScreenWidth;
	imageAreas->Picture->Bitmap->Height = sysConfig.m_nScreenHeight;
	imageAreas->Height = sysConfig.m_nScreenHeight;
	if (imageAreas->Width < scrAreas->Width)
		imageAreas->Left = (scrAreas->Width - imageAreas->Width) / 2;
	else
		imageAreas->Left = 0;
	if (imageAreas->Height < scrAreas->Height)
		imageAreas->Top = (scrAreas->Height - imageAreas->Height) / 2;
	else
		imageAreas->Top = 0;
	imageAreas->Canvas->Brush->Color = clBlack;
	imageAreas->Canvas->FillRect(Rect(0, 0, scrAreas->Width, scrAreas->Height));

	imageAreas->Canvas->Pen->Style = psSolid;
	imageAreas->Canvas->Pen->Width = 1;
	imageAreas->Canvas->Pen->Color = clLime;
	imageAreas->Canvas->Font = lbAreaOfAct->Font;
	imageAreas->Canvas->Font->Color = clWhite;

	UnicodeString s = UnicodeString(nAreaIndex + 1);
	int nLeft = sysConfig.m_nSubAreaLeft[nAreaIndex];
	int nTop = sysConfig.m_nSubAreaTop[nAreaIndex];
	int nWidth = sysConfig.m_nSubAreaWidth[nAreaIndex];
	int nHeight = sysConfig.m_nSubAreaHeight[nAreaIndex];
	int nTextWidth = imageAreas->Picture->Bitmap->Canvas->TextWidth(s);
	int nTextHeight = imageAreas->Picture->Bitmap->Canvas->TextHeight(s);
	imageAreas->Canvas->Rectangle(TRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight));
	imageAreas->Canvas->TextOut(nLeft + (nWidth - nTextWidth) / 2 , nTop + (nHeight - nTextHeight) /2, s);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuSaveClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	if (!systemSaveDialog->Execute())
		return;

	if (FileExists(systemSaveDialog->FileName))
		DeleteFile(systemSaveDialog->FileName);

	TOKEN_PRIVILEGES tp;
	HANDLE hToken;
	LUID luid;
	LONG rc = ERROR_SUCCESS;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken ))
		rc = GetLastError();

	if (rc == ERROR_SUCCESS)
	{
		if (!LookupPrivilegeValue(NULL, SE_BACKUP_NAME, &luid))
			rc = GetLastError();
	}

	if (rc == ERROR_SUCCESS)
	{
		tp.PrivilegeCount           = 1;
		tp.Privileges[0].Luid       = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),	NULL, NULL );
		rc = GetLastError();
	}

	if (rc != ERROR_SUCCESS)
	{
		if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"系统配置和所有节目内容导出失败!", NULL, MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Export Failed!", NULL, MB_OK | MB_ICONWARNING);
		return;
	}

	HKEY hKey;
	wchar_t wsKey[] = L"Software\\Yuepoch\\LedPanelManV3";
	RegOpenKey(HKEY_CURRENT_USER, wsKey, &hKey);
	int nResult = RegSaveKeyW(hKey, systemSaveDialog->FileName.c_str(), NULL);
	RegCloseKey(hKey);
	AdjustTokenPrivileges(hToken, TRUE, NULL, 0, NULL, NULL);
	CloseHandle(hToken);

	if (nResult)
	{
		if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"系统配置和所有节目内容导出失败!", NULL, MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Export Failed!!", NULL, MB_OK | MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuOpenClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		if (Application->MessageBox(L"导入系统配置和节目内容将导致现有的系统配置和节目内容被替换。您确定继续吗？", L"导入配置和节目内容", MB_ICONQUESTION | MB_YESNO) != IDYES)
			return;
	}
	else
	{
		if (Application->MessageBox(L"Import new system configuration and act contents will replace current configuration and act contents. Are you sure to continue?", L"Import", MB_ICONQUESTION | MB_YESNO) != IDYES)
			return;
	}

	if (!systemOpenDialog->Execute())
		return;

	TOKEN_PRIVILEGES tp;
	HANDLE hToken;
	LUID luid;
	LONG rc = ERROR_SUCCESS;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken ))
		rc = GetLastError();

	if (rc == ERROR_SUCCESS)
	{
		if (!LookupPrivilegeValue(NULL, SE_RESTORE_NAME, &luid))
			rc = GetLastError();
	}

	if (rc == ERROR_SUCCESS)
	{
		tp.PrivilegeCount           = 1;
		tp.Privileges[0].Luid       = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),	NULL, NULL );
		rc = GetLastError();
	}

	if (rc != ERROR_SUCCESS)
	{
		Application->MessageBox((sysConfig.m_nLanguage == LANGUAGE_CHINESE) ? L"系统配置和节目内容导入失败!" : L"Import failed!", NULL, MB_OK | MB_ICONWARNING);
		return;
	}

	HKEY hKey = NULL;
	wchar_t wsKey[] = L"Software\\Yuepoch\\LedPanelManV3";
	RegCreateKey(HKEY_CURRENT_USER, wsKey, &hKey);
	int nResult = RegRestoreKeyW(hKey, systemOpenDialog->FileName.c_str(), NULL);
	RegCloseKey(hKey);
	if (nResult)
	{
		if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"系统配置和节目内容导入失败！如果您正在使用注册表编辑工具，请关闭后重试。", NULL, MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Import failed!", NULL, MB_OK | MB_ICONWARNING);
		return;
	}

	m_bPreview = false;
	m_bDirty = false;
	m_nActIndex = 0;

	m_dtu = NULL;
	m_Node = NULL;

	vt->Clear();
	listActs->Clear();

	vt->NodeDataSize = sizeof(CDtu);
	sysConfig.LoadMainConfig("Software\\Yuepoch\\LedPanelManV3\\" + sysConfig.m_sRootName);
	LoadAllCard();
	InitActInputWindow();

	PVirtualNode Node = vt->GetFirstSelected();
	CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
	LoadDynamicActText(dtu);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuSelectAllSuccessedClick(TObject *Sender)
{
	CDtu* dtu;
	PVirtualNode Node = vt->GetFirst();

	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if ((dtu->m_nNodeType == NODE_CARD) && (Node->CheckState == csCheckedNormal))
		{
			if ((dtu->m_sStatus == "-") || (wcsstr(dtu->m_sStatus.c_str(), L"失败") != NULL) || (wcsstr(dtu->m_sStatus.c_str(), L"Failed") != NULL))
				Node->CheckState = csUncheckedNormal;
		}
		vt->InvalidateNode(Node);
		Node = vt->GetNext(Node);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuSelectAllFailedClick(TObject *Sender)
{
	CDtu* dtu;
	PVirtualNode Node = vt->GetFirst();

	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if ((dtu->m_nNodeType == NODE_CARD) && (Node->CheckState == csCheckedNormal))
		{
			if (!((dtu->m_sStatus == "-") || (wcsstr(dtu->m_sStatus.c_str(), L"失败") != NULL) || (wcsstr(dtu->m_sStatus.c_str(), L"Failed") != NULL)))
				Node->CheckState = csUncheckedNormal;
		}
		vt->InvalidateNode(Node);
		Node = vt->GetNext(Node);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuAutoWriteClockSetupClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif
	TfrmAdjustClockSetup* frmAdjustClockSetup = new TfrmAdjustClockSetup(this);
	frmAdjustClockSetup->editAutoClockInerval->Text = sysConfig.m_nAutoClockInterval;
	frmAdjustClockSetup->editAutoClockMinute->Text = sysConfig.m_nAutoClockMinute;

	if (frmAdjustClockSetup->ShowModal() == mrOk)
	{
		sysConfig.m_nAutoClockInterval = frmAdjustClockSetup->editAutoClockInerval->Text.ToInt();
		if (sysConfig.m_nAutoClockInterval < 0)
			sysConfig.m_nAutoClockInterval = 1;

		sysConfig.m_nAutoClockMinute = frmAdjustClockSetup->editAutoClockMinute->Text.ToInt();
		if ((sysConfig.m_nAutoClockMinute < 0) || (sysConfig.m_nAutoClockMinute > 59))
			sysConfig.m_nAutoClockMinute = 57;

		sysConfig.SaveConfig();
		timerAutoWriteClock->Enabled = (sysConfig.m_nAutoClockInterval != 0);
	}

	delete frmAdjustClockSetup;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	SaveAct();
	DeleteFile("c:\\map.htm");
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuImportV3Click(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	if (Application->MessageBox(L"导入GS7020BMan V3.X系统配置和节目内容将导致现有的系统配置和节目内容被替换。您确定继续吗？", L"导入V3.X配置和节目内容", MB_ICONQUESTION | MB_YESNO) != IDYES)
		return;

	UnicodeString sPath;
	if (!SelectDirectory("请选择GS7020BMan V3.X软件所在的文件夹", "", sPath))
		return;

	TCustomIniFile* ini = new TRegistryIniFile("Software\\Yuepoch");
	ini->EraseSection("LedPanelManV3");
	delete ini;

	vt->Clear();
	PVirtualNode NodeRoot = vt->AddChild(NULL);
	CDtu* dtu = (CDtu*)(vt->GetNodeData(NodeRoot));
	dtu->m_sName = sysConfig.m_sRootName;
	dtu->m_sSection = "Software\\Yuepoch\\LedPanelManV3";
	dtu->m_nNodeType = NODE_ALL_CARD_ROOT;
	dtu->m_dtuParent = NULL;
	NodeRoot->CheckType = ctTriStateCheckBox;
	NodeRoot->CheckState = csUncheckedNormal;

	TIniFile* sysini = new TIniFile(sPath + "\\config.ini");
	sysConfig.m_sComPort = sysini->ReadString("SYS", "SerialPort", "COM1");
	sysConfig.m_nCommMethod = sysini->ReadInteger("SYS", "CommMethod", 0);
	sysConfig.m_sAPN = sysini->ReadString("NET", "APN", "cmnet");
#ifdef FIXED_IP
	sysConfig.m_sServer = FIXED_IP;
	sysConfig.m_nSocketPort = 3700;
#else
	sysConfig.m_sServer = sysini->ReadString("NET", "HostName", DEAFULT_DATA_CENTER);
	sysConfig.m_nSocketPort = sysini->ReadInteger("NET", "HostPort", 3700);
#endif
	sysConfig.m_sUserName = sysini->ReadString("NET", "HostUserName", "test");
	sysConfig.m_sPassword = sysini->ReadString("NET", "HostUserPwd", "123456");
	sysConfig.m_nTimeout_Network = sysini->ReadInteger("NET", "NetTimeOut", 4) * 1000;
	sysConfig.m_nRetry_Network = sysini->ReadInteger("NET", "Retries", 5);
	//m_nAutoRetryTime = sysini->ReadInteger("NET", "AutoTxActTime", 30);
	sysConfig.m_nScreenWidth = sysini->ReadInteger("LEDBOARD", "LedBoardWidth", 128); // 屏幕宽度（像素）
	sysConfig.m_nScreenHeight = sysini->ReadInteger("LEDBOARD", "LedBoardHeight", 16); // 屏幕单行高度（像素）
	sysConfig.m_bDoubleColor = sysini->ReadBool("LEDBOARD", "DoubleColor", false);
	sysConfig.m_bSupportWarning = sysini->ReadBool("LEDBOARD", "SupportWarnning", false);
	sysConfig.m_nMaxActCount = sysini->ReadInteger("LEDBOARD", "MaxActCount", 127); // 屏幕单行高度（像素）
	if ((sysConfig.m_nMaxActCount != 63) && (sysConfig.m_nMaxActCount != 31))
		sysConfig.m_nMaxActCount = 127;
	sysConfig.m_ulScanMode = sysini->ReadInteger("LEDBOARD", "ScanMode", SCAN_MODE_16_1_A);
	sysConfig.m_bNegData = sysini->ReadBool("LEDBOARD", "NegData", true);
	sysConfig.m_bNegOE = sysini->ReadBool("LEDBOARD", "NegOE", true);
#ifdef FIXED_IP
	sysConfig.m_sDataCenter = FIXED_IP;
	sysConfig.m_nSocketPort_Card = 3700;
#else
	sysConfig.m_sDataCenter = sysini->ReadString("LEDBOARD", "Server", DEAFULT_DATA_CENTER);
	sysConfig.m_nSocketPort_Card = sysini->ReadInteger("LEDBOARD", "ServerSocketPort", 3700);
#endif
	sysConfig.m_bActSecond = sysini->ReadBool("LEDBOARD", "ActSecond", false);
	sysConfig.m_bActSupportRepeat = sysini->ReadBool("LEDBOARD", "ActRepeat", false);
	sysConfig.m_bSupportInsertPlay = sysini->ReadBool("LEDBOARD", "InsertPlay", false);
	char szHourName[] = "LightStartHour0";
	char szMinuteName[] = "LightStartMin0";
	char szValueName[] = "LightValue0";
	for (int i = 0; i < MAX_LIGHT_CONTROL_COUNT; i++)
	{
		szHourName[strlen(szHourName) - 1] = i + '0';
		szMinuteName[strlen(szMinuteName) - 1] = i + '0';
		szValueName[strlen(szValueName) - 1] = i + '0';
		if (i == 0)
		{
			sysConfig.m_nLightControlStartHour[i] = sysini->ReadInteger("LEDBOARD", szHourName, 0);
			sysConfig.m_nLightControlStartMinute[i] = sysini->ReadInteger("LEDBOARD", szMinuteName, 0);
		}
		else
		{
			sysConfig.m_nLightControlStartHour[i] = sysini->ReadInteger("LEDBOARD", szHourName, 23);
			sysConfig.m_nLightControlStartMinute[i] = sysini->ReadInteger("LEDBOARD", szMinuteName, 59);
		}
		sysConfig.m_nLightControlValue[i] = sysini->ReadInteger("LEDBOARD", szValueName, 7);
	}

	char szAreaLeftName[] = "AreaLeft0";
	char szAreaTopName[] = "AreaTop0";
	char szAreaWidthName[] = "AreaWidth0";
	char szAreaHeightName[] = "AreaHeight0";
	for (int i= 0; i < MAX_AREA_COUNT; i++)
	{
		szAreaLeftName[strlen(szAreaLeftName) - 1] = i + '0';
		szAreaTopName[strlen(szAreaTopName) - 1] = i + '0';
		szAreaWidthName[strlen(szAreaWidthName) - 1] = i + '0';
		szAreaHeightName[strlen(szAreaHeightName) - 1] = i + '0';
		sysConfig.m_nSubAreaLeft[i] = sysini->ReadInteger("LEDBOARD", szAreaLeftName, 0);
		sysConfig.m_nSubAreaTop[i] = sysini->ReadInteger("LEDBOARD", szAreaTopName, 0);
		sysConfig.m_nSubAreaWidth[i] = sysini->ReadInteger("LEDBOARD", szAreaWidthName, 128);
		sysConfig.m_nSubAreaHeight[i] = sysini->ReadInteger("LEDBOARD", szAreaHeightName, 16);
	}

	sysConfig.m_nSubAreaCount = sysini->ReadInteger("LEDBOARD", "SubAreaCount", 1);
	if (sysConfig.m_nSubAreaCount < 1)
		sysConfig.m_nSubAreaCount = 1;
	if (sysConfig.m_nSubAreaCount > MAX_AREA_COUNT)
		sysConfig.m_nSubAreaCount = MAX_AREA_COUNT;

	for (int nIndex = 0; nIndex < sysConfig.m_nMaxActCount; nIndex++)
	{
		sysConfig.LoadActVol_V3(sysini, nIndex);
		sysConfig.SaveAct(m_dtu, nIndex);
	}

	delete sysini;

	sysConfig.SaveConfig();

	try
	{
		vt1->LoadFromFile(sPath + "\\config\\current\\CardList.gs7");
	}
	catch(...)
	{
	}

	PVirtualNode Node = vt1->GetFirst();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt1->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_ALL_CARD_ROOT)
		{
			dtu->m_sSection = "Software\\Yuepoch\\LedPanelManV3";
			dtu->m_dtuParent = NULL;
		}
		else
		{
			CDtu* dtuParent = (CDtu*)(vt1->GetNodeData(Node->Parent));
			dtu->m_sSection = dtuParent->m_sSection + "\\" + dtuParent->m_sName + "\\LedPanels";
			dtu->m_dtuParent = dtuParent;
			WriteGroupList(Node->Parent);
		}
		Node = vt1->GetNext(Node);
	}

	Node = vt1->GetFirst();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt1->GetNodeData(Node));
		dtu->SaveConfig();
		Node = vt1->GetNext(Node);
	}

	vt1->Clear();
	m_bPreview = false;
	m_bDirty = false;
	m_nActIndex = 0;

	m_dtu = NULL;
	m_Node = NULL;

	vt->Clear();
	listActs->Clear();


	vt->NodeDataSize = sizeof(CDtu);
	LoadAllCard();
	InitActInputWindow();

	Node = vt->GetFirstSelected();
	if (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		LoadDynamicActText(dtu);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::vt1GetNodeDataSize(TBaseVirtualTree *Sender,
		  int &NodeDataSize)
{
	NodeDataSize = sizeof(CDtu);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::vt1InitNode(TBaseVirtualTree *Sender, PVirtualNode ParentNode,
		  PVirtualNode Node, TVirtualNodeInitStates &InitialStates)
{
	CDtu* pNodeData = (CDtu*)(vt->GetNodeData(Node));
	if ((pNodeData->m_nNodeType == NODE_GROUP) || (pNodeData->m_nNodeType == NODE_ALL_CARD_ROOT))
		InitialStates = InitialStates<<ivsHasChildren;
	else
		InitialStates = InitialStates>>ivsHasChildren;
	if ((Node->ChildCount == 0) || (pNodeData->m_nNodeType == NODE_CARD))
		InitialStates = InitialStates<<ivsExpanded;
}
//---------------------------------------------------------------------------

void TfrmMain::CheckUniqueDtuName(CDtu* dtuCheck, PVirtualNode NodeCheck)
{
	int nIndex = 1;
	UnicodeString sName = dtuCheck->m_sName;

	for (;;)
	{
		PVirtualNode Node = vt1->GetFirst();
		while (Node != NULL)
		{
			CDtu* dtu = (CDtu*)(vt1->GetNodeData(Node));
			if (dtu->m_nNodeType != NODE_ALL_CARD_ROOT && NodeCheck != Node)
			{
				if (sName == dtu->m_sName)
				{
					sName = dtuCheck->m_sName + UnicodeString(nIndex++);
					continue;
				}
			}
			Node = vt1->GetNext(Node);
		}
		break;
	}

	dtuCheck->m_sName = sName;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::vt1LoadNode(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TStream *Stream)
{
	WORD nLen;
	char s[1024];

	CDtu* dtu = (CDtu*)(vt1->GetNodeData(Node));
	dtu->Create();
	Stream->Read(&dtu->m_nNodeType, sizeof(dtu->m_nNodeType));

	Stream->Read(&nLen, sizeof(nLen));
	Stream->Read(s, nLen);
	dtu->m_sName = s;
	dtu->m_sName = TrimLeft(dtu->m_sName);
	dtu->m_sName = TrimRight(dtu->m_sName);
	CheckUniqueDtuName(dtu, Node);

	if (dtu->m_nNodeType == NODE_ALL_CARD_ROOT)
		dtu->m_sName = sysConfig.m_sRootName;
	else if (dtu->m_nNodeType == NODE_CARD)
	{
		Stream->Read(&nLen, sizeof(nLen));
		Stream->Read(s, nLen);
		dtu->m_sCode = s;
		dtu->m_sCode = TrimLeft(dtu->m_sCode);
		dtu->m_sCode = TrimRight(dtu->m_sCode);
	}
}
//---------------------------------------------------------------------------

void TfrmMain::WriteGroupList(PVirtualNode pParentNode)
{
	CDtu* dtu = (CDtu*)(vt1->GetNodeData(pParentNode));
	TCustomIniFile* ini = new TRegistryIniFile(dtu->m_sSection + "\\" + dtu->m_sName);
	int nIndex = 0;

	ini->EraseSection("List");
	PVirtualNode Node = vt1->GetFirstChild(pParentNode);
	unsigned int nLevel = vt1->GetNodeLevel(Node);
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt1->GetNodeData(Node));
		if (vt1->GetNodeLevel(Node) == nLevel)
			ini->WriteString("List", UnicodeString(nIndex++), dtu->m_sName);
		if (Node == vt1->GetLastChild(pParentNode))
			break;
		Node = vt1->GetNext(Node, false);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnBlueClick(TObject *Sender)
{
	wndTextEdit->ChangeTextColor(COLOR_BLUE);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnWhiteClick(TObject *Sender)
{
	wndTextEdit->ChangeTextColor(COLOR_WHITE);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnAquaClick(TObject *Sender)
{
	wndTextEdit->ChangeTextColor(COLOR_AQUA);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnBlue_BackClick(TObject *Sender)
{
	wndTextEdit->ChangeBackColor(BACKGROUND_COLOR_BLUE);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnPink_BackClick(TObject *Sender)
{
	wndTextEdit->ChangeBackColor(BACKGROUND_COLOR_PINK);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnAqua_BackClick(TObject *Sender)
{
	wndTextEdit->ChangeBackColor(BACKGROUND_COLOR_AQUA);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnPinkClick(TObject *Sender)
{
	wndTextEdit->ChangeTextColor(COLOR_PINK);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnYellow_BackClick(TObject *Sender)
{
	wndTextEdit->ChangeBackColor(BACKGROUND_COLOR_YELLOW);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuReTxSetupClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif
	TfrmReTxSetup* frmReTxSetup = new TfrmReTxSetup(this);
	frmReTxSetup->editAutoRetryTime->Text = sysConfig.m_nAutoReTxActInterval;
	frmReTxSetup->checkOnlyLogLastTx->Checked = sysConfig.m_bAutoReTx_OnlyLast;

	if (frmReTxSetup->ShowModal() == mrOk)
	{
		sysConfig.m_nAutoReTxActInterval = frmReTxSetup->editAutoRetryTime->Text.ToInt();
		if (sysConfig.m_nAutoReTxActInterval < 0)
			sysConfig.m_nAutoReTxActInterval = 1;
		sysConfig.m_bAutoReTx_OnlyLast = frmReTxSetup->checkOnlyLogLastTx->Checked;

		sysConfig.SaveConfig();
		m_nWaitReTxTimerCount = 0;
		timer_AutoWaitReTxAct->Enabled = (sysConfig.m_nAutoClockInterval != 0);
	}

	mnuReTxSetup->Checked = (sysConfig.m_nAutoReTxActInterval != 0);
	delete frmReTxSetup;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuCommConfigClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	TfrmCommConfig *frmCommConfig = new TfrmCommConfig(this);
	frmCommConfig->ShowModal();
	delete frmCommConfig;
}
//---------------------------------------------------------------------------

bool TfrmMain::VerifyAct(CDtu* dtu)
{
	BYTE buf[1024];

	int nLeftDataLen = sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) - dtu->m_nDataSent;
	int nDataLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize : nLeftDataLen;
	int nStartPos = (dtu->m_nTxAct) * sysConfig.GetOneActVolMaxLen() + dtu->m_nDataSent;
	sysConfig.ReadActData(dtu, dtu->m_nTxAct, dtu->m_nDataSent, nDataLen, buf);

	int nHeadLen = (sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) ? sizeof(MMI_CMD_HEAD_HEB) : sizeof(MMI_CMD_HEAD);

	DWORD* pStartPos = (DWORD*)(dtu->m_ucRxBuf + nHeadLen);
	WORD* pusDataLen = (WORD*)(pStartPos + 1);
	BYTE* pucData = (BYTE*)(pusDataLen + 1);

	if (*pStartPos != (DWORD)nStartPos)
		return false;

	if (*pusDataLen != nDataLen)
		return false;

	if ((dtu->m_nDataSent == 0) && (sysConfig.GetActDataSize(dtu, dtu->m_nTxAct) > sysConfig.m_nMaxPackSize))
		buf[1] = 0xFF;

	if (memcmp(pucData, buf, nDataLen) != 0)
		return false;

	return true;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::CalcAllPlayTime()
{
	DWORD dwPlayTime = 0;

	for (int nActIndex = 0; nActIndex < sysConfig.m_nMaxActCount; nActIndex++)
	{
		if (nActIndex == sysConfig.GetActRefIndex(m_dtu, nActIndex))
			dwPlayTime += CalcPlayTime(nActIndex) * sysConfig.GetActRepeatCount(m_dtu, nActIndex);
	}

	return dwPlayTime;
}

void __fastcall TfrmMain::mnuParkDigitClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif
	int nSelected = GetCheckedCardCount();

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"发送停车位数据", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Send Park Values", MB_OK | MB_ICONWARNING);
		return;
	}

	TfrmParkDigit *frmParkDigit = new TfrmParkDigit(this);
	frmParkDigit->editPark1->Text = m_nParkValue[0];
	frmParkDigit->editPark2->Text = m_nParkValue[1];
	frmParkDigit->editPark3->Text = m_nParkValue[2];
	frmParkDigit->editPark4->Text = m_nParkValue[3];
	frmParkDigit->editPark5->Text = m_nParkValue[4];
	frmParkDigit->editPark6->Text = m_nParkValue[5];
	frmParkDigit->editPark7->Text = m_nParkValue[6];
	frmParkDigit->editPark8->Text = m_nParkValue[7];
	frmParkDigit->editPark1->Font->Color = m_clParkColor[0];
	frmParkDigit->editPark2->Font->Color = m_clParkColor[1];
	frmParkDigit->editPark3->Font->Color = m_clParkColor[2];
	frmParkDigit->editPark4->Font->Color = m_clParkColor[3];
	frmParkDigit->editPark5->Font->Color = m_clParkColor[4];
	frmParkDigit->editPark6->Font->Color = m_clParkColor[5];
	frmParkDigit->editPark7->Font->Color = m_clParkColor[6];
	frmParkDigit->editPark8->Font->Color = m_clParkColor[7];
	frmParkDigit->checkDoubleColor->Checked = m_bParkDigit_DoubleColor;
	frmParkDigit->comboBoxLightValue->ItemIndex = m_nParkDigit_Lightvalue;

	frmParkDigit->ShowModal();
	if (frmParkDigit->ModalResult != mrOk)
	{
		delete frmParkDigit;
		return;
	}

	m_nParkValue[0] = StrToInt(frmParkDigit->editPark1->Text);
	m_nParkValue[1] = StrToInt(frmParkDigit->editPark2->Text);
	m_nParkValue[2] = StrToInt(frmParkDigit->editPark3->Text);
	m_nParkValue[3] = StrToInt(frmParkDigit->editPark4->Text);
	m_nParkValue[4] = StrToInt(frmParkDigit->editPark5->Text);
	m_nParkValue[5] = StrToInt(frmParkDigit->editPark6->Text);
	m_nParkValue[6] = StrToInt(frmParkDigit->editPark7->Text);
	m_nParkValue[7] = StrToInt(frmParkDigit->editPark8->Text);
	m_clParkColor[0] = frmParkDigit->editPark1->Font->Color;
	m_clParkColor[1] = frmParkDigit->editPark2->Font->Color;
	m_clParkColor[2] = frmParkDigit->editPark3->Font->Color;
	m_clParkColor[3] = frmParkDigit->editPark4->Font->Color;
	m_clParkColor[4] = frmParkDigit->editPark5->Font->Color;
	m_clParkColor[5] = frmParkDigit->editPark6->Font->Color;
	m_clParkColor[6] = frmParkDigit->editPark7->Font->Color;
	m_clParkColor[7] = frmParkDigit->editPark8->Font->Color;
	m_bParkDigit_DoubleColor = frmParkDigit->checkDoubleColor->Checked;
	m_nParkDigit_Lightvalue = frmParkDigit->comboBoxLightValue->ItemIndex;
	m_bParkDigit_Init = frmParkDigit->checkInit->Checked;

	delete frmParkDigit;

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "发送停车位数据" : "Send Park Values"))
		return;

	::Sleep(20);

	StartOperation();
	timerParkDigit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerParkDigitTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_WriteParkValue(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(m_bParkDigit_DoubleColor ? MMI_CMD_PARK_DIGIT_EXT : MMI_CMD_PARK_DIGIT, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
					m_ComPort.Close();
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_WriteParkValue(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_WriteParkValue(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(m_bParkDigit_DoubleColor ? MMI_CMD_PARK_DIGIT_EXT : MMI_CMD_PARK_DIGIT, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
							{
								dtu->m_nRetries++;
								SendCommand_WriteParkValue(dtu);
								dtu->m_dwTxStartTime = ::GetTickCount();
								nSendDtuCount++;
							}
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timerParkDigit->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WriteParkValue(CDtu* dtu)
{
	BYTE buf[2048];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	BYTE* pDigit = (BYTE*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();

	if (m_bParkDigit_DoubleColor)
	{
		pHead->ucCmd = MMI_CMD_PARK_DIGIT_EXT;
		pHead->usLen = 25;
		*pDigit = m_nParkDigit_Lightvalue;
		if (m_bParkDigit_Init)
			*pDigit |= 0x80;
		pDigit++;
		for (int i = 0; i < 8; i++)
		{
			if (m_clParkColor[i] == clRed)
				*pDigit++ = 1;
			else if (m_clParkColor[i] == clLime)
				*pDigit++ = 2;
			else if (m_clParkColor[i] == clYellow)
				*pDigit++ = 3;
			else if (m_clParkColor[i] == clBlue)
				*pDigit++ = 4;
			else if (m_clParkColor[i] == clFuchsia)
				*pDigit++ = 5;
			else if (m_clParkColor[i] == clAqua)
				*pDigit++ = 6;
			else
				*pDigit++ = 7;
			if ((m_nParkValue[i] < 0) || (m_nParkValue[i] > 9999))
				m_nParkValue[i] = 0;
			*pDigit++ = m_nParkValue[i] % 10 + (((m_nParkValue[i] % 100) / 10) << 4);
			*pDigit++ = (m_nParkValue[i] / 100) % 10 + ((m_nParkValue[i] / 1000) << 4);
		}
	}
	else
	{
		pHead->ucCmd = MMI_CMD_PARK_DIGIT;
		pHead->usLen = 16;
		for (int i = 0; i < 8; i++)
		{
			if ((m_nParkValue[i] < 0) || (m_nParkValue[i] > 9999))
				m_nParkValue[i] = 0;
			*pDigit++ = m_nParkValue[i] % 10 + (((m_nParkValue[i] % 100) / 10) << 4);
			*pDigit++ = (m_nParkValue[i] / 100) % 10 + ((m_nParkValue[i] / 1000) << 4);
		}
	}

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuCardTypeClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	TfrmCardType* frmCardType = new TfrmCardType(this, sysConfig.m_nCardType);
	int mr = frmCardType->ShowModal();
	int nCardType = frmCardType->m_nCardType;
	delete frmCardType;

	if (mr == mrOk)
	{
		sysConfig.m_nCardType = nCardType;
		UpdateCaption();
		sysConfig.SaveConfig();
		sysConfig.InitCardTypeAndRootName();
		InitActInputWindow();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::UpdateCaption()
{
#ifdef AUTO_SIZE_TEXT
	char szCaption[] =
	{
	0x59 ^ 0x55, 0x75 ^ 0x55, 0x65 ^ 0x55, 0x70 ^ 0x55,
	0x6F ^ 0x55, 0x63 ^ 0x55, 0x68 ^ 0x55, 0x20 ^ 0x55,
	0xCF ^ 0x55, 0xD4 ^ 0x55, 0xCA ^ 0x55, 0xBE ^ 0x55,
	0xC6 ^ 0x55, 0xC1 ^ 0x55, 0xB9 ^ 0x55, 0xDC ^ 0x55,
	0xC0 ^ 0x55, 0xED ^ 0x55, 0x20 ^ 0x55, 0x59 ^ 0x55,
	0x4C ^ 0x55, 0x65 ^ 0x55, 0x64 ^ 0x55, 0x4D ^ 0x55,
	0x61 ^ 0x55, 0x6E ^ 0x55, 0x20 ^ 0x55, 0x56 ^ 0x55,
	0x33 ^ 0x55, 0x2E ^ 0x55, 0x39 ^ 0x55, 0x35 ^ 0x55,
	0x45 ^ 0x55, 0x20 ^ 0x55, 0x2D ^ 0x55, 0x20 ^ 0x55,
	0
	};
#else
#ifdef SUN_BO_NO_YledMan
	char szCaption[] =
	{
	0x59 ^ 0x55, 0x75 ^ 0x55, 0x65 ^ 0x55, 0x70 ^ 0x55,
	0x6F ^ 0x55, 0x63 ^ 0x55, 0x68 ^ 0x55, 0x20 ^ 0x55,
	0xCF ^ 0x55, 0xD4 ^ 0x55, 0xCA ^ 0x55, 0xBE ^ 0x55,
	0xC6 ^ 0x55, 0xC1 ^ 0x55, 0xB9 ^ 0x55, 0xDC ^ 0x55,
	0xC0 ^ 0x55, 0xED ^ 0x55, 0x20 ^ 0x55, /*0x59 ^ 0x55,
	0x4C ^ 0x55, 0x65 ^ 0x55, 0x64 ^ 0x55, 0x4D ^ 0x55,
	0x61 ^ 0x55, 0x6E ^ 0x55, 0x20 ^ 0x55, */0x56 ^ 0x55,
	0x33 ^ 0x55, 0x2E ^ 0x55, 0x39 ^ 0x55, 0x35 ^ 0x55,
	0x20 ^ 0x55, 0x2D ^ 0x55, 0x20 ^ 0x55, 0
	};
#else
	char szCaption[] =
	{
	0x59 ^ 0x55, 0x75 ^ 0x55, 0x65 ^ 0x55, 0x70 ^ 0x55,
	0x6F ^ 0x55, 0x63 ^ 0x55, 0x68 ^ 0x55, 0x20 ^ 0x55,
	0xCF ^ 0x55, 0xD4 ^ 0x55, 0xCA ^ 0x55, 0xBE ^ 0x55,
	0xC6 ^ 0x55, 0xC1 ^ 0x55, 0xB9 ^ 0x55, 0xDC ^ 0x55,
	0xC0 ^ 0x55, 0xED ^ 0x55, 0x20 ^ 0x55, 0x59 ^ 0x55,
	0x4C ^ 0x55, 0x65 ^ 0x55, 0x64 ^ 0x55, 0x4D ^ 0x55,
	0x61 ^ 0x55, 0x6E ^ 0x55, 0x20 ^ 0x55, 0x56 ^ 0x55,
	0x33 ^ 0x55, 0x2E ^ 0x55, 0x39 ^ 0x55, 0x35 ^ 0x55,
	0x20 ^ 0x55, 0x2D ^ 0x55, 0x20 ^ 0x55, 0
	};
#endif
#endif
	SYSTEMTIME st;
	::GetLocalTime(&st);

	UnicodeString sCaption;
	if (m_nLanguage == LANGUAGE_CHINESE)
	{
		char *s = szCaption;
		while (*s)
			*s++ ^= 0x55;

		if (st.wYear < 2015)
			sCaption = UnicodeString(szCaption + 8);
		else
			sCaption = UnicodeString(szCaption);
	}
	else
#ifdef AUTO_SIZE_TEXT
		sCaption = "Yuepoch LED Sign Manager V3.951E - ";
#else
		sCaption = "Yuepoch LED Sign Manager V3.953 - ";
#endif

	switch (sysConfig.m_nCardType)
	{
		case CARD_TYPE_GS7020B:
			sCaption += "GS7020B/GS7020E/CS7020B";
			break;
		case CARD_TYPE_GS7030B:
			sCaption += "GS7030B/CS7030B";
			break;
		case CARD_TYPE_GS2012A:
			sCaption += "GS2012A/CS2012A";
			break;
		case CARD_TYPE_YL2010A:
			sCaption += "YL2010A";
			break;
		case CARD_TYPE_YL2010A_GPS_JTX:
			sCaption += "YL2010A_GPS_JTX";
			break;
		case CARD_TYPE_YL2010C:
			sCaption += "YL2010C";
			break;
		case CARD_TYPE_YL2010AF:
			sCaption += "YL2010AF";
			break;
		case CARD_TYPE_GS7020BF:
			sCaption += "GS7020BF/GS7020EF";
			break;
		case CARD_TYPE_YL2010AR:
			sCaption += "YL2010AR";
			break;
		case CARD_TYPE_PD102B:
			sCaption += "PD102B";
			break;
		case CARD_TYPE_GS7020BF_HEB:
			sCaption += "GS7020EF-H";
			break;
		case CARD_TYPE_GS9026:
			sCaption += "GS9026";
			break;
		case CARD_TYPE_GS7020BF_W:
			sCaption += "GS7020BF-W/GS7020EF-W";
			break;
		case CARD_TYPE_GS7020BX:
			sCaption += "GS7020BX/GS7020EX";
			break;
		case CARD_TYPE_GS7020BT:
			sCaption += "GS7020BT/GS7020ET/CS7020BT";
			break;
		case CARD_TYPE_GS7020F:
			sCaption += "GS7020F";
			break;
		case CARD_TYPE_GS7030BV2:
			sCaption += "GS7030BV2";
			break;
		case CARD_TYPE_GS7022B:
			sCaption += "GS7022H/CS7022H/GS7022B/GS7022E/CS7022E";
			break;
		case CARD_TYPE_GS7020BQ:
			sCaption += "GS7020HQ/GS7020BQ/GS7020EQ/CS7020HQ/CS7020EQ";
			break;
		case CARD_TYPE_GS7020BP:
			sCaption += "G(C)S7020H(B/E)P";
			break;
		case CARD_TYPE_GS2012A_S:
			sCaption += "GS2012A-S";
			break;
		case CARD_TYPE_GS7024E:
			sCaption += "GS7024B/GS7024E/CS7024B";
			break;
		case CARD_TYPE_WF2012B:
			sCaption += "WF2012B";
			break;
		case CARD_TYPE_GS2012AS_64:
			sCaption += "GS2012AS-64 GS7020BS-64";
			break;
		case CARD_TYPE_GS9020S:
			sCaption += "GS9020S";
			break;
		case CARD_TYPE_GS7020HS:
			sCaption += "GS7020HS/GS7020IS";
			break;
		case CARD_TYPE_GS2012B:
			sCaption += "GS2012B/CS2012B";
			break;
		case CARD_TYPE_GS9020:
			sCaption += "GS9020";
			break;
		case CARD_TYPE_GS7020H:
			sCaption += "GS7020H CS7020H";
			break;
		case CARD_TYPE_GS9025:
			sCaption += "GS9025";
			break;
		case CARD_TYPE_GS9023:
			sCaption += "GS9023";
			break;
		case CARD_TYPE_GS9020F_GPS:
			sCaption += "GS9020F_GPS";
			break;
		case CARD_TYPE_GS7020HFA:
			sCaption += "GS7020HFA/CS7020HFA";
			break;
		case CARD_TYPE_SN2P:
			sCaption += "SN2P";
			break;
		default:
			sCaption += "GS7020B/GS7020E/CS7020B";
			break;
	}

	Caption = sCaption;
}
//---------------------------------------------------------------------------

void TfrmMain::Encrypt(CDtu* dtu, int nLen, BYTE* p)
{
	while (nLen)
	{
		*p ^= (dtu->m_ucKey1 ^ 0x23) ; p++; nLen--;
		if (nLen)
		{
			*p ^= (dtu->m_ucKey2 ^ 0xFB); p++; nLen--;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuFlashTestClick(TObject *Sender)
{
	TfrmFlashTest* frmFlashTest = new TfrmFlashTest(this);
	if (frmFlashTest->ShowModal() != mrOk)
	{
		delete frmFlashTest;
		return;
	}

	m_ucTestData[0] = frmFlashTest->Edit1->Text.ToInt();
	m_ucTestData[1] = frmFlashTest->Edit2->Text.ToInt();
	m_ucTestData[2] = frmFlashTest->Edit3->Text.ToInt();
	delete frmFlashTest;

	int nSelected = GetCheckedCardCount();

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"FLASH测试", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Flash Test", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "FLASH测试" : "Flash Test"))
		return;

	::Sleep(20);

	StartOperation();
	timerFlashTest->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerFlashTestTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				SendCommand_WriteFlashTestData(dtu);
				dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
				dtu->m_dwTxStartTime = ::GetTickCount();
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_WRITE_FLASH, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_WriteFlashTestData(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_WriteFlashTestData(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_WRITE_FLASH, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
							{
								dtu->m_nRetries++;
								SendCommand_WriteFlashTestData(dtu);
								dtu->m_dwTxStartTime = ::GetTickCount();
								nSendDtuCount++;
							}
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timerFlashTest->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WriteFlashTestData(CDtu* dtu)
{
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		SendCommand_WriteFlashTestData_HEB(dtu);
	else
		SendCommand_WriteFlashTestData_Normal(dtu);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WriteFlashTestData_Normal(CDtu* dtu)
{
	BYTE buf[2048];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	DWORD* pStartPos = (DWORD*)(pHead + 1);
	BYTE* p = (BYTE*)(pStartPos + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->m_nCardId;
	pHead->ucCmd =  MMI_CMD_WRITE_FLASH;
	pHead->usLen = 7;
	*pStartPos = 2048 * 1024 - 4096;
	*p++ = m_ucTestData[0];
	*p++ = m_ucTestData[1];
	*p = m_ucTestData[2];

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);

	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WriteFlashTestData_HEB(CDtu* dtu)
{
	BYTE buf[2048];
	MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;
	DWORD* pStartPos = (DWORD*)(pHead + 1);
	BYTE* p = (BYTE*)(pStartPos + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->m_nCardId;
	pHead->ucCmd =  MMI_CMD_WRITE_FLASH;
	pHead->usLen = 7;
	*pStartPos = 2048 * 1024 - 4096;
	*p++ = m_ucTestData[0];
	*p++ = m_ucTestData[1];
	*p = m_ucTestData[2];

	pHead->ucKey1 = dtu->m_ucKey1 = GetTickCount();
	pHead->ucKey2 = dtu->m_ucKey2 = GetTickCount() >> 8;
	buf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, buf);
	Encrypt(dtu, pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);

	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnTxWeatherWarnningClick(TObject *Sender)
{
/*	TfrmWeather_16x16_C1* frmWeather = new TfrmWeather_16x16_C1(this);

	SaveAct();

	if (frmWeather->ShowModal() == mrOk)
	{
		UnicodeString sActDesc;

		comboEditAct->ItemIndex = frmWeather->m_nObjectIndex + 100;
		comboEditActChange(this);
		m_nActIndex = frmWeather->m_nObjectIndex + 100;
		UpdateActVol();

		switch (m_nActIndex)
		{
		case 100:
			sysConfig.m_sActDesc = "红色预警信号: " + frmWeather->m_sText;
			break;
		case 101:
			sysConfig.m_sActDesc = "橙色预警信号: " + frmWeather->m_sText;
			break;
		case 102:
			sysConfig.m_sActDesc = "黄色预警信号: " + frmWeather->m_sText;
			break;
		case 103:
			sysConfig.m_sActDesc = "蓝色预警信号: " + frmWeather->m_sText;
			break;
		}
		sysConfig.m_nActArea = frmWeather->m_nObjectIndex + 1;
		sysConfig.m_sActText = "";
		sysConfig.m_sActActiveText = "";
		sysConfig.m_nActEnterStyle = ENTER_STYLE_DIRECT;
		sysConfig.m_nActEnterSpeed = 16;
		sysConfig.m_nActExitStyle = EXIT_STYLE_NULL;
		sysConfig.m_nActExitSpeed = 16;
		sysConfig.m_nActShowTime = 3000;
		if (sysConfig.m_picAct != NULL)
		{
			delete sysConfig.m_picAct;
			sysConfig.m_picAct = NULL;
		}

		sysConfig.m_nActImagePosX = 0;
		sysConfig.m_nActImagePosY = 0;

		sysConfig.m_picAct = new TPicture;
		sysConfig.m_picAct->Assign(frmWeather->m_Image->Picture);
		UpdateActVol();
		comboAreaChange(this);
		listActs->Items->Item[m_nActIndex]->Checked = true;
	}

	delete frmWeather;
*/

	TfrmWeather64x64* frmWeather = new TfrmWeather64x64(this);
	for (int i = 0; i < WEATHER_ICON_COUNT; i++)
		frmWeather->m_bSelected[i] = m_bSelected[i];

	if (frmWeather->ShowModal() == mrOk)
	{
		for (int i = 0; i < WEATHER_ICON_COUNT; i++)
			m_bSelected[i] = frmWeather->m_bSelected[i];
		delete frmWeather;
	}
	else
	{
		delete frmWeather;
		return;
	}

	//////////////
	int nSelected = GetCheckedCardCount();
	if (nSelected == 0)
	{
		//if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"发布气象预警信号", MB_OK | MB_ICONWARNING);
		//else
		//	Application->MessageBox(L"Please select at least one LED panel.", L"Adjust Card's Clock", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "发布气象预警信号" : "Weather Tag"))
		return;

	::Sleep(20);

	StartOperation();

	timerWeather->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuRootPathClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	SaveAct();

	TfrmRootName* frmRootName = new TfrmRootName(this);
	frmRootName->ShowModal();
	int nResult = frmRootName->m_nResult;
	int bDeleted = frmRootName->m_bDeleted;
	delete frmRootName;

	switch (nResult)
	{
	case 0:	//修改
		vt->Refresh();
		break;
	case 1:	//新建
		sysConfig.SaveConfig();
		vt->Clear();
		LoadAllCard();
		InitActInputWindow();
		break;
	case 2:	//跳转
		sysConfig.LoadMainConfig(UnicodeString(REG_ROOT_NAME) + "\\" + sysConfig.m_sRootName);
		sysConfig.InitCardTypeAndRootName();
		vt->Clear();
		LoadAllCard();
		InitActInputWindow();
		UpdateCaption();
		break;
	}

	if (bDeleted)
	{
		LoadAllRootNodeName();
		LoadAllCardName();
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuCacelWarnningClick(TObject *Sender)
{
	int nSelected = GetCheckedCardCount();

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"取消显示屏报警信息", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Stop Warnning Display", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "取消显示屏报警信息" : "Stop Warnning Display"))
		return;

	::Sleep(20);

	StartOperation();
	timer_CancelWarnning->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timer_CancelWarnningTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				SendCommand_CancelWarnning(dtu);
				dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
				dtu->m_dwTxStartTime = ::GetTickCount();
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_CANCEL_WARNNING, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_CancelWarnning(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_CancelWarnning(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_CANCEL_WARNNING, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
							{
								dtu->m_nRetries++;
								SendCommand_CancelWarnning(dtu);
								dtu->m_dwTxStartTime = ::GetTickCount();
								nSendDtuCount++;
							}
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timer_CancelWarnning->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_CancelWarnning(CDtu* dtu)
{
	BYTE buf[64];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_CMD_CANCEL_WARNNING;
	pHead->usLen = 0;

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

void TfrmMain::CreateAutoTimeHintImage()
{
	int nImageWidth, nImageHeight;

	imgActive->Picture->Bitmap->PixelFormat = pf4bit;
	imgActive->Picture->Bitmap->Canvas->Brush->Color = clBlack;
	imgActive->Picture->Bitmap->Canvas->Font->Name = sysConfig.m_sActFontName;
	imgActive->Picture->Bitmap->Canvas->Font->Size = sysConfig.m_nActFontSize;
	imgActive->Picture->Bitmap->Canvas->Font->Style = TFontStyles(sysConfig.m_nActFontStyle);
	imgActive->Picture->Bitmap->Canvas->Font->Color = clRed;
	nImageHeight = sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea];
	nImageWidth = imgActive->Picture->Bitmap->Canvas->TextWidth("提醒您: 现在是年月日时分0123456789");
	imgActive->Picture->Bitmap->SetSize(nImageWidth, nImageHeight);
	imgActive->Picture->Bitmap->Canvas->FillRect(TRect(0, 0, nImageWidth, nImageHeight));
	imgActive->Picture->Bitmap->Canvas->TextOut(0, 0, "提醒您: 现在是年月日时分0123456789");
}
//---------------------------------------------------------------------------

BYTE* TfrmMain::CreateAutoHintFlashImage(BYTE* p)
{
	int x, y, i;
	BYTE c;
	IMAGE_OBJECT_HEAD* pFlashImageHead;
	int nByteHeight = (sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea] + 7) >> 3;

	pFlashImageHead = (IMAGE_OBJECT_HEAD*)p;
	pFlashImageHead->ucObjectType = OBJ_TYPE_SINGLE_COLOR_IMAGE;
	pFlashImageHead->ucActType = ACT_TYPE_DIRECT_SHOW;
	pFlashImageHead->usRepeatCount = 1;
	pFlashImageHead->ulPara = 0;   //显示时间us
	pFlashImageHead->usDisplayPosX = 0;	//显示区域坐上角X坐标
	pFlashImageHead->usDisplayPosY = 0;	//显示区域坐上角Y坐标
	pFlashImageHead->usDisplayWidth = imgActive->Picture->Bitmap->Width; //显示区域宽度
	pFlashImageHead->usDisplayHeight = sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea];	//显示区域高度
	pFlashImageHead->usImageWidth = imgActive->Picture->Bitmap->Width;		//图像宽度
	pFlashImageHead->usImageHeight = sysConfig.m_nSubAreaHeight[sysConfig.m_nActArea];		//图像高度
	p = (BYTE*)(pFlashImageHead + 1);

	for (x = 0; x < imgActive->Picture->Bitmap->Width; x++)
	{
		c = 0;
		i = 0;
		for (y = 0; y < nByteHeight * 8; y++)
		{
			c <<= 1;
			if (y < imgActive->Picture->Bitmap->Height)
			{
				if (imgActive->Picture->Bitmap->Canvas->Pixels[x][y] & 0xE0E0E0 & RGB(255, 255, 255))
					c |= 1;
			}

			i++;
			if (i >= 8)
			{
				*p++ = c;
				i = 0;
				c =0;
			}
		}
	}

	return p;
}
//---------------------------------------------------------------------------

DWORD TfrmMain::GetDisplayTime_AutoTimeHint()
{
	imgActive->Picture->Bitmap->Canvas->Font->Name = sysConfig.m_sActFontName;
	imgActive->Picture->Bitmap->Canvas->Font->Size = sysConfig.m_nActFontSize;
	imgActive->Picture->Bitmap->Canvas->Font->Style = TFontStyles(sysConfig.m_nActFontStyle);
	imgActive->Picture->Bitmap->Canvas->Font->Color = clRed;
	int nImageWidth = imgActive->Picture->Bitmap->Canvas->TextWidth("提醒您: 现在是2011年12月23日32时34分");
	return sysConfig.m_nActEnterSpeed * (nImageWidth + sysConfig.m_nSubAreaWidth[sysConfig.m_nActArea]);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::udpWarnningUDPRead(TIdUDPListenerThread *AThread, TBytes AData,
		  TIdSocketHandle *ABinding)
{
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		udpWarnningUDPRead_HEB(AData);
	else
		udpWarnningUDPRead_Normal(AData);
}
//---------------------------------------------------------------------------

void TfrmMain::udpWarnningUDPRead_Normal(TBytes AData)
{
#pragma pack(1)
	struct
	{
		char  szGsmCode[12];
		BYTE  ucFrameType;
		MMI_CMD_HEAD stMmiHead;
		BYTE  ucCheckSum;
	}stFrame;
#pragma pack()
	BYTE* p;
	int i;
	CDtu* dtu;

	if (AData.Length != (int)sizeof(stFrame))
		return;

	p = (BYTE*)&stFrame;
	for (i = 0; i < (int)sizeof(stFrame); i++)
		*p++ = AData[i];

	if (stFrame.ucFrameType != FT_WARNNING)
		return;

	if (stFrame.stMmiHead.ucCmd != MMI_CMD_WARNNING)
		return;

	size_t nStrLen = 0;
	for (i = 0; i < 12; i++)
	{
		if (stFrame.szGsmCode[i])
			nStrLen++;
		else
			break;
	}
	if (nStrLen > sizeof(stFrame.szGsmCode) - 1)
		return;

	PVirtualNode Node = vt->GetFirst();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
		{
			if (::strcmp(dtu->m_sCode.c_str(), stFrame.szGsmCode) == 0)
			{
				SendCommand_AckWarnning(dtu);
				TfrmWarnning *frmWarnning = new TfrmWarnning(NULL);
				frmWarnning->lbName->Caption = dtu->m_sName;
				char s[128];
				SYSTEMTIME st;
				::GetLocalTime(&st);
				sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				frmWarnning->lbTime->Caption = s;
				frmWarnning->ShowModal();
				delete frmWarnning;
				return;
			}
		}

		Node = vt->GetNext(Node);
	}

}
//---------------------------------------------------------------------------

void TfrmMain::udpWarnningUDPRead_HEB(TBytes AData)
{
#pragma pack(1)
	struct
	{
		char szGsmCode[12];
		BYTE ucFrameType;
		MMI_CMD_HEAD_HEB stMmiHead;
		DWORD ulCheckResult;
		BYTE ucCheckSum;
	}stFrame;
#pragma pack()
	BYTE* p;
	int i;
	int nLen = AData.Length;


	if (nLen != (int)sizeof(stFrame) && nLen != (int)sizeof(stFrame) - 3)
		return;

	p = (BYTE*)&stFrame;
	if (nLen == (int)sizeof(stFrame))
	{
		for (i = 0; i < (int)sizeof(stFrame); i++)
			*p++ = AData[i];
	}
	else
	{
		stFrame.ulCheckResult = 0;
		for (i = 0; i < (int)sizeof(stFrame) - 4; i++)
			*p++ = AData[i];
		stFrame.ucCheckSum = AData[i];
	}

	if (stFrame.ucFrameType != FT_WARNNING)
		return;

	if (stFrame.stMmiHead.ucCmd != MMI_CMD_WARNNING)
		return;

	size_t nStrLen = 0;
	for (i = 0; i < 12; i++)
	{
		if (stFrame.szGsmCode[i])
			nStrLen++;
		else
			break;
	}
	if (nStrLen > sizeof(stFrame.szGsmCode) - 1)
		return;

	for (LIST_PTR::iterator it = m_lstDtu.begin(); it != m_lstDtu.end(); it++)
	{
		CDtu* dtu = (CDtu*)(*it);
		if (::strcmp(dtu->m_sCode.c_str(), stFrame.szGsmCode) == 0)
		{
			SendCommand_AckWarnning_HEB(dtu);
			if (stFrame.ulCheckResult == dtu->m_ulLastCheckResult)
			{
				if ((::GetTickCount() - dtu->m_dwlastCheckResultReportTime) < 60 * 60 * 1000)
				{
					dtu->m_dwlastCheckResultReportTime = ::GetTickCount();
					return;
				}
			}
			dtu->m_ulLastCheckResult = stFrame.ulCheckResult;
			dtu->m_dwlastCheckResultReportTime = ::GetTickCount();
			frmCheckResult->AddItem(dtu->m_sName, stFrame.ulCheckResult);
			if (frmCheckResult->Visible == false)
				frmCheckResult->ShowModal();
			return;
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_AckWarnning(CDtu* dtu)
{
	BYTE buf[64];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_CMD_WARNNING;
	pHead->usLen = 0;

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	bool bOldActive = udpSocket->Active;
	udpSocket->Active = true;
	SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	udpSocket->Active = bOldActive;
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_AckWarnning_HEB(CDtu* dtu)
{
	BYTE buf[64];
	MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_CMD_WARNNING;
	pHead->usLen = 0;
	pHead->ucKey1 = dtu->m_ucKey1 = GetTickCount();
	pHead->ucKey2 = dtu->m_ucKey2 = GetTickCount() >> 8;
	buf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, buf);
	Encrypt(dtu, pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);

	bool bOldActive = udpSocket->Active;
	udpSocket->Active = true;
	SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
	udpSocket->Active = bOldActive;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuDownloadDotFontClick(TObject *Sender)
{
	int nSelected = GetCheckedCardCount();
	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请先选择要下载字库的显示屏控制卡(左侧复选框打勾)", L"下载字库", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Download Font", MB_OK | MB_ICONWARNING);
		return;
	}

	if (sysConfig.m_nCommMethod != 0)
	{
		Application->MessageBox(L"请选择串口通信", L"下载字库", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "下载字库" : "Download Font"))
		return;

	if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
	{
		m_pucGS9025DotFont = new BYTE[0x169400];
#ifdef _DEBUG
		int nPos = 0;
		ReadFromFile("GS9025_ASC16_S.fon", 0, m_pucGS9025DotFont + nPos, 96 * 16); nPos += 96 * 16;
		ReadFromFile("GS9025_HZK16_S.fon", 0, m_pucGS9025DotFont + nPos, 7050 * 32); nPos += 7050 * 32;
		ReadFromFile("GS9025_ASC16_H.fon", 0, m_pucGS9025DotFont + nPos, 96 * 16); nPos += 96 * 16;
		ReadFromFile("GS9025_HZK16_H.fon", 0, m_pucGS9025DotFont + nPos, 7050 * 32); nPos += 7050 * 32;
		//ReadFromFile("GS9025_ASC24_S.fon", 0, m_pucGS9025DotFont + nPos, 96 * 48); nPos += 96 * 48;
		//ReadFromFile("GS9025_HZK24_S.fon", 0, m_pucGS9025DotFont + nPos, 7050 * 72); nPos += 7050 * 72;
		//12*24宋体 ASCII
		memcpy(m_pucGS9025DotFont + nPos, TDotFont24::ucAsciiFont + 0x20 * 48, 96 * 48); nPos += 96 * 48;
		//24*24宋体 汉字
		memcpy(m_pucGS9025DotFont + nPos, TDotFont24::ucHanziFont, 3 * 94 * 72);
		memcpy(m_pucGS9025DotFont + nPos + 3 * 94 * 72, TDotFont24::ucHanziFont + 9 * 94 * 72, 72 * 94 * 72); nPos += 7050 * 72;
		ReadFromFile("GS9025_ASC24_H.fon", 0, m_pucGS9025DotFont + nPos, 96 * 48); nPos += 96 * 48;
		ReadFromFile("GS9025_HZK24_H.fon", 0, m_pucGS9025DotFont + nPos, 7050 * 72);
#else
		Create9025DotFont(m_pucGS9025DotFont);
#endif
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9023)
	{
		m_pucGS9025DotFont = new BYTE[0x6F000];
#ifdef _DEBUG
		int nPos = 0;
		ReadFromFile("GS9025_ASC16_S.fon", 0, m_pucGS9025DotFont + nPos, 96 * 16); nPos += 96 * 16;
		ReadFromFile("GS9025_HZK16_S.fon", 0, m_pucGS9025DotFont + nPos, 7050 * 32); nPos += 7050 * 32;
		ReadFromFile("GS9025_ASC16_H.fon", 0, m_pucGS9025DotFont + nPos, 96 * 16); nPos += 96 * 16;
		ReadFromFile("GS9025_HZK16_H.fon", 0, m_pucGS9025DotFont + nPos, 7050 * 32);
#else
		Create9023DotFont(m_pucGS9025DotFont);
#endif
	}
	else
		MakeUnicodeTable();
	StartOperation();
	timerDownloadDotFont->Enabled = true;
}
//---------------------------------------------------------------------------

void TfrmMain::MakeUnicodeTable()
{
	WORD usHanZi;
	WORD usUnicode;
	WORD usHanZiIndex = 0x8000;
	BYTE i, j;
	m_pucUnicodeTable = new WORD[65536];

	memset(m_pucUnicodeTable, 0, 65536 * 2);

	for (i = 0xA1; i <= 0xA9; i++)
	{
		for (j = 0xA1; j <= 0xFE; j++)
		{
			usHanZi = i + (j << 8);
			int n = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)&usHanZi, 2, (LPWSTR)&usUnicode, 1);
			if (n == 1)
				m_pucUnicodeTable[usUnicode] = usHanZiIndex;
			usHanZiIndex++;
		}
	}

	for (i = 0xB0; i <= 0xF7; i++)
	{
		for (j = 0xA1; j <= 0xFE; j++)
		{
			usHanZi = i + (j << 8);
			int n = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCSTR)&usHanZi, 2, (LPWSTR)&usUnicode, 1);
			if (n == 1)
				m_pucUnicodeTable[usUnicode] = usHanZiIndex;
			usHanZiIndex++;
		}
	}

	for (int n = 0x20; n < 0x80; n++)
		m_pucUnicodeTable[n] = n;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerDownloadDotFontTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		switch (dtu->m_nTxStatus)
		{
		case TX_STATUS_READY:
			if (!m_ComPort.GetOpen())
				m_ComPort.Open();
			SendCommand_WriteDotFont(dtu);
			dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
			dtu->m_dwTxStartTime = ::GetTickCount();
			break;
		case TX_STATUS_WAIT_ECHO:
			if (dtu->m_nTxStep == 0)
			{
				if (WaitForCmdEcho(MMI_CMD_STOP_DISPLAY, dtu, 0))
				{
					dtu->m_nRetries = 0;
					if (m_nLanguage == LANGUAGE_CHINESE)
						dtu->m_sStatus= "已暂停显示,准备发送数据";
					else
						dtu->m_sStatus= "Act Playing Stopped, Preparing Sending Data";
					vt->InvalidateNode(Node);
					dtu->m_nTxStatus = TX_STATUS_READY;
					dtu->m_nTxStep = 1;
					m_ComPort.Close();
				}
			}
			else if (dtu->m_nTxStep == 1)
			{
				if (WaitForCmdEcho(MMI_CMD_WRITE_FLASH, dtu, 4))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_READY;
					int nLenAll;
					if (sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64)
					{
						if (sysConfig.m_nSmsActFontSize == 64)
							nLenAll = 128 * 1024 + 96 * 256 + 7708 * 512;
						else if (sysConfig.m_nSmsActFontSize == 48)
							nLenAll = 128 * 1024 + 96 * 256 + 7708 * 288;
						else //if (sysConfig.m_nSmsActFontSize == 32)
							nLenAll = 128 * 1024 + 96 * 256 + 7708 * 128;
					}
					else if (sysConfig.m_nCardType == CARD_TYPE_GS9020S)
					{
						if (sysConfig.m_nSmsActFontSize == 16)
							nLenAll = 128 * 1024 + 96 * 16 + 7708 * 32;
						else //if (sysConfig.m_nSmsActFontSize == 32)
							nLenAll = 128 * 1024 + 96 * 64 + 7708 * 128;
					}
					else if (sysConfig.m_nCardType == CARD_TYPE_GS7020HS)
					{
						if (sysConfig.m_nSmsActFontSize == 16)
							nLenAll = 128 * 1024 + 96 * 16 + 7708 * 32;
						else //if (sysConfig.m_nSmsActFontSize == 32)
							nLenAll = 128 * 1024 + 96 * 64 + 7708 * 128;
					}
					else if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
						nLenAll = 0x169400;
					else if (sysConfig.m_nCardType == CARD_TYPE_GS9023)
						nLenAll = 0x6F000;
					else
						nLenAll = (128 + 244 + 972) * 1024;
					if (dtu->m_nDataSent + sysConfig.m_nMaxPackSize >= nLenAll)
					{
						if (m_nLanguage == LANGUAGE_CHINESE)
							dtu->m_sStatus = "准备重新开始节目显示";
						else
							dtu->m_sStatus = "Restarting Playing Acts";
						vt->InvalidateNode(Node);
						dtu->m_nTxStep = 2;
					}
					else
					{
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nDataSent += sysConfig.m_nMaxPackSize;
						//int nPercent = dtu->m_nDataSent * 100 / nLenAll;
						//dtu->m_sStatus = UnicodeString(nPercent) + "%";
						dtu->m_sStatus = UnicodeString(dtu->m_nDataSent);
						vt->InvalidateNode(Node);
						dtu->m_nTxStep = 1;
					}
					//m_ComPort.Close();
				}
			}
			else
			{
				if (WaitForCmdEcho(MMI_CMD_RESTART_DISPLAY, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
					m_ComPort.Close();
				}
			}

			if (dtu->m_nTxStatus == TX_STATUS_WAIT_ECHO)
			{
				dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
				if (dwTime > (DWORD)((sysConfig.m_nCommMethod == 0) ? sysConfig.m_nTimeout_SerialPort : sysConfig.m_nTimeout_Network * 1000))
				{
					if (dtu->m_nRetries >= ((sysConfig.m_nCommMethod == 0) ? sysConfig.m_nRetry_SerialPort : sysConfig.m_nRetry_Network))
					{
						dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
						if (m_nLanguage == LANGUAGE_CHINESE)
							dtu->m_sStatus = "失败";
						else
							dtu->m_sStatus = "Failed";
						vt->InvalidateNode(Node);
						m_ComPort.Close();
					}
					else
					{
						dtu->m_nRetries++;
						SendCommand_WriteDotFont(dtu);
						dtu->m_dwTxStartTime = ::GetTickCount();
					}
				}
			}
			break;
		case TX_STATUS_END_OK:
			nEnd++;
			break;
		case TX_STATUS_END_TIME_OUT:
			nEnd++;
			break;
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timerDownloadDotFont->Enabled = false;

		if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
			delete m_pucGS9025DotFont;
		else if (sysConfig.m_nCardType == CARD_TYPE_GS9023)
			delete m_pucGS9025DotFont;
		else
			delete m_pucUnicodeTable;

		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::ReadFromFile(UnicodeString sFileName, int nPos, BYTE* buf, int nLen)
{
	int hFile = FileOpen(ExtractFilePath(Application->ExeName) + sFileName, fmOpenRead);
	FileSeek(hFile, nPos, 0);
	FileRead(hFile, buf, nLen);
	FileClose(hFile);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WriteDotFont(CDtu* dtu)
{
	BYTE buf[2048];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	DWORD* pStartPos = (DWORD*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	if (dtu->m_nTxStep == 0)
	{
		if (sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64 || sysConfig.m_nCardType == CARD_TYPE_GS9020S || sysConfig.m_nCardType == CARD_TYPE_GS7020HS || sysConfig.m_nCardType == CARD_TYPE_GS9025 || sysConfig.m_nCardType == CARD_TYPE_GS9023)
		{
			pHead->ucCmd = 0x3F;	//波特率调整到115200*4
			pHead->usLen = 0;
			buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
			if (sysConfig.m_nCommMethod == 0)
			{
				m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
				m_ComPort.Close();
				m_ComPort.SetBaudRate(115200*4);
				m_ComPort.Open();
				Sleep(500);
			}
			pHead->usHeadTag = MMI_START_ID;
			pHead->ucCardId = dtu->GetCardId();
		}
		pHead->ucCmd = MMI_CMD_STOP_DISPLAY;
		pHead->usLen = 0;
	}
	else if (dtu->m_nTxStep == 1)
	{
		if (sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64)
		{
			pHead->ucCmd = MMI_CMD_WRITE_FLASH;
			pHead->usLen = sysConfig.m_nMaxPackSize + 4;
			*pStartPos = dtu->m_nDataSent + 0x400000;
			if (dtu->m_nDataSent < 128 * 1024)	//Unicode Table
				memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], ((BYTE*)m_pucUnicodeTable) + dtu->m_nDataSent, sysConfig.m_nMaxPackSize);
			else
			{
				if (sysConfig.m_nSmsActFontSize == 64)
				{
					if (dtu->m_nDataSent < (128 * 1024 + 96 * 256))	//32*64 ASCII
					{
						int nPos = dtu->m_nDataSent - (128 * 1024);
						ReadFromFile("ASC64.fon", nPos, &buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], 1024);
					}
					else
					{
						int nPos = dtu->m_nDataSent - (128 * 1024) - 96 * 256;
						ReadFromFile("HZK64.fon", nPos, &buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], 1024);
					}
				}
				else if (sysConfig.m_nSmsActFontSize == 48)
				{
					if (dtu->m_nDataSent < (128 * 1024 + 96 * 256))	//24*48 ASCII
					{
						int nPos = dtu->m_nDataSent - (128 * 1024);
						ReadFromFile("ASC48.fon", nPos, &buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], 1024);
					}
					else
					{
						int nPos = dtu->m_nDataSent - (128 * 1024) - 96 * 256;
						ReadFromFile("HZK48.fon", nPos, &buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], 1024);
					}
				}
				else //if (sysConfig.m_nSmsActFontSize == 32)
				{
					if (dtu->m_nDataSent < (128 * 1024 + 96 * 256))	//16*32 ASCII
					{
						int nPos = dtu->m_nDataSent - (128 * 1024);
						if (nPos < 96 * 64)
						{
							if ((nPos + sysConfig.m_nMaxPackSize) < 96 * 64)
								memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont32::ucHanziFont + (82 * 94) * 128 + nPos, sysConfig.m_nMaxPackSize);
							else
							{
								memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont32::ucHanziFont + (82 * 94) * 128 + nPos, 96 * 64 - nPos);
								memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont32::ucHanziFont, sysConfig.m_nMaxPackSize - (96 * 64 - nPos));
							}
						}
						else
							memset(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], 0xFF, sysConfig.m_nMaxPackSize);
					}
					else
					{
						int nPos = dtu->m_nDataSent - (128 * 1024) - 96 * 256;
						memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont32::ucHanziFont + nPos, sysConfig.m_nMaxPackSize);
					}
				}
			}
		}
		else if (sysConfig.m_nCardType == CARD_TYPE_GS9020S || sysConfig.m_nCardType == CARD_TYPE_GS7020HS)
		{
			pHead->ucCmd = MMI_CMD_WRITE_FLASH;
			pHead->usLen = sysConfig.m_nMaxPackSize + 4;
			*pStartPos = dtu->m_nDataSent + 768 * 1024;
			if (dtu->m_nDataSent < 128 * 1024)	//Unicode Table
				memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], ((BYTE*)m_pucUnicodeTable) + dtu->m_nDataSent, sysConfig.m_nMaxPackSize);
			else
			{
				if (sysConfig.m_nSmsActFontSize == 16)
				{
					if (dtu->m_nDataSent < (128 * 1024 + 96 * 16))	//32*64 ASCII
					{
						int nPos = dtu->m_nDataSent - (128 * 1024);
						memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucAsciiFont + nPos, sysConfig.m_nMaxPackSize);
					}
					else
					{
						int nPos = dtu->m_nDataSent - (128 * 1024) - 96 * 16;
						if (nPos < 9 * 94 * 32)
						{
							if ((nPos + sysConfig.m_nMaxPackSize) < 9 * 94 * 32)
								memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucHanziFont + nPos, sysConfig.m_nMaxPackSize);
							else
							{
								memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucHanziFont + nPos, 9 * 94 * 32 - nPos);
								memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucHanziFont + 15 * 94 * 32, sysConfig.m_nMaxPackSize - (9 * 94 * 32 - nPos));
							}
						}
						else
						{
							nPos -= 9 * 94 * 32;
							nPos += 15 * 94 * 32;
							if ((nPos + sysConfig.m_nMaxPackSize) < TDotFont16::nHanziDotFontDataSize)
								memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucHanziFont + nPos, sysConfig.m_nMaxPackSize);
							else
								memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucHanziFont + nPos, TDotFont16::nHanziDotFontDataSize - nPos);
						}
					}
				}
				else //if (sysConfig.m_nSmsActFontSize == 32)
				{
					int nPos = dtu->m_nDataSent - (128 * 1024);
					if (nPos < 96 * 64)
					{
						if ((nPos + sysConfig.m_nMaxPackSize) < 96 * 64)
							memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont32::ucHanziFont + (82 * 94) * 128 + nPos, sysConfig.m_nMaxPackSize);
						else
						{
							memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont32::ucHanziFont + (82 * 94) * 128 + nPos, 96 * 64 - nPos);
							memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont32::ucHanziFont, sysConfig.m_nMaxPackSize - (96 * 64 - nPos));
						}
					}
					else
					{
						int nPos = dtu->m_nDataSent - (128 * 1024) - 96 * 64;
						memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont32::ucHanziFont + nPos, sysConfig.m_nMaxPackSize);
					}
				}
			}
		}
		else if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
		{
			pHead->ucCmd = MMI_CMD_WRITE_FLASH;
			pHead->usLen = sysConfig.m_nMaxPackSize + 4;
			*pStartPos = dtu->m_nDataSent + 512 * 1024;
			memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], m_pucGS9025DotFont + dtu->m_nDataSent, sysConfig.m_nMaxPackSize);
		}
		else if (sysConfig.m_nCardType == CARD_TYPE_GS9023)
		{
			pHead->ucCmd = MMI_CMD_WRITE_FLASH;
			pHead->usLen = sysConfig.m_nMaxPackSize + 4;
			*pStartPos = dtu->m_nDataSent + 1536 * 1024;
			memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], m_pucGS9025DotFont + dtu->m_nDataSent, sysConfig.m_nMaxPackSize);
		}
		else
		{
			pHead->ucCmd = MMI_CMD_WRITE_FLASH;
			pHead->usLen = sysConfig.m_nMaxPackSize + 4;
			if (sysConfig.m_nCardType == CARD_TYPE_GS2012A_S)
				*pStartPos = dtu->m_nDataSent + 0x400000;
			else
				*pStartPos = dtu->m_nDataSent + 0x80000;
			if (dtu->m_nDataSent < 128 * 1024)	//Unicode Table
				memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], ((BYTE*)m_pucUnicodeTable) + dtu->m_nDataSent, sysConfig.m_nMaxPackSize);
			else if (dtu->m_nDataSent < (128 + 244) * 1024)	//16*16
			{
				int nPos = dtu->m_nDataSent - (128 * 1024);
				if (nPos < 96 * 16)
				{
					if ((nPos + sysConfig.m_nMaxPackSize) < 96 * 16)
						memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucAsciiFont + nPos, sysConfig.m_nMaxPackSize);
					else
					{
						memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucAsciiFont + nPos, 96 * 16 - nPos);
						memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucHanziFont, sysConfig.m_nMaxPackSize - (96 * 16 - nPos));
					}
				}
				else
				{
					nPos -= 96 * 16;
					if (nPos < 9 * 94 * 32)
					{
						if ((nPos + sysConfig.m_nMaxPackSize) < 9 * 94 * 32)
							memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucHanziFont + nPos, sysConfig.m_nMaxPackSize);
						else
						{
							memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucHanziFont + nPos, 9 * 94 * 32 - nPos);
							memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucHanziFont + 15 * 94 * 32, sysConfig.m_nMaxPackSize - (9 * 94 * 32 - nPos));
						}
					}
					else
					{
						nPos -= 9 * 94 * 32;
						nPos += 15 * 94 * 32;
						if ((nPos + sysConfig.m_nMaxPackSize) < TDotFont16::nHanziDotFontDataSize)
							memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucHanziFont + nPos, sysConfig.m_nMaxPackSize);
						else
							memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont16::ucHanziFont + nPos, TDotFont16::nHanziDotFontDataSize - nPos);
					}
				}
			}
			else
			{
				int nPos = dtu->m_nDataSent - ((128 + 244)* 1024);
				if (nPos < 96 * 64)
				{
					if ((nPos + sysConfig.m_nMaxPackSize) < 96 * 64)
						memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont32::ucHanziFont + (82 * 94) * 128 + nPos, sysConfig.m_nMaxPackSize);
					else
					{
						memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont32::ucHanziFont + (82 * 94) * 128 + nPos, 96 * 64 - nPos);
						memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont32::ucHanziFont, sysConfig.m_nMaxPackSize - (96 * 64 - nPos));
					}
				}
				else
				{
					nPos -= 96 * 64;
					memcpy(&buf[sizeof(MMI_CMD_HEAD) + sizeof(DWORD)], TDotFont32::ucHanziFont + nPos, sysConfig.m_nMaxPackSize);
				}
			}
		}
	}
	else if (dtu->m_nTxStep == 2)
	{
		pHead->ucCmd = MMI_CMD_RESTART_DISPLAY;
		pHead->usLen = 0;
	}

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	//else
	//	SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuCheckResultClick(TObject *Sender)
{
	frmCheckResult->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuQueryCheckResultClick(TObject *Sender)
{
	int nSelected = GetCheckedCardCount();

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"查询显示屏工作状态", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Query Self Check Result of LED Sign", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "查询显示屏工作状态" : "Query Self Check Result of LED Sign"))
		return;

	::Sleep(20);

	StartOperation();
	timer_CheckResult->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timer_CheckResultTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_QueryCheckResult(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_QUERY_CHECK, dtu, (sysConfig.m_nCardType == CARD_TYPE_GS9026) ? 4 : 1))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					UpdateLedSignStatus(dtu);
					vt->InvalidateNode(Node);
					m_ComPort.Close();
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_QueryCheckResult(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_QueryCheckResult(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_QUERY_CHECK, dtu, (sysConfig.m_nCardType == CARD_TYPE_GS9026) ? 4 : 1))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					UpdateLedSignStatus(dtu);
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
							{
								dtu->m_nRetries++;
								SendCommand_QueryCheckResult(dtu);
								dtu->m_dwTxStartTime = ::GetTickCount();
								nSendDtuCount++;
							}
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timer_CheckResult->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_QueryCheckResult(CDtu* dtu)
{
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		SendCommand_QueryCheckResult_HEB(dtu);
	else
		SendCommand_QueryCheckResult_Normal(dtu);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_QueryCheckResult_Normal(CDtu* dtu)
{
	BYTE buf[64];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_CMD_QUERY_CHECK;
	pHead->usLen = 0;
	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);

	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_QueryCheckResult_HEB(CDtu* dtu)
{
	BYTE buf[64];
	MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_CMD_QUERY_CHECK;
	pHead->usLen = 0;
	pHead->ucKey1 = dtu->m_ucKey1 = GetTickCount();
	pHead->ucKey2 = dtu->m_ucKey2 = GetTickCount() >> 8;
	buf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, buf);
	Encrypt(dtu, pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);

	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
}
//---------------------------------------------------------------------------

void TfrmMain::UpdateLedSignStatus(CDtu* dtu)
{
	BYTE* p;
	UnicodeString s;
	DWORD ulData;
	WORD usLen;

	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
	{
		MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)dtu->m_ucRxBuf;
		usLen = pHead->usLen;
		p = dtu->m_ucRxBuf + sizeof(MMI_CMD_HEAD_HEB);
	}
	else
	{
		MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)dtu->m_ucRxBuf;
		usLen = pHead->usLen;
		p = dtu->m_ucRxBuf + sizeof(MMI_CMD_HEAD);
	}

	if (usLen == 1)
		ulData = *p;
	else if (usLen == 2)
		ulData = *((WORD*)p);
	else if (usLen == 4)
		ulData = *((DWORD*)p);
	else
		ulData = 0;

	if (ulData != 0)
	{
		s = "故障: ";
		if (ulData & CHECK_RESULT_OE_BIT_1)
			s += "OE1 ";
		if (ulData & CHECK_RESULT_CK_BIT_1)
			s += "CK1 ";
		if (ulData & CHECK_RESULT_ST_BIT_1)
			s += "ST1 ";
		if (ulData & CHECK_RESULT_A_BIT_1)
			s += "A1 ";
		if (ulData & CHECK_RESULT_B_BIT_1)
			s += "B1 ";
		if (ulData & CHECK_RESULT_C_BIT_1)
			s += "C1 ";
		if (ulData & CHECK_RESULT_D_BIT_1)
			s += "D1 ";
		if (ulData & CHECK_RESULT_R1_BIT_1)
			s += "R1 ";
		if (ulData & CHECK_RESULT_R2_BIT_1)
			s += "R2 ";
		if (ulData & CHECK_RESULT_G1_BIT_1)
			s += "G1 ";
		if (ulData & CHECK_RESULT_G2_BIT_1)
			s += "G2 ";

		if (ulData & CHECK_RESULT_OE_BIT_2)
			s += "OE2 ";
		if (ulData & CHECK_RESULT_CK_BIT_2)
			s += "CK2 ";
		if (ulData & CHECK_RESULT_ST_BIT_2)
			s += "ST2 ";
		if (ulData & CHECK_RESULT_A_BIT_2)
			s += "A2 ";
		if (ulData & CHECK_RESULT_B_BIT_2)
			s += "B2 ";
		if (ulData & CHECK_RESULT_C_BIT_2)
			s += "C2 ";
		if (ulData & CHECK_RESULT_D_BIT_2)
			s += "D2 ";
		if (ulData & CHECK_RESULT_R1_BIT_2)
			s += "R3 ";
		if (ulData & CHECK_RESULT_R2_BIT_2)
			s += "R4 ";
		if (ulData & CHECK_RESULT_G1_BIT_2)
			s += "G3 ";
		if (ulData & CHECK_RESULT_G2_BIT_2)
			s += "G4 ";
	}
	else
		s = "OK";

	dtu->m_sStatus = s;
}
//---------------------------------------------------------------------------

void TfrmMain::LoadAllRootNodeName()
{
	wchar_t szName[256];

	m_lstAllRootNodeName->Clear();

	TRegistry *reg = new TRegistry();
	reg->RootKey = HKEY_CURRENT_USER;
	if (reg->OpenKeyReadOnly(REG_ROOT_NAME))
	{
		DWORD dwIndex = 0;
		while (RegEnumKey(reg->CurrentKey, dwIndex++, szName, 256) == ERROR_SUCCESS)
			m_lstAllRootNodeName->Append(szName);
	}

	delete reg;
}
//---------------------------------------------------------------------------

void TfrmMain::LoadAllCardName()
{
	ClearDtuList();

	for (int nIndex = 0; nIndex < m_lstAllRootNodeName->Count ; nIndex++)
		LoadCardNameTree("Software\\Yuepoch\\LedPanelManV3\\" + m_lstAllRootNodeName->Strings[nIndex]);
}
//---------------------------------------------------------------------------

void TfrmMain::LoadCardNameTree(UnicodeString sSection)
{
	TCustomIniFile* ini = new TRegistryIniFile(sSection);
	int nIndex = 0;

	for (;;)
	{
		UnicodeString sName = ini->ReadString("List", UnicodeString(nIndex), "");
		if (sName == "")
			break;

		CDtu* dtu = new CDtu();
		dtu->m_sName = sName;
		dtu->m_sSection = sSection + "\\LedPanels";
		dtu->LoadConfig();

		if (dtu->m_nNodeType == NODE_GROUP)
		{
			LoadCardNameTree(dtu->m_sSection + "\\" + dtu->m_sName);
			delete dtu;
		}
		else
			m_lstDtu.insert(m_lstDtu.end(), dtu);

		nIndex++;
	}

	delete ini;
}
//---------------------------------------------------------------------------

void TfrmMain::ClearDtuList()
{
	for (LIST_PTR::iterator it = m_lstDtu.begin(); it != m_lstDtu.end(); it++)
	{
		CDtu* dtu = (CDtu*)(*it);
		delete dtu;
	}

	m_lstDtu.clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuShrinkClick(TObject *Sender)
{
	PVirtualNode Node = vt->GetFirst();
	while (Node != NULL)
	{
		CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_GROUP)
			vt->FullCollapse(Node);
		Node = vt->GetNext(Node);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuActDefineClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif

	TListItem *itemAct;
	int nSelected = 0;

	for (int i = 0; i < listActs->Items->Count; i++)
	{
		itemAct = listActs->Items->Item[i];
		if (itemAct->Checked)
			nSelected++;
	}

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要定义的节目。", L"错误", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select the acts to be defined.", L"Error", MB_OK | MB_ICONWARNING);
		return;
	}

	TfrmActDefine *frmActDefine = new TfrmActDefine(this);
	bool bOk = (frmActDefine->ShowModal() == mrOk);
	bool bAll = frmActDefine->rdAll->Checked;
	delete frmActDefine;

	if (!bOk)
		return;

	for (int i = 0; i < listActs->Items->Count; i++)
	{
		itemAct = listActs->Items->Item[i];
		if (itemAct->Checked)
		{
			sysConfig.m_bActSuitForAllCard[i] = bAll;
			itemAct->Caption = GetActIndexText(i);
			comboEditAct->Items->Strings[i] = GetActIndexText(i);
		}
	}

	sysConfig.SaveConfig();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnTestClick(TObject *Sender)
{
#ifdef _DEBUG
	int nSelected = GetCheckedCardCount();

	if (nSelected != 1)
	{
		Application->MessageBox(L"请选择1个要执行操作的显示屏", L"", MB_OK | MB_ICONWARNING);
		return;
	}

/*	if (sysConfig.m_nCommMethod != 0)
	{
		Application->MessageBox(L"请选择串口通信", L"", MB_OK | MB_ICONWARNING);
		return;
	}
*/
	if (!OpenCommPort(0, "Test"))
		return;

	::Sleep(20);

	StartOperation();
	m_dtu->m_nTxStep = 0;
	m_dtu->m_nTxStatus = TX_STATUS_READY;
	Timer1->Enabled = true;
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Timer1Timer(TObject *Sender)
{
#ifdef _DEBUG
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_ReadWriteChipId(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (dtu->m_nTxStep == 0)
				{
					if (WaitForCmdEcho(MMI_CMD_READ_CHIP_ID, dtu, 8))
					{
						dtu->m_nRetries = 0;
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 1;
						if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
							memcpy(dtu->m_ucChipId, dtu->m_ucRxBuf + sizeof(MMI_CMD_HEAD_HEB), 8);
						else
							memcpy(dtu->m_ucChipId, dtu->m_ucRxBuf + sizeof(MMI_CMD_HEAD), 8);
						m_ComPort.Close();
					}
				}
				else if (dtu->m_nTxStep == 1)
				{
					if (WaitForCmdEcho(MMI_CMD_WRITE_CHIP_ID, dtu, 0))
					{
						dtu->m_nRetries = 0;
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
						m_ComPort.Close();
					}
				}

				if (dtu->m_nTxStatus == TX_STATUS_WAIT_ECHO)
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							log.LogTx(dtu, false);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_ReadWriteChipId(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else	//GPRS
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_ReadWriteChipId(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (dtu->m_nTxStep == 0)
				{
					if (WaitForCmdEcho(MMI_CMD_READ_CHIP_ID, dtu, 8))
					{
						dtu->m_nRetries = 0;
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 1;
						if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
							memcpy(dtu->m_ucChipId, dtu->m_ucRxBuf + sizeof(MMI_CMD_HEAD_HEB), 8);
						else
							memcpy(dtu->m_ucChipId, dtu->m_ucRxBuf + sizeof(MMI_CMD_HEAD), 8);
					}
				}
				else if (dtu->m_nTxStep == 1)
				{
					if (WaitForCmdEcho(MMI_CMD_WRITE_CHIP_ID, m_dtu, 0))
					{
						dtu->m_nRetries = 0;
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
					}
				}

				if (dtu->m_nTxStatus == TX_STATUS_WAIT_ECHO)
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							log.LogTx(dtu, false);
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_ReadWriteChipId(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
							nSendDtuCount++;
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();
		Timer1->Enabled = false;
		EndOperation();
	}
#endif
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_ReadWriteChipId(CDtu* dtu)
{
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		SendCommand_ReadWriteChipId_HEB(dtu);
	else
		SendCommand_ReadWriteChipId_Normal(dtu);
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_ReadWriteChipId_Normal(CDtu* dtu)
{
#ifdef _DEBUG
	BYTE buf[64];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	BYTE* p = (BYTE*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	if (dtu->m_nTxStep == 0)
	{
		pHead->ucCmd = MMI_CMD_READ_CHIP_ID;
		pHead->usLen = 1;
		*p = 0x78;
	}
	else
	{
		pHead->ucCmd = MMI_CMD_WRITE_CHIP_ID;
		pHead->usLen = 8;
		memcpy(p, dtu->m_ucChipId, 8);
		for (int i = 0; i < 8; i++)
		{
			if ((p[i] != 0) && (p[i] != (i + 0x55)))
				p[i] ^= (i + 0x55);
		}
	}

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
#endif
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_ReadWriteChipId_HEB(CDtu* dtu)
{
#ifdef _DEBUG
	BYTE buf[64];
	MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;
	BYTE* p = (BYTE*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	if (dtu->m_nTxStep == 0)
	{
		pHead->ucCmd = MMI_CMD_READ_CHIP_ID;
		pHead->usLen = 1;
		*p = 0x78;
	}
	else
	{
		pHead->ucCmd = MMI_CMD_WRITE_CHIP_ID;
		pHead->usLen = 8;
		memcpy(p, dtu->m_ucChipId, 8);
		for (int i = 0; i < 8; i++)
		{
			if ((p[i] != 0) && (p[i] != (i + 0x55)))
				p[i] ^= (i + 0x55);
		}
	}

	pHead->ucKey1 = dtu->m_ucKey1 = GetTickCount();
	pHead->ucKey2 = dtu->m_ucKey2 = GetTickCount() >> 8;
	buf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, buf);
	Encrypt(dtu, pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
#endif
}
//---------------------------------------------------------------------------

void TfrmMain::GetPseudoIp(UnicodeString wsSimCode, BYTE* pucIp)
{
/*
伪IP同SIM卡转换函数及过程
Sim卡号转换成伪IP的方法：
1．	先将SIM卡号的第一位去掉
如13512345006 ，去掉第一位，得到3512345006
2．	得到的十位数，两两一组，得到五组数
如：3512345006 得到 35 12 34 50 06
3．	取后四组数，分别用二进制表示
如：
a)	35 12 34 50 06 的后四组数为 12 34 50 06
b)	12的2进制数为00001100
c)	34的2进制数为00100010
d)	50的2进制数为00110010
e)	06的2进制数为00000110
f)	得到的四组数分别是00001100  00100010  00110010  00000110
4．	取第二步中得到的第一组数减30，以2进制表示，并分别加到第三步中得到的四个数中
a)	35 12 34 50 06 的第一组数为 35
b)	35 - 30 = 5
c)	5的二进制数为0101
d)	使用第三步中的第一个数，在高位加上0（5的二进制数中的第一位），得到 00001100，转换成10进制数为12
e)	使用第三步中的第二个数，在高位加上1（5的二进制数中的第二位），得到 10100010 ，转换成10进制数为162
f)	使用第三步中的第三个数，在高位加上0（5的二进制数中的第三位），得到 00110010，转换成10进制数为50
g)	使用第三步中的第四个数，在高位加上1（5的二进制数中的第四位），得到 10000110，转换成10进制数为134
5．	得到伪IP
13512345006的伪IP为12.162.50.134
*/
	char szCode[32];
	strcpy(szCode, AnsiString(wsSimCode).c_str());
	//1．	先将SIM卡号的第一位去掉
	//如13512345006 ，去掉第一位，得到3512345006
	strcpy(szCode, szCode + 1);

	//2．	得到的十位数，两两一组，得到五组数
	//如：3512345006 得到 35 12 34 50 06
	BYTE uc[5];
	uc[0] = (szCode[0] - 0x30) * 10 + (szCode[1] - 0x30);
	uc[1] = (szCode[2] - 0x30) * 10 + (szCode[3] - 0x30);
	uc[2] = (szCode[4] - 0x30) * 10 + (szCode[5] - 0x30);
	uc[3] = (szCode[6] - 0x30) * 10 + (szCode[7] - 0x30);
	uc[4] = (szCode[8] - 0x30) * 10 + (szCode[9] - 0x30);

	//3．	取后四组数，分别用二进制表示
	//如：
	//a)	35 12 34 50 06 的后四组数为 12 34 50 06
	//b)	12的2进制数为00001100
	//c)	34的2进制数为00100010
	//d)	50的2进制数为00110010
	//e)	06的2进制数为00000110
	//f)	得到的四组数分别是00001100  00100010  00110010  00000110

	//4．	取第二步中得到的第一组数减30，以2进制表示，并分别加到第三步中得到的四个数中
	//a)	35 12 34 50 06 的第一组数为 35
	//b)	35 - 30 = 5
	//c)	5的二进制数为0101
	//d)	使用第三步中的第一个数，在高位加上0（5的二进制数中的第一位），得到 00001100，转换成10进制数为12
	//e)	使用第三步中的第二个数，在高位加上1（5的二进制数中的第二位），得到 10100010 ，转换成10进制数为162
	//f)	使用第三步中的第三个数，在高位加上0（5的二进制数中的第三位），得到 00110010，转换成10进制数为50
	//g)	使用第三步中的第四个数，在高位加上1（5的二进制数中的第四位），得到 10000110，转换成10进制数为134
	uc[0] -= 30;
	uc[1] += ((uc[0] & 0x08) << 4);
	uc[2] += ((uc[0] & 0x04) << 5);
	uc[3] += ((uc[0] & 0x02) << 6);
	uc[4] += ((uc[0] & 0x01) << 7);

	//5．	得到伪IP
	//13512345006的伪IP为12.162.50.134
	pucIp[0] = uc[4];
	pucIp[1] = uc[3];
	pucIp[2] = uc[2];
	pucIp[3] = uc[1];
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuEraseAllActClick(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!bDogOk)
	{
		Application->MessageBox(L"请插入2个软件狗重试。", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}
#endif
	int nSelected = 0;

	SaveAct();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
		dtu->m_nTxStep = 0;
		dtu->m_nTxStatus = TX_STATUS_READY;
		nSelected++;
		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要操作的显示屏", L"清空显示屏节目内容", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED sign.", L"Erase All Acts", MB_OK | MB_ICONWARNING);
		return;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "清空显示屏节目内容" : "Erase All Acts"))
		return;

	StartOperation();
	timerClearAllActs->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerClearAllActsTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_ClearAllActs(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (dtu->m_nTxStep == 0)
				{
					if (WaitForCmdEcho(MMI_CMD_STOP_DISPLAY, dtu, 0))
					{
						dtu->m_nRetries = 0;
						if (m_nLanguage == LANGUAGE_CHINESE)
							dtu->m_sStatus= "已暂停显示,准备清除节目";
						else
							dtu->m_sStatus= "Act Playing Stopped, Preparing Erase All Acts";
						vt->InvalidateNode(Node);
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 1;
						m_ComPort.Close();
					}
				}
				else if (dtu->m_nTxStep == 1)
				{
					if (WaitForCmdEcho(MMI_CMD_ERASE_ALL_ACTS, dtu, 0))
					{
						m_ComPort.Close();
						dtu->m_nRetries = 0;
						if (m_nLanguage == LANGUAGE_CHINESE)
							dtu->m_sStatus = "准备重新开始节目显示";
						else
							dtu->m_sStatus = "Restarting Playing Acts";
						vt->InvalidateNode(Node);
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 2;
					}
				}
				else if (dtu->m_nTxStep == 2)
				{
					if (WaitForCmdEcho(MMI_CMD_RESTART_DISPLAY, dtu, 0))
					{
						dtu->m_nRetries = 0;
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
						log.LogTx(dtu, true);
						m_ComPort.Close();
					}
				}

				if (dtu->m_nTxStatus == TX_STATUS_WAIT_ECHO)
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					DWORD dwTimeOver = (DWORD)sysConfig.m_nTimeout_SerialPort;
					if (dtu->m_nTxStep == 2)
						dwTimeOver += 5000;

					if (dwTime > dwTimeOver)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							log.LogTx(dtu, false);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_ClearAllActs(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else	//GPRS
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_ClearAllActs(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (dtu->m_nTxStep == 0)
				{
					if (WaitForCmdEcho(MMI_CMD_STOP_DISPLAY, dtu, 0))
					{
						dtu->m_nRetries = 0;
						if (m_nLanguage == LANGUAGE_CHINESE)
							dtu->m_sStatus= "已暂停显示,准备清除节目";
						else
							dtu->m_sStatus= "Act Playing Stopped, Preparing Erase All Acts";
						vt->InvalidateNode(Node);
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 1;
					}
				}
				else if (dtu->m_nTxStep == 1)
				{
					if (WaitForCmdEcho(MMI_CMD_ERASE_ALL_ACTS, dtu, 0))
					{
						dtu->m_nRetries = 0;
						if (m_nLanguage == LANGUAGE_CHINESE)
							dtu->m_sStatus = "准备重新开始节目显示";
						else
							dtu->m_sStatus = "Restarting Playing Acts";
						vt->InvalidateNode(Node);
						dtu->m_nTxStatus = TX_STATUS_READY;
						dtu->m_nTxStep = 2;
					}
				}
				else if (dtu->m_nTxStep == 2)
				{
					if (WaitForCmdEcho(MMI_CMD_RESTART_DISPLAY, dtu, 0))
					{
						dtu->m_nRetries = 0;
						dtu->m_nTxStatus = TX_STATUS_END_OK;
						dtu->m_sStatus = "OK";
						vt->InvalidateNode(Node);
						log.LogTx(dtu, true);
					}
				}

				if (dtu->m_nTxStatus == TX_STATUS_WAIT_ECHO)
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					DWORD dwTimeOver = (DWORD)sysConfig.m_nTimeout_Network * 1000;
					if (dtu->m_nTxStep == 2)
						dwTimeOver += 5000;
					if (dwTime > dwTimeOver)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							log.LogTx(dtu, false);
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_ClearAllActs(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
							nSendDtuCount++;
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();
		timerClearAllActs->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_ClearAllActs(CDtu* dtu)
{
	BYTE buf[2048];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->usLen = 0;
	if (dtu->m_nTxStep == 0)
		pHead->ucCmd = MMI_CMD_STOP_DISPLAY;
	else if (dtu->m_nTxStep == 1)
		pHead->ucCmd = MMI_CMD_ERASE_ALL_ACTS;
	else if (dtu->m_nTxStep == 2)
		pHead->ucCmd = MMI_CMD_RESTART_DISPLAY;

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuCircleClockClick(TObject *Sender)
{
	char s[2];

	sysConfig.DeleteAct(m_dtu, m_nActIndex);
	sysConfig.m_nActEnterStyle = ENTER_STYLE_DIRECT;
	sysConfig.m_nActExitStyle = EXIT_STYLE_NULL;
	sysConfig.m_nActShowTime = 999999999;
	sysConfig.m_sActText = "A";
	sysConfig.m_sActTextColor = "A";
	sysConfig.m_sActDesc = "模拟时钟";
	s[0] = '0' + 30;
	s[1] = 0;
	sysConfig.m_sActActiveText = s;
	UpdateActVol();
	wndTextEdit->Refresh();
	btnCalcDisplayTimeClick(this);
}
//---------------------------------------------------------------------------

#ifdef TWO_SOFT_DOG
short int DogBytes,DogAddr;
unsigned long DogPassword,NewPassword;
unsigned long DogResult;
unsigned char DogCascade;
void *DogData;

bool TfrmMain::Dog_Check(int nIndex)
{
	DogCascade = nIndex;
	return (DogCheck() == 0);
}
//---------------------------------------------------------------------------
#endif

void __fastcall TfrmMain::Timer_CheckDogTimer(TObject *Sender)
{
#ifdef TWO_SOFT_DOG
	if (!Dog_Check(0) || !Dog_Check(1))
	{
		bDogOk = false;
		panelScreen->Enabled = false;
		panelMisc->Enabled = false;
	}
	else
	{
		bDogOk = true;
		panelScreen->Enabled = true;
		panelMisc->Enabled = true;
	}
#else
	bDogOk = true;
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuRamTextClick(TObject *Sender)
{
	mnuRamText->Checked = !mnuRamText->Checked;
}
//---------------------------------------------------------------------------

void TfrmMain::Create9025DotFont(BYTE* p)
{
	char c = 0x20;
	int i, x, y;
	BYTE byte;
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
			*p++ = byte;
		}
	}

	//////////////////////////////
	//16*16宋体汉字
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
				*p++ = byte;

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				*p++ = byte;
			}
		}
	}
	for (nQu = 0xB0; nQu < (0xA1 + 87); nQu++)
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
				*p++ = byte;

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				*p++ = byte;
			}
		}
	}

	//////////////////////////////
	//8*16黑体 ASCII
	c = 0x20;
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
			*p++ = byte;
		}
		c++;
	}

	//////////////////////////////
	//16*16黑体汉字
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
				*p++ = byte;

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				*p++ = byte;
			}
		}
	}
	for (nQu = 0xB0; nQu < (0xA1 + 87); nQu++)
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
				*p++ = byte;

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				*p++ = byte;
			}
		}
	}

	//////////////////////////////
	//12*24宋体 ASCII
	memcpy(p, TDotFont24::ucAsciiFont + 0x20 * 48, 96 * 48);
	p += (96 * 48);

	//////////////////////////////
	//24*24宋体 汉字
	memcpy(p, TDotFont24::ucHanziFont, 3 * 94 * 72);
	p += (3 * 94 * 72);
	memcpy(p, TDotFont24::ucHanziFont + 9 * 94 * 72, 72 * 94 * 72);
	p += (72 * 94 * 72);

	//////////////////////////////
	//12*24黑体 ASCII
	c = 0x20;
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
			*p++ = byte;

			byte = 0;
			for (x = 0; x < 4; x++)
			{
				byte <<= 1;
				if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
					byte |= 1;
			}
			byte <<= 4;
			*p++ = byte;
		}
		c++;
	}

	//////////////////////////////
	//24*24黑体汉字
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
				*p++ = byte;

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				*p++ = byte;

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 16][y] & 0xE0E0E0)
						byte |= 1;
				}
				*p++ = byte;
			}
		}
	}
	for (nQu = 0xB0; nQu < (0xA1 + 87); nQu++)
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
				*p++ = byte;

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				*p++ = byte;

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 16][y] & 0xE0E0E0)
						byte |= 1;
				}
				*p++ = byte;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmMain::Create9023DotFont(BYTE* p)
{
	char c = 0x20;
	int i, x, y;
	BYTE byte;
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
			*p++ = byte;
		}
	}

	//////////////////////////////
	//16*16宋体汉字
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
				*p++ = byte;

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				*p++ = byte;
			}
		}
	}
	for (nQu = 0xB0; nQu < (0xA1 + 87); nQu++)
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
				*p++ = byte;

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				*p++ = byte;
			}
		}
	}

	//////////////////////////////
	//8*16黑体 ASCII
	c = 0x20;
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
			*p++ = byte;
		}
		c++;
	}

	//////////////////////////////
	//16*16黑体汉字
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
				*p++ = byte;

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				*p++ = byte;
			}
		}
	}
	for (nQu = 0xB0; nQu < (0xA1 + 87); nQu++)
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
				*p++ = byte;

				byte = 0;
				for (x = 0; x < 8; x++)
				{
					byte <<= 1;
					if (Image1->Canvas->Pixels[x + 8][y] & 0xE0E0E0)
						byte |= 1;
				}
				*p++ = byte;
			}
		}
	}
}
//---------------------------------------------------------------------------
/*
#define INT8U BYTE
#define INT16U WORD

INT8U gps_ucYear, gps_ucMonth, gps_ucDay, gps_ucHour, gps_ucMinute, gps_ucSecond, gps_ucOldSecond, gps_ucWeek;
INT8U gps_ucWeiDu[4];
INT8U gps_ucJingDu[4];
INT8U gps_ucSpeed[2];
INT8U gps_ucDegree[2];
BOOL gps_bFixed = FALSE;
const INT16U act_nYearDays[12] =
{
	0, 31, 59 , 90, 120, 151, 181, 212, 243, 273, 304, 334
};

BYTE mmi_uart_ucRxBuf[256];
int mmi_uart_nRxPtr;

void gps_Init(void)
{
	gps_bFixed = FALSE;
	gps_ucWeiDu[0] = 0;
	gps_ucWeiDu[1] = 0;
	gps_ucWeiDu[2] = 0;
	gps_ucWeiDu[3] = 0;
	gps_ucJingDu[0] = 0;
	gps_ucJingDu[1] = 0;
	gps_ucJingDu[2] = 0;
	gps_ucJingDu[3] = 0;
	gps_ucSpeed[0] = 0;
	gps_ucSpeed[1] = 0;
	gps_ucDegree[0] = 0;
	gps_ucDegree[1] = 0;

	strcpy((char*)mmi_uart_ucRxBuf, "$GPRMC,070953.000,A,3957.6680,N,11617.3336,E,1.73,281.50,110813,,,A*");
	mmi_uart_nRxPtr = strlen((char*)mmi_uart_ucRxBuf);
}

void gps_Proc(void)
{
	char* s = (char*)mmi_uart_ucRxBuf;
	char* s1;
	int nDays;
	int nDiv;

	if (mmi_uart_nRxPtr > 100)
	{
		mmi_uart_nRxPtr = 0;
		return;
	}

	if (mmi_uart_ucRxBuf[mmi_uart_nRxPtr - 1] != '*')
		return;

	mmi_uart_ucRxBuf[mmi_uart_nRxPtr - 1] = 0;
	mmi_uart_nRxPtr = 0;

	if (mmi_uart_ucRxBuf[6] != ',')
		return;

	//字段0：$GPRMC，语句ID，表明该语句为Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐最小定位信息
	mmi_uart_ucRxBuf[6] = 0;
	if (strcmp(s, "$GPRMC") != 0)
		return;

	//字段1：UTC时间，hhmmss.sss格式
	s += 7;
	s1 = s;
	s = strchr(s1, ',');
	if (s == NULL)
		return;
	if ((s - s1) != 10)
		return;
	if (s1[6] != '.')
		return;
	gps_ucHour = (s1[0] - '0') * 10 + (s1[1] - '0');	//Hour
	gps_ucMinute = (s1[2] - '0') * 10 + (s1[3] - '0');	//Minute
	gps_ucSecond = (s1[4] - '0') * 10 + (s1[5] - '0');	//Second

	//字段2：状态，A=定位，V=未定位
	s++;
	s1 = s;
	s = strchr(s1, ',');
	if (s == NULL)
		return;
	if ((s - s1) != 1)
		return;
	mmi_uart_ucRxBuf[0] = (*s1 == 'A') ? 1 : 0;	//是否定位

	//字段3：经度ddmm.mmmm，度分格式（前导位数不足则补0）
	s++;
	s1 = s;
	s = strchr(s1, ',');
	if (s == NULL)
		return;
	if ((s - s1) != 9)
		return;
	if (s1[4] != '.')
		return;

	mmi_uart_ucRxBuf[1] = ((s1[0] -'0') << 4) | (s1[1] -'0');
	mmi_uart_ucRxBuf[2] = ((s1[2] -'0') << 4) | (s1[3] -'0');
	mmi_uart_ucRxBuf[3] = ((s1[5] -'0') << 4) | (s1[6] -'0');
	mmi_uart_ucRxBuf[4] = ((s1[7] -'0') << 4) | (s1[8] -'0');
	//字段4：纬度N（北纬）或S（南纬）
	s++;
	s1 = s;
	s = strchr(s1, ',');
	if (s == NULL)
		return;
	if ((s - s1) != 1)
		return;
	if (*s1 == 'S')
		mmi_uart_ucRxBuf[1] |= 0x80;
	else if (*s1 != 'N')
		return;

	//字段5：经度dddmm.mmmm，度分格式（前导位数不足则补0）
	s++;
	s1 = s;
	s = strchr(s1, ',');
	if (s == NULL)
		return;
	if ((s - s1) != 10)
		return;
	if (s1[5] != '.')
		return;

	mmi_uart_ucRxBuf[5] = ((s1[0] -'0') << 4) | (s1[1] -'0');
	mmi_uart_ucRxBuf[6] = ((s1[2] -'0') << 4) | (s1[3] -'0');
	mmi_uart_ucRxBuf[7] = ((s1[4] -'0') << 4) | (s1[6] -'0');
	mmi_uart_ucRxBuf[8] = ((s1[7] -'0') << 4) | (s1[8] -'0');

	//字段6：经度E（东经）或W（西经）
	s++;
	s1 = s;
	s = strchr(s1, ',');
	if (s == NULL)
		return;
	if ((s - s1) != 1)
		return;
	if (*s1 == 'W')
		mmi_uart_ucRxBuf[5] |= 0x80;
	else if (*s1 != 'E')
		return;

	//字段7：速度，节，Knots
	s++;
	s1 = s;
	s = strchr(s1, ',');
	if (s == NULL)
		return;
	nDays = 0;
	nDiv = 0;
	while (s1 != s)
	{
		if (nDiv == 0)
		{
			if (*s1 != '.')
			{
				nDays *= 10;
				nDays += (*s1 - '0');
			}
			else
				nDiv = 1;
		}
		else
		{
			nDiv *= 10;
			nDays *= 10;
			nDays += (*s1 - '0');
		}

		if (nDays >= 10000)
			break;
		s1++;
	}
	if (nDiv == 0)
		nDiv = 1;
	nDays *= 1852;
	nDays /= (1000 * nDiv);
	if (nDays >= 9999)
		nDays = 9999;
	gps_ucSpeed[0] = ((nDays / 1000) << 4);
	nDays %= 1000;
	gps_ucSpeed[0] |= (nDays / 100);
	nDays %= 100;
	gps_ucSpeed[1] = ((nDays / 10) << 4) | (nDays % 10);	//m/s

	//字段8：方位角，度
	s++;
	s1 = s;
	s = strchr(s1, ',');
	if (s == NULL)
		return;
	nDays = 0;
	while ((s1 != s) && (*s1 != '.'))
	{
		nDays *= 10;
		nDays += (*s1 - '0');
		s1++;
	}
	if (nDays >= 359)
		nDays = 359;
	gps_ucDegree[0] = (nDays / 100);
	nDays %= 100;
	gps_ucDegree[1] = ((nDays / 10) << 4) | (nDays % 10);

	//字段9：UTC日期，DDMMYY格式
	s++;
	s1 = s;
	s = strchr(s1, ',');
	if (s == NULL)
		return;
	if ((s - s1) != 6)
		return;
	gps_ucDay = (s1[0] - '0') * 10 + (s1[1] - '0');	//Day
	gps_ucMonth = (s1[2] - '0') * 10 + (s1[3] - '0');	//Month
	gps_ucYear = (s1[4] - '0') * 10 + (s1[5] - '0');	//Year

	if (!mmi_uart_ucRxBuf[0])
	{
		return;
	}


	// UTC转换为东8区
	gps_ucHour += 8;	//Hour
	if (gps_ucHour >= 24)
	{
		gps_ucHour -= 24;
		gps_ucDay++;	//Day
		if (((gps_ucMonth == 1) || (gps_ucMonth == 3) || (gps_ucMonth == 5) ||
			 (gps_ucMonth == 7) || (gps_ucMonth == 8) || (gps_ucMonth == 10) ||
		     (gps_ucMonth == 12)) && (gps_ucDay > 31))
		{
			gps_ucDay = 1;
			gps_ucMonth++;
		}
		else if (((gps_ucMonth == 4) || (gps_ucMonth == 6) || (gps_ucMonth == 9) ||
		          (gps_ucMonth == 11)) && (gps_ucDay > 30))
		{
			gps_ucDay = 1;
			gps_ucMonth++;
		}
		else
		{
			if ((gps_ucYear & 3) == 0)
			{
				if (gps_ucDay > 29)
				{
					gps_ucDay = 1;
					gps_ucMonth++;
				}
			}
			else
			{
				if (gps_ucDay > 28)
				{
					gps_ucDay = 1;
					gps_ucMonth++;
				}
			}
		}
		if (gps_ucMonth > 12)
		{
			gps_ucMonth = 1;
			gps_ucYear++;
		}
	}

	if (gps_bFixed)
	{
		if (gps_ucSecond != gps_ucOldSecond)
		{
			{
				nDays = gps_ucYear * 365 + 6;		//2000年1月1日是星期六
				nDays += ((gps_ucYear / 4) + 1);
				nDays += act_nYearDays[gps_ucMonth - 1];
				if (gps_ucMonth > 2)
				{
					if ((gps_ucYear & 3) == 0)
						nDays++;
				}
				nDays += (gps_ucDay - 1);
				gps_ucWeek = (nDays % 7);
				if (gps_ucWeek == 0) gps_ucWeek = 7;
			}
		}
	}

	gps_ucWeiDu[0] = mmi_uart_ucRxBuf[1];
	gps_ucWeiDu[1] = mmi_uart_ucRxBuf[2];
	gps_ucWeiDu[2] = mmi_uart_ucRxBuf[3];
	gps_ucWeiDu[3] = mmi_uart_ucRxBuf[4];
	gps_ucJingDu[0] = mmi_uart_ucRxBuf[5];
	gps_ucJingDu[1] = mmi_uart_ucRxBuf[6];
	gps_ucJingDu[2] = mmi_uart_ucRxBuf[7];
	gps_ucJingDu[3] = mmi_uart_ucRxBuf[8];

	gps_bFixed = TRUE;
	gps_ucOldSecond = gps_ucSecond;

}
*/
void __fastcall TfrmMain::btnGpsClick(TObject *Sender)
{
//	gps_Init();
//	gps_Proc();
	int nSelected = GetCheckedCardCount();

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"获取控制卡(车辆)位置信息", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Query Position Infomation of Card", MB_OK | MB_ICONWARNING);
		return;
	}

	if (nSelected > 1)
	{
		Application->MessageBox(L"请只选择一个要执行操作的显示屏", L"获取控制卡(车辆)位置信息", MB_OK | MB_ICONWARNING);
		return;
	}

	if (sysConfig.m_nCommMethod == 0)
	{
		Application->MessageBox(L"仅支持通过无线通信方式获取位置信息", L"获取控制卡(车辆)位置信息", MB_OK | MB_ICONWARNING);
		return;
    }

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? L"获取控制卡(车辆)位置信息" : L"Query Position Infomation of Card"))
		return;

	::Sleep(20);

	StartOperation();
	timerQueryPosition->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerQueryPositionTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpStatusPack();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		switch (dtu->m_nTxStatus)
		{
		case TX_STATUS_READY:
			if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
			{
				SendCommand_QueryPosition(dtu);
				dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
				dtu->m_dwTxStartTime = ::GetTickCount();
				nSendDtuCount++;
			}
			break;
		case TX_STATUS_WAIT_ECHO:
			if (dtu->m_nTxStep == 1)	//在线
			{
				dtu->m_nTxStatus = TX_STATUS_END_OK;
				UpdatePositionInfo(dtu);
				vt->InvalidateNode(Node);
			}
			else if (dtu->m_nTxStep == 2) //不在线
			{
				dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
				dtu->m_sStatus = "离线";
				vt->InvalidateNode(Node);
			}
			else
			{
				dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
				if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
				{
					dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
					dtu->m_sStatus = "-";
					vt->InvalidateNode(Node);
				}
			}
			break;
		case TX_STATUS_END_OK:
			nEnd++;
			break;
		case TX_STATUS_END_TIME_OUT:
			nEnd++;
			break;
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;

		timerQueryPosition->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_QueryPosition(CDtu* dtu)
{
/*	BYTE buf[64];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_CMD_QUERY_POSITION;
	pHead->usLen = 0;
	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
*/
#pragma pack(1)
	struct
	{
		char szUserName[12];
		BYTE ucFrameType;
		char szPassword[16];
		char szDtuCode[12];
	}stData;
#pragma pack()
	size_t i;

	::strcpy(stData.szUserName, AnsiString(sysConfig.m_sUserName).c_str());
	for (i = 0; i < strlen(stData.szUserName); i++)
		stData.szUserName[i] ^= 0xAA;
	::strcpy(stData.szPassword, AnsiString(sysConfig.m_sPassword).c_str());
	for (i = 0; i < strlen(stData.szPassword); i++)
		stData.szPassword[i] ^= 0xAA;
	stData.ucFrameType = FT_GPS;
	::strcpy(stData.szDtuCode, dtu->m_sCode.c_str());

	TIdBytes ar;
	ar.set_length(sizeof(stData));
	BYTE* p = (BYTE*)&stData;
	for (i = 0; i < sizeof(stData); i++)
		ar[i] = *p++;
	try
	{
		udpSocket->SendBuffer(ar);
	}
	catch (Exception &ex)
	{
	}
}
//---------------------------------------------------------------------------

void TfrmMain::UpdatePositionInfo(CDtu* dtu)
{
//	BYTE* p = dtu->m_ucRxBuf + sizeof(MMI_CMD_HEAD);
	BYTE* p = dtu->m_ucRxBuf;

	char s[256];

	if (p[6] == 0 && p[7] == 0 && p[8] == 0 && p[9] == 0)
	{
		dtu->m_sStatus = "在线,但未成功定位";
		return;
	}

	int wd_int = BCD2Hex(p[6]);
	int wd_frac = ((BCD2Hex(p[7]) * 10000 + BCD2Hex(p[8]) * 100 + BCD2Hex(p[9]))) * 10 / 6;
	int jd_int = BCD2Hex(p[10])* 10 + (p[11] >> 4);
	int jd_frac = ((p[11] & 15) * 10000 + BCD2Hex(p[12]) * 100 + BCD2Hex(p[13])) * 10 / 6;

	sprintf(s, "时间:%02d-%02d-%02d %02d:%02d:%02d 纬度:%d.%d, 经度:%d.%d, 速度:%02X%02Xkm/小时, 方向:%02X%02X度",
			p[0], p[1], p[2], p[3], p[4], p[5], wd_int, wd_frac, jd_int, jd_frac, p[14], p[15], p[16], p[17]);
	dtu->m_sStatus = s;

	memcpy(frmGpsMap->ucGpsData, p, 18);

	frmGpsMap->m_dtu = dtu;
	frmGpsMap->Caption = "GPS 定位: " + dtu->m_sName + " -" + dtu->m_sCode;
	frmGpsMap->Show();
	frmGpsMap->StartBrowse();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuAdjustLightnessClick(TObject *Sender)
{
	int nSelected = GetCheckedCardCount();

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"手动调节显示屏亮度", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Adjust Brightness of LED signs", MB_OK | MB_ICONWARNING);
		return;
	}

	TfrmAdjustLightness *frmAdjustLightness = new TfrmAdjustLightness(this);
	frmAdjustLightness->ShowModal();
	if (frmAdjustLightness->ModalResult != mrOk)
	{
		delete frmAdjustLightness;
		return;
	}

	m_nTempLightValue = frmAdjustLightness->comboBoxLightValue0->ItemIndex;
	SYSTEMTIME st;
	DateTimeToSystemTime(frmAdjustLightness->timeStart0->DateTime, st);
	m_nTempLightMinutes = st.wHour * 60 + st.wMinute;

	delete frmAdjustLightness;

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "手动调节显示屏亮度" : "Adjust Brightness of LED signs"))
		return;

	::Sleep(20);

	StartOperation();
	timerAdjustLightness->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerAdjustLightnessTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_AdjustLightness(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_TEMP_ADJUST_LIGHT, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
					m_ComPort.Close();
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_AdjustLightness(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_AdjustLightness(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_TEMP_ADJUST_LIGHT, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
							{
								dtu->m_nRetries++;
								SendCommand_AdjustLightness(dtu);
								dtu->m_dwTxStartTime = ::GetTickCount();
								nSendDtuCount++;
							}
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timerAdjustLightness->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_AdjustLightness(CDtu* dtu)
{
	BYTE buf[64];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	BYTE* p = (BYTE*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_TEMP_ADJUST_LIGHT;
	pHead->usLen = 4;

	*((WORD*)p) = m_nTempLightMinutes;
	p++; p++;
	*p++ = m_nTempLightValue;
	*p = 0;

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnuBrightnessSetupClick(TObject *Sender)
{
	int nSelected = GetCheckedCardCount();

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"配置显示屏亮度自动调节方案", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L" Config Auto-adjusting Scheme of Brightness of LED Signs", MB_OK | MB_ICONWARNING);
		return;
	}

	TfrmBrightnessSetup *frmBrightnessSetup = new TfrmBrightnessSetup(this);
	frmBrightnessSetup->ShowModal();
	if (frmBrightnessSetup->ModalResult != mrOk)
	{
		delete frmBrightnessSetup;
		return;
	}

	delete frmBrightnessSetup;

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "手动调节显示屏亮度" : "Adjust Brightness of LED signs"))
		return;

	::Sleep(20);

	StartOperation();
	timerBrightnessSetup->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerBrightnessSetupTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_WriteBrightnessConfig(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_WRITE_LIGHT_CONFIG, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
					m_ComPort.Close();
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_WriteBrightnessConfig(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_WriteBrightnessConfig(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_WRITE_LIGHT_CONFIG, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
							{
								dtu->m_nRetries++;
								SendCommand_WriteBrightnessConfig(dtu);
								dtu->m_dwTxStartTime = ::GetTickCount();
								nSendDtuCount++;
							}
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timerBrightnessSetup->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WriteBrightnessConfig(CDtu* dtu)
{
	typedef struct
	{
		BYTE ucLightControlStartHour[MAX_LIGHT_CONTROL_COUNT];		//亮度调节开始时间
		BYTE ucLightControlStartMinute[MAX_LIGHT_CONTROL_COUNT];	//亮度调节开始时间
		BYTE ucLightControlValue[MAX_LIGHT_CONTROL_COUNT];			//亮度调节值(0-15, 0最亮)
	} LIGHT_TABLE;
	BYTE buf[256];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	LIGHT_TABLE* p = (LIGHT_TABLE*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_WRITE_LIGHT_CONFIG;
	pHead->usLen = sizeof(LIGHT_TABLE);

	for (int i = 0; i < 8; i++)
	{
		p->ucLightControlStartHour[i] = sysConfig.m_nLightControlStartHour[i];
		p->ucLightControlStartMinute[i] = sysConfig.m_nLightControlStartMinute[i];
		p->ucLightControlValue[i] = sysConfig.m_nLightControlValue[i];
	}

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

#ifdef XINJIANG_AUTO_WEB_DATA
__fastcall TWebDataThread::TWebDataThread(void):TThread(true)
{
}
//---------------------------------------------------------------------------

void __fastcall TWebDataThread::Execute()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	WORD m_nOldMinute = st.wMinute;
	TStringStream *s = new TStringStream();

	while (!Terminated)
	{
		Sleep(10000);
		::GetLocalTime(&st);
		if (st.wMinute != m_nOldMinute)
		{
			m_nOldMinute = st.wMinute;
			try
			{
				frmMain->IdHTTP1->Get(L"http://www.xjtq.gov.cn/gzfw.asp", s);
				frmMain->m_sWebData = s->DataString;
			}
			catch (Exception &ex)
			{
			}
		}
	}

	delete s;
}
//---------------------------------------------------------------------------
#endif

void __fastcall TfrmMain::timerWebDataTimer(TObject *Sender)
{
#ifdef XINJIANG_AUTO_WEB_DATA
	UnicodeString sStationName;
	UnicodeString sTemperature;
	UnicodeString sHumidity;
	UnicodeString sPressure;
	UnicodeString sRainfall;
	UnicodeString sWindDirection;
	UnicodeString sWindSpeed;

	if (m_sWebData == m_sOldWebData)
		return;
	m_sOldWebData = m_sWebData;
	//Memo1->Lines->Clear();
	//Memo1->Lines->Append(m_sOldWebData);
	//站号 站名 气温( 0.1℃)  湿度(%) 气压(0.1hPa) 降雨量(0.1mm) 风向(?) 风速(0.1m/s)
	PVirtualNode Node = vt->GetFirst();
	while (Node != NULL)
	{
		CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
		{
			if (!dtu->m_sWebDataId.IsEmpty())
			{
				wchar_t* s = m_sOldWebData.c_str();
				UnicodeString sId = dtu->m_sWebDataId + "</DIV></TD>";
				wchar_t* szId = sId.c_str();

				//<DIV align=center>51058</DIV></TD>
				//<TD width=107 bgColor=#ffffff height=25 bordercolor="#000000">
				//阿克达拉</TD>
				//<TD width=73 bgColor=#ffffff height=25 bordercolor="#000000">
				// <DIV align=center>104 </DIV></TD>
				//<TD width=64 bgColor=#ffffff height=25 bordercolor="#000000">
				//  <DIV align=center>86 </DIV></TD>
				//<TD width=78 bgColor=#ffffff height=25 bordercolor="#000000">
				//  <DIV align=center>9586 </DIV></TD>
				//<TD width=56 bgColor=#ffffff height=25 bordercolor="#000000">
				//  <DIV align=center>0   </DIV></TD>
				//<TD width=53 bgColor=#ffffff height=25 bordercolor="#000000">
				//  <DIV align=center>304</DIV></TD>
				//<TD width=69 bgColor=#ffffff height=25 bordercolor="#000000">
				//  <DIV align=center>12 </DIV></TD>

				s = wcsstr(s, szId);
				if (s != NULL)
				{
					s += wcslen(szId);
					s = wcsstr(s, L"<TD");	//站名
					s = GetWebText(s, sStationName);
					if (s != NULL)
						s = GetWebText(s, sTemperature);
					if (s != NULL)
						s = GetWebText(s, sHumidity);
					if (s != NULL)
						s = GetWebText(s, sPressure);
					if (s != NULL)
						s = GetWebText(s, sRainfall);
					if (s != NULL)
						s = GetWebText(s, sWindDirection);
					if (s != NULL)
						s = GetWebText(s, sWindSpeed);
					/*ShowMessage(sStationName + " 气温:" + sTemperature +
						" 湿度:" + sHumidity +
						" 气压:" + sPressure +
						" 降雨量:" + sRainfall +
						" 风向:" + sWindDirection +
						" 风速:" + sWindSpeed);*/
				}
			}
		}
		Node = vt->GetNext(Node);
	}
#else
	timerWebData->Enabled = false;
#endif
}
//---------------------------------------------------------------------------

#ifdef XINJIANG_AUTO_WEB_DATA
wchar_t* TfrmMain::GetWebText(wchar_t* s, UnicodeString& sResult)
{
	wchar_t* s1;
	wchar_t* s2;
	sResult = "";

	s = wcsstr(s, L">");
	if (s == NULL)
		return NULL;

	s++;
	while (*s && (*s == '\r' || *s == '\n' || *s == ' '))
		s++;

	if (s == NULL)
		return NULL;
	if (wcsncmp(s, L"<DIV", 4) == 0)
	{
		s = wcsstr(s, L">");
		if (s == NULL)
			return NULL;
		s++;
	}

	s1 = wcsstr(s, L"</");
	*s1 = 0;
	while (*s && (*s == '\r' || *s == '\n' || *s == ' '))
		s++;
	s2 = s;
	for (;;)
	{
		if (*s2 == 0 || *s2 == '\r' || *s2 == '\n' || *s2 == ' ')
			break;
		s2++;
	}
	*s1 = '<';
	wchar_t c = *s2;
	*s2 = 0;

	sResult = s;
	*s2 = c;
	s = wcsstr(s, L"</TD>");
	if (s == NULL)
		return NULL;

	return s + 5;
}
#endif
//---------------------------------------------------------------------------

void __fastcall TfrmMain::listActsChange(TObject *Sender, TListItem *Item, TItemChange Change)
{
#ifdef FIRST_ACT_DISABLED
	if (Item->Index == 0 && Change == ctState)
	{
		if (Item->Checked)
			Item->Checked = false;
	}
#endif

}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerWeatherTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_WeatherIco(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_WEATHER_ICON, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
					m_ComPort.Close();
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_WeatherIco(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_WeatherIco(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_WEATHER_ICON, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
							{
								dtu->m_nRetries++;
								SendCommand_WeatherIco(dtu);
								dtu->m_dwTxStartTime = ::GetTickCount();
								nSendDtuCount++;
							}
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timerWeather->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WeatherIco(CDtu* dtu)
{
	BYTE buf[64];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	WORD* p = (WORD*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_CMD_WEATHER_ICON;
	pHead->usLen = 0;

	for (int i = 0; i < WEATHER_ICON_COUNT; i++)
	{
		if (m_bSelected[i])
		{
			*p++ = i;
			pHead->usLen += 2;
		}
	}

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::mnuTxLedDigitClick(TObject *Sender)
{
	int nSelected = GetCheckedCardCount();
	BYTE ucDigitData[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x80, 0x00};

	if (nSelected == 0)
	{
		if (m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"请选择要执行操作的显示屏", L"发送LED数码管数据", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Please select at least one LED panel.", L"Send LED Digits", MB_OK | MB_ICONWARNING);
		return;
	}

	TfrmLedDigit *frmLedDigit = new TfrmLedDigit(this);
	frmLedDigit->editDigit->Text = m_sLedDigit;
	frmLedDigit->ShowModal();
	if (frmLedDigit->ModalResult != mrOk)
	{
		delete frmLedDigit;
		return;
	}

	m_sLedDigit = frmLedDigit->editDigit->Text;
	UnicodeString sDigits = frmLedDigit->editDigit->Text;
	delete frmLedDigit;

	wchar_t* s = sDigits.c_str();
	while (*s)
	{
		if (*s == ' ' || *s == '.'  || (*s >= '0' && *s <= '9'))
			s++;
		else
		{
			Application->MessageBox(L"仅允许输入数字、空格和小数点", L"发送LED数码管数据", MB_OK | MB_ICONWARNING);
			return;
		}
	}


	s = sDigits.c_str();
	int nLen = 0;
	while (*s)
	{
		if (nLen >= 4)
		{
			Application->MessageBox(L"数字太长", L"发送LED数码管数据", MB_OK | MB_ICONWARNING);
			return;
		}

		if (*s == ' ')
		{
			m_ucLedDigit[nLen++] = 0;
			s++;
		}
		else if (*s >= '0' && *s <= '9')
		{
			m_ucLedDigit[nLen++] = ucDigitData[*s - '0'];
			s++;
			if (*s == '.')
			{
				m_ucLedDigit[nLen - 1] |= 0x80;
				s++;
			}
		}
	}

	while (nLen < 4)
	{
		m_ucLedDigit[3] = m_ucLedDigit[2];
		m_ucLedDigit[2] = m_ucLedDigit[1];
		m_ucLedDigit[1] = m_ucLedDigit[0];
		m_ucLedDigit[0] = 0;
		nLen++;
	}

	if (!OpenCommPort(0, (m_nLanguage == LANGUAGE_CHINESE) ? "发送LED数码管数据" : "Send LED Digits"))
		return;

	::Sleep(20);

	StartOperation();
	timerLedDigit->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerLedDigitTimer(TObject *Sender)
{
	int nSelected = 0;
	int nEnd = 0;
	CDtu* dtu;
	int nSendDtuCount = 0;
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpData();

	PVirtualNode Node = GetFirstCheckedCard();
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		nSelected++;

		if (sysConfig.m_nCommMethod == 0)
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (!m_ComPort.GetOpen())
				{
					m_ComPort.Open();
					SendCommand_WriteLedDigit(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_LED_DIGIT, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
					m_ComPort.Close();
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_SerialPort)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_SerialPort)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
							m_ComPort.Close();
						}
						else
						{
							dtu->m_nRetries++;
							SendCommand_WriteLedDigit(dtu);
							dtu->m_dwTxStartTime = ::GetTickCount();
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}
		else
		{
			switch (dtu->m_nTxStatus)
			{
			case TX_STATUS_READY:
				if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
				{
					SendCommand_WriteLedDigit(dtu);
					dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
					dtu->m_dwTxStartTime = ::GetTickCount();
					nSendDtuCount++;
				}
				break;
			case TX_STATUS_WAIT_ECHO:
				if (WaitForCmdEcho(MMI_CMD_LED_DIGIT, dtu, 0))
				{
					dtu->m_nRetries = 0;
					dtu->m_nTxStatus = TX_STATUS_END_OK;
					dtu->m_sStatus = "OK";
					vt->InvalidateNode(Node);
				}
				else
				{
					dwTime = ::GetTickCount() - dtu->m_dwTxStartTime;
					if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
					{
						if (dtu->m_nRetries >= sysConfig.m_nRetry_Network)
						{
							dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
							if (m_nLanguage == LANGUAGE_CHINESE)
								dtu->m_sStatus = "失败";
							else
								dtu->m_sStatus = "Failed";
							vt->InvalidateNode(Node);
						}
						else
						{
							if (nSendDtuCount < MAX_TX_DTU_COUNT_ONE_TIME)
							{
								dtu->m_nRetries++;
								SendCommand_WriteLedDigit(dtu);
								dtu->m_dwTxStartTime = ::GetTickCount();
								nSendDtuCount++;
							}
						}
					}
				}
				break;
			case TX_STATUS_END_OK:
				nEnd++;
				break;
			case TX_STATUS_END_TIME_OUT:
				nEnd++;
				break;
			}
		}

		Node = GetNextCheckedCard(Node);
	}

	if (nSelected == nEnd)
	{
		udpSocket->Active = false;
		m_ComPort.Close();

		timerLedDigit->Enabled = false;
		EndOperation();
	}
}
//---------------------------------------------------------------------------

void TfrmMain::SendCommand_WriteLedDigit(CDtu* dtu)
{
	BYTE buf[2048];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	BYTE* pDigit = (BYTE*)(pHead + 1);

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = dtu->GetCardId();
	pHead->ucCmd = MMI_CMD_LED_DIGIT;
	pHead->usLen = 4;
	pDigit[0] = m_ucLedDigit[0];
	pDigit[1] = m_ucLedDigit[1];
	pDigit[2] = m_ucLedDigit[2];
	pDigit[3] = m_ucLedDigit[3];

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(dtu, pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------


