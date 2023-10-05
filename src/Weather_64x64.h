//---------------------------------------------------------------------------

#ifndef Weather_64x64H
#define Weather_64x64H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <pngimage.hpp>

#define WEATHER_ICON_COUNT	30
//---------------------------------------------------------------------------
class TfrmWeather64x64 : public TForm
{
__published:	// IDE-managed Components
	TButton *btnOk;
	TButton *btnCancel;
	TImage *Image8;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox2;
	TImage *Image3;
	TCheckBox *CheckBox3;
	TImage *Image4;
	TCheckBox *CheckBox4;
	TImage *Image5;
	TCheckBox *CheckBox5;
	TImage *Image7;
	TCheckBox *CheckBox6;
	TImage *Image1;
	TCheckBox *CheckBox7;
	TImage *Image2;
	TCheckBox *CheckBox8;
	TImage *Image6;
	TCheckBox *CheckBox9;
	TImage *Image9;
	TCheckBox *CheckBox10;
	TImage *Image10;
	TCheckBox *CheckBox11;
	TImage *Image11;
	TCheckBox *CheckBox12;
	TImage *Image12;
	TCheckBox *CheckBox13;
	TImage *Image13;
	TCheckBox *CheckBox14;
	TImage *Image14;
	TCheckBox *CheckBox15;
	TImage *Image15;
	TCheckBox *CheckBox16;
	TImage *Image16;
	TCheckBox *CheckBox17;
	TImage *Image17;
	TCheckBox *CheckBox18;
	TImage *Image18;
	TCheckBox *CheckBox19;
	TImage *Image19;
	TCheckBox *CheckBox20;
	TImage *Image20;
	TCheckBox *CheckBox21;
	TImage *Image21;
	TCheckBox *CheckBox22;
	TImage *Image22;
	TCheckBox *CheckBox23;
	TImage *Image23;
	TCheckBox *CheckBox24;
	TImage *Image24;
	TCheckBox *CheckBox25;
	TImage *Image25;
	TCheckBox *CheckBox27;
	TImage *Image27;
	TCheckBox *CheckBox28;
	TImage *Image28;
	TCheckBox *CheckBox26;
	TImage *Image26;
	TCheckBox *CheckBox29;
	TImage *Image29;
	TCheckBox *CheckBox30;
	TImage *Image30;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmWeather64x64(TComponent* Owner);
	bool m_bSelected[WEATHER_ICON_COUNT];
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmWeather64x64 *frmWeather64x64;
//---------------------------------------------------------------------------
#endif
