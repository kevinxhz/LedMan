//---------------------------------------------------------------------------

#ifndef PasswordH
#define PasswordH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmPassword : public TForm
{
__published:	// IDE-managed Components
	TImage *imgKey;
	TLabeledEdit *editOldPassword;
	TButton *btnOk;
	TButton *btnCancel;
	TLabeledEdit *editNewPassword;
	TLabeledEdit *editConfirmPassword;
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmPassword(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPassword *frmPassword;
//---------------------------------------------------------------------------
#endif
