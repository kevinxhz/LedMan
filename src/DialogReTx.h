//---------------------------------------------------------------------------

#ifndef DialogReTxH
#define DialogReTxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmAutoReTx : public TForm
{
__published:	// IDE-managed Components
	TLabel *lbNote1;
	TImage *Image1;
	TLabel *lbNote3;
	TButton *btnOk;
	TButton *btnCancel;
	TButton *btnNo;
	TLabel *lbTime;
	TLabel *lbNote2;
	TTimer *timerWait;
	void __fastcall timerWaitTimer(TObject *Sender);
private:	// User declarations
	int m_nWaitTime;
public:		// User declarations
	__fastcall TfrmAutoReTx(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAutoReTx *frmAutoReTx;
//---------------------------------------------------------------------------
#endif
