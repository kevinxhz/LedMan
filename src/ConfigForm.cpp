//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "global.h"
#include "ConfigForm.h"
#include <inifiles.hpp>
#include <registry.hpp>
#include "SysConfig.h"
#include "TextEditForm.h"
#include "CustMsgDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmConfig::TfrmConfig(TComponent* Owner, CDtu* dtu)
	: TForm(Owner)
{
	m_dtu = dtu;
	m_nCardId = dtu->GetCardId();
	editCardId->Text = m_nCardId;
	editNewCardId->Text = m_nCardId;

	checkTCP->Visible = (sysConfig.m_nCardType == CARD_TYPE_GS9020 ||
						 sysConfig.m_nCardType == CARD_TYPE_GS7020B  ||
						 sysConfig.m_nCardType == CARD_TYPE_GS7020H  ||
						 sysConfig.m_nCardType == CARD_TYPE_GS2012A_S  ||
						 sysConfig.m_nCardType == CARD_TYPE_GS9025  ||
						 sysConfig.m_nCardType == CARD_TYPE_GS9023  ||
						 sysConfig.m_nCardType == CARD_TYPE_GS2012A
						);
	checkLoginEcho->Visible = false;//(sysConfig.m_nCardType == CARD_TYPE_GS7020B);

	sheetComPort->TabVisible = (sysConfig.m_nCardType == CARD_TYPE_YL2010A ||
								sysConfig.m_nCardType == CARD_TYPE_YL2010C ||
								sysConfig.m_nCardType == CARD_TYPE_YL2010AR ||
								sysConfig.m_nCardType == CARD_TYPE_YL2010AF ||
								sysConfig.m_nCardType == CARD_TYPE_YL2010A_GPS_JTX ||
								sysConfig.m_nCardType == CARD_TYPE_PD102B   ||
								sysConfig.m_nCardType == CARD_TYPE_GS7020B
								);
	if (sheetComPort->TabVisible)
	{
		if (sysConfig.m_nCardType == CARD_TYPE_GS7020B)
		{
			lbCurrentCardId->Visible = false;
			lbNewCardId->Visible = false;
			btnSearchCard->Visible = false;
			editCardId->Visible = false;
			editNewCardId->Visible = false;
			lbBaudRate->Top = lbCurrentCardId->Top;
			cbBaudrate->Top = editCardId->Top;
		}
	}
	sheetSms->TabVisible = (sysConfig.m_nCardType == CARD_TYPE_GS7022B ||
							sysConfig.m_nCardType == CARD_TYPE_GS2012A_S ||
							sysConfig.m_nCardType == CARD_TYPE_GS9020S ||
							sysConfig.m_nCardType == CARD_TYPE_GS7020HS ||
							sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64
							);

	sheetWifi->TabVisible = (sysConfig.m_nCardType == CARD_TYPE_WF2012B);
	sheetGPRS->TabVisible = (sysConfig.m_nCardType != CARD_TYPE_WF2012B &&
							 sysConfig.m_nCardType != CARD_TYPE_GS2012AS_64 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9020S &&
							 sysConfig.m_nCardType != CARD_TYPE_GS7020HS
							);
	sheetGps->TabVisible = false;//(sysConfig.m_nCardType == CARD_TYPE_GS9020F_GPS);
	sheetEthernet->TabVisible = (sysConfig.m_nCardType == CARD_TYPE_SN2P);

	if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
	{
		sheetSubArea->TabVisible = false;
		sheetAutoLightControl->TabVisible = false;
		sheetGPRS->TabVisible = false;
		lbActCount->Enabled = false;
		comboBoxMaxActCount->Enabled = false;
		rbtnFullColor->Enabled = false;
		checkActSecond->Enabled = false;
		checkActSupportRepeat->Enabled = false;
		checkSupportInsertPlay->Enabled = false;
		checkSupportWarnning->Enabled = false;
		checkSyncPlay->Enabled = false;
		checkSyncTimeHint->Enabled = false;
		checkSupportLedSignCheck->Enabled = false;
	}

	checkActSecond->Visible = (sysConfig.m_nCardType != CARD_TYPE_GS2012AS_64 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9020S &&
							 sysConfig.m_nCardType != CARD_TYPE_GS7020HS &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9025 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9023
							);
	checkActSupportRepeat->Visible = (sysConfig.m_nCardType != CARD_TYPE_GS2012AS_64 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9020S &&
							 sysConfig.m_nCardType != CARD_TYPE_GS7020HS &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9025 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9023
							);
	checkSupportInsertPlay->Visible = (sysConfig.m_nCardType != CARD_TYPE_GS2012AS_64 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9020S &&
							 sysConfig.m_nCardType != CARD_TYPE_GS7020HS &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9025 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9023
							);
	checkSupportWarnning->Visible = (sysConfig.m_nCardType != CARD_TYPE_GS2012AS_64 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9020S &&
							 sysConfig.m_nCardType != CARD_TYPE_GS7020HS &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9025 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9023
							);
	checkSyncPlay->Visible = (sysConfig.m_nCardType != CARD_TYPE_GS2012AS_64 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9020S &&
							 sysConfig.m_nCardType != CARD_TYPE_GS7020HS &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9025 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9023
							);
	editScreenHeight->Enabled = (sysConfig.m_nCardType != CARD_TYPE_GS2012AS_64 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9025 &&
							 sysConfig.m_nCardType != CARD_TYPE_GS9023
							);
	if (sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64)
	{
		checkByteReverse->Top = checkActSecond->Top;
		editScreenHeight->Text = "64";
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
	{
		checkByteReverse->Top = checkActSecond->Top;
		editScreenHeight->Text = "32";
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9023)
	{
		checkByteReverse->Top = checkActSecond->Top;
		editScreenHeight->Text = "16";
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9020S)
		checkByteReverse->Top = checkActSecond->Top;
	else if (sysConfig.m_nCardType == CARD_TYPE_GS7020HS)
		checkByteReverse->Top = checkActSecond->Top;

	lbVoiceArea->Visible = (sysConfig.m_nCardType == CARD_TYPE_GS2012A_S);
	editVoiceArea->Visible = (sysConfig.m_nCardType == CARD_TYPE_GS2012A_S);

	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "控制卡配置 - " + dtu->m_sName;
		sheetGPRS->Caption = "GPRS/CDMA/3G通信";
		lbCode->Caption = "设备编码";
		lbServer->Caption = "服务器IP地址或域名";
		lbSocket->Caption = "端口号";
		lbActCount->Caption = "控制卡节目容量";
		sheetSubArea->Caption = "分区管理";
		lbAreaCount->Caption = "有效分区个数";
		lbAreaWidth->Caption = "宽度";
		lbAreaHeight->Caption = "高度";
		sheetLedPanel->Caption = "显示屏(控制卡)";
		lbPanelWidth->Caption = "宽度";
		lbPanelHeight->Caption = "高度";
		lbScanMode->Caption = "扫描方式";
		checkOE->Caption = "OE反相";
		checkData->Caption = "数据反相";
		checkActSupportRepeat->Caption = "控制卡支持单条节目重复播放";
		checkSupportInsertPlay->Caption = "将最后6条节目做为插播信息使用";
		checkSyncPlay->Caption = "多屏同步播放";
		checkByteReverse->Caption = "数据字节逆序";
		rbtnSingleColor->Caption = "单色";
		rbtnDoubleColor->Caption = "双色";
		if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF) || (sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS7020BF_W) || (sysConfig.m_nCardType == CARD_TYPE_GS9026) || (sysConfig.m_nCardType == CARD_TYPE_GS9020F_GPS) || (sysConfig.m_nCardType == CARD_TYPE_GS7020HFA))
			checkSupportWarnning->Caption = "延时打开屏幕显示";
		else
			checkSupportWarnning->Caption = "允许开关量报警";
		checkActSecond->Caption = "播放时段精确到秒";
		btnReadConfig->Caption = "读取控制卡配置";
		btnWriteConfig->Caption = "写入控制卡配置";
		btnOk->Caption = "确定";
		btnCancel->Caption = "取消";
		sheetAutoLightControl->Caption = "自动亮度调节";
		lbBrightnessStartTime->Caption = "开始时间";
		lbBrightnessValue->Caption = "亮度值";

		comboBoxLightValue0->Items->Strings[0] = "0(关屏)";
		comboBoxLightValue0->Items->Strings[1] = "1(最暗)";
		comboBoxLightValue0->Items->Strings[2] = "2(较暗)";
		comboBoxLightValue0->Items->Strings[3] = "3(稍暗)";
		comboBoxLightValue0->Items->Strings[4] = "4(中度)";
		comboBoxLightValue0->Items->Strings[5] = "5(稍亮)";
		comboBoxLightValue0->Items->Strings[6] = "6(较亮)";
		comboBoxLightValue0->Items->Strings[7] = "7(最亮)";

		comboBoxLightValue1->Items->Strings[0] = "0(关屏)";
		comboBoxLightValue1->Items->Strings[1] = "1(最暗)";
		comboBoxLightValue1->Items->Strings[2] = "2(较暗)";
		comboBoxLightValue1->Items->Strings[3] = "3(稍暗)";
		comboBoxLightValue1->Items->Strings[4] = "4(中度)";
		comboBoxLightValue1->Items->Strings[5] = "5(稍亮)";
		comboBoxLightValue1->Items->Strings[6] = "6(较亮)";
		comboBoxLightValue1->Items->Strings[7] = "7(最亮)";

		comboBoxLightValue2->Items->Strings[0] = "0(关屏)";
		comboBoxLightValue2->Items->Strings[1] = "1(最暗)";
		comboBoxLightValue2->Items->Strings[2] = "2(较暗)";
		comboBoxLightValue2->Items->Strings[3] = "3(稍暗)";
		comboBoxLightValue2->Items->Strings[4] = "4(中度)";
		comboBoxLightValue2->Items->Strings[5] = "5(稍亮)";
		comboBoxLightValue2->Items->Strings[6] = "6(较亮)";
		comboBoxLightValue2->Items->Strings[7] = "7(最亮)";

		comboBoxLightValue3->Items->Strings[0] = "0(关屏)";
		comboBoxLightValue3->Items->Strings[1] = "1(最暗)";
		comboBoxLightValue3->Items->Strings[2] = "2(较暗)";
		comboBoxLightValue3->Items->Strings[3] = "3(稍暗)";
		comboBoxLightValue3->Items->Strings[4] = "4(中度)";
		comboBoxLightValue3->Items->Strings[5] = "5(稍亮)";
		comboBoxLightValue3->Items->Strings[6] = "6(较亮)";
		comboBoxLightValue3->Items->Strings[7] = "7(最亮)";

		comboBoxLightValue4->Items->Strings[0] = "0(关屏)";
		comboBoxLightValue4->Items->Strings[1] = "1(最暗)";
		comboBoxLightValue4->Items->Strings[2] = "2(较暗)";
		comboBoxLightValue4->Items->Strings[3] = "3(稍暗)";
		comboBoxLightValue4->Items->Strings[4] = "4(中度)";
		comboBoxLightValue4->Items->Strings[5] = "5(稍亮)";
		comboBoxLightValue4->Items->Strings[6] = "6(较亮)";
		comboBoxLightValue4->Items->Strings[7] = "7(最亮)";

		comboBoxLightValue5->Items->Strings[0] = "0(关屏)";
		comboBoxLightValue5->Items->Strings[1] = "1(最暗)";
		comboBoxLightValue5->Items->Strings[2] = "2(较暗)";
		comboBoxLightValue5->Items->Strings[3] = "3(稍暗)";
		comboBoxLightValue5->Items->Strings[4] = "4(中度)";
		comboBoxLightValue5->Items->Strings[5] = "5(稍亮)";
		comboBoxLightValue5->Items->Strings[6] = "6(较亮)";
		comboBoxLightValue5->Items->Strings[7] = "7(最亮)";

		comboBoxLightValue6->Items->Strings[0] = "0(关屏)";
		comboBoxLightValue6->Items->Strings[1] = "1(最暗)";
		comboBoxLightValue6->Items->Strings[2] = "2(较暗)";
		comboBoxLightValue6->Items->Strings[3] = "3(稍暗)";
		comboBoxLightValue6->Items->Strings[4] = "4(中度)";
		comboBoxLightValue6->Items->Strings[5] = "5(稍亮)";
		comboBoxLightValue6->Items->Strings[6] = "6(较亮)";
		comboBoxLightValue6->Items->Strings[7] = "7(最亮)";

		comboBoxLightValue7->Items->Strings[0] = "0(关屏)";
		comboBoxLightValue7->Items->Strings[1] = "1(最暗)";
		comboBoxLightValue7->Items->Strings[2] = "2(较暗)";
		comboBoxLightValue7->Items->Strings[3] = "3(稍暗)";
		comboBoxLightValue7->Items->Strings[4] = "4(中度)";
		comboBoxLightValue7->Items->Strings[5] = "5(稍亮)";
		comboBoxLightValue7->Items->Strings[6] = "6(较亮)";
		comboBoxLightValue7->Items->Strings[7] = "7(最亮)";

		sheetComPort->Caption = "串口通信";
	}
	else
	{
		Caption = "Configuration of Card - " + dtu->m_sName;
		sheetGPRS->Caption = "GPRS/CDMA/3G Communication";
		lbCode->Caption = "Code of The Card";
		lbServer->Caption = "Server's IP Address or Domain Name";
		lbSocket->Caption = "Socket Port";
		lbActCount->Caption = "Maximum Count of Acts In The Card";
		sheetSubArea->Caption = "Area Division";
		lbAreaCount->Caption = "Valid Area Count";
		lbAreaWidth->Caption = "Width";
		lbAreaHeight->Caption = "Height";
		sheetLedPanel->Caption = "LED Panel/Card";
		lbPanelWidth->Caption = "Width";
		lbPanelHeight->Caption = "Height";
		lbScanMode->Caption = "Scan Mode";
		checkOE->Caption = "Negative OE";
		checkData->Caption = "Negative Data";
		checkActSupportRepeat->Caption = "Support Repeatedly playing";
		checkSupportInsertPlay->Caption = "Use the Last 6 Acts for Emergent Information";
		checkSyncPlay->Caption = "All LED Signs Playing Same Act Synchronously";
		checkByteReverse->Caption = "Reverse Bits in Data Byte";
		rbtnSingleColor->Caption = "Monochrome";
		rbtnDoubleColor->Caption = "Double Color";
		checkSupportWarnning->Caption = "Support Warnning";
		checkActSecond->Caption = "Period Precision:Second";
		btnReadConfig->Caption = "Read From Card";
		btnWriteConfig->Caption = "Write To Card";
		btnOk->Caption = "OK";
		btnCancel->Caption = "Cancel";
		sheetAutoLightControl->Caption = "Automatic Brightness Control";
		lbBrightnessStartTime->Caption = "Start Time";
		lbBrightnessValue->Caption = "Brightness";

		comboBoxLightValue0->Items->Strings[0] = "0 Dark";
		comboBoxLightValue0->Items->Strings[1] = "1";
		comboBoxLightValue0->Items->Strings[2] = "2";
		comboBoxLightValue0->Items->Strings[3] = "3";
		comboBoxLightValue0->Items->Strings[4] = "4";
		comboBoxLightValue0->Items->Strings[5] = "5";
		comboBoxLightValue0->Items->Strings[6] = "6";
		comboBoxLightValue0->Items->Strings[7] = "7 Brightest";

		comboBoxLightValue1->Items->Strings[0] = "0 Dark";
		comboBoxLightValue1->Items->Strings[1] = "1";
		comboBoxLightValue1->Items->Strings[2] = "2";
		comboBoxLightValue1->Items->Strings[3] = "3";
		comboBoxLightValue1->Items->Strings[4] = "4";
		comboBoxLightValue1->Items->Strings[5] = "5";
		comboBoxLightValue1->Items->Strings[6] = "6";
		comboBoxLightValue1->Items->Strings[7] = "7 Brightest";

		comboBoxLightValue2->Items->Strings[0] = "0 Dark";
		comboBoxLightValue2->Items->Strings[1] = "1";
		comboBoxLightValue2->Items->Strings[2] = "2";
		comboBoxLightValue2->Items->Strings[3] = "3";
		comboBoxLightValue2->Items->Strings[4] = "4";
		comboBoxLightValue2->Items->Strings[5] = "5";
		comboBoxLightValue2->Items->Strings[6] = "6";
		comboBoxLightValue2->Items->Strings[7] = "7 Brightest";

		comboBoxLightValue3->Items->Strings[0] = "0 Dark";
		comboBoxLightValue3->Items->Strings[1] = "1";
		comboBoxLightValue3->Items->Strings[2] = "2";
		comboBoxLightValue3->Items->Strings[3] = "3";
		comboBoxLightValue3->Items->Strings[4] = "4";
		comboBoxLightValue3->Items->Strings[5] = "5";
		comboBoxLightValue3->Items->Strings[6] = "6";
		comboBoxLightValue3->Items->Strings[7] = "7 Brightest";

		comboBoxLightValue4->Items->Strings[0] = "0 Dark";
		comboBoxLightValue4->Items->Strings[1] = "1";
		comboBoxLightValue4->Items->Strings[2] = "2";
		comboBoxLightValue4->Items->Strings[3] = "3";
		comboBoxLightValue4->Items->Strings[4] = "4";
		comboBoxLightValue4->Items->Strings[5] = "5";
		comboBoxLightValue4->Items->Strings[6] = "6";
		comboBoxLightValue4->Items->Strings[7] = "7 Brightest";

		comboBoxLightValue5->Items->Strings[0] = "0 Dark";
		comboBoxLightValue5->Items->Strings[1] = "1";
		comboBoxLightValue5->Items->Strings[2] = "2";
		comboBoxLightValue5->Items->Strings[3] = "3";
		comboBoxLightValue5->Items->Strings[4] = "4";
		comboBoxLightValue5->Items->Strings[5] = "5";
		comboBoxLightValue5->Items->Strings[6] = "6";
		comboBoxLightValue5->Items->Strings[7] = "7 Brightest";

		comboBoxLightValue6->Items->Strings[0] = "0 Dark";
		comboBoxLightValue6->Items->Strings[1] = "1";
		comboBoxLightValue6->Items->Strings[2] = "2";
		comboBoxLightValue6->Items->Strings[3] = "3";
		comboBoxLightValue6->Items->Strings[4] = "4";
		comboBoxLightValue6->Items->Strings[5] = "5";
		comboBoxLightValue6->Items->Strings[6] = "6";
		comboBoxLightValue6->Items->Strings[7] = "7 Brightest";

		comboBoxLightValue7->Items->Strings[0] = "0 Dark";
		comboBoxLightValue7->Items->Strings[1] = "1";
		comboBoxLightValue7->Items->Strings[2] = "2";
		comboBoxLightValue7->Items->Strings[3] = "3";
		comboBoxLightValue7->Items->Strings[4] = "4";
		comboBoxLightValue7->Items->Strings[5] = "5";
		comboBoxLightValue7->Items->Strings[6] = "6";
		comboBoxLightValue7->Items->Strings[7] = "7 Brightest";
	}
#ifdef FIXED_IP
	editDataCenter->Enabled = false;
	editSocketPort_Card->Enabled = false;
	checkTCP->Enabled = false;
#endif
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::comboBoxAreaCountChange(TObject *Sender)
{
	editAreaLeft1->Enabled = (comboBoxAreaCount->ItemIndex >= 1);
	editAreaTop1->Enabled = (comboBoxAreaCount->ItemIndex >= 1);
	editAreaWidth1->Enabled = (comboBoxAreaCount->ItemIndex >= 1);
	editAreaHeight1->Enabled = (comboBoxAreaCount->ItemIndex >= 1);

	editAreaLeft2->Enabled = (comboBoxAreaCount->ItemIndex >= 2);
	editAreaTop2->Enabled = (comboBoxAreaCount->ItemIndex >= 2);
	editAreaWidth2->Enabled = (comboBoxAreaCount->ItemIndex >= 2);
	editAreaHeight2->Enabled = (comboBoxAreaCount->ItemIndex >= 2);

	editAreaLeft3->Enabled = (comboBoxAreaCount->ItemIndex >= 3);
	editAreaTop3->Enabled = (comboBoxAreaCount->ItemIndex >= 3);
	editAreaWidth3->Enabled = (comboBoxAreaCount->ItemIndex >= 3);
	editAreaHeight3->Enabled = (comboBoxAreaCount->ItemIndex >= 3);

	editAreaLeft4->Enabled = (comboBoxAreaCount->ItemIndex >= 4);
	editAreaTop4->Enabled = (comboBoxAreaCount->ItemIndex >= 4);
	editAreaWidth4->Enabled = (comboBoxAreaCount->ItemIndex >= 4);
	editAreaHeight4->Enabled = (comboBoxAreaCount->ItemIndex >= 4);

	editAreaLeft5->Enabled = (comboBoxAreaCount->ItemIndex >= 5);
	editAreaTop5->Enabled = (comboBoxAreaCount->ItemIndex >= 5);
	editAreaWidth5->Enabled = (comboBoxAreaCount->ItemIndex >= 5);
	editAreaHeight5->Enabled = (comboBoxAreaCount->ItemIndex >= 5);

	editAreaLeft6->Enabled = (comboBoxAreaCount->ItemIndex >= 6);
	editAreaTop6->Enabled = (comboBoxAreaCount->ItemIndex >= 6);
	editAreaWidth6->Enabled = (comboBoxAreaCount->ItemIndex >= 6);
	editAreaHeight6->Enabled = (comboBoxAreaCount->ItemIndex >= 6);

	editAreaLeft7->Enabled = (comboBoxAreaCount->ItemIndex >= 7);
	editAreaTop7->Enabled = (comboBoxAreaCount->ItemIndex >= 7);
	editAreaWidth7->Enabled = (comboBoxAreaCount->ItemIndex >= 7);
	editAreaHeight7->Enabled = (comboBoxAreaCount->ItemIndex >= 7);

	DrawAreaImage();
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::btnWriteConfigClick(TObject *Sender)
{
	m_nCardId = editCardId->Text.ToInt();

	if (!OpenComm())
		return;

	//::Sleep(20);
	//SendCommand_StopDisplay();
	//::Sleep(200);
	m_ComPort.Open();
	SendCmd_WriteConfig();

	StartComm();
	timerWriteConfig->Enabled = true;
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCmd_WriteConfig_PD102B()
{
	SETUP_PARA_PD102B setup_Para;
	BYTE buf[1024];

	memset(&setup_Para, 0, sizeof(setup_Para));

	setup_Para.ucScanMode = comboBoxScanMode->ItemIndex;
	setup_Para.ucScanMode |= checkOE->Checked ? 0x80: 0;
	setup_Para.ucScanMode |= checkData->Checked ? 0x40: 0;
	setup_Para.ucScanMode |= rbtnDoubleColor->Checked ? 0x20: 0;
	setup_Para.ucScreenByteWidth = editScreenWidth->Text.ToInt() / 8;
	setup_Para.ucScreenHeight = editScreenHeight->Text.ToInt();
	setup_Para.ucCardId = editNewCardId->Text.ToInt();
	setup_Para.ucBaudrateIndex = cbBaudrate->ItemIndex;

	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = m_nCardId;
	pHead->ucCmd = MMI_CMD_WRITE_CONFIG;
	pHead->usLen = sizeof(SETUP_PARA_PD102B);
	memcpy(pHead + 1, &setup_Para, sizeof(SETUP_PARA_PD102B));
	buf[sizeof(SETUP_PARA_PD102B) + sizeof(MMI_CMD_HEAD)] = CalcChecksum(sizeof(SETUP_PARA_PD102B) + sizeof(MMI_CMD_HEAD), buf);

	int nLen = sizeof(SETUP_PARA_PD102B) + sizeof(MMI_CMD_HEAD) + 1;
	if (sysConfig.m_nCommMethod == 0)
		WriteComPort(nLen, buf);
	else
		SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), nLen, buf);
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCmd_WriteConfig_SN2P()
{
	SETUP_PARA_SN2P setup_Para;
	BYTE buf[1024];

	memset(&setup_Para, 0, sizeof(setup_Para));

	WORD usLocalSocketPort;
	BYTE bNegData;
	BYTE bNegOE;
	WORD usScreenWidth;
	WORD usScreenHeight;
	BYTE bDoubleColor;
	BYTE ucRes1;
	BYTE usRes2;
	BYTE usRes3;
	BYTE ucLocalIp[4];
	BYTE ucGateway[4];
	BYTE ucNetMask[4];

	setup_Para.ulScanMode = comboBoxScanMode->ItemIndex;
	if (checkByteReverse->Checked)
		setup_Para.ulScanMode |= 0x40000000;
	setup_Para.usLocalSocketPort = editSocketPort_Card->Text.ToInt();
	setup_Para.bNegData = checkData->Checked;
	setup_Para.bNegOE = checkOE->Checked;
	setup_Para.bDoubleColor = rbtnDoubleColor->Checked;
	setup_Para.usScreenWidth = editScreenWidth->Text.ToInt();
	setup_Para.usScreenHeight = editScreenHeight->Text.ToInt();
	*((DWORD*)setup_Para.ucLocalIp) = inet_addr(AnsiString(editNewLocalIp->Text).c_str());
	*((DWORD*)setup_Para.ucNetMask) = inet_addr(AnsiString(editNetMask->Text).c_str());
	*((DWORD*)setup_Para.ucGateway) = inet_addr(AnsiString(editGateway->Text).c_str());

	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = m_nCardId;
	pHead->ucCmd = MMI_CMD_WRITE_CONFIG;
	pHead->usLen = sizeof(setup_Para);
	memcpy(pHead + 1, &setup_Para, sizeof(setup_Para));
	buf[sizeof(setup_Para) + sizeof(MMI_CMD_HEAD)] = CalcChecksum(sizeof(setup_Para) + sizeof(MMI_CMD_HEAD), buf);

	int nLen = sizeof(setup_Para) + sizeof(MMI_CMD_HEAD) + 1;
	if (sysConfig.m_nCommMethod == 0)
		WriteComPort(nLen, buf);
	else
		SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), nLen, buf);
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCmd_WriteConfig_YL2010A()
{
	SETUP_PARA_YL2010A setup_Para;
	BYTE buf[1024];
	int ulBaudRate[] = {115200, 57600, 38400, 19200, 9600, 4800, 2400};

	memset(&setup_Para, 0, sizeof(setup_Para));

	setup_Para.ulVersion = SOFT_VER;
	setup_Para.ulScanMode = comboBoxScanMode->ItemIndex;
	if (checkByteReverse->Checked)
		setup_Para.ulScanMode |= 0x40000000;
	setup_Para.bNegData = checkData->Checked;
	setup_Para.bNegOE = checkOE->Checked;
	setup_Para.bSupportWarnning = checkSupportWarnning->Checked;
	setup_Para.bDoubleColor = rbtnDoubleColor->Checked;
	setup_Para.usScreenWidth = editScreenWidth->Text.ToInt();
	setup_Para.usScreenHeight = editScreenHeight->Text.ToInt();
	setup_Para.ucMaxActCount = m_nSelectActCountTable[comboBoxMaxActCount->ItemIndex];
	setup_Para.ulBaudrate = ulBaudRate[cbBaudrate->ItemIndex];
	setup_Para.ucCardId = editNewCardId->Text.ToInt();
	setup_Para.bActSupportRepeat = checkActSupportRepeat->Checked;
	setup_Para.bActSupportSync = checkSyncPlay->Checked;
	setup_Para.bSupportInsertPlay = checkSupportInsertPlay->Checked;
	setup_Para.bFullColor = rbtnFullColor->Checked;

	SYSTEMTIME st;

	DateTimeToSystemTime(timeStart0->Time, st);
	setup_Para.ucLightControlStartHour[0] = st.wHour;
	setup_Para.ucLightControlStartMinute[0] = st.wMinute;

	DateTimeToSystemTime(timeStart1->Time, st);
	setup_Para.ucLightControlStartHour[1] = st.wHour;
	setup_Para.ucLightControlStartMinute[1] = st.wMinute;

	DateTimeToSystemTime(timeStart2->Time, st);
	setup_Para.ucLightControlStartHour[2] = st.wHour;
	setup_Para.ucLightControlStartMinute[2] = st.wMinute;

	DateTimeToSystemTime(timeStart3->Time, st);
	setup_Para.ucLightControlStartHour[3] = st.wHour;
	setup_Para.ucLightControlStartMinute[3] = st.wMinute;

	DateTimeToSystemTime(timeStart4->Time, st);
	setup_Para.ucLightControlStartHour[4] = st.wHour;
	setup_Para.ucLightControlStartMinute[4] = st.wMinute;

	DateTimeToSystemTime(timeStart5->Time, st);
	setup_Para.ucLightControlStartHour[5] = st.wHour;
	setup_Para.ucLightControlStartMinute[5] = st.wMinute;

	DateTimeToSystemTime(timeStart6->Time, st);
	setup_Para.ucLightControlStartHour[6] = st.wHour;
	setup_Para.ucLightControlStartMinute[6] = st.wMinute;

	DateTimeToSystemTime(timeStart7->Time, st);
	setup_Para.ucLightControlStartHour[7] = st.wHour;
	setup_Para.ucLightControlStartMinute[7] = st.wMinute;

	setup_Para.ucLightControlValue[0] = comboBoxLightValue0->ItemIndex;
	setup_Para.ucLightControlValue[1] = comboBoxLightValue1->ItemIndex;
	setup_Para.ucLightControlValue[2] = comboBoxLightValue2->ItemIndex;
	setup_Para.ucLightControlValue[3] = comboBoxLightValue3->ItemIndex;
	setup_Para.ucLightControlValue[4] = comboBoxLightValue4->ItemIndex;
	setup_Para.ucLightControlValue[5] = comboBoxLightValue5->ItemIndex;
	setup_Para.ucLightControlValue[6] = comboBoxLightValue6->ItemIndex;
	setup_Para.ucLightControlValue[7] = comboBoxLightValue7->ItemIndex;

	setup_Para.usSubAreaLeft[0] = editAreaLeft0->Text.ToInt();
	setup_Para.usSubAreaLeft[1] = editAreaLeft1->Text.ToInt();
	setup_Para.usSubAreaLeft[2] = editAreaLeft2->Text.ToInt();
	setup_Para.usSubAreaLeft[3] = editAreaLeft3->Text.ToInt();
	setup_Para.usSubAreaLeft[4] = editAreaLeft4->Text.ToInt();
	setup_Para.usSubAreaLeft[5] = editAreaLeft5->Text.ToInt();
	setup_Para.usSubAreaLeft[6] = editAreaLeft6->Text.ToInt();
	setup_Para.usSubAreaLeft[7] = editAreaLeft7->Text.ToInt();

	setup_Para.usSubAreaTop[0] = editAreaTop0->Text.ToInt();
	setup_Para.usSubAreaTop[1] = editAreaTop1->Text.ToInt();
	setup_Para.usSubAreaTop[2] = editAreaTop2->Text.ToInt();
	setup_Para.usSubAreaTop[3] = editAreaTop3->Text.ToInt();
	setup_Para.usSubAreaTop[4] = editAreaTop4->Text.ToInt();
	setup_Para.usSubAreaTop[5] = editAreaTop5->Text.ToInt();
	setup_Para.usSubAreaTop[6] = editAreaTop6->Text.ToInt();
	setup_Para.usSubAreaTop[7] = editAreaTop7->Text.ToInt();

	setup_Para.usSubAreaWidth[0] = editAreaWidth0->Text.ToInt();
	setup_Para.usSubAreaWidth[1] = editAreaWidth1->Text.ToInt();
	setup_Para.usSubAreaWidth[2] = editAreaWidth2->Text.ToInt();
	setup_Para.usSubAreaWidth[3] = editAreaWidth3->Text.ToInt();
	setup_Para.usSubAreaWidth[4] = editAreaWidth4->Text.ToInt();
	setup_Para.usSubAreaWidth[5] = editAreaWidth5->Text.ToInt();
	setup_Para.usSubAreaWidth[6] = editAreaWidth6->Text.ToInt();
	setup_Para.usSubAreaWidth[7] = editAreaWidth7->Text.ToInt();

	setup_Para.usSubAreaHeight[0] = editAreaHeight0->Text.ToInt();
	setup_Para.usSubAreaHeight[1] = editAreaHeight1->Text.ToInt();
	setup_Para.usSubAreaHeight[2] = editAreaHeight2->Text.ToInt();
	setup_Para.usSubAreaHeight[3] = editAreaHeight3->Text.ToInt();
	setup_Para.usSubAreaHeight[4] = editAreaHeight4->Text.ToInt();
	setup_Para.usSubAreaHeight[5] = editAreaHeight5->Text.ToInt();
	setup_Para.usSubAreaHeight[6] = editAreaHeight6->Text.ToInt();
	setup_Para.usSubAreaHeight[7] = editAreaHeight7->Text.ToInt();

	setup_Para.ucSubAreaCount = comboBoxAreaCount->ItemIndex + 1;

	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = editCardId->Text.ToInt();
	pHead->ucCmd = MMI_CMD_WRITE_CONFIG;
	pHead->usLen = sizeof(setup_Para);
	memcpy(pHead + 1, &setup_Para, sizeof(setup_Para));
	buf[sizeof(setup_Para) + sizeof(MMI_CMD_HEAD)] = CalcChecksum(sizeof(setup_Para) + sizeof(MMI_CMD_HEAD), buf);
	int nLen = sizeof(setup_Para) + sizeof(MMI_CMD_HEAD) + 1;

	if (sysConfig.m_nCommMethod == 0)
		WriteComPort(nLen, buf);
	else
		SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), nLen, buf);
}

//---------------------------------------------------------------------------
void TfrmConfig::SendCmd_WriteConfig_GS7020B()
{
	SETUP_PARA_GS7020B setup_Para;
	BYTE buf[1024];

	memset(&setup_Para, 0, sizeof(setup_Para));

	setup_Para.ulVersion = SOFT_VER;
	setup_Para.ulScanMode = comboBoxScanMode->ItemIndex;
	if (checkByteReverse->Checked)
		setup_Para.ulScanMode |= 0x40000000;
	setup_Para.usSocketPort = editSocketPort_Card->Text.ToInt();
	strcpy(setup_Para.szAPN, AnsiString(editAPN->Text).c_str());
	strcpy(setup_Para.szDeviceCode, AnsiString(editDeviceCode->Text).c_str());
	strcpy(setup_Para.szDataCenter, AnsiString(editDataCenter->Text).c_str());
	setup_Para.bActSecond = checkActSecond->Checked;
	setup_Para.bActSupportRepeat = checkActSupportRepeat->Checked;
	setup_Para.bSupportInsertPlay = checkSupportInsertPlay->Checked;
	setup_Para.ucScanLineCycleCount = 0;
	setup_Para.bActSupportSync = checkSyncPlay->Checked;
	setup_Para.bActSupportCycleTime = checkSyncTimeHint->Checked;
	setup_Para.bNegData = checkData->Checked;
	setup_Para.bNegOE = checkOE->Checked;
	setup_Para.bSupportWarnning = checkSupportWarnning->Checked;
	setup_Para.bDoubleColor = rbtnDoubleColor->Checked;
	setup_Para.usScreenWidth = editScreenWidth->Text.ToInt();
	setup_Para.usScreenHeight = editScreenHeight->Text.ToInt();
	setup_Para.ucMaxActCount = m_nSelectActCountTable[comboBoxMaxActCount->ItemIndex];
	setup_Para.bSupport_LedSign_Check = checkSupportLedSignCheck->Checked;
	setup_Para.bTCP = checkTCP->Checked;
	setup_Para.bWaitFistLoginEcho = checkLoginEcho->Checked;
	setup_Para.bFullColor = rbtnFullColor->Checked;

	SYSTEMTIME st;

	DateTimeToSystemTime(timeStart0->Time, st);
	setup_Para.ucLightControlStartHour[0] = st.wHour;
	setup_Para.ucLightControlStartMinute[0] = st.wMinute;

	DateTimeToSystemTime(timeStart1->Time, st);
	setup_Para.ucLightControlStartHour[1] = st.wHour;
	setup_Para.ucLightControlStartMinute[1] = st.wMinute;

	DateTimeToSystemTime(timeStart2->Time, st);
	setup_Para.ucLightControlStartHour[2] = st.wHour;
	setup_Para.ucLightControlStartMinute[2] = st.wMinute;

	DateTimeToSystemTime(timeStart3->Time, st);
	setup_Para.ucLightControlStartHour[3] = st.wHour;
	setup_Para.ucLightControlStartMinute[3] = st.wMinute;

	DateTimeToSystemTime(timeStart4->Time, st);
	setup_Para.ucLightControlStartHour[4] = st.wHour;
	setup_Para.ucLightControlStartMinute[4] = st.wMinute;

	DateTimeToSystemTime(timeStart5->Time, st);
	setup_Para.ucLightControlStartHour[5] = st.wHour;
	setup_Para.ucLightControlStartMinute[5] = st.wMinute;

	DateTimeToSystemTime(timeStart6->Time, st);
	setup_Para.ucLightControlStartHour[6] = st.wHour;
	setup_Para.ucLightControlStartMinute[6] = st.wMinute;

	DateTimeToSystemTime(timeStart7->Time, st);
	setup_Para.ucLightControlStartHour[7] = st.wHour;
	setup_Para.ucLightControlStartMinute[7] = st.wMinute;

	setup_Para.ucLightControlValue[0] = comboBoxLightValue0->ItemIndex;
	setup_Para.ucLightControlValue[1] = comboBoxLightValue1->ItemIndex;
	setup_Para.ucLightControlValue[2] = comboBoxLightValue2->ItemIndex;
	setup_Para.ucLightControlValue[3] = comboBoxLightValue3->ItemIndex;
	setup_Para.ucLightControlValue[4] = comboBoxLightValue4->ItemIndex;
	setup_Para.ucLightControlValue[5] = comboBoxLightValue5->ItemIndex;
	setup_Para.ucLightControlValue[6] = comboBoxLightValue6->ItemIndex;
	setup_Para.ucLightControlValue[7] = comboBoxLightValue7->ItemIndex;

	setup_Para.usSubAreaLeft[0] = editAreaLeft0->Text.ToInt();
	setup_Para.usSubAreaLeft[1] = editAreaLeft1->Text.ToInt();
	setup_Para.usSubAreaLeft[2] = editAreaLeft2->Text.ToInt();
	setup_Para.usSubAreaLeft[3] = editAreaLeft3->Text.ToInt();
	setup_Para.usSubAreaLeft[4] = editAreaLeft4->Text.ToInt();
	setup_Para.usSubAreaLeft[5] = editAreaLeft5->Text.ToInt();
	setup_Para.usSubAreaLeft[6] = editAreaLeft6->Text.ToInt();
	setup_Para.usSubAreaLeft[7] = editAreaLeft7->Text.ToInt();

	setup_Para.usSubAreaTop[0] = editAreaTop0->Text.ToInt();
	setup_Para.usSubAreaTop[1] = editAreaTop1->Text.ToInt();
	setup_Para.usSubAreaTop[2] = editAreaTop2->Text.ToInt();
	setup_Para.usSubAreaTop[3] = editAreaTop3->Text.ToInt();
	setup_Para.usSubAreaTop[4] = editAreaTop4->Text.ToInt();
	setup_Para.usSubAreaTop[5] = editAreaTop5->Text.ToInt();
	setup_Para.usSubAreaTop[6] = editAreaTop6->Text.ToInt();
	setup_Para.usSubAreaTop[7] = editAreaTop7->Text.ToInt();

	setup_Para.usSubAreaWidth[0] = editAreaWidth0->Text.ToInt();
	setup_Para.usSubAreaWidth[1] = editAreaWidth1->Text.ToInt();
	setup_Para.usSubAreaWidth[2] = editAreaWidth2->Text.ToInt();
	setup_Para.usSubAreaWidth[3] = editAreaWidth3->Text.ToInt();
	setup_Para.usSubAreaWidth[4] = editAreaWidth4->Text.ToInt();
	setup_Para.usSubAreaWidth[5] = editAreaWidth5->Text.ToInt();
	setup_Para.usSubAreaWidth[6] = editAreaWidth6->Text.ToInt();
	setup_Para.usSubAreaWidth[7] = editAreaWidth7->Text.ToInt();

	setup_Para.usSubAreaHeight[0] = editAreaHeight0->Text.ToInt();
	setup_Para.usSubAreaHeight[1] = editAreaHeight1->Text.ToInt();
	setup_Para.usSubAreaHeight[2] = editAreaHeight2->Text.ToInt();
	setup_Para.usSubAreaHeight[3] = editAreaHeight3->Text.ToInt();
	setup_Para.usSubAreaHeight[4] = editAreaHeight4->Text.ToInt();
	setup_Para.usSubAreaHeight[5] = editAreaHeight5->Text.ToInt();
	setup_Para.usSubAreaHeight[6] = editAreaHeight6->Text.ToInt();
	setup_Para.usSubAreaHeight[7] = editAreaHeight7->Text.ToInt();

	setup_Para.ucSubAreaCount = comboBoxAreaCount->ItemIndex + 1;
	setup_Para.ulUserBaudRate = setup_Para.ulModembaudRate = cbBaudrate->Text.ToInt();

	if (sysConfig.m_nCardType == CARD_TYPE_GS9020F_GPS)
	{
		strcpy(setup_Para.szGpsServer, AnsiString(editGpsServer->Text).c_str());
		setup_Para.usGpsSocketPort = editGpsSocketPort->Text.ToInt();
		setup_Para.usGpsReportInterval = editGpsReportInterval->Text.ToInt();
		setup_Para.ulGpsId = inet_addr(AnsiString(editGpsPseudoIp->Text).c_str());
	}

	int nLen;
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB)  || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
	{
		MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;
		pHead->usHeadTag = MMI_START_ID;
		pHead->ucCardId = editCardId->Text.ToInt();
		pHead->ucCmd = MMI_CMD_WRITE_CONFIG;
		pHead->usLen = sizeof(setup_Para);
		memcpy(pHead + 1, &setup_Para, sizeof(setup_Para));
		pHead->ucKey1 = m_ucKey1 = GetTickCount();
		pHead->ucKey2 = m_ucKey2 = GetTickCount() >> 8;
		buf[sizeof(setup_Para) + sizeof(MMI_CMD_HEAD_HEB)] = CalcChecksum(sizeof(setup_Para) + sizeof(MMI_CMD_HEAD_HEB), buf);
		Encrypt(pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);
		nLen = sizeof(setup_Para) + sizeof(MMI_CMD_HEAD_HEB) + 1;
	}
	else
	{
		MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
		pHead->usHeadTag = MMI_START_ID;
		pHead->ucCardId = m_nCardId;
		pHead->ucCmd = MMI_CMD_WRITE_CONFIG;
		pHead->usLen = sizeof(setup_Para);
		memcpy(pHead + 1, &setup_Para, sizeof(setup_Para));
		buf[sizeof(setup_Para) + sizeof(MMI_CMD_HEAD)] = CalcChecksum(sizeof(setup_Para) + sizeof(MMI_CMD_HEAD), buf);
		nLen = sizeof(setup_Para) + sizeof(MMI_CMD_HEAD) + 1;
	}

	if (sysConfig.m_nCommMethod == 0)
		WriteComPort(nLen, buf);
	else
		SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), nLen, buf);
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCmd_WriteConfig_GS9025()
{
	SETUP_PARA_GS7020B setup_Para;
	BYTE buf[1024];

	memset(&setup_Para, 0, sizeof(setup_Para));

	setup_Para.ulVersion = SOFT_VER;
	setup_Para.ulScanMode = comboBoxScanMode->ItemIndex;
	if (checkByteReverse->Checked)
		setup_Para.ulScanMode |= 0x40000000;
	setup_Para.usSocketPort = editSocketPort_Card->Text.ToInt();
	strcpy(setup_Para.szAPN, AnsiString(editAPN->Text).c_str());
	strcpy(setup_Para.szDeviceCode, AnsiString(editDeviceCode->Text).c_str());
	strcpy(setup_Para.szDataCenter, AnsiString(editDataCenter->Text).c_str());
	setup_Para.bActSecond = false;
	setup_Para.bActSupportRepeat = true;
	setup_Para.bSupportInsertPlay = false;
	setup_Para.ucScanLineCycleCount = 0;
	setup_Para.bActSupportSync = false;
	setup_Para.bActSupportCycleTime = false;
	setup_Para.bNegData = checkData->Checked;
	setup_Para.bNegOE = checkOE->Checked;
	setup_Para.bSupportWarnning = false;
	setup_Para.bDoubleColor = false;
	setup_Para.usScreenWidth = editScreenWidth->Text.ToInt();
	if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
	{
		setup_Para.usScreenHeight = 32;
		setup_Para.ucMaxActCount = 16;
	}
	else	// GS9023
	{
		setup_Para.usScreenHeight = 16;
		setup_Para.ucMaxActCount = 128;
	}
	setup_Para.bSupport_LedSign_Check = false;
	setup_Para.bTCP = checkTCP->Checked;
	setup_Para.bWaitFistLoginEcho = false;

	SYSTEMTIME st;

	DateTimeToSystemTime(timeStart0->Time, st);
	setup_Para.ucLightControlStartHour[0] = st.wHour;
	setup_Para.ucLightControlStartMinute[0] = st.wMinute;

	DateTimeToSystemTime(timeStart1->Time, st);
	setup_Para.ucLightControlStartHour[1] = st.wHour;
	setup_Para.ucLightControlStartMinute[1] = st.wMinute;

	DateTimeToSystemTime(timeStart2->Time, st);
	setup_Para.ucLightControlStartHour[2] = st.wHour;
	setup_Para.ucLightControlStartMinute[2] = st.wMinute;

	DateTimeToSystemTime(timeStart3->Time, st);
	setup_Para.ucLightControlStartHour[3] = st.wHour;
	setup_Para.ucLightControlStartMinute[3] = st.wMinute;

	DateTimeToSystemTime(timeStart4->Time, st);
	setup_Para.ucLightControlStartHour[4] = st.wHour;
	setup_Para.ucLightControlStartMinute[4] = st.wMinute;

	DateTimeToSystemTime(timeStart5->Time, st);
	setup_Para.ucLightControlStartHour[5] = st.wHour;
	setup_Para.ucLightControlStartMinute[5] = st.wMinute;

	DateTimeToSystemTime(timeStart6->Time, st);
	setup_Para.ucLightControlStartHour[6] = st.wHour;
	setup_Para.ucLightControlStartMinute[6] = st.wMinute;

	DateTimeToSystemTime(timeStart7->Time, st);
	setup_Para.ucLightControlStartHour[7] = st.wHour;
	setup_Para.ucLightControlStartMinute[7] = st.wMinute;

	setup_Para.ucLightControlValue[0] = comboBoxLightValue0->ItemIndex;
	setup_Para.ucLightControlValue[1] = comboBoxLightValue1->ItemIndex;
	setup_Para.ucLightControlValue[2] = comboBoxLightValue2->ItemIndex;
	setup_Para.ucLightControlValue[3] = comboBoxLightValue3->ItemIndex;
	setup_Para.ucLightControlValue[4] = comboBoxLightValue4->ItemIndex;
	setup_Para.ucLightControlValue[5] = comboBoxLightValue5->ItemIndex;
	setup_Para.ucLightControlValue[6] = comboBoxLightValue6->ItemIndex;
	setup_Para.ucLightControlValue[7] = comboBoxLightValue7->ItemIndex;

	setup_Para.usSubAreaLeft[0] = editAreaLeft0->Text.ToInt();
	setup_Para.usSubAreaLeft[1] = editAreaLeft1->Text.ToInt();
	setup_Para.usSubAreaLeft[2] = editAreaLeft2->Text.ToInt();
	setup_Para.usSubAreaLeft[3] = editAreaLeft3->Text.ToInt();
	setup_Para.usSubAreaLeft[4] = editAreaLeft4->Text.ToInt();
	setup_Para.usSubAreaLeft[5] = editAreaLeft5->Text.ToInt();
	setup_Para.usSubAreaLeft[6] = editAreaLeft6->Text.ToInt();
	setup_Para.usSubAreaLeft[7] = editAreaLeft7->Text.ToInt();

	setup_Para.usSubAreaTop[0] = editAreaTop0->Text.ToInt();
	setup_Para.usSubAreaTop[1] = editAreaTop1->Text.ToInt();
	setup_Para.usSubAreaTop[2] = editAreaTop2->Text.ToInt();
	setup_Para.usSubAreaTop[3] = editAreaTop3->Text.ToInt();
	setup_Para.usSubAreaTop[4] = editAreaTop4->Text.ToInt();
	setup_Para.usSubAreaTop[5] = editAreaTop5->Text.ToInt();
	setup_Para.usSubAreaTop[6] = editAreaTop6->Text.ToInt();
	setup_Para.usSubAreaTop[7] = editAreaTop7->Text.ToInt();

	setup_Para.usSubAreaWidth[0] = editAreaWidth0->Text.ToInt();
	setup_Para.usSubAreaWidth[1] = editAreaWidth1->Text.ToInt();
	setup_Para.usSubAreaWidth[2] = editAreaWidth2->Text.ToInt();
	setup_Para.usSubAreaWidth[3] = editAreaWidth3->Text.ToInt();
	setup_Para.usSubAreaWidth[4] = editAreaWidth4->Text.ToInt();
	setup_Para.usSubAreaWidth[5] = editAreaWidth5->Text.ToInt();
	setup_Para.usSubAreaWidth[6] = editAreaWidth6->Text.ToInt();
	setup_Para.usSubAreaWidth[7] = editAreaWidth7->Text.ToInt();

	setup_Para.usSubAreaHeight[0] = editAreaHeight0->Text.ToInt();
	setup_Para.usSubAreaHeight[1] = editAreaHeight1->Text.ToInt();
	setup_Para.usSubAreaHeight[2] = editAreaHeight2->Text.ToInt();
	setup_Para.usSubAreaHeight[3] = editAreaHeight3->Text.ToInt();
	setup_Para.usSubAreaHeight[4] = editAreaHeight4->Text.ToInt();
	setup_Para.usSubAreaHeight[5] = editAreaHeight5->Text.ToInt();
	setup_Para.usSubAreaHeight[6] = editAreaHeight6->Text.ToInt();
	setup_Para.usSubAreaHeight[7] = editAreaHeight7->Text.ToInt();

	setup_Para.ucSubAreaCount = comboBoxAreaCount->ItemIndex + 1;
	setup_Para.ulUserBaudRate = setup_Para.ulModembaudRate = cbBaudrate->Text.ToInt();

	int nLen;
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = m_nCardId;
	pHead->ucCmd = MMI_CMD_WRITE_CONFIG;
	pHead->usLen = sizeof(setup_Para);
	memcpy(pHead + 1, &setup_Para, sizeof(setup_Para));
	buf[sizeof(setup_Para) + sizeof(MMI_CMD_HEAD)] = CalcChecksum(sizeof(setup_Para) + sizeof(MMI_CMD_HEAD), buf);
	nLen = sizeof(setup_Para) + sizeof(MMI_CMD_HEAD) + 1;

	if (sysConfig.m_nCommMethod == 0)
		WriteComPort(nLen, buf);
	else
		SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), nLen, buf);
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCmd_WriteConfig_WF2012B()
{
	SETUP_PARA_WF2012B setup_Para;
	BYTE buf[1024];

	memset(&setup_Para, 0, sizeof(setup_Para));

	setup_Para.ulVersion = 0;
	setup_Para.ulScanMode = comboBoxScanMode->ItemIndex;
	if (checkByteReverse->Checked)
		setup_Para.ulScanMode |= 0x40000000;
	setup_Para.usSocketPort = editWifiSocketPort_Card->Text.ToInt();
	strcpy(setup_Para.szSSID, AnsiString(editSSID->Text).c_str());
	strcpy(setup_Para.szDeviceCode, AnsiString(editWifiDeviceCode->Text).c_str());
	strcpy(setup_Para.szDataCenter, AnsiString(editWifiServer->Text).c_str());
	strcpy(setup_Para.szPassword, AnsiString(editWifiPassword->Text).c_str());
	strcpy(setup_Para.szDnsServer, AnsiString(editWifiDnsServer->Text).c_str());
	setup_Para.bActSecond = checkActSecond->Checked;
	setup_Para.bActSupportRepeat = checkActSupportRepeat->Checked;
	setup_Para.bSupportInsertPlay = checkSupportInsertPlay->Checked;
	setup_Para.ucScanLineCycleCount = 0;
	setup_Para.bNegData = checkData->Checked;
	setup_Para.bNegOE = checkOE->Checked;
	setup_Para.bSupportWarnning = checkSupportWarnning->Checked;
	setup_Para.bDoubleColor = rbtnDoubleColor->Checked;
	setup_Para.usScreenWidth = editScreenWidth->Text.ToInt();
	setup_Para.usScreenHeight = editScreenHeight->Text.ToInt();
	setup_Para.ucMaxActCount = m_nSelectActCountTable[comboBoxMaxActCount->ItemIndex];

	SYSTEMTIME st;

	DateTimeToSystemTime(timeStart0->Time, st);
	setup_Para.ucLightControlStartHour[0] = st.wHour;
	setup_Para.ucLightControlStartMinute[0] = st.wMinute;

	DateTimeToSystemTime(timeStart1->Time, st);
	setup_Para.ucLightControlStartHour[1] = st.wHour;
	setup_Para.ucLightControlStartMinute[1] = st.wMinute;

	DateTimeToSystemTime(timeStart2->Time, st);
	setup_Para.ucLightControlStartHour[2] = st.wHour;
	setup_Para.ucLightControlStartMinute[2] = st.wMinute;

	DateTimeToSystemTime(timeStart3->Time, st);
	setup_Para.ucLightControlStartHour[3] = st.wHour;
	setup_Para.ucLightControlStartMinute[3] = st.wMinute;

	DateTimeToSystemTime(timeStart4->Time, st);
	setup_Para.ucLightControlStartHour[4] = st.wHour;
	setup_Para.ucLightControlStartMinute[4] = st.wMinute;

	DateTimeToSystemTime(timeStart5->Time, st);
	setup_Para.ucLightControlStartHour[5] = st.wHour;
	setup_Para.ucLightControlStartMinute[5] = st.wMinute;

	DateTimeToSystemTime(timeStart6->Time, st);
	setup_Para.ucLightControlStartHour[6] = st.wHour;
	setup_Para.ucLightControlStartMinute[6] = st.wMinute;

	DateTimeToSystemTime(timeStart7->Time, st);
	setup_Para.ucLightControlStartHour[7] = st.wHour;
	setup_Para.ucLightControlStartMinute[7] = st.wMinute;

	setup_Para.ucLightControlValue[0] = comboBoxLightValue0->ItemIndex;
	setup_Para.ucLightControlValue[1] = comboBoxLightValue1->ItemIndex;
	setup_Para.ucLightControlValue[2] = comboBoxLightValue2->ItemIndex;
	setup_Para.ucLightControlValue[3] = comboBoxLightValue3->ItemIndex;
	setup_Para.ucLightControlValue[4] = comboBoxLightValue4->ItemIndex;
	setup_Para.ucLightControlValue[5] = comboBoxLightValue5->ItemIndex;
	setup_Para.ucLightControlValue[6] = comboBoxLightValue6->ItemIndex;
	setup_Para.ucLightControlValue[7] = comboBoxLightValue7->ItemIndex;

	setup_Para.usSubAreaLeft[0] = editAreaLeft0->Text.ToInt();
	setup_Para.usSubAreaLeft[1] = editAreaLeft1->Text.ToInt();
	setup_Para.usSubAreaLeft[2] = editAreaLeft2->Text.ToInt();
	setup_Para.usSubAreaLeft[3] = editAreaLeft3->Text.ToInt();
	setup_Para.usSubAreaLeft[4] = editAreaLeft4->Text.ToInt();
	setup_Para.usSubAreaLeft[5] = editAreaLeft5->Text.ToInt();
	setup_Para.usSubAreaLeft[6] = editAreaLeft6->Text.ToInt();
	setup_Para.usSubAreaLeft[7] = editAreaLeft7->Text.ToInt();

	setup_Para.usSubAreaTop[0] = editAreaTop0->Text.ToInt();
	setup_Para.usSubAreaTop[1] = editAreaTop1->Text.ToInt();
	setup_Para.usSubAreaTop[2] = editAreaTop2->Text.ToInt();
	setup_Para.usSubAreaTop[3] = editAreaTop3->Text.ToInt();
	setup_Para.usSubAreaTop[4] = editAreaTop4->Text.ToInt();
	setup_Para.usSubAreaTop[5] = editAreaTop5->Text.ToInt();
	setup_Para.usSubAreaTop[6] = editAreaTop6->Text.ToInt();
	setup_Para.usSubAreaTop[7] = editAreaTop7->Text.ToInt();

	setup_Para.usSubAreaWidth[0] = editAreaWidth0->Text.ToInt();
	setup_Para.usSubAreaWidth[1] = editAreaWidth1->Text.ToInt();
	setup_Para.usSubAreaWidth[2] = editAreaWidth2->Text.ToInt();
	setup_Para.usSubAreaWidth[3] = editAreaWidth3->Text.ToInt();
	setup_Para.usSubAreaWidth[4] = editAreaWidth4->Text.ToInt();
	setup_Para.usSubAreaWidth[5] = editAreaWidth5->Text.ToInt();
	setup_Para.usSubAreaWidth[6] = editAreaWidth6->Text.ToInt();
	setup_Para.usSubAreaWidth[7] = editAreaWidth7->Text.ToInt();

	setup_Para.usSubAreaHeight[0] = editAreaHeight0->Text.ToInt();
	setup_Para.usSubAreaHeight[1] = editAreaHeight1->Text.ToInt();
	setup_Para.usSubAreaHeight[2] = editAreaHeight2->Text.ToInt();
	setup_Para.usSubAreaHeight[3] = editAreaHeight3->Text.ToInt();
	setup_Para.usSubAreaHeight[4] = editAreaHeight4->Text.ToInt();
	setup_Para.usSubAreaHeight[5] = editAreaHeight5->Text.ToInt();
	setup_Para.usSubAreaHeight[6] = editAreaHeight6->Text.ToInt();
	setup_Para.usSubAreaHeight[7] = editAreaHeight7->Text.ToInt();

	setup_Para.ucSubAreaCount = comboBoxAreaCount->ItemIndex + 1;

	int nLen;
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = m_nCardId;
	pHead->ucCmd = MMI_CMD_WRITE_CONFIG;
	pHead->usLen = sizeof(setup_Para);
	memcpy(pHead + 1, &setup_Para, sizeof(setup_Para));
	buf[sizeof(setup_Para) + sizeof(MMI_CMD_HEAD)] = CalcChecksum(sizeof(setup_Para) + sizeof(MMI_CMD_HEAD), buf);
	nLen = sizeof(setup_Para) + sizeof(MMI_CMD_HEAD) + 1;

	if (sysConfig.m_nCommMethod == 0)
		WriteComPort(nLen, buf);
	else
		SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), nLen, buf);
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCmd_WriteConfig_GS7022B()
{
	SETUP_PARA_GS7022B setup_Para;
	BYTE buf[1024];

	memset(&setup_Para, 0, sizeof(setup_Para));

	setup_Para.ulVersion = SOFT_VER;
	setup_Para.ulScanMode = comboBoxScanMode->ItemIndex;
	if (checkByteReverse->Checked)
		setup_Para.ulScanMode |= 0x40000000;
	setup_Para.usSocketPort = editSocketPort_Card->Text.ToInt();
	strcpy(setup_Para.szAPN, AnsiString(editAPN->Text).c_str());
	strcpy(setup_Para.szDeviceCode, AnsiString(editDeviceCode->Text).c_str());
	strcpy(setup_Para.szDataCenter, AnsiString(editDataCenter->Text).c_str());
	setup_Para.bActSecond = checkActSecond->Checked;
	setup_Para.bActSupportRepeat = checkActSupportRepeat->Checked;
	setup_Para.bSupportInsertPlay = checkSupportInsertPlay->Checked;
	setup_Para.ucScanLineCycleCount = 0;
	setup_Para.bActSupportSync = checkSyncPlay->Checked;
	setup_Para.bNegData = checkData->Checked;
	setup_Para.bNegOE = checkOE->Checked;
	setup_Para.bSupportWarnning = checkSupportWarnning->Checked;
	setup_Para.bDoubleColor = rbtnDoubleColor->Checked;
	setup_Para.usScreenWidth = editScreenWidth->Text.ToInt();
	setup_Para.usScreenHeight = editScreenHeight->Text.ToInt();
	setup_Para.ucMaxActCount = m_nSelectActCountTable[comboBoxMaxActCount->ItemIndex];
	if (sysConfig.m_nCardType == CARD_TYPE_GS2012A_S)
	{
		setup_Para.ucVoiceOutputAreaIndex = editVoiceArea->Text.ToInt();
		setup_Para.bTCP = checkTCP->Checked;
	}

	SYSTEMTIME st;

	DateTimeToSystemTime(timeStart0->Time, st);
	setup_Para.ucLightControlStartHour[0] = st.wHour;
	setup_Para.ucLightControlStartMinute[0] = st.wMinute;

	DateTimeToSystemTime(timeStart1->Time, st);
	setup_Para.ucLightControlStartHour[1] = st.wHour;
	setup_Para.ucLightControlStartMinute[1] = st.wMinute;

	DateTimeToSystemTime(timeStart2->Time, st);
	setup_Para.ucLightControlStartHour[2] = st.wHour;
	setup_Para.ucLightControlStartMinute[2] = st.wMinute;

	DateTimeToSystemTime(timeStart3->Time, st);
	setup_Para.ucLightControlStartHour[3] = st.wHour;
	setup_Para.ucLightControlStartMinute[3] = st.wMinute;

	DateTimeToSystemTime(timeStart4->Time, st);
	setup_Para.ucLightControlStartHour[4] = st.wHour;
	setup_Para.ucLightControlStartMinute[4] = st.wMinute;

	DateTimeToSystemTime(timeStart5->Time, st);
	setup_Para.ucLightControlStartHour[5] = st.wHour;
	setup_Para.ucLightControlStartMinute[5] = st.wMinute;

	DateTimeToSystemTime(timeStart6->Time, st);
	setup_Para.ucLightControlStartHour[6] = st.wHour;
	setup_Para.ucLightControlStartMinute[6] = st.wMinute;

	DateTimeToSystemTime(timeStart7->Time, st);
	setup_Para.ucLightControlStartHour[7] = st.wHour;
	setup_Para.ucLightControlStartMinute[7] = st.wMinute;

	setup_Para.ucLightControlValue[0] = comboBoxLightValue0->ItemIndex;
	setup_Para.ucLightControlValue[1] = comboBoxLightValue1->ItemIndex;
	setup_Para.ucLightControlValue[2] = comboBoxLightValue2->ItemIndex;
	setup_Para.ucLightControlValue[3] = comboBoxLightValue3->ItemIndex;
	setup_Para.ucLightControlValue[4] = comboBoxLightValue4->ItemIndex;
	setup_Para.ucLightControlValue[5] = comboBoxLightValue5->ItemIndex;
	setup_Para.ucLightControlValue[6] = comboBoxLightValue6->ItemIndex;
	setup_Para.ucLightControlValue[7] = comboBoxLightValue7->ItemIndex;

	setup_Para.usSubAreaLeft[0] = editAreaLeft0->Text.ToInt();
	setup_Para.usSubAreaLeft[1] = editAreaLeft1->Text.ToInt();
	setup_Para.usSubAreaLeft[2] = editAreaLeft2->Text.ToInt();
	setup_Para.usSubAreaLeft[3] = editAreaLeft3->Text.ToInt();
	setup_Para.usSubAreaLeft[4] = editAreaLeft4->Text.ToInt();
	setup_Para.usSubAreaLeft[5] = editAreaLeft5->Text.ToInt();
	setup_Para.usSubAreaLeft[6] = editAreaLeft6->Text.ToInt();
	setup_Para.usSubAreaLeft[7] = editAreaLeft7->Text.ToInt();

	setup_Para.usSubAreaTop[0] = editAreaTop0->Text.ToInt();
	setup_Para.usSubAreaTop[1] = editAreaTop1->Text.ToInt();
	setup_Para.usSubAreaTop[2] = editAreaTop2->Text.ToInt();
	setup_Para.usSubAreaTop[3] = editAreaTop3->Text.ToInt();
	setup_Para.usSubAreaTop[4] = editAreaTop4->Text.ToInt();
	setup_Para.usSubAreaTop[5] = editAreaTop5->Text.ToInt();
	setup_Para.usSubAreaTop[6] = editAreaTop6->Text.ToInt();
	setup_Para.usSubAreaTop[7] = editAreaTop7->Text.ToInt();

	setup_Para.usSubAreaWidth[0] = editAreaWidth0->Text.ToInt();
	setup_Para.usSubAreaWidth[1] = editAreaWidth1->Text.ToInt();
	setup_Para.usSubAreaWidth[2] = editAreaWidth2->Text.ToInt();
	setup_Para.usSubAreaWidth[3] = editAreaWidth3->Text.ToInt();
	setup_Para.usSubAreaWidth[4] = editAreaWidth4->Text.ToInt();
	setup_Para.usSubAreaWidth[5] = editAreaWidth5->Text.ToInt();
	setup_Para.usSubAreaWidth[6] = editAreaWidth6->Text.ToInt();
	setup_Para.usSubAreaWidth[7] = editAreaWidth7->Text.ToInt();

	setup_Para.usSubAreaHeight[0] = editAreaHeight0->Text.ToInt();
	setup_Para.usSubAreaHeight[1] = editAreaHeight1->Text.ToInt();
	setup_Para.usSubAreaHeight[2] = editAreaHeight2->Text.ToInt();
	setup_Para.usSubAreaHeight[3] = editAreaHeight3->Text.ToInt();
	setup_Para.usSubAreaHeight[4] = editAreaHeight4->Text.ToInt();
	setup_Para.usSubAreaHeight[5] = editAreaHeight5->Text.ToInt();
	setup_Para.usSubAreaHeight[6] = editAreaHeight6->Text.ToInt();
	setup_Para.usSubAreaHeight[7] = editAreaHeight7->Text.ToInt();

	setup_Para.ucSubAreaCount = comboBoxAreaCount->ItemIndex + 1;

	strcpy(setup_Para.m_szSmsUsers[0], AnsiString(editSmsUser0->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[1], AnsiString(editSmsUser1->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[2], AnsiString(editSmsUser2->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[3], AnsiString(editSmsUser3->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[4], AnsiString(editSmsUser4->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[5], AnsiString(editSmsUser5->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[6], AnsiString(editSmsUser6->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[7], AnsiString(editSmsUser7->Text).c_str());
	setup_Para.ucReplaySms = checkEchoSms0->Checked ? 0x01 : 0;
	setup_Para.ucReplaySms |= (checkEchoSms1->Checked ? 0x02 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms2->Checked ? 0x04 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms3->Checked ? 0x08 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms4->Checked ? 0x10 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms5->Checked ? 0x20 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms6->Checked ? 0x40 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms7->Checked ? 0x80 : 0);

	setup_Para.ulShowTime = editShowTime->Text.ToInt();
	setup_Para.ucEnterStyle = comboEnterStyle->ItemIndex;
	setup_Para.ucEnterSpeed = editEnterSpeed->Text.ToInt();
	setup_Para.ucExitStyle = comboExitStyle->ItemIndex;
	setup_Para.ucExitSpeed = editExitSpeed->Text.ToInt();
	setup_Para.ucShutterWidth = 8;
	setup_Para.ucStartYear = 0;
	setup_Para.ucStartMonth = 1;
	setup_Para.ucStartDay = 1;
	setup_Para.ucStartHour = 0;
	setup_Para.ucStartMinute = 0;
	setup_Para.ucEndYear = 99;
	setup_Para.ucEndMonth = 12;
	setup_Para.ucEndDay = 31;
	setup_Para.ucEndHour = 23;
	setup_Para.ucEndMinute = 59;
	setup_Para.ucAlignFlags = 0;
	setup_Para.ucRepeatCount = editRepeatCount->Text.ToInt();
	setup_Para.ucRes4 = 0;
	setup_Para.ucFontSize = comboFont->Text.ToInt();
	setup_Para.ulStartSecondInDay[0] = 0;
	setup_Para.ulStartSecondInDay[1] = 0;
	setup_Para.ulStartSecondInDay[2] = 0;
	setup_Para.ulStartSecondInDay[3] = 0;
	setup_Para.ulEndSecondInDay[0] = 24 * 60 * 60 - 1;
	setup_Para.ulEndSecondInDay[1] = 24 * 60 * 60 - 1;
	setup_Para.ulEndSecondInDay[2] = 24 * 60 * 60 - 1;
	setup_Para.ulEndSecondInDay[3] = 24 * 60 * 60 - 1;

	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = m_nCardId;
	pHead->ucCmd = MMI_CMD_WRITE_CONFIG;
	pHead->usLen = sizeof(setup_Para);
	memcpy(pHead + 1, &setup_Para, sizeof(setup_Para));
	buf[sizeof(setup_Para) + sizeof(MMI_CMD_HEAD)] = CalcChecksum(sizeof(setup_Para) + sizeof(MMI_CMD_HEAD), buf);
	int nLen = sizeof(setup_Para) + sizeof(MMI_CMD_HEAD) + 1;

	if (sysConfig.m_nCommMethod == 0)
		WriteComPort(nLen, buf);
	else
		SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), nLen, buf);
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCmd_WriteConfig_GS9020S()
{
	SETUP_PARA_GS9020S setup_Para;
	BYTE buf[1024];

	memset(&setup_Para, 0, sizeof(setup_Para));

	setup_Para.usScreenWidth = editScreenWidth->Text.ToInt();
	setup_Para.usScreenHeight = editScreenHeight->Text.ToInt();
	setup_Para.ucScanMode = comboBoxScanMode->ItemIndex;
	if (checkData->Checked)
		setup_Para.ucFlags |= 0x01;
	if (checkOE->Checked)
		setup_Para.ucFlags |= 0x02;
	if (checkByteReverse->Checked)
		setup_Para.ucFlags |= 0x08;

	SYSTEMTIME st;

	DateTimeToSystemTime(timeStart0->Time, st);
	setup_Para.ucLightControlStartHour[0] = st.wHour;
	setup_Para.ucLightControlStartMinute[0] = st.wMinute;

	DateTimeToSystemTime(timeStart1->Time, st);
	setup_Para.ucLightControlStartHour[1] = st.wHour;
	setup_Para.ucLightControlStartMinute[1] = st.wMinute;

	DateTimeToSystemTime(timeStart2->Time, st);
	setup_Para.ucLightControlStartHour[2] = st.wHour;
	setup_Para.ucLightControlStartMinute[2] = st.wMinute;

	DateTimeToSystemTime(timeStart3->Time, st);
	setup_Para.ucLightControlStartHour[3] = st.wHour;
	setup_Para.ucLightControlStartMinute[3] = st.wMinute;

	setup_Para.ucLightControlValue[0] = comboBoxLightValue0->ItemIndex;
	setup_Para.ucLightControlValue[1] = comboBoxLightValue1->ItemIndex;
	setup_Para.ucLightControlValue[2] = comboBoxLightValue2->ItemIndex;
	setup_Para.ucLightControlValue[3] = comboBoxLightValue3->ItemIndex;

	strcpy(setup_Para.m_szSmsUsers[0], AnsiString(editSmsUser0->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[1], AnsiString(editSmsUser1->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[2], AnsiString(editSmsUser2->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[3], AnsiString(editSmsUser3->Text).c_str());
	setup_Para.ucReplaySms = checkEchoSms0->Checked ? 0x01 : 0;
	setup_Para.ucReplaySms |= (checkEchoSms1->Checked ? 0x02 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms2->Checked ? 0x04 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms3->Checked ? 0x08 : 0);

	setup_Para.ulShowTime = editShowTime->Text.ToInt();
	setup_Para.ucEnterStyle = comboEnterStyle->ItemIndex;
	setup_Para.ucEnterSpeed = editEnterSpeed->Text.ToInt();
	setup_Para.ucExitStyle = comboExitStyle->ItemIndex;
	setup_Para.ucExitSpeed = editExitSpeed->Text.ToInt();
	setup_Para.ucShutterWidth = 8;
	setup_Para.usStartYear = 0;
	setup_Para.ucStartMonth = 1;
	setup_Para.ucStartDay = 1;
	setup_Para.ucStartHour = 0;
	setup_Para.ucStartMinute = 0;
	setup_Para.usEndYear = 2099;
	setup_Para.ucEndMonth = 12;
	setup_Para.ucEndDay = 31;
	setup_Para.ucEndHour = 23;
	setup_Para.ucEndMinute = 59;
	setup_Para.ucAlignFlags = 0;
	setup_Para.ucRepeatCount = editRepeatCount->Text.ToInt();
	setup_Para.ucFontSize = comboFont->Text.ToInt();
	setup_Para.ulStartSecondInDay[0] = 0;
	setup_Para.ulStartSecondInDay[1] = 0;
	setup_Para.ulStartSecondInDay[2] = 0;
	setup_Para.ulStartSecondInDay[3] = 0;
	setup_Para.ulEndSecondInDay[0] = 24 * 60 * 60 - 1;
	setup_Para.ulEndSecondInDay[1] = 24 * 60 * 60 - 1;
	setup_Para.ulEndSecondInDay[2] = 24 * 60 * 60 - 1;
	setup_Para.ulEndSecondInDay[3] = 24 * 60 * 60 - 1;

	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = m_nCardId;
	pHead->ucCmd = MMI_CMD_WRITE_CONFIG;
	pHead->usLen = sizeof(setup_Para);
	memcpy(pHead + 1, &setup_Para, sizeof(setup_Para));
	buf[sizeof(setup_Para) + sizeof(MMI_CMD_HEAD)] = CalcChecksum(sizeof(setup_Para) + sizeof(MMI_CMD_HEAD), buf);
	int nLen = sizeof(setup_Para) + sizeof(MMI_CMD_HEAD) + 1;

	if (sysConfig.m_nCommMethod == 0)
		WriteComPort(nLen, buf);
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCmd_WriteConfig_GS7020HS()
{
	SETUP_PARA_GS7020HS setup_Para;
	BYTE buf[1024];

	memset(&setup_Para, 0, sizeof(setup_Para));

	setup_Para.usScreenWidth = editScreenWidth->Text.ToInt();
	setup_Para.usScreenHeight = editScreenHeight->Text.ToInt();
	setup_Para.ucScanMode = comboBoxScanMode->ItemIndex;
	if (checkData->Checked)
		setup_Para.ucFlags |= 0x01;
	if (checkOE->Checked)
		setup_Para.ucFlags |= 0x02;
	if (checkByteReverse->Checked)
		setup_Para.ucFlags |= 0x08;

	SYSTEMTIME st;

	DateTimeToSystemTime(timeStart0->Time, st);
	setup_Para.ucLightControlStartHour[0] = st.wHour;
	setup_Para.ucLightControlStartMinute[0] = st.wMinute;

	DateTimeToSystemTime(timeStart1->Time, st);
	setup_Para.ucLightControlStartHour[1] = st.wHour;
	setup_Para.ucLightControlStartMinute[1] = st.wMinute;

	DateTimeToSystemTime(timeStart2->Time, st);
	setup_Para.ucLightControlStartHour[2] = st.wHour;
	setup_Para.ucLightControlStartMinute[2] = st.wMinute;

	DateTimeToSystemTime(timeStart3->Time, st);
	setup_Para.ucLightControlStartHour[3] = st.wHour;
	setup_Para.ucLightControlStartMinute[3] = st.wMinute;

	setup_Para.ucLightControlValue[0] = comboBoxLightValue0->ItemIndex;
	setup_Para.ucLightControlValue[1] = comboBoxLightValue1->ItemIndex;
	setup_Para.ucLightControlValue[2] = comboBoxLightValue2->ItemIndex;
	setup_Para.ucLightControlValue[3] = comboBoxLightValue3->ItemIndex;
	setup_Para.ucLightControlValue[4] = comboBoxLightValue4->ItemIndex;
	setup_Para.ucLightControlValue[5] = comboBoxLightValue5->ItemIndex;
	setup_Para.ucLightControlValue[6] = comboBoxLightValue6->ItemIndex;
	setup_Para.ucLightControlValue[7] = comboBoxLightValue7->ItemIndex;

	strcpy(setup_Para.m_szSmsUsers[0], AnsiString(editSmsUser0->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[1], AnsiString(editSmsUser1->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[2], AnsiString(editSmsUser2->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[3], AnsiString(editSmsUser3->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[4], AnsiString(editSmsUser4->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[5], AnsiString(editSmsUser5->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[6], AnsiString(editSmsUser6->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[7], AnsiString(editSmsUser7->Text).c_str());
	setup_Para.ucReplaySms = checkEchoSms0->Checked ? 0x01 : 0;
	setup_Para.ucReplaySms |= (checkEchoSms1->Checked ? 0x02 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms2->Checked ? 0x04 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms3->Checked ? 0x08 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms4->Checked ? 0x10 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms5->Checked ? 0x20 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms6->Checked ? 0x40 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms7->Checked ? 0x80 : 0);

	setup_Para.ulShowTime = editShowTime->Text.ToInt();
	setup_Para.ucEnterStyle = comboEnterStyle->ItemIndex;
	setup_Para.ucEnterSpeed = editEnterSpeed->Text.ToInt();
	setup_Para.ucExitStyle = comboExitStyle->ItemIndex;
	setup_Para.ucExitSpeed = editExitSpeed->Text.ToInt();
	setup_Para.ucShutterWidth = 8;
	setup_Para.usStartYear = 0;
	setup_Para.ucStartMonth = 1;
	setup_Para.ucStartDay = 1;
	setup_Para.ucStartHour = 0;
	setup_Para.ucStartMinute = 0;
	setup_Para.usEndYear = 2099;
	setup_Para.ucEndMonth = 12;
	setup_Para.ucEndDay = 31;
	setup_Para.ucEndHour = 23;
	setup_Para.ucEndMinute = 59;
	setup_Para.ucAlignFlags = 0;
	setup_Para.ucRepeatCount = editRepeatCount->Text.ToInt();
	setup_Para.ucFontSize = comboFont->Text.ToInt();
	setup_Para.ulStartSecondInDay[0] = 0;
	setup_Para.ulStartSecondInDay[1] = 0;
	setup_Para.ulStartSecondInDay[2] = 0;
	setup_Para.ulStartSecondInDay[3] = 0;
	setup_Para.ulEndSecondInDay[0] = 24 * 60 * 60 - 1;
	setup_Para.ulEndSecondInDay[1] = 24 * 60 * 60 - 1;
	setup_Para.ulEndSecondInDay[2] = 24 * 60 * 60 - 1;
	setup_Para.ulEndSecondInDay[3] = 24 * 60 * 60 - 1;

	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = m_nCardId;
	pHead->ucCmd = MMI_CMD_WRITE_CONFIG;
	pHead->usLen = sizeof(setup_Para);
	memcpy(pHead + 1, &setup_Para, sizeof(setup_Para));
	buf[sizeof(setup_Para) + sizeof(MMI_CMD_HEAD)] = CalcChecksum(sizeof(setup_Para) + sizeof(MMI_CMD_HEAD), buf);
	int nLen = sizeof(setup_Para) + sizeof(MMI_CMD_HEAD) + 1;

	if (sysConfig.m_nCommMethod == 0)
		WriteComPort(nLen, buf);
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCmd_WriteConfig_GS2012AS_64()
{
	SETUP_PARA_GS2012AS_64 setup_Para;
	BYTE buf[1024];

	memset(&setup_Para, 0, sizeof(setup_Para));

	setup_Para.usScreenWidth = editScreenWidth->Text.ToInt();
	setup_Para.usScreenHeight = editScreenHeight->Text.ToInt();
	setup_Para.ucScanMode = comboBoxScanMode->ItemIndex;
	if (checkData->Checked)
		setup_Para.ucFlags |= 0x01;
	if (checkOE->Checked)
		setup_Para.ucFlags |= 0x02;
	if (checkByteReverse->Checked)
		setup_Para.ucFlags |= 0x08;
	if (rbtnDoubleColor->Checked)
		setup_Para.ucFlags |= 0x10;

	SYSTEMTIME st;

	DateTimeToSystemTime(timeStart0->Time, st);
	setup_Para.ucLightControlStartHour[0] = st.wHour;
	setup_Para.ucLightControlStartMinute[0] = st.wMinute;

	DateTimeToSystemTime(timeStart1->Time, st);
	setup_Para.ucLightControlStartHour[1] = st.wHour;
	setup_Para.ucLightControlStartMinute[1] = st.wMinute;

	DateTimeToSystemTime(timeStart2->Time, st);
	setup_Para.ucLightControlStartHour[2] = st.wHour;
	setup_Para.ucLightControlStartMinute[2] = st.wMinute;

	DateTimeToSystemTime(timeStart3->Time, st);
	setup_Para.ucLightControlStartHour[3] = st.wHour;
	setup_Para.ucLightControlStartMinute[3] = st.wMinute;

	DateTimeToSystemTime(timeStart4->Time, st);
	setup_Para.ucLightControlStartHour[4] = st.wHour;
	setup_Para.ucLightControlStartMinute[4] = st.wMinute;

	DateTimeToSystemTime(timeStart5->Time, st);
	setup_Para.ucLightControlStartHour[5] = st.wHour;
	setup_Para.ucLightControlStartMinute[5] = st.wMinute;

	DateTimeToSystemTime(timeStart6->Time, st);
	setup_Para.ucLightControlStartHour[6] = st.wHour;
	setup_Para.ucLightControlStartMinute[6] = st.wMinute;

	DateTimeToSystemTime(timeStart7->Time, st);
	setup_Para.ucLightControlStartHour[7] = st.wHour;
	setup_Para.ucLightControlStartMinute[7] = st.wMinute;

	setup_Para.ucLightControlValue[0] = comboBoxLightValue0->ItemIndex;
	setup_Para.ucLightControlValue[1] = comboBoxLightValue1->ItemIndex;
	setup_Para.ucLightControlValue[2] = comboBoxLightValue2->ItemIndex;
	setup_Para.ucLightControlValue[3] = comboBoxLightValue3->ItemIndex;
	setup_Para.ucLightControlValue[4] = comboBoxLightValue4->ItemIndex;
	setup_Para.ucLightControlValue[5] = comboBoxLightValue5->ItemIndex;
	setup_Para.ucLightControlValue[6] = comboBoxLightValue6->ItemIndex;
	setup_Para.ucLightControlValue[7] = comboBoxLightValue7->ItemIndex;

	setup_Para.usSubAreaLeft[0] = editAreaLeft0->Text.ToInt();
	setup_Para.usSubAreaLeft[1] = editAreaLeft1->Text.ToInt();
	setup_Para.usSubAreaLeft[2] = editAreaLeft2->Text.ToInt();
	setup_Para.usSubAreaLeft[3] = editAreaLeft3->Text.ToInt();
	setup_Para.usSubAreaLeft[4] = editAreaLeft4->Text.ToInt();
	setup_Para.usSubAreaLeft[5] = editAreaLeft5->Text.ToInt();
	setup_Para.usSubAreaLeft[6] = editAreaLeft6->Text.ToInt();
	setup_Para.usSubAreaLeft[7] = editAreaLeft7->Text.ToInt();

	setup_Para.usSubAreaTop[0] = editAreaTop0->Text.ToInt();
	setup_Para.usSubAreaTop[1] = editAreaTop1->Text.ToInt();
	setup_Para.usSubAreaTop[2] = editAreaTop2->Text.ToInt();
	setup_Para.usSubAreaTop[3] = editAreaTop3->Text.ToInt();
	setup_Para.usSubAreaTop[4] = editAreaTop4->Text.ToInt();
	setup_Para.usSubAreaTop[5] = editAreaTop5->Text.ToInt();
	setup_Para.usSubAreaTop[6] = editAreaTop6->Text.ToInt();
	setup_Para.usSubAreaTop[7] = editAreaTop7->Text.ToInt();

	setup_Para.usSubAreaWidth[0] = editAreaWidth0->Text.ToInt();
	setup_Para.usSubAreaWidth[1] = editAreaWidth1->Text.ToInt();
	setup_Para.usSubAreaWidth[2] = editAreaWidth2->Text.ToInt();
	setup_Para.usSubAreaWidth[3] = editAreaWidth3->Text.ToInt();
	setup_Para.usSubAreaWidth[4] = editAreaWidth4->Text.ToInt();
	setup_Para.usSubAreaWidth[5] = editAreaWidth5->Text.ToInt();
	setup_Para.usSubAreaWidth[6] = editAreaWidth6->Text.ToInt();
	setup_Para.usSubAreaWidth[7] = editAreaWidth7->Text.ToInt();

	setup_Para.usSubAreaHeight[0] = editAreaHeight0->Text.ToInt();
	setup_Para.usSubAreaHeight[1] = editAreaHeight1->Text.ToInt();
	setup_Para.usSubAreaHeight[2] = editAreaHeight2->Text.ToInt();
	setup_Para.usSubAreaHeight[3] = editAreaHeight3->Text.ToInt();
	setup_Para.usSubAreaHeight[4] = editAreaHeight4->Text.ToInt();
	setup_Para.usSubAreaHeight[5] = editAreaHeight5->Text.ToInt();
	setup_Para.usSubAreaHeight[6] = editAreaHeight6->Text.ToInt();
	setup_Para.usSubAreaHeight[7] = editAreaHeight7->Text.ToInt();

	setup_Para.ucSubAreaCount = comboBoxAreaCount->ItemIndex + 1;

	strcpy(setup_Para.m_szSmsUsers[0], AnsiString(editSmsUser0->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[1], AnsiString(editSmsUser1->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[2], AnsiString(editSmsUser2->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[3], AnsiString(editSmsUser3->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[4], AnsiString(editSmsUser4->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[5], AnsiString(editSmsUser5->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[6], AnsiString(editSmsUser6->Text).c_str());
	strcpy(setup_Para.m_szSmsUsers[7], AnsiString(editSmsUser7->Text).c_str());
	setup_Para.ucReplaySms = checkEchoSms0->Checked ? 0x01 : 0;
	setup_Para.ucReplaySms |= (checkEchoSms1->Checked ? 0x02 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms2->Checked ? 0x04 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms3->Checked ? 0x08 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms4->Checked ? 0x10 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms5->Checked ? 0x20 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms6->Checked ? 0x40 : 0);
	setup_Para.ucReplaySms |= (checkEchoSms7->Checked ? 0x80 : 0);

	setup_Para.ulShowTime = editShowTime->Text.ToInt();
	setup_Para.ucEnterStyle = comboEnterStyle->ItemIndex;
	setup_Para.ucEnterSpeed = editEnterSpeed->Text.ToInt();
	setup_Para.ucExitStyle = comboExitStyle->ItemIndex;
	setup_Para.ucExitSpeed = editExitSpeed->Text.ToInt();
	setup_Para.ucShutterWidth = 8;
	setup_Para.usStartYear = 2000;
	setup_Para.ucStartMonth = 1;
	setup_Para.ucStartDay = 1;
	setup_Para.ucStartHour = 0;
	setup_Para.ucStartMinute = 0;
	setup_Para.usEndYear = 2099;
	setup_Para.ucEndMonth = 12;
	setup_Para.ucEndDay = 31;
	setup_Para.ucEndHour = 23;
	setup_Para.ucEndMinute = 59;
	setup_Para.ucAlignFlags = 0;
	setup_Para.ucRepeatCount = editRepeatCount->Text.ToInt();
	setup_Para.ucRes4 = 0;
	setup_Para.ucFontSize = comboFont->Text.ToInt();
	setup_Para.ulStartSecondInDay[0] = 0;
	setup_Para.ulStartSecondInDay[1] = 0;
	setup_Para.ulStartSecondInDay[2] = 0;
	setup_Para.ulStartSecondInDay[3] = 0;
	setup_Para.ulEndSecondInDay[0] = 24 * 60 * 60 - 1;
	setup_Para.ulEndSecondInDay[1] = 24 * 60 * 60 - 1;
	setup_Para.ulEndSecondInDay[2] = 24 * 60 * 60 - 1;
	setup_Para.ulEndSecondInDay[3] = 24 * 60 * 60 - 1;

	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;
	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = m_nCardId;
	pHead->ucCmd = MMI_CMD_WRITE_CONFIG;
	pHead->usLen = sizeof(setup_Para);
	memcpy(pHead + 1, &setup_Para, sizeof(setup_Para));
	buf[sizeof(setup_Para) + sizeof(MMI_CMD_HEAD)] = CalcChecksum(sizeof(setup_Para) + sizeof(MMI_CMD_HEAD), buf);
	int nLen = sizeof(setup_Para) + sizeof(MMI_CMD_HEAD) + 1;

	if (sysConfig.m_nCommMethod == 0)
		WriteComPort(nLen, buf);
	else
		SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), nLen, buf);
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCmd_WriteConfig()
{
	if (sysConfig.m_nCardType == CARD_TYPE_YL2010A)
		SendCmd_WriteConfig_YL2010A();
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010A_GPS_JTX)
		SendCmd_WriteConfig_YL2010A();
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010C)
		SendCmd_WriteConfig_YL2010A();
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010AR)
		SendCmd_WriteConfig_YL2010A();
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010AF)
		SendCmd_WriteConfig_YL2010A();
	else if (sysConfig.m_nCardType == CARD_TYPE_GS7022B)
		SendCmd_WriteConfig_GS7022B();
	else if (sysConfig.m_nCardType == CARD_TYPE_GS2012A_S)
		SendCmd_WriteConfig_GS7022B();
	else if (sysConfig.m_nCardType == CARD_TYPE_PD102B)
		SendCmd_WriteConfig_PD102B();
	else if (sysConfig.m_nCardType == CARD_TYPE_WF2012B)
		SendCmd_WriteConfig_WF2012B();
	else if (sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64)
		SendCmd_WriteConfig_GS2012AS_64();
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9020S)
		SendCmd_WriteConfig_GS9020S();
	else if (sysConfig.m_nCardType == CARD_TYPE_GS7020HS)
		SendCmd_WriteConfig_GS7020HS();
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
		SendCmd_WriteConfig_GS9025();
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9023)
		SendCmd_WriteConfig_GS9025();
	else if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
		SendCmd_WriteConfig_SN2P();
	else
		SendCmd_WriteConfig_GS7020B();
}

//---------------------------------------------------------------------------
void __fastcall TfrmConfig::timerWriteConfigTimer(TObject *Sender)
{
	BYTE buf[32];

	if (sysConfig.m_nCommMethod != 0)
	{
		if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		{
			int nRxLen = ReciveUdpData(buf, sizeof(MMI_CMD_HEAD_HEB) + 1);

			if (nRxLen >= (int)(sizeof(MMI_CMD_HEAD_HEB) + 1))
			{
				MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;

				if ((pHead->usHeadTag == MMI_START_ID) &&
					 (pHead->ucCardId == m_nCardId) &&
					 (pHead->ucCmd == MMI_CMD_WRITE_CONFIG) &&
					 (pHead->ucKey1 == m_ucKey1) &&
					 (pHead->ucKey2 == m_ucKey2) &&
					 (pHead->usLen == 0))
				{
					Encrypt(pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);
					if (CalcChecksum(sizeof(MMI_CMD_HEAD_HEB), buf) == buf[sizeof(MMI_CMD_HEAD_HEB)])
					{
						timerWriteConfig->Enabled = false;
						EndComm();
						if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
							Application->MessageBox(L"写配置成功。", L"配置", MB_OK | MB_ICONINFORMATION);
						else
							Application->MessageBox(L"Write Card Settings OK。", L"Card Configuration", MB_OK | MB_ICONINFORMATION);
						return;
					}
				}
			}
		}
		else
		{
			int nRxLen = ReciveUdpData(buf, sizeof(MMI_CMD_HEAD) + 1);

			if (nRxLen >= (int)(sizeof(MMI_CMD_HEAD) + 1))
			{
				MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

				if ((pHead->usHeadTag == MMI_START_ID) &&
					 (pHead->ucCardId == m_nCardId) &&
					 (pHead->ucCmd == MMI_CMD_WRITE_CONFIG) &&
					 (pHead->usLen == 0))
				{
					if (CalcChecksum(sizeof(MMI_CMD_HEAD), buf) == buf[sizeof(MMI_CMD_HEAD)])
					{
						timerWriteConfig->Enabled = false;
						EndComm();
						if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
							Application->MessageBox(L"写配置成功。", L"配置", MB_OK | MB_ICONINFORMATION);
						else
							Application->MessageBox(L"Write Card Settings OK。", L"Card Configuration", MB_OK | MB_ICONINFORMATION);

						switch (sysConfig.m_nCardType)
						{
						case CARD_TYPE_YL2010A:
						case CARD_TYPE_YL2010A_GPS_JTX:
						case CARD_TYPE_YL2010C:
						case CARD_TYPE_YL2010AR:
						case CARD_TYPE_YL2010AF:
						case CARD_TYPE_PD102B:
							editCardId->Text = editNewCardId->Text;
							break;
						case CARD_TYPE_SN2P:
							editLocalIp->Text = editNewLocalIp->Text;
							break;
						}
						return;
					}
				}
			}
		}

		m_nTimeout += timerReadConfig->Interval;
		int nMaxTime = sysConfig.m_nTimeout_Network * 1000;
		if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
			nMaxTime = 200;
		if (m_nTimeout >= nMaxTime)
		{
			m_nRetries++;
			if (m_nRetries < sysConfig.m_nRetry_Network)
			{
				m_nTimeout = 0;
				SendCmd_WriteConfig();
			}
			else
			{
				timerWriteConfig->Enabled = false;
				EndComm();
				if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
					Application->MessageBox(L"写配置失败!", L"配置", MB_OK | MB_ICONWARNING);
				else
					Application->MessageBox(L"Failed to Write Card Setings!", L"Card Configuration", MB_OK | MB_ICONWARNING);
			}
		}

		return;
	}

	while (m_ComPort.GetInQueue())
	{
		switch (m_nRxStep)
		{
		case 0:
			m_ComPort.Read(1, buf);
			if (buf[0] == MMI_START_ID1)
				m_nRxStep = 1;
			break;
		case 1:
			m_ComPort.Read(1, buf + 1);
			if (buf[1] == MMI_START_ID2)
				m_nRxStep = 2;
			else
				m_nRxStep = 0;
			break;
		default:
			if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
			{
				if (m_ComPort.GetInQueue() >= (int)(sizeof(MMI_CMD_HEAD_HEB) - 1))
				{
					m_ComPort.Read(sizeof(MMI_CMD_HEAD_HEB) - 1, buf + 2);
					MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;

					if ((pHead->usHeadTag == MMI_START_ID) &&
						 (pHead->ucCardId == m_nCardId) &&
						 (pHead->ucCmd == MMI_CMD_WRITE_CONFIG) &&
						 (pHead->ucKey1 == m_ucKey1) &&
						 (pHead->ucKey2 == m_ucKey2) &&
						 (pHead->usLen == 0))
					{
						Encrypt(pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);
						if (CalcChecksum(sizeof(MMI_CMD_HEAD_HEB), buf) == buf[sizeof(MMI_CMD_HEAD_HEB)])
						{
							timerWriteConfig->Enabled = false;
							EndComm();
							if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
								Application->MessageBox(L"写配置成功。", L"配置", MB_OK | MB_ICONINFORMATION);
							else
								Application->MessageBox(L"Write Card Settings OK。", L"Card Configuration", MB_OK | MB_ICONINFORMATION);
							return;
						}
					}
					else
						m_nRxStep = 0;
				}
			}
			else
			{
				if (m_ComPort.GetInQueue() >= (int)(sizeof(MMI_CMD_HEAD) - 1))
				{
					m_ComPort.Read(sizeof(MMI_CMD_HEAD) - 1, buf + 2);
					MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

					if ((pHead->usHeadTag == MMI_START_ID) &&
						 (pHead->ucCardId == m_nCardId) &&
						 (pHead->ucCmd == MMI_CMD_WRITE_CONFIG) &&
						 (pHead->usLen == 0))
					{
						if (CalcChecksum(sizeof(MMI_CMD_HEAD), buf) == buf[sizeof(MMI_CMD_HEAD)])
						{
							timerWriteConfig->Enabled = false;
							EndComm();
							if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
								Application->MessageBox(L"写配置成功。", L"配置", MB_OK | MB_ICONINFORMATION);
							else
								Application->MessageBox(L"Write Card Settings OK。", L"Card Configuration", MB_OK | MB_ICONINFORMATION);

							switch (sysConfig.m_nCardType)
							{
							case CARD_TYPE_YL2010A:
							case CARD_TYPE_YL2010A_GPS_JTX:
							case CARD_TYPE_YL2010C:
							case CARD_TYPE_YL2010AR:
							case CARD_TYPE_YL2010AF:
							case CARD_TYPE_PD102B:
								editCardId->Text = editNewCardId->Text;
								break;
							case CARD_TYPE_SN2P:
								editLocalIp->Text = editNewLocalIp->Text;
								break;
							}
							return;
						}
					}
					else
						m_nRxStep = 0;
				}
			}
			break;
		}
	}

	m_nTimeout += timerWriteConfig->Interval;
	if (m_nTimeout > sysConfig.m_nTimeout_SerialPort)
	{
		timerWriteConfig->Enabled = false;
		EndComm();
		if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
			Application->MessageBox(L"写配置失败!", L"配置", MB_OK | MB_ICONWARNING);
		else
			Application->MessageBox(L"Failed to Write Card Setings!", L"Card Configuration", MB_OK | MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::btnReadConfigClick(TObject *Sender)
{
	m_nCardId = editCardId->Text.ToInt();

	if (!OpenComm())
		return;

	::Sleep(20);
	SendCmd_ReadConfig();

	StartComm();
	timerReadConfig->Enabled = true;
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCmd_ReadConfig()
{
	BYTE buf[32];

	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
	{
		MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;

		pHead->usHeadTag = MMI_START_ID;
		pHead->ucCardId = m_nCardId;
		pHead->ucCmd = MMI_CMD_READ_CONFIG;
		pHead->usLen = 0;
		pHead->ucKey1 = m_ucKey1 = GetTickCount();
		pHead->ucKey2 = m_ucKey2 = GetTickCount() >> 8;
		buf[sizeof(MMI_CMD_HEAD_HEB)] = CalcChecksum(sizeof(MMI_CMD_HEAD_HEB), buf);
		Encrypt(pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);

		if (sysConfig.m_nCommMethod == 0)
			m_ComPort.Write(sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
		else
			SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
	}
	else
	{
		MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

		pHead->usHeadTag = MMI_START_ID;
		pHead->ucCardId = m_nCardId;
		pHead->ucCmd = MMI_CMD_READ_CONFIG;
		pHead->usLen = 0;
		buf[sizeof(MMI_CMD_HEAD)] = CalcChecksum(sizeof(MMI_CMD_HEAD), buf);

		if (sysConfig.m_nCommMethod == 0)
			m_ComPort.Write(sizeof(MMI_CMD_HEAD) + 1, buf);
		else
			SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::timerReadConfigTimer(TObject *Sender)
{
	BYTE buf[1024];
	int nSeup_ParaLen;

	if (sysConfig.m_nCardType == CARD_TYPE_YL2010A)
		nSeup_ParaLen = sizeof(SETUP_PARA_YL2010A);
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010A_GPS_JTX)
		nSeup_ParaLen = sizeof(SETUP_PARA_YL2010A);
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010C)
		nSeup_ParaLen = sizeof(SETUP_PARA_YL2010A);
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010AR)
		nSeup_ParaLen = sizeof(SETUP_PARA_YL2010A);
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010AF)
		nSeup_ParaLen = sizeof(SETUP_PARA_YL2010A);
	else if (sysConfig.m_nCardType == CARD_TYPE_GS7022B)
		nSeup_ParaLen = sizeof(SETUP_PARA_GS7022B);
	else if (sysConfig.m_nCardType == CARD_TYPE_GS2012A_S)
		nSeup_ParaLen = sizeof(SETUP_PARA_GS7022B);
	else if (sysConfig.m_nCardType == CARD_TYPE_PD102B)
		nSeup_ParaLen = sizeof(SETUP_PARA_PD102B);
	else if (sysConfig.m_nCardType == CARD_TYPE_WF2012B)
		nSeup_ParaLen = sizeof(SETUP_PARA_WF2012B);
	else if (sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64)
		nSeup_ParaLen = sizeof(SETUP_PARA_GS2012AS_64);
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9020S)
		nSeup_ParaLen = sizeof(SETUP_PARA_GS9020S);
	else if (sysConfig.m_nCardType == CARD_TYPE_GS7020HS)
		nSeup_ParaLen = sizeof(SETUP_PARA_GS7020HS);
	else if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
		nSeup_ParaLen = sizeof(SETUP_PARA_SN2P);
	else
		nSeup_ParaLen = sizeof(SETUP_PARA_GS7020B);

	if (sysConfig.m_nCommMethod != 0)
	{
		if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		{
			int nRxLen = ReciveUdpData(buf, sizeof(MMI_CMD_HEAD_HEB) + nSeup_ParaLen + 1);
			if (nRxLen >= (int)(sizeof(MMI_CMD_HEAD_HEB) + nSeup_ParaLen + 1))
			{
				MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;

				if ((pHead->usHeadTag == MMI_START_ID) &&
					(pHead->ucCardId == m_nCardId) &&
					(pHead->ucCmd == MMI_CMD_READ_CONFIG) &&
					(pHead->ucKey1 == m_ucKey1) &&
					(pHead->ucKey2 == m_ucKey2) &&
					(pHead->usLen == nSeup_ParaLen))
				{
					Encrypt(pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);
					if (CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + nSeup_ParaLen, buf) == buf[sizeof(MMI_CMD_HEAD_HEB) + nSeup_ParaLen])
					{
						timerReadConfig->Enabled = false;
						EndComm();

						UpdateConfigData(pHead + 1);
						if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
							Application->MessageBox(L"读配置成功。", L"配置", MB_OK | MB_ICONINFORMATION);
						else
							Application->MessageBox(L"Read Card Settings OK。", L"Card Configuration", MB_OK | MB_ICONINFORMATION);
						return;
					}
				}
			}
		}
		else
		{
			int nRxLen = ReciveUdpData(buf, sizeof(MMI_CMD_HEAD) + nSeup_ParaLen + 1);

			if (nRxLen >= (int)(sizeof(MMI_CMD_HEAD) + nSeup_ParaLen + 1))
			{
				MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

				if ((pHead->usHeadTag == MMI_START_ID) &&
					(pHead->ucCardId == m_nCardId) &&
					(pHead->ucCmd == MMI_CMD_READ_CONFIG) &&
					(pHead->usLen == nSeup_ParaLen))
				{
					if (CalcChecksum(sizeof(MMI_CMD_HEAD) + nSeup_ParaLen, buf) == buf[sizeof(MMI_CMD_HEAD) + nSeup_ParaLen])
					{
						timerReadConfig->Enabled = false;
						EndComm();

						UpdateConfigData(pHead + 1);
						if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
							Application->MessageBox(L"读配置成功。", L"配置", MB_OK | MB_ICONINFORMATION);
						else
							Application->MessageBox(L"Read Card Settings OK。", L"Card Configuration", MB_OK | MB_ICONINFORMATION);
						return;
					}
				}
			}
		}

		m_nTimeout += timerReadConfig->Interval;
		int nMaxTime = sysConfig.m_nTimeout_Network * 1000;
		if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
			nMaxTime = 200;
		if (m_nTimeout >= nMaxTime)
		{
			m_nRetries++;
			if (m_nRetries < sysConfig.m_nRetry_Network)
			{
				m_nTimeout = 0;
				SendCmd_ReadConfig();
			}
			else
			{
				timerReadConfig->Enabled = false;
				EndComm();
				if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
					Application->MessageBox(L"读配置失败!", L"配置", MB_OK | MB_ICONWARNING);
				else
					Application->MessageBox(L"Failed to Read Card Setings!", L"Card Configuration", MB_OK | MB_ICONWARNING);
			}
		}

		return;
	}

	while (m_ComPort.GetInQueue())
	{
		switch (m_nRxStep)
		{
		case 0:
			m_ComPort.Read(1, m_dtu->m_ucRxBuf);
			if (m_dtu->m_ucRxBuf[0] == MMI_START_ID1)
				m_nRxStep = 1;
			break;
		case 1:
			m_ComPort.Read(1, m_dtu->m_ucRxBuf + 1);
			if (m_dtu->m_ucRxBuf[1] == MMI_START_ID2)
				m_nRxStep = 2;
			else
				m_nRxStep = 0;
			break;
		default:
			if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
			{
				if (m_ComPort.GetInQueue() >= (int)(sizeof(MMI_CMD_HEAD_HEB) + nSeup_ParaLen - 1))
				{
					buf[0] = m_dtu->m_ucRxBuf[0];
					buf[1] = m_dtu->m_ucRxBuf[1];
					m_ComPort.Read(sizeof(MMI_CMD_HEAD_HEB) + nSeup_ParaLen - 1, buf + 2);
					MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;

					if ((pHead->usHeadTag == MMI_START_ID) &&
						 (pHead->ucCardId == m_nCardId) &&
						 (pHead->ucCmd == MMI_CMD_READ_CONFIG) &&
						 (pHead->ucKey1 == m_ucKey1) &&
						 (pHead->ucKey2 == m_ucKey2) &&
						 (pHead->usLen == nSeup_ParaLen))
					{
						Encrypt(pHead->usLen + 1, &buf[sizeof(MMI_CMD_HEAD_HEB)]);
						if (CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + nSeup_ParaLen, buf) == buf[sizeof(MMI_CMD_HEAD_HEB) + nSeup_ParaLen])
						{
							timerReadConfig->Enabled = false;
							EndComm();

							UpdateConfigData(pHead + 1);
							if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
								Application->MessageBox(L"读配置成功。", L"配置", MB_OK | MB_ICONINFORMATION);
							else
								Application->MessageBox(L"Read Card Settings OK。", L"Card Configuration", MB_OK | MB_ICONINFORMATION);
							return;
						}
						else
							m_nRxStep = 0;
					}
				}
			}
			else
			{
				if (m_ComPort.GetInQueue() >= (int)(sizeof(MMI_CMD_HEAD) + nSeup_ParaLen - 1))
				{
					buf[0] = m_dtu->m_ucRxBuf[0];
					buf[1] = m_dtu->m_ucRxBuf[1];
					m_ComPort.Read(sizeof(MMI_CMD_HEAD) + nSeup_ParaLen - 1, buf + 2);
					MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

					if ((pHead->usHeadTag == MMI_START_ID) &&
						 (pHead->ucCardId == m_nCardId) &&
						 (pHead->ucCmd == MMI_CMD_READ_CONFIG) &&
						 (pHead->usLen == nSeup_ParaLen))
					{
						if (CalcChecksum(sizeof(MMI_CMD_HEAD) + nSeup_ParaLen, buf) == buf[sizeof(MMI_CMD_HEAD) + nSeup_ParaLen])
						{
							timerReadConfig->Enabled = false;
							EndComm();

							UpdateConfigData(pHead + 1);
							if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
								Application->MessageBox(L"读配置成功。", L"配置", MB_OK | MB_ICONINFORMATION);
							else
								Application->MessageBox(L"Read Card Settings OK。", L"Card Configuration", MB_OK | MB_ICONINFORMATION);
							return;
						}
						else
							m_nRxStep = 0;
					}
				}
			}
			break;
		}
		if (m_nRxStep == 2)
			break;
	}

	m_nTimeout += timerReadConfig->Interval;
	if (m_nTimeout >= sysConfig.m_nTimeout_SerialPort)
	{
		m_nRetries++;
		if (m_nRetries < sysConfig.m_nRetry_SerialPort)
		{
			m_nTimeout = 0;
			SendCmd_ReadConfig();
		}
		else
		{
			timerReadConfig->Enabled = false;
			EndComm();
			if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
				Application->MessageBox(L"读配置失败!", L"配置", MB_OK | MB_ICONWARNING);
			else
				Application->MessageBox(L"Failed to Read Card Setings!", L"Card Configuration", MB_OK | MB_ICONWARNING);
		}
	}
}
//---------------------------------------------------------------------------

void TfrmConfig::UpdateConfigData_YL2010A(void* p)
{
	SETUP_PARA_YL2010A* pSetup_Para = (SETUP_PARA_YL2010A*)p;;

	comboBoxScanMode->ItemIndex = (pSetup_Para->ulScanMode & 0xFF);
	checkByteReverse->Checked = (pSetup_Para->ulScanMode & 0x40000000);
	checkData->Checked = pSetup_Para->bNegData;
	checkOE->Checked = pSetup_Para->bNegOE;
	if (pSetup_Para->bFullColor)
		rbtnFullColor->Checked = true;
	else
	{
		if (pSetup_Para->bDoubleColor)
			rbtnDoubleColor->Checked = true;
		else
			rbtnSingleColor->Checked = true;
	}
	checkSupportWarnning->Checked = pSetup_Para->bSupportWarnning;
	checkSupportInsertPlay->Checked = pSetup_Para->bSupportInsertPlay;
	checkSyncPlay->Checked = pSetup_Para->bActSupportSync;
	checkActSupportRepeat->Checked = pSetup_Para->bActSupportRepeat;
	editScreenWidth->Text = pSetup_Para->usScreenWidth;
	editScreenHeight->Text = pSetup_Para->usScreenHeight;
	comboBoxMaxActCount->ItemIndex = 0;
	for (int i = 0; i < m_nSelectActCountTableSize; i++)
	{
		if (pSetup_Para->ucMaxActCount == m_nSelectActCountTable[i])
		{
			comboBoxMaxActCount->ItemIndex = i;
			break;
		}
	}

	editNewCardId->Text = pSetup_Para->ucCardId;
	switch (pSetup_Para->ulBaudrate)
	{
	case 115200:
		cbBaudrate->ItemIndex = 0; break;
	case 57600:
		cbBaudrate->ItemIndex = 1; break;
	case 38400:
		cbBaudrate->ItemIndex = 2; break;
	case 19200:
		cbBaudrate->ItemIndex = 3; break;
	case 9600:
		cbBaudrate->ItemIndex = 4; break;
	case 4800:
		cbBaudrate->ItemIndex = 5; break;
	case 2400:
		cbBaudrate->ItemIndex = 6; break;
	default:
		cbBaudrate->ItemIndex = 0; break;
	}

	SYSTEMTIME st;
	::GetLocalTime(&st);
	st.wSecond = 0;
	st.wMilliseconds = 0;

	st.wHour = pSetup_Para->ucLightControlStartHour[0];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[0];
	timeStart0->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[1];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[1];
	timeStart1->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[2];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[2];
	timeStart2->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[3];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[3];
	timeStart3->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[4];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[4];
	timeStart4->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[5];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[5];
	timeStart5->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[6];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[6];
	timeStart6->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[7];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[7];
	timeStart7->Time = SystemTimeToDateTime(st);

	comboBoxLightValue0->ItemIndex = pSetup_Para->ucLightControlValue[0];
	comboBoxLightValue1->ItemIndex = pSetup_Para->ucLightControlValue[1];
	comboBoxLightValue2->ItemIndex = pSetup_Para->ucLightControlValue[2];
	comboBoxLightValue3->ItemIndex = pSetup_Para->ucLightControlValue[3];
	comboBoxLightValue4->ItemIndex = pSetup_Para->ucLightControlValue[4];
	comboBoxLightValue5->ItemIndex = pSetup_Para->ucLightControlValue[5];
	comboBoxLightValue6->ItemIndex = pSetup_Para->ucLightControlValue[6];
	comboBoxLightValue7->ItemIndex = pSetup_Para->ucLightControlValue[7];

	editAreaLeft0->Text = pSetup_Para->usSubAreaLeft[0];
	editAreaLeft1->Text = pSetup_Para->usSubAreaLeft[1];
	editAreaLeft2->Text = pSetup_Para->usSubAreaLeft[2];
	editAreaLeft3->Text = pSetup_Para->usSubAreaLeft[3];
	editAreaLeft4->Text = pSetup_Para->usSubAreaLeft[4];
	editAreaLeft5->Text = pSetup_Para->usSubAreaLeft[5];
	editAreaLeft6->Text = pSetup_Para->usSubAreaLeft[6];
	editAreaLeft7->Text = pSetup_Para->usSubAreaLeft[7];

	editAreaTop0->Text = pSetup_Para->usSubAreaTop[0];
	editAreaTop1->Text = pSetup_Para->usSubAreaTop[1];
	editAreaTop2->Text = pSetup_Para->usSubAreaTop[2];
	editAreaTop3->Text = pSetup_Para->usSubAreaTop[3];
	editAreaTop4->Text = pSetup_Para->usSubAreaTop[4];
	editAreaTop5->Text = pSetup_Para->usSubAreaTop[5];
	editAreaTop6->Text = pSetup_Para->usSubAreaTop[6];
	editAreaTop7->Text = pSetup_Para->usSubAreaTop[7];

	editAreaWidth0->Text = pSetup_Para->usSubAreaWidth[0];
	editAreaWidth1->Text = pSetup_Para->usSubAreaWidth[1];
	editAreaWidth2->Text = pSetup_Para->usSubAreaWidth[2];
	editAreaWidth3->Text = pSetup_Para->usSubAreaWidth[3];
	editAreaWidth4->Text = pSetup_Para->usSubAreaWidth[4];
	editAreaWidth5->Text = pSetup_Para->usSubAreaWidth[5];
	editAreaWidth6->Text = pSetup_Para->usSubAreaWidth[6];
	editAreaWidth7->Text = pSetup_Para->usSubAreaWidth[7];

	editAreaHeight0->Text = pSetup_Para->usSubAreaHeight[0];
	editAreaHeight1->Text = pSetup_Para->usSubAreaHeight[1];
	editAreaHeight2->Text = pSetup_Para->usSubAreaHeight[2];
	editAreaHeight3->Text = pSetup_Para->usSubAreaHeight[3];
	editAreaHeight4->Text = pSetup_Para->usSubAreaHeight[4];
	editAreaHeight5->Text = pSetup_Para->usSubAreaHeight[5];
	editAreaHeight6->Text = pSetup_Para->usSubAreaHeight[6];
	editAreaHeight7->Text = pSetup_Para->usSubAreaHeight[7];

	comboBoxAreaCount->ItemIndex = pSetup_Para->ucSubAreaCount - 1;
	comboBoxAreaCountChange(this);
	DrawAreaImage();

	switch (sysConfig.m_nCardType)
	{
	case CARD_TYPE_YL2010A:
	case CARD_TYPE_YL2010A_GPS_JTX:
	case CARD_TYPE_YL2010C:
	case CARD_TYPE_YL2010AR:
	case CARD_TYPE_YL2010AF:
		checkActSecond->Checked = true;
		break;
	case CARD_TYPE_YL2010T:
		checkActSecond->Enabled = false;
		checkActSupportRepeat->Enabled = false;
		checkSupportInsertPlay->Enabled = false;
		checkSyncPlay->Enabled = false;
		checkActSecond->Checked = true;
		checkActSupportRepeat->Checked = false;
		checkSupportInsertPlay->Checked = false;
		checkSyncPlay->Checked = false;
		break;
	}
}
//---------------------------------------------------------------------------

void TfrmConfig::UpdateConfigData_GS7020B(void* p)
{
	SETUP_PARA_GS7020B* pSetup_Para = (SETUP_PARA_GS7020B*)p;

	comboBoxScanMode->ItemIndex = (pSetup_Para->ulScanMode & 0xFF);
	checkByteReverse->Checked = (pSetup_Para->ulScanMode & 0x40000000);
	editSocketPort_Card->Text = pSetup_Para->usSocketPort;
	editDeviceCode->Text = pSetup_Para->szDeviceCode;
	editAPN->Text = pSetup_Para->szAPN;
	editDataCenter->Text = pSetup_Para->szDataCenter;
	checkSyncPlay->Checked = pSetup_Para->bActSupportSync;

	if (sysConfig.m_nCardType == CARD_TYPE_GS9020 ||
		sysConfig.m_nCardType == CARD_TYPE_GS7020B ||
		sysConfig.m_nCardType == CARD_TYPE_GS7020H ||
		sysConfig.m_nCardType == CARD_TYPE_GS9025 ||
		sysConfig.m_nCardType == CARD_TYPE_GS9023 ||
		sysConfig.m_nCardType == CARD_TYPE_GS2012A_S ||
		sysConfig.m_nCardType == CARD_TYPE_GS2012A
		)
		checkTCP->Checked = pSetup_Para->bTCP;
	else
		checkTCP->Checked = false;

	//if (sysConfig.m_nCardType == CARD_TYPE_GS7020B)
	//	checkLoginEcho->Checked = pSetup_Para->bWaitFistLoginEcho;
	//else
	checkLoginEcho->Checked = false;
	checkLoginEcho->Visible = false;

	switch (sysConfig.m_nCardType)
	{
	case CARD_TYPE_GS7020BF:
	case CARD_TYPE_GS7020BF_HEB:
	case CARD_TYPE_GS9026:
	case CARD_TYPE_GS7020BF_W:
	case CARD_TYPE_GS9020F_GPS:
	case CARD_TYPE_GS7020HFA:
		checkActSecond->Checked = true;
		checkActSupportRepeat->Checked = true;
		checkSupportInsertPlay->Checked = true;
		break;
	default:
		checkActSecond->Checked = pSetup_Para->bActSecond;
		checkActSupportRepeat->Checked = pSetup_Para->bActSupportRepeat;
		checkSupportInsertPlay->Checked = pSetup_Para->bSupportInsertPlay;
		break;
	}
	checkSupportLedSignCheck->Checked = pSetup_Para->bSupport_LedSign_Check;
	checkSyncTimeHint->Checked = pSetup_Para->bActSupportCycleTime;
	checkData->Checked = pSetup_Para->bNegData;
	checkOE->Checked = pSetup_Para->bNegOE;
	if (pSetup_Para->bFullColor)
		rbtnFullColor->Checked = true;
	else
	{
		if (pSetup_Para->bDoubleColor)
			rbtnDoubleColor->Checked = true;
		else
			rbtnSingleColor->Checked = true;
    }
	checkSupportWarnning->Checked = pSetup_Para->bSupportWarnning;
	editScreenWidth->Text = pSetup_Para->usScreenWidth;
	editScreenHeight->Text = pSetup_Para->usScreenHeight;
	comboBoxMaxActCount->ItemIndex = 0;
	for (int i = 0; i < m_nSelectActCountTableSize; i++)
	{
		if (pSetup_Para->ucMaxActCount == m_nSelectActCountTable[i])
		{
			comboBoxMaxActCount->ItemIndex = i;
			break;
		}
	}

	SYSTEMTIME st;
	::GetLocalTime(&st);
	st.wSecond = 0;
	st.wMilliseconds = 0;

	st.wHour = pSetup_Para->ucLightControlStartHour[0];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[0];
	timeStart0->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[1];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[1];
	timeStart1->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[2];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[2];
	timeStart2->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[3];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[3];
	timeStart3->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[4];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[4];
	timeStart4->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[5];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[5];
	timeStart5->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[6];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[6];
	timeStart6->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[7];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[7];
	timeStart7->Time = SystemTimeToDateTime(st);

	comboBoxLightValue0->ItemIndex = pSetup_Para->ucLightControlValue[0];
	comboBoxLightValue1->ItemIndex = pSetup_Para->ucLightControlValue[1];
	comboBoxLightValue2->ItemIndex = pSetup_Para->ucLightControlValue[2];
	comboBoxLightValue3->ItemIndex = pSetup_Para->ucLightControlValue[3];
	comboBoxLightValue4->ItemIndex = pSetup_Para->ucLightControlValue[4];
	comboBoxLightValue5->ItemIndex = pSetup_Para->ucLightControlValue[5];
	comboBoxLightValue6->ItemIndex = pSetup_Para->ucLightControlValue[6];
	comboBoxLightValue7->ItemIndex = pSetup_Para->ucLightControlValue[7];

	editAreaLeft0->Text = pSetup_Para->usSubAreaLeft[0];
	editAreaLeft1->Text = pSetup_Para->usSubAreaLeft[1];
	editAreaLeft2->Text = pSetup_Para->usSubAreaLeft[2];
	editAreaLeft3->Text = pSetup_Para->usSubAreaLeft[3];
	editAreaLeft4->Text = pSetup_Para->usSubAreaLeft[4];
	editAreaLeft5->Text = pSetup_Para->usSubAreaLeft[5];
	editAreaLeft6->Text = pSetup_Para->usSubAreaLeft[6];
	editAreaLeft7->Text = pSetup_Para->usSubAreaLeft[7];

	editAreaTop0->Text = pSetup_Para->usSubAreaTop[0];
	editAreaTop1->Text = pSetup_Para->usSubAreaTop[1];
	editAreaTop2->Text = pSetup_Para->usSubAreaTop[2];
	editAreaTop3->Text = pSetup_Para->usSubAreaTop[3];
	editAreaTop4->Text = pSetup_Para->usSubAreaTop[4];
	editAreaTop5->Text = pSetup_Para->usSubAreaTop[5];
	editAreaTop6->Text = pSetup_Para->usSubAreaTop[6];
	editAreaTop7->Text = pSetup_Para->usSubAreaTop[7];

	editAreaWidth0->Text = pSetup_Para->usSubAreaWidth[0];
	editAreaWidth1->Text = pSetup_Para->usSubAreaWidth[1];
	editAreaWidth2->Text = pSetup_Para->usSubAreaWidth[2];
	editAreaWidth3->Text = pSetup_Para->usSubAreaWidth[3];
	editAreaWidth4->Text = pSetup_Para->usSubAreaWidth[4];
	editAreaWidth5->Text = pSetup_Para->usSubAreaWidth[5];
	editAreaWidth6->Text = pSetup_Para->usSubAreaWidth[6];
	editAreaWidth7->Text = pSetup_Para->usSubAreaWidth[7];

	editAreaHeight0->Text = pSetup_Para->usSubAreaHeight[0];
	editAreaHeight1->Text = pSetup_Para->usSubAreaHeight[1];
	editAreaHeight2->Text = pSetup_Para->usSubAreaHeight[2];
	editAreaHeight3->Text = pSetup_Para->usSubAreaHeight[3];
	editAreaHeight4->Text = pSetup_Para->usSubAreaHeight[4];
	editAreaHeight5->Text = pSetup_Para->usSubAreaHeight[5];
	editAreaHeight6->Text = pSetup_Para->usSubAreaHeight[6];
	editAreaHeight7->Text = pSetup_Para->usSubAreaHeight[7];

	comboBoxAreaCount->ItemIndex = pSetup_Para->ucSubAreaCount - 1;
	comboBoxAreaCountChange(this);
	DrawAreaImage();

	switch (pSetup_Para->ulUserBaudRate)
	{
	case 115200:
		cbBaudrate->ItemIndex = 0;
		break;
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
}
//---------------------------------------------------------------------------

void TfrmConfig::UpdateConfigData_SN2P(void* p)
{
	SETUP_PARA_SN2P* pSetup_Para = (SETUP_PARA_SN2P*)p;

	comboBoxScanMode->ItemIndex = (pSetup_Para->ulScanMode & 0xFF);
	checkByteReverse->Checked = (pSetup_Para->ulScanMode & 0x40000000);
	editSocketPort_Card->Text = pSetup_Para->usLocalSocketPort;
	checkData->Checked = pSetup_Para->bNegData;
	checkOE->Checked = pSetup_Para->bNegOE;
	if (pSetup_Para->bDoubleColor)
		rbtnDoubleColor->Checked = true;
	else
		rbtnSingleColor->Checked = true;
	editScreenWidth->Text = pSetup_Para->usScreenWidth;
	editScreenHeight->Text = pSetup_Para->usScreenHeight;

	char szTemp[32];
	sprintf(szTemp, "%d.%d.%d.%d", pSetup_Para->ucLocalIp[0], pSetup_Para->ucLocalIp[1], pSetup_Para->ucLocalIp[2], pSetup_Para->ucLocalIp[3]);
	editLocalIp->Text = szTemp;
	editNewLocalIp->Text = szTemp;
	sprintf(szTemp, "%d.%d.%d.%d", pSetup_Para->ucGateway[0], pSetup_Para->ucGateway[1], pSetup_Para->ucGateway[2], pSetup_Para->ucGateway[3]);
	editGateway->Text = szTemp;
	sprintf(szTemp, "%d.%d.%d.%d", pSetup_Para->ucNetMask[0], pSetup_Para->ucNetMask[1], pSetup_Para->ucNetMask[2], pSetup_Para->ucNetMask[3]);
	editNetMask->Text = szTemp;
}
//---------------------------------------------------------------------------

void TfrmConfig::UpdateConfigData_WF2012B(void* p)
{
	SETUP_PARA_WF2012B* pSetup_Para = (SETUP_PARA_WF2012B*)p;

	comboBoxScanMode->ItemIndex = (pSetup_Para->ulScanMode & 0xFF);
	checkByteReverse->Checked = (pSetup_Para->ulScanMode & 0x40000000);
	editWifiSocketPort_Card->Text = pSetup_Para->usSocketPort;
	editWifiDeviceCode->Text = pSetup_Para->szDeviceCode;
	editSSID->Text = pSetup_Para->szSSID;
	editWifiPassword->Text = pSetup_Para->szPassword;
	editWifiServer->Text = pSetup_Para->szDataCenter;
	editWifiDnsServer->Text = pSetup_Para->szDnsServer;
	checkActSecond->Checked = pSetup_Para->bActSecond;
	checkActSupportRepeat->Checked = pSetup_Para->bActSupportRepeat;
	checkSupportInsertPlay->Checked = pSetup_Para->bSupportInsertPlay;
	checkData->Checked = pSetup_Para->bNegData;
	checkOE->Checked = pSetup_Para->bNegOE;
	if (pSetup_Para->bDoubleColor)
		rbtnDoubleColor->Checked = true;
	else
		rbtnSingleColor->Checked = true;
	checkSupportWarnning->Checked = pSetup_Para->bSupportWarnning;
	editScreenWidth->Text = pSetup_Para->usScreenWidth;
	editScreenHeight->Text = pSetup_Para->usScreenHeight;
	comboBoxMaxActCount->ItemIndex = 0;
	for (int i = 0; i < m_nSelectActCountTableSize; i++)
	{
		if (pSetup_Para->ucMaxActCount == m_nSelectActCountTable[i])
		{
			comboBoxMaxActCount->ItemIndex = i;
			break;
		}
	}

	SYSTEMTIME st;
	::GetLocalTime(&st);
	st.wSecond = 0;
	st.wMilliseconds = 0;

	st.wHour = pSetup_Para->ucLightControlStartHour[0];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[0];
	timeStart0->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[1];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[1];
	timeStart1->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[2];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[2];
	timeStart2->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[3];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[3];
	timeStart3->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[4];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[4];
	timeStart4->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[5];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[5];
	timeStart5->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[6];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[6];
	timeStart6->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[7];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[7];
	timeStart7->Time = SystemTimeToDateTime(st);

	comboBoxLightValue0->ItemIndex = pSetup_Para->ucLightControlValue[0];
	comboBoxLightValue1->ItemIndex = pSetup_Para->ucLightControlValue[1];
	comboBoxLightValue2->ItemIndex = pSetup_Para->ucLightControlValue[2];
	comboBoxLightValue3->ItemIndex = pSetup_Para->ucLightControlValue[3];
	comboBoxLightValue4->ItemIndex = pSetup_Para->ucLightControlValue[4];
	comboBoxLightValue5->ItemIndex = pSetup_Para->ucLightControlValue[5];
	comboBoxLightValue6->ItemIndex = pSetup_Para->ucLightControlValue[6];
	comboBoxLightValue7->ItemIndex = pSetup_Para->ucLightControlValue[7];

	editAreaLeft0->Text = pSetup_Para->usSubAreaLeft[0];
	editAreaLeft1->Text = pSetup_Para->usSubAreaLeft[1];
	editAreaLeft2->Text = pSetup_Para->usSubAreaLeft[2];
	editAreaLeft3->Text = pSetup_Para->usSubAreaLeft[3];
	editAreaLeft4->Text = pSetup_Para->usSubAreaLeft[4];
	editAreaLeft5->Text = pSetup_Para->usSubAreaLeft[5];
	editAreaLeft6->Text = pSetup_Para->usSubAreaLeft[6];
	editAreaLeft7->Text = pSetup_Para->usSubAreaLeft[7];

	editAreaTop0->Text = pSetup_Para->usSubAreaTop[0];
	editAreaTop1->Text = pSetup_Para->usSubAreaTop[1];
	editAreaTop2->Text = pSetup_Para->usSubAreaTop[2];
	editAreaTop3->Text = pSetup_Para->usSubAreaTop[3];
	editAreaTop4->Text = pSetup_Para->usSubAreaTop[4];
	editAreaTop5->Text = pSetup_Para->usSubAreaTop[5];
	editAreaTop6->Text = pSetup_Para->usSubAreaTop[6];
	editAreaTop7->Text = pSetup_Para->usSubAreaTop[7];

	editAreaWidth0->Text = pSetup_Para->usSubAreaWidth[0];
	editAreaWidth1->Text = pSetup_Para->usSubAreaWidth[1];
	editAreaWidth2->Text = pSetup_Para->usSubAreaWidth[2];
	editAreaWidth3->Text = pSetup_Para->usSubAreaWidth[3];
	editAreaWidth4->Text = pSetup_Para->usSubAreaWidth[4];
	editAreaWidth5->Text = pSetup_Para->usSubAreaWidth[5];
	editAreaWidth6->Text = pSetup_Para->usSubAreaWidth[6];
	editAreaWidth7->Text = pSetup_Para->usSubAreaWidth[7];

	editAreaHeight0->Text = pSetup_Para->usSubAreaHeight[0];
	editAreaHeight1->Text = pSetup_Para->usSubAreaHeight[1];
	editAreaHeight2->Text = pSetup_Para->usSubAreaHeight[2];
	editAreaHeight3->Text = pSetup_Para->usSubAreaHeight[3];
	editAreaHeight4->Text = pSetup_Para->usSubAreaHeight[4];
	editAreaHeight5->Text = pSetup_Para->usSubAreaHeight[5];
	editAreaHeight6->Text = pSetup_Para->usSubAreaHeight[6];
	editAreaHeight7->Text = pSetup_Para->usSubAreaHeight[7];

	comboBoxAreaCount->ItemIndex = pSetup_Para->ucSubAreaCount - 1;
	comboBoxAreaCountChange(this);
	DrawAreaImage();
}
//---------------------------------------------------------------------------

void TfrmConfig::UpdateConfigData_GS7022B(void* p)
{
	SETUP_PARA_GS7022B* pSetup_Para = (SETUP_PARA_GS7022B*)p;

	comboBoxScanMode->ItemIndex = (pSetup_Para->ulScanMode & 0xFF);
	checkByteReverse->Checked = (pSetup_Para->ulScanMode & 0x40000000);
	editSocketPort_Card->Text = pSetup_Para->usSocketPort;
	editDeviceCode->Text = pSetup_Para->szDeviceCode;
	editAPN->Text = pSetup_Para->szAPN;
	editDataCenter->Text = pSetup_Para->szDataCenter;

	checkActSecond->Checked = pSetup_Para->bActSecond;
	checkActSupportRepeat->Checked = pSetup_Para->bActSupportRepeat;
	checkSupportInsertPlay->Checked = pSetup_Para->bSupportInsertPlay;
	checkSyncPlay->Checked = pSetup_Para->bActSupportSync;
	checkData->Checked = pSetup_Para->bNegData;
	checkOE->Checked = pSetup_Para->bNegOE;
	if (pSetup_Para->bDoubleColor)
		rbtnDoubleColor->Checked = true;
	else
		rbtnSingleColor->Checked = true;
	checkSupportWarnning->Checked = pSetup_Para->bSupportWarnning;
	editScreenWidth->Text = pSetup_Para->usScreenWidth;
	editScreenHeight->Text = pSetup_Para->usScreenHeight;
	comboBoxMaxActCount->ItemIndex = 0;
	editVoiceArea->Text = pSetup_Para->ucVoiceOutputAreaIndex;

	SYSTEMTIME st;
	::GetLocalTime(&st);
	st.wSecond = 0;
	st.wMilliseconds = 0;

	st.wHour = pSetup_Para->ucLightControlStartHour[0];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[0];
	timeStart0->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[1];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[1];
	timeStart1->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[2];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[2];
	timeStart2->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[3];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[3];
	timeStart3->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[4];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[4];
	timeStart4->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[5];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[5];
	timeStart5->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[6];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[6];
	timeStart6->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[7];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[7];
	timeStart7->Time = SystemTimeToDateTime(st);

	comboBoxLightValue0->ItemIndex = pSetup_Para->ucLightControlValue[0];
	comboBoxLightValue1->ItemIndex = pSetup_Para->ucLightControlValue[1];
	comboBoxLightValue2->ItemIndex = pSetup_Para->ucLightControlValue[2];
	comboBoxLightValue3->ItemIndex = pSetup_Para->ucLightControlValue[3];
	comboBoxLightValue4->ItemIndex = pSetup_Para->ucLightControlValue[4];
	comboBoxLightValue5->ItemIndex = pSetup_Para->ucLightControlValue[5];
	comboBoxLightValue6->ItemIndex = pSetup_Para->ucLightControlValue[6];
	comboBoxLightValue7->ItemIndex = pSetup_Para->ucLightControlValue[7];

	editAreaLeft0->Text = pSetup_Para->usSubAreaLeft[0];
	editAreaLeft1->Text = pSetup_Para->usSubAreaLeft[1];
	editAreaLeft2->Text = pSetup_Para->usSubAreaLeft[2];
	editAreaLeft3->Text = pSetup_Para->usSubAreaLeft[3];
	editAreaLeft4->Text = pSetup_Para->usSubAreaLeft[4];
	editAreaLeft5->Text = pSetup_Para->usSubAreaLeft[5];
	editAreaLeft6->Text = pSetup_Para->usSubAreaLeft[6];
	editAreaLeft7->Text = pSetup_Para->usSubAreaLeft[7];

	editAreaTop0->Text = pSetup_Para->usSubAreaTop[0];
	editAreaTop1->Text = pSetup_Para->usSubAreaTop[1];
	editAreaTop2->Text = pSetup_Para->usSubAreaTop[2];
	editAreaTop3->Text = pSetup_Para->usSubAreaTop[3];
	editAreaTop4->Text = pSetup_Para->usSubAreaTop[4];
	editAreaTop5->Text = pSetup_Para->usSubAreaTop[5];
	editAreaTop6->Text = pSetup_Para->usSubAreaTop[6];
	editAreaTop7->Text = pSetup_Para->usSubAreaTop[7];

	editAreaWidth0->Text = pSetup_Para->usSubAreaWidth[0];
	editAreaWidth1->Text = pSetup_Para->usSubAreaWidth[1];
	editAreaWidth2->Text = pSetup_Para->usSubAreaWidth[2];
	editAreaWidth3->Text = pSetup_Para->usSubAreaWidth[3];
	editAreaWidth4->Text = pSetup_Para->usSubAreaWidth[4];
	editAreaWidth5->Text = pSetup_Para->usSubAreaWidth[5];
	editAreaWidth6->Text = pSetup_Para->usSubAreaWidth[6];
	editAreaWidth7->Text = pSetup_Para->usSubAreaWidth[7];

	editAreaHeight0->Text = pSetup_Para->usSubAreaHeight[0];
	editAreaHeight1->Text = pSetup_Para->usSubAreaHeight[1];
	editAreaHeight2->Text = pSetup_Para->usSubAreaHeight[2];
	editAreaHeight3->Text = pSetup_Para->usSubAreaHeight[3];
	editAreaHeight4->Text = pSetup_Para->usSubAreaHeight[4];
	editAreaHeight5->Text = pSetup_Para->usSubAreaHeight[5];
	editAreaHeight6->Text = pSetup_Para->usSubAreaHeight[6];
	editAreaHeight7->Text = pSetup_Para->usSubAreaHeight[7];

	comboBoxAreaCount->ItemIndex = pSetup_Para->ucSubAreaCount - 1;
	comboBoxAreaCountChange(this);
	DrawAreaImage();

	for (int i = 0; i < MAX_SMS_USER; i++)
		 pSetup_Para->m_szSmsUsers[i][PHONE_NUMBER_LEN - 1] = 0;

	editSmsUser0->Text = pSetup_Para->m_szSmsUsers[0];
	editSmsUser1->Text = pSetup_Para->m_szSmsUsers[1];
	editSmsUser2->Text = pSetup_Para->m_szSmsUsers[2];
	editSmsUser3->Text = pSetup_Para->m_szSmsUsers[3];
	editSmsUser4->Text = pSetup_Para->m_szSmsUsers[4];
	editSmsUser5->Text = pSetup_Para->m_szSmsUsers[5];
	editSmsUser6->Text = pSetup_Para->m_szSmsUsers[6];
	editSmsUser7->Text = pSetup_Para->m_szSmsUsers[7];
	checkEchoSms0->Checked = pSetup_Para->ucReplaySms & 0x01;
	checkEchoSms1->Checked = pSetup_Para->ucReplaySms & 0x02;
	checkEchoSms2->Checked = pSetup_Para->ucReplaySms & 0x04;
	checkEchoSms3->Checked = pSetup_Para->ucReplaySms & 0x08;
	checkEchoSms4->Checked = pSetup_Para->ucReplaySms & 0x10;
	checkEchoSms5->Checked = pSetup_Para->ucReplaySms & 0x20;
	checkEchoSms6->Checked = pSetup_Para->ucReplaySms & 0x40;
	checkEchoSms7->Checked = pSetup_Para->ucReplaySms & 0x80;

	editShowTime->Text = pSetup_Para->ulShowTime;
	comboEnterStyle->ItemIndex = pSetup_Para->ucEnterStyle;
	editEnterSpeed->Text = pSetup_Para->ucEnterSpeed;
	comboExitStyle->ItemIndex = pSetup_Para->ucExitStyle;
	editExitSpeed->Text = pSetup_Para->ucExitSpeed;
	editRepeatCount->Text = pSetup_Para->ucRepeatCount;
	switch (pSetup_Para->ucFontSize)
	{
	case 16:
		comboFont->ItemIndex = 0;
		break;
	case 32:
		comboFont->ItemIndex = 1;
		break;
	case 48:
		comboFont->ItemIndex = 2;
		break;
	case 64:
		comboFont->ItemIndex = 3;
		break;
	default:
		comboFont->ItemIndex = 0;
		break;
	}

	checkTCP->Checked = pSetup_Para->bTCP;
}
//---------------------------------------------------------------------------

void TfrmConfig::UpdateConfigData_GS2012AS_64(void* p)
{
	SETUP_PARA_GS2012AS_64* pSetup_Para = (SETUP_PARA_GS2012AS_64*)p;

	editScreenWidth->Text = pSetup_Para->usScreenWidth;
	editScreenHeight->Text = pSetup_Para->usScreenHeight;
	comboBoxScanMode->ItemIndex = pSetup_Para->ucScanMode;
	checkData->Checked = (pSetup_Para->ucFlags & 1);
	checkOE->Checked = (pSetup_Para->ucFlags & 2);
	if (pSetup_Para->ucFlags & 0x10)
		rbtnDoubleColor->Checked = true;
	else
		rbtnSingleColor->Checked = true;
	checkByteReverse->Checked = (pSetup_Para->ucFlags & 0x08);
	comboBoxMaxActCount->ItemIndex = 0;

	SYSTEMTIME st;
	::GetLocalTime(&st);
	st.wSecond = 0;
	st.wMilliseconds = 0;

	st.wHour = pSetup_Para->ucLightControlStartHour[0];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[0];
	timeStart0->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[1];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[1];
	timeStart1->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[2];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[2];
	timeStart2->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[3];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[3];
	timeStart3->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[4];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[4];
	timeStart4->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[5];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[5];
	timeStart5->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[6];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[6];
	timeStart6->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[7];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[7];
	timeStart7->Time = SystemTimeToDateTime(st);

	comboBoxLightValue0->ItemIndex = pSetup_Para->ucLightControlValue[0];
	comboBoxLightValue1->ItemIndex = pSetup_Para->ucLightControlValue[1];
	comboBoxLightValue2->ItemIndex = pSetup_Para->ucLightControlValue[2];
	comboBoxLightValue3->ItemIndex = pSetup_Para->ucLightControlValue[3];
	comboBoxLightValue4->ItemIndex = pSetup_Para->ucLightControlValue[4];
	comboBoxLightValue5->ItemIndex = pSetup_Para->ucLightControlValue[5];
	comboBoxLightValue6->ItemIndex = pSetup_Para->ucLightControlValue[6];
	comboBoxLightValue7->ItemIndex = pSetup_Para->ucLightControlValue[7];

	editAreaLeft0->Text = pSetup_Para->usSubAreaLeft[0];
	editAreaLeft1->Text = pSetup_Para->usSubAreaLeft[1];
	editAreaLeft2->Text = pSetup_Para->usSubAreaLeft[2];
	editAreaLeft3->Text = pSetup_Para->usSubAreaLeft[3];
	editAreaLeft4->Text = pSetup_Para->usSubAreaLeft[4];
	editAreaLeft5->Text = pSetup_Para->usSubAreaLeft[5];
	editAreaLeft6->Text = pSetup_Para->usSubAreaLeft[6];
	editAreaLeft7->Text = pSetup_Para->usSubAreaLeft[7];

	editAreaTop0->Text = pSetup_Para->usSubAreaTop[0];
	editAreaTop1->Text = pSetup_Para->usSubAreaTop[1];
	editAreaTop2->Text = pSetup_Para->usSubAreaTop[2];
	editAreaTop3->Text = pSetup_Para->usSubAreaTop[3];
	editAreaTop4->Text = pSetup_Para->usSubAreaTop[4];
	editAreaTop5->Text = pSetup_Para->usSubAreaTop[5];
	editAreaTop6->Text = pSetup_Para->usSubAreaTop[6];
	editAreaTop7->Text = pSetup_Para->usSubAreaTop[7];

	editAreaWidth0->Text = pSetup_Para->usSubAreaWidth[0];
	editAreaWidth1->Text = pSetup_Para->usSubAreaWidth[1];
	editAreaWidth2->Text = pSetup_Para->usSubAreaWidth[2];
	editAreaWidth3->Text = pSetup_Para->usSubAreaWidth[3];
	editAreaWidth4->Text = pSetup_Para->usSubAreaWidth[4];
	editAreaWidth5->Text = pSetup_Para->usSubAreaWidth[5];
	editAreaWidth6->Text = pSetup_Para->usSubAreaWidth[6];
	editAreaWidth7->Text = pSetup_Para->usSubAreaWidth[7];

	editAreaHeight0->Text = pSetup_Para->usSubAreaHeight[0];
	editAreaHeight1->Text = pSetup_Para->usSubAreaHeight[1];
	editAreaHeight2->Text = pSetup_Para->usSubAreaHeight[2];
	editAreaHeight3->Text = pSetup_Para->usSubAreaHeight[3];
	editAreaHeight4->Text = pSetup_Para->usSubAreaHeight[4];
	editAreaHeight5->Text = pSetup_Para->usSubAreaHeight[5];
	editAreaHeight6->Text = pSetup_Para->usSubAreaHeight[6];
	editAreaHeight7->Text = pSetup_Para->usSubAreaHeight[7];

	comboBoxAreaCount->ItemIndex = pSetup_Para->ucSubAreaCount - 1;
	comboBoxAreaCountChange(this);
	DrawAreaImage();

	for (int i = 0; i < MAX_SMS_USER; i++)
		 pSetup_Para->m_szSmsUsers[i][PHONE_NUMBER_LEN - 1] = 0;

	editSmsUser0->Text = pSetup_Para->m_szSmsUsers[0];
	editSmsUser1->Text = pSetup_Para->m_szSmsUsers[1];
	editSmsUser2->Text = pSetup_Para->m_szSmsUsers[2];
	editSmsUser3->Text = pSetup_Para->m_szSmsUsers[3];
	editSmsUser4->Text = pSetup_Para->m_szSmsUsers[4];
	editSmsUser5->Text = pSetup_Para->m_szSmsUsers[5];
	editSmsUser6->Text = pSetup_Para->m_szSmsUsers[6];
	editSmsUser7->Text = pSetup_Para->m_szSmsUsers[7];
	checkEchoSms0->Checked = pSetup_Para->ucReplaySms & 0x01;
	checkEchoSms1->Checked = pSetup_Para->ucReplaySms & 0x02;
	checkEchoSms2->Checked = pSetup_Para->ucReplaySms & 0x04;
	checkEchoSms3->Checked = pSetup_Para->ucReplaySms & 0x08;
	checkEchoSms4->Checked = pSetup_Para->ucReplaySms & 0x10;
	checkEchoSms5->Checked = pSetup_Para->ucReplaySms & 0x20;
	checkEchoSms6->Checked = pSetup_Para->ucReplaySms & 0x40;
	checkEchoSms7->Checked = pSetup_Para->ucReplaySms & 0x80;

	editShowTime->Text = pSetup_Para->ulShowTime;
	comboEnterStyle->ItemIndex = pSetup_Para->ucEnterStyle;
	editEnterSpeed->Text = pSetup_Para->ucEnterSpeed;
	comboExitStyle->ItemIndex = pSetup_Para->ucExitStyle;
	editExitSpeed->Text = pSetup_Para->ucExitSpeed;
	editRepeatCount->Text = pSetup_Para->ucRepeatCount;
	switch (pSetup_Para->ucFontSize)
	{
	case 16:
		comboFont->ItemIndex = 0;
		break;
	case 32:
		comboFont->ItemIndex = 1;
		break;
	case 48:
		comboFont->ItemIndex = 2;
		break;
	case 64:
		comboFont->ItemIndex = 3;
		break;
	default:
		comboFont->ItemIndex = 0;
		break;
	}
}
//---------------------------------------------------------------------------

void TfrmConfig::UpdateConfigData_GS9020S(void* p)
{
	SETUP_PARA_GS9020S* pSetup_Para = (SETUP_PARA_GS9020S*)p;

	editScreenWidth->Text = pSetup_Para->usScreenWidth;
	editScreenHeight->Text = pSetup_Para->usScreenHeight;
	comboBoxScanMode->ItemIndex = pSetup_Para->ucScanMode;
	checkData->Checked = (pSetup_Para->ucFlags & 1);
	checkOE->Checked = (pSetup_Para->ucFlags & 2);
	rbtnSingleColor->Checked = true;
	checkByteReverse->Checked = (pSetup_Para->ucFlags & 0x08);
	comboBoxMaxActCount->ItemIndex = 0;

	SYSTEMTIME st;
	::GetLocalTime(&st);
	st.wSecond = 0;
	st.wMilliseconds = 0;

	st.wHour = pSetup_Para->ucLightControlStartHour[0];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[0];
	timeStart0->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[1];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[1];
	timeStart1->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[2];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[2];
	timeStart2->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[3];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[3];
	timeStart3->Time = SystemTimeToDateTime(st);

	comboBoxLightValue0->ItemIndex = pSetup_Para->ucLightControlValue[0];
	comboBoxLightValue1->ItemIndex = pSetup_Para->ucLightControlValue[1];
	comboBoxLightValue2->ItemIndex = pSetup_Para->ucLightControlValue[2];
	comboBoxLightValue3->ItemIndex = pSetup_Para->ucLightControlValue[3];

	for (int i = 0; i < 4; i++)
		 pSetup_Para->m_szSmsUsers[i][PHONE_NUMBER_LEN - 1] = 0;

	editSmsUser0->Text = pSetup_Para->m_szSmsUsers[0];
	editSmsUser1->Text = pSetup_Para->m_szSmsUsers[1];
	editSmsUser2->Text = pSetup_Para->m_szSmsUsers[2];
	editSmsUser3->Text = pSetup_Para->m_szSmsUsers[3];
	checkEchoSms0->Checked = pSetup_Para->ucReplaySms & 0x01;
	checkEchoSms1->Checked = pSetup_Para->ucReplaySms & 0x02;
	checkEchoSms2->Checked = pSetup_Para->ucReplaySms & 0x04;
	checkEchoSms3->Checked = pSetup_Para->ucReplaySms & 0x08;

	editShowTime->Text = pSetup_Para->ulShowTime;
	comboEnterStyle->ItemIndex = pSetup_Para->ucEnterStyle;
	editEnterSpeed->Text = pSetup_Para->ucEnterSpeed;
	comboExitStyle->ItemIndex = pSetup_Para->ucExitStyle;
	editExitSpeed->Text = pSetup_Para->ucExitSpeed;
	editRepeatCount->Text = pSetup_Para->ucRepeatCount;
	switch (pSetup_Para->ucFontSize)
	{
	case 16:
		comboFont->ItemIndex = 0;
		break;
	default:
		comboFont->ItemIndex = 1;
		break;
	}
}
//---------------------------------------------------------------------------

void TfrmConfig::UpdateConfigData_GS7020HS(void* p)
{
	SETUP_PARA_GS7020HS* pSetup_Para = (SETUP_PARA_GS7020HS*)p;

	editScreenWidth->Text = pSetup_Para->usScreenWidth;
	editScreenHeight->Text = pSetup_Para->usScreenHeight;
	comboBoxScanMode->ItemIndex = pSetup_Para->ucScanMode;
	checkData->Checked = (pSetup_Para->ucFlags & 1);
	checkOE->Checked = (pSetup_Para->ucFlags & 2);
	rbtnSingleColor->Checked = true;
	checkByteReverse->Checked = (pSetup_Para->ucFlags & 0x08);
	comboBoxMaxActCount->ItemIndex = 0;

	SYSTEMTIME st;
	::GetLocalTime(&st);
	st.wSecond = 0;
	st.wMilliseconds = 0;

	st.wHour = pSetup_Para->ucLightControlStartHour[0];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[0];
	timeStart0->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[1];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[1];
	timeStart1->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[2];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[2];
	timeStart2->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[3];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[3];
	timeStart3->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[4];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[4];
	timeStart4->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[5];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[5];
	timeStart5->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[6];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[6];
	timeStart6->Time = SystemTimeToDateTime(st);

	st.wHour = pSetup_Para->ucLightControlStartHour[7];
	st.wMinute = pSetup_Para->ucLightControlStartMinute[7];
	timeStart7->Time = SystemTimeToDateTime(st);

	comboBoxLightValue0->ItemIndex = pSetup_Para->ucLightControlValue[0];
	comboBoxLightValue1->ItemIndex = pSetup_Para->ucLightControlValue[1];
	comboBoxLightValue2->ItemIndex = pSetup_Para->ucLightControlValue[2];
	comboBoxLightValue3->ItemIndex = pSetup_Para->ucLightControlValue[3];
	comboBoxLightValue3->ItemIndex = pSetup_Para->ucLightControlValue[4];
	comboBoxLightValue3->ItemIndex = pSetup_Para->ucLightControlValue[5];
	comboBoxLightValue3->ItemIndex = pSetup_Para->ucLightControlValue[6];
	comboBoxLightValue3->ItemIndex = pSetup_Para->ucLightControlValue[7];

	for (int i = 0; i < 7; i++)
		 pSetup_Para->m_szSmsUsers[i][PHONE_NUMBER_LEN - 1] = 0;

	editSmsUser0->Text = pSetup_Para->m_szSmsUsers[0];
	editSmsUser1->Text = pSetup_Para->m_szSmsUsers[1];
	editSmsUser2->Text = pSetup_Para->m_szSmsUsers[2];
	editSmsUser3->Text = pSetup_Para->m_szSmsUsers[3];
	editSmsUser4->Text = pSetup_Para->m_szSmsUsers[4];
	editSmsUser5->Text = pSetup_Para->m_szSmsUsers[5];
	editSmsUser6->Text = pSetup_Para->m_szSmsUsers[6];
	editSmsUser7->Text = pSetup_Para->m_szSmsUsers[7];
	checkEchoSms0->Checked = pSetup_Para->ucReplaySms & 0x01;
	checkEchoSms1->Checked = pSetup_Para->ucReplaySms & 0x02;
	checkEchoSms2->Checked = pSetup_Para->ucReplaySms & 0x04;
	checkEchoSms3->Checked = pSetup_Para->ucReplaySms & 0x08;
	checkEchoSms4->Checked = pSetup_Para->ucReplaySms & 0x10;
	checkEchoSms5->Checked = pSetup_Para->ucReplaySms & 0x20;
	checkEchoSms6->Checked = pSetup_Para->ucReplaySms & 0x40;
	checkEchoSms7->Checked = pSetup_Para->ucReplaySms & 0x80;

	editShowTime->Text = pSetup_Para->ulShowTime;
	comboEnterStyle->ItemIndex = pSetup_Para->ucEnterStyle;
	editEnterSpeed->Text = pSetup_Para->ucEnterSpeed;
	comboExitStyle->ItemIndex = pSetup_Para->ucExitStyle;
	editExitSpeed->Text = pSetup_Para->ucExitSpeed;
	editRepeatCount->Text = pSetup_Para->ucRepeatCount;
	switch (pSetup_Para->ucFontSize)
	{
	case 16:
		comboFont->ItemIndex = 0;
		break;
	default:
		comboFont->ItemIndex = 1;
		break;
	}
}
//---------------------------------------------------------------------------

void TfrmConfig::UpdateConfigData_PD102B(void* p)
{
	SETUP_PARA_PD102B* pSetup_Para = (SETUP_PARA_PD102B*)p;;

	comboBoxScanMode->ItemIndex = pSetup_Para->ucScanMode & 15;
	checkOE->Checked = pSetup_Para->ucScanMode & 0x80;
	checkData->Checked = pSetup_Para->ucScanMode & 0x40;
	if ( pSetup_Para->ucScanMode & 0x20)
		rbtnDoubleColor->Checked = true;
	else
		rbtnSingleColor->Checked = true;
	editScreenWidth->Text = pSetup_Para->ucScreenByteWidth * 8;
	editScreenHeight->Text = pSetup_Para->ucScreenHeight;
	cbBaudrate->ItemIndex = pSetup_Para->ucBaudrateIndex;
	editNewCardId->Text = editCardId->Text;

	editAreaLeft0->Text = 0;
	editAreaTop0->Text = 0;
	editAreaWidth0->Text = editScreenWidth->Text;
	editAreaHeight0->Text = editScreenHeight->Text;
	comboBoxAreaCount->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void TfrmConfig::UpdateConfigData(void* p)
{
	if (sysConfig.m_nCardType == CARD_TYPE_YL2010A)
		UpdateConfigData_YL2010A(p);
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010A_GPS_JTX)
		UpdateConfigData_YL2010A(p);
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010AR)
		UpdateConfigData_YL2010A(p);
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010C)
		UpdateConfigData_YL2010A(p);
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010AF)
		UpdateConfigData_YL2010A(p);
	else if (sysConfig.m_nCardType == CARD_TYPE_GS7022B)
		UpdateConfigData_GS7022B(p);
	else if (sysConfig.m_nCardType == CARD_TYPE_GS2012A_S)
		UpdateConfigData_GS7022B(p);
	else if (sysConfig.m_nCardType == CARD_TYPE_PD102B)
		UpdateConfigData_PD102B(p);
	else if (sysConfig.m_nCardType == CARD_TYPE_WF2012B)
		UpdateConfigData_WF2012B(p);
	else if (sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64)
		UpdateConfigData_GS2012AS_64(p);
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9020S)
		UpdateConfigData_GS9020S(p);
	else if (sysConfig.m_nCardType == CARD_TYPE_GS7020HS)
		UpdateConfigData_GS7020HS(p);
	else if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
		UpdateConfigData_SN2P(p);
	else
		UpdateConfigData_GS7020B(p);
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::FormShow(TObject *Sender)
{
	DrawAreaImage();
}
//---------------------------------------------------------------------------

void TfrmConfig::DrawAreaImage()
{
	int nScreenWidth = editScreenWidth->Text.ToInt();
	int nScreenHeight = editScreenHeight->Text.ToInt();
	imageAreas->Picture->Bitmap->Width = nScreenWidth;
	imageAreas->Width = nScreenWidth;
	imageAreas->Picture->Bitmap->Height = nScreenHeight;
	imageAreas->Height = nScreenHeight;
	if (imageAreas->Width < scrAreas->Width)
		imageAreas->Left = (scrAreas->Width - imageAreas->Width) / 2;
	else
		imageAreas->Left = 0;
	if (imageAreas->Height < scrAreas->Height)
		imageAreas->Top = (scrAreas->Height - imageAreas->Height) / 2;
	else
		imageAreas->Top = 0;
	imageAreas->Canvas->Brush->Color = clBlack;
	imageAreas->Canvas->FillRect(Rect(0, 0, scrAreas->Width, scrAreas->Height));

	int nLeft, nTop, nWidth, nHeight, nTextWidth, nTextHeight;
	imageAreas->Canvas->Pen->Style = psSolid;
	imageAreas->Canvas->Pen->Width = 1;
	imageAreas->Canvas->Pen->Color = clLime;
	imageAreas->Canvas->Font = lbArea0->Font;
	imageAreas->Canvas->Font->Color = clWhite;

	nLeft = editAreaLeft0->Text.ToInt();
	nTop = editAreaTop0->Text.ToInt();
	nWidth = editAreaWidth0->Text.ToInt();
	nHeight = editAreaHeight0->Text.ToInt();
	nTextWidth = imageAreas->Picture->Bitmap->Canvas->TextWidth("1");
	nTextHeight = imageAreas->Picture->Bitmap->Canvas->TextHeight("1");
	imageAreas->Canvas->Rectangle(TRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight));
	imageAreas->Canvas->TextOut(nLeft + (nWidth - nTextWidth) / 2 , nTop + (nHeight - nTextHeight) /2, "1");

	int nSubAreaCount = comboBoxAreaCount->ItemIndex + 1;

	if (nSubAreaCount < 2)
		return;
	nLeft = editAreaLeft1->Text.ToInt();
	nTop = editAreaTop1->Text.ToInt();
	nWidth = editAreaWidth1->Text.ToInt();
	nHeight = editAreaHeight1->Text.ToInt();
	nTextWidth = imageAreas->Picture->Bitmap->Canvas->TextWidth("2");
	nTextHeight = imageAreas->Picture->Bitmap->Canvas->TextHeight("2");
	imageAreas->Canvas->Rectangle(TRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight));
	imageAreas->Canvas->TextOut(nLeft + (nWidth - nTextWidth) / 2 , nTop + (nHeight - nTextHeight) /2, "2");

	if (nSubAreaCount < 3)
		return;
	nLeft = editAreaLeft2->Text.ToInt();
	nTop = editAreaTop2->Text.ToInt();
	nWidth = editAreaWidth2->Text.ToInt();
	nHeight = editAreaHeight2->Text.ToInt();
	nTextWidth = imageAreas->Picture->Bitmap->Canvas->TextWidth("3");
	nTextHeight = imageAreas->Picture->Bitmap->Canvas->TextHeight("3");
	imageAreas->Canvas->Rectangle(TRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight));
	imageAreas->Canvas->TextOut(nLeft + (nWidth - nTextWidth) / 2 , nTop + (nHeight - nTextHeight) /2, "3");

	if (nSubAreaCount < 4)
		return;
	nLeft = editAreaLeft3->Text.ToInt();
	nTop = editAreaTop3->Text.ToInt();
	nWidth = editAreaWidth3->Text.ToInt();
	nHeight = editAreaHeight3->Text.ToInt();
	nTextWidth = imageAreas->Picture->Bitmap->Canvas->TextWidth("4");
	nTextHeight = imageAreas->Picture->Bitmap->Canvas->TextHeight("4");
	imageAreas->Canvas->Rectangle(TRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight));
	imageAreas->Canvas->TextOut(nLeft + (nWidth - nTextWidth) / 2 , nTop + (nHeight - nTextHeight) /2, "4");

	if (nSubAreaCount < 5)
		return;
	nLeft = editAreaLeft4->Text.ToInt();
	nTop = editAreaTop4->Text.ToInt();
	nWidth = editAreaWidth4->Text.ToInt();
	nHeight = editAreaHeight4->Text.ToInt();
	nTextWidth = imageAreas->Picture->Bitmap->Canvas->TextWidth("5");
	nTextHeight = imageAreas->Picture->Bitmap->Canvas->TextHeight("5");
	imageAreas->Canvas->Rectangle(TRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight));
	imageAreas->Canvas->TextOut(nLeft + (nWidth - nTextWidth) / 2 , nTop + (nHeight - nTextHeight) /2, "5");

	if (nSubAreaCount < 6)
		return;
	nLeft = editAreaLeft5->Text.ToInt();
	nTop = editAreaTop5->Text.ToInt();
	nWidth = editAreaWidth5->Text.ToInt();
	nHeight = editAreaHeight5->Text.ToInt();
	nTextWidth = imageAreas->Picture->Bitmap->Canvas->TextWidth("6");
	nTextHeight = imageAreas->Picture->Bitmap->Canvas->TextHeight("6");
	imageAreas->Canvas->Rectangle(TRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight));
	imageAreas->Canvas->TextOut(nLeft + (nWidth - nTextWidth) / 2 , nTop + (nHeight - nTextHeight) /2, "6");

	if (nSubAreaCount < 7)
		return;
	nLeft = editAreaLeft6->Text.ToInt();
	nTop = editAreaTop6->Text.ToInt();
	nWidth = editAreaWidth6->Text.ToInt();
	nHeight = editAreaHeight6->Text.ToInt();
	nTextWidth = imageAreas->Picture->Bitmap->Canvas->TextWidth("7");
	nTextHeight = imageAreas->Picture->Bitmap->Canvas->TextHeight("7");
	imageAreas->Canvas->Rectangle(TRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight));
	imageAreas->Canvas->TextOut(nLeft + (nWidth - nTextWidth) / 2 , nTop + (nHeight - nTextHeight) /2, "7");

	if (nSubAreaCount < 8)
		return;
	nLeft = editAreaLeft7->Text.ToInt();
	nTop = editAreaTop7->Text.ToInt();
	nWidth = editAreaWidth7->Text.ToInt();
	nHeight = editAreaHeight7->Text.ToInt();
	nTextWidth = imageAreas->Picture->Bitmap->Canvas->TextWidth("8");
	nTextHeight = imageAreas->Picture->Bitmap->Canvas->TextHeight("8");
	imageAreas->Canvas->Rectangle(TRect(nLeft, nTop, nLeft + nWidth, nTop + nHeight));
	imageAreas->Canvas->TextOut(nLeft + (nWidth - nTextWidth) / 2 , nTop + (nHeight - nTextHeight) /2, "8");
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::editSubAreaChange(TObject *Sender)
{
	DrawAreaImage();
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::FormCreate(TObject *Sender)
{
	switch (sysConfig.m_nCardType)
	{
		case CARD_TYPE_GS7020BF_W:
			m_nSelectActCountTable[0] = 255;
			m_nSelectActCountTableSize = 1;
			break;
		case CARD_TYPE_GS7022B:
			m_nSelectActCountTable[0] = 32;
			m_nSelectActCountTableSize = 1;
			break;
		case CARD_TYPE_GS2012A_S:
			m_nSelectActCountTable[0] = 64;
			m_nSelectActCountTableSize = 1;
			break;
		case CARD_TYPE_GS2012AS_64:
			m_nSelectActCountTable[0] = 32;
			m_nSelectActCountTableSize = 1;
			break;
		case CARD_TYPE_GS9020S:
			m_nSelectActCountTable[0] = 32;
			m_nSelectActCountTableSize = 1;
			break;
		case CARD_TYPE_GS7020HS:
			m_nSelectActCountTable[0] = 32;
			m_nSelectActCountTableSize = 1;
			break;
		case CARD_TYPE_PD102B:
			m_nSelectActCountTable[0] = 31;
			m_nSelectActCountTableSize = 1;
			break;
		case CARD_TYPE_GS7024E:
		case CARD_TYPE_GS7020HFA:
			m_nSelectActCountTable[0] = 255;
			m_nSelectActCountTable[1] = 127;
			m_nSelectActCountTable[2] = 63;
			m_nSelectActCountTable[3] = 31;
			m_nSelectActCountTableSize = 4;
			break;
		case CARD_TYPE_GS9025:
			m_nSelectActCountTable[0] = 16;
			m_nSelectActCountTableSize = 1;
			break;
		case CARD_TYPE_GS9023:
			m_nSelectActCountTable[0] = 128;
			m_nSelectActCountTableSize = 1;
			break;
		default:
			m_nSelectActCountTable[0] = 127;
			m_nSelectActCountTable[1] = 63;
			m_nSelectActCountTable[2] = 31;
			m_nSelectActCountTableSize = 3;
			break;
	}
	comboBoxMaxActCount->Clear();
	for (int i = 0; i < m_nSelectActCountTableSize; i++)
		comboBoxMaxActCount->Items->Add(m_nSelectActCountTable[i]);

	switch (sysConfig.m_nCardBaudRate)
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
	editDeviceCode->Text = m_dtu->m_sCode;
	editWifiDeviceCode->Text = m_dtu->m_sCode;
	editAPN->Text = sysConfig.m_sAPN;
	comboBoxScanMode->ItemIndex = sysConfig.m_ulScanMode;
	checkByteReverse->Checked = sysConfig.m_bByteReverse;
	editSocketPort_Card->Text = sysConfig.m_nSocketPort_Card;
	editWifiSocketPort_Card->Text = sysConfig.m_nSocketPort_Card;
	checkData->Checked = sysConfig.m_bNegData;
	checkOE->Checked = sysConfig.m_bNegOE;
	checkSupportLedSignCheck->Checked = sysConfig.m_bSupportLedSignCheck;
	if (sysConfig.m_bFullColor)
		rbtnFullColor->Checked = true;
	else
	{
		if (sysConfig.m_bDoubleColor)
			rbtnDoubleColor->Checked = true;
		else
			rbtnSingleColor->Checked = true;
    }
	checkSupportWarnning->Checked = sysConfig.m_bSupportWarning;
#ifdef AUTO_SIZE_TEXT
	editScreenWidth->Text = m_dtu->m_nScreenWidth;
	editScreenHeight->Text = m_dtu->m_nScreenHeight;
#else
	editScreenWidth->Text = sysConfig.m_nScreenWidth;
	editScreenHeight->Text = sysConfig.m_nScreenHeight;
#endif
	editDataCenter->Text = sysConfig.m_sDataCenter;
	editWifiServer->Text = sysConfig.m_sDataCenter;
	editSSID->Text = m_dtu->m_sWifiSSID;
	editWifiPassword->Text = m_dtu->m_sWifiPassword;
	editWifiDnsServer->Text = m_dtu->m_sWifiDnsServer;

	switch (sysConfig.m_nCardType)
	{
		case CARD_TYPE_GS9025:
		case CARD_TYPE_GS9023:
			checkTCP->Checked = sysConfig.m_bTCP;
			rbtnFullColor->Visible = false;
			rbtnDoubleColor->Visible = false;
			checkActSecond->Checked = false;
			checkActSupportRepeat->Checked = true;
			checkSupportInsertPlay->Checked = false;
			checkSyncPlay->Checked = false;
			checkSyncTimeHint->Checked = false;
			break;
		case CARD_TYPE_GS9020:
		case CARD_TYPE_GS7020B:
		case CARD_TYPE_GS7020H:
			checkActSecond->Checked = sysConfig.m_bActSecond;
			checkActSupportRepeat->Checked = sysConfig.m_bActSupportRepeat;
			checkSupportInsertPlay->Checked = sysConfig.m_bSupportInsertPlay;
			checkSyncPlay->Checked = sysConfig.m_bActSupportSync;
			checkSyncTimeHint->Checked = sysConfig.m_bSupportCycleTimeHint;
			checkTCP->Checked = sysConfig.m_bTCP;
			//if (sysConfig.m_nCardType == CARD_TYPE_GS7020B)
			//	checkLoginEcho->Checked = sysConfig.m_bLoginEcho;
			//else
				checkLoginEcho->Checked = false;
			break;
		case CARD_TYPE_PD102B:
			checkActSecond->Checked = false;
			checkActSupportRepeat->Checked = false;
			checkSupportInsertPlay->Checked = false;
			checkSyncPlay->Checked = false;
			checkSyncTimeHint->Checked = false;
			checkTCP->Checked = false;
			checkLoginEcho->Checked = false;
			break;
		case CARD_TYPE_GS2012A:
		case CARD_TYPE_GS2012A_S:
			checkActSecond->Checked = sysConfig.m_bActSecond;
			checkActSupportRepeat->Checked = sysConfig.m_bActSupportRepeat;
			checkSupportInsertPlay->Checked = sysConfig.m_bSupportInsertPlay;
			checkSyncPlay->Checked = sysConfig.m_bActSupportSync;
			checkSyncTimeHint->Checked = sysConfig.m_bSupportCycleTimeHint;
			checkTCP->Checked = sysConfig.m_bTCP;
			checkLoginEcho->Checked = false;
			break;
		default:
			checkActSecond->Checked = sysConfig.m_bActSecond;
			checkActSupportRepeat->Checked = sysConfig.m_bActSupportRepeat;
			checkSupportInsertPlay->Checked = sysConfig.m_bSupportInsertPlay;
			checkSyncPlay->Checked = sysConfig.m_bActSupportSync;
			checkSyncTimeHint->Checked = sysConfig.m_bSupportCycleTimeHint;
			checkTCP->Checked = false;
			checkLoginEcho->Checked = false;
			break;
	}
	comboBoxMaxActCount->ItemIndex = 0;
	for (int i = 0; i < m_nSelectActCountTableSize; i++)
	{
		if (sysConfig.m_nMaxActCount == m_nSelectActCountTable[i])
		{
			comboBoxMaxActCount->ItemIndex = i;
			break;
		}
	}

	SYSTEMTIME st;
	::GetLocalTime(&st);
	st.wSecond = 0;
	st.wMilliseconds = 0;

	st.wHour = sysConfig.m_nLightControlStartHour[0];
	st.wMinute = sysConfig.m_nLightControlStartMinute[0];
	timeStart0->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[1];
	st.wMinute = sysConfig.m_nLightControlStartMinute[1];
	timeStart1->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[2];
	st.wMinute = sysConfig.m_nLightControlStartMinute[2];
	timeStart2->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[3];
	st.wMinute = sysConfig.m_nLightControlStartMinute[3];
	timeStart3->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[4];
	st.wMinute = sysConfig.m_nLightControlStartMinute[4];
	timeStart4->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[5];
	st.wMinute = sysConfig.m_nLightControlStartMinute[5];
	timeStart5->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[6];
	st.wMinute = sysConfig.m_nLightControlStartMinute[6];
	timeStart6->Time = SystemTimeToDateTime(st);

	st.wHour = sysConfig.m_nLightControlStartHour[7];
	st.wMinute = sysConfig.m_nLightControlStartMinute[7];
	timeStart7->Time = SystemTimeToDateTime(st);

	comboBoxLightValue0->ItemIndex = sysConfig.m_nLightControlValue[0];
	comboBoxLightValue1->ItemIndex = sysConfig.m_nLightControlValue[1];
	comboBoxLightValue2->ItemIndex = sysConfig.m_nLightControlValue[2];
	comboBoxLightValue3->ItemIndex = sysConfig.m_nLightControlValue[3];
	comboBoxLightValue4->ItemIndex = sysConfig.m_nLightControlValue[4];
	comboBoxLightValue5->ItemIndex = sysConfig.m_nLightControlValue[5];
	comboBoxLightValue6->ItemIndex = sysConfig.m_nLightControlValue[6];
	comboBoxLightValue7->ItemIndex = sysConfig.m_nLightControlValue[7];

#ifdef AUTO_SIZE_TEXT
	editAreaLeft0->Text = m_dtu->m_nSubAreaLeft[0];
	editAreaLeft1->Text = m_dtu->m_nSubAreaLeft[1];
	editAreaLeft2->Text = m_dtu->m_nSubAreaLeft[2];
	editAreaLeft3->Text = m_dtu->m_nSubAreaLeft[3];
	editAreaLeft4->Text = m_dtu->m_nSubAreaLeft[4];
	editAreaLeft5->Text = m_dtu->m_nSubAreaLeft[5];
	editAreaLeft6->Text = m_dtu->m_nSubAreaLeft[6];
	editAreaLeft7->Text = m_dtu->m_nSubAreaLeft[7];

	editAreaTop0->Text = m_dtu->m_nSubAreaTop[0];
	editAreaTop1->Text = m_dtu->m_nSubAreaTop[1];
	editAreaTop2->Text = m_dtu->m_nSubAreaTop[2];
	editAreaTop3->Text = m_dtu->m_nSubAreaTop[3];
	editAreaTop4->Text = m_dtu->m_nSubAreaTop[4];
	editAreaTop5->Text = m_dtu->m_nSubAreaTop[5];
	editAreaTop6->Text = m_dtu->m_nSubAreaTop[6];
	editAreaTop7->Text = m_dtu->m_nSubAreaTop[7];

	editAreaWidth0->Text = m_dtu->m_nSubAreaWidth[0];
	editAreaWidth1->Text = m_dtu->m_nSubAreaWidth[1];
	editAreaWidth2->Text = m_dtu->m_nSubAreaWidth[2];
	editAreaWidth3->Text = m_dtu->m_nSubAreaWidth[3];
	editAreaWidth4->Text = m_dtu->m_nSubAreaWidth[4];
	editAreaWidth5->Text = m_dtu->m_nSubAreaWidth[5];
	editAreaWidth6->Text = m_dtu->m_nSubAreaWidth[6];
	editAreaWidth7->Text = m_dtu->m_nSubAreaWidth[7];

	editAreaHeight0->Text = m_dtu->m_nSubAreaHeight[0];
	editAreaHeight1->Text = m_dtu->m_nSubAreaHeight[1];
	editAreaHeight2->Text = m_dtu->m_nSubAreaHeight[2];
	editAreaHeight3->Text = m_dtu->m_nSubAreaHeight[3];
	editAreaHeight4->Text = m_dtu->m_nSubAreaHeight[4];
	editAreaHeight5->Text = m_dtu->m_nSubAreaHeight[5];
	editAreaHeight6->Text = m_dtu->m_nSubAreaHeight[6];
	editAreaHeight7->Text = m_dtu->m_nSubAreaHeight[7];
	comboBoxAreaCount->ItemIndex = m_dtu->m_nSubAreaCount - 1;
#else
	editAreaLeft0->Text = sysConfig.m_nSubAreaLeft[0];
	editAreaLeft1->Text = sysConfig.m_nSubAreaLeft[1];
	editAreaLeft2->Text = sysConfig.m_nSubAreaLeft[2];
	editAreaLeft3->Text = sysConfig.m_nSubAreaLeft[3];
	editAreaLeft4->Text = sysConfig.m_nSubAreaLeft[4];
	editAreaLeft5->Text = sysConfig.m_nSubAreaLeft[5];
	editAreaLeft6->Text = sysConfig.m_nSubAreaLeft[6];
	editAreaLeft7->Text = sysConfig.m_nSubAreaLeft[7];

	editAreaTop0->Text = sysConfig.m_nSubAreaTop[0];
	editAreaTop1->Text = sysConfig.m_nSubAreaTop[1];
	editAreaTop2->Text = sysConfig.m_nSubAreaTop[2];
	editAreaTop3->Text = sysConfig.m_nSubAreaTop[3];
	editAreaTop4->Text = sysConfig.m_nSubAreaTop[4];
	editAreaTop5->Text = sysConfig.m_nSubAreaTop[5];
	editAreaTop6->Text = sysConfig.m_nSubAreaTop[6];
	editAreaTop7->Text = sysConfig.m_nSubAreaTop[7];

	editAreaWidth0->Text = sysConfig.m_nSubAreaWidth[0];
	editAreaWidth1->Text = sysConfig.m_nSubAreaWidth[1];
	editAreaWidth2->Text = sysConfig.m_nSubAreaWidth[2];
	editAreaWidth3->Text = sysConfig.m_nSubAreaWidth[3];
	editAreaWidth4->Text = sysConfig.m_nSubAreaWidth[4];
	editAreaWidth5->Text = sysConfig.m_nSubAreaWidth[5];
	editAreaWidth6->Text = sysConfig.m_nSubAreaWidth[6];
	editAreaWidth7->Text = sysConfig.m_nSubAreaWidth[7];

	editAreaHeight0->Text = sysConfig.m_nSubAreaHeight[0];
	editAreaHeight1->Text = sysConfig.m_nSubAreaHeight[1];
	editAreaHeight2->Text = sysConfig.m_nSubAreaHeight[2];
	editAreaHeight3->Text = sysConfig.m_nSubAreaHeight[3];
	editAreaHeight4->Text = sysConfig.m_nSubAreaHeight[4];
	editAreaHeight5->Text = sysConfig.m_nSubAreaHeight[5];
	editAreaHeight6->Text = sysConfig.m_nSubAreaHeight[6];
	editAreaHeight7->Text = sysConfig.m_nSubAreaHeight[7];
	comboBoxAreaCount->ItemIndex = sysConfig.m_nSubAreaCount - 1;
#endif
	comboBoxAreaCountChange(this);

	if (sysConfig.m_nCardType == CARD_TYPE_GS7022B ||
		sysConfig.m_nCardType == CARD_TYPE_GS2012A_S ||
		sysConfig.m_nCardType == CARD_TYPE_GS9020S ||
		sysConfig.m_nCardType == CARD_TYPE_GS7020HS ||
		sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64
		)
	{
		editSmsUser0->Text = sysConfig.m_szSmsUser[0];
		editSmsUser1->Text = sysConfig.m_szSmsUser[1];
		editSmsUser2->Text = sysConfig.m_szSmsUser[2];
		editSmsUser3->Text = sysConfig.m_szSmsUser[3];
		editSmsUser4->Text = sysConfig.m_szSmsUser[4];
		editSmsUser5->Text = sysConfig.m_szSmsUser[5];
		editSmsUser6->Text = sysConfig.m_szSmsUser[6];
		editSmsUser7->Text = sysConfig.m_szSmsUser[7];
		checkEchoSms0->Checked = sysConfig.m_bEchoSms[0];
		checkEchoSms1->Checked = sysConfig.m_bEchoSms[1];
		checkEchoSms2->Checked = sysConfig.m_bEchoSms[2];
		checkEchoSms3->Checked = sysConfig.m_bEchoSms[3];
		checkEchoSms4->Checked = sysConfig.m_bEchoSms[4];
		checkEchoSms5->Checked = sysConfig.m_bEchoSms[5];
		checkEchoSms6->Checked = sysConfig.m_bEchoSms[6];
		checkEchoSms7->Checked = sysConfig.m_bEchoSms[7];

		editShowTime->Text = sysConfig.m_nSmsActShowTime;
		comboEnterStyle->ItemIndex = sysConfig.m_nSmsActEnterStyle;
		editEnterSpeed->Text = sysConfig.m_nSmsActEnterSpeed;
		comboExitStyle->ItemIndex = sysConfig.m_nSmsActExitStyle;
		editExitSpeed->Text = sysConfig.m_nSmsActExitSpeed;
		editRepeatCount->Text = sysConfig.m_nSmsActRepeatCount;
		switch (sysConfig.m_nSmsActFontSize)
		{
		case 16:
			comboFont->ItemIndex = 0;
			break;
		case 32:
			comboFont->ItemIndex = 1;
			break;
		case 48:
			comboFont->ItemIndex = 2;
			break;
		case 64:
			comboFont->ItemIndex = 3;
			break;
		default:
			comboFont->ItemIndex = 0;
			break;
		}
	}

	if (sysConfig.m_nCardType == CARD_TYPE_GS2012A_S)
		editVoiceArea->Text = sysConfig.m_nVoiceOutputAreaIndex;

	switch (sysConfig.m_nCardType)
	{
	case CARD_TYPE_GS7020BF:
	case CARD_TYPE_GS7020BF_HEB:
	case CARD_TYPE_GS9026:
	case CARD_TYPE_GS7020BF_W:
	case CARD_TYPE_GS9020F_GPS:
	case CARD_TYPE_GS7020HFA:
		checkActSecond->Enabled = false;
		checkActSupportRepeat->Enabled = false;
		checkSupportInsertPlay->Enabled = false;
		checkActSecond->Checked = true;
		checkActSupportRepeat->Checked = true;
		checkSupportInsertPlay->Checked = true;
		if (sysConfig.m_nCardType == CARD_TYPE_GS9026)
		{
			checkSupportLedSignCheck->Visible = true;
			checkSupportLedSignCheck->Top = checkSyncTimeHint->Top;
		}
		break;
	case CARD_TYPE_YL2010A:
	case CARD_TYPE_YL2010A_GPS_JTX:
	case CARD_TYPE_YL2010C:
	case CARD_TYPE_YL2010AR:
	case CARD_TYPE_YL2010AF:
		checkActSecond->Enabled = false;
		checkActSecond->Checked = true;
		break;
	case CARD_TYPE_GS7020BX:
		checkSyncTimeHint->Visible = true;
		break;
	case CARD_TYPE_GS9020S:
		sheetSubArea->TabVisible = false;
		timeStart4->Visible = false;
		timeStart5->Visible = false;
		timeStart6->Visible = false;
		timeStart7->Visible = false;
		comboBoxLightValue4->Visible = false;
		comboBoxLightValue5->Visible = false;
		comboBoxLightValue6->Visible = false;
		comboBoxLightValue7->Visible = false;
		lbLight4->Visible = false;
		lbLight5->Visible = false;
		lbLight6->Visible = false;
		lbLight7->Visible = false;
		editSmsUser4->Visible = false;
		editSmsUser5->Visible = false;
		editSmsUser6->Visible = false;
		editSmsUser7->Visible = false;
		checkEchoSms4->Visible = false;
		checkEchoSms5->Visible = false;
		checkEchoSms6->Visible = false;
		checkEchoSms7->Visible = false;
		break;
	case CARD_TYPE_GS7020HS:
		sheetSubArea->TabVisible = false;
		break;
	case CARD_TYPE_PD102B:
		checkActSecond->Visible = false;
		checkActSupportRepeat->Visible = false;
		checkSupportInsertPlay->Visible = false;
		checkSupportWarnning->Visible = false;
		checkSyncPlay->Visible = false;
		checkByteReverse->Visible = false;
		checkSyncTimeHint->Visible = false;
		sheetSubArea->TabVisible = false;
		sheetAutoLightControl->TabVisible = false;
		cbBaudrate->Items->Delete(0);
		switch (sysConfig.m_nBaudRate)
		{
		case 57600:
			cbBaudrate->ItemIndex = 0; break;
		case 38400:
			cbBaudrate->ItemIndex = 1; break;
		case 19200:
			cbBaudrate->ItemIndex = 2; break;
		case 9600:
			cbBaudrate->ItemIndex = 3; break;
		case 4800:
			cbBaudrate->ItemIndex = 4; break;
		case 2400:
			cbBaudrate->ItemIndex = 5; break;
		default:
			cbBaudrate->ItemIndex = 0; break;
		}
		break;
	}

#ifdef XINJIANG_AUTO_WEB_DATA
	editWebDataId->Text = m_dtu->m_sWebDataId;
#else
	sheetWebData->TabVisible = false;
#endif

	editNewLocalIp->Text = m_dtu->m_sLocalIp;
	editLocalIp->Text = m_dtu->m_sLocalIp;
	editGateway->Text = m_dtu->m_sGateWay;
	editNetMask->Text = m_dtu->m_sSubMask;
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::btnOkClick(TObject *Sender)
{
	switch (sysConfig.m_nCardType)
	{
	case CARD_TYPE_WF2012B:
		m_dtu->m_sCode = editWifiDeviceCode->Text;
		m_dtu->m_nCardId = CARD_ID_GS7020B;
		m_dtu->m_sWifiSSID = editSSID->Text;
		m_dtu->m_sWifiPassword = editWifiPassword->Text;
		m_dtu->m_sWifiDnsServer = editWifiDnsServer->Text;
		break;
	case CARD_TYPE_YL2010A:
	case CARD_TYPE_YL2010A_GPS_JTX:
	case CARD_TYPE_YL2010C:
	case CARD_TYPE_YL2010AR:
	case CARD_TYPE_YL2010AF:
	case CARD_TYPE_PD102B:
		m_dtu->m_sCode = editDeviceCode->Text;
		m_dtu->m_nCardId = editCardId->Text.ToInt();
		break;
	default:
		m_dtu->m_sCode = editDeviceCode->Text;
		m_dtu->m_nCardId = CARD_ID_GS7020B;
		break;
	}

	sysConfig.m_nCardBaudRate = cbBaudrate->Text.ToInt();
	sysConfig.m_ulScanMode = comboBoxScanMode->ItemIndex;
	sysConfig.m_bByteReverse = checkByteReverse->Checked;
	if (sysConfig.m_nCardType == CARD_TYPE_WF2012B)
	{
		sysConfig.m_nSocketPort_Card = editWifiSocketPort_Card->Text.ToInt();
		sysConfig.m_sDataCenter = editWifiServer->Text;
	}
	else
	{
		sysConfig.m_nSocketPort_Card = editSocketPort_Card->Text.ToInt();
		sysConfig.m_sDataCenter = editDataCenter->Text;
	}
	sysConfig.m_bNegData = checkData->Checked;
	sysConfig.m_bNegOE = checkOE->Checked;
	sysConfig.m_bSupportLedSignCheck = checkSupportLedSignCheck->Checked;
	sysConfig.m_bDoubleColor = rbtnDoubleColor->Checked;
	sysConfig.m_bFullColor = rbtnFullColor->Checked;
	sysConfig.m_bSupportWarning = checkSupportWarnning->Checked;
#ifdef AUTO_SIZE_TEXT
	m_dtu->m_nScreenWidth = editScreenWidth->Text.ToInt();
	m_dtu->m_nScreenHeight = editScreenHeight->Text.ToInt();
#endif
	sysConfig.m_nScreenWidth = editScreenWidth->Text.ToInt();
	sysConfig.m_nScreenHeight = editScreenHeight->Text.ToInt();
	sysConfig.m_sAPN = editAPN->Text;
	sysConfig.m_bActSecond = checkActSecond->Checked;
	sysConfig.m_bActSupportRepeat = checkActSupportRepeat->Checked;
	sysConfig.m_bSupportInsertPlay = checkSupportInsertPlay->Checked;
	sysConfig.m_nMaxActCount = comboBoxMaxActCount->Text.ToInt();
	sysConfig.m_bActSupportSync = checkSyncPlay->Checked;
	sysConfig.m_bSupportCycleTimeHint = checkSyncTimeHint->Checked;
	sysConfig.m_bTCP = checkTCP->Checked;
	sysConfig.m_bLoginEcho = checkLoginEcho->Checked;
	if (sysConfig.m_nCardType == CARD_TYPE_GS2012A_S)
		sysConfig.m_nVoiceOutputAreaIndex = editVoiceArea->Text.ToInt();
	else
		sysConfig.m_nVoiceOutputAreaIndex = 0;

	SYSTEMTIME st;

	DateTimeToSystemTime(timeStart0->Time, st);
	sysConfig.m_nLightControlStartHour[0] = st.wHour;
	sysConfig.m_nLightControlStartMinute[0] = st.wMinute;

	DateTimeToSystemTime(timeStart1->Time, st);
	sysConfig.m_nLightControlStartHour[1] = st.wHour;
	sysConfig.m_nLightControlStartMinute[1] = st.wMinute;

	DateTimeToSystemTime(timeStart2->Time, st);
	sysConfig.m_nLightControlStartHour[2] = st.wHour;
	sysConfig.m_nLightControlStartMinute[2] = st.wMinute;

	DateTimeToSystemTime(timeStart3->Time, st);
	sysConfig.m_nLightControlStartHour[3] = st.wHour;
	sysConfig.m_nLightControlStartMinute[3] = st.wMinute;

	DateTimeToSystemTime(timeStart4->Time, st);
	sysConfig.m_nLightControlStartHour[4] = st.wHour;
	sysConfig.m_nLightControlStartMinute[4] = st.wMinute;

	DateTimeToSystemTime(timeStart5->Time, st);
	sysConfig.m_nLightControlStartHour[5] = st.wHour;
	sysConfig.m_nLightControlStartMinute[5] = st.wMinute;

	DateTimeToSystemTime(timeStart6->Time, st);
	sysConfig.m_nLightControlStartHour[6] = st.wHour;
	sysConfig.m_nLightControlStartMinute[6] = st.wMinute;

	DateTimeToSystemTime(timeStart7->Time, st);
	sysConfig.m_nLightControlStartHour[7] = st.wHour;
	sysConfig.m_nLightControlStartMinute[7] = st.wMinute;

	sysConfig.m_nLightControlValue[0] = comboBoxLightValue0->ItemIndex;
	sysConfig.m_nLightControlValue[1] = comboBoxLightValue1->ItemIndex;
	sysConfig.m_nLightControlValue[2] = comboBoxLightValue2->ItemIndex;
	sysConfig.m_nLightControlValue[3] = comboBoxLightValue3->ItemIndex;
	sysConfig.m_nLightControlValue[4] = comboBoxLightValue4->ItemIndex;
	sysConfig.m_nLightControlValue[5] = comboBoxLightValue5->ItemIndex;
	sysConfig.m_nLightControlValue[6] = comboBoxLightValue6->ItemIndex;
	sysConfig.m_nLightControlValue[7] = comboBoxLightValue7->ItemIndex;

#ifdef AUTO_SIZE_TEXT
	m_dtu->m_nSubAreaLeft[0] = editAreaLeft0->Text.ToInt();
	m_dtu->m_nSubAreaLeft[1] = editAreaLeft1->Text.ToInt();
	m_dtu->m_nSubAreaLeft[2] = editAreaLeft2->Text.ToInt();
	m_dtu->m_nSubAreaLeft[3] = editAreaLeft3->Text.ToInt();
	m_dtu->m_nSubAreaLeft[4] = editAreaLeft4->Text.ToInt();
	m_dtu->m_nSubAreaLeft[5] = editAreaLeft5->Text.ToInt();
	m_dtu->m_nSubAreaLeft[6] = editAreaLeft6->Text.ToInt();
	m_dtu->m_nSubAreaLeft[7] = editAreaLeft7->Text.ToInt();

	m_dtu->m_nSubAreaTop[0] = editAreaTop0->Text.ToInt();
	m_dtu->m_nSubAreaTop[1] = editAreaTop1->Text.ToInt();
	m_dtu->m_nSubAreaTop[2] = editAreaTop2->Text.ToInt();
	m_dtu->m_nSubAreaTop[3] = editAreaTop3->Text.ToInt();
	m_dtu->m_nSubAreaTop[4] = editAreaTop4->Text.ToInt();
	m_dtu->m_nSubAreaTop[5] = editAreaTop5->Text.ToInt();
	m_dtu->m_nSubAreaTop[6] = editAreaTop6->Text.ToInt();
	m_dtu->m_nSubAreaTop[7] = editAreaTop7->Text.ToInt();

	m_dtu->m_nSubAreaWidth[0] = editAreaWidth0->Text.ToInt();
	m_dtu->m_nSubAreaWidth[1] = editAreaWidth1->Text.ToInt();
	m_dtu->m_nSubAreaWidth[2] = editAreaWidth2->Text.ToInt();
	m_dtu->m_nSubAreaWidth[3] = editAreaWidth3->Text.ToInt();
	m_dtu->m_nSubAreaWidth[4] = editAreaWidth4->Text.ToInt();
	m_dtu->m_nSubAreaWidth[5] = editAreaWidth5->Text.ToInt();
	m_dtu->m_nSubAreaWidth[6] = editAreaWidth6->Text.ToInt();
	m_dtu->m_nSubAreaWidth[7] = editAreaWidth7->Text.ToInt();

	m_dtu->m_nSubAreaHeight[0] = editAreaHeight0->Text.ToInt();
	m_dtu->m_nSubAreaHeight[1] = editAreaHeight1->Text.ToInt();
	m_dtu->m_nSubAreaHeight[2] = editAreaHeight2->Text.ToInt();
	m_dtu->m_nSubAreaHeight[3] = editAreaHeight3->Text.ToInt();
	m_dtu->m_nSubAreaHeight[4] = editAreaHeight4->Text.ToInt();
	m_dtu->m_nSubAreaHeight[5] = editAreaHeight5->Text.ToInt();
	m_dtu->m_nSubAreaHeight[6] = editAreaHeight6->Text.ToInt();
	m_dtu->m_nSubAreaHeight[7] = editAreaHeight7->Text.ToInt();

	m_dtu->m_nSubAreaCount = comboBoxAreaCount->ItemIndex + 1;

	if (sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64)
	{
		m_dtu->m_nSubAreaCount = 1;
		/*sysConfig.m_nSubAreaLeft[0] = 0;
		sysConfig.m_nSubAreaTop[0] = 0;
		sysConfig.m_nSubAreaWidth[0] = sysConfig.m_nScreenWidth;
		sysConfig.m_nSubAreaHeight[0] = sysConfig.m_nScreenHeight;
		*/
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9020S || sysConfig.m_nCardType == CARD_TYPE_GS7020HS || sysConfig.m_nCardType == CARD_TYPE_PD102B)
	{
		m_dtu->m_nSubAreaCount = 1;
		m_dtu->m_nSubAreaLeft[0] = 0;
		m_dtu->m_nSubAreaTop[0] = 0;
		m_dtu->m_nSubAreaWidth[0] = m_dtu->m_nScreenWidth;
		m_dtu->m_nSubAreaHeight[0] = m_dtu->m_nScreenHeight;
	}
#endif

	sysConfig.m_nSubAreaLeft[0] = editAreaLeft0->Text.ToInt();
	sysConfig.m_nSubAreaLeft[1] = editAreaLeft1->Text.ToInt();
	sysConfig.m_nSubAreaLeft[2] = editAreaLeft2->Text.ToInt();
	sysConfig.m_nSubAreaLeft[3] = editAreaLeft3->Text.ToInt();
	sysConfig.m_nSubAreaLeft[4] = editAreaLeft4->Text.ToInt();
	sysConfig.m_nSubAreaLeft[5] = editAreaLeft5->Text.ToInt();
	sysConfig.m_nSubAreaLeft[6] = editAreaLeft6->Text.ToInt();
	sysConfig.m_nSubAreaLeft[7] = editAreaLeft7->Text.ToInt();

	sysConfig.m_nSubAreaTop[0] = editAreaTop0->Text.ToInt();
	sysConfig.m_nSubAreaTop[1] = editAreaTop1->Text.ToInt();
	sysConfig.m_nSubAreaTop[2] = editAreaTop2->Text.ToInt();
	sysConfig.m_nSubAreaTop[3] = editAreaTop3->Text.ToInt();
	sysConfig.m_nSubAreaTop[4] = editAreaTop4->Text.ToInt();
	sysConfig.m_nSubAreaTop[5] = editAreaTop5->Text.ToInt();
	sysConfig.m_nSubAreaTop[6] = editAreaTop6->Text.ToInt();
	sysConfig.m_nSubAreaTop[7] = editAreaTop7->Text.ToInt();

	sysConfig.m_nSubAreaWidth[0] = editAreaWidth0->Text.ToInt();
	sysConfig.m_nSubAreaWidth[1] = editAreaWidth1->Text.ToInt();
	sysConfig.m_nSubAreaWidth[2] = editAreaWidth2->Text.ToInt();
	sysConfig.m_nSubAreaWidth[3] = editAreaWidth3->Text.ToInt();
	sysConfig.m_nSubAreaWidth[4] = editAreaWidth4->Text.ToInt();
	sysConfig.m_nSubAreaWidth[5] = editAreaWidth5->Text.ToInt();
	sysConfig.m_nSubAreaWidth[6] = editAreaWidth6->Text.ToInt();
	sysConfig.m_nSubAreaWidth[7] = editAreaWidth7->Text.ToInt();

	sysConfig.m_nSubAreaHeight[0] = editAreaHeight0->Text.ToInt();
	sysConfig.m_nSubAreaHeight[1] = editAreaHeight1->Text.ToInt();
	sysConfig.m_nSubAreaHeight[2] = editAreaHeight2->Text.ToInt();
	sysConfig.m_nSubAreaHeight[3] = editAreaHeight3->Text.ToInt();
	sysConfig.m_nSubAreaHeight[4] = editAreaHeight4->Text.ToInt();
	sysConfig.m_nSubAreaHeight[5] = editAreaHeight5->Text.ToInt();
	sysConfig.m_nSubAreaHeight[6] = editAreaHeight6->Text.ToInt();
	sysConfig.m_nSubAreaHeight[7] = editAreaHeight7->Text.ToInt();

	sysConfig.m_nSubAreaCount = comboBoxAreaCount->ItemIndex + 1;

	if (sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64)
	{
		sysConfig.m_nSubAreaCount = 1;
		/*sysConfig.m_nSubAreaLeft[0] = 0;
		sysConfig.m_nSubAreaTop[0] = 0;
		sysConfig.m_nSubAreaWidth[0] = sysConfig.m_nScreenWidth;
		sysConfig.m_nSubAreaHeight[0] = sysConfig.m_nScreenHeight;
		*/
	}
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9020S || sysConfig.m_nCardType == CARD_TYPE_GS7020HS || sysConfig.m_nCardType == CARD_TYPE_PD102B)
	{
		sysConfig.m_nSubAreaCount = 1;
		sysConfig.m_nSubAreaLeft[0] = 0;
		sysConfig.m_nSubAreaTop[0] = 0;
		sysConfig.m_nSubAreaWidth[0] = sysConfig.m_nScreenWidth;
		sysConfig.m_nSubAreaHeight[0] = sysConfig.m_nScreenHeight;
	}

	if (sysConfig.m_nCardType == CARD_TYPE_GS7022B ||
		sysConfig.m_nCardType == CARD_TYPE_GS2012A_S ||
		sysConfig.m_nCardType == CARD_TYPE_GS9020S ||
		sysConfig.m_nCardType == CARD_TYPE_GS7020HS ||
		sysConfig.m_nCardType == CARD_TYPE_GS2012AS_64)
	{
		sysConfig.m_szSmsUser[0] = editSmsUser0->Text;
		sysConfig.m_szSmsUser[1] = editSmsUser1->Text;
		sysConfig.m_szSmsUser[2] = editSmsUser2->Text;
		sysConfig.m_szSmsUser[3] = editSmsUser3->Text;
		sysConfig.m_szSmsUser[4] = editSmsUser4->Text;
		sysConfig.m_szSmsUser[5] = editSmsUser5->Text;
		sysConfig.m_szSmsUser[6] = editSmsUser6->Text;
		sysConfig.m_szSmsUser[7] = editSmsUser7->Text;
		sysConfig.m_bEchoSms[0] = checkEchoSms0->Checked;
		sysConfig.m_bEchoSms[1] = checkEchoSms1->Checked;
		sysConfig.m_bEchoSms[2] = checkEchoSms2->Checked;
		sysConfig.m_bEchoSms[3] = checkEchoSms3->Checked;
		sysConfig.m_bEchoSms[4] = checkEchoSms4->Checked;
		sysConfig.m_bEchoSms[5] = checkEchoSms5->Checked;
		sysConfig.m_bEchoSms[6] = checkEchoSms6->Checked;
		sysConfig.m_bEchoSms[7] = checkEchoSms7->Checked;

		sysConfig.m_nSmsActShowTime = editShowTime->Text.ToInt();
		sysConfig.m_nSmsActEnterStyle = comboEnterStyle->ItemIndex;
		sysConfig.m_nSmsActEnterSpeed = editEnterSpeed->Text.ToInt();
		sysConfig.m_nSmsActExitStyle = comboExitStyle->ItemIndex;
		sysConfig.m_nSmsActExitSpeed = editExitSpeed->Text.ToInt();
		sysConfig.m_nSmsActRepeatCount = editRepeatCount->Text.ToInt();
		sysConfig.m_nSmsActFontSize = comboFont->Text.ToInt();
	}

	sysConfig.m_sGpsServer = editGpsServer->Text;
	sysConfig.m_nGpsSocketPort = editGpsSocketPort->Text.ToInt();
	sysConfig.m_nGpsReportInterval = editGpsReportInterval->Text.ToInt();
	sysConfig.m_sGpsId = editGpsPseudoIp->Text;

	m_dtu->m_sLocalIp = editLocalIp->Text;
	m_dtu->m_sGateWay = editGateway->Text;
	m_dtu->m_sSubMask = editNetMask->Text;

#ifdef XINJIANG_AUTO_WEB_DATA
	m_dtu->m_sWebDataId = editWebDataId->Text;
#endif

	m_dtu->SaveConfig();
}
//---------------------------------------------------------------------------

int TfrmConfig::ReciveUdpData(BYTE* pRxBuf, int nWantLen)
{
#pragma pack(1)
	struct
	{
		char  szGsmCode[12];
		BYTE  ucFrameType;
	}stHead;
#pragma pack()
	TIdBytes buf;
	BYTE* p;
	int nLen, i;
	buf.set_length(2048);

	if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
	{
		for (;;)
		{
			nLen = udpSocket->ReceiveBuffer(buf, 0);
			if (nLen == nWantLen)
			{
				for (i = 0; i < nWantLen; i++)
					pRxBuf[i] = buf[i];
				return nLen;
			}
			else if (nLen <= 0)
				break;
		}
	}
	else
	{
		for (;;)
		{
			nLen = udpSocket->ReceiveBuffer(buf, 0);

			if (nLen <= 0)
				break;

			if (nLen < (int)(sizeof(stHead) + nWantLen))
				continue;

			p = (BYTE*)&stHead;
			for (i = 0; i < (int)sizeof(stHead); i++)
				*p++ = buf[i];

			if (stHead.ucFrameType != FT_DATA)
				continue;

			size_t nStrLen = 0;
			for (i = 0; i < 12; i++)
			{
				if (stHead.szGsmCode[i])
					nStrLen++;
				else
					break;
			}
			if (nStrLen > sizeof(stHead.szGsmCode) - 1)
				continue;

			if (::strcmp(m_dtu->m_sCode.c_str(), stHead.szGsmCode) == 0)
			{
				for (i = 0; i < nWantLen; i++)
					pRxBuf[i] = buf[i + sizeof(stHead)];
				return nLen;
			}
		}
	}

	return 0;
}
//---------------------------------------------------------------------------

void TfrmConfig::SendData_UDP(char* szDtuCode, int nLen, BYTE* p)
{
#pragma pack(1)
	typedef struct
	{
		char szUserName[12];
		BYTE ucFrameType;
		char szPassword[16];
		char szDtuCode[12];
	}TX_DATA_HEADER;
#pragma pack()
	BYTE buf[2000];
	size_t i;

	if (sysConfig.m_nCardType == CARD_TYPE_SN2P)
	{
		TIdBytes ar;
		ar.set_length(nLen);
		for (i = 0; i < nLen; i++)
			ar[i] = p[i];
		if (checkBroadcast->Checked)
			udpSocket->SendBuffer("255.255.255.255", 3700, ar);
		else
		{
			try
			{
				udpSocket->SendBuffer(editLocalIp->Text, 3700, ar);
			}
			catch (Exception &ex)
			{
			}
		}
	}
	else
	{
		TX_DATA_HEADER* pHeader = (TX_DATA_HEADER*)buf;
		::strcpy(pHeader->szUserName, AnsiString(sysConfig.m_sUserName).c_str());
		for (i = 0; i < ::strlen(pHeader->szUserName); i++)
			pHeader->szUserName[i] ^= 0xAA;
		::strcpy(pHeader->szPassword, AnsiString(sysConfig.m_sPassword).c_str());
		for (i = 0; i < ::strlen(pHeader->szPassword); i++)
			pHeader->szPassword[i] ^= 0xAA;
		::strcpy(pHeader->szDtuCode, szDtuCode);
		pHeader->ucFrameType = FT_DATA;
		memcpy(buf + sizeof(TX_DATA_HEADER), p, nLen);

		TIdBytes ar;
		ar.set_length(nLen + sizeof(TX_DATA_HEADER));
		for (i = 0; i < nLen + sizeof(TX_DATA_HEADER); i++)
			ar[i] = buf[i];
		udpSocket->SendBuffer(ar);
	}
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCommand_StopDisplay()
{
	BYTE buf[2048];

	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
	{
		MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)buf;

		pHead->usHeadTag = MMI_START_ID;
		pHead->ucCardId = m_nCardId;
		pHead->ucCmd = MMI_CMD_STOP_DISPLAY;
		pHead->usLen = 0;

		buf[sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD_HEB) + pHead->usLen, buf);
		if (sysConfig.m_nCommMethod == 0)
			m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
		else
			SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), pHead->usLen + sizeof(MMI_CMD_HEAD_HEB) + 1, buf);
	}
	else
	{
		MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

		pHead->usHeadTag = MMI_START_ID;
		pHead->ucCardId = m_nCardId;
		pHead->ucCmd = MMI_CMD_STOP_DISPLAY;
		pHead->usLen = 0;

		buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
		if (sysConfig.m_nCommMethod == 0)
			m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
		else
			SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	}
}
//---------------------------------------------------------------------------

bool TfrmConfig::OpenComm()
{
	if (sysConfig.m_nCommMethod == 0)
	{
		m_ComPort.SetPortName(sysConfig.m_sComPort);
		m_ComPort.SetBaudRate(sysConfig.m_nBaudRate);
		if (!m_ComPort.Open())
		{
			if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
			{
				UnicodeString sInfo = L"串口:";
				sInfo += sysConfig.m_sComPort;
				sInfo += L" 打开失败";
				Application->MessageBox(sInfo.c_str(), L"配置", MB_OK | MB_ICONWARNING);
			}
			else
			{
				UnicodeString sInfo = L" Serial Port: ";
				sInfo += sysConfig.m_sComPort;
				sInfo += L" Open Failed";
				Application->MessageBox(sInfo.c_str(), L"Card Configuration", MB_OK | MB_ICONWARNING);
			}
			return false;
		}
	}
	else
	{
		udpSocket->Host = sysConfig.m_sServer;
		udpSocket->Port = sysConfig.m_nSocketPort;
		udpSocket->Active = true;
	}

	return true;
}
//---------------------------------------------------------------------------

void TfrmConfig::StartComm()
{
	m_nTimeout = 0;
	m_nRetries = 0;
	m_nRxStep = 0;
	PageControl->Enabled = false;
	btnReadConfig->Enabled = false;
	btnWriteConfig->Enabled = false;
	btnOk->Enabled = false;
}
//---------------------------------------------------------------------------

void TfrmConfig::EndComm()
{
	udpSocket->Active = false;
	m_ComPort.Close();
	PageControl->Enabled = true;
	btnReadConfig->Enabled = true;
	btnWriteConfig->Enabled = true;
	btnOk->Enabled = true;
}
//---------------------------------------------------------------------------

BYTE TfrmConfig::CalcChecksum(int nLen, BYTE* p)
{
	BYTE uc = 0;
	int i;

	for (i = 0; i < nLen; i++)
		uc += *p++;
	uc ^= 0xFF;

	return uc;
}
//---------------------------------------------------------------------------

void TfrmConfig::Encrypt(int nLen, BYTE* p)
{
	while (nLen)
	{
		*p ^= (m_ucKey1 ^ 0x23) ; p++; nLen--;
		if (nLen)
		{
			*p ^= (m_ucKey2 ^ 0xFB); p++; nLen--;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::btnSearchCardClick(TObject *Sender)
{
	m_nCardId = editCardId->Text.ToInt();
	m_nSearchCount = 0;

	if (!OpenComm())
		return;

	::Sleep(20);
	StartComm();
	SendCommand_Echo();

	timerSearchCard->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfig::timerSearchCardTimer(TObject *Sender)
{
	BYTE buf[1024];

	if (sysConfig.m_nCommMethod)
	{
		int nRxLen = ReciveUdpData(buf, sizeof(MMI_CMD_HEAD) + 1);

		if (nRxLen >= (int)(sizeof(MMI_CMD_HEAD) + 1))
		{
			MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

			if ((pHead->usHeadTag == MMI_START_ID) &&
				 (pHead->ucCardId == m_nCardId) &&
				 (pHead->ucCmd == MMI_CMD_ECHO) &&
				 (pHead->usLen == 0) &&
				 (CalcChecksum(sizeof(MMI_CMD_HEAD), buf) == buf[sizeof(MMI_CMD_HEAD)]))
			{
				timerSearchCard->Enabled = false;
				EndComm();
				if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
					Application->MessageBox(L"查找成功。", L"配置", MB_OK | MB_ICONINFORMATION);
				else
					Application->MessageBox(L"Searcing Card OK。", L"Card Configuration", MB_OK | MB_ICONINFORMATION);
				return;
			}
		}

		m_nTimeout += timerReadConfig->Interval;
		if (m_nTimeout > sysConfig.m_nTimeout_Network * 1000)
		{
			m_nSearchCount++;
			if (m_nSearchCount < 256)
			{
				m_nCardId++;
				m_nCardId &= 255;
				editCardId->Text = m_nCardId;
				m_nTimeout = 0;
				m_nRxStep = 0;
				SendCommand_Echo();
			}
			else
			{
				m_nCardId++;
				m_nCardId &= 255;
				editCardId->Text = m_nCardId;
				timerSearchCard->Enabled = false;
				EndComm();
				if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
					Application->MessageBox(L"查找失败!", L"配置", MB_OK | MB_ICONWARNING);
				else
					Application->MessageBox(L"Failed to Search Card!", L"Card Configuration", MB_OK | MB_ICONWARNING);
			}
		}

		return;
	}

	while (m_ComPort.GetInQueue())
	{
		switch (m_nRxStep)
		{
		case 0:
			m_ComPort.Read(1, m_dtu->m_ucRxBuf);
			if (m_dtu->m_ucRxBuf[0] == MMI_START_ID1)
				m_nRxStep = 1;
			break;
		case 1:
			m_ComPort.Read(1, m_dtu->m_ucRxBuf + 1);
			if (m_dtu->m_ucRxBuf[1] == MMI_START_ID2)
				m_nRxStep = 2;
			else
				m_nRxStep = 0;
			break;
		default:
			if (m_ComPort.GetInQueue() >= (int)(sizeof(MMI_CMD_HEAD) - 1))
			{
				buf[0] = m_dtu->m_ucRxBuf[0];
				buf[1] = m_dtu->m_ucRxBuf[1];
				m_ComPort.Read(sizeof(MMI_CMD_HEAD) - 1, buf + 2);
				MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

				if ((pHead->usHeadTag == MMI_START_ID) &&
					 (pHead->ucCardId == m_nCardId) &&
					 (pHead->ucCmd == MMI_CMD_ECHO) &&
					 (pHead->usLen == 0) &&
					 (CalcChecksum(sizeof(MMI_CMD_HEAD), buf) == buf[sizeof(MMI_CMD_HEAD)]))
				{
					timerSearchCard->Enabled = false;
					EndComm();
					if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
						Application->MessageBox(L"查找成功。", L"配置", MB_OK | MB_ICONINFORMATION);
					else
						Application->MessageBox(L"Searcing Card OK。", L"Card Configuration", MB_OK | MB_ICONINFORMATION);
					return;
				}
				else
					m_nRxStep = 0;
			}
			break;
		}
		if (m_nRxStep == 2)
			break;
	}

	m_nTimeout += timerSearchCard->Interval;
	if (m_nTimeout >= sysConfig.m_nTimeout_SerialPort)
	{
		m_nRetries++;
		if (m_nRetries < sysConfig.m_nRetry_SerialPort)
		{
			m_nTimeout = 0;
			SendCommand_Echo();
		}
		else
		{
			m_nSearchCount++;
			if (m_nSearchCount < 256)
			{
				m_nCardId++;
				m_nCardId &= 255;
				editCardId->Text = m_nCardId;
				m_nTimeout = 0;
				m_nRetries = 0;
				m_nRxStep = 0;
				SendCommand_Echo();
			}
			else
			{
				m_nCardId++;
				m_nCardId &= 255;
				editCardId->Text = m_nCardId;
				timerSearchCard->Enabled = false;
				EndComm();
				if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
					Application->MessageBox(L"查找失败!", L"配置", MB_OK | MB_ICONWARNING);
				else
					Application->MessageBox(L"Failed to Search Card!", L"Card Configuration", MB_OK | MB_ICONWARNING);
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmConfig::SendCommand_Echo()
{
	BYTE buf[2048];
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)buf;

	pHead->usHeadTag = MMI_START_ID;
	pHead->ucCardId = m_nCardId;
	pHead->ucCmd = MMI_CMD_ECHO;
	pHead->usLen = 0;

	buf[sizeof(MMI_CMD_HEAD) + pHead->usLen] = CalcChecksum(sizeof(MMI_CMD_HEAD) + pHead->usLen, buf);
	if (sysConfig.m_nCommMethod == 0)
		m_ComPort.Write(pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
	else
		SendData_UDP(AnsiString(editDeviceCode->Text).c_str(), pHead->usLen + sizeof(MMI_CMD_HEAD) + 1, buf);
}
//---------------------------------------------------------------------------

void TfrmConfig::WriteComPort(int nLen, BYTE* p)
{
	while (nLen >= 32)
	{
		m_ComPort.Write(32, p);
		nLen -= 32;
		p += 32;
		Sleep(10);
	}

	if (nLen)
		m_ComPort.Write(nLen, p);
}

