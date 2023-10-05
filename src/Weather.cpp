//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Weather.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmWeather *frmWeather;
//---------------------------------------------------------------------------
__fastcall TfrmWeather::TfrmWeather(TComponent* Owner)
	: TForm(Owner)
{
	m_SelectedIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmWeather::Image2Click(TObject *Sender)
{
	TImage* img = (TImage*)Sender;
	m_SelectedIndex = img->Tag;

	imgSelected->Picture = img->Picture;
}
//---------------------------------------------------------------------------
