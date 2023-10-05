//---------------------------------------------------------------------------

#ifndef SysConfigH
#define SysConfigH
//---------------------------------------------------------------------------

#include "global.h"
#include "act.h"
#include "dtu.h"
#include <inifiles.hpp>
using namespace std;
#include <list>

#define TEXT_ALIGN_LEFT		0
#define TEXT_ALIGN_CENTER	1
#define TEXT_ALIGN_RIGHT	2

#define ENTER_STYLE_DIRECT					0
#define ENTER_STYLE_SCROLL_LEFT				1
#define ENTER_STYLE_SCROLL_RIGHT			2
#define ENTER_STYLE_SCROLL_UP				3
#define ENTER_STYLE_SCROLL_DOWN				4
#define ENTER_STYLE_MOVE_LEFT				5
#define ENTER_STYLE_MOVE_RIGHT				6
#define ENTER_STYLE_MOVE_UP					7
#define ENTER_STYLE_MOVE_DOWN				8
#define ENTER_STYLE_LEFT_TO_RIGHT_SPREAD	9
#define ENTER_STYLE_RIGHT_TO_LEFT_SPREAD	10
#define ENTER_STYLE_TOP_TO_BOTTOM_SPREAD	11
#define ENTER_STYLE_BOTTOM_TO_TOP_SPREAD	12
#define ENTER_STYLE_H_TO_CENTER_SPREAD		13
#define ENTER_STYLE_H_FROM_CENTER_SPREAD	14
#define ENTER_STYLE_V_TO_CENTER_SPREAD		15
#define ENTER_STYLE_V_FROM_CENTER_SPREAD	16
#define ENTER_STYLE_H_SHUTTER				17
#define ENTER_STYLE_V_SHUTTER				18
#define ENTER_STYLE_FLASH					19

#define EXIT_STYLE_DIRECT					0
#define EXIT_STYLE_MOVE_LEFT				1
#define EXIT_STYLE_MOVE_RIGHT				2
#define EXIT_STYLE_MOVE_UP					3
#define EXIT_STYLE_MOVE_DOWN				4
#define EXIT_STYLE_LEFT_TO_RIGHT_ERASE		5
#define EXIT_STYLE_RIGHT_TO_LEFT_ERASE		6
#define EXIT_STYLE_TOP_TO_BOTTOM_ERASE		7
#define EXIT_STYLE_BOTTOM_TO_TOP_ERASE		8
#define EXIT_STYLE_H_TO_CENTER_ERASE		9
#define EXIT_STYLE_H_FROM_CENTER_ERASE		10
#define EXIT_STYLE_V_TO_CENTER_ERASE		11
#define EXIT_STYLE_V_FROM_CENTER_ERASE		12
#define EXIT_STYLE_H_SHUTTER				13
#define EXIT_STYLE_V_SHUTTER				14
#define EXIT_STYLE_FLASH					15
#define EXIT_STYLE_NULL						16

class CSysConfig
{
public:
	CSysConfig();
	~CSysConfig();

	UnicodeString m_sRootName;
	int m_nCardType;
	int m_nMaxPackSize;

	int m_nLanguage;
	UnicodeString m_sSystemPassword;
	int m_nAutoClockInterval;
	int m_nAutoClockMinute;
	int m_nAutoReTxActInterval;
	bool m_bAutoReTx_OnlyLast;
	int m_nCommMethod;
	UnicodeString m_sComPort;
	int m_nBaudRate;
	int m_nTimeout_SerialPort;
	int m_nRetry_SerialPort;
	UnicodeString m_sServer;
	int m_nSocketPort;
	int m_nRetry_Network;
	int m_nTimeout_Network;
	UnicodeString m_sUserName;
	UnicodeString m_sPassword;

	int m_nSocketPort_Card;
	int m_nScreenWidth;
	int m_nScreenHeight;
	int m_ulScanMode;
	bool m_bNegData;
	bool m_bNegOE;
	bool m_bByteReverse;
	bool m_bDoubleColor;
	bool m_bFullColor;
	bool m_bSupportLedSignCheck;
	int m_nSubAreaCount;
	int m_nSubAreaLeft[MAX_AREA_COUNT];
	int m_nSubAreaTop[MAX_AREA_COUNT];
	int m_nSubAreaWidth[MAX_AREA_COUNT];
	int m_nSubAreaHeight[MAX_AREA_COUNT];
	int m_nLightControlStartHour[MAX_LIGHT_CONTROL_COUNT];
	int m_nLightControlStartMinute[MAX_LIGHT_CONTROL_COUNT];
	int m_nLightControlValue[MAX_LIGHT_CONTROL_COUNT];
	int m_nMaxActCount;
	bool m_bActSecond;
	bool m_bActSupportRepeat;
	bool m_bSupportInsertPlay;
	bool m_bSupportWarning;
	bool m_bSupportCycleTimeHint;
	UnicodeString m_sAPN;
	UnicodeString m_sDataCenter;
	bool m_bActSupportSync;
	bool m_bTCP;
	bool m_bLoginEcho;
	int m_bActSuitForAllCard[MAX_ACT_COUNT];

	void SaveSystemPassword(UnicodeString sSystemPassword);
	void SaveConfig();

	//节目定义
	UnicodeString m_sActDesc;
	int m_nActArea;
	int m_nActEnterStyle;
	int m_nActEnterSpeed;
	int m_nActExitStyle;
	int m_nActExitSpeed;
	int m_nActShowTime;
	int m_nActShutterWidth;
	int m_nActFlashLightTime;
	int m_nActFlashDarkTime;
	int m_nActStartYear;
	int m_nActStartMonth;
	int m_nActStartDay;
	int m_nActStartHour;
	int m_nActStartMin;
	int m_nActEndYear;
	int m_nActEndMonth;
	int m_nActEndDay;
	int m_nActEndHour;
	int m_nActEndMin;
	int m_nActStartWeek;
	int m_nActEndWeek;
	int m_nActRepeatCount;
	int m_nActStartMinuteInDay[ACT_TIME_PERIOD_COUNT_24];
	int m_nActEndMinuteInDay[ACT_TIME_PERIOD_COUNT_24];
	int m_nActStartSecondInDay[ACT_TIME_PERIOD_COUNT_24];
	int m_nActEndSecondInDay[ACT_TIME_PERIOD_COUNT_24];
	int m_nActRefIndex;
	UnicodeString m_sActText;
	UnicodeString m_sActTextColor;
	UnicodeString m_sActActiveText;
	UnicodeString m_sActFontName;
	bool m_bDotFont;
	int m_nDotFontSize;
	int m_nActFontSize;
	int m_nActFontStyle;
	int m_nActHorzAlignment;
	int m_nActVertAlignment;
	int m_nActDownCounterBaseYear;
	int m_nActDownCounterBaseMonth;
	int m_nActDownCounterBaseDay;
	TPicture* m_picAct;
	int m_nActImagePosX;
	int m_nActImagePosY;
	int m_nSycleTimeActIndex;
	UnicodeString m_szSmsUser[8];
	bool m_bEchoSms[8];
	int m_nSmsActShowTime;
	int m_nSmsActEnterStyle;
	int m_nSmsActEnterSpeed;
	int m_nSmsActExitStyle;
	int m_nSmsActExitSpeed;
	int m_nSmsActRepeatCount;
	int m_nSmsActFontSize;
	int m_nVoiceOutputAreaIndex;
	int m_nCardBaudRate;
	UnicodeString m_sGpsServer;
	int m_nGpsSocketPort;
	int m_nGpsReportInterval;
	UnicodeString m_sGpsId;

	BOOL m_bOldInifileExist;
	UnicodeString m_sCardType;

	void LoadAct(CDtu* dtu, int nIndex);
	void CheckActiveText();
	void CheckTextColor();
	void SaveAct(CDtu* dtu, int nIndex);
	void DeleteAct(CDtu* dtu, int nIndex);
	UnicodeString GetActText(CDtu* dtu, int nActIndex);
	UnicodeString GetActDesc(CDtu* dtu, int nActIndex);
	bool CheckActHasPicture(CDtu* dtu, int nActIndex);
	int GetActRefIndex(CDtu* dtu, int nActIndex);
	int GetActRepeatCount(CDtu* dtu, int nActIndex);
	int GetActArea(CDtu* dtu, int nActIndex);
	int GetActEnterStyle(CDtu* dtu, int nActIndex);
	int GetActExitStyle(CDtu* dtu, int nActIndex);
	int GetActEnterSpeed(CDtu* dtu, int nActIndex);
	int GetActExitSpeed(CDtu* dtu, int nActIndex);
	int GetActShowTime(CDtu* dtu, int nActIndex);
	int GetActShutterWidth(CDtu* dtu, int nActIndex);
	int GetMaxPackSize();
	int GetOneActVolMaxLen();
	void SaveActData(CDtu* dtu, int nActIndex, BYTE* p, int nLen);
	int ReadActData(CDtu* dtu, int nActIndex, int nStartPos, int nReadLen, BYTE* p);
	int GetActDataSize(CDtu* dtu, int nActIndex);
	void SaveActToFile(UnicodeString sFileName);
	void OpenActFile(UnicodeString sFileName);
	UnicodeString ProcessTextRead(UnicodeString s);
	UnicodeString PrepareTextToWrite(UnicodeString s);
	bool GetDotFont(CDtu* dtu, int nActIndex);
	int GetDotFontSize(CDtu* dtu, int nActIndex);
	bool TestIfHasPicture(CDtu* dtu, int nActIndex);
	void LoadActVol_V3(TIniFile* sysini, int nIndex);
	UnicodeString ProcessTextRead_V3(UnicodeString s);
	void MakeCardTypeName();
	void InitCardTypeAndRootName();
	void LoadMainConfig(UnicodeString sSection);
	CDtu* GetActDtu(CDtu* dtu, int nActIndex);
};

class CUtils
{
public:
	void GetRGBColor(WORD usColor, WORD usActive, DWORD* pdwFColor, DWORD* pdwBColor);
	DWORD GetBitmapColor(DWORD dwColor);
};

extern CSysConfig sysConfig;
extern CUtils utils;

extern unsigned char* prog;
extern int g_nSizeofProg;
extern int BCD2Hex(BYTE c);

#endif
