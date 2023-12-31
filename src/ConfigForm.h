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
#define PHONE_NUMBER_LEN					24 //电话号码最长20个数字+国际号前缀
#define SSID_LEN							32
#define PASSWORD_LEN						32

#pragma pack(1)
typedef struct
{
	DWORD ulVersion;						//版本0x100
	DWORD ulScanMode;						//低8位为扫描方式，D31-ABCD扫描线逆序, D30-扫描数据字节逆序
	WORD usSocketPort;						//中心SOCKET端口号
	BYTE bNegData;							//数据反相
	BYTE bNegOE;							//OE反相
	WORD usScreenWidth;						//屏宽
	WORD usScreenHeight;					//屏高
	char szDeviceCode[DEVICE_ID_LEN];		//设备编码
	char szDataCenter[NET_NAME_LEN];		//中心IP或域名
	char szDnsServer1[IP_ADDR_STR_LEN];		//DNS服务器1
	char szDnsServer2[IP_ADDR_STR_LEN];		//DNS服务器2
	BYTE ucLightControlStartHour[MAX_LIGHT_CONTROL_COUNT];		//亮度调节开始时间_小时
	BYTE ucLightControlStartMinute[MAX_LIGHT_CONTROL_COUNT];	//亮度调节开始时间_分钟
	BYTE ucLightControlValue[MAX_LIGHT_CONTROL_COUNT];			//亮度调节值(0-15, 0最亮)
	WORD usSubAreaLeft[MAX_AREA_COUNT];		//分区左上角水平位置
	WORD usSubAreaTop[MAX_AREA_COUNT];		//分区左上角垂直位置
	WORD usSubAreaWidth[MAX_AREA_COUNT];	//分区宽度
	WORD usSubAreaHeight[MAX_AREA_COUNT];	//分区高度
	BYTE ucSubAreaCount;					//分区数量
	BYTE ucScanLineCycleCount;
	BYTE bSupportWarnning;
	BYTE bDoubleColor;
	char szAPN[APN_LEN];					//APN
	BYTE bActSecond;
	BYTE ucMaxActCount;
	BYTE bActSupportRepeat;
	BYTE bSupportInsertPlay;
	char szGpsServer[NET_NAME_LEN];			//GPS服务器
	WORD usGpsSocketPort;					//GPS连接端口
	WORD usGpsReportInterval;				//GPS位置报告周期(秒)
	DWORD ulGpsId;							//GPS伪IP地址
	BYTE bActSupportSync;					//同步显示
	BYTE bActSupportCycleTime;				//循环报时
	BYTE bSupport_LedSign_Check;			//支持显示屏环路检测
	BYTE bTCP;
	DWORD ulUserBaudRate;
	DWORD ulModembaudRate;
	BYTE ucCascade;							//0:无级联 1:主卡 2:副卡
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
	DWORD ulVersion;						//版本0x100
	DWORD ulScanMode;						//低8位为扫描方式，D31-ABCD扫描线逆序, D30-扫描数据字节逆序
	WORD usSocketPort;						//中心SOCKET端口号
	BYTE bNegData;							//数据反相
	BYTE bNegOE;							//OE反相
	WORD usScreenWidth;						//屏宽
	WORD usScreenHeight;					//屏高
	char szDeviceCode[DEVICE_ID_LEN];		//设备编码
	char szDataCenter[NET_NAME_LEN];		//中心IP或域名
	BYTE ucLightControlStartHour[MAX_LIGHT_CONTROL_COUNT];		//亮度调节开始时间_小时
	BYTE ucLightControlStartMinute[MAX_LIGHT_CONTROL_COUNT];	//亮度调节开始时间_分钟
	BYTE ucLightControlValue[MAX_LIGHT_CONTROL_COUNT];			//亮度调节值(0-15, 0最亮)
	WORD usSubAreaLeft[MAX_AREA_COUNT];		//分区左上角水平位置
	WORD usSubAreaTop[MAX_AREA_COUNT];		//分区左上角垂直位置
	WORD usSubAreaWidth[MAX_AREA_COUNT];	//分区宽度
	WORD usSubAreaHeight[MAX_AREA_COUNT];	//分区高度
	BYTE ucSubAreaCount;					//分区数量
	BYTE ucScanLineCycleCount;
	BYTE bSupportWarnning;
	BYTE bDoubleColor;
	char szAPN[APN_LEN];					//APN
	BYTE bActSecond;
	BYTE ucMaxActCount;
	BYTE bActSupportRepeat;
	BYTE bSupportInsertPlay;
	BYTE bActSupportSync;					//同步显示
	BYTE bActSupportCycleTime;				//循环报时
	BYTE ucVoiceOutputAreaIndex;
	BYTE bTCP;
	char m_szSmsUsers[MAX_SMS_USER][PHONE_NUMBER_LEN];	//短信用户 len = 192
	DWORD ulShowTime;		//默认短信命令停留时间(ms)
	BYTE ucEnterStyle;		//默认短信进入方式(0-?)
	BYTE ucEnterSpeed;		//默认短信进入速度(ms)
	BYTE ucExitStyle;		//默认短信退出方式(0-?)
	BYTE ucExitSpeed;		//默认短信退出速度(ms)
	BYTE ucShutterWidth; 	//默认短信百叶窗叶片宽度
	BYTE ucStartYear; 		//默认短信起始播放时间_年
	BYTE ucStartMonth; 		//默认短信起始播放时间_月
	BYTE ucStartDay; 		//默认短信起始播放时间_日
	BYTE ucStartHour; 		//默认短信起始播放时间_时
	BYTE ucStartMinute; 	//默认短信起始播放时间_分
	BYTE ucEndYear; 		//默认短信终止播放时间_年
	BYTE ucEndMonth; 		//默认短信终止播放时间_月
	BYTE ucEndDay; 			//默认短信终止播放时间_日
	BYTE ucEndHour; 		//默认短信终止播放时间_时
	BYTE ucEndMinute; 		//默认短信终止播放时间_分
	BYTE ucAlignFlags;		//默认短信 D7:垂直居中排列，D6_D5: 00左对齐，01水平居中，10右对齐
	BYTE ucRepeatCount;		//默认短信重复播放次数
	BYTE ucReplaySms;       //是否回复短信	D7-D0分别表示8个用户
	BYTE ucRes4;
	BYTE ucFontSize;		//默认短信字体大小 16/32
	DWORD ulStartSecondInDay[4]; 	//默认短信每日播放时段开始时间
	DWORD ulEndSecondInDay[4]; 		//默认短信每日播放时段结束时间
} SETUP_PARA_GS7022B;

typedef struct
{
	WORD usScreenWidth;						//屏宽
	WORD usScreenHeight;					//屏高
	BYTE ucScanMode;						//扫描方式
	BYTE ucFlags;							//D0:数据反相,D1:OE反相,D2:ABCD扫描线逆序,D3扫描数据字节逆序
											//D4:双色
	BYTE ucFontSize;						//默认短信字体大小
	BYTE ucRes2;
	BYTE ucLightControlStartHour[4];		//亮度调节开始时间
	BYTE ucLightControlStartMinute[4];		//亮度调节开始时间
	BYTE ucLightControlValue[4];			//亮度调节值(0-7, 7最亮)
	char m_szSmsUsers[4][24];				//短信用户
	DWORD ulShowTime;						//默认短信命令停留时间(ms)
	BYTE ucEnterStyle;						//默认短信进入方式(0-?)
	BYTE ucEnterSpeed;						//默认短信进入速度(ms)
	BYTE ucExitStyle;						//默认短信退出方式(0-?)
	BYTE ucExitSpeed;						//默认短信退出速度(ms)
	WORD usStartYear; 						//默认短信起始播放时间_年
	BYTE ucStartMonth; 						//默认短信起始播放时间_月
	BYTE ucStartDay; 						//默认短信起始播放时间_日
	BYTE ucStartHour; 						//默认短信起始播放时间_时
	BYTE ucStartMinute; 					//默认短信起始播放时间_分
	WORD usEndYear; 						//默认短信终止播放时间_年
	BYTE ucEndMonth; 						//默认短信终止播放时间_月
	BYTE ucEndDay; 							//默认短信终止播放时间_日
	BYTE ucEndHour; 						//默认短信终止播放时间_时
	BYTE ucEndMinute; 						//默认短信终止播放时间_分
	BYTE ucShutterWidth; 					//默认短信百叶窗叶片宽度
	BYTE ucAlignFlags;						//默认短信 D7:垂直居中排列，D6_D5: 00左对齐，01水平居中，10右对齐
	BYTE ucRepeatCount;						//默认短信重复播放次数
	BYTE ucReplaySms;						//是否回复短信	D7-D0分别表示8个用户
	DWORD ulStartSecondInDay[4]; 			//默认短信每日播放时段开始时间
	DWORD ulEndSecondInDay[4]; 				//默认短信每日播放时段结束时间
} SETUP_PARA_GS9020S;

typedef struct
{
	WORD usScreenWidth;						//屏宽
	WORD usScreenHeight;					//屏高
	BYTE ucScanMode;						//扫描方式
	BYTE ucFlags;							//D0:数据反相,D1:OE反相,D2:ABCD扫描线逆序,D3扫描数据字节逆序
											//D4:双色
	BYTE ucFontSize;						//默认短信字体大小
	BYTE ucRes2;
	BYTE ucLightControlStartHour[8];		//亮度调节开始时间
	BYTE ucLightControlStartMinute[8];		//亮度调节开始时间
	BYTE ucLightControlValue[8];			//亮度调节值(0-7, 7最亮)
	char m_szSmsUsers[8][24];				//短信用户
	DWORD ulShowTime;						//默认短信命令停留时间(ms)
	BYTE ucEnterStyle;						//默认短信进入方式(0-?)
	BYTE ucEnterSpeed;						//默认短信进入速度(ms)
	BYTE ucExitStyle;						//默认短信退出方式(0-?)
	BYTE ucExitSpeed;						//默认短信退出速度(ms)
	WORD usStartYear; 						//默认短信起始播放时间_年
	BYTE ucStartMonth; 						//默认短信起始播放时间_月
	BYTE ucStartDay; 						//默认短信起始播放时间_日
	BYTE ucStartHour; 						//默认短信起始播放时间_时
	BYTE ucStartMinute; 					//默认短信起始播放时间_分
	WORD usEndYear; 						//默认短信终止播放时间_年
	BYTE ucEndMonth; 						//默认短信终止播放时间_月
	BYTE ucEndDay; 							//默认短信终止播放时间_日
	BYTE ucEndHour; 						//默认短信终止播放时间_时
	BYTE ucEndMinute; 						//默认短信终止播放时间_分
	BYTE ucShutterWidth; 					//默认短信百叶窗叶片宽度
	BYTE ucAlignFlags;						//默认短信 D7:垂直居中排列，D6_D5: 00左对齐，01水平居中，10右对齐
	BYTE ucRepeatCount;						//默认短信重复播放次数
	BYTE ucReplaySms;						//是否回复短信	D7-D0分别表示8个用户
	DWORD ulStartSecondInDay[4]; 			//默认短信每日播放时段开始时间
	DWORD ulEndSecondInDay[4]; 				//默认短信每日播放时段结束时间

	WORD m_wsWarnningText[16];			//报警文字
	char m_szWarnningPhoneNumber[24];		//报警电话
	char m_szGprsDeviceId[12];
	char m_szAPN[32];
	char m_szDataCenterMachineName[32];
	DWORD m_nOnlineReportInterval;
	WORD m_usSocketPort;
	WORD m_usRes;
} SETUP_PARA_GS7020HS;

typedef struct
{
	DWORD ulVersion;						//版本0x010100
	DWORD ulLen;
	DWORD ulScanMode;						//低8位为扫描方式，D31-ABCD扫描线逆序, D30-扫描数据字节逆序
	WORD usScreenWidth;						//屏宽
	WORD usScreenHeight;					//屏高
	BYTE ucLightControlStartHour[MAX_LIGHT_CONTROL_COUNT];		//亮度调节开始时间-秒
	BYTE ucLightControlStartMinute[MAX_LIGHT_CONTROL_COUNT];	//亮度调节开始时间-分
	BYTE ucLightControlValue[MAX_LIGHT_CONTROL_COUNT];			//亮度调节值(0-15, 0最亮)
	WORD usSubAreaLeft[MAX_AREA_COUNT];		//分区左上角水平位置
	WORD usSubAreaTop[MAX_AREA_COUNT];		//分区左上角垂直位置
	WORD usSubAreaWidth[MAX_AREA_COUNT];	//分区宽度
	WORD usSubAreaHeight[MAX_AREA_COUNT];	//分区高度
	BYTE bNegData;							//数据反相
	BYTE bNegOE;							//OE反相
	BYTE ucSubAreaCount;					//分区数量
	BYTE ucCardId;							//卡号
	BYTE bSupportWarnning;					//支持输入报警, 报警显示使用最后一条节目
	BYTE bDoubleColor;						//双色
	BYTE bSupportInsertPlay;
	BYTE ucMaxActCount;				    	//节目容量
	DWORD ulBaudrate;						//串口波特率
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
	BYTE ucScanMode;						//D0-D3扫描方式, D7:OE反相, D6:数据反相, D5:双色
	BYTE ucScreenByteWidth;					//屏宽 / 8
	BYTE ucScreenHeight;					//屏高
	BYTE ucLightControlValue;			    //亮度调节值(0-15, 0最亮)
	BYTE ucCardId;                          //0-255
	BYTE ucBaudrateIndex;                   //D0-D3 波特率索引
} SETUP_PARA_PD102B;

typedef struct
{
	DWORD ulVersion;						//版本0x100
	DWORD ulScanMode;						//低8位为扫描方式，D31-ABCD扫描线逆序, D30-扫描数据字节逆序
	WORD usSocketPort;						//中心SOCKET端口号
	BYTE bNegData;							//数据反相
	BYTE bNegOE;							//OE反相
	WORD usScreenWidth;						//屏宽
	WORD usScreenHeight;					//屏高
	char szDeviceCode[DEVICE_ID_LEN];		//设备编码
	char szDataCenter[NET_NAME_LEN];		//中心IP或域名
	char szPassword[PASSWORD_LEN];			//密码
	BYTE ucLightControlStartHour[MAX_LIGHT_CONTROL_COUNT];		//亮度调节开始时间_小时
	BYTE ucLightControlStartMinute[MAX_LIGHT_CONTROL_COUNT];	//亮度调节开始时间_分钟
	BYTE ucLightControlValue[MAX_LIGHT_CONTROL_COUNT];			//亮度调节值(0-15, 0最亮)
	WORD usSubAreaLeft[MAX_AREA_COUNT];		//分区左上角水平位置
	WORD usSubAreaTop[MAX_AREA_COUNT];		//分区左上角垂直位置
	WORD usSubAreaWidth[MAX_AREA_COUNT];	//分区宽度
	WORD usSubAreaHeight[MAX_AREA_COUNT];	//分区高度
	BYTE ucSubAreaCount;					//分区数量
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
	WORD usScreenWidth;						//屏宽
	WORD usScreenHeight;					//屏高
	BYTE ucScanMode;						//扫描方式
	BYTE ucFlags;							//D0:数据反相,D1:OE反相,D2:ABCD扫描线逆序,D3扫描数据字节逆序
											//D4:双色
	BYTE ucRes1;
	BYTE ucSubAreaCount;					//分区数量
	WORD usSubAreaLeft[MAX_AREA_COUNT];		//分区左上角水平位置
	WORD usSubAreaTop[MAX_AREA_COUNT];		//分区左上角垂直位置
	WORD usSubAreaWidth[MAX_AREA_COUNT];	//分区宽度
	WORD usSubAreaHeight[MAX_AREA_COUNT];	//分区高度
	BYTE ucLightControlStartHour[MAX_LIGHT_CONTROL_COUNT];		//亮度调节开始时间
	BYTE ucLightControlStartMinute[MAX_LIGHT_CONTROL_COUNT];	//亮度调节开始时间
	BYTE ucLightControlValue[MAX_LIGHT_CONTROL_COUNT];			//亮度调节值(0-7, 7最亮)
	char m_szSmsUsers[MAX_SMS_USER][PHONE_NUMBER_LEN];			//短信用户
	DWORD ulShowTime;						//默认短信命令停留时间(ms)
	BYTE ucEnterStyle;						//默认短信进入方式(0-?)
	BYTE ucEnterSpeed;						//默认短信进入速度(ms)
	BYTE ucExitStyle;						//默认短信退出方式(0-?)
	BYTE ucExitSpeed;						//默认短信退出速度(ms)
	WORD usStartYear; 						//默认短信起始播放时间_年
	BYTE ucStartMonth; 						//默认短信起始播放时间_月
	BYTE ucStartDay; 						//默认短信起始播放时间_日
	BYTE ucStartHour; 						//默认短信起始播放时间_时
	BYTE ucStartMinute; 					//默认短信起始播放时间_分
	WORD usEndYear; 						//默认短信终止播放时间_年
	BYTE ucEndMonth; 						//默认短信终止播放时间_月
	BYTE ucEndDay; 							//默认短信终止播放时间_日
	BYTE ucEndHour; 						//默认短信终止播放时间_时
	BYTE ucEndMinute; 						//默认短信终止播放时间_分
	BYTE ucShutterWidth; 					//默认短信百叶窗叶片宽度
	BYTE ucAlignFlags;						//默认短信 D7:垂直居中排列，D6_D5: 00左对齐，01水平居中，10右对齐
	BYTE ucRepeatCount;						//默认短信重复播放次数
	BYTE ucReplaySms;						//是否回复短信	D7-D0分别表示8个用户
	BYTE ucFontSize;						//默认短信字体大小 64
	BYTE ucRes3;
	BYTE ucRes4;
	BYTE ucRes5;
	DWORD ulStartSecondInDay[4]; 			//默认短信每日播放时段开始时间
	DWORD ulEndSecondInDay[4]; 				//默认短信每日播放时段结束时间
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
