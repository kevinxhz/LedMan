//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "ParkDigit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmParkDigit::TfrmParkDigit(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmParkDigit::btnRed1Click(TObject *Sender)
{
	TEdit* edit[8] = {editPark1, editPark2, editPark3, editPark4, editPark5, editPark6, editPark7, editPark8};
	edit[((TSpeedButton*)Sender)->Tag]->Font->Color = clRed;
}
//---------------------------------------------------------------------------
void __fastcall TfrmParkDigit::btnGreen1Click(TObject *Sender)
{
	TEdit* edit[8] = {editPark1, editPark2, editPark3, editPark4, editPark5, editPark6, editPark7, editPark8};
	edit[((TSpeedButton*)Sender)->Tag]->Font->Color = clLime;
}
//---------------------------------------------------------------------------
void __fastcall TfrmParkDigit::btnYellow1Click(TObject *Sender)
{
	TEdit* edit[8] = {editPark1, editPark2, editPark3, editPark4, editPark5, editPark6, editPark7, editPark8};
	edit[((TSpeedButton*)Sender)->Tag]->Font->Color = clYellow;
}
//---------------------------------------------------------------------------
void __fastcall TfrmParkDigit::btnBlue1Click(TObject *Sender)
{
	TEdit* edit[8] = {editPark1, editPark2, editPark3, editPark4, editPark5, editPark6, editPark7, editPark8};
	edit[((TSpeedButton*)Sender)->Tag]->Font->Color = clBlue;
}
//---------------------------------------------------------------------------

void __fastcall TfrmParkDigit::btnPink1Click(TObject *Sender)
{
	TEdit* edit[8] = {editPark1, editPark2, editPark3, editPark4, editPark5, editPark6, editPark7, editPark8};
	edit[((TSpeedButton*)Sender)->Tag]->Font->Color = clFuchsia;
}
//---------------------------------------------------------------------------

void __fastcall TfrmParkDigit::btnAqua1Click(TObject *Sender)
{
	TEdit* edit[8] = {editPark1, editPark2, editPark3, editPark4, editPark5, editPark6, editPark7, editPark8};
	edit[((TSpeedButton*)Sender)->Tag]->Font->Color = clAqua;
}
//---------------------------------------------------------------------------

void __fastcall TfrmParkDigit::btnWhite1Click(TObject *Sender)
{
	TEdit* edit[8] = {editPark1, editPark2, editPark3, editPark4, editPark5, editPark6, editPark7, editPark8};
	edit[((TSpeedButton*)Sender)->Tag]->Font->Color = clWhite;
}
//---------------------------------------------------------------------------

