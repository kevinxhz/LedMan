//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BrightnessSetup.h"
#include "SysConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmBrightnessSetup::TfrmBrightnessSetup(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmBrightnessSetup::FormCreate(TObject *Sender)
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	st.wSecond = 0;
	st.wMilliseconds = 0;

	st.wHour = sysConfig.m_nLightControlStartHour[0];
	st.wMinute = sysConfig.m_nLightControlStartMinute[0];
	timeStart0->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[1];
	st.wMinute = sysConfig.m_nLightControlStartMinute[1];
	timeStart1->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[2];
	st.wMinute = sysConfig.m_nLightControlStartMinute[2];
	timeStart2->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[3];
	st.wMinute = sysConfig.m_nLightControlStartMinute[3];
	timeStart3->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[4];
	st.wMinute = sysConfig.m_nLightControlStartMinute[4];
	timeStart4->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[5];
	st.wMinute = sysConfig.m_nLightControlStartMinute[5];
	timeStart5->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[6];
	st.wMinute = sysConfig.m_nLightControlStartMinute[6];
	timeStart6->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[7];
	st.wMinute = sysConfig.m_nLightControlStartMinute[7];
	timeStart7->Time = SystemTimeToDateTime(st);

	comboBoxLightValue0->ItemIndex = sysConfig.m_nLightControlValue[0];
	comboBoxLightValue1->ItemIndex = sysConfig.m_nLightControlValue[1];
	comboBoxLightValue2->ItemIndex = sysConfig.m_nLightControlValue[2];
	comboBoxLightValue3->ItemIndex = sysConfig.m_nLightControlValue[3];
	comboBoxLightValue4->ItemIndex = sysConfig.m_nLightControlValue[4];
	comboBoxLightValue5->ItemIndex = sysConfig.m_nLightControlValue[5];
	comboBoxLightValue6->ItemIndex = sysConfig.m_nLightControlValue[6];
	comboBoxLightValue7->ItemIndex = sysConfig.m_nLightControlValue[7];
}
//---------------------------------------------------------------------------

void __fastcall TfrmBrightnessSetup::btnOkClick(TObject *Sender)
{
	SYSTEMTIME st;

	DateTimeToSystemTime(timeStart0->Time, st);
	sysConfig.m_nLightControlStartHour[0] = st.wHour;
	sysConfig.m_nLightControlStartMinute[0] = st.wMinute;

	DateTimeToSystemTime(timeStart1->Time, st);
	sysConfig.m_nLightControlStartHour[1] = st.wHour;
	sysConfig.m_nLightControlStartMinute[1] = st.wMinute;

	DateTimeToSystemTime(timeStart2->Time, st);
	sysConfig.m_nLightControlStartHour[2] = st.wHour;
	sysConfig.m_nLightControlStartMinute[2] = st.wMinute;

	DateTimeToSystemTime(timeStart3->Time, st);
	sysConfig.m_nLightControlStartHour[3] = st.wHour;
	sysConfig.m_nLightControlStartMinute[3] = st.wMinute;

	DateTimeToSystemTime(timeStart4->Time, st);
	sysConfig.m_nLightControlStartHour[4] = st.wHour;
	sysConfig.m_nLightControlStartMinute[4] = st.wMinute;

	DateTimeToSystemTime(timeStart5->Time, st);
	sysConfig.m_nLightControlStartHour[5] = st.wHour;
	sysConfig.m_nLightControlStartMinute[5] = st.wMinute;

	DateTimeToSystemTime(timeStart6->Time, st);
	sysConfig.m_nLightControlStartHour[6] = st.wHour;
	sysConfig.m_nLightControlStartMinute[6] = st.wMinute;

	DateTimeToSystemTime(timeStart7->Time, st);
	sysConfig.m_nLightControlStartHour[7] = st.wHour;
	sysConfig.m_nLightControlStartMinute[7] = st.wMinute;

	sysConfig.m_nLightControlValue[0] = comboBoxLightValue0->ItemIndex;
	sysConfig.m_nLightControlValue[1] = comboBoxLightValue1->ItemIndex;
	sysConfig.m_nLightControlValue[2] = comboBoxLightValue2->ItemIndex;
	sysConfig.m_nLightControlValue[3] = comboBoxLightValue3->ItemIndex;
	sysConfig.m_nLightControlValue[4] = comboBoxLightValue4->ItemIndex;
	sysConfig.m_nLightControlValue[5] = comboBoxLightValue5->ItemIndex;
	sysConfig.m_nLightControlValue[6] = comboBoxLightValue6->ItemIndex;
	sysConfig.m_nLightControlValue[7] = comboBoxLightValue7->ItemIndex;
}
//---------------------------------------------------------------------------
