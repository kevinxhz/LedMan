//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Password.h"
#include "SysConfig.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmPassword::TfrmPassword(TComponent* Owner)
	: TForm(Owner)
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "修改管理员密码";
		editOldPassword->EditLabel->Caption = "请输入旧密码：";
		editNewPassword->EditLabel->Caption = "请输入新密码：";
		editConfirmPassword->EditLabel->Caption = "请再输入新密码：";
		btnOk->Caption = "确定";
		btnCancel->Caption = "取消";
	}
	else
	{
		Caption = "Change Operating Password of Software";
		editOldPassword->EditLabel->Caption = "Please input the old password:";
		editNewPassword->EditLabel->Caption = "Please input new password:";
		editConfirmPassword->EditLabel->Caption = "Please input new password again:";
		btnOk->Caption = "&OK";
		btnCancel->Caption = "&Cancel";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmPassword::btnOkClick(TObject *Sender)
{
	if ((editOldPassword->Enabled) && (editOldPassword->Text != sysConfig.m_sSystemPassword))
	{
		if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
			::MessageBox(this->Handle, L"请输入正确的密码！", L"旧密码", MB_OK | MB_ICONWARNING);
		else
			::MessageBox(this->Handle, L"Please input correct password!", L"Old Password Error", MB_OK | MB_ICONWARNING);
		ModalResult = mrNone;
		return;
	}

	if (editNewPassword->Text != editConfirmPassword->Text)
	{
		if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
			::MessageBox(this->Handle, L"确认密码与新密码不一致，请检查！", L"确认密码错误", MB_OK | MB_ICONWARNING);
		else
			::MessageBox(this->Handle, L"please confirm input same new password.", L"New Password Confirm Failed", MB_OK | MB_ICONWARNING);
		ModalResult = mrNone;
		return;
	}

	sysConfig.SaveSystemPassword(editNewPassword->Text);

	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
		::MessageBox(this->Handle, L"修改密码成功！请牢记新密码，否则退出后将不能登录！", L"修改密码成功", MB_OK);
	else
		::MessageBox(this->Handle, L"Change password OK! please remember the new password well to avoid to be refused by the software.", L"Change Password OK", MB_OK);
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmPassword::FormCreate(TObject *Sender)
{
	editOldPassword->Enabled = !sysConfig.m_sSystemPassword.IsEmpty();
}
//---------------------------------------------------------------------------
