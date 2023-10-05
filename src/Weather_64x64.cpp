//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Weather_64x64.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmWeather64x64 *frmWeather64x64;
//---------------------------------------------------------------------------
__fastcall TfrmWeather64x64::TfrmWeather64x64(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmWeather64x64::FormCreate(TObject *Sender)
{
	for (int i = 0; i < WEATHER_ICON_COUNT; i++)
		m_bSelected[i] = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWeather64x64::btnOkClick(TObject *Sender)
{
	for (int i = 1; i < ControlCount; i++)
	{
		if (Controls[i]->ClassNameIs("TCheckBox"))
		{
			TCheckBox *p = dynamic_cast<TCheckBox*>(Controls[i]);
			m_bSelected[p->Tag] = p->Checked;
		}
	}
}
//---------------------------------------------------------------------------

