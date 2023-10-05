//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "SearchCard.h"
#include "SysConfig.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSearchCard *frmSearchCard;
//---------------------------------------------------------------------------
__fastcall TfrmSearchCard::TfrmSearchCard(TComponent* Owner)
	: TForm(Owner)
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "������ʾ��";
		lbNote->Caption = "������������ʾ�����ƿ��Ŀ��š���Ż���������һ��������λ����ʾ��";
		lbCardId->Caption = "����";
		lbCode->Caption = "������)�豸����";
		lbDescription->Caption = "������)����";
		btnSearch->Caption = "����(&S)";
		btnCancel->Caption = "ȡ��(&C)";
	}
	else
	{
		Caption = "Search LED Panel(Card)";
		lbNote->Caption = "You can use part of card code or descriptions to find target.";
		lbCardId->Caption = "Card Id";
		lbCode->Caption = "(Part of) Device Code";
		lbDescription->Caption = "(Part of) Descriptions";
		btnSearch->Caption = "&Search";
		btnCancel->Caption = "&Cancel";
	}
}
//---------------------------------------------------------------------------
