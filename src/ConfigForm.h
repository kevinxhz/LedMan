//---------------------------------------------------------------------------

#ifndef ConfigFormH
#define ConfigFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include "SerialPort.h"
#include "SysConfig.h"
#include "dtu.h"

#define CARD_ID_GS7020B						88
#define VERSION_GS7020B						0xB0100
#define MAX_SMS_USER						8
#define PHONE_NUMBER_LEN					24 //�绰�����20������+���ʺ�ǰ׺
#define SSID_LEN							32
#define PASSWORD_LEN						32

#pragma pack(1)
typedef struct
{
	DWORD ulVersion;						//�汾0x100
	DWORD ulScanMode;						//��8λΪɨ�跽ʽ��D31-ABCDɨ��������, D30-ɨ�������ֽ�����
	WORD usSocketPort;						//����SOCKET�˿ں�
	BYTE bNegData;							//���ݷ���
	BYTE bNegOE;							//OE����
	WORD usScreenWidth;						//����
	WORD usScreenHeight;					//����
	char szDeviceCode[DEVICE_ID_LEN];		//�豸����
	char szDataCenter[NET_NAME_LEN];		//����IP������
	char szDnsServer1[IP_ADDR_STR_LEN];		//DNS������1
	char szDnsServer2[IP_ADDR_STR_LEN];		//DNS������2
	BYTE ucLightControlStartHour[MAX_LIGHT_CONTROL_COUNT];		//���ȵ��ڿ�ʼʱ��_Сʱ
	BYTE ucLightControlStartMinute[MAX_LIGHT_CONTROL_COUNT];	//���ȵ��ڿ�ʼʱ��_����
	BYTE ucLightControlValue[MAX_LIGHT_CONTROL_COUNT];			//���ȵ���ֵ(0-15, 0����)
	WORD usSubAreaLeft[MAX_AREA_COUNT];		//�������Ͻ�ˮƽλ��
	WORD usSubAreaTop[MAX_AREA_COUNT];		//�������ϽǴ�ֱλ��
	WORD usSubAreaWidth[MAX_AREA_COUNT];	//��������
	WORD usSubAreaHeight[MAX_AREA_COUNT];	//�����߶�
	BYTE ucSubAreaCount;					//��������
	BYTE ucScanLineCycleCount;
	BYTE bSupportWarnning;
	BYTE bDoubleColor;
	char szAPN[APN_LEN];					//APN
	BYTE bActSecond;
	BYTE ucMaxActCount;
	BYTE bActSupportRepeat;
	BYTE bSupportInsertPlay;
	char szGpsServer[NET_NAME_LEN];			//GPS������
	WORD usGpsSocketPort;					//GPS���Ӷ˿�
	WORD usGpsReportInterval;				//GPSλ�ñ�������(��)
	DWORD ulGpsId;							//GPSαIP��ַ
	BYTE bActSupportSync;					//ͬ����ʾ
	BYTE bActSupportCycleTime;				//ѭ����ʱ
	BYTE bSupport_LedSign_Check;			//֧����ʾ����·���
	BYTE bTCP;
	DWORD ulUserBaudRate;
	DWORD ulModembaudRate;
	BYTE ucCascade;							//0:�޼��� 1:���� 2:����
	BYTE bWaitFistLoginEcho;
	WORD usRes1;
	WORD usLeftPosInFull;
	WORD usTopPosInFull;
	WORD usWidthInFull;
	WORD usHeightInFull;
	DWORD ulRes[5];
	BYTE ucRes2;
	BYTE ucRes3;
	BYTE ucRes4;
	BYTE bFullColor;
} SETUP_PARA_GS7020B;

typedef struct
{
	DWORD ulVersion;						//�汾0x100
	DWORD ulScanMode;						//��8λΪɨ�跽ʽ��D31-ABCDɨ��������, D30-ɨ�������ֽ�����
	WORD usSocketPort;						//����SOCKET�˿ں�
	BYTE bNegData;							//���ݷ���
	BYTE bNegOE;							//OE����
	WORD usScreenWidth;						//����
	WORD usScreenHeight;					//����
	char szDeviceCode[DEVICE_ID_LEN];		//�豸����
	char szDataCenter[NET_NAME_LEN];		//����IP������
	BYTE ucLightControlStartHour[MAX_LIGHT_CONTROL_COUNT];		//���ȵ��ڿ�ʼʱ��_Сʱ
	BYTE ucLightControlStartMinute[MAX_LIGHT_CONTROL_COUNT];	//���ȵ��ڿ�ʼʱ��_����
	BYTE ucLightControlValue[MAX_LIGHT_CONTROL_COUNT];			//���ȵ���ֵ(0-15, 0����)
	WORD usSubAreaLeft[MAX_AREA_COUNT];		//�������Ͻ�ˮƽλ��
	WORD usSubAreaTop[MAX_AREA_COUNT];		//�������ϽǴ�ֱλ��
	WORD usSubAreaWidth[MAX_AREA_COUNT];	//��������
	WORD usSubAreaHeight[MAX_AREA_COUNT];	//�����߶�
	BYTE ucSubAreaCount;					//��������
	BYTE ucScanLineCycleCount;
	BYTE bSupportWarnning;
	BYTE bDoubleColor;
	char szAPN[APN_LEN];					//APN
	BYTE bActSecond;
	BYTE ucMaxActCount;
	BYTE bActSupportRepeat;
	BYTE bSupportInsertPlay;
	BYTE bActSupportSync;					//ͬ����ʾ
	BYTE bActSupportCycleTime;				//ѭ����ʱ
	BYTE ucVoiceOutputAreaIndex;
	BYTE bTCP;
	char m_szSmsUsers[MAX_SMS_USER][PHONE_NUMBER_LEN];	//�����û� len = 192
	DWORD ulShowTime;		//Ĭ�϶�������ͣ��ʱ��(ms)
	BYTE ucEnterStyle;		//Ĭ�϶��Ž��뷽ʽ(0-?)
	BYTE ucEnterSpeed;		//Ĭ�϶��Ž����ٶ�(ms)
	BYTE ucExitStyle;		//Ĭ�϶����˳���ʽ(0-?)
	BYTE ucExitSpeed;		//Ĭ�϶����˳��ٶ�(ms)
	BYTE ucShutterWidth; 	//Ĭ�϶��Ű�Ҷ��ҶƬ����
	BYTE ucStartYear; 		//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucStartMonth; 		//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucStartDay; 		//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucStartHour; 		//Ĭ�϶�����ʼ����ʱ��_ʱ
	BYTE ucStartMinute; 	//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucEndYear; 		//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucEndMonth; 		//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucEndDay; 			//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucEndHour; 		//Ĭ�϶�����ֹ����ʱ��_ʱ
	BYTE ucEndMinute; 		//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucAlignFlags;		//Ĭ�϶��� D7:��ֱ�������У�D6_D5: 00����룬01ˮƽ���У�10�Ҷ���
	BYTE ucRepeatCount;		//Ĭ�϶����ظ����Ŵ���
	BYTE ucReplaySms;       //�Ƿ�ظ�����	D7-D0�ֱ��ʾ8���û�
	BYTE ucRes4;
	BYTE ucFontSize;		//Ĭ�϶��������С 16/32
	DWORD ulStartSecondInDay[4]; 	//Ĭ�϶���ÿ�ղ���ʱ�ο�ʼʱ��
	DWORD ulEndSecondInDay[4]; 		//Ĭ�϶���ÿ�ղ���ʱ�ν���ʱ��
} SETUP_PARA_GS7022B;

typedef struct
{
	WORD usScreenWidth;						//����
	WORD usScreenHeight;					//����
	BYTE ucScanMode;						//ɨ�跽ʽ
	BYTE ucFlags;							//D0:���ݷ���,D1:OE����,D2:ABCDɨ��������,D3ɨ�������ֽ�����
											//D4:˫ɫ
	BYTE ucFontSize;						//Ĭ�϶��������С
	BYTE ucRes2;
	BYTE ucLightControlStartHour[4];		//���ȵ��ڿ�ʼʱ��
	BYTE ucLightControlStartMinute[4];		//���ȵ��ڿ�ʼʱ��
	BYTE ucLightControlValue[4];			//���ȵ���ֵ(0-7, 7����)
	char m_szSmsUsers[4][24];				//�����û�
	DWORD ulShowTime;						//Ĭ�϶�������ͣ��ʱ��(ms)
	BYTE ucEnterStyle;						//Ĭ�϶��Ž��뷽ʽ(0-?)
	BYTE ucEnterSpeed;						//Ĭ�϶��Ž����ٶ�(ms)
	BYTE ucExitStyle;						//Ĭ�϶����˳���ʽ(0-?)
	BYTE ucExitSpeed;						//Ĭ�϶����˳��ٶ�(ms)
	WORD usStartYear; 						//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucStartMonth; 						//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucStartDay; 						//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucStartHour; 						//Ĭ�϶�����ʼ����ʱ��_ʱ
	BYTE ucStartMinute; 					//Ĭ�϶�����ʼ����ʱ��_��
	WORD usEndYear; 						//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucEndMonth; 						//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucEndDay; 							//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucEndHour; 						//Ĭ�϶�����ֹ����ʱ��_ʱ
	BYTE ucEndMinute; 						//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucShutterWidth; 					//Ĭ�϶��Ű�Ҷ��ҶƬ����
	BYTE ucAlignFlags;						//Ĭ�϶��� D7:��ֱ�������У�D6_D5: 00����룬01ˮƽ���У�10�Ҷ���
	BYTE ucRepeatCount;						//Ĭ�϶����ظ����Ŵ���
	BYTE ucReplaySms;						//�Ƿ�ظ�����	D7-D0�ֱ��ʾ8���û�
	DWORD ulStartSecondInDay[4]; 			//Ĭ�϶���ÿ�ղ���ʱ�ο�ʼʱ��
	DWORD ulEndSecondInDay[4]; 				//Ĭ�϶���ÿ�ղ���ʱ�ν���ʱ��
} SETUP_PARA_GS9020S;

typedef struct
{
	WORD usScreenWidth;						//����
	WORD usScreenHeight;					//����
	BYTE ucScanMode;						//ɨ�跽ʽ
	BYTE ucFlags;							//D0:���ݷ���,D1:OE����,D2:ABCDɨ��������,D3ɨ�������ֽ�����
											//D4:˫ɫ
	BYTE ucFontSize;						//Ĭ�϶��������С
	BYTE ucRes2;
	BYTE ucLightControlStartHour[8];		//���ȵ��ڿ�ʼʱ��
	BYTE ucLightControlStartMinute[8];		//���ȵ��ڿ�ʼʱ��
	BYTE ucLightControlValue[8];			//���ȵ���ֵ(0-7, 7����)
	char m_szSmsUsers[8][24];				//�����û�
	DWORD ulShowTime;						//Ĭ�϶�������ͣ��ʱ��(ms)
	BYTE ucEnterStyle;						//Ĭ�϶��Ž��뷽ʽ(0-?)
	BYTE ucEnterSpeed;						//Ĭ�϶��Ž����ٶ�(ms)
	BYTE ucExitStyle;						//Ĭ�϶����˳���ʽ(0-?)
	BYTE ucExitSpeed;						//Ĭ�϶����˳��ٶ�(ms)
	WORD usStartYear; 						//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucStartMonth; 						//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucStartDay; 						//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucStartHour; 						//Ĭ�϶�����ʼ����ʱ��_ʱ
	BYTE ucStartMinute; 					//Ĭ�϶�����ʼ����ʱ��_��
	WORD usEndYear; 						//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucEndMonth; 						//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucEndDay; 							//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucEndHour; 						//Ĭ�϶�����ֹ����ʱ��_ʱ
	BYTE ucEndMinute; 						//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucShutterWidth; 					//Ĭ�϶��Ű�Ҷ��ҶƬ����
	BYTE ucAlignFlags;						//Ĭ�϶��� D7:��ֱ�������У�D6_D5: 00����룬01ˮƽ���У�10�Ҷ���
	BYTE ucRepeatCount;						//Ĭ�϶����ظ����Ŵ���
	BYTE ucReplaySms;						//�Ƿ�ظ�����	D7-D0�ֱ��ʾ8���û�
	DWORD ulStartSecondInDay[4]; 			//Ĭ�϶���ÿ�ղ���ʱ�ο�ʼʱ��
	DWORD ulEndSecondInDay[4]; 				//Ĭ�϶���ÿ�ղ���ʱ�ν���ʱ��

	WORD m_wsWarnningText[16];			//��������
	char m_szWarnningPhoneNumber[24];		//�����绰
	char m_szGprsDeviceId[12];
	char m_szAPN[32];
	char m_szDataCenterMachineName[32];
	DWORD m_nOnlineReportInterval;
	WORD m_usSocketPort;
	WORD m_usRes;
} SETUP_PARA_GS7020HS;

typedef struct
{
	DWORD ulVersion;						//�汾0x010100
	DWORD ulLen;
	DWORD ulScanMode;						//��8λΪɨ�跽ʽ��D31-ABCDɨ��������, D30-ɨ�������ֽ�����
	WORD usScreenWidth;						//����
	WORD usScreenHeight;					//����
	BYTE ucLightControlStartHour[MAX_LIGHT_CONTROL_COUNT];		//���ȵ��ڿ�ʼʱ��-��
	BYTE ucLightControlStartMinute[MAX_LIGHT_CONTROL_COUNT];	//���ȵ��ڿ�ʼʱ��-��
	BYTE ucLightControlValue[MAX_LIGHT_CONTROL_COUNT];			//���ȵ���ֵ(0-15, 0����)
	WORD usSubAreaLeft[MAX_AREA_COUNT];		//�������Ͻ�ˮƽλ��
	WORD usSubAreaTop[MAX_AREA_COUNT];		//�������ϽǴ�ֱλ��
	WORD usSubAreaWidth[MAX_AREA_COUNT];	//��������
	WORD usSubAreaHeight[MAX_AREA_COUNT];	//�����߶�
	BYTE bNegData;							//���ݷ���
	BYTE bNegOE;							//OE����
	BYTE ucSubAreaCount;					//��������
	BYTE ucCardId;							//����
	BYTE bSupportWarnning;					//֧�����뱨��, ������ʾʹ�����һ����Ŀ
	BYTE bDoubleColor;						//˫ɫ
	BYTE bSupportInsertPlay;
	BYTE ucMaxActCount;				    	//��Ŀ����
	DWORD ulBaudrate;						//���ڲ�����
	BYTE bActSupportSync;
	BYTE bActSupportRepeat;
	BYTE ucRes1;
	BYTE ucRes2;
	DWORD ulRes[30];
	BYTE ucRes3;
	BYTE ucRes4;
	BYTE ucRes5;
	BYTE bFullColor;
} SETUP_PARA_YL2010A;

typedef struct
{
	BYTE ucScanMode;						//D0-D3ɨ�跽ʽ, D7:OE����, D6:���ݷ���, D5:˫ɫ
	BYTE ucScreenByteWidth;					//���� / 8
	BYTE ucScreenHeight;					//����
	BYTE ucLightControlValue;			    //���ȵ���ֵ(0-15, 0����)
	BYTE ucCardId;                          //0-255
	BYTE ucBaudrateIndex;                   //D0-D3 ����������
} SETUP_PARA_PD102B;

typedef struct
{
	DWORD ulVersion;						//�汾0x100
	DWORD ulScanMode;						//��8λΪɨ�跽ʽ��D31-ABCDɨ��������, D30-ɨ�������ֽ�����
	WORD usSocketPort;						//����SOCKET�˿ں�
	BYTE bNegData;							//���ݷ���
	BYTE bNegOE;							//OE����
	WORD usScreenWidth;						//����
	WORD usScreenHeight;					//����
	char szDeviceCode[DEVICE_ID_LEN];		//�豸����
	char szDataCenter[NET_NAME_LEN];		//����IP������
	char szPassword[PASSWORD_LEN];			//����
	BYTE ucLightControlStartHour[MAX_LIGHT_CONTROL_COUNT];		//���ȵ��ڿ�ʼʱ��_Сʱ
	BYTE ucLightControlStartMinute[MAX_LIGHT_CONTROL_COUNT];	//���ȵ��ڿ�ʼʱ��_����
	BYTE ucLightControlValue[MAX_LIGHT_CONTROL_COUNT];			//���ȵ���ֵ(0-15, 0����)
	WORD usSubAreaLeft[MAX_AREA_COUNT];		//�������Ͻ�ˮƽλ��
	WORD usSubAreaTop[MAX_AREA_COUNT];		//�������ϽǴ�ֱλ��
	WORD usSubAreaWidth[MAX_AREA_COUNT];	//��������
	WORD usSubAreaHeight[MAX_AREA_COUNT];	//�����߶�
	BYTE ucSubAreaCount;					//��������
	BYTE ucScanLineCycleCount;
	BYTE bSupportWarnning;
	BYTE bDoubleColor;
	char szSSID[SSID_LEN];					//SSID
	BYTE bActSecond;
	BYTE ucMaxActCount;
	BYTE bActSupportRepeat;
	BYTE bSupportInsertPlay;
	DWORD ulRes[18];
	char szDnsServer[IP_ADDR_STR_LEN];		//DNS Server IP
} SETUP_PARA_WF2012B;

typedef struct
{
	WORD usScreenWidth;						//����
	WORD usScreenHeight;					//����
	BYTE ucScanMode;						//ɨ�跽ʽ
	BYTE ucFlags;							//D0:���ݷ���,D1:OE����,D2:ABCDɨ��������,D3ɨ�������ֽ�����
											//D4:˫ɫ
	BYTE ucRes1;
	BYTE ucSubAreaCount;					//��������
	WORD usSubAreaLeft[MAX_AREA_COUNT];		//�������Ͻ�ˮƽλ��
	WORD usSubAreaTop[MAX_AREA_COUNT];		//�������ϽǴ�ֱλ��
	WORD usSubAreaWidth[MAX_AREA_COUNT];	//��������
	WORD usSubAreaHeight[MAX_AREA_COUNT];	//�����߶�
	BYTE ucLightControlStartHour[MAX_LIGHT_CONTROL_COUNT];		//���ȵ��ڿ�ʼʱ��
	BYTE ucLightControlStartMinute[MAX_LIGHT_CONTROL_COUNT];	//���ȵ��ڿ�ʼʱ��
	BYTE ucLightControlValue[MAX_LIGHT_CONTROL_COUNT];			//���ȵ���ֵ(0-7, 7����)
	char m_szSmsUsers[MAX_SMS_USER][PHONE_NUMBER_LEN];			//�����û�
	DWORD ulShowTime;						//Ĭ�϶�������ͣ��ʱ��(ms)
	BYTE ucEnterStyle;						//Ĭ�϶��Ž��뷽ʽ(0-?)
	BYTE ucEnterSpeed;						//Ĭ�϶��Ž����ٶ�(ms)
	BYTE ucExitStyle;						//Ĭ�϶����˳���ʽ(0-?)
	BYTE ucExitSpeed;						//Ĭ�϶����˳��ٶ�(ms)
	WORD usStartYear; 						//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucStartMonth; 						//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucStartDay; 						//Ĭ�϶�����ʼ����ʱ��_��
	BYTE ucStartHour; 						//Ĭ�϶�����ʼ����ʱ��_ʱ
	BYTE ucStartMinute; 					//Ĭ�϶�����ʼ����ʱ��_��
	WORD usEndYear; 						//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucEndMonth; 						//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucEndDay; 							//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucEndHour; 						//Ĭ�϶�����ֹ����ʱ��_ʱ
	BYTE ucEndMinute; 						//Ĭ�϶�����ֹ����ʱ��_��
	BYTE ucShutterWidth; 					//Ĭ�϶��Ű�Ҷ��ҶƬ����
	BYTE ucAlignFlags;						//Ĭ�϶��� D7:��ֱ�������У�D6_D5: 00����룬01ˮƽ���У�10�Ҷ���
	BYTE ucRepeatCount;						//Ĭ�϶����ظ����Ŵ���
	BYTE ucReplaySms;						//�Ƿ�ظ�����	D7-D0�ֱ��ʾ8���û�
	BYTE ucFontSize;						//Ĭ�϶��������С 64
	BYTE ucRes3;
	BYTE ucRes4;
	BYTE ucRes5;
	DWORD ulStartSecondInDay[4]; 			//Ĭ�϶���ÿ�ղ���ʱ�ο�ʼʱ��
	DWORD ulEndSecondInDay[4]; 				//Ĭ�϶���ÿ�ղ���ʱ�ν���ʱ��
	DWORD ulRes[128];
} SETUP_PARA_GS2012AS_64;

typedef struct
{
	DWORD ulScanMode;
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
} SETUP_PARA_SN2P;


#pragma pack()

//---------------------------------------------------------------------------
class TfrmConfig : public TForm
{
__published:	// IDE-managed Components
	TButton *btnWriteConfig;
	TButton *btnReadConfig;
	TButton *btnOk;
	TButton *btnCancel;
	TTimer *timerWriteConfig;
	TTimer *timerReadConfig;
	TIdUDPClient *udpSocket;
	TPageControl *PageControl;
	TTabSheet *sheetLedPanel;
	TTabSheet *sheetSubArea;
	TLabel *lbPanelWidth;
	TLabel *lbPanelHeight;
	TLabel *lbScanMode;
	TLabel *lbActCount;
	TEdit *editScreenWidth;
	TEdit *editScreenHeight;
	TCheckBox *checkOE;
	TCheckBox *checkData;
	TComboBox *comboBoxScanMode;
	TCheckBox *checkActSecond;
	TComboBox *comboBoxMaxActCount;
	TLabel *lbAreaCount;
	TLabel *lbArea0;
	TLabel *Label8;
	TLabel *Label10;
	TLabel *lbAreaWidth;
	TLabel *lbAreaHeight;
	TLabel *lbArea1;
	TLabel *lbArea2;
	TLabel *lbArea3;
	TLabel *lbArea4;
	TLabel *lbArea7;
	TLabel *lbArea5;
	TLabel *lbArea6;
	TComboBox *comboBoxAreaCount;
	TEdit *editAreaLeft0;
	TEdit *editAreaTop0;
	TEdit *editAreaWidth0;
	TEdit *editAreaHeight0;
	TEdit *editAreaLeft1;
	TEdit *editAreaTop1;
	TEdit *editAreaWidth1;
	TEdit *editAreaHeight1;
	TEdit *editAreaLeft2;
	TEdit *editAreaTop2;
	TEdit *editAreaWidth2;
	TEdit *editAreaHeight2;
	TEdit *editAreaLeft3;
	TEdit *editAreaTop3;
	TEdit *editAreaWidth3;
	TEdit *editAreaHeight3;
	TEdit *editAreaLeft4;
	TEdit *editAreaTop4;
	TEdit *editAreaWidth4;
	TEdit *editAreaHeight4;
	TEdit *editAreaLeft7;
	TEdit *editAreaTop7;
	TEdit *editAreaWidth7;
	TEdit *editAreaHeight7;
	TEdit *editAreaLeft5;
	TEdit *editAreaTop5;
	TEdit *editAreaWidth5;
	TEdit *editAreaHeight5;
	TEdit *editAreaLeft6;
	TEdit *editAreaTop6;
	TEdit *editAreaWidth6;
	TEdit *editAreaHeight6;
	TScrollBox *scrAreas;
	TImage *imageAreas;
	TTabSheet *sheetAutoLightControl;
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
	TTabSheet *sheetGPRS;
	TLabel *lbServer;
	TLabel *lbSocket;
	TLabel *lbCode;
	TLabel *labelAPN;
	TEdit *editDeviceCode;
	TEdit *editDataCenter;
	TEdit *editSocketPort_Card;
	TEdit *editAPN;
	TCheckBox *checkActSupportRepeat;
	TCheckBox *checkSupportInsertPlay;
	TRadioButton *rbtnSingleColor;
	TRadioButton *rbtnDoubleColor;
	TCheckBox *checkSupportWarnning;
	TCheckBox *checkSyncPlay;
	TTabSheet *sheetComPort;
	TLabel *lbCurrentCardId;
	TEdit *editCardId;
	TComboBox *cbBaudrate;
	TLabel *lbBaudRate;
	TButton *btnSearchCard;
	TLabel *lbNewCardId;
	TEdit *editNewCardId;
	TTimer *timerSearchCard;
	TCheckBox *checkByteReverse;
	TTabSheet *sheetSms;
	TLabel *Label3;
	TEdit *editSmsUser0;
	TEdit *editSmsUser1;
	TEdit *editSmsUser2;
	TEdit *editSmsUser3;
	TEdit *editSmsUser4;
	TEdit *editSmsUser5;
	TEdit *editSmsUser6;
	TEdit *editSmsUser7;
	TCheckBox *checkEchoSms0;
	TCheckBox *checkEchoSms1;
	TCheckBox *checkEchoSms2;
	TCheckBox *checkEchoSms3;
	TCheckBox *checkEchoSms4;
	TCheckBox *checkEchoSms5;
	TCheckBox *checkEchoSms6;
	TCheckBox *checkEchoSms7;
	TCheckBox *checkSyncTimeHint;
	TGroupBox *GroupBox1;
	TLabel *lbEnterStyle;
	TComboBox *comboEnterStyle;
	TLabel *lbExitStyle;
	TComboBox *comboExitStyle;
	TLabel *lbEnterSpeed;
	TEdit *editEnterSpeed;
	TLabel *lbStayTime;
	TEdit *editShowTime;
	TLabel *lbSpeed1;
	TLabel *lbStayTimeHint;
	TLabel *lbRepeatCount;
	TEdit *editRepeatCount;
	TLabel *lbExitSpeed;
	TEdit *editExitSpeed;
	TLabel *lbSpeed2;
	TLabel *Label4;
	TComboBox *comboFont;
	TLabel *lbVoiceArea;
	TEdit *editVoiceArea;
	TCheckBox *checkSupportLedSignCheck;
	TTabSheet *sheetWifi;
	TLabel *Label5;
	TEdit *editWifiDeviceCode;
	TLabel *Label6;
	TEdit *editWifiServer;
	TLabel *Label7;
	TEdit *editWifiSocketPort_Card;
	TEdit *editSSID;
	TLabel *Label9;
	TLabel *Label11;
	TEdit *editWifiPassword;
	TLabel *Label12;
	TEdit *editWifiDnsServer;
	TCheckBox *checkTCP;
	TRadioButton *rbtnFullColor;
	TCheckBox *checkLoginEcho;
	TTabSheet *sheetGps;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label13;
	TLabel *Label14;
	TEdit *editGpsServer;
	TEdit *editGpsSocketPort;
	TEdit *editGpsPseudoIp;
	TEdit *editGpsReportInterval;
	TTabSheet *sheetWebData;
	TLabel *Label15;
	TEdit *editWebDataId;
	TTabSheet *sheetEthernet;
	TLabel *Label16;
	TEdit *editLocalIp;
	TLabel *Label17;
	TEdit *editNetMask;
	TEdit *editGateway;
	TLabel *Label18;
	TLabel *Label19;
	TEdit *editNewLocalIp;
	TCheckBox *checkBroadcast;
	void __fastcall comboBoxAreaCountChange(TObject *Sender);
	void __fastcall btnWriteConfigClick(TObject *Sender);
	void __fastcall timerWriteConfigTimer(TObject *Sender);
	void __fastcall btnReadConfigClick(TObject *Sender);
	void __fastcall timerReadConfigTimer(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall editSubAreaChange(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnSearchCardClick(TObject *Sender);
	void __fastcall timerSearchCardTimer(TObject *Sender);
private:	// User declarations
	CSerialPort m_ComPort;
	int m_nTimeout;
	int m_nRxStep;
	int m_nRetries;
	CDtu* m_dtu;
	int m_nCardId;
	int m_nSearchCount;
	void UpdateConfigData(void* p);
	void UpdateConfigData_YL2010A(void* p);
	void UpdateConfigData_GS7020B(void* p);
	void UpdateConfigData_GS7022B(void* p);
	void UpdateConfigData_GS9020S(void* p);
	void UpdateConfigData_GS7020HS(void* p);
	void UpdateConfigData_PD102B(void* p);
	void UpdateConfigData_WF2012B(void* p);
	void UpdateConfigData_GS2012AS_64(void* p);
	void UpdateConfigData_SN2P(void* p);
	BYTE CalcChecksum(int nLen, BYTE* p);
	void DrawAreaImage();
	BYTE m_ucKey1, m_ucKey2;
	void Encrypt(int nLen, BYTE* p);
public:		// User declarations
	__fastcall TfrmConfig(TComponent* Owner, CDtu* dtu);
	void SendData_UDP(char* szDtuCode, int nLen, BYTE* p);
	int ReciveUdpData(BYTE* pRxBuf, int nWantLen);
	void SendCmd_ReadConfig();
	void SendCmd_WriteConfig_YL2010A();
	void SendCmd_WriteConfig_GS7020B();
	void SendCmd_WriteConfig_GS7022B();
	void SendCmd_WriteConfig_GS9020S();
	void SendCmd_WriteConfig_GS7020HS();
	void SendCmd_WriteConfig_PD102B();
	void SendCmd_WriteConfig_WF2012B();
	void SendCmd_WriteConfig_GS2012AS_64();
	void SendCmd_WriteConfig_GS9025();
	void SendCmd_WriteConfig_SN2P();
	void SendCmd_WriteConfig();
	void SendCommand_StopDisplay();
	bool OpenComm();
	void StartComm();
	void EndComm();
	void SendCommand_Echo();
	void WriteComPort(int nLen, BYTE* p);

	int m_nSelectActCountTable[8];
	int m_nSelectActCountTableSize;
};
//---------------------------------------------------------------------------

#endif