//---------------------------------------------------------------------------

#ifndef SearchCardH
#define SearchCardH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmSearchCard : public TForm
{
__published:	// IDE-managed Components
	TLabel *lbCode;
	TEdit *editCode;
	TLabel *lbDescription;
	TEdit *editDesc;
	TButton *btnSearch;
	TButton *btnCancel;
	TLabel *lbNote;
	TEdit *editId;
	TLabel *lbCardId;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmSearchCard(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSearchCard *frmSearchCard;
//---------------------------------------------------------------------------
#endif
