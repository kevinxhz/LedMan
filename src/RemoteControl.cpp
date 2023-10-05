//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RemoteControl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRemoteControl *frmRemoteControl;
//---------------------------------------------------------------------------
__fastcall TfrmRemoteControl::TfrmRemoteControl(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmRemoteControl::BitBtn1Click(TObject *Sender)
{
	m_nSelect = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRemoteControl::BitBtn2Click(TObject *Sender)
{
	m_nSelect = 1;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRemoteControl::BitBtn3Click(TObject *Sender)
{
	m_nSelect = 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRemoteControl::BitBtn4Click(TObject *Sender)
{
	m_nSelect = 3;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRemoteControl::BitBtn5Click(TObject *Sender)
{
	m_nSelect = 4;
}
//---------------------------------------------------------------------------
