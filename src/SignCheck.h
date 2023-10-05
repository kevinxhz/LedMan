//---------------------------------------------------------------------------

#ifndef SignCheckH
#define SignCheckH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#define CHECK_RESULT_OE_BIT_1	0x00000001
#define CHECK_RESULT_CK_BIT_1	0x00000002
#define CHECK_RESULT_ST_BIT_1	0x00000004
#define CHECK_RESULT_A_BIT_1	0x00000008
#define CHECK_RESULT_B_BIT_1	0x00000010
#define CHECK_RESULT_R1_BIT_1	0x00000020
#define CHECK_RESULT_R2_BIT_1	0x00000040
#define CHECK_RESULT_G1_BIT_1	0x00000080
#define CHECK_RESULT_G2_BIT_1	0x00000100
#define CHECK_RESULT_C_BIT_1	0x00000200
#define CHECK_RESULT_D_BIT_1	0x00000400

#define CHECK_RESULT_OE_BIT_2	0x00010000
#define CHECK_RESULT_CK_BIT_2	0x00020000
#define CHECK_RESULT_ST_BIT_2	0x00040000
#define CHECK_RESULT_A_BIT_2	0x00080000
#define CHECK_RESULT_B_BIT_2	0x00100000
#define CHECK_RESULT_R1_BIT_2	0x00200000
#define CHECK_RESULT_R2_BIT_2	0x00400000
#define CHECK_RESULT_G1_BIT_2	0x00800000
#define CHECK_RESULT_G2_BIT_2	0x01000000
#define CHECK_RESULT_C_BIT_2	0x02000000
#define CHECK_RESULT_D_BIT_2	0x04000000

//---------------------------------------------------------------------------
class TfrmCheckResult : public TForm
{
__published:	// IDE-managed Components
	TListBox *lstCheck;
	TButton *btnCancel;
	TButton *btnDelete;
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnDeleteClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmCheckResult(TComponent* Owner);
	void AddItem(UnicodeString sName, DWORD ulData);
	void SaveToFile();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCheckResult *frmCheckResult;
//---------------------------------------------------------------------------
#endif
