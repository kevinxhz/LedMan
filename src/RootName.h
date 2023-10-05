//---------------------------------------------------------------------------

#ifndef RootNameH
#define RootNameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
class TfrmMain;

//---------------------------------------------------------------------------
class TfrmRootName : public TForm
{
__published:	// IDE-managed Components
	TLabel *lbOldName;
	TLabel *lbNewName;
	TButton *btnChange;
	TButton *btnCreateNew;
	TButton *btnCancel;
	TComboBox *comboRootName;
	TButton *btnJump;
	TEdit *editRootName;
	TButton *btnDelete;
	TShape *Shape1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnChangeClick(TObject *Sender);
	void __fastcall btnCreateNewClick(TObject *Sender);
	void __fastcall btnJumpClick(TObject *Sender);
	void __fastcall btnDeleteClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmRootName(TComponent* Owner);
	TfrmMain *m_frmMain;
	int m_nResult;
	bool m_bDeleted;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRootName *frmRootName;
//---------------------------------------------------------------------------
#endif
