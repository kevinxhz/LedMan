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
		Caption = "查找显示屏";
		lbNote->Caption = "您可以输入显示屏控制卡的卡号、编号或其描述的一部分来定位该显示屏";
		lbCardId->Caption = "卡号";
		lbCode->Caption = "（部分)设备编码";
		lbDescription->Caption = "（部分)描述";
		btnSearch->Caption = "查找(&S)";
		btnCancel->Caption = "取消(&C)";
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
