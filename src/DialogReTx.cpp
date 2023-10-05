//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DialogReTx.h"
#include "SysConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAutoReTx *frmAutoReTx;
//---------------------------------------------------------------------------
__fastcall TfrmAutoReTx::TfrmAutoReTx(TComponent* Owner)
	: TForm(Owner)
{
	m_nWaitTime = 30;
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "自动重发";
		lbNote1->Caption = "某些显示屏的节目信息尚未成功发送。您确定要现在要自动重发吗？";
		lbNote2->Caption = "如果您未在如下的时间内进行操作，系统将自动开始重发。";
		lbNote3->Caption = "剩余时间:";
		btnOk->Caption = "开始";
		btnNo->Caption = "取消自动重发";
		btnCancel->Caption = "推迟到下次";
	}
	else
	{
		Caption = "Automaic Retry Send Acts";
		lbNote1->Caption = "Any acts of some panels was not sent successfully. Do you want to retry now?";
		lbNote2->Caption = "If you do not make up you mind in the time below, system will send automaticlly.";
		lbNote3->Caption = "Remaining:";
		btnOk->Caption = "Start";
		btnNo->Caption = "Cancel Auto Retry";
		btnCancel->Caption = "Defer to Next";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAutoReTx::timerWaitTimer(TObject *Sender)
{
	m_nWaitTime--;
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
		lbTime->Caption = AnsiString(m_nWaitTime) + "秒";
	else
		lbTime->Caption = AnsiString(m_nWaitTime) + " s";
	if (!m_nWaitTime)
	{
		this->ModalResult = mrOk;
		this->CloseModal();
	}
}
//---------------------------------------------------------------------------

