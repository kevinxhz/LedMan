//---------------------------------------------------------------------------
#ifndef DtuH
#define DtuH
//---------------------------------------------------------------------------
#include "global.h"
#include <inifiles.hpp>
#include <registry.hpp>

#define FT_LOGIN  0xA0
#define FT_DATA   0xA3
#define FT_STATUS 0xA5
#define FT_GPS 	  0xA6
#define FT_WARNNING 0xEE

#define TX_STATUS_WAIT_OPEN			0
#define TX_STATUS_OPEN_ERROR		1
#define TX_STATUS_READY				2
#define TX_STATUS_WAIT_ECHO			3
#define TX_STATUS_END_OK			4
#define TX_STATUS_END_TIME_OUT		5

#define NODE_GROUP			0
#define NODE_CARD			1
#define NODE_ALL_CARD_ROOT	2

//默认节目内容
#define	DEFAULT_ACT_DESC 					""
#define	DEFAULT_ACT_AREA					0
#define	DEFAULT_ACT_ENTER_STYLE 			ENTER_STYLE_DIRECT
#define	DEFAULT_ACT_ENTER_SPEED				16
#define	DEFAULT_ACT_EXIT_STYLE              EXIT_STYLE_NULL
#define	DEFAULT_ACT_EXIT_SPEED				16
#define	DEFAULT_ACT_SHOW_TIME				3000
#define	DEFAULT_ACT_ST_WIDTH				8
#define	DEFAULT_ACT_FLASH_LIGHT_TIME		500
#define	DEFAULT_ACT_FLASH_DARK_TIME			500
#define	DEFAULT_ACT_START_YEAR				2000
#define	DEFAULT_ACT_END_YEAR				2099
/////////////////////

#include "SerialPort.h"
#include "VirtualTrees.hpp"
#include "act.h"

class CDtu
{
public:
	CDtu();
	virtual ~CDtu();
	void Create();
	void Create(CDtu* dtu);

	CDtu* m_dtuParent;
	int m_nNodeType;
	UnicodeString m_sSection;
	UnicodeString m_sName;
	AnsiString m_sCode;
	UnicodeString m_sStatus;
	UnicodeString m_sWifiSSID;
	UnicodeString m_sWifiPassword;
	UnicodeString m_sWifiDnsServer;

	UnicodeString m_sWebDataId;

	UnicodeString m_sLocalIp;
	UnicodeString m_sGateWay;
	UnicodeString m_sSubMask;

	BYTE m_ucRxBuf[1024 + 64];
	int m_nRxLen;
	int m_nTxStatus;
	DWORD m_dwTxStartTime;
	int m_nRetries;
	int m_nComPortRxStep;
	bool m_bWantVerifyAct;
	BYTE m_ucKey1, m_ucKey2;
	DWORD m_ulLastCheckResult;
	DWORD m_dwlastCheckResultReportTime;
#ifdef AUTO_SIZE_TEXT
	int m_nScreenWidth;
	int m_nScreenHeight;
	int m_nSubAreaCount;
	int m_nSubAreaLeft[MAX_AREA_COUNT];
	int m_nSubAreaTop[MAX_AREA_COUNT];
	int m_nSubAreaWidth[MAX_AREA_COUNT];
	int m_nSubAreaHeight[MAX_AREA_COUNT];
	void ApplyCurrentScreenConfig(void);
	int CompareScreenConfig(void);
#endif
	BOOLEAN m_bActTxOk[MAX_ACT_COUNT];
	BYTE m_ucChipId[8];

	void ClearActTxOkStatus();
	void WriteTxActStatus();
	void ReadTxActStatus();
	bool VerifyProg();
	bool VerifyProg_HEB();
	bool VerifyProg_Normal();

	//节目发送
	int m_nDataSent;	//已经发送的节目内容长度
	int m_nTxAct;		//正在发送的节目序号
	int m_nTxStep;		//命令执行步骤

	//控制卡配置
	int m_nCardId;
	int m_nCardFontSize;

	void LoadConfig();
	void SaveConfig();
	void DeleteRegistry();
	void Rename(UnicodeString sNewName);
	void CopyTo(UnicodeString sDestSection);
	int GetCardId();
	bool m_bTxDataCreated;
	void CopyTxData(int nActIndex);

};

#endif
