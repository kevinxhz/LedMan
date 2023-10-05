//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TxLogView.h"
#include "SysConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmTxLogView::TfrmTxLogView(TComponent* Owner, CDtu* dtuRoot)
	: TForm(Owner)
{
	m_dtuRoot = dtuRoot;

	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "发送日志";
		btnPrev->Caption = "上一次发送记录";
		btnNext->Caption = "下一次发送记录";
		lbOk->Caption = "发送成功的目标显示屏:";
		lbError->Caption = "发送失败的目标显示屏:";
		lbActs->Caption = "发送的节目内容:";
	}
	else
	{
		Caption = "Act Sending Log";
		btnPrev->Caption = "Previous Log";
		btnNext->Caption = "Next Log";
		lbOk->Caption = "LED panels which acts was sent successfully:";
		lbError->Caption = "LED panels which acts was not sent successfully:";
		lbActs->Caption = "Act's content sent:";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmTxLogView::FormCreate(TObject *Sender)
{
	TIniFile* ini = new TIniFile(ExtractFilePath(Application->ExeName) + "log\\TxLogSum.txt");
	m_nLogTxCount = ini->ReadInteger("ACTLOG", "TxCount", 0);
	delete ini;

	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
		lbCount->Caption = "累计共发送" + AnsiString(m_nLogTxCount) + "次";
	else
		lbCount->Caption = "Total sent " + AnsiString(m_nLogTxCount) + " times";
	m_nLogIndex = m_nLogTxCount;
	InitView();
}
//---------------------------------------------------------------------------

void TfrmTxLogView::InitView()
{
	lstAct->Clear();
	lstCardOk->Clear();
	lstCardFail->Clear();

	if (m_nLogTxCount == 0)
	{
		btnPrev->Enabled = false;
		btnNext->Enabled = false;
		if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
			lbCount->Caption = "无发送记录";
		else
			lbCount->Caption = "Empty Log";
		lbTime->Caption = "";
		return;
	}

	btnPrev->Enabled = (m_nLogIndex > 1);
	btnNext->Enabled  = (m_nLogIndex < m_nLogTxCount);

	TIniFile* ini = new TIniFile(ExtractFilePath(Application->ExeName) + "log\\TxLogSum.txt");
	AnsiString sTime = ini->ReadString("ACTLOG", "TX" + AnsiString(m_nLogIndex), "");
	delete ini;

	AnsiString sPath = ExtractFilePath(Application->ExeName) + "log\\" + sTime + ".txt";

	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
		lbTime->Caption = "第" + AnsiString(m_nLogIndex) + "发送时间: " + sTime;
	else
	{
		char sTemp[256];
		strncpy(sTemp, sTime.c_str(), 128);
		char* s = sTemp;
		while (*s)
		{
			if (*s == '_')
			{
				*s = ' ';
				s++;
				break;
			}
			s++;
		}

		while (*s)
		{
			if (*s == '_')
				*s = ':';
			s++;
		}

		lbTime->Caption = "The " + AnsiString(m_nLogIndex) + " time sending start at: " + sTemp;
	}

	ini = new TIniFile(sPath);
	AnsiString sActDesc;

	for (int nActIndex = 0; nActIndex < sysConfig.m_nMaxActCount; nActIndex++)
	{
		if (ini->ValueExists("ACT", "ActText" + AnsiString(nActIndex + 1)))
		{
			sActDesc = ini->ReadString("ACT", "ActText" + AnsiString(nActIndex + 1), "");
			if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
				lstAct->AddItem("节目" + AnsiString(nActIndex + 1) + " " + sActDesc, NULL);
			else
				lstAct->AddItem("Act " + AnsiString(nActIndex + 1) + " " + sActDesc, NULL);
		}
		else if (ini->ValueExists("ACT", "ActDesc" + AnsiString(nActIndex + 1)))
		{
			sActDesc = ini->ReadString("ACT", "ActDesc" + AnsiString(nActIndex + 1), "");
			if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
				lstAct->AddItem("节目" + AnsiString(nActIndex + 1) + " " + sActDesc, NULL);
			else
				lstAct->AddItem("Act " + AnsiString(nActIndex + 1) + " " + sActDesc, NULL);
		}
	}

	if (lstAct->Items->Count)
		lstAct->ItemIndex = 0;

	AnsiString sCardCode;
	AnsiString sCardName;
	int nCardId = 1;
	bool bOk;
	for (;;)
	{

		sCardCode = ini->ReadString("CARD", "CODE_" + AnsiString(nCardId), "");
		if (sCardCode.IsEmpty())
			break;

		sCardName = ini->ReadString("CARD", "NAME_" + AnsiString(nCardId), "");
		bOk = ini->ReadBool("CARD", "OK_" + AnsiString(nCardId), false);

		if (bOk)
			lstCardOk->AddItem(sCardCode + " " + sCardName, NULL);
		else
			lstCardFail->AddItem(sCardCode + " " + sCardName, NULL);

		nCardId++;
	}

	if (lstCardOk->Items->Count)
		lstCardOk->ItemIndex = 0;
	if (lstCardFail->Items->Count)
		lstCardFail->ItemIndex = 0;

	delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTxLogView::btnPrevClick(TObject *Sender)
{
	m_nLogIndex--;
	InitView();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTxLogView::btnNextClick(TObject *Sender)
{
	m_nLogIndex++;
	InitView();
}
//---------------------------------------------------------------------------

