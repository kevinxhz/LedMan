//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Weather_16x16_C1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmWeather_16x16_C1 *frmWeather_16x16_C1;
//---------------------------------------------------------------------------
__fastcall TfrmWeather_16x16_C1::TfrmWeather_16x16_C1(TComponent* Owner)
	: TForm(Owner)
{
	m_nType = 0;
	m_nObjectIndex = 0;
	m_Image = Image0;
	m_sText = "±©Ñ©";
}
//---------------------------------------------------------------------------
void __fastcall TfrmWeather_16x16_C1::radioType0Click(TObject *Sender)
{
	m_nType = ((TRadioButton*)Sender)->Tag;
	m_sText = ((TRadioButton*)Sender)->Caption;
	switch (m_nType)
	{
		case 0:	m_Image = Image0; break;
		case 1:	m_Image = Image1; break;
		case 2:	m_Image = Image2; break;
		case 3:	m_Image = Image3; break;
		case 4:	m_Image = Image4; break;
		case 5:	m_Image = Image5; break;
		case 6:	m_Image = Image6; break;
		case 7:	m_Image = Image7; break;
		case 8:	m_Image = Image8; break;
		case 9:	m_Image = Image9; break;
		case 10: m_Image = Image10; break;
		case 11: m_Image = Image11; break;
		case 12: m_Image = Image12; break;
		case 13: m_Image = Image13; break;
		case 14: m_Image = Image14; break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmWeather_16x16_C1::rdObject0Click(TObject *Sender)
{
	m_nObjectIndex = ((TRadioButton*)Sender)->Tag;
}
//---------------------------------------------------------------------------
