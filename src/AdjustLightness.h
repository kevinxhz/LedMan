//---------------------------------------------------------------------------

#ifndef AdjustLightnessH
#define AdjustLightnessH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmAdjustLightness : public TForm
{
__published:	// IDE-managed Components
	TComboBox *comboBoxLightValue0;
	TLabel *lbBrightnessValue;
	TDateTimePicker *timeStart0;
	TLabel *lbBrightnessStartTime;
	TButton *btnOk;
	TButton *btnCancel;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmAdjustLightness(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
