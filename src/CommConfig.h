//---------------------------------------------------------------------------

#ifndef CommConfigH
#define CommConfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmCommConfig : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *groupMethod;
	TRadioButton *rbtnSerialPort;
	TRadioButton *rbtnGPRS;
	TGroupBox *groupNetwork;
	TLabel *lbIpAddr;
	TLabel *lbTimeout_Network;
	TLabel *lbUserName;
	TLabel *lbPassword;
	TLabel *lbRetry_Network;
	TLabel *lbPort;
	TEdit *editServer;
	TEdit *editTimeout_Network;
	TEdit *editUserName;
	TEdit *editPassword;
	TEdit *editSocketPort_Server;
	TEdit *editRetryCount_Network;
	TGroupBox *groupSerialPort;
	TLabel *lbSerialPort;
	TLabel *lbBaudRate;
	TLabel *lbTimeout_SerialPort;
	TLabel *lbRetry_SerialPort;
	TComboBox *comboBoxComPort;
	TEdit *editTimeout_SerialPort;
	TEdit *editRetryCount_SerialPort;
	TComboBox *cbBaudrate;
	TButton *btnCancel;
	TButton *btnOk;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmCommConfig(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCommConfig *frmCommConfig;
//---------------------------------------------------------------------------
#endif
