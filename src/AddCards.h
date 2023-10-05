//---------------------------------------------------------------------------

#ifndef AddCardsH
#define AddCardsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmAddCards : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *editCount;
	TButton *btnOk;
	TButton *btnCancel;
	void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmAddCards(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAddCards *frmAddCards;
//---------------------------------------------------------------------------
#endif
