//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ActiveType.h"
#include "SysConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmActiveType *frmActiveType;
//---------------------------------------------------------------------------
__fastcall TfrmActiveType::TfrmActiveType(TComponent* Owner)
	: TForm(Owner)
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "�趨��̬��������";
		btnOk->Caption = "ȷ��";
		btnCancel->Caption = "ȡ��";
		Label1->Caption = "��̬����:";
		comboActiveType->Items->Clear();
		comboActiveType->Items->Add("4λ������(2010)");
		comboActiveType->Items->Add("2λ������(10)");
		comboActiveType->Items->Add("2λ������");
		comboActiveType->Items->Add("2λ������");
		comboActiveType->Items->Add("2λ����Сʱ");
		comboActiveType->Items->Add("2λ���ַ���");
		comboActiveType->Items->Add("2λ������");
		comboActiveType->Items->Add("1����������(��~��)");
		comboActiveType->Items->Add("2������ũ����");
		comboActiveType->Items->Add("2������ũ���·�");
		comboActiveType->Items->Add("2������ũ������");
		comboActiveType->Items->Add("3λ���ֵ���ʱ����");
		comboActiveType->Items->Add("2λ���ֵ���ʱ����");
		comboActiveType->Items->Add("1λ���ֵ���ʱ����");
		comboActiveType->Items->Add("2λ�����¶�(-9~99)");
		comboActiveType->Items->Add("2λ����ʪ��(00~99)");
		comboActiveType->Items->Add("3λ�����¶�(-99~99)");
		comboActiveType->Items->Add("4λ�����¶�(-99~99.9");
		comboActiveType->Items->Add("4λ����ʪ��(0~99.9)");
		comboActiveType->Items->Add("2�����ַ���");
		comboActiveType->Items->Add("1�ַ��ⲿ����");
		comboActiveType->Items->Add("2�ַ��ⲿ����");
		comboActiveType->Items->Add("3�ַ��ⲿ����");
		comboActiveType->Items->Add("4�ַ��ⲿ����");
		comboActiveType->Items->Add("5�ַ��ⲿ����");
		comboActiveType->Items->Add("6�ַ��ⲿ����");
		comboActiveType->Items->Add("7�ַ��ⲿ����");
		lbTimeBase->Caption = "����ʱ��׼���ڣ�";
	}
	else
	{
		Caption = "Active Type of Text";
		btnOk->Caption = "Ok";
		btnCancel->Caption = "Cancel";
		Label1->Caption = "Active Type:";
		comboActiveType->Items->Clear();
		comboActiveType->Items->Add("Year - 4 Digits");
		comboActiveType->Items->Add("Year - 2 Digits");
		comboActiveType->Items->Add("Month - 2 Digits");
		comboActiveType->Items->Add("Day Of Month - 2 Digits");
		comboActiveType->Items->Add("Hour - 2 Digits");
		comboActiveType->Items->Add("Minute - 2 Digits");
		comboActiveType->Items->Add("Second - 2 Digits");
		comboActiveType->Items->Add("Week - 1 Chinese Char");
		comboActiveType->Items->Add("Lunar Year - 2 Chinese Char");
		comboActiveType->Items->Add("Lunar Month - 2 Chinese Char");
		comboActiveType->Items->Add("Lunar Date -  2 Chinese Char");
		comboActiveType->Items->Add("Day Down Counter - 3 Digits");
		comboActiveType->Items->Add("Day Down Counter - 2 Digits");
		comboActiveType->Items->Add("Day Down Counter - 1 Digit");
		lbTimeBase->Caption = "Base Date of Down Counter:";
	}
	comboActiveType->ItemIndex = 0;
	comboActiveTypeClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmActiveType::comboActiveTypeClick(TObject *Sender)
{
	switch (comboActiveType->ItemIndex)
	{
	case 11:
	case 12:
	case 13:
		lbTimeBase->Visible = true;
		dateCounterBy->Visible = true;
		lbExtDataId->Visible = false;
		cbExtDataId->Visible = false;
		break;
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
		lbTimeBase->Visible = false;
		dateCounterBy->Visible = false;
		lbExtDataId->Visible = true;
		cbExtDataId->Visible = true;
		break;
	default:
		lbTimeBase->Visible = false;
		dateCounterBy->Visible = false;
		lbExtDataId->Visible = false;
		cbExtDataId->Visible = false;
		break;
	}
}
//---------------------------------------------------------------------------
