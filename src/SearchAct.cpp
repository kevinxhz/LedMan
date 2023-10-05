//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SearchAct.h"
#include "SysConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSearchAct *frmSearchAct;
//---------------------------------------------------------------------------
__fastcall TfrmSearchAct::TfrmSearchAct(TComponent* Owner)
	: TForm(Owner)
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "查找节目";
		lbText->Caption = "节目文字内容的全部或部分，*表示查找所有包含有效内容的节目:";
		lbDescription->Caption = "节目说明的全部或部分，*表示查找所有已注明说明的节目:";
		btnSearch->Caption = "查找(&S)";
		btnCancel->Caption = "取消(&C)";
	}
	else
	{
		Caption = "Search Acts";
		lbText->Caption = "(part of ) Act's text. * means all acts which has any text";
		lbDescription->Caption = "(part of ) Act's description. * means all acts which has any description";
		btnSearch->Caption = "&Search";
		btnCancel->Caption = "&Cancel";
	}
}
//---------------------------------------------------------------------------
