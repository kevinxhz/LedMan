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
		Caption = "�Զ��ط�";
		lbNote1->Caption = "ĳЩ��ʾ���Ľ�Ŀ��Ϣ��δ�ɹ����͡���ȷ��Ҫ����Ҫ�Զ��ط���";
		lbNote2->Caption = "�����δ�����µ�ʱ���ڽ��в�����ϵͳ���Զ���ʼ�ط���";
		lbNote3->Caption = "ʣ��ʱ��:";
		btnOk->Caption = "��ʼ";
		btnNo->Caption = "ȡ���Զ��ط�";
		btnCancel->Caption = "�Ƴٵ��´�";
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
		lbTime->Caption = AnsiString(m_nWaitTime) + "��";
	else
		lbTime->Caption = AnsiString(m_nWaitTime) + " s";
	if (!m_nWaitTime)
	{
		this->ModalResult = mrOk;
		this->CloseModal();
	}
}
//---------------------------------------------------------------------------

