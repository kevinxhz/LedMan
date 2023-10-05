//---------------------------------------------------------------------------

#ifndef RemoteControlH
#define RemoteControlH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmRemoteControl : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TBitBtn *BitBtn3;
	TBitBtn *BitBtn4;
	TBitBtn *BitBtn5;
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall BitBtn3Click(TObject *Sender);
	void __fastcall BitBtn4Click(TObject *Sender);
	void __fastcall BitBtn5Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmRemoteControl(TComponent* Owner);
	int m_nSelect;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRemoteControl *frmRemoteControl;
//---------------------------------------------------------------------------
#endif
