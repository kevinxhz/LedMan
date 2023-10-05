//---------------------------------------------------------------------------

#ifndef CardTypeH
#define CardTypeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmCardType : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TListBox *lstCardType;
	TMemo *memoNote;
	TButton *btnOk;
	TButton *btnCancel;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall lstCardTypeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmCardType(TComponent* Owner, int nCardType);

	int m_nCardType;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCardType *frmCardType;
//---------------------------------------------------------------------------
#endif
