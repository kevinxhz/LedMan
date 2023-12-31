//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ActiveType.h"
#include "SysConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmActiveType *frmActiveType;
//---------------------------------------------------------------------------
__fastcall TfrmActiveType::TfrmActiveType(TComponent* Owner)
	: TForm(Owner)
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "设定动态文字类型";
		btnOk->Caption = "确定";
		btnCancel->Caption = "取消";
		Label1->Caption = "动态类型:";
		comboActiveType->Items->Clear();
		comboActiveType->Items->Add("4位数字年(2010)");
		comboActiveType->Items->Add("2位数字年(10)");
		comboActiveType->Items->Add("2位数字月");
		comboActiveType->Items->Add("2位数字日");
		comboActiveType->Items->Add("2位数字小时");
		comboActiveType->Items->Add("2位数字分钟");
		comboActiveType->Items->Add("2位数字秒");
		comboActiveType->Items->Add("1个汉字星期(日~六)");
		comboActiveType->Items->Add("2个汉字农历年");
		comboActiveType->Items->Add("2个汉字农历月份");
		comboActiveType->Items->Add("2个汉字农历日期");
		comboActiveType->Items->Add("3位数字倒计时天数");
		comboActiveType->Items->Add("2位数字倒计时天数");
		comboActiveType->Items->Add("1位数字倒计时天数");
		comboActiveType->Items->Add("2位数字温度(-9~99)");
		comboActiveType->Items->Add("2位数字湿度(00~99)");
		comboActiveType->Items->Add("3位数字温度(-99~99)");
		comboActiveType->Items->Add("4位数字温度(-99~99.9");
		comboActiveType->Items->Add("4位数字湿度(0~99.9)");
		comboActiveType->Items->Add("2个汉字风向");
		comboActiveType->Items->Add("1字符外部数据");
		comboActiveType->Items->Add("2字符外部数据");
		comboActiveType->Items->Add("3字符外部数据");
		comboActiveType->Items->Add("4字符外部数据");
		comboActiveType->Items->Add("5字符外部数据");
		comboActiveType->Items->Add("6字符外部数据");
		comboActiveType->Items->Add("7字符外部数据");
		lbTimeBase->Caption = "倒计时基准日期：";
	}
	else
	{
		Caption = "Active Type of Text";
		btnOk->Caption = "Ok";
		btnCancel->Caption = "Cancel";
		Label1->Caption = "Active Type:";
		comboActiveType->Items->Clear();
		comboActiveType->Items->Add("Year - 4 Digits");
		comboActiveType->Items->Add("Year - 2 Digits");
		comboActiveType->Items->Add("Month - 2 Digits");
		comboActiveType->Items->Add("Day Of Month - 2 Digits");
		comboActiveType->Items->Add("Hour - 2 Digits");
		comboActiveType->Items->Add("Minute - 2 Digits");
		comboActiveType->Items->Add("Second - 2 Digits");
		comboActiveType->Items->Add("Week - 1 Chinese Char");
		comboActiveType->Items->Add("Lunar Year - 2 Chinese Char");
		comboActiveType->Items->Add("Lunar Month - 2 Chinese Char");
		comboActiveType->Items->Add("Lunar Date -  2 Chinese Char");
		comboActiveType->Items->Add("Day Down Counter - 3 Digits");
		comboActiveType->Items->Add("Day Down Counter - 2 Digits");
		comboActiveType->Items->Add("Day Down Counter - 1 Digit");
		lbTimeBase->Caption = "Base Date of Down Counter:";
	}
	comboActiveType->ItemIndex = 0;
	comboActiveTypeClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmActiveType::comboActiveTypeClick(TObject *Sender)
{
	switch (comboActiveType->ItemIndex)
	{
	case 11:
	case 12:
	case 13:
		lbTimeBase->Visible = true;
		dateCounterBy->Visible = true;
		lbExtDataId->Visible = false;
		cbExtDataId->Visible = false;
		break;
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
		lbTimeBase->Visible = false;
		dateCounterBy->Visible = false;
		lbExtDataId->Visible = true;
		cbExtDataId->Visible = true;
		break;
	default:
		lbTimeBase->Visible = false;
		dateCounterBy->Visible = false;
		lbExtDataId->Visible = false;
		cbExtDataId->Visible = false;
		break;
	}
}
//---------------------------------------------------------------------------

