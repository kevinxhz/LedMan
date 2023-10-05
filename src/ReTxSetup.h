//---------------------------------------------------------------------------

#ifndef ReTxSetupH
#define ReTxSetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmReTxSetup : public TForm
{
__published:	// IDE-managed Components
	TButton *btnOk;
	TButton *btnCancel;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *editAutoRetryTime;
	TCheckBox *checkOnlyLogLastTx;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmReTxSetup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmReTxSetup *frmReTxSetup;
//---------------------------------------------------------------------------
#endif
