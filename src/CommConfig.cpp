//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CommConfig.h"
#include "SysConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmCommConfig *frmCommConfig;
//---------------------------------------------------------------------------
__fastcall TfrmCommConfig::TfrmCommConfig(TComponent* Owner)
	: TForm(Owner)
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		groupMethod->Caption = "通信方式选择";
		rbtnSerialPort->Caption = "串口（RS232/RS485）";
		rbtnGPRS->Caption = "3G/GPRS/CDMA/WIFI/网络转发";
		lbSerialPort->Caption = "计算机串口";
		lbBaudRate->Caption = "通信速率";
		lbTimeout_SerialPort->Caption = "超时时间(毫秒)";
		lbRetry_SerialPort->Caption = "重试次数";
		groupNetwork->Caption = "3G/GPRS/CDMA/WIFI/网络转发";
		lbIpAddr->Caption = "服务器IP地址或域名";
		lbPort->Caption = "端口号";
		lbTimeout_Network->Caption = "超时时间(秒)";
		lbRetry_Network->Caption = "重试次数";
		lbUserName->Caption = "用户名";
		lbPassword->Caption = "密码";
		btnOk->Caption = "确定";
		btnCancel->Caption = "取消";
	}
	else
	{
		groupMethod->Caption = "Communication Method Select";
		rbtnSerialPort->Caption = "Serial Port(RS232/RS485)";
		rbtnGPRS->Caption = "3G/GPRS/CDMA/WIFI/NetWork Exchange";
		lbSerialPort->Caption = "Serial Port";
		lbBaudRate->Caption = "Communication Speed";
		lbTimeout_SerialPort->Caption = "Timeout(ms)";
		lbRetry_SerialPort->Caption = "Retries";
		groupNetwork->Caption = "3G/GPRS/CDMA/WIFI/NetWork Exchange Server";
		lbIpAddr->Caption = "IP Address or Domain Name";
		lbPort->Caption = "Socket Port";
		lbTimeout_Network->Caption = "Timeout(s)";
		lbRetry_Network->Caption = "Retries";
		lbUserName->Caption = "User Name";
		lbPassword->Caption = "Password";
		btnOk->Caption = "OK";
		btnCancel->Caption = "Cancel";
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCommConfig::FormCreate(TObject *Sender)
{
	wchar_t szName[256];
	BYTE ucValue[256];

	TRegistry *reg = new TRegistry();
	reg->RootKey = HKEY_LOCAL_MACHINE;
	if (reg->OpenKeyReadOnly(L"HARDWARE\\DEVICEMAP\\SERIALCOMM"))
	{
		DWORD dwIndex = 0;
		DWORD dwNameLen = 256;
		DWORD dwValueLen = 256;
		while (RegEnumValue(reg->CurrentKey, dwIndex++, szName, &dwNameLen, NULL, NULL, ucValue, &dwValueLen) == ERROR_SUCCESS)
		{
			comboBoxComPort->Items->Add((wchar_t*)ucValue);
			dwNameLen = 256;
			dwValueLen = 256;
		}
	}

	delete reg;

	if (sysConfig.m_nCommMethod)
		rbtnGPRS->Checked = true;
	else
		rbtnSerialPort->Checked = true;

	comboBoxComPort->Text = sysConfig.m_sComPort;
	switch (sysConfig.m_nBaudRate)
	{
	case 57600:
		cbBaudrate->ItemIndex = 1;
		break;
	case 38400:
		cbBaudrate->ItemIndex = 2;
		break;
	case 19200:
		cbBaudrate->ItemIndex = 3;
		break;
	case 9600:
		cbBaudrate->ItemIndex = 4;
		break;
	case 4800:
		cbBaudrate->ItemIndex = 5;
		break;
	case 2400:
		cbBaudrate->ItemIndex = 6;
		break;
	default:
		cbBaudrate->ItemIndex = 0;
		break;
	}
	editTimeout_SerialPort->Text = sysConfig.m_nTimeout_SerialPort;
	editRetryCount_SerialPort->Text = sysConfig.m_nRetry_SerialPort;
	editServer->Text = 	sysConfig.m_sServer;
	editSocketPort_Server->Text = sysConfig.m_nSocketPort;
	editRetryCount_Network->Text = sysConfig.m_nRetry_Network;
	editTimeout_Network->Text = sysConfig.m_nTimeout_Network;
	editUserName->Text = sysConfig.m_sUserName;
	editPassword->Text = sysConfig.m_sPassword;
#ifdef FIXED_IP
	editServer->Enabled = false;
	editSocketPort_Server->Enabled = false;
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfrmCommConfig::btnOkClick(TObject *Sender)
{
	sysConfig.m_nCommMethod = (rbtnSerialPort->Checked) ? 0: 1;
	sysConfig.m_sComPort = comboBoxComPort->Text;
	sysConfig.m_nBaudRate = cbBaudrate->Items->Strings[cbBaudrate->ItemIndex].ToInt();
	sysConfig.m_nTimeout_SerialPort = editTimeout_SerialPort->Text.ToInt();
	sysConfig.m_nRetry_SerialPort = editRetryCount_SerialPort->Text.ToInt();
	sysConfig.m_sServer = editServer->Text;
	sysConfig.m_nSocketPort = editSocketPort_Server->Text.ToInt();
	sysConfig.m_nTimeout_Network = editTimeout_Network->Text.ToInt();
	sysConfig.m_nRetry_Network = editRetryCount_Network->Text.ToInt();
	sysConfig.m_sUserName = editUserName->Text;
	sysConfig.m_sPassword = editPassword->Text;
	sysConfig.SaveConfig();
}
//---------------------------------------------------------------------------
