//---------------------------------------------------------------------------

#ifndef AboutH
#define AboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmAbout : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TButton *btnOk;
	TLabel *Label7;
	TEdit *editCardType;
	TButton *Button1;
	TImage *Image1;
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
	int OutputByte(int hFile, BYTE byte, int nCount);
public:		// User declarations
	__fastcall TfrmAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
#endif
