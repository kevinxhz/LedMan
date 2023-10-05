//---------------------------------------------------------------------------

#ifndef Weather_16x16_C1H
#define Weather_16x16_C1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TfrmWeather_16x16_C1 : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TRadioButton *radioType0;
	TImage *Image0;
	TRadioButton *radioType1;
	TImage *Image1;
	TImage *Image2;
	TRadioButton *radioType2;
	TImage *Image3;
	TRadioButton *radioType3;
	TRadioButton *radioType4;
	TImage *Image4;
	TRadioButton *radioType5;
	TImage *Image5;
	TRadioButton *radioType6;
	TImage *Image6;
	TRadioButton *radioType8;
	TImage *Image8;
	TRadioButton *radioType9;
	TImage *Image9;
	TRadioButton *radioType10;
	TImage *Image10;
	TRadioButton *radioType11;
	TImage *Image11;
	TRadioButton *radioType12;
	TImage *Image12;
	TRadioButton *radioType13;
	TImage *Image13;
	TRadioButton *radioType7;
	TImage *Image7;
	TButton *btnOk;
	TRadioButton *radioType14;
	TButton *btnCancel;
	TGroupBox *GroupBox2;
	TRadioButton *rdObject0;
	TRadioButton *rdObject1;
	TRadioButton *rdObject2;
	TRadioButton *rdObject3;
	TImage *Image14;
	TImage *Image16;
	TImage *Image17;
	TImage *Image18;
	TImage *Image15;
	void __fastcall radioType0Click(TObject *Sender);
	void __fastcall rdObject0Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmWeather_16x16_C1(TComponent* Owner);
	int m_nType;
	int m_nObjectIndex;
	TImage* m_Image;
	UnicodeString m_sText;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmWeather_16x16_C1 *frmWeather_16x16_C1;
//---------------------------------------------------------------------------
#endif
