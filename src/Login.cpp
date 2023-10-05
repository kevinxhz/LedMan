#include <vcl.h>
#pragma hdrstop
#include "Login.h"
#include "SysConfig.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TLoginForm::TLoginForm(TComponent* Owner)
  : TForm(Owner)
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "µÇÂ¼";
		editPassword->EditLabel->Caption = "ÇëÊäÈëÃÜÂë£º";
		btnOk->Caption = "È·¶¨";
		btnCancel->Caption = "È¡Ïû";
	}
	else
	{
		Caption = "Login";
		editPassword->EditLabel->Caption = "Please Input Password:";
		btnOk->Caption = "&OK";
		btnCancel->Caption = "&Cancel";
	}
}
//---------------------------------------------------------------------------
void __fastcall TLoginForm::btnOkClick(TObject *Sender)
{
	if (editPassword->Text == sysConfig.m_sSystemPassword)
		ModalResult = mrOk;
	else
	{
		if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
			::MessageBox(this->Handle, L"ÇëÊäÈëÕýÈ·µÄµÇÂ¼ÃÜÂë£¡", L"ÃÜÂë´íÎó", MB_OK | MB_ICONWARNING);
		else
			::MessageBox(this->Handle, L"Please input correct password!", L"Password Error", MB_OK | MB_ICONWARNING);
		editPassword->Text = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TLoginForm::btnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

