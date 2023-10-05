//---------------------------------------------------------------------------

#ifndef SelectFontH
#define SelectFontH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmFontSelect : public TForm
{
__published:	// IDE-managed Components
	TRadioButton *rbtn16;
	TRadioButton *rbtn24;
	TRadioButton *rbtn32;
	TRadioButton *rbtnWindows;
	TButton *btnOk;
	TButton *btnCancel;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmFontSelect(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmFontSelect *frmFontSelect;
//---------------------------------------------------------------------------
#endif
