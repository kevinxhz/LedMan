//---------------------------------------------------------------------------
#pragma hdrstop
#include "SysConfig.h"
#include <vcl.h>
#include <inifiles.hpp>
#include <registry.hpp>
#include <stdio.h>
#include "act.h"
#include "dtu.h"
#pragma package(smart_init)
#include "prog_GS7020B.h"
#include "prog_GS7030B.h"
#include "prog_GS7020BF.h"
#include "prog_GS7020BF_HEB.h"
#include "prog_GS7020BF_W.h"
#include "prog_YL2010AR.h"
#include "prog_GS7020BX.h"
#include "prog_GS7020HQ.h"
#include "prog_GS7020BT.h"
#include "prog_GS7030BV2.h"
#include "prog_GS7022B.h"
#include "prog_GS2012A.h"
#include "prog_YL2010A.h"
#include "prog_GS2012A_S.h"
#include "prog_GS7020BP.h"
#include "prog_GS9026.h"
#include "prog_GS7024E.h"
#include "prog_GS2012B.h"
#include "prog_GS9020.h"
#include "prog_GS7020H.h"
#include "prog_GS9025.h"
#include "prog_GS9023.h"
#include "prog_GS9020F_GPS.h"
#include "prog_GS7020HFA.h"
#include "prog_SN2P.h"

//---------------------------------------------------------------------------
unsigned char* prog;
int g_nSizeofProg;

CSysConfig sysConfig;
CUtils utils;

//---------------------------------------------------------------------------
CSysConfig::CSysConfig()
{
	m_bOldInifileExist = false;

	if (FileExists(ExtractFilePath(Application->ExeName) + "config.ini"))
	{
		TIniFile* iniConfig = new TIniFile(ExtractFilePath(Application->ExeName) + "config.ini");
		m_sRootName = iniConfig->ReadString("config", "RootName", "All");
		m_sCardType = iniConfig->ReadString("config", "CardType", "GS7020B");
		delete iniConfig;
		m_bOldInifileExist = true;
		DeleteFile(ExtractFilePath(Application->ExeName) + "config.ini");
	}

	UnicodeString sSection;
	HKEY hKey;

	if (RegOpenKey(HKEY_CURRENT_USER, (UnicodeString(REG_ROOT_NAME) + "\\Config").c_str(), &hKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);

		sSection = REG_ROOT_NAME;
		LoadMainConfig(sSection);
		m_sRootName = "All";
		SaveConfig();

		TCustomIniFile* sysini = new TRegistryIniFile(sSection);
		sysini->EraseSection("Config");
		sysini->EraseSection("System");
		delete sysini;
	}
	else
	{
		if (!m_bOldInifileExist)
		{
			TCustomIniFile* ini = new TRegistryIniFile("Software\\Yuepoch");
			m_sRootName = ini->ReadString("LedPanelManV3", "RootName", "所有显示屏");
			delete ini;
		}

		sSection = UnicodeString(REG_ROOT_NAME) + "\\" + m_sRootName;
		LoadMainConfig(sSection);
	}

	if (m_bOldInifileExist)
	{
	   SaveConfig();
	   m_bOldInifileExist = false;
	}
	InitCardTypeAndRootName();
}

//---------------------------------------------------------------------------
void CSysConfig::LoadMainConfig(UnicodeString sSection)
{
	TCustomIniFile* sysini = new TRegistryIniFile(sSection);

	m_nCommMethod = sysini->ReadInteger("Config", "CommMethod", 0);
	m_sComPort = sysini->ReadString("Config", "ComPort", "COM1");
	m_nBaudRate = sysini->ReadInteger("Config", "BaudRate", 115200);
	m_nTimeout_SerialPort = sysini->ReadInteger("Config", "Timeout_SerialPort", 200);
	m_nRetry_SerialPort = sysini->ReadInteger("Config", "Retry_SerialPort", 2);
#ifdef FIXED_IP
	m_sServer = FIXED_IP;
	m_nSocketPort = 3700;
#else
	m_sServer = sysini->ReadString("Config", "Server", DEAFULT_DATA_CENTER);
	m_nSocketPort = sysini->ReadInteger("Config", "SocketPort", 3700);
#endif
	m_nTimeout_Network = sysini->ReadInteger("Config", "Timeout_Network", 4);
	m_nRetry_Network = sysini->ReadInteger("Config", "Retry_Network", 5);
	m_sUserName = sysini->ReadString("Config", "UserName", "test");
	m_sPassword = sysini->ReadString("Config", "Password", "123456");
	m_sSystemPassword = sysini->ReadString("System", "SystemPassword", "");
	m_nLanguage = sysini->ReadInteger("System", "Language", LANGUAGE_CHINESE);
	m_nAutoClockInterval = sysini->ReadInteger("System", "AutoWriteClockInterval", 0);
	m_nAutoClockMinute = sysini->ReadInteger("System", "AutoWriteClockMinute", 57);
	m_nAutoReTxActInterval = sysini->ReadInteger("System", "AutoReTxInterval", 0);
	m_bAutoReTx_OnlyLast = sysini->ReadBool("System", "AutoReTxOnlyLast", true);

	m_nCardBaudRate = sysini->ReadInteger("Config", "CardBaudRate", 115200);

	m_nScreenWidth = sysini->ReadInteger("Config", "ScreenWidth", 128);
	m_nScreenHeight = sysini->ReadInteger("Config", "ScreenHeight", 16);
	m_ulScanMode = sysini->ReadInteger("Config", "ScanMode", SCAN_MODE_16_1_B);
	m_bNegData = sysini->ReadBool("Config", "NegData", true);
	m_bNegOE = sysini->ReadBool("Config", "NegOE", false);
	m_bByteReverse = sysini->ReadBool("Config", "ByteReverse", false);
	m_bDoubleColor = sysini->ReadBool("Config", "DoubleColor", false);
	m_bFullColor = sysini->ReadBool("Config", "FullColor", false);
	m_bActSecond = sysini->ReadBool("Config", "ActSecond", false);
	m_bActSupportRepeat = sysini->ReadBool("Config", "SupportRepeat", false);
	m_bSupportInsertPlay = sysini->ReadBool("Config", "SupportInsert", false);
	m_bSupportWarning = sysini->ReadBool("Config", "SupportWarning", false);
	m_bSupportCycleTimeHint = sysini->ReadBool("Config", "TimeHint", false);
	m_sAPN = sysini->ReadString("Config", "APN", "cmnet");
#ifdef FIXED_IP
	m_sDataCenter = FIXED_IP;
	m_nSocketPort_Card = 3700;
	m_bTCP = false;
#else
	m_nSocketPort_Card = sysini->ReadInteger("Config", "SocketPort_Card", 3700);
	m_sDataCenter = sysini->ReadString("Config", "DataCenter", DEAFULT_DATA_CENTER);
	m_bTCP = sysini->ReadBool("Config", "TCP", false);
#endif
	m_bSupportLedSignCheck = sysini->ReadBool("Config", "SupportLedSignCheck", false);
	m_bLoginEcho = sysini->ReadBool("Config", "LoginEcho", false);

	m_sGpsServer = sysini->ReadString("Config", "GPS_Server", "192.168.1.1");
	m_nGpsSocketPort = sysini->ReadInteger("Config", "GPS_SocketPort", 3700);
	m_nGpsReportInterval = sysini->ReadInteger("Config", "GPS_ReportInterval", 30);
	m_sGpsId = sysini->ReadString("Config", "GPS_Id", "192.168.1.1");

	m_nSubAreaCount = sysini->ReadInteger("Config", "SubAreaCount", 1);
	if (m_nSubAreaCount < 1)
		m_nSubAreaCount = 1;
	if (m_nSubAreaCount > MAX_AREA_COUNT)
		m_nSubAreaCount = MAX_AREA_COUNT;

	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		m_nSubAreaLeft[i] = sysini->ReadInteger("Config", "AreaLeft" + IntToStr(i), 0);
		m_nSubAreaTop[i] = sysini->ReadInteger("Config", "AreaTop" + IntToStr(i), 0);
		m_nSubAreaWidth[i] = sysini->ReadInteger("Config", "AreaWidth" + IntToStr(i), m_nScreenWidth);
		if (m_nSubAreaWidth[i] < 8) m_nSubAreaWidth[i] = 8;
		m_nSubAreaHeight[i] = sysini->ReadInteger("Config", "AreaHeight" + IntToStr(i), m_nScreenHeight);
		if (m_nSubAreaHeight[i] < 8) m_nSubAreaHeight[i] = 8;
	}
	for (int i = 0; i < MAX_LIGHT_CONTROL_COUNT; i++)
	{
		m_nLightControlStartHour[i] = sysini->ReadInteger("Config", "LightStartHour" + IntToStr(i), (i == 0) ? 0 : 23);
		m_nLightControlStartMinute[i] = sysini->ReadInteger("Config", "LightStartMin" + IntToStr(i), (i == 0) ? 0 : 59);
		m_nLightControlValue[i] = sysini->ReadInteger("Config", "LightValue" + IntToStr(i), 7);
	}

	m_nMaxActCount = sysini->ReadInteger("Config", "MaxActCount", 127);
	m_bActSupportSync = sysini->ReadBool("Config", "SyncPlay", false);
	m_nVoiceOutputAreaIndex = sysini->ReadInteger("Config", "VoiceArea", 0);

	if (!m_bOldInifileExist)
	{
		m_sCardType = sysini->ReadString("System", "CardType", "GS7020B");

		TCustomIniFile* ini = new TRegistryIniFile("Software\\Yuepoch");
		m_sRootName = ini->ReadString("LedPanelManV3", "RootName", "所有显示屏");
		delete ini;
	}

	for (int i = 0; i < 8; i++)
	{
		m_szSmsUser[i] = sysini->ReadString("SMS", "Users" + IntToStr(i), "");
		m_bEchoSms[i] = sysini->ReadBool("SMS", "Echo" + IntToStr(i), false);
	}
	m_nSmsActShowTime = sysini->ReadInteger("SMS", "ShowTime", 3000);
	m_nSmsActEnterStyle = sysini->ReadInteger("SMS", "EnterStyle", ENTER_STYLE_DIRECT);
	m_nSmsActEnterSpeed = sysini->ReadInteger("SMS", "EnterSpeed", 16);
	m_nSmsActExitStyle = sysini->ReadInteger("SMS", "ExitStyle", EXIT_STYLE_NULL);
	m_nSmsActExitSpeed = sysini->ReadInteger("SMS", "ExitSpeed", 16);
	m_nSmsActRepeatCount = sysini->ReadInteger("SMS", "RepearCount", 1);
	m_nSmsActFontSize = sysini->ReadInteger("SMS", "FontSize", 16);

	UnicodeString sTemp = sysini->ReadString("System", "ActsScope", "");
	wchar_t* ws = sTemp.c_str();
	int nLen = wcslen(ws);
	for (int i = 0; i < MAX_ACT_COUNT; i++)
	{
		m_bActSuitForAllCard[i] = true;
#ifndef AUTO_SIZE_TEXT
		if (nLen > i)
		{
			if (ws[i] == '1')
				m_bActSuitForAllCard[i] = false;
		}
#endif
	}

	delete sysini;
}
//---------------------------------------------------------------------------

CSysConfig::~CSysConfig()
{
}
//---------------------------------------------------------------------------

void CSysConfig::SaveConfig()
{
	TCustomIniFile* sysini = new TRegistryIniFile(REG_ROOT_NAME + UnicodeString("\\") + m_sRootName);

	sysini->WriteInteger("Config", "CommMethod", m_nCommMethod);
	sysini->WriteString("Config", "ComPort", m_sComPort);
	sysini->WriteInteger("Config", "BaudRate", m_nBaudRate);
	sysini->WriteInteger("Config", "Timeout_SerialPort", m_nTimeout_SerialPort);
	sysini->WriteInteger("Config", "Retry_SerialPort", m_nRetry_SerialPort);
	sysini->WriteString("Config", "Server", m_sServer);
	sysini->WriteInteger("Config", "SocketPort", m_nSocketPort);
	sysini->WriteInteger("Config", "Timeout_Network", m_nTimeout_Network);
	sysini->WriteInteger("Config", "Retry_Network", m_nRetry_Network);
	sysini->WriteString("Config", "UserName", m_sUserName);
	sysini->WriteString("Config", "Password", m_sPassword);
	sysini->WriteString("System", "SystemPassword", m_sSystemPassword);
	sysini->WriteInteger("System", "Language", m_nLanguage);
	sysini->WriteInteger("System", "AutoWriteClockInterval", m_nAutoClockInterval);
	sysini->WriteInteger("System", "AutoWriteClockMinute", m_nAutoClockMinute);
	sysini->WriteInteger("System", "AutoReTxInterval", m_nAutoReTxActInterval);
	sysini->WriteBool("System", "AutoReTxOnlyLast", m_bAutoReTx_OnlyLast);

	sysini->WriteInteger("Config", "CardBaudRate", m_nCardBaudRate);

	sysini->WriteInteger("Config", "SocketPort_Card", m_nSocketPort_Card);
	sysini->WriteInteger("Config", "ScreenWidth", m_nScreenWidth);
	sysini->WriteInteger("Config", "ScreenHeight", m_nScreenHeight);
	sysini->WriteInteger("Config", "ScanMode", m_ulScanMode);
	sysini->WriteBool("Config", "NegData", m_bNegData);
	sysini->WriteBool("Config", "NegOE", m_bNegOE);
	sysini->WriteBool("Config", "ByteReverse", m_bByteReverse);
	sysini->WriteBool("Config", "DoubleColor", m_bDoubleColor);
	sysini->WriteBool("Config", "FullColor", m_bFullColor);
	sysini->WriteBool("Config", "ActSecond", m_bActSecond);
	sysini->WriteBool("Config", "SupportRepeat", m_bActSupportRepeat);
	sysini->WriteBool("Config", "SupportInsert", m_bSupportInsertPlay);
	sysini->WriteBool("Config", "SupportWarning", m_bSupportWarning);
	sysini->WriteBool("Config", "TimeHint", m_bSupportCycleTimeHint);
	sysini->WriteString("Config", "APN", m_sAPN);
	sysini->WriteString("Config", "DataCenter", m_sDataCenter);
	sysini->WriteBool("Config", "SupportLedSignCheck", m_bSupportLedSignCheck);
	sysini->WriteBool("Config", "TCP", m_bTCP);
	sysini->WriteBool("Config", "LoginEcho", m_bLoginEcho);

	sysini->WriteString("Config", "GPS_Server", m_sGpsServer);
	sysini->WriteInteger("Config", "GPS_SocketPort", m_nGpsSocketPort);
	sysini->WriteInteger("Config", "GPS_ReportInterval", m_nGpsReportInterval);
	sysini->WriteString("Config", "GPS_Id", m_sGpsId);

	sysini->WriteInteger("Config", "SubAreaCount", m_nSubAreaCount);
	for (int i = 0; i < MAX_AREA_COUNT; i++)
	{
		sysini->WriteInteger("Config", "AreaLeft" + IntToStr(i), m_nSubAreaLeft[i]);
		sysini->WriteInteger("Config", "AreaTop" + IntToStr(i), m_nSubAreaTop[i]);
		sysini->WriteInteger("Config", "AreaWidth" + IntToStr(i), m_nSubAreaWidth[i]);
		sysini->WriteInteger("Config", "AreaHeight" + IntToStr(i), m_nSubAreaHeight[i]);
	}
	for (int i = 0; i < MAX_LIGHT_CONTROL_COUNT; i++)
	{
		sysini->WriteInteger("Config", "LightStartHour" + IntToStr(i), m_nLightControlStartHour[i]);
		sysini->WriteInteger("Config", "LightStartMin" + IntToStr(i), m_nLightControlStartMinute[i]);
		sysini->WriteInteger("Config", "LightValue" + IntToStr(i), m_nLightControlValue[i]);
	}
	sysini->WriteInteger("Config", "MaxActCount", m_nMaxActCount);
	sysini->WriteBool("Config", "SyncPlay", m_bActSupportSync);
	sysini->WriteInteger("Config", "VoiceArea", m_nVoiceOutputAreaIndex);

	MakeCardTypeName();
	sysini->WriteString("System", "CardType", m_sCardType);

	for (int i = 0; i < 8; i++)
	{
		sysini->WriteString("SMS", "Users" + IntToStr(i), m_szSmsUser[i]);
		sysini->WriteBool("SMS",  "Echo" + IntToStr(i), m_bEchoSms[i]);
	}
	sysini->WriteInteger("SMS", "ShowTime", m_nSmsActShowTime);
	sysini->WriteInteger("SMS", "EnterStyle", m_nSmsActEnterStyle);
	sysini->WriteInteger("SMS", "EnterSpeed", m_nSmsActEnterSpeed);
	sysini->WriteInteger("SMS", "ExitStyle", m_nSmsActExitStyle);
	sysini->WriteInteger("SMS", "ExitSpeed", m_nSmsActExitSpeed);
	sysini->WriteInteger("SMS", "RepearCount", m_nSmsActRepeatCount);
	sysini->WriteInteger("SMS", "FontSize", m_nSmsActFontSize);

	UnicodeString sTemp = "";
	for (int i = 0; i < MAX_ACT_COUNT; i++)
	{
		if (m_bActSuitForAllCard[i])
			sTemp += "0";
		else
			sTemp += "1";
	}
	sysini->WriteString("System", "ActsScope", sTemp);

	delete sysini;

	sysini = new TRegistryIniFile("Software\\Yuepoch");
	sysini->WriteString("LedPanelManV3", "RootName", m_sRootName);
	delete sysini;
}
//---------------------------------------------------------------------------

void CSysConfig::SaveSystemPassword(UnicodeString sSystemPassword)
{
	m_sSystemPassword = sSystemPassword;
	SaveConfig();
}
//---------------------------------------------------------------------------

void CUtils::GetRGBColor(WORD usColor, WORD usActive, DWORD* pdwFColor, DWORD* pdwBColor)
{
	switch (usColor & (COLOR_WHITE | BACKGROUND_COLOR_WHITE))
	{
	case COLOR_BLACK | BACKGROUND_COLOR_BLACK:
		usColor = COLOR_RED;
		break;
	case COLOR_RED | BACKGROUND_COLOR_RED:
		usColor = COLOR_RED;
		break;
	case COLOR_GREEN | BACKGROUND_COLOR_GREEN:
		usColor = COLOR_GREEN;
		break;
	case COLOR_BLUE | BACKGROUND_COLOR_BLUE:
		usColor = COLOR_BLUE;
		break;
	case COLOR_YELLOW | BACKGROUND_COLOR_YELLOW:
		usColor = COLOR_YELLOW;
		break;
	case COLOR_PINK | BACKGROUND_COLOR_PINK:
		usColor = COLOR_PINK;
		break;
	case COLOR_AQUA | BACKGROUND_COLOR_AQUA:
		usColor = COLOR_AQUA;
		break;
	case COLOR_WHITE | BACKGROUND_COLOR_WHITE:
		usColor = COLOR_WHITE;
		break;
	}

	switch (usColor & COLOR_WHITE)
	{
	case COLOR_BLACK:
		*pdwFColor = (usActive == '$') ? RGB(0, 0, 0) : RGB(0xC0, 0xC0, 0xC0);
		break;
	case COLOR_RED:
		*pdwFColor = (usActive == '$') ? RGB(255, 0, 0) : RGB(127, 0, 0);
		break;
	case COLOR_GREEN:
		*pdwFColor = (usActive == '$') ? RGB(0, 255, 0) : RGB(0, 127, 0);
		break;
	case COLOR_BLUE:
		*pdwFColor = (usActive == '$') ? RGB(0, 0, 255) : RGB(0, 0, 127);
		break;
	case COLOR_YELLOW:
		*pdwFColor = (usActive == '$') ? RGB(255, 255, 0) : RGB(127, 127, 0);
		break;
	case COLOR_PINK:
		*pdwFColor = (usActive == '$') ? RGB(255, 0, 255) : RGB(127, 0, 127);
		break;
	case COLOR_AQUA:
		*pdwFColor = (usActive == '$') ? RGB(0, 255, 255) : RGB(0, 127, 127);
		break;
	case COLOR_WHITE:
		*pdwFColor = (usActive == '$') ? RGB(255, 255, 255) : RGB(127, 127, 127);
		break;
	}

	switch (usColor & BACKGROUND_COLOR_WHITE)
	{
	case BACKGROUND_COLOR_BLACK:
		*pdwBColor = RGB(0, 0, 0);
		break;
	case BACKGROUND_COLOR_RED:
		*pdwBColor = RGB(255, 0, 0);
		break;
	case BACKGROUND_COLOR_GREEN:
		*pdwBColor = RGB(0, 255, 0);
		break;
	case BACKGROUND_COLOR_BLUE:
		*pdwBColor = RGB(0, 0, 255);
		break;
	case BACKGROUND_COLOR_YELLOW:
		*pdwBColor = RGB(255, 255, 0);
		break;
	case BACKGROUND_COLOR_PINK:
		*pdwBColor = RGB(255, 0, 255);
		break;
	case BACKGROUND_COLOR_AQUA:
		*pdwBColor = RGB(0, 255, 255);
		break;
	case BACKGROUND_COLOR_WHITE:
		*pdwBColor = RGB(255, 255, 255);
		break;
	}
}

DWORD CUtils::GetBitmapColor(DWORD dwColor)
{
	DWORD dw = (dwColor & 0xFF) << 16;

	dw += (dwColor & 0xFF00);
	dw += ((dwColor & 0xFF0000) >> 16);

	return dw;
}

void CSysConfig::SaveAct(CDtu* dtu, int nIndex)
{
	dtu = GetActDtu(dtu, nIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nIndex + 1);

	ini->EraseSection(wsSection);
	if (m_sActDesc.IsEmpty() &&  m_sActText.IsEmpty() && m_picAct == NULL)
	{
		delete ini;
		return;
	}

	ini->WriteString(wsSection, "ActDesc", m_sActDesc);
	ini->WriteInteger(wsSection, "ActArea", m_nActArea);
	ini->WriteInteger(wsSection, "EnterStyle", m_nActEnterStyle);
	ini->WriteInteger(wsSection, "EnterSpeed", m_nActEnterSpeed);
	ini->WriteInteger(wsSection, "ExitStyle", m_nActExitStyle);
	ini->WriteInteger(wsSection, "ExitSpeed", m_nActExitSpeed);
	ini->WriteInteger(wsSection, "ShowTime", m_nActShowTime);
	ini->WriteInteger(wsSection, "ShutterWidth", m_nActShutterWidth);
	ini->WriteInteger(wsSection, "FlashLightTime", m_nActFlashLightTime);
	ini->WriteInteger(wsSection, "FlashDarkTime", m_nActFlashDarkTime);
	ini->WriteInteger(wsSection, "StartYear", m_nActStartYear);
	ini->WriteInteger(wsSection, "StartMonth", m_nActStartMonth);
	ini->WriteInteger(wsSection, "StartDay", m_nActStartDay);
	ini->WriteInteger(wsSection, "StartHour", m_nActStartHour);
	ini->WriteInteger(wsSection, "StartMin", m_nActStartMin);
	ini->WriteInteger(wsSection, "EndYear", m_nActEndYear);
	ini->WriteInteger(wsSection, "EndMonth", m_nActEndMonth);
	ini->WriteInteger(wsSection, "EndDay", m_nActEndDay);
	ini->WriteInteger(wsSection, "EndHour", m_nActEndHour);
	ini->WriteInteger(wsSection, "EndMin", m_nActEndMin);
	ini->WriteInteger(wsSection, "StartWeek", m_nActStartWeek);
	ini->WriteInteger(wsSection, "EndWeek", m_nActEndWeek);
	ini->WriteInteger(wsSection, "RepeatCount", m_nActRepeatCount);
	for (int i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
	{
		ini->WriteInteger(wsSection, "StartMinuteInday" + UnicodeString(i), m_nActStartMinuteInDay[i]);
		ini->WriteInteger(wsSection, "EndMinuteInday" + UnicodeString(i), m_nActEndMinuteInDay[i]);
		ini->WriteInteger(wsSection, "StartSecondInday" + UnicodeString(i), m_nActStartSecondInDay[i]);
		ini->WriteInteger(wsSection, "EndSecondInday" + UnicodeString(i), m_nActEndSecondInDay[i]);
	}
	ini->WriteInteger(wsSection, "RefAct", m_nActRefIndex);

	ini->WriteString(wsSection, "ActText", PrepareTextToWrite(m_sActText));
	ini->WriteString(wsSection, "Active", PrepareTextToWrite(m_sActActiveText));
	ini->WriteString(wsSection, "TextColor", PrepareTextToWrite(m_sActTextColor));
	ini->WriteString(wsSection, "FontName", m_sActFontName);
	ini->WriteBool(wsSection, "IsDotFont", m_bDotFont);
	ini->WriteInteger(wsSection, "DotFontSize", m_nDotFontSize);
	ini->WriteInteger(wsSection, "FontSize", m_nActFontSize);
	ini->WriteInteger(wsSection, "FontStyle", m_nActFontStyle);
	ini->WriteInteger(wsSection, "HorzAlign", m_nActHorzAlignment);
	ini->WriteInteger(wsSection, "VertAlign", m_nActVertAlignment);
	ini->WriteInteger(wsSection, "DownCounterBaseYear", m_nActDownCounterBaseYear);
	ini->WriteInteger(wsSection, "DownCounterBaseMonth", m_nActDownCounterBaseMonth);
	ini->WriteInteger(wsSection, "DownCounterBaseDay", m_nActDownCounterBaseDay);

	ini->WriteInteger(wsSection, "ImagePosX", m_nActImagePosX);
	ini->WriteInteger(wsSection, "ImagePosY", m_nActImagePosY);
	if (m_picAct == NULL)
	{
		ini->WriteInteger(wsSection, "ImageWidth", 0);
		ini->WriteInteger(wsSection, "ImageHeight", 0);
	}
	else
	{
		ini->WriteInteger(wsSection, "ImageWidth", m_picAct->Bitmap->Width);
		ini->WriteInteger(wsSection, "ImageHeight", m_picAct->Bitmap->Height);
		TMemoryStream* stream = new TMemoryStream;
		m_picAct->Bitmap->SaveToStream(stream);
		stream->Position = 0;
		ini->WriteBinaryStream(wsSection, "ImageVol", stream);
		delete stream;
	}

	delete ini;
}
//---------------------------------------------------------------------------

void CSysConfig::LoadAct(CDtu* dtu, int nIndex)
{
	dtu = GetActDtu(dtu, nIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nIndex + 1);

	SYSTEMTIME st;
	::GetLocalTime(&st);

	m_sActDesc = ini->ReadString(wsSection, "ActDesc", "");
	m_nActArea = ini->ReadInteger(wsSection, "ActArea", 0);
	if ((m_nActArea >= m_nSubAreaCount) || (m_nActArea < 0))
		m_nActArea = 0;
	m_nActEnterStyle = ini->ReadInteger(wsSection, "EnterStyle", ENTER_STYLE_DIRECT);
	m_nActEnterSpeed = ini->ReadInteger(wsSection, "EnterSpeed", 16);
	m_nActExitStyle = ini->ReadInteger(wsSection, "ExitStyle", EXIT_STYLE_NULL);
	m_nActExitSpeed = ini->ReadInteger(wsSection, "ExitSpeed", 16);
	m_nActShowTime = ini->ReadInteger(wsSection, "ShowTime", 3000);
	m_nActShutterWidth = ini->ReadInteger(wsSection, "ShutterWidth", 8);
	if (m_nActShutterWidth <= 0)
		m_nActShutterWidth = 8;
	m_nActFlashLightTime = ini->ReadInteger(wsSection, "FlashLightTime", 500);
	m_nActFlashDarkTime = ini->ReadInteger(wsSection, "FlashDarkTime", 500);
	m_nActStartYear = ini->ReadInteger(wsSection, "StartYear", st.wYear);
	m_nActStartMonth = ini->ReadInteger(wsSection, "StartMonth", st.wMonth);
	m_nActStartDay = ini->ReadInteger(wsSection, "StartDay", st.wDay);
	m_nActStartHour = ini->ReadInteger(wsSection, "StartHour", 0);
	m_nActStartMin = ini->ReadInteger(wsSection, "StartMin", 0);
	m_nActEndYear = ini->ReadInteger(wsSection, "EndYear", 2099);
	m_nActEndMonth = ini->ReadInteger(wsSection, "EndMonth", 12);
	m_nActEndDay = ini->ReadInteger(wsSection, "EndDay", 31);
	m_nActEndHour = ini->ReadInteger(wsSection, "EndHour", 23);
	m_nActEndMin = ini->ReadInteger(wsSection, "EndMin", 59);
	m_nActStartWeek = ini->ReadInteger(wsSection, "StartWeek", 0);
	m_nActEndWeek = ini->ReadInteger(wsSection, "EndWeek", 6);
	m_nActRepeatCount = ini->ReadInteger(wsSection, "RepeatCount", 1);
	for (int i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
	{
		if (i == 0)
		{
			m_nActStartMinuteInDay[i] = ini->ReadInteger(wsSection, "StartMinuteInday" + UnicodeString(i), 0);
			m_nActEndMinuteInDay[i] = ini->ReadInteger(wsSection, "EndMinuteInday" + UnicodeString(i), 60 * 24 - 1);
			m_nActStartSecondInDay[i] = ini->ReadInteger(wsSection, "StartSecondInday" + UnicodeString(i), 0);
			m_nActEndSecondInDay[i] = ini->ReadInteger(wsSection, "EndSecondInday" + UnicodeString(i), 59);
		}
		else
		{
			m_nActStartMinuteInDay[i] = ini->ReadInteger(wsSection, "StartMinuteInday" + UnicodeString(i), 0);
			m_nActEndMinuteInDay[i] = ini->ReadInteger(wsSection, "EndMinuteInday" + UnicodeString(i), 0);
			m_nActStartSecondInDay[i] = ini->ReadInteger(wsSection, "StartSecondInday" + UnicodeString(i), 0);
			m_nActEndSecondInDay[i] = ini->ReadInteger(wsSection, "EndSecondInday" + UnicodeString(i), 0);
		}
	}
	m_nActRefIndex = ini->ReadInteger(wsSection, "RefAct", nIndex);
	if (m_nActRefIndex > m_nMaxActCount || m_nActRefIndex < 0)
		m_nActRefIndex = nIndex;

	m_sActText = ProcessTextRead(ini->ReadString(wsSection, "ActText", ""));
	m_sActTextColor = ProcessTextRead(ini->ReadString(wsSection, "TextColor", ""));
	m_sActActiveText = ProcessTextRead(ini->ReadString(wsSection, "Active", ""));
	CheckActiveText();
	CheckTextColor();
	m_bDotFont = ini->ReadBool(wsSection, "IsDotFont", false);
	m_nDotFontSize = ini->ReadInteger(wsSection, "DotFontSize", 16);
	m_sActFontName = ini->ReadString(wsSection, "FontName", "宋体");
	m_nActFontSize = ini->ReadInteger(wsSection, "FontSize", 12);
	m_nActFontStyle = ini->ReadInteger(wsSection, "FontStyle", 0);
	m_nActHorzAlignment = ini->ReadInteger(wsSection, "HorzAlign", TEXT_ALIGN_LEFT);
	m_nActVertAlignment = ini->ReadInteger(wsSection, "VertAlign", 0);
	m_nActDownCounterBaseYear = ini->ReadInteger(wsSection, "DownCounterBaseYear", 2050);
	m_nActDownCounterBaseMonth = ini->ReadInteger(wsSection, "DownCounterBaseMonth", 1);
	m_nActDownCounterBaseDay = ini->ReadInteger(wsSection, "DownCounterBaseDay", 1);

	if (m_picAct != NULL)
	{
		delete m_picAct;
		m_picAct = NULL;
	}

	m_nActImagePosX = ini->ReadInteger(wsSection, "ImagePosX", 0);
	m_nActImagePosY = ini->ReadInteger(wsSection, "ImagePosY", 0);
	int nImageWidth	= ini->ReadInteger(wsSection, "ImageWidth", 0);
	int nImageHeight = ini->ReadInteger(wsSection, "ImageHeight", 0);

	if (nImageWidth > 4096 || nImageWidth <= 0 || nImageHeight > 4096 || nImageHeight <= 0)
	{
		delete ini;
		return;
	}

	if ((nImageWidth != 0) && (nImageHeight != 0))
	{
		TMemoryStream* stream = new TMemoryStream;
		ini->ReadBinaryStream(wsSection, "ImageVol", stream);

		m_picAct = new TPicture;
		m_picAct->Bitmap->Width = nImageWidth;
		m_picAct->Bitmap->Height = nImageHeight;
		m_picAct->Bitmap->Canvas->Brush->Color = clBlack;
		m_picAct->Bitmap->Canvas->FillRect(Rect(0, 0, nImageWidth, nImageHeight));
		m_picAct->Bitmap->LoadFromStream(stream);

		delete stream;
	}

	delete ini;
}
//---------------------------------------------------------------------------

void CSysConfig::CheckActiveText()
{
	wchar_t *ws, *wsActive;

	if (m_sActActiveText.Length() != m_sActText.Length())
	{
		m_sActActiveText = "";

		ws = m_sActText.c_str();
		while (ws && *ws)
		{
			if (*ws == '\r')
			   m_sActActiveText = m_sActActiveText + '\r';
			else if (*ws == '\n')
			   m_sActActiveText = m_sActActiveText + '\n';
			else
			   m_sActActiveText = m_sActActiveText + '$';

			ws++;
		}
		return;
	}

	ws = m_sActText.c_str();
	wsActive = m_sActActiveText.c_str();
	while (ws && *ws)
	{
		if (*ws == '\r')
			*wsActive = '\r';
		else if (*ws == '\n')
			*wsActive = '\n';
		else if (*wsActive == '\r')
			*wsActive = '1';
		ws++; wsActive++;
	}
}
//---------------------------------------------------------------------------

void CSysConfig::CheckTextColor()
{
	wchar_t *ws, *wsColor;

	if (m_sActTextColor.Length() != m_sActText.Length())
	{
		m_sActTextColor = "";

		ws = m_sActText.c_str();
		while (ws && *ws)
		{
			if (*ws == '\r')
			   m_sActTextColor = m_sActTextColor + '\r';
			else if (*ws == '\n')
			   m_sActTextColor = m_sActTextColor + '\n';
			else
			   m_sActTextColor = m_sActTextColor + 'A';

			ws++;
		}
		return;
	}

	ws = m_sActText.c_str();
	wsColor = m_sActTextColor.c_str();
	while (ws && *ws)
	{
		if (*ws == '\r')
			*wsColor = '\r';
		else if (*ws == '\n')
			*wsColor = '\n';
		else if (*wsColor < 0x40)
			*wsColor = (*wsColor & 3) | 0x40;
		ws++; wsColor++;
	}
}
//---------------------------------------------------------------------------

void CSysConfig::DeleteAct(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);
	ini->EraseSection(wsSection);
	delete ini;
}
//---------------------------------------------------------------------------

UnicodeString CSysConfig::GetActDesc(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	UnicodeString s = ini->ReadString(wsSection, "ActDesc", "");
	delete ini;
	return s;
}
//---------------------------------------------------------------------------

UnicodeString CSysConfig::GetActText(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	UnicodeString s = ini->ReadString(wsSection, "ActText", "");
	delete ini;
	return s;
}
//---------------------------------------------------------------------------

int CSysConfig::GetActRefIndex(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);
	int n = ini->ReadInteger(wsSection, "RefAct", 0);
	delete ini;

	return n;
}
//---------------------------------------------------------------------------

int CSysConfig::GetActRepeatCount(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);
	int n = ini->ReadInteger(wsSection, "RepeatCount", 0);
	delete ini;

	return n;
}
//---------------------------------------------------------------------------

bool CSysConfig::CheckActHasPicture(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	int nImageWidth	= ini->ReadInteger(wsSection, "ImageWidth", 0);
	int nImageHeight = ini->ReadInteger(wsSection, "ImageHeight", 0);
	delete ini;

	if (nImageWidth > 4096 || nImageWidth <= 0 || nImageHeight > 4096 || nImageHeight <= 0)
		return false;

	if ((nImageWidth != 0) && (nImageHeight != 0))
		return true;

	return false;
}
//---------------------------------------------------------------------------

int CSysConfig::GetActArea(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	int nActArea = ini->ReadInteger(wsSection, "ActArea", 0);
	if ((nActArea >= m_nSubAreaCount) || (nActArea < 0))
		nActArea = 0;
	delete ini;
	return nActArea;
}
//---------------------------------------------------------------------------

int CSysConfig::GetActEnterStyle(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	int nActEnterStyle = ini->ReadInteger(wsSection, "EnterStyle", 16);
	delete ini;
	return nActEnterStyle;
}
//---------------------------------------------------------------------------

int CSysConfig::GetActEnterSpeed(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	int nActEnterSpeed = ini->ReadInteger(wsSection, "EnterSpeed", 16);
	delete ini;
	return nActEnterSpeed;
}
//---------------------------------------------------------------------------

int CSysConfig::GetActExitStyle(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	int nActExitStyle = ini->ReadInteger(wsSection, "ExitStyle", EXIT_STYLE_NULL);
	delete ini;
	return nActExitStyle;
}
//---------------------------------------------------------------------------

int CSysConfig::GetActExitSpeed(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	int nActExitSpeed = ini->ReadInteger(wsSection, "ExitSpeed", EXIT_STYLE_NULL);
	delete ini;
	return nActExitSpeed;
}
//---------------------------------------------------------------------------

int CSysConfig::GetActShowTime(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	int nActShowTime = ini->ReadInteger(wsSection, "ShowTime", 3000);
	delete ini;
	return nActShowTime;
}
//---------------------------------------------------------------------------

bool CSysConfig::GetDotFont(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	bool bDotFont = ini->ReadBool(wsSection, "IsDotFont", false);
	delete ini;
	return bDotFont;
}
//---------------------------------------------------------------------------

int CSysConfig::GetDotFontSize(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	int nDotFontSize = ini->ReadInteger(wsSection, "DotFontSize", 16);
	delete ini;
	return nDotFontSize;
}
//---------------------------------------------------------------------------

bool CSysConfig::TestIfHasPicture(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	int nImageWidth	= ini->ReadInteger(wsSection, "ImageWidth", 0);
	int nImageHeight = ini->ReadInteger(wsSection, "ImageHeight", 0);

	if (nImageWidth > 4096 || nImageWidth <= 0 || nImageHeight > 4096 || nImageHeight <= 0)
	{
		delete ini;
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------

int CSysConfig::GetActShutterWidth(CDtu* dtu, int nActIndex)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	int nActShutterWidth = ini->ReadInteger(wsSection, "ShutterWidth", 8);
	if (nActShutterWidth <= 0)
		nActShutterWidth = 8;
	delete ini;
	return nActShutterWidth;
}
//---------------------------------------------------------------------------

void CSysConfig::SaveActToFile(UnicodeString sFileName)
{
	TIniFile* ini = new TIniFile(sFileName);
	UnicodeString wsSection = "ACT";

	ini->WriteString(wsSection, "ActDesc", m_sActDesc);
	ini->WriteInteger(wsSection, "ActArea", m_nActArea);
	ini->WriteInteger(wsSection, "EnterStyle", m_nActEnterStyle);
	ini->WriteInteger(wsSection, "EnterSpeed", m_nActEnterSpeed);
	ini->WriteInteger(wsSection, "ExitStyle", m_nActExitStyle);
	ini->WriteInteger(wsSection, "ExitSpeed", m_nActExitSpeed);
	ini->WriteInteger(wsSection, "ShowTime", m_nActShowTime);
	ini->WriteInteger(wsSection, "ShutterWidth", m_nActShutterWidth);
	ini->WriteInteger(wsSection, "FlashLightTime", m_nActFlashLightTime);
	ini->WriteInteger(wsSection, "FlashDarkTime", m_nActFlashDarkTime);
	ini->WriteInteger(wsSection, "StartYear", m_nActStartYear);
	ini->WriteInteger(wsSection, "StartMonth", m_nActStartMonth);
	ini->WriteInteger(wsSection, "StartDay", m_nActStartDay);
	ini->WriteInteger(wsSection, "StartHour", m_nActStartHour);
	ini->WriteInteger(wsSection, "StartMin", m_nActStartMin);
	ini->WriteInteger(wsSection, "EndYear", m_nActEndYear);
	ini->WriteInteger(wsSection, "EndMonth", m_nActEndMonth);
	ini->WriteInteger(wsSection, "EndDay", m_nActEndDay);
	ini->WriteInteger(wsSection, "EndHour", m_nActEndHour);
	ini->WriteInteger(wsSection, "EndMin", m_nActEndMin);
	ini->WriteInteger(wsSection, "StartWeek", m_nActStartWeek);
	ini->WriteInteger(wsSection, "EndWeek", m_nActEndWeek);
	ini->WriteInteger(wsSection, "RepeatCount", m_nActRepeatCount);
	for (int i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
	{
		ini->WriteInteger(wsSection, "StartMinuteInday" + UnicodeString(i), m_nActStartMinuteInDay[i]);
		ini->WriteInteger(wsSection, "EndMinuteInday" + UnicodeString(i), m_nActEndMinuteInDay[i]);
		ini->WriteInteger(wsSection, "StartSecondInday" + UnicodeString(i), m_nActStartSecondInDay[i]);
		ini->WriteInteger(wsSection, "EndSecondInday" + UnicodeString(i), m_nActEndSecondInDay[i]);
	}
	ini->WriteInteger(wsSection, "RefAct", m_nActRefIndex);

	ini->WriteString(wsSection, "ActText", PrepareTextToWrite(m_sActText));
	ini->WriteString(wsSection, "Active", PrepareTextToWrite(m_sActActiveText));
	ini->WriteString(wsSection, "TextColor", PrepareTextToWrite(m_sActTextColor));
	ini->WriteString(wsSection, "FontName", m_sActFontName);
	ini->WriteInteger(wsSection, "FontSize", m_nActFontSize);
	ini->WriteBool(wsSection, "FontStyle", m_nActFontStyle);
	ini->WriteInteger(wsSection, "HorzAlign", m_nActHorzAlignment);
	ini->WriteInteger(wsSection, "VertAlign", m_nActVertAlignment);
	ini->WriteInteger(wsSection, "DownCounterBaseYear", m_nActDownCounterBaseYear);
	ini->WriteInteger(wsSection, "DownCounterBaseMonth", m_nActDownCounterBaseMonth);
	ini->WriteInteger(wsSection, "DownCounterBaseDay", m_nActDownCounterBaseDay);

	ini->WriteInteger(wsSection, "ImagePosX", m_nActImagePosX);
	ini->WriteInteger(wsSection, "ImagePosY", m_nActImagePosY);
	if (m_picAct == NULL)
	{
		ini->WriteInteger(wsSection, "ImageWidth", 0);
		ini->WriteInteger(wsSection, "ImageHeight", 0);
	}
	else
	{
		ini->WriteInteger(wsSection, "ImageWidth", m_picAct->Bitmap->Width);
		ini->WriteInteger(wsSection, "ImageHeight", m_picAct->Bitmap->Height);
		TMemoryStream* stream = new TMemoryStream;
		m_picAct->Bitmap->SaveToStream(stream);
		stream->Position = 0;
		ini->WriteBinaryStream(wsSection, "ImageVol", stream);
		delete stream;
	}

	delete ini;
}
//---------------------------------------------------------------------------

void CSysConfig::OpenActFile(UnicodeString sFileName)
{
	TIniFile* ini = new TIniFile(sFileName);
	UnicodeString wsSection = "ACT";

	SYSTEMTIME st;
	::GetLocalTime(&st);

	m_sActDesc = ini->ReadString(wsSection, "ActDesc", "");
	m_nActArea = ini->ReadInteger(wsSection, "ActArea", 0);
	if ((m_nActArea >= m_nSubAreaCount) || (m_nActArea < 0))
		m_nActArea = 0;
	m_nActEnterStyle = ini->ReadInteger(wsSection, "EnterStyle", ENTER_STYLE_DIRECT);
	m_nActEnterSpeed = ini->ReadInteger(wsSection, "EnterSpeed", 16);
	m_nActExitStyle = ini->ReadInteger(wsSection, "ExitStyle", EXIT_STYLE_NULL);
	m_nActExitSpeed = ini->ReadInteger(wsSection, "ExitSpeed", 16);
	m_nActShowTime = ini->ReadInteger(wsSection, "ShowTime", 3000);
	m_nActShutterWidth = ini->ReadInteger(wsSection, "ShutterWidth", 8);
	if (m_nActShutterWidth <= 0)
		m_nActShutterWidth = 8;
	m_nActFlashLightTime = ini->ReadInteger(wsSection, "FlashLightTime", 500);
	m_nActFlashDarkTime = ini->ReadInteger(wsSection, "FlashDarkTime", 500);
	m_nActStartYear = ini->ReadInteger(wsSection, "StartYear", st.wYear);
	m_nActStartMonth = ini->ReadInteger(wsSection, "StartMonth", st.wMonth);
	m_nActStartDay = ini->ReadInteger(wsSection, "StartDay", st.wDay);
	m_nActStartHour = ini->ReadInteger(wsSection, "StartHour", 0);
	m_nActStartMin = ini->ReadInteger(wsSection, "StartMin", 0);
	m_nActEndYear = ini->ReadInteger(wsSection, "EndYear", 2099);
	m_nActEndMonth = ini->ReadInteger(wsSection, "EndMonth", 12);
	m_nActEndDay = ini->ReadInteger(wsSection, "EndDay", 31);
	m_nActEndHour = ini->ReadInteger(wsSection, "EndHour", 23);
	m_nActEndMin = ini->ReadInteger(wsSection, "EndMin", 59);
	m_nActStartWeek = ini->ReadInteger(wsSection, "StartWeek", 0);
	m_nActEndWeek = ini->ReadInteger(wsSection, "EndWeek", 6);
	m_nActRepeatCount = ini->ReadInteger(wsSection, "RepeatCount", 1);
	for (int i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
	{
		if (i == 0)
		{
			m_nActStartMinuteInDay[i] = ini->ReadInteger(wsSection, "StartMinuteInday" + UnicodeString(i), 0);
			m_nActEndMinuteInDay[i] = ini->ReadInteger(wsSection, "EndMinuteInday" + UnicodeString(i), 60 * 24 - 1);
			m_nActStartSecondInDay[i] = ini->ReadInteger(wsSection, "StartSecondInday" + UnicodeString(i), 0);
			m_nActEndSecondInDay[i] = ini->ReadInteger(wsSection, "EndSecondInday" + UnicodeString(i), 59);
		}
		else
		{
			m_nActStartMinuteInDay[i] = ini->ReadInteger(wsSection, "StartMinuteInday" + UnicodeString(i), 0);
			m_nActEndMinuteInDay[i] = ini->ReadInteger(wsSection, "EndMinuteInday" + UnicodeString(i), 0);
			m_nActStartSecondInDay[i] = ini->ReadInteger(wsSection, "StartSecondInday" + UnicodeString(i), 0);
			m_nActEndSecondInDay[i] = ini->ReadInteger(wsSection, "EndSecondInday" + UnicodeString(i), 0);
		}
	}
	m_nActRefIndex = ini->ReadInteger(wsSection, "RefAct", -1);

	m_sActText = ProcessTextRead(ini->ReadString(wsSection, "ActText", ""));
	m_sActTextColor = ProcessTextRead(ini->ReadString(wsSection, "TextColor", ""));
	m_sActActiveText = ProcessTextRead(ini->ReadString(wsSection, "Active", ""));
	CheckActiveText();
	CheckTextColor();
	m_sActFontName = ini->ReadString(wsSection, "FontName", "宋体");
	m_nActFontSize = ini->ReadInteger(wsSection, "FontSize", 12);
	m_nActFontStyle = ini->ReadInteger(wsSection, "FontStyle", 0);
	m_nActHorzAlignment = ini->ReadInteger(wsSection, "HorzAlign", TEXT_ALIGN_LEFT);
	m_nActVertAlignment = ini->ReadInteger(wsSection, "VertAlign", 0);
	m_nActDownCounterBaseYear = ini->ReadInteger(wsSection, "DownCounterBaseYear", 2050);
	m_nActDownCounterBaseMonth = ini->ReadInteger(wsSection, "DownCounterBaseMonth", 1);
	m_nActDownCounterBaseDay = ini->ReadInteger(wsSection, "DownCounterBaseDay", 1);

	if (m_picAct != NULL)
	{
		delete m_picAct;
		m_picAct = NULL;
	}

	m_nActImagePosX = ini->ReadInteger(wsSection, "ImagePosX", 0);
	m_nActImagePosY = ini->ReadInteger(wsSection, "ImagePosY", 0);
	int nImageWidth	= ini->ReadInteger(wsSection, "ImageWidth", 0);
	int nImageHeight = ini->ReadInteger(wsSection, "ImageHeight", 0);

	if (nImageWidth > 4096 || nImageWidth <= 0 || nImageHeight > 4096 || nImageHeight <= 0)
	{
		delete ini;
		return;
	}

	if ((nImageWidth != 0) && (nImageHeight != 0))
	{
		TMemoryStream* stream = new TMemoryStream;
		ini->ReadBinaryStream(wsSection, "ImageVol", stream);

		m_picAct = new TPicture;
		m_picAct->Bitmap->Width = nImageWidth;
		m_picAct->Bitmap->Height = nImageHeight;
		m_picAct->Bitmap->Canvas->Brush->Color = clBlack;
		m_picAct->Bitmap->Canvas->FillRect(Rect(0, 0, nImageWidth, nImageHeight));
		m_picAct->Bitmap->LoadFromStream(stream);

		delete stream;
	}

	delete ini;
}
//---------------------------------------------------------------------------

void CSysConfig::LoadActVol_V3(TIniFile* sysini, int nIndex)
{
	SYSTEMTIME st;
	::GetLocalTime(&st);

	m_sActDesc = sysini->ReadString("ACTS", "ActDesc" + AnsiString(nIndex + 1), "");
	m_nActArea = sysini->ReadInteger("ACTS", "ActArea" + AnsiString(nIndex + 1), 0);
	if (m_nActArea >= m_nSubAreaCount)
		m_nActArea = 0;
	m_nActEnterStyle = sysini->ReadInteger("ACTS", "EnterStyle" + AnsiString(nIndex + 1), ENTER_STYLE_DIRECT);
	m_nActEnterSpeed = sysini->ReadInteger("ACTS", "EnterSpeed" + AnsiString(nIndex + 1), 16);
	m_nActExitStyle = sysini->ReadInteger("ACTS", "ExitStyle" + AnsiString(nIndex + 1), EXIT_STYLE_NULL);
	m_nActExitSpeed = sysini->ReadInteger("ACTS", "ExitSpeed" + AnsiString(nIndex + 1), 16);
	m_nActShowTime = sysini->ReadInteger("ACTS", "ShowTime" + AnsiString(nIndex + 1), 3000);
	m_nActRepeatCount = sysini->ReadInteger("ACTS", "RepeatCount" + AnsiString(nIndex + 1), 1);
	m_nActShutterWidth = sysini->ReadInteger("ACTS", "ShutterWidth" + AnsiString(nIndex + 1), 8);
	if (m_nActShutterWidth <= 0)
		m_nActShutterWidth = 8;
	m_nActFlashLightTime = sysini->ReadInteger("ACTS", "FlashLightTime" + AnsiString(nIndex + 1), 500);
	m_nActFlashDarkTime = sysini->ReadInteger("ACTS", "FlashDarkTime" + AnsiString(nIndex + 1), 500);
	m_nActStartYear = sysini->ReadInteger("ACTS", "StartYear" + AnsiString(nIndex + 1), st.wYear);
	m_nActStartMonth = sysini->ReadInteger("ACTS", "StartMonth" + AnsiString(nIndex + 1), st.wMonth);
	m_nActStartDay = sysini->ReadInteger("ACTS", "StartDay" + AnsiString(nIndex + 1), st.wDay);
	m_nActStartHour = sysini->ReadInteger("ACTS", "StartHour" + AnsiString(nIndex + 1), 0);
	m_nActStartMin = sysini->ReadInteger("ACTS", "StartMin" + AnsiString(nIndex + 1), 0);
	m_nActEndYear = sysini->ReadInteger("ACTS", "EndYear" + AnsiString(nIndex + 1), 2099);
	m_nActEndMonth = sysini->ReadInteger("ACTS", "EndMonth" + AnsiString(nIndex + 1), 12);
	m_nActEndDay = sysini->ReadInteger("ACTS", "EndDay" + AnsiString(nIndex + 1), 31);
	m_nActEndHour = sysini->ReadInteger("ACTS", "EndHour" + AnsiString(nIndex + 1), 23);
	m_nActEndMin = sysini->ReadInteger("ACTS", "EndMin" + AnsiString(nIndex + 1), 59);
	for (int i = 0; i < ACT_TIME_PERIOD_COUNT_24; i++)
	{
		if (i == 0)
		{
			m_nActStartMinuteInDay[i] = sysini->ReadInteger("ACTS", "StartMinuteInday" + UnicodeString(i) + AnsiString(nIndex + 1), 0);
			m_nActEndMinuteInDay[i] = sysini->ReadInteger("ACTS", "EndMinuteInday" + UnicodeString(i) + AnsiString(nIndex + 1), 60 * 24 - 1);
			m_nActStartSecondInDay[i] = sysini->ReadInteger("ACTS", "StartSecondInday" + UnicodeString(i) + AnsiString(nIndex + 1), 0);
			m_nActEndSecondInDay[i] = sysini->ReadInteger("ACTS", "EndSecondInday" + UnicodeString(i) + AnsiString(nIndex + 1), 59);
		}
		else
		{
			m_nActStartMinuteInDay[i] = sysini->ReadInteger("ACTS", "StartMinuteInday" + UnicodeString(i) + AnsiString(nIndex + 1), 0);
			m_nActEndMinuteInDay[i] = sysini->ReadInteger("ACTS", "EndMinuteInday" + UnicodeString(i) + AnsiString(nIndex + 1), 0);
			m_nActStartSecondInDay[i] = sysini->ReadInteger("ACTS", "StartSecondInday" + UnicodeString(i) + AnsiString(nIndex + 1), 0);
			m_nActEndSecondInDay[i] = sysini->ReadInteger("ACTS", "EndSecondInday" + UnicodeString(i) + AnsiString(nIndex + 1), 0);
		}
	}

	m_sActText = ProcessTextRead_V3(sysini->ReadString("ACTS", "Text" + AnsiString(nIndex + 1), ""));
	m_sActTextColor = ProcessTextRead_V3(sysini->ReadString("ACTS", "TextColor" + AnsiString(nIndex + 1), ""));
	m_sActActiveText = ProcessTextRead_V3(sysini->ReadString("ACTS", "Active" + AnsiString(nIndex + 1), ""));
	CheckActiveText();
	CheckTextColor();
	m_sActFontName = sysini->ReadString("ACTS", "FontName" + AnsiString(nIndex + 1), "宋体");
	m_nActFontSize = sysini->ReadInteger("ACTS", "FontSize" + AnsiString(nIndex + 1), 10);
	m_nActFontStyle = sysini->ReadInteger("ACTS", "FontStyle" + AnsiString(nIndex + 1), 0);
	m_nActHorzAlignment = sysini->ReadInteger("ACTS", "HorzAlign" + AnsiString(nIndex + 1), TEXT_ALIGN_LEFT);
	m_nActVertAlignment = sysini->ReadInteger("ACTS", "VertAlign" + AnsiString(nIndex + 1), 0);
	m_nActDownCounterBaseYear = sysini->ReadInteger("ACTS", "DownCounterBaseYear" + AnsiString(nIndex + 1), 2050);
	m_nActDownCounterBaseMonth = sysini->ReadInteger("ACTS", "DownCounterBaseMonth" + AnsiString(nIndex + 1), 1);
	m_nActDownCounterBaseDay = sysini->ReadInteger("ACTS", "DownCounterBaseDay" + AnsiString(nIndex + 1), 1);
}
//---------------------------------------------------------------------------

UnicodeString CSysConfig::ProcessTextRead_V3(UnicodeString s)
{
	UnicodeString r;
	wchar_t* p = s.c_str();
	bool bFirst = true;

	while (*p)
	{
		if (*p == '\\')
		{
			if (!bFirst)
				r += "\r\n";
		}
		else
			r += *p;
		p++;
		bFirst = false;
	}

	return r;
}
//---------------------------------------------------------------------------

UnicodeString CSysConfig::ProcessTextRead(UnicodeString s)
{
	UnicodeString r;
	wchar_t* p = s.c_str();
	bool bEx = false;

	while (*p)
	{
		if (!bEx)
		{
			if (*p == '\\')
				bEx = true;
			else
				r += *p;
		}
		else
		{
			if (*p  == 'r')
				r += '\r';
			else if (*p  == 'n')
				r += '\n';
			else if (*p == '\\')
				r += '\\';
			bEx = false;
		}

		p++;
	}

	return r;
}
//---------------------------------------------------------------------------

UnicodeString CSysConfig::PrepareTextToWrite(UnicodeString s)
{
	UnicodeString r;
	wchar_t* p = s.c_str();

	while (*p)
	{
		if (*p == '\\')
			r += "\\\\";
		else if (*p == '\r')
			r += "\\r";
		else if (*p == '\n')
			r += "\\n";
		else
			r += *p;
		p++;
	}

	return r;
}
//---------------------------------------------------------------------------

int CSysConfig::GetOneActVolMaxLen()
{
	if (m_nCardType == CARD_TYPE_GS2012A || m_nCardType == CARD_TYPE_WF2012B || m_nCardType == CARD_TYPE_GS2012B)
	{
		switch (m_nMaxActCount)
		{
		case 63:
			return 16384 * 2 * 4;
		case 31:
			return 16384 * 4 * 4;
		default:
			return 16384 * 4;
		}
	}
	else if (m_nCardType == CARD_TYPE_GS7020HFA)
	{
		switch (m_nMaxActCount)
		{
		case 127:
			return 8192 * 1024 / 128;
		case 63:
			return 8192 * 1024 / 64;
		case 31:
			return 8192 * 1024 / 32;
		default:
			return 8192 * 1024 / 256;
		}
	}
	else if (m_nCardType == CARD_TYPE_GS7024E)
	{
		switch (m_nMaxActCount)
		{
		case 127:
			return 16384 * 1 * 4;
		case 63:
			return 16384 * 2 * 4;
		case 31:
			return 16384 * 4 * 4;
		default:
			return 16384 * 2;	//255
/*
		switch (m_nMaxActCount)
		{
		case 127:
			return 16384 * 1;
		case 63:
			return 16384 * 2;
		case 31:
			return 16384 * 4;
		default:
			return 16384 / 2;	//255
*/
		}
	}
	else if (m_nCardType == CARD_TYPE_GS7022B)
		return 16384;
	else if (m_nCardType == CARD_TYPE_GS2012A_S)
		return 65536;
	else if (m_nCardType == CARD_TYPE_GS2012AS_64)
		return 65536;
	else if (m_nCardType == CARD_TYPE_GS9020S)
		return 16384;
	else if (m_nCardType == CARD_TYPE_GS7020HS)
		return 16384;
	else if (m_nCardType == CARD_TYPE_PD102B)
		return 32768;
	else if (m_nCardType == CARD_TYPE_GS9025)
		return 16384;
	else if (m_nCardType == CARD_TYPE_GS9023)
		return 12288;
	else
	{
		switch (m_nMaxActCount)
		{
		case 511:
			return 4096;
		case 255:
			return 8192;
		case 63:
			return 16384 * 2;
		case 31:
			return 16384 * 4;
		default:
			return 16384;
		}
	}
}
//---------------------------------------------------------------------------

void CSysConfig::SaveActData(CDtu* dtu, int nActIndex, BYTE* p, int nLen)
{
	dtu = GetActDtu(dtu, nActIndex);
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	TMemoryStream* stream = new TMemoryStream;
	stream->SetSize(nLen);
	stream->Position = 0;
	if (nLen)
		stream->WriteBuffer(p, nLen);
	stream->Position = 0;
	ini->WriteBinaryStream(wsSection, "ActData", stream);
	delete stream;

	ini->WriteInteger(wsSection, "ActDataSize", nLen);

	delete ini;
}
//---------------------------------------------------------------------------

int CSysConfig::GetActDataSize(CDtu* dtu, int nActIndex)
{
#ifndef AUTO_SIZE_TEXT
	dtu = GetActDtu(dtu, nActIndex);
#endif
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	int nLen = ini->ReadInteger(wsSection, "ActDataSize", 0);
	if (nLen < 4)
		nLen = 4;

	delete ini;
	return nLen;
}
//---------------------------------------------------------------------------

int CSysConfig::ReadActData(CDtu* dtu, int nActIndex, int nStartPos, int nReadLen, BYTE* p)
{
#ifndef AUTO_SIZE_TEXT
	dtu = GetActDtu(dtu, nActIndex);
#endif
	UnicodeString sActRoot = dtu->m_sSection + "\\" + dtu->m_sName + "\\Acts";
	TCustomIniFile* ini = new TRegistryIniFile(sActRoot);
	UnicodeString wsSection = "ACT" + UnicodeString(nActIndex + 1);

	if ((nReadLen <= 4) && (nStartPos == 0))
	{
		*(DWORD*)p = 0xFFFFFFFF;
		return nReadLen;
	}

	TMemoryStream* stream = new TMemoryStream;
	int nAllLen = ini->ReadBinaryStream(wsSection, "ActData", stream);

	int nResult;
	if (nAllLen >= nStartPos + nReadLen)
	{
		stream->Position = nStartPos;
		stream->ReadBuffer(p, nReadLen);
		nResult = nReadLen;
	}
	else if (nAllLen <= nStartPos)
	{
		*p = 0xFF;
		nResult = 1;
	}
	else
	{
		stream->Position = nStartPos;
		stream->ReadBuffer(p, nAllLen - nStartPos);
		nResult = nAllLen - nStartPos;
	}

	delete stream;
	delete ini;

	return nResult;
}
//---------------------------------------------------------------------------

void CSysConfig::MakeCardTypeName()
{
	switch (m_nCardType)
	{
		case CARD_TYPE_GS7020B: m_sCardType = "GS7020B"; break;
		case CARD_TYPE_GS7030B: m_sCardType = "GS7030B"; break;
		case CARD_TYPE_GS2012A: m_sCardType = "GS2012A"; break;
		case CARD_TYPE_YL2010A: m_sCardType = "YL2010A"; break;
		case CARD_TYPE_YL2010C: m_sCardType = "YL2010C"; break;
		case CARD_TYPE_YL2010AF: m_sCardType = "YL2010AF"; break;
		case CARD_TYPE_GS7020BF: m_sCardType = "GS7020BF"; break;
		case CARD_TYPE_YL2010AR: m_sCardType = "YL2010AR"; break;
		case CARD_TYPE_PD102B: m_sCardType = "PD102B"; break;
		case CARD_TYPE_GS7020BF_HEB: m_sCardType = "GS7020BF_HEB"; break;
		case CARD_TYPE_GS9026: m_sCardType = "GS9026"; break;
		case CARD_TYPE_GS7020BF_W: m_sCardType = "GS7020BF_W"; break;
		case CARD_TYPE_GS7020BX: m_sCardType = "GS7020BX"; break;
		case CARD_TYPE_GS7020BT: m_sCardType = "GS7020BT"; break;
		case CARD_TYPE_GS7030BV2: m_sCardType = "GS7030BV2"; break;
		case CARD_TYPE_GS7022B: m_sCardType = "GS7022B"; break;
		case CARD_TYPE_GS7020BQ: m_sCardType = "GS7020BQ"; break;
		case CARD_TYPE_GS7020F: m_sCardType = "GS7020F"; break;
		case CARD_TYPE_GS7020BP: m_sCardType = "GS7020BP"; break;
		case CARD_TYPE_GS2012A_S: m_sCardType = "GS2012A-S"; break;
		case CARD_TYPE_GS7024E: m_sCardType = "GS7024E"; break;
		case CARD_TYPE_WF2012B: m_sCardType = "WF2012B"; break;
		case CARD_TYPE_GS2012AS_64: m_sCardType = "GS2012AS-64"; break;
		case CARD_TYPE_GS2012B: m_sCardType = "GS2012B"; break;
		case CARD_TYPE_GS9020: m_sCardType = "GS9020"; break;
		case CARD_TYPE_YL2010A_GPS_JTX: m_sCardType = "YL2010A_GPS_JTX"; break;
		case CARD_TYPE_GS9020S: m_sCardType = "GS9020S"; break;
		case CARD_TYPE_GS7020HS: m_sCardType = "GS7020HS"; break;
		case CARD_TYPE_GS7020H: m_sCardType = "GS7020H"; break;
		case CARD_TYPE_GS9025: m_sCardType = "GS9025"; break;
		case CARD_TYPE_GS9023: m_sCardType = "GS9023"; break;
		case CARD_TYPE_GS9020F_GPS: m_sCardType = "GS9020F_GPS"; break;
		case CARD_TYPE_GS7020HFA: m_sCardType = "GS7020HFA"; break;
		case CARD_TYPE_SN2P: m_sCardType = "SN2P"; break;
		default: m_sCardType = "GS7020B"; break;
	}
}
//---------------------------------------------------------------------------

void CSysConfig::InitCardTypeAndRootName()
{
	if (m_sCardType == "GS7020B")
	{
		m_nCardType = CARD_TYPE_GS7020B;
		prog = prog_GS7020B;
		g_nSizeofProg = g_nSizeofProg_GS7020B;
	}
	else if (m_sCardType == "GS7020BF")
	{
		m_nCardType = CARD_TYPE_GS7020BF;
		prog = prog_GS7020BF;
		g_nSizeofProg = g_nSizeofProg_GS7020BF;
	}
	else if (m_sCardType == "GS7020BF_HEB")
	{
		m_nCardType = CARD_TYPE_GS7020BF_HEB;
		prog = prog_GS7020BF_HEB;
		g_nSizeofProg = g_nSizeofProg_GS7020BF_HEB;
	}
	else if (m_sCardType == "GS9026")
	{
		m_nCardType = CARD_TYPE_GS9026;
		prog = prog_GS9026;
		g_nSizeofProg = g_nSizeofProg_GS9026;
	}
	else if (m_sCardType == "GS7020BF_W")
	{
		m_nCardType = CARD_TYPE_GS7020BF_W;
		prog = prog_GS7020BF_W;
		g_nSizeofProg = g_nSizeofProg_GS7020BF_W;
	}
	else if (m_sCardType == "GS7030B")
	{
		m_nCardType = CARD_TYPE_GS7030B;
		prog = prog_GS7030B;
		g_nSizeofProg = g_nSizeofProg_GS7030B;
	}
	else if (m_sCardType == "YL2010AF")
	{
		m_nCardType = CARD_TYPE_YL2010AF;
		prog = NULL;
	}
	else if (m_sCardType == "YL2010A")
	{
		m_nCardType = CARD_TYPE_YL2010A;
		prog = prog_YL2010A;
		g_nSizeofProg = g_nSizeofProg_YL2010A;
	}
	else if (m_sCardType == "YL2010C")
	{
		m_nCardType = CARD_TYPE_YL2010C;
		prog = NULL;
	}
	else if (m_sCardType == "GS2012A" || m_sCardType == "YL2012A")
	{
		m_nCardType = CARD_TYPE_GS2012A;
		prog = prog_GS2012A;
		g_nSizeofProg = g_nSizeofProg_GS2012A;
	}
	else if (m_sCardType == "YL2010AR")
	{
		m_nCardType = CARD_TYPE_YL2010AR;
		prog = prog_YL2010AR;
		g_nSizeofProg = g_nSizeofProg_YL2010AR;
	}
	else if (m_sCardType == "PD102B")
	{
		m_nCardType = CARD_TYPE_PD102B;
		prog = NULL;
	}
	else if (m_sCardType == "GS7020BX")
	{
		m_nCardType = CARD_TYPE_GS7020BX;
		prog = prog_GS7020BX;
		g_nSizeofProg = g_nSizeofProg_GS7020BX;
	}
	else if (m_sCardType == "GS7020BT")
	{
		m_nCardType = CARD_TYPE_GS7020BT;
		prog = prog_GS7020BT;
		g_nSizeofProg = g_nSizeofProg_GS7020BT;
	}
	else if (m_sCardType == "GS7030BV2")
	{
		m_nCardType = CARD_TYPE_GS7030BV2;
		prog = prog_GS7030BV2;
		g_nSizeofProg = g_nSizeofProg_GS7030BV2;
	}
	else if (m_sCardType == "GS7022B")
	{
		m_nCardType = CARD_TYPE_GS7022B;
		prog = prog_GS7022B;
		g_nSizeofProg = g_nSizeofProg_GS7022B;
	}
	else if (m_sCardType == "GS7020BQ")
	{
		m_nCardType = CARD_TYPE_GS7020BQ;
		prog = prog_GS7020HQ;
		g_nSizeofProg = g_nSizeofProg_GS7020HQ;
	}
	else if (m_sCardType == "GS7020F")
	{
		m_nCardType = CARD_TYPE_GS7020F;
		prog = prog_GS7020B;
		g_nSizeofProg = g_nSizeofProg_GS7020B;
	}
	else if (m_sCardType == "GS7020BP")
	{
		m_nCardType = CARD_TYPE_GS7020BP;
		prog = prog_GS7020BP;
		g_nSizeofProg = g_nSizeofProg_GS7020BP;
	}
	else if (m_sCardType == "GS2012A-S" || m_sCardType == "YL2012A-S")
	{
		m_nCardType = CARD_TYPE_GS2012A_S;
		prog = prog_GS2012A_S;
		g_nSizeofProg = g_nSizeofProg_GS2012A_S;
	}
	else if (m_sCardType == "GS7024E")
	{
		m_nCardType = CARD_TYPE_GS7024E;
		prog = prog_GS7024E;
		g_nSizeofProg = g_nSizeofProg_GS7024E;
	}
	else if (m_sCardType == "WF2012B")
	{
		m_nCardType = CARD_TYPE_WF2012B;
		prog = NULL;
	}
	else if (m_sCardType == "GS2012AS-64")
	{
		m_nCardType = CARD_TYPE_GS2012AS_64;
		prog = NULL;
	}
	else if (m_sCardType == "GS9020S")
	{
		m_nCardType = CARD_TYPE_GS9020S;
		prog = NULL;
	}
	else if (m_sCardType == "GS7020HS")
	{
		m_nCardType = CARD_TYPE_GS7020HS;
		prog = NULL;
	}
	else if (m_sCardType == "GS2012B")
	{
		m_nCardType = CARD_TYPE_GS2012B;
		prog = prog_GS2012B;
		g_nSizeofProg = g_nSizeofProg_GS2012B;
	}
	else if (m_sCardType == "GS9020")
	{
		m_nCardType = CARD_TYPE_GS9020;
		prog = prog_GS9020;
		g_nSizeofProg = g_nSizeofProg_GS9020;
	}
	else if (m_sCardType == "YL2010A_GPS_JTX")
	{
		m_nCardType = CARD_TYPE_YL2010A_GPS_JTX;
		prog = prog_YL2010A;
		g_nSizeofProg = g_nSizeofProg_YL2010A;
	}
	else if (m_sCardType == "GS7020H")
	{
		m_nCardType = CARD_TYPE_GS7020H;
		prog = prog_GS7020H;
		g_nSizeofProg = g_nSizeofProg_GS7020H;
	}
	else if (m_sCardType == "GS9025")
	{
		m_nCardType = CARD_TYPE_GS9025;
		prog = prog_GS9025;
		g_nSizeofProg = g_nSizeofProg_GS9025;
	}
	else if (m_sCardType == "GS9023")
	{
		m_nCardType = CARD_TYPE_GS9023;
		prog = prog_GS9023;
		g_nSizeofProg = g_nSizeofProg_GS9023;
	}
	else if (m_sCardType == "GS9020F_GPS")
	{
		m_nCardType = CARD_TYPE_GS9020F_GPS;
		prog = prog_GS9020F_GPS;
		g_nSizeofProg = g_nSizeofProg_GS9020F_GPS;
	}
	else if (m_sCardType == "GS7020HFA")
	{
		m_nCardType = CARD_TYPE_GS7020HFA;
		prog = prog_GS7020HFA;
		g_nSizeofProg = g_nSizeofProg_GS7020HFA;
	}
	else if (m_sCardType == "SN2P")
	{
		m_nCardType = CARD_TYPE_SN2P;
		prog = prog_SN2P;
		g_nSizeofProg = g_nSizeofProg_SN2P;
	}
	else
	{
		m_nCardType = CARD_TYPE_GS7020B;
		prog = prog_GS7020B;
		g_nSizeofProg = g_nSizeofProg_GS7020B;
	}

	if (m_nCardType == CARD_TYPE_GS2012A)
		m_nMaxPackSize = GS2012A_PACK_SIZE;
	else if (m_nCardType == CARD_TYPE_GS2012A_S)
		m_nMaxPackSize = 1024;
	else if (m_nCardType == CARD_TYPE_WF2012B)
		m_nMaxPackSize = 1024;
	else if (m_nCardType == CARD_TYPE_GS7030B)
		m_nMaxPackSize = 512;
	else if (m_nCardType == CARD_TYPE_GS7030BV2)
		m_nMaxPackSize = 512;
	else if (m_nCardType == CARD_TYPE_PD102B)
		m_nMaxPackSize = 64;
	else if (m_nCardType == CARD_TYPE_GS2012AS_64)
		m_nMaxPackSize = 1024;
	else if (m_nCardType == CARD_TYPE_GS2012B)
		m_nMaxPackSize = 1024;
	else if (m_nCardType == CARD_TYPE_GS9020S)
		m_nMaxPackSize = 512;
	else if (m_nCardType == CARD_TYPE_GS7020HS)
		m_nMaxPackSize = 512;
	else
		m_nMaxPackSize = 512;

	if (m_nCardType == CARD_TYPE_GS7022B)
		m_nMaxActCount = 32;
	else if (m_nCardType == CARD_TYPE_GS2012A_S)
		m_nMaxActCount = 32;
	else if (m_nCardType == CARD_TYPE_GS2012AS_64)
		m_nMaxActCount = 32;
	else if (m_nCardType == CARD_TYPE_GS9020S)
		m_nMaxActCount = 32;
	else if (m_nCardType == CARD_TYPE_GS7020HS)
		m_nMaxActCount = 32;
	else if (m_nCardType == CARD_TYPE_GS7020BF_W)
		m_nMaxActCount = 255;
	else if (m_nCardType == CARD_TYPE_GS9025)
		m_nMaxActCount = 16;
	else if (m_nCardType == CARD_TYPE_GS9023)
		m_nMaxActCount = 128;
	else if (m_nCardType == CARD_TYPE_GS7024E || m_nCardType == CARD_TYPE_GS7020HFA)
	{
		switch (m_nMaxActCount)
		{
		case 255:
		case 127:
		case 63:
		case 31:
			break;
		default:
			m_nMaxActCount = 127;
		}
	}
	else
	{
		switch (m_nMaxActCount)
		{
		case 127:
		case 63:
		case 31:
			break;
		default:
			m_nMaxActCount = 127;
		}
	}

	m_nSycleTimeActIndex = m_nMaxActCount - 1;
	if (m_bSupportWarning)
		m_nSycleTimeActIndex--;
	if (m_bSupportInsertPlay)
		m_nSycleTimeActIndex -= 5;
}
//---------------------------------------------------------------------------

CDtu* CSysConfig::GetActDtu(CDtu* dtu, int nActIndex)
{
	if (dtu->m_nNodeType == NODE_ALL_CARD_ROOT)
		return dtu;

	if (dtu->m_nNodeType == NODE_CARD && dtu->m_dtuParent->m_nNodeType == NODE_ALL_CARD_ROOT)
		return dtu->m_dtuParent;

	CDtu* dtu_Group = dtu;
	while (dtu_Group->m_nNodeType != NODE_GROUP)
		dtu_Group = dtu_Group->m_dtuParent;

	while ((dtu_Group->m_dtuParent)->m_nNodeType != NODE_ALL_CARD_ROOT)
		dtu_Group = dtu_Group->m_dtuParent;

	if (sysConfig.m_bActSuitForAllCard[nActIndex])
		return dtu_Group->m_dtuParent;
	else
		return dtu_Group;
}
//---------------------------------------------------------------------------

int BCD2Hex(BYTE c)
{
	return (c >> 4) * 10 + (c & 15);
}
