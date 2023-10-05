//---------------------------------------------------------------------------

#ifndef FlashTestH
#define FlashTestH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmFlashTest : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TButton *btnOk;
	TButton *btnCancel;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmFlashTest(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmFlashTest *frmFlashTest;
//---------------------------------------------------------------------------
#endif
