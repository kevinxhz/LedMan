//---------------------------------------------------------------------------

#ifndef TxLogViewH
#define TxLogViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
#include "dtu.h"

class TfrmTxLogView : public TForm
{
__published:	// IDE-managed Components
	TLabel *lbCount;
	TLabel *lbActs;
	TLabel *lbOk;
	TListBox *lstAct;
	TListBox *lstCardOk;
	TListBox *lstCardFail;
	TButton *btnPrev;
	TButton *btnNext;
	TLabel *lbError;
	TLabel *lbTime;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnPrevClick(TObject *Sender);
	void __fastcall btnNextClick(TObject *Sender);
private:	// User declarations
	CDtu* m_dtuRoot;
public:		// User declarations
	__fastcall TfrmTxLogView(TComponent* Owner, CDtu* dtuRoot);
	void InitView();
	int m_nLogIndex;
	int m_nLogTxCount;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmTxLogView *frmTxLogView;
//---------------------------------------------------------------------------
#endif
