//---------------------------------------------------------------------------

#ifndef ActiveTypeH
#define ActiveTypeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmActiveType : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TComboBox *comboActiveType;
	TLabel *lbTimeBase;
	TDateTimePicker *dateCounterBy;
	TButton *btnOk;
	TButton *btnCancel;
	TLabel *lbExtDataId;
	TComboBox *cbExtDataId;
	void __fastcall comboActiveTypeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmActiveType(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmActiveType *frmActiveType;
//---------------------------------------------------------------------------
#endif
