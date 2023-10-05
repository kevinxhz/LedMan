//---------------------------------------------------------------------------

#ifndef WarnningH
#define WarnningH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <MPlayer.hpp>
//---------------------------------------------------------------------------
class TfrmWarnning : public TForm
{
__published:	// IDE-managed Components
	TLabel *lbName;
	TLabel *Label2;
	TButton *btnClose;
	TMediaPlayer *mediaPlayer;
	TLabel *lbTime;
	TTimer *Timer1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmWarnning(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmWarnning *frmWarnning;
//---------------------------------------------------------------------------
#endif
