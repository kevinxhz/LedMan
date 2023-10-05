//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <stdio.h>

#include "SignCheck.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmCheckResult *frmCheckResult;

//---------------------------------------------------------------------------
__fastcall TfrmCheckResult::TfrmCheckResult(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TfrmCheckResult::AddItem(UnicodeString sName, DWORD ulData)
{
	char s[128];

	SYSTEMTIME st;
	::GetLocalTime(&st);
	sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	UnicodeString sItem = s;
	sItem += sName;

	if (ulData)
	{
		sItem += " ÏÔÊ¾ÆÁ¹ÊÕÏ: ";
		if (ulData & CHECK_RESULT_OE_BIT_1)
			sItem += "OE1 ";
		if (ulData & CHECK_RESULT_CK_BIT_1)
			sItem += "CK1 ";
		if (ulData & CHECK_RESULT_ST_BIT_1)
			sItem += "ST1 ";
		if (ulData & CHECK_RESULT_A_BIT_1)
			sItem += "A1 ";
		if (ulData & CHECK_RESULT_B_BIT_1)
			sItem += "B1 ";
		if (ulData & CHECK_RESULT_C_BIT_1)
			sItem += "C1 ";
		if (ulData & CHECK_RESULT_D_BIT_1)
			sItem += "D1 ";
		if (ulData & CHECK_RESULT_R1_BIT_1)
			sItem += "R1 ";
		if (ulData & CHECK_RESULT_R2_BIT_1)
			sItem += "R2 ";
		if (ulData & CHECK_RESULT_G1_BIT_1)
			sItem += "G1 ";
		if (ulData & CHECK_RESULT_G2_BIT_1)
			sItem += "G2 ";

		if (ulData & CHECK_RESULT_OE_BIT_2)
			sItem += "OE2 ";
		if (ulData & CHECK_RESULT_CK_BIT_2)
			sItem += "CK2 ";
		if (ulData & CHECK_RESULT_ST_BIT_2)
			sItem += "ST2 ";
		if (ulData & CHECK_RESULT_A_BIT_2)
			sItem += "A2 ";
		if (ulData & CHECK_RESULT_B_BIT_2)
			sItem += "B2 ";
		if (ulData & CHECK_RESULT_C_BIT_2)
			sItem += "C2 ";
		if (ulData & CHECK_RESULT_D_BIT_2)
			sItem += "D2 ";
		if (ulData & CHECK_RESULT_R1_BIT_2)
			sItem += "R3 ";
		if (ulData & CHECK_RESULT_R2_BIT_2)
			sItem += "R4 ";
		if (ulData & CHECK_RESULT_G1_BIT_2)
			sItem += "G3 ";
		if (ulData & CHECK_RESULT_G2_BIT_2)
			sItem += "G4 ";
	}
	else
		sItem += " ÏÔÊ¾ÆÁ¹ÊÕÏ»Ö¸´";

	lstCheck->Items->Append(sItem);

	SaveToFile();
}
//---------------------------------------------------------------------------

void TfrmCheckResult::SaveToFile()
{
	AnsiString szItem;

	DeleteFile(ExtractFilePath(Application->ExeName) + "log\\CheckResult.txt");
	int hFile = FileCreate(ExtractFilePath(Application->ExeName) + "log\\CheckResult.txt", fmOpenWrite);
	for (int i = 0; i < lstCheck->Count; i++)
	{
		szItem = lstCheck->Items->Strings[i] + "\r\n";
		FileWrite(hFile, szItem.c_str(), szItem.Length());
	}
	FileClose(hFile);
}
//---------------------------------------------------------------------------

void __fastcall TfrmCheckResult::btnCancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmCheckResult::btnDeleteClick(TObject *Sender)
{
	for (int i = lstCheck->Count - 1; i >= 0; i--)
	{
		if (lstCheck->Selected[i])
			lstCheck->Items->Delete(i);
	}

	SaveToFile();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCheckResult::FormResize(TObject *Sender)
{
	if (this->Width < 256)
		this->Width = 256;
	if (this->Height < 256)
		this->Height = 256;

	lstCheck->Width = this->Width - 147;
	lstCheck->Height = this->Height - 72;
	btnDelete->Left = this->Width - 116;
	btnCancel->Left = this->Width - 116;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCheckResult::FormCreate(TObject *Sender)
{
	char buf[256];
	char c;

	int hFile = FileOpen(ExtractFilePath(Application->ExeName) + "log\\CheckResult.txt", fmOpenRead);

	char* s = buf;
	int nLen = 0;
	while (FileRead(hFile, &c, 1) == 1)
	{
		if (c == '\r' || c == '\n')
		{
			if (nLen)
			{
				*s = 0;
				lstCheck->Items->Append(buf);
				s = buf;
				nLen = 0;
			}
		}
		else
		{
			if (nLen < 255)
			{
				*s++ = c;
				nLen++;
			}
		}
	}

	FileClose(hFile);
}
//---------------------------------------------------------------------------

