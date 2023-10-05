//---------------------------------------------------------------------------

#ifndef ActDefineH
#define ActDefineH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmActDefine : public TForm
{
__published:	// IDE-managed Components
	TRadioButton *rdAll;
	TRadioButton *rdGroup;
	TButton *btnOk;
	TButton *btnCancel;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmActDefine(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmActDefine *frmActDefine;
//---------------------------------------------------------------------------
#endif
