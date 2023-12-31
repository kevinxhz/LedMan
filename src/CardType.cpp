//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "global.h"
#include "CardType.h"
#include "SysConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmCardType::TfrmCardType(TComponent* Owner, int nCardType)
	: TForm(Owner)
{
	m_nCardType = nCardType;

	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "控制卡型号选择";
		Label1->Caption = "请正确选择本软件管理的控制卡型号:";
		btnOk->Caption = "确定";
		btnCancel->Caption = "取消";
		lstCardType->Items->Strings[0] = "GS" + UnicodeString(7020) + "B/GS" + UnicodeString(7020) + "E(GPRS) CS" + UnicodeString(7020) + "B(CDMA)  LED 显示屏控制卡";
		lstCardType->Items->Strings[1] = "GS" + UnicodeString(7030) + "B(GPRS) CS" + UnicodeString(7030) + "B(CDMA) LED 显示屏控制卡";
		lstCardType->Items->Strings[2] = "GS" + UnicodeString(2012) + "A(GPRS) CS" + UnicodeString(2012) + "A(CDMA) LED 显示屏控制卡";
		lstCardType->Items->Strings[3] = "YL" + UnicodeString(2010) + "A RS232/RS485 LED 显示屏控制卡";
		lstCardType->Items->Strings[4] = "YL2010AF RS232/RS485 LED 显示屏控制卡(动态分区)";
		lstCardType->Items->Strings[5] = "GS7020BF/GS7020EF GPRS LED 显示屏控制卡(动态分区)";
		lstCardType->Items->Strings[6] = "YL2010AR RS232/RS485 LED 显示屏控制(支持串口命令点播)";
		lstCardType->Items->Strings[7] = "PD102B RS232/RS485 图文条屏控制卡";
		lstCardType->Items->Strings[8] = "GS7020EF-H 公交车专用 GPRS LED 显示屏控制卡(动态分区)";
		lstCardType->Items->Strings[9] = "GS7020B(E)F-W 公交车专用 GPRS LED 显示屏控制卡(动态分区, 支持报站)";
		lstCardType->Items->Strings[10] = "GS7020BX/GS7020EX 多时段同步显示冠名报时车载屏专用控制卡";
		lstCardType->Items->Strings[11] = "GS7020BT/GS7020ET(GPRS) CS7020BT(CDMA)  LED 显示屏控制卡(按钮启动显示测试)";
		lstCardType->Items->Strings[12] = "GS7030BV2 GPRS LED 显示屏控制卡";
		lstCardType->Items->Strings[13] = "GS7022B/E/H(GPRS) CS7022B/H(CDMA)  LED 显示屏控制卡(同时支持短信)";
		lstCardType->Items->Strings[14] = "GS7020HQ/GS7020BQ/GS7020EQ(GPRS) CS7020HQ/CS7020BQ(CDMA) 气象预警屏控制卡(支持4个16x16的预警图标显示板)";
		lstCardType->Items->Strings[15] = "GS7020F GPRS LED Sign Control Card (International Version)";
		lstCardType->Items->Strings[16] = "GS7020BP/GS7020EP(GPRS) CS7020BP(CDMA)  车位信息屏专用控制卡";
		lstCardType->Items->Strings[17] = "GS2012A-S GPRS+短信 LED 显示屏控制卡";
		lstCardType->Items->Strings[18] = "GS9026 GPRS 车载LED 显示屏控制卡(兼容GS7020EF-H)";
		lstCardType->Items->Strings[19] = "GS7024B/GS7024E(GPRS) CS7024B(CDMA) LED 显示屏控制卡(8MB FLASH)";
		lstCardType->Items->Strings[20] = "WF2012B WIFI/RS232 LED 显示屏控制卡";
		lstCardType->Items->Strings[21] = "GS2012AS-64 GS2012BS-64 64点高短信 LED 显示屏控制卡";
		lstCardType->Items->Strings[22] = "GS" + UnicodeString(2012) + "B(GPRS) CS" + UnicodeString(2012) + "B(CDMA) LED 显示屏控制卡";
		lstCardType->Items->Strings[23] = "GS" + UnicodeString(9020) + " GPRS 小条屏控制卡(兼容GS7020B GS7020E)";
		lstCardType->Items->Strings[24] = "YL2010C RS232/RS485 全彩(无灰度) LED 显示屏控制卡";
		lstCardType->Items->Strings[25] = "YL" + UnicodeString(2010) + "A RS232/RS485 LED 显示屏控制卡，JTX GPS汽车行驶记录仪专用";
		lstCardType->Items->Strings[26] = "GS" + UnicodeString(9020) + "S 小条屏短信控制卡";
		lstCardType->Items->Strings[27] = "GS" + UnicodeString(7020) + "H(GPRS) CS" + UnicodeString(7020) + "H(CDMA)  LED 显示屏控制卡";
		lstCardType->Items->Strings[28] = "GS" + UnicodeString(9025) + " GPRS 公交车站用32点高条屏控制卡(内置字库)";
		lstCardType->Items->Strings[29] = "GS" + UnicodeString(9023) + " GPRS 16点高小条屏控制卡(内置16点宋体和黑体字库)";
		lstCardType->Items->Strings[30] = "GS7020HS GS7020IS GSM 短信控制卡";
		lstCardType->Items->Strings[31] = "GS9020F-GPS 车载GPRS LED显示屏控制卡，支持外扩GPS定位模块";
		lstCardType->Items->Strings[32] = "GS7020HFA(GPRS)/CS7020HFA(CDMA) LED 显示屏控制卡(动态分区)";
		lstCardType->Items->Strings[33] = "SN2P 以太网/串口 车位信息显示屏控制卡";
		memoNote->Visible = true;
	}
	else
	{
		Caption = "Card Type";
		Label1->Caption = "Please select correct card type:";
		btnOk->Caption = "OK";
		btnCancel->Caption = "Cancel";
		lstCardType->Items->Strings[0] = "GS7020B/GS7020E(GPRS) CS7020B(CDMA)";
		lstCardType->Items->Strings[1] = "GS7030B(GPRS) CS7030B(CDMA)";
		lstCardType->Items->Strings[2] = "GS2012A(GPRS) CS2012A(CDMA)";
		lstCardType->Items->Strings[3] = "YL2010A RS232/RS485";
		lstCardType->Items->Strings[4] = "YL2010AF RS232/RS485, Support Dynamic Area Combination";
		lstCardType->Items->Strings[5] = "GS7020BF/GS7020EF GPRS, Support Dynamic Area Combination";
		lstCardType->Items->Strings[6] = "YL2010AR RS232/RS485 Support Mannual Select Displaying Act";
		lstCardType->Items->Strings[7] = "PD102B RS232/RS485 Tiny Card";
		lstCardType->Items->Strings[8] = "GS7020EF-H GPRS, Special for Bus, support Panel Test";
		lstCardType->Items->Strings[9] = "GS7020B(E)F-W GPRS, Special for Bus";
		lstCardType->Items->Strings[10] = "GS7020BX/GS7020EX GPRS,  Special for Taxi";
		lstCardType->Items->Strings[11] = "GS7020BT/GS7020ET(GPRS) CS7020BT(CDMA)";
		lstCardType->Items->Strings[12] = "GS7030BV2 GPRS";
		lstCardType->Items->Strings[13] = "GS7022B/GS7022E(GPRS) CS7022B(CDMA), Support SMS";
		lstCardType->Items->Strings[14] = "GS7020HQ/GS7020BQ/GS7020EQ(GPRS) CS7020HQ/CS7020BQ(CDMA)";
		lstCardType->Items->Strings[15] = "GS7020F GPRS LED Sign Control Card (International Version)";
		lstCardType->Items->Strings[16] = "GS7020BP/GS7020EP(GPRS) CS7020BP(CDMA) LED Sign Control Card for Park Info.";
		lstCardType->Items->Strings[17] = "GS2012A-S GPRS and SMS";
		lstCardType->Items->Strings[18] = "GS9026 GPRS, Special for Bus/Taxi, support Panel Test";
		lstCardType->Items->Strings[19] = "GS7024B/GS7024E(GPRS) CS7024B(CDMA) (8MB FLASH)";
		lstCardType->Items->Strings[20] = "WF2012B WIFI/RS232";
		lstCardType->Items->Strings[21] = "GS2012AS-64 GS2012BS-64 SMS, Height: 64";
		lstCardType->Items->Strings[22] = "GS2012B(GPRS) CS2012B(CDMA)";
		lstCardType->Items->Strings[23] = "GS9020 GPRS";
		lstCardType->Items->Strings[24] = "YL2010C RS232/RS485 Seven Colors";
		lstCardType->Items->Strings[25] = "YL2010A RS232/RS485，JTX GPS";
		lstCardType->Items->Strings[26] = "GS9020S SMS";
		lstCardType->Items->Strings[27] = "GS7020H(GPRS) CS7020H(CDMA)";
		lstCardType->Items->Strings[28] = "GS9025 GPRS Bus Station";
		lstCardType->Items->Strings[29] = "GS9023 GPRS Text Based";
		lstCardType->Items->Strings[30] = "GS7020HS GS7020IS SMS";
		lstCardType->Items->Strings[31] = "GS9020F-GPS";
		lstCardType->Items->Strings[32] = "GS7020HFA";
		lstCardType->Items->Strings[33] = "SN2P";
		memoNote->Visible = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmCardType::FormCreate(TObject *Sender)
{
	switch (m_nCardType)
	{
		case CARD_TYPE_GS7020B:
			lstCardType->ItemIndex = 0;
			break;
		case CARD_TYPE_GS7030B:
			lstCardType->ItemIndex = 1;
			break;
		case CARD_TYPE_GS2012A:
			lstCardType->ItemIndex = 2;
			break;
		case CARD_TYPE_YL2010A:
			lstCardType->ItemIndex = 3;
			break;
		case CARD_TYPE_YL2010AF:
			lstCardType->ItemIndex = 4;
			break;
		case CARD_TYPE_GS7020BF:
			lstCardType->ItemIndex = 5;
			break;
		case CARD_TYPE_YL2010AR:
			lstCardType->ItemIndex = 6;
			break;
		case CARD_TYPE_PD102B:
			lstCardType->ItemIndex = 7;
			break;
		case CARD_TYPE_GS7020BF_HEB:
			lstCardType->ItemIndex = 8;
			break;
		case CARD_TYPE_GS7020BF_W:
			lstCardType->ItemIndex = 9;
			break;
		case CARD_TYPE_GS7020BX:
			lstCardType->ItemIndex = 10;
			break;
		case CARD_TYPE_GS7020BT:
			lstCardType->ItemIndex = 11;
			break;
		case CARD_TYPE_GS7030BV2:
			lstCardType->ItemIndex = 12;
			break;
		case CARD_TYPE_GS7022B:
			lstCardType->ItemIndex = 13;
			break;
		case CARD_TYPE_GS7020BQ:
			lstCardType->ItemIndex = 14;
			break;
		case CARD_TYPE_GS7020F:
			lstCardType->ItemIndex = 15;
			break;
		case CARD_TYPE_GS7020BP:
			lstCardType->ItemIndex = 16;
			break;
		case CARD_TYPE_GS2012A_S:
			lstCardType->ItemIndex = 17;
			break;
		case CARD_TYPE_GS9026:
			lstCardType->ItemIndex = 18;
			break;
		case CARD_TYPE_GS7024E:
			lstCardType->ItemIndex = 19;
			break;
		case CARD_TYPE_WF2012B:
			lstCardType->ItemIndex = 20;
			break;
		case CARD_TYPE_GS2012AS_64:
			lstCardType->ItemIndex = 21;
			break;
		case CARD_TYPE_GS2012B:
			lstCardType->ItemIndex = 22;
			break;
		case CARD_TYPE_GS9020:
			lstCardType->ItemIndex = 23;
			break;
		case CARD_TYPE_YL2010C:
			lstCardType->ItemIndex = 24;
			break;
		case CARD_TYPE_YL2010A_GPS_JTX:
			lstCardType->ItemIndex = 25;
			break;
		case CARD_TYPE_GS9020S:
			lstCardType->ItemIndex = 26;
			break;
		case CARD_TYPE_GS7020H:
			lstCardType->ItemIndex = 27;
			break;
		case CARD_TYPE_GS9025:
			lstCardType->ItemIndex = 28;
			break;
		case CARD_TYPE_GS9023:
			lstCardType->ItemIndex = 29;
			break;
		case CARD_TYPE_GS7020HS:
			lstCardType->ItemIndex = 30;
			break;
		case CARD_TYPE_GS9020F_GPS:
			lstCardType->ItemIndex = 31;
			break;
		case CARD_TYPE_GS7020HFA:
			lstCardType->ItemIndex = 32;
			break;
		case CARD_TYPE_SN2P:
			lstCardType->ItemIndex = 33;
			break;
		default:
			lstCardType->ItemIndex = 0;
			break;
	}

	lstCardTypeClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmCardType::lstCardTypeClick(TObject *Sender)
{
	switch (lstCardType->ItemIndex)
	{
	case 0:	//GS7020B
		m_nCardType = CARD_TYPE_GS7020B;
		memoNote->Text = "GPRS/CDMA通信。支持8个固定分区。最大支持点数: 单色40960，双色20480。2个08接口, 4个12接口。";
		break;
	case 1:	//GS7030B
		m_nCardType = CARD_TYPE_GS7030B;
		memoNote->Text = "GPRS/CDMA通信。支持8个固定分区。最大支持点数: 单色128*1024，双色64*1024。2个08接口, 4个12接口。";
		break;
	case 2:	//GS2012A
		m_nCardType = CARD_TYPE_GS2012A;
		memoNote->Text = "GPRS/CDMA通信。支持8个固定分区。最大支持点数: 单色128*3072，单色256*1536，双色128*1536，双色256*768。50PIN标准接口，支持各种标准HUB板。";
		break;
	case 3:	//YL2010A
		m_nCardType = CARD_TYPE_YL2010A;
		memoNote->Text = "RS232或RS485通信。支持8个固定分区。最大支持点数: 单色40960，双色20480。2个08接口, 4个12接口。";
		break;
	case 4:	//YL2010AF
		m_nCardType = CARD_TYPE_YL2010AF;
		memoNote->Text = "RS232或RS485通信。支持8个动态分区。最大支持点数: 单色24576，双色12288。2个08接口, 4个12接口。";
		break;
	case 5:	//GS7020BF
		m_nCardType = CARD_TYPE_GS7020BF;
		memoNote->Text = "GPRS通信。支持8个动态分区。最大支持点数: 单色24576，双色12288。2个08接口, 4个12接口。";
		break;
	case 6:	//YL2010AR
		m_nCardType = CARD_TYPE_YL2010AR;
		memoNote->Text = "RS232或RS485通信。支持8个固定分区。最大支持点数: 单色40960，双色20480。2个08接口, 4个12接口。后半部分节目可通过串口命令点播。";
		break;
	case 7:	//PD102B
		m_nCardType = CARD_TYPE_PD102B;
		memoNote->Text = "RS232或RS485通信。最大支持点数: 单色192*32、384*16，双色192*16、96*32。1个08接口, 1个12接口。";
		break;
	case 8:	//GS7020BF_HRB
		m_nCardType = CARD_TYPE_GS7020BF_HEB;
		memoNote->Text = "GPRS通信。HEB客户定制。支持8个动态分区。最大支持点数: 单色24576，双色12288。2个08接口, 4个12接口。";
		break;
	case 9:	//GS7020BF_W
		m_nCardType = CARD_TYPE_GS7020BF_W;
		memoNote->Text = "GPRS通信。RS232/RS485接口。支持8个动态分区。支持接入报站器。255条节目, 最大256*32单双色。08/12/04接口";
		break;
	case 10:	//GS7020BX
		m_nCardType = CARD_TYPE_GS7020BX;
		memoNote->Text = "GPRS通信。无分区。支持循环冠名报时，多屏同步显示，每日24个显示时段。最大支持点数: 单色8192，双色4096。2个08接口, 4个12接口。";
		break;
	case 11:	//GS7020BT
		m_nCardType = CARD_TYPE_GS7020BT;
		memoNote->Text = "GPRS/CDMA通信。支持8个固定分区。最大支持点数: 单色40960，双色20480。2个08接口, 4个12接口。DB9插头１脚和５脚短接即开始测试显示屏。";
		break;
	case 12:	//GS7030BV2
		m_nCardType = CARD_TYPE_GS7030BV2;
		memoNote->Text = "GPRS通信。支持8个固定分区。最大支持点数: 单色128*1024，双色64*1024。2个08接口, 4个12接口。";
		break;
	case 13:	//GS7022B
		m_nCardType = CARD_TYPE_GS7022B;
		memoNote->Text = "GPRS/CDMA通信, 同时支持短信发送节目。支持8个固定分区。最大支持点数: 单色32768，双色16384。2个08接口, 4个12接口。";
		break;
	case 14:	//GS7020BQ
		m_nCardType = CARD_TYPE_GS7020BQ;
		memoNote->Text = "GPRS/CDMA通信。支持8个固定分区。最大支持点数: 单色32768，双色16384。2个08接口, 4个12接口。支持显示4个16*16的单色气象预警图标";
		break;
	case 15:	//GS7020F
		m_nCardType = CARD_TYPE_GS7020F;
		memoNote->Text = "GPRS通信, 四频。支持8个固定分区。最大支持点数: 单色40960，双色20480。2个08接口, 4个12接口。";
		break;
	case 16:	//GS7020BP
		m_nCardType = CARD_TYPE_GS7020BP;
		memoNote->Text = "GPRS/CDMA通信。交通诱导车位信息屏专用控制卡, 静态单/双色。最大支持8个3位数字显示。";
		break;
	case 17:	//GS2012A-S
		m_nCardType = CARD_TYPE_GS2012A_S;
		memoNote->Text = "GPRS通信, 同时支持短信发送节目。支持8个固定分区。最大支持点数: 单色128*3072，单色256*1536，双色128*1536，双色256*768。50PIN标准接口，支持各种标准HUB板。";
		break;
	case 18:	//GS9026
		m_nCardType = CARD_TYPE_GS9026;
		memoNote->Text = "GPRS通信。车载屏专用。支持显示单元板故障检测。支持8个动态分区。最大支持点数: 单/双色 32*256。1个08接口, 1个12接口。";
		break;
	case 19:	//GS7024E
		m_nCardType = CARD_TYPE_GS7024E;
		memoNote->Text = "GPRS/CDMA通信。8MB FLASH, 支持8个固定分区。最大支持点数: 单色40960，双色20480。2个08接口, 4个12接口。";
		break;
	case 20:	//WF2012B
		m_nCardType = CARD_TYPE_WF2012B;
		memoNote->Text = "WIFI无线通信。支持8个固定分区。最大支持点数: 单色128*2048，双色128*1024。4个08接口, 8个12接口。";
		break;
	case 21:	//GS2012AS_64
		m_nCardType = CARD_TYPE_GS2012AS_64;
		memoNote->Text = "64点高短信控制卡。最大支持点数: 单色64*3072(A)，单色64*2048(B)，双色64*1536(A)，双色64*1024(B)。";
		break;
	case 22:	//GS2012B
		m_nCardType = CARD_TYPE_GS2012B;
		memoNote->Text = "GPRS/CDMA通信。支持8个固定分区。最大支持点数: 单色128*2048，双色128*1024。4个08接口, 8个12接口。";
		break;
	case 23:	//GS9020
		m_nCardType = CARD_TYPE_GS9020;
		memoNote->Text = "GPRS通信。支持8个固定分区。最大支持点数: 单色32*256，16*512 双色32*128, 16*256。1个08接口, 2个12接口。";
		break;
	case 24:	//YL2010C
		m_nCardType = CARD_TYPE_YL2010C;
		memoNote->Text = "RS232或RS485通信。支持8个固定分区。最大支持点数: 全彩13632。2个08接口, 4个12接口。";
		break;
	case 25:	//YL2010A_GPS_JTX
		m_nCardType = CARD_TYPE_YL2010A_GPS_JTX;
		memoNote->Text = "RS232或RS485通信。JTX GPS专用，支持8个固定分区。最大支持点数: 单色40960，双色20480。2个08接口, 4个12接口。";
		break;
	case 26:	//GS9020S
		m_nCardType = CARD_TYPE_GS9020S;
		memoNote->Text = "小条屏短信控制卡。最大支持点数: 单色32*256。";
		break;
	case 27:	//GS7020H
		m_nCardType = CARD_TYPE_GS7020H;
		memoNote->Text = "GPRS/CDMA通信。支持8个固定分区。最大支持点数: 单色40960，双色20480。2个08接口, 4个12接口。";
		break;
	case 28:	//GS9025
		m_nCardType = CARD_TYPE_GS9025;
		memoNote->Text = "GPRS通信。公交车站用条屏控制卡。 控制范围: 32*256。1个08接口, 2个12接口。内置16*16、24*24宋体及黑体字库，可接收基于文本的普通节目和即时信息。";
		break;
	case 29:	//GS9023
		m_nCardType = CARD_TYPE_GS9023;
		memoNote->Text = "GPRS通信。16点高小条屏控制卡。控制范围: 16*256。1个08接口, 2个12接口。内置16*16宋体及黑体字库，可接收基于文本的节目信息。";
		break;
	case 30:	//GS7020HS
		m_nCardType = CARD_TYPE_GS7020HS;
		memoNote->Text = "GSM 短信LED显示屏控制卡。最大支持点数: 单色40960，双色20480。2个08接口, 4个12接口。";
		break;
	case 31:	//GS9020F_GPS
		m_nCardType = CARD_TYPE_GS9020F_GPS;
		memoNote->Text = "GPRS通信。支持8个固定分区。最大支持点数: 单色16*256,  双色16*128。1个08接口, 2个12接口。";
		break;
	case 32:	//GS7020HFA
		m_nCardType = CARD_TYPE_GS7020HFA;
		memoNote->Text = "GPRS/CDMA通信。支持8个动态分区。最大支持点数: 单色40960，双色20480。2个08接口, 4个12接口。";
		break;
	case 33:	//SN2P
		m_nCardType = CARD_TYPE_SN2P;
		memoNote->Text = "以太网/RS232/RS485通信。交通诱导车位信息屏专用控制卡, 静态单/双色。最大支持8个3位数字显示。";
		break;
	}
}
//---------------------------------------------------------------------------
