//---------------------------------------------------------------------------

#ifndef LedDigitH
#define LedDigitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmLedDigit : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *editDigit;
	TButton *btnOk;
	TButton *btnCancel;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmLedDigit(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
