//---------------------------------------------------------------------------

#ifndef SearchActH
#define SearchActH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmSearchAct : public TForm
{
__published:	// IDE-managed Components
	TEdit *editActDesc;
	TButton *btnSearch;
	TButton *btnCancel;
	TLabel *lbText;
	TEdit *editActText;
	TLabel *lbDescription;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmSearchAct(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSearchAct *frmSearchAct;
//---------------------------------------------------------------------------
#endif
