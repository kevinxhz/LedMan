//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "GpsMap.h"
#include "SysConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "IdBaseComponent"
#pragma link "IdComponent"
#pragma link "IdUDPBase"
#pragma link "IdUDPClient"
#pragma resource "*.dfm"
TfrmGpsMap *frmGpsMap;
//---------------------------------------------------------------------------
__fastcall TfrmGpsMap::TfrmGpsMap(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TfrmGpsMap::StartBrowse()
{
	int wd_int = BCD2Hex(ucGpsData[6]);
	int wd_frac = ((BCD2Hex(ucGpsData[7]) * 10000 + BCD2Hex(ucGpsData[8]) * 100 + BCD2Hex(ucGpsData[9]))) * 10 / 6;
	int jd_int = BCD2Hex(ucGpsData[10])* 10 + (ucGpsData[11] >> 4);
	int jd_frac = ((ucGpsData[11] & 15) * 10000 + BCD2Hex(ucGpsData[12]) * 100 + BCD2Hex(ucGpsData[13])) * 10 / 6;

	AnsiString sbr = "<!DOCTYPE html>\r\n";
	sbr += "<html>\r\n";
	sbr += "<head>\r\n";
	sbr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\" />\r\n";
	sbr += "<meta http-equiv=\"content-type\" content=\"text/html; charset=gb2312\"/>\r\n";
	sbr += "<style type=\"text/css\">\r\n";
	sbr += "html {height: auto;}\r\n";
	sbr += "body {height: auto;margin: 0;padding: 0;}\r\n";
	sbr += "#map_canvas {width:";
	sbr += AnsiString(ClientWidth);
	sbr += "px;height: ";
	sbr += AnsiString(ClientHeight);
	sbr += "px;position: absolute;}\r\n";
	sbr += "@media print {#map_canvas {height: 950px;}}\r\n";
	sbr += "</style>\r\n";
	sbr += "<title>北京禹风时代信息技术有限公司</title>\r\n";
	sbr += "<script type=\"text/javascript\" src=\"http://api.go2map.com/maps/js/api_v2.5.1.js\"></script>\r\n";
	sbr += "<script type=\"text/javascript\">\r\n";
	sbr += "  function initialize() {\r\n";
	sbr += "	var myLatlng = new sogou.maps.LatLng(";
	sbr += AnsiString(wd_int) + "." + AnsiString(wd_frac);
	sbr += ",";
	sbr += AnsiString(jd_int) + "." + AnsiString(jd_frac);
	sbr += ");\r\n";
	sbr += "    var myOptions = {\r\n";
	sbr += "      zoom: 15,\r\n";
	sbr += "      center: myLatlng,\r\n";
	sbr += "      mapTypeId: sogou.maps.MapTypeId.ROADMAP\r\n";
	sbr += "    }\r\n";
	sbr += "    var map = new sogou.maps.Map(document.getElementById(\"map_canvas\"), myOptions);\r\n";
	sbr += "    var marker = new sogou.maps.Marker({\r\n";
	sbr += "        position: myLatlng,\r\n";
	sbr += "        map: map,\r\n";
	sbr += "        title:\"";
	sbr += m_dtu->m_sName;
	sbr += " 速度:";
	sbr += GetSpeedString();
	sbr += " 方向:";
	sbr += GetDegreeString() + "(";
	sbr += GetTimeString() + ")";
	sbr += "\"\r\n";
	sbr += "    });\r\n";
	sbr += "  }\r\n";
	sbr += "</script>\r\n";
	sbr += "</head>\r\n";
	sbr += "<body onload=\"initialize()\">\r\n";
	sbr += "  <div id=\"map_canvas\"></div>\r\n";
	sbr += "</body>\r\n";
	sbr += "</html>\r\n";
	int hFile = FileCreate("c:\\map.htm");
	FileWrite(hFile, sbr.c_str(), sbr.Length());
	FileClose(hFile);

	wb->Navigate("c:\\map.htm");
}
//---------------------------------------------------------------------------

void __fastcall TfrmGpsMap::btnRefreshClick(TObject *Sender)
{
	if (!OpenCommPort())
	{
		Application->MessageBox(L"网络连接失败", L"错误", MB_OK | MB_ICONWARNING);
		return;
	}

	timerQueryPosition->Enabled = true;
	btnRefresh->Enabled = false;
	btnStop->Enabled = true;
}
//---------------------------------------------------------------------------

bool TfrmGpsMap::OpenCommPort()
{
	try
	{
		udpSocket->Host = sysConfig.m_sServer;
		udpSocket->Port = sysConfig.m_nSocketPort;
		udpSocket->Active = true;
	}
	catch (Exception &ex)
	{
		return false;
	}

	m_dtu->m_nRxLen = 0;
	m_dtu->m_nTxStep = 0;
	m_dtu->m_nDataSent = 0;
	m_dtu->m_nRetries = 0;
	m_dtu->m_nComPortRxStep = 0;
	m_dtu->m_nTxStatus = TX_STATUS_READY;

	return true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmGpsMap::timerQueryPositionTimer(TObject *Sender)
{
	DWORD dwTime;

	if (udpSocket->Active)
		ReceiveUdpStatusPack();

	switch (m_dtu->m_nTxStatus)
	{
	case TX_STATUS_READY:
		SendCommand_QueryPosition(m_dtu);
		m_dtu->m_nTxStatus = TX_STATUS_WAIT_ECHO;
		m_dtu->m_dwTxStartTime = ::GetTickCount();
		break;
	case TX_STATUS_WAIT_ECHO:
		if (m_dtu->m_nTxStep == 1)	//在线
		{
			//m_dtu->m_nTxStatus = TX_STATUS_END_OK;
			//timerQueryPosition->Enabled = false;
			//udpSocket->Active = false;
			StartBrowse();
			OpenCommPort();
		}
		else if (m_dtu->m_nTxStep == 2) //不在线
		{
			//m_dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
			//timerQueryPosition->Enabled = false;
			//udpSocket->Active = false;
			OpenCommPort();
		}
		else
		{
			dwTime = ::GetTickCount() - m_dtu->m_dwTxStartTime;
			if (dwTime > (DWORD)sysConfig.m_nTimeout_Network * 1000)
			{
				//m_dtu->m_nTxStatus = TX_STATUS_END_TIME_OUT;
				//timerQueryPosition->Enabled = false;
				//udpSocket->Active = false;
				OpenCommPort();
			}
		}
		break;
	}
}
//---------------------------------------------------------------------------

void TfrmGpsMap::ReceiveUdpStatusPack()
{
#pragma pack(1)
	struct
	{
		char szGsmCode[12];
		BYTE ucFrameType;
		char cStatus;
		BYTE ucGpsData[18];
	}stHead;
#pragma pack()
	TIdBytes buf;
	BYTE* p;
	int nLen, i;
	buf.set_length(2048);

	if (!udpSocket->Active)
		return;

	for (;;)
	{
		try
		{
			nLen = udpSocket->ReceiveBuffer(buf, 0);
		}
		catch (Exception &ex)
		{
			break;
		}

		if (nLen <= 0)
			break;

		if (nLen < (int)sizeof(stHead))
			continue;

		p = (BYTE*)&stHead;
		for (i = 0; i < (int)sizeof(stHead); i++)
			*p++ = buf[i];

		if (stHead.ucFrameType != FT_GPS)
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

		if (m_dtu->m_nTxStatus == TX_STATUS_WAIT_ECHO)
		{
			if (::strcmp(m_dtu->m_sCode.c_str(), stHead.szGsmCode) == 0)
			{
				if (!stHead.cStatus)
					m_dtu->m_nTxStep = 2;
				else
				{
					if (memcmp(stHead.ucGpsData, ucGpsData, 18) == 0)
						m_dtu->m_nTxStep = 2;
					else
					{
						m_dtu->m_nTxStep = 1;
						memcpy(ucGpsData, stHead.ucGpsData, 18);
					}
				}
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------

void TfrmGpsMap::SendCommand_QueryPosition(CDtu* dtu)
{
#pragma pack(1)
	struct
	{
		char szUserName[12];
		BYTE ucFrameType;
		char szPassword[16];
		char szDtuCode[12];
	}stData;
#pragma pack()
	size_t i;

	::strcpy(stData.szUserName, AnsiString(sysConfig.m_sUserName).c_str());
	for (i = 0; i < strlen(stData.szUserName); i++)
		stData.szUserName[i] ^= 0xAA;
	::strcpy(stData.szPassword, AnsiString(sysConfig.m_sPassword).c_str());
	for (i = 0; i < strlen(stData.szPassword); i++)
		stData.szPassword[i] ^= 0xAA;
	stData.ucFrameType = FT_GPS;
	::strcpy(stData.szDtuCode, dtu->m_sCode.c_str());

	TIdBytes ar;
	ar.set_length(sizeof(stData));
	BYTE* p = (BYTE*)&stData;
	for (i = 0; i < sizeof(stData); i++)
		ar[i] = *p++;
	try
	{
		udpSocket->SendBuffer(ar);
	}
	catch (Exception &ex)
	{
	}
}
//---------------------------------------------------------------------------

void TfrmGpsMap::SendData_UDP(CDtu* dtu, int nLen, BYTE* p)
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

	TX_DATA_HEADER* pHeader = (TX_DATA_HEADER*)buf;
	::strcpy(pHeader->szUserName, AnsiString(sysConfig.m_sUserName).c_str());
	for (i = 0; i < ::strlen(pHeader->szUserName); i++)
		pHeader->szUserName[i] ^= 0xAA;
	::strcpy(pHeader->szPassword, AnsiString(sysConfig.m_sPassword).c_str());
	for (i = 0; i < ::strlen(pHeader->szPassword); i++)
		pHeader->szPassword[i] ^= 0xAA;
	::strncpy(pHeader->szDtuCode, dtu->m_sCode.c_str(), 11);
	pHeader->szDtuCode[11] = 0;
	pHeader->ucFrameType = FT_DATA;
	memcpy(buf + sizeof(TX_DATA_HEADER), p, nLen);

	TIdBytes ar;
	ar.set_length(nLen + sizeof(TX_DATA_HEADER));
	for (i = 0; i < nLen + sizeof(TX_DATA_HEADER); i++)
		ar[i] = buf[i];
	udpSocket->SendBuffer(ar);
}
//---------------------------------------------------------------------------

void __fastcall TfrmGpsMap::FormDestroy(TObject *Sender)
{
	udpSocket->Active = false;
	timerQueryPosition->Enabled = false;
	btnRefresh->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmGpsMap::btnStopClick(TObject *Sender)
{
	timerQueryPosition->Enabled = false;
	udpSocket->Active = false;
	btnRefresh->Enabled = true;
	btnStop->Enabled = false;
}
//---------------------------------------------------------------------------

AnsiString TfrmGpsMap::GetSpeedString()
{
	int nSpeed = BCD2Hex(ucGpsData[14]) * 100 + BCD2Hex(ucGpsData[15]);
	return AnsiString(nSpeed) + "km/小时";
}
//---------------------------------------------------------------------------

AnsiString TfrmGpsMap::GetDegreeString()
{
	int nDegree = BCD2Hex(ucGpsData[16]) * 100 + BCD2Hex(ucGpsData[17]);

	if (nDegree < 22 || nDegree > 337)
		return "北";
	else if (nDegree >= 22 && nDegree < 67)
		return "东北";
	else if (nDegree >= 67 && nDegree < 112)
		return "东";
	else if (nDegree >= 112 && nDegree < 157)
		return "东南";
	else if (nDegree >= 157 && nDegree < 202)
		return "南";
	else if (nDegree >= 202 && nDegree < 247)
		return "西南";
	else if (nDegree >= 247 && nDegree < 292)
		return "西";
	else //if (nDegree >= 292 && nDegree <= 337)
		return "西北";
}
//---------------------------------------------------------------------------

AnsiString TfrmGpsMap::GetTimeString()
{
	char s[32];
	sprintf(s, "%02d:%02d:%02d", ucGpsData[3], ucGpsData[4], ucGpsData[5]);

	return AnsiString(s);
}
//---------------------------------------------------------------------------

