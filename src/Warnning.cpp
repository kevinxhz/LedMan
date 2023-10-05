//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Warnning.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmWarnning *frmWarnning;
//---------------------------------------------------------------------------
__fastcall TfrmWarnning::TfrmWarnning(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmWarnning::FormShow(TObject *Sender)
{
	Timer1->Enabled = true;
	mediaPlayer->Play();
}
//---------------------------------------------------------------------------

void __fastcall TfrmWarnning::Timer1Timer(TObject *Sender)
{
	mediaPlayer->Play();
}
//---------------------------------------------------------------------------


