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
		Caption = "���ƿ��ͺ�ѡ��";
		Label1->Caption = "����ȷѡ�����������Ŀ��ƿ��ͺ�:";
		btnOk->Caption = "ȷ��";
		btnCancel->Caption = "ȡ��";
		lstCardType->Items->Strings[0] = "GS" + UnicodeString(7020) + "B/GS" + UnicodeString(7020) + "E(GPRS) CS" + UnicodeString(7020) + "B(CDMA)  LED ��ʾ�����ƿ�";
		lstCardType->Items->Strings[1] = "GS" + UnicodeString(7030) + "B(GPRS) CS" + UnicodeString(7030) + "B(CDMA) LED ��ʾ�����ƿ�";
		lstCardType->Items->Strings[2] = "GS" + UnicodeString(2012) + "A(GPRS) CS" + UnicodeString(2012) + "A(CDMA) LED ��ʾ�����ƿ�";
		lstCardType->Items->Strings[3] = "YL" + UnicodeString(2010) + "A RS232/RS485 LED ��ʾ�����ƿ�";
		lstCardType->Items->Strings[4] = "YL2010AF RS232/RS485 LED ��ʾ�����ƿ�(��̬����)";
		lstCardType->Items->Strings[5] = "GS7020BF/GS7020EF GPRS LED ��ʾ�����ƿ�(��̬����)";
		lstCardType->Items->Strings[6] = "YL2010AR RS232/RS485 LED ��ʾ������(֧�ִ�������㲥)";
		lstCardType->Items->Strings[7] = "PD102B RS232/RS485 ͼ���������ƿ�";
		lstCardType->Items->Strings[8] = "GS7020EF-H ������ר�� GPRS LED ��ʾ�����ƿ�(��̬����)";
		lstCardType->Items->Strings[9] = "GS7020B(E)F-W ������ר�� GPRS LED ��ʾ�����ƿ�(��̬����, ֧�ֱ�վ)";
		lstCardType->Items->Strings[10] = "GS7020BX/GS7020EX ��ʱ��ͬ����ʾ������ʱ������ר�ÿ��ƿ�";
		lstCardType->Items->Strings[11] = "GS7020BT/GS7020ET(GPRS) CS7020BT(CDMA)  LED ��ʾ�����ƿ�(��ť������ʾ����)";
		lstCardType->Items->Strings[12] = "GS7030BV2 GPRS LED ��ʾ�����ƿ�";
		lstCardType->Items->Strings[13] = "GS7022B/E/H(GPRS) CS7022B/H(CDMA)  LED ��ʾ�����ƿ�(ͬʱ֧�ֶ���)";
		lstCardType->Items->Strings[14] = "GS7020HQ/GS7020BQ/GS7020EQ(GPRS) CS7020HQ/CS7020BQ(CDMA) ����Ԥ�������ƿ�(֧��4��16x16��Ԥ��ͼ����ʾ��)";
		lstCardType->Items->Strings[15] = "GS7020F GPRS LED Sign Control Card (International Version)";
		lstCardType->Items->Strings[16] = "GS7020BP/GS7020EP(GPRS) CS7020BP(CDMA)  ��λ��Ϣ��ר�ÿ��ƿ�";
		lstCardType->Items->Strings[17] = "GS2012A-S GPRS+���� LED ��ʾ�����ƿ�";
		lstCardType->Items->Strings[18] = "GS9026 GPRS ����LED ��ʾ�����ƿ�(����GS7020EF-H)";
		lstCardType->Items->Strings[19] = "GS7024B/GS7024E(GPRS) CS7024B(CDMA) LED ��ʾ�����ƿ�(8MB FLASH)";
		lstCardType->Items->Strings[20] = "WF2012B WIFI/RS232 LED ��ʾ�����ƿ�";
		lstCardType->Items->Strings[21] = "GS2012AS-64 GS2012BS-64 64��߶��� LED ��ʾ�����ƿ�";
		lstCardType->Items->Strings[22] = "GS" + UnicodeString(2012) + "B(GPRS) CS" + UnicodeString(2012) + "B(CDMA) LED ��ʾ�����ƿ�";
		lstCardType->Items->Strings[23] = "GS" + UnicodeString(9020) + " GPRS С�������ƿ�(����GS7020B GS7020E)";
		lstCardType->Items->Strings[24] = "YL2010C RS232/RS485 ȫ��(�޻Ҷ�) LED ��ʾ�����ƿ�";
		lstCardType->Items->Strings[25] = "YL" + UnicodeString(2010) + "A RS232/RS485 LED ��ʾ�����ƿ���JTX GPS������ʻ��¼��ר��";
		lstCardType->Items->Strings[26] = "GS" + UnicodeString(9020) + "S С�������ſ��ƿ�";
		lstCardType->Items->Strings[27] = "GS" + UnicodeString(7020) + "H(GPRS) CS" + UnicodeString(7020) + "H(CDMA)  LED ��ʾ�����ƿ�";
		lstCardType->Items->Strings[28] = "GS" + UnicodeString(9025) + " GPRS ������վ��32����������ƿ�(�����ֿ�)";
		lstCardType->Items->Strings[29] = "GS" + UnicodeString(9023) + " GPRS 16���С�������ƿ�(����16������ͺ����ֿ�)";
		lstCardType->Items->Strings[30] = "GS7020HS GS7020IS GSM ���ſ��ƿ�";
		lstCardType->Items->Strings[31] = "GS9020F-GPS ����GPRS LED��ʾ�����ƿ���֧������GPS��λģ��";
		lstCardType->Items->Strings[32] = "GS7020HFA(GPRS)/CS7020HFA(CDMA) LED ��ʾ�����ƿ�(��̬����)";
		lstCardType->Items->Strings[33] = "SN2P ��̫��/���� ��λ��Ϣ��ʾ�����ƿ�";
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
		lstCardType->Items->Strings[25] = "YL2010A RS232/RS485��JTX GPS";
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
		memoNote->Text = "GPRS/CDMAͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ40960��˫ɫ20480��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 1:	//GS7030B
		m_nCardType = CARD_TYPE_GS7030B;
		memoNote->Text = "GPRS/CDMAͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ128*1024��˫ɫ64*1024��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 2:	//GS2012A
		m_nCardType = CARD_TYPE_GS2012A;
		memoNote->Text = "GPRS/CDMAͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ128*3072����ɫ256*1536��˫ɫ128*1536��˫ɫ256*768��50PIN��׼�ӿڣ�֧�ָ��ֱ�׼HUB�塣";
		break;
	case 3:	//YL2010A
		m_nCardType = CARD_TYPE_YL2010A;
		memoNote->Text = "RS232��RS485ͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ40960��˫ɫ20480��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 4:	//YL2010AF
		m_nCardType = CARD_TYPE_YL2010AF;
		memoNote->Text = "RS232��RS485ͨ�š�֧��8����̬���������֧�ֵ���: ��ɫ24576��˫ɫ12288��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 5:	//GS7020BF
		m_nCardType = CARD_TYPE_GS7020BF;
		memoNote->Text = "GPRSͨ�š�֧��8����̬���������֧�ֵ���: ��ɫ24576��˫ɫ12288��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 6:	//YL2010AR
		m_nCardType = CARD_TYPE_YL2010AR;
		memoNote->Text = "RS232��RS485ͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ40960��˫ɫ20480��2��08�ӿ�, 4��12�ӿڡ���벿�ֽ�Ŀ��ͨ����������㲥��";
		break;
	case 7:	//PD102B
		m_nCardType = CARD_TYPE_PD102B;
		memoNote->Text = "RS232��RS485ͨ�š����֧�ֵ���: ��ɫ192*32��384*16��˫ɫ192*16��96*32��1��08�ӿ�, 1��12�ӿڡ�";
		break;
	case 8:	//GS7020BF_HRB
		m_nCardType = CARD_TYPE_GS7020BF_HEB;
		memoNote->Text = "GPRSͨ�š�HEB�ͻ����ơ�֧��8����̬���������֧�ֵ���: ��ɫ24576��˫ɫ12288��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 9:	//GS7020BF_W
		m_nCardType = CARD_TYPE_GS7020BF_W;
		memoNote->Text = "GPRSͨ�š�RS232/RS485�ӿڡ�֧��8����̬������֧�ֽ��뱨վ����255����Ŀ, ���256*32��˫ɫ��08/12/04�ӿ�";
		break;
	case 10:	//GS7020BX
		m_nCardType = CARD_TYPE_GS7020BX;
		memoNote->Text = "GPRSͨ�š��޷�����֧��ѭ��������ʱ������ͬ����ʾ��ÿ��24����ʾʱ�Ρ����֧�ֵ���: ��ɫ8192��˫ɫ4096��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 11:	//GS7020BT
		m_nCardType = CARD_TYPE_GS7020BT;
		memoNote->Text = "GPRS/CDMAͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ40960��˫ɫ20480��2��08�ӿ�, 4��12�ӿڡ�DB9��ͷ���źͣ��Ŷ̽Ӽ���ʼ������ʾ����";
		break;
	case 12:	//GS7030BV2
		m_nCardType = CARD_TYPE_GS7030BV2;
		memoNote->Text = "GPRSͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ128*1024��˫ɫ64*1024��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 13:	//GS7022B
		m_nCardType = CARD_TYPE_GS7022B;
		memoNote->Text = "GPRS/CDMAͨ��, ͬʱ֧�ֶ��ŷ��ͽ�Ŀ��֧��8���̶����������֧�ֵ���: ��ɫ32768��˫ɫ16384��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 14:	//GS7020BQ
		m_nCardType = CARD_TYPE_GS7020BQ;
		memoNote->Text = "GPRS/CDMAͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ32768��˫ɫ16384��2��08�ӿ�, 4��12�ӿڡ�֧����ʾ4��16*16�ĵ�ɫ����Ԥ��ͼ��";
		break;
	case 15:	//GS7020F
		m_nCardType = CARD_TYPE_GS7020F;
		memoNote->Text = "GPRSͨ��, ��Ƶ��֧��8���̶����������֧�ֵ���: ��ɫ40960��˫ɫ20480��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 16:	//GS7020BP
		m_nCardType = CARD_TYPE_GS7020BP;
		memoNote->Text = "GPRS/CDMAͨ�š���ͨ�յ���λ��Ϣ��ר�ÿ��ƿ�, ��̬��/˫ɫ�����֧��8��3λ������ʾ��";
		break;
	case 17:	//GS2012A-S
		m_nCardType = CARD_TYPE_GS2012A_S;
		memoNote->Text = "GPRSͨ��, ͬʱ֧�ֶ��ŷ��ͽ�Ŀ��֧��8���̶����������֧�ֵ���: ��ɫ128*3072����ɫ256*1536��˫ɫ128*1536��˫ɫ256*768��50PIN��׼�ӿڣ�֧�ָ��ֱ�׼HUB�塣";
		break;
	case 18:	//GS9026
		m_nCardType = CARD_TYPE_GS9026;
		memoNote->Text = "GPRSͨ�š�������ר�á�֧����ʾ��Ԫ����ϼ�⡣֧��8����̬���������֧�ֵ���: ��/˫ɫ 32*256��1��08�ӿ�, 1��12�ӿڡ�";
		break;
	case 19:	//GS7024E
		m_nCardType = CARD_TYPE_GS7024E;
		memoNote->Text = "GPRS/CDMAͨ�š�8MB FLASH, ֧��8���̶����������֧�ֵ���: ��ɫ40960��˫ɫ20480��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 20:	//WF2012B
		m_nCardType = CARD_TYPE_WF2012B;
		memoNote->Text = "WIFI����ͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ128*2048��˫ɫ128*1024��4��08�ӿ�, 8��12�ӿڡ�";
		break;
	case 21:	//GS2012AS_64
		m_nCardType = CARD_TYPE_GS2012AS_64;
		memoNote->Text = "64��߶��ſ��ƿ������֧�ֵ���: ��ɫ64*3072(A)����ɫ64*2048(B)��˫ɫ64*1536(A)��˫ɫ64*1024(B)��";
		break;
	case 22:	//GS2012B
		m_nCardType = CARD_TYPE_GS2012B;
		memoNote->Text = "GPRS/CDMAͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ128*2048��˫ɫ128*1024��4��08�ӿ�, 8��12�ӿڡ�";
		break;
	case 23:	//GS9020
		m_nCardType = CARD_TYPE_GS9020;
		memoNote->Text = "GPRSͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ32*256��16*512 ˫ɫ32*128, 16*256��1��08�ӿ�, 2��12�ӿڡ�";
		break;
	case 24:	//YL2010C
		m_nCardType = CARD_TYPE_YL2010C;
		memoNote->Text = "RS232��RS485ͨ�š�֧��8���̶����������֧�ֵ���: ȫ��13632��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 25:	//YL2010A_GPS_JTX
		m_nCardType = CARD_TYPE_YL2010A_GPS_JTX;
		memoNote->Text = "RS232��RS485ͨ�š�JTX GPSר�ã�֧��8���̶����������֧�ֵ���: ��ɫ40960��˫ɫ20480��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 26:	//GS9020S
		m_nCardType = CARD_TYPE_GS9020S;
		memoNote->Text = "С�������ſ��ƿ������֧�ֵ���: ��ɫ32*256��";
		break;
	case 27:	//GS7020H
		m_nCardType = CARD_TYPE_GS7020H;
		memoNote->Text = "GPRS/CDMAͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ40960��˫ɫ20480��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 28:	//GS9025
		m_nCardType = CARD_TYPE_GS9025;
		memoNote->Text = "GPRSͨ�š�������վ���������ƿ��� ���Ʒ�Χ: 32*256��1��08�ӿ�, 2��12�ӿڡ�����16*16��24*24���弰�����ֿ⣬�ɽ��ջ����ı�����ͨ��Ŀ�ͼ�ʱ��Ϣ��";
		break;
	case 29:	//GS9023
		m_nCardType = CARD_TYPE_GS9023;
		memoNote->Text = "GPRSͨ�š�16���С�������ƿ������Ʒ�Χ: 16*256��1��08�ӿ�, 2��12�ӿڡ�����16*16���弰�����ֿ⣬�ɽ��ջ����ı��Ľ�Ŀ��Ϣ��";
		break;
	case 30:	//GS7020HS
		m_nCardType = CARD_TYPE_GS7020HS;
		memoNote->Text = "GSM ����LED��ʾ�����ƿ������֧�ֵ���: ��ɫ40960��˫ɫ20480��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 31:	//GS9020F_GPS
		m_nCardType = CARD_TYPE_GS9020F_GPS;
		memoNote->Text = "GPRSͨ�š�֧��8���̶����������֧�ֵ���: ��ɫ16*256,  ˫ɫ16*128��1��08�ӿ�, 2��12�ӿڡ�";
		break;
	case 32:	//GS7020HFA
		m_nCardType = CARD_TYPE_GS7020HFA;
		memoNote->Text = "GPRS/CDMAͨ�š�֧��8����̬���������֧�ֵ���: ��ɫ40960��˫ɫ20480��2��08�ӿ�, 4��12�ӿڡ�";
		break;
	case 33:	//SN2P
		m_nCardType = CARD_TYPE_SN2P;
		memoNote->Text = "��̫��/RS232/RS485ͨ�š���ͨ�յ���λ��Ϣ��ר�ÿ��ƿ�, ��̬��/˫ɫ�����֧��8��3λ������ʾ��";
		break;
	}
}
//---------------------------------------------------------------------------