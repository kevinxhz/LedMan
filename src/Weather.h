//---------------------------------------------------------------------------

#ifndef WeatherH
#define WeatherH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TfrmWeather : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TImage *Image2;
	TImage *Image3;
	TImage *Image4;
	TImage *Image6;
	TImage *Image8;
	TImage *Image9;
	TImage *Image10;
	TImage *Image11;
	TImage *Image12;
	TImage *Image13;
	TImage *Image15;
	TImage *Image16;
	TImage *Image17;
	TImage *Image18;
	TImage *Image19;
	TImage *Image20;
	TImage *Image21;
	TImage *Image22;
	TImage *Image23;
	TImage *Image24;
	TImage *Image5;
	TImage *Image26;
	TImage *Image27;
	TImage *Image28;
	TImage *Image30;
	TImage *Image35;
	TImage *Image36;
	TImage *Image37;
	TImage *Image38;
	TImage *Image39;
	TImage *Image40;
	TImage *Image42;
	TImage *Image43;
	TImage *Image46;
	TImage *Image47;
	TImage *Image48;
	TImage *Image50;
	TImage *Image51;
	TImage *Image53;
	TImage *Image54;
	TImage *Image55;
	TImage *Image56;
	TImage *Image57;
	TImage *Image58;
	TImage *imgSelected;
	TLabel *Label1;
	TButton *btnOk;
	TButton *btnCancel;
	void __fastcall Image2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmWeather(TComponent* Owner);
	int m_SelectedIndex;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmWeather *frmWeather;
//---------------------------------------------------------------------------
#endif
