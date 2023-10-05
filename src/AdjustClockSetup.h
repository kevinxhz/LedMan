//---------------------------------------------------------------------------

#ifndef AdjustClockSetupH
#define AdjustClockSetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmAdjustClockSetup : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *editAutoClockInerval;
	TLabel *Label2;
	TEdit *editAutoClockMinute;
	TLabel *Label3;
	TButton *btnOk;
	TButton *btnCancel;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmAdjustClockSetup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAdjustClockSetup *frmAdjustClockSetup;
//---------------------------------------------------------------------------
#endif
