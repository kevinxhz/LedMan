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
		Caption = "批量新建显示屏(控制卡)";
		btnOk->Caption = "确定";
		btnCancel->Caption = "取消";
		Label2->Caption = "1. 确认所选的显示屏(控制卡)已正确配置，且其设备编号的后三位为";
		Label3->Caption = "该批连号的控制卡中最小的一个";
		Label4->Caption = "2. 输入要新建的显示屏（控制卡）的数量(1-998):";
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
		::MessageBox(this->Handle, L"请重新输入显示屏（控制卡）的数量(1-998)!", NULL, MB_OK | MB_ICONWARNING);
		ModalResult = mrNone;
		return;
	}
}
//---------------------------------------------------------------------------

