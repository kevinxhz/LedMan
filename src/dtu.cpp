//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include "global.h"
#include "sysconfig.h"
#include <inifiles.hpp>
#include <registry.hpp>
#include "dtu.h"
#include "act.h"
#include "CardList.h"
#include "ConfigForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CDtu::CDtu()
{
}

void CDtu::Create()
{
	m_sCode = "12345678901";
	m_nCardId = CARD_ID_GS7020B;
	m_ulLastCheckResult = 0;
	m_dwlastCheckResultReportTime = ::GetTickCount() - 60 * 60 * 1000;
	m_sWifiSSID = "TP-LINK_12345678";
	m_sWifiPassword = "123456";
	m_sWifiDnsServer = "192.168.1.1";
}
//---------------------------------------------------------------------------

void CDtu::Create(CDtu* dtu)
{
	m_nNodeType = dtu->m_nNodeType;
	m_sSection = dtu->m_sSection;
	if (dtu->m_sCode.Length() > 3)
	{
		char s[4];
		strcpy(s, dtu->m_sCode.c_str() + dtu->m_sCode.Length() - 3);
		sprintf(s, "%03d", dtu->m_sCode.SubString(dtu->m_sCode.Length() - 2, 3).ToInt() + 1);
		m_sCode = dtu->m_sCode.SubString(1, dtu->m_sCode.Length() - 3) + s;
	}
	else
		m_sCode = AnsiString(dtu->m_sCode.ToInt() + 1);

	m_nCardId = dtu->m_nCardId;
}
//---------------------------------------------------------------------------

CDtu::~CDtu()
{
}
//---------------------------------------------------------------------------

void CDtu::ClearActTxOkStatus()
{
	bool bWantWriteIniFile = false;

	for (int i = 0; i < sysConfig.m_nMaxActCount; i++)
	{
		if (!m_bActTxOk[i])
		{
			bWantWriteIniFile = true;
			m_bActTxOk[i] = true;
		}
	}

	if (bWantWriteIniFile)
		WriteTxActStatus();
}

void CDtu::WriteTxActStatus()
{
	char s[MAX_ACT_COUNT + 1];

	TIniFile* ini = new TIniFile(ExtractFilePath(Application->ExeName) + "TxStatus.txt");

	for (int i = 0; i < sysConfig.m_nMaxActCount; i++)
		s[i] = m_bActTxOk[i] ? '1': '0';
	s[sysConfig.m_nMaxActCount] = 0;
	ini->WriteString("TX", m_sCode, s);

	delete ini;
}
//---------------------------------------------------------------------------

void CDtu::ReadTxActStatus()
{
	AnsiString s;
	TIniFile* ini = new TIniFile(ExtractFilePath(Application->ExeName) + "TxStatus.txt");

	s = ini->ReadString("TX", m_sCode, "");
	char *p = s.c_str();
	memset(m_bActTxOk, 1, sizeof(m_bActTxOk));

	int i = 0;
	while (*p)
	{
		if (*p == '0')
			m_bActTxOk[i] = false;
		i++; p++;
		if (i >= sysConfig.m_nMaxActCount)
			break;
	}

	delete ini;
}
//---------------------------------------------------------------------------

void CDtu::LoadConfig()
{
	TCustomIniFile* ini = new TRegistryIniFile(m_sSection + "\\" + m_sName);

	m_nNodeType = ini->ReadInteger("Config", "NodeTpe", NODE_CARD);
	m_nCardId = ini->ReadInteger("Config", "CardId", 88);
	m_sCode = ini->ReadString("Config", "DtuCode", "12345678901");
	m_sWifiSSID = ini->ReadString("Config", "WifiSSID", "TP-LINK_12345678");
	m_sWifiPassword = ini->ReadString("Config", "WifiPassword", "123456");
	m_sWifiDnsServer = ini->ReadString("Config", "WifiDnsServer", "192.168.1.1");
#ifdef AUTO_SIZE_TEXT
	m_nScreenWidth = ini->ReadInteger("Config", "ScreenWidth", 128);
	m_nScreenHeight = ini->ReadInteger("Config", "ScreenHeight", 16);
	m_nSubAreaCount = ini->ReadInteger("Config", "SubAreaCount", 1);
	if (m_nSubAreaCount < 1)
		m_nSubAreaCount = 1;
	if (m_nSubAreaCount > MAX_AREA_COUNT)
		m_nSubAreaCount = MAX_AREA_COUNT;
	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		m_nSubAreaLeft[i] = ini->ReadInteger("Config", "AreaLeft" + IntToStr(i), 0);
		m_nSubAreaTop[i] = ini->ReadInteger("Config", "AreaTop" + IntToStr(i), 0);
		m_nSubAreaWidth[i] = ini->ReadInteger("Config", "AreaWidth" + IntToStr(i), m_nScreenWidth);
		if (m_nSubAreaWidth[i] < 8) m_nSubAreaWidth[i] = 8;
		m_nSubAreaHeight[i] = ini->ReadInteger("Config", "AreaHeight" + IntToStr(i), m_nScreenHeight);
		if (m_nSubAreaHeight[i] < 8) m_nSubAreaHeight[i] = 8;
	}
#endif
#ifdef XINJIANG_AUTO_WEB_DATA
	m_sWebDataId = ini->ReadString("Config", "WebDataId", "");
#endif

	m_sLocalIp = ini->ReadString("Config", "LocalIp", "192.168.1.111");
	m_sGateWay = ini->ReadString("Config", "GateWay", "192.168.1.1");
	m_sSubMask = ini->ReadString("Config", "SubMask", "255.255.255.0");

	delete ini;
}
//---------------------------------------------------------------------------

#ifdef AUTO_SIZE_TEXT
void CDtu::ApplyCurrentScreenConfig(void)
{
	sysConfig.m_nScreenWidth = m_nScreenWidth;
	sysConfig.m_nScreenHeight = m_nScreenHeight;
	sysConfig.m_nSubAreaCount = m_nSubAreaCount;
	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		sysConfig.m_nSubAreaLeft[i] = m_nSubAreaLeft[i];
		sysConfig.m_nSubAreaTop[i] = m_nSubAreaTop[i];
		sysConfig.m_nSubAreaWidth[i] = m_nSubAreaWidth[i];
		sysConfig.m_nSubAreaHeight[i] = m_nSubAreaHeight[i];
	}
}

int CDtu::CompareScreenConfig(void)
{
	if (sysConfig.m_nScreenWidth != m_nScreenWidth)
		return 1;
	if (sysConfig.m_nScreenHeight != m_nScreenHeight)
		return 1;
	if (sysConfig.m_nSubAreaCount != m_nSubAreaCount)
		return 1;
	for (int i = 0; i < m_nSubAreaCount; i++)
	{
		if (sysConfig.m_nSubAreaLeft[i] != m_nSubAreaLeft[i])
			return 1;
		if (sysConfig.m_nSubAreaTop[i] != m_nSubAreaTop[i])
			return 1;
		if (sysConfig.m_nSubAreaWidth[i] != m_nSubAreaWidth[i])
			return 1;
		if (sysConfig.m_nSubAreaHeight[i] != m_nSubAreaHeight[i])
			return 1;
	}

	return 0;
}
#endif
//---------------------------------------------------------------------------

void CDtu::SaveConfig()
{
	TCustomIniFile* ini = new TRegistryIniFile(m_sSection + "\\" + m_sName);

	ini->WriteString("Config", "DtuCode", m_sCode);
	ini->WriteInteger("Config", "NodeTpe", m_nNodeType);
	ini->WriteInteger("Config", "CardId", m_nCardId);
	ini->WriteString("Config", "WifiSSID", m_sWifiSSID);
	ini->WriteString("Config", "WifiPassword", m_sWifiPassword);
	ini->WriteString("Config", "WifiDnsServer", m_sWifiDnsServer);
#ifdef AUTO_SIZE_TEXT
	ini->WriteInteger("Config", "ScreenWidth", m_nScreenWidth);
	ini->WriteInteger("Config", "ScreenHeight", m_nScreenHeight);
	ini->WriteInteger("Config", "SubAreaCount", m_nSubAreaCount);
	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		ini->WriteInteger("Config", "AreaLeft" + IntToStr(i), m_nSubAreaLeft[i]);
		ini->WriteInteger("Config", "AreaTop" + IntToStr(i), m_nSubAreaTop[i]);
		ini->WriteInteger("Config", "AreaWidth" + IntToStr(i), m_nSubAreaWidth[i]);
		ini->WriteInteger("Config", "AreaHeight" + IntToStr(i), m_nSubAreaHeight[i]);
	}
#endif

#ifdef XINJIANG_AUTO_WEB_DATA
	ini->WriteString("Config", "WebDataId", m_sWebDataId);
#endif

	ini->WriteString("Config", "LocalIp", m_sLocalIp);
	ini->WriteString("Config", "GateWay", m_sGateWay);
	ini->WriteString("Config", "SubMask", m_sSubMask);

	delete ini;
}
//---------------------------------------------------------------------------

void CDtu::DeleteRegistry()
{
	TCustomIniFile* ini = new TRegistryIniFile(m_sSection);
	ini->EraseSection(m_sName);
	delete ini;
}
//---------------------------------------------------------------------------

void CDtu::Rename(UnicodeString sNewName)
{
	TCustomIniFile* iniSrc = new TRegistryIniFile(m_sSection + "\\" + m_sName);
	TCustomIniFile* iniDst = new TRegistryIniFile(m_sSection + "\\" + sNewName);

	SYSTEMTIME st;
	::GetLocalTime(&st);
	for (int nIndex = 0; nIndex < MAX_ACT_COUNT; nIndex++)
	{
		UnicodeString wsSection = "ACT" + UnicodeString(nIndex + 1);

		if (iniSrc->SectionExists(wsSection))
		{
			iniDst->WriteString(wsSection, "ActDesc", iniSrc->ReadString(wsSection, "ActDesc", ""));
			iniDst->WriteInteger(wsSection, "ActArea", iniSrc->ReadInteger(wsSection, "ActArea", 0));
			iniDst->WriteInteger(wsSection, "EnterStyle", iniSrc->ReadInteger(wsSection, "EnterStyle", ENTER_STYLE_DIRECT));
			iniDst->WriteInteger(wsSection, "EnterSpeed", iniSrc->ReadInteger(wsSection, "EnterSpeed", 16));
			iniDst->WriteInteger(wsSection, "ExitStyle", iniSrc->ReadInteger(wsSection, "ExitStyle", EXIT_STYLE_NULL));
			iniDst->WriteInteger(wsSection, "ExitSpeed", iniSrc->ReadInteger(wsSection, "ExitSpeed", 16));
			iniDst->WriteInteger(wsSection, "ShowTime", iniSrc->ReadInteger(wsSection, "ShowTime", 3000));
			iniDst->WriteInteger(wsSection, "ShutterWidth", iniSrc->ReadInteger(wsSection, "ShutterWidth", 8));
			iniDst->WriteInteger(wsSection, "FlashLightTime", iniSrc->ReadInteger(wsSection, "FlashLightTime", 500));
			iniDst->WriteInteger(wsSection, "FlashDarkTime", iniSrc->ReadInteger(wsSection, "FlashDarkTime", 500));
			iniDst->WriteInteger(wsSection, "StartYear", iniSrc->ReadInteger(wsSection, "StartYear", st.wYear));
			iniDst->WriteInteger(wsSection, "StartMonth", iniSrc->ReadInteger(wsSection, "StartMonth", st.wMonth));
			iniDst->WriteInteger(wsSection, "StartDay", iniSrc->ReadInteger(wsSection, "StartDay", st.wDay));
			iniDst->WriteInteger(wsSection, "StartHour", iniSrc->ReadInteger(wsSection, "StartHour", 0));
			iniDst->WriteInteger(wsSection, "StartMin", iniSrc->ReadInteger(wsSection, "StartMin", 0));
			iniDst->WriteInteger(wsSection, "EndYear", iniSrc->ReadInteger(wsSection, "EndYear", 2099));
			iniDst->WriteInteger(wsSection, "EndMonth", iniSrc->ReadInteger(wsSection, "EndMonth", 12));
			iniDst->WriteInteger(wsSection, "EndDay", iniSrc->ReadInteger(wsSection, "EndDay", 31));
			iniDst->WriteInteger(wsSection, "EndHour", iniSrc->ReadInteger(wsSection, "EndHour", 23));
			iniDst->WriteInteger(wsSection, "EndMin", iniSrc->ReadInteger(wsSection, "EndMin", 59));
			iniDst->WriteInteger(wsSection, "StartWeek", iniSrc->ReadInteger(wsSection, "StartWeek", 0));
			iniDst->WriteInteger(wsSection, "EndWeek", iniSrc->ReadInteger(wsSection, "EndWeek", 6));
			for (int i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
			{
				if (i == 0)
				{
					iniDst->WriteInteger(wsSection, "StartMinuteInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "StartMinuteInday" + UnicodeString(i), 0));
					iniDst->WriteInteger(wsSection, "EndMinuteInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "EndMinuteInday" + UnicodeString(i), 60 * 24 - 1));
					iniDst->WriteInteger(wsSection, "StartSecondInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "StartSecondInday" + UnicodeString(i), 0));
					iniDst->WriteInteger(wsSection, "EndSecondInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "EndSecondInday" + UnicodeString(i), 59));
				}
				else
				{
					iniDst->WriteInteger(wsSection, "StartMinuteInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "StartMinuteInday" + UnicodeString(i), 0));
					iniDst->WriteInteger(wsSection, "EndMinuteInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "EndMinuteInday" + UnicodeString(i), 0));
					iniDst->WriteInteger(wsSection, "StartSecondInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "StartSecondInday" + UnicodeString(i), 0));
					iniDst->WriteInteger(wsSection, "EndSecondInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "EndSecondInday" + UnicodeString(i), 0));
				}
			}
			iniDst->WriteInteger(wsSection, "RefAct", iniSrc->ReadInteger(wsSection, "RefAct", nIndex));

			iniDst->WriteString(wsSection, "ActText", iniSrc->ReadString(wsSection, "ActText", ""));
			iniDst->WriteString(wsSection, "TextColor", iniSrc->ReadString(wsSection, "TextColor", ""));
			iniDst->WriteString(wsSection, "Active", iniSrc->ReadString(wsSection, "Active", ""));
			iniDst->WriteString(wsSection, "FontName", iniSrc->ReadString(wsSection, "FontName", "����"));
			iniDst->WriteInteger(wsSection, "FontSize", iniSrc->ReadInteger(wsSection, "FontSize", 12));
			iniDst->WriteBool(wsSection, "FontStyle", iniSrc->ReadInteger(wsSection, "FontStyle", 0));
			iniDst->WriteInteger(wsSection, "HorzAlign", iniSrc->ReadInteger(wsSection, "HorzAlign", TEXT_ALIGN_LEFT));
			iniDst->WriteInteger(wsSection, "VertAlign", iniSrc->ReadInteger(wsSection, "VertAlign", 0));
			iniDst->WriteInteger(wsSection, "DownCounterBaseYear", iniSrc->ReadInteger(wsSection, "DownCounterBaseYear", 2050));
			iniDst->WriteInteger(wsSection, "DownCounterBaseMonth", iniSrc->ReadInteger(wsSection, "DownCounterBaseMonth", 1));
			iniDst->WriteInteger(wsSection, "DownCounterBaseDay", iniSrc->ReadInteger(wsSection, "DownCounterBaseDay", 1));

			iniDst->WriteInteger(wsSection, "ImagePosX", iniSrc->ReadInteger(wsSection, "ImagePosX", 0));
			iniDst->WriteInteger(wsSection, "ImagePosY", iniSrc->ReadInteger(wsSection, "ImagePosY", 0));
			iniDst->WriteInteger(wsSection, "ImageWidth", iniSrc->ReadInteger(wsSection, "ImageWidth", 0));
			iniDst->WriteInteger(wsSection, "ImageHeight", iniSrc->ReadInteger(wsSection, "ImageHeight", 0));

			TMemoryStream* stream = new TMemoryStream;
			iniSrc->ReadBinaryStream(wsSection, "ImageVol", stream);
			stream->Position = 0;
			iniDst->WriteBinaryStream(wsSection, "ImageVol", stream);
			delete stream;
		}
	}

	delete iniDst;
	delete iniSrc;

	DeleteRegistry();
	m_sName = sNewName;
	SaveConfig();
}
//---------------------------------------------------------------------------

void CDtu::CopyTo(UnicodeString sDestSection)
{
	if (sDestSection == m_sSection)
		return;

	TCustomIniFile* iniSrc = new TRegistryIniFile(m_sSection + "\\" + m_sName);
	TCustomIniFile* iniDst = new TRegistryIniFile(sDestSection + "\\" + m_sName);

	SYSTEMTIME st;
	::GetLocalTime(&st);
	for (int nIndex = 0; nIndex < MAX_ACT_COUNT; nIndex++)
	{
		UnicodeString wsSection = "ACT" + UnicodeString(nIndex + 1);

		if (iniSrc->SectionExists(wsSection))
		{
			iniDst->WriteString(wsSection, "ActDesc", iniSrc->ReadString(wsSection, "ActDesc", ""));
			iniDst->WriteInteger(wsSection, "ActArea", iniSrc->ReadInteger(wsSection, "ActArea", 0));
			iniDst->WriteInteger(wsSection, "EnterStyle", iniSrc->ReadInteger(wsSection, "EnterStyle", ENTER_STYLE_DIRECT));
			iniDst->WriteInteger(wsSection, "EnterSpeed", iniSrc->ReadInteger(wsSection, "EnterSpeed", 16));
			iniDst->WriteInteger(wsSection, "ExitStyle", iniSrc->ReadInteger(wsSection, "ExitStyle", EXIT_STYLE_NULL));
			iniDst->WriteInteger(wsSection, "ExitSpeed", iniSrc->ReadInteger(wsSection, "ExitSpeed", 16));
			iniDst->WriteInteger(wsSection, "ShowTime", iniSrc->ReadInteger(wsSection, "ShowTime", 3000));
			iniDst->WriteInteger(wsSection, "ShutterWidth", iniSrc->ReadInteger(wsSection, "ShutterWidth", 8));
			iniDst->WriteInteger(wsSection, "FlashLightTime", iniSrc->ReadInteger(wsSection, "FlashLightTime", 500));
			iniDst->WriteInteger(wsSection, "FlashDarkTime", iniSrc->ReadInteger(wsSection, "FlashDarkTime", 500));
			iniDst->WriteInteger(wsSection, "StartYear", iniSrc->ReadInteger(wsSection, "StartYear", st.wYear));
			iniDst->WriteInteger(wsSection, "StartMonth", iniSrc->ReadInteger(wsSection, "StartMonth", st.wMonth));
			iniDst->WriteInteger(wsSection, "StartDay", iniSrc->ReadInteger(wsSection, "StartDay", st.wDay));
			iniDst->WriteInteger(wsSection, "StartHour", iniSrc->ReadInteger(wsSection, "StartHour", 0));
			iniDst->WriteInteger(wsSection, "StartMin", iniSrc->ReadInteger(wsSection, "StartMin", 0));
			iniDst->WriteInteger(wsSection, "EndYear", iniSrc->ReadInteger(wsSection, "EndYear", 2099));
			iniDst->WriteInteger(wsSection, "EndMonth", iniSrc->ReadInteger(wsSection, "EndMonth", 12));
			iniDst->WriteInteger(wsSection, "EndDay", iniSrc->ReadInteger(wsSection, "EndDay", 31));
			iniDst->WriteInteger(wsSection, "EndHour", iniSrc->ReadInteger(wsSection, "EndHour", 23));
			iniDst->WriteInteger(wsSection, "EndMin", iniSrc->ReadInteger(wsSection, "EndMin", 59));
			iniDst->WriteInteger(wsSection, "StartWeek", iniSrc->ReadInteger(wsSection, "StartWeek", 0));
			iniDst->WriteInteger(wsSection, "EndWeek", iniSrc->ReadInteger(wsSection, "EndWeek", 6));
			for (int i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
			{
				if (i == 0)
				{
					iniDst->WriteInteger(wsSection, "StartMinuteInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "StartMinuteInday" + UnicodeString(i), 0));
					iniDst->WriteInteger(wsSection, "EndMinuteInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "EndMinuteInday" + UnicodeString(i), 60 * 24 - 1));
					iniDst->WriteInteger(wsSection, "StartSecondInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "StartSecondInday" + UnicodeString(i), 0));
					iniDst->WriteInteger(wsSection, "EndSecondInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "EndSecondInday" + UnicodeString(i), 59));
				}
				else
				{
					iniDst->WriteInteger(wsSection, "StartMinuteInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "StartMinuteInday" + UnicodeString(i), 0));
					iniDst->WriteInteger(wsSection, "EndMinuteInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "EndMinuteInday" + UnicodeString(i), 0));
					iniDst->WriteInteger(wsSection, "StartSecondInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "StartSecondInday" + UnicodeString(i), 0));
					iniDst->WriteInteger(wsSection, "EndSecondInday" + UnicodeString(i), iniSrc->ReadInteger(wsSection, "EndSecondInday" + UnicodeString(i), 0));
				}
			}
			iniDst->WriteInteger(wsSection, "RefAct", iniSrc->ReadInteger(wsSection, "RefAct", nIndex));

			iniDst->WriteString(wsSection, "ActText", iniSrc->ReadString(wsSection, "ActText", ""));
			iniDst->WriteString(wsSection, "TextColor", iniSrc->ReadString(wsSection, "TextColor", ""));
			iniDst->WriteString(wsSection, "Active", iniSrc->ReadString(wsSection, "Active", ""));
			iniDst->WriteString(wsSection, "FontName", iniSrc->ReadString(wsSection, "FontName", "����"));
			iniDst->WriteInteger(wsSection, "FontSize", iniSrc->ReadInteger(wsSection, "FontSize", 12));
			iniDst->WriteBool(wsSection, "FontStyle", iniSrc->ReadInteger(wsSection, "FontStyle", 0));
			iniDst->WriteInteger(wsSection, "HorzAlign", iniSrc->ReadInteger(wsSection, "HorzAlign", TEXT_ALIGN_LEFT));
			iniDst->WriteInteger(wsSection, "VertAlign", iniSrc->ReadInteger(wsSection, "VertAlign", 0));
			iniDst->WriteInteger(wsSection, "DownCounterBaseYear", iniSrc->ReadInteger(wsSection, "DownCounterBaseYear", 2050));
			iniDst->WriteInteger(wsSection, "DownCounterBaseMonth", iniSrc->ReadInteger(wsSection, "DownCounterBaseMonth", 1));
			iniDst->WriteInteger(wsSection, "DownCounterBaseDay", iniSrc->ReadInteger(wsSection, "DownCounterBaseDay", 1));

			iniDst->WriteInteger(wsSection, "ImagePosX", iniSrc->ReadInteger(wsSection, "ImagePosX", 0));
			iniDst->WriteInteger(wsSection, "ImagePosY", iniSrc->ReadInteger(wsSection, "ImagePosY", 0));
			iniDst->WriteInteger(wsSection, "ImageWidth", iniSrc->ReadInteger(wsSection, "ImageWidth", 0));
			iniDst->WriteInteger(wsSection, "ImageHeight", iniSrc->ReadInteger(wsSection, "ImageHeight", 0));

			TMemoryStream* stream = new TMemoryStream;
			iniSrc->ReadBinaryStream(wsSection, "ImageVol", stream);
			stream->Position = 0;
			iniDst->WriteBinaryStream(wsSection, "ImageVol", stream);
			delete stream;
		}
	}

	delete iniDst;
	delete iniSrc;

	DeleteRegistry();
	m_sSection = sDestSection;
	SaveConfig();
}
//---------------------------------------------------------------------------

bool CDtu::VerifyProg()
{
	if ((sysConfig.m_nCardType == CARD_TYPE_GS7020BF_HEB) || (sysConfig.m_nCardType == CARD_TYPE_GS9026))
		return VerifyProg_HEB();
	else
		return VerifyProg_Normal();
}
//---------------------------------------------------------------------------

bool CDtu::VerifyProg_Normal()
{
	MMI_CMD_HEAD* pHead = (MMI_CMD_HEAD*)m_ucRxBuf;
	DWORD* pStartPos = (DWORD*)(pHead + 1);
	WORD* pusDataLen = (WORD*)(pStartPos + 1);
	BYTE* pucData = (BYTE*)(pusDataLen + 1);

	int nLeftDataLen = g_nSizeofProg - m_nDataSent;
	int nDataLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize: nLeftDataLen;

	DWORD dwWant;
	if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
		dwWant = (DWORD)(256 * 1024 + m_nDataSent);
	else
		dwWant = (DWORD)(1024 * 1024 + m_nDataSent);

	if (*pStartPos != dwWant)
		return false;

	if (*pusDataLen != nDataLen)
		return false;

	if (memcmp(pucData, prog + m_nDataSent, nDataLen) != 0)
		return false;

	return true;
}
//---------------------------------------------------------------------------

bool CDtu::VerifyProg_HEB()
{
	MMI_CMD_HEAD_HEB* pHead = (MMI_CMD_HEAD_HEB*)m_ucRxBuf;
	DWORD* pStartPos = (DWORD*)(pHead + 1);
	WORD* pusDataLen = (WORD*)(pStartPos + 1);
	BYTE* pucData = (BYTE*)(pusDataLen + 1);

	int nLeftDataLen = g_nSizeofProg - m_nDataSent;
	int nDataLen = (nLeftDataLen > sysConfig.m_nMaxPackSize) ? sysConfig.m_nMaxPackSize: nLeftDataLen;

	if (*pStartPos != DWORD(1024 * 1024 + m_nDataSent))
		return false;

	if (*pusDataLen != nDataLen)
		return false;

	if (memcmp(pucData, prog + m_nDataSent, nDataLen) != 0)
		return false;

	return true;
}
//---------------------------------------------------------------------------

int CDtu::GetCardId()
{
	if (sysConfig.m_nCardType == CARD_TYPE_YL2010A)
		return m_nCardId;
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010A_GPS_JTX)
		return m_nCardId;
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010C)
		return m_nCardId;
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010AR)
		return m_nCardId;
	else if (sysConfig.m_nCardType == CARD_TYPE_YL2010AF)
		return m_nCardId;
	else if (sysConfig.m_nCardType == CARD_TYPE_PD102B)
		return m_nCardId;
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9025)
		return 89;
	else if (sysConfig.m_nCardType == CARD_TYPE_GS9023)
		return 89;
	else
		return CARD_ID_GS7020B;
}
//---------------------------------------------------------------------------

#ifdef AUTO_SIZE_TEXT
void CDtu::CopyTxData(int nActIndex)
{
	CDtu* dtuRoot = this->m_dtuParent;
	while (dtuRoot->m_nNodeType != NODE_ALL_CARD_ROOT)
		dtuRoot = dtuRoot->m_dtuParent;

	UnicodeString sActRoot = dtuRoot->m_sSection + "\\" + dtuRoot->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);
	TMemoryStream* stream = new TMemoryStream;
	int nLen = ini->ReadBinaryStream(wsSection, "ActData", stream);

	UnicodeString sActRootThis = m_sSection + "\\" + m_sName + "\\Acts";
	TCustomIniFile* iniThis = new TRegistryIniFile(sActRootThis);
	stream->Position = 0;
	iniThis->WriteBinaryStream(wsSection, "ActData", stream);
	iniThis->WriteInteger(wsSection, "ActDataSize", nLen);

	delete stream;
	delete ini;
	delete iniThis;
}
#endif
//---------------------------------------------------------------------------


