//---------------------------------------------------------------------------

#ifndef ParkDigitH
#define ParkDigitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmParkDigit : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TEdit *editPark1;
	TEdit *editPark2;
	TEdit *editPark3;
	TEdit *editPark4;
	TEdit *editPark5;
	TEdit *editPark6;
	TEdit *editPark7;
	TEdit *editPark8;
	TButton *btnOk;
	TButton *btnCancel;
	TSpeedButton *btnRed1;
	TSpeedButton *btnGreen1;
	TSpeedButton *btnYellow1;
	TSpeedButton *btnRed2;
	TSpeedButton *btnGreen2;
	TSpeedButton *btnYellow2;
	TSpeedButton *btnRed3;
	TSpeedButton *btnGreen3;
	TSpeedButton *btnYellow3;
	TSpeedButton *btnYellow4;
	TSpeedButton *btnGreen4;
	TSpeedButton *btnRed4;
	TSpeedButton *btnYellow5;
	TSpeedButton *btnGreen5;
	TSpeedButton *btnRed5;
	TSpeedButton *btnYellow6;
	TSpeedButton *btnGreen6;
	TSpeedButton *btnRed6;
	TSpeedButton *btnYellow7;
	TSpeedButton *btnGreen7;
	TSpeedButton *btnRed7;
	TCheckBox *checkDoubleColor;
	TSpeedButton *btnRed8;
	TSpeedButton *btnGreen8;
	TSpeedButton *btnYellow8;
	TComboBox *comboBoxLightValue;
	TLabel *lbBrightnessValue;
	TCheckBox *checkInit;
	TSpeedButton *btnBlue1;
	TSpeedButton *btnPink1;
	TSpeedButton *btnAqua1;
	TSpeedButton *btnWhite1;
	TSpeedButton *btnBlue2;
	TSpeedButton *btnPink2;
	TSpeedButton *btnAqua2;
	TSpeedButton *btnWhite2;
	TSpeedButton *btnBlue3;
	TSpeedButton *btnPink3;
	TSpeedButton *btnAqua3;
	TSpeedButton *btnWhite3;
	TSpeedButton *btnBlue4;
	TSpeedButton *btnPink4;
	TSpeedButton *btnAqua4;
	TSpeedButton *btnWhite4;
	TSpeedButton *btnBlue5;
	TSpeedButton *btnPink5;
	TSpeedButton *btnAqua5;
	TSpeedButton *btnWhite5;
	TSpeedButton *btnBlue6;
	TSpeedButton *btnPink6;
	TSpeedButton *btnAqua6;
	TSpeedButton *btnWhite6;
	TSpeedButton *btnBlue7;
	TSpeedButton *btnPink7;
	TSpeedButton *btnAqua7;
	TSpeedButton *btnWhite7;
	TSpeedButton *btnBlue8;
	TSpeedButton *btnPink8;
	TSpeedButton *btnAqua8;
	TSpeedButton *btnWhite8;
	void __fastcall btnRed1Click(TObject *Sender);
	void __fastcall btnGreen1Click(TObject *Sender);
	void __fastcall btnYellow1Click(TObject *Sender);
	void __fastcall btnBlue1Click(TObject *Sender);
	void __fastcall btnPink1Click(TObject *Sender);
	void __fastcall btnAqua1Click(TObject *Sender);
	void __fastcall btnWhite1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmParkDigit(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmParkDigit *frmParkDigit;
//---------------------------------------------------------------------------
#endif
