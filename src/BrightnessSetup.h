//---------------------------------------------------------------------------

#ifndef BrightnessSetupH
#define BrightnessSetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmBrightnessSetup : public TForm
{
__published:	// IDE-managed Components
	TLabel *lbLight0;
	TLabel *lbBrightnessStartTime;
	TLabel *lbBrightnessValue;
	TLabel *lbLight1;
	TLabel *lbLight2;
	TLabel *lbLight3;
	TLabel *lbLight4;
	TLabel *lbLight7;
	TLabel *lbLight5;
	TLabel *lbLight6;
	TDateTimePicker *timeStart0;
	TDateTimePicker *timeStart1;
	TDateTimePicker *timeStart2;
	TDateTimePicker *timeStart3;
	TDateTimePicker *timeStart4;
	TDateTimePicker *timeStart5;
	TDateTimePicker *timeStart6;
	TDateTimePicker *timeStart7;
	TComboBox *comboBoxLightValue0;
	TComboBox *comboBoxLightValue1;
	TComboBox *comboBoxLightValue2;
	TComboBox *comboBoxLightValue3;
	TComboBox *comboBoxLightValue4;
	TComboBox *comboBoxLightValue5;
	TComboBox *comboBoxLightValue6;
	TComboBox *comboBoxLightValue7;
	TButton *btnOk;
	TButton *btnCancel;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmBrightnessSetup(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
