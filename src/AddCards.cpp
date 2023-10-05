//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddCards.h"
#include "SysConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAddCards *frmAddCards;
//---------------------------------------------------------------------------
__fastcall TfrmAddCards::TfrmAddCards(TComponent* Owner)
	: TForm(Owner)
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "�����½���ʾ��(���ƿ�)";
		btnOk->Caption = "ȷ��";
		btnCancel->Caption = "ȡ��";
		Label2->Caption = "1. ȷ����ѡ����ʾ��(���ƿ�)����ȷ���ã������豸��ŵĺ���λΪ";
		Label3->Caption = "�������ŵĿ��ƿ�����С��һ��";
		Label4->Caption = "2. ����Ҫ�½�����ʾ�������ƿ���������(1-998):";
	}
	else
	{
		Caption = "Add Cards(LED signs)";
		btnOk->Caption = "Ok";
		btnCancel->Caption = "Cancel";
		Label2->Caption = "1. Be sure the selected card has been configued ok, and";
		Label3->Caption = "its code is the smallest one";
		Label4->Caption = "2. Input the count of cards to be add(1-998):";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmAddCards::btnOkClick(TObject *Sender)
{
	int nCount = editCount->Text.ToInt();
	if ((nCount < 1) || (nCount > 998))
	{
		::MessageBox(this->Handle, L"������������ʾ�������ƿ���������(1-998)!", NULL, MB_OK | MB_ICONWARNING);
		ModalResult = mrNone;
		return;
	}
}
//---------------------------------------------------------------------------

