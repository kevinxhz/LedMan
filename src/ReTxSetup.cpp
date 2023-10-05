//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ReTxSetup.h"
#include "SysConfig.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmReTxSetup::TfrmReTxSetup(TComponent* Owner)
	: TForm(Owner)
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "自动重发";
		btnOk->Caption = "确定";
		btnCancel->Caption = "取消";
		Label2->Caption = "节目发送失败后每隔";
		Label3->Caption = "分钟自动重发（0表示不自动重发）";
		editAutoRetryTime->Left = 138;
		Label3->Left = 191;
		checkOnlyLogLastTx->Caption = "手动发送时清除之前的发送失败记录";
	}
	else
	{
		Caption = "Automatical Re-Send";
		btnOk->Caption = "OK";
		btnCancel->Caption = "Cancel";
		Label2->Caption = "If Failed to Send Acts, After";
		Label3->Caption = "Minutes to Retry(0- Never)";
		editAutoRetryTime->Left = 164;
		Label3->Left = 218;
		checkOnlyLogLastTx->Caption = "Clear failed logs before send acts manually";
	}
}
//---------------------------------------------------------------------------

