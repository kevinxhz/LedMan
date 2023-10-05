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
		Caption = "�Զ��ط�";
		btnOk->Caption = "ȷ��";
		btnCancel->Caption = "ȡ��";
		Label2->Caption = "��Ŀ����ʧ�ܺ�ÿ��";
		Label3->Caption = "�����Զ��ط���0��ʾ���Զ��ط���";
		editAutoRetryTime->Left = 138;
		Label3->Left = 191;
		checkOnlyLogLastTx->Caption = "�ֶ�����ʱ���֮ǰ�ķ���ʧ�ܼ�¼";
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

