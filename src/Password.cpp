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
		Caption = "�޸Ĺ���Ա����";
		editOldPassword->EditLabel->Caption = "����������룺";
		editNewPassword->EditLabel->Caption = "�����������룺";
		editConfirmPassword->EditLabel->Caption = "�������������룺";
		btnOk->Caption = "ȷ��";
		btnCancel->Caption = "ȡ��";
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
			::MessageBox(this->Handle, L"��������ȷ�����룡", L"������", MB_OK | MB_ICONWARNING);
		else
			::MessageBox(this->Handle, L"Please input correct password!", L"Old Password Error", MB_OK | MB_ICONWARNING);
		ModalResult = mrNone;
		return;
	}

	if (editNewPassword->Text != editConfirmPassword->Text)
	{
		if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
			::MessageBox(this->Handle, L"ȷ�������������벻һ�£����飡", L"ȷ���������", MB_OK | MB_ICONWARNING);
		else
			::MessageBox(this->Handle, L"please confirm input same new password.", L"New Password Confirm Failed", MB_OK | MB_ICONWARNING);
		ModalResult = mrNone;
		return;
	}

	sysConfig.SaveSystemPassword(editNewPassword->Text);

	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
		::MessageBox(this->Handle, L"�޸�����ɹ������μ������룬�����˳��󽫲��ܵ�¼��", L"�޸�����ɹ�", MB_OK);
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
