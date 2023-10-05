#pragma hdrstop
#include "SerialPort.h"
#include "Sysconfig.h"
#pragma package(smart_init)

CSerialPort::CSerialPort()
{
	m_sPortName = "COM1:";
	m_hCommPort = NULL;
	m_nBaudRate = 115200;
	m_bOpened = FALSE;
}

CSerialPort::~CSerialPort()
{
	Close();
}

BOOL CSerialPort::GetOpen()
{
	return m_bOpened;
}

void CSerialPort::SetPortName(AnsiString sPortName)
{
	char* sName = sPortName.c_str();
	char* s = &sName[strlen(sName) - 1];

	for (;;)
	{
		if ((*s == ':') || (*s == '\r') || (*s == '\n'))
			*s = 0;
		else
			break;

		if (s != sName)
			s--;
		else
			break;
	}

	m_sPortName = sPortName;
}

void CSerialPort::SetBaudRate(int nBaudRate)
{
	m_nBaudRate = nBaudRate;
}

BOOL CSerialPort::Open()
{
	if (m_bOpened) return TRUE;

	AnsiString sName = "\\\\.\\";
	sName += m_sPortName;

	m_hCommPort = CreateFileA(
		sName.c_str(),					// communication port string (COMX)
		GENERIC_READ | GENERIC_WRITE,	// read/write types
		NULL,							// comm devices must be opened with exclusive access
		NULL,							// no security attributes
		OPEN_EXISTING,					// comm devices must use OPEN_EXISTING
		NULL,
		NULL);							// template must be 0 for comm devices
	if (m_hCommPort == INVALID_HANDLE_VALUE)
		return FALSE;

	m_bOpened = TRUE;

	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));

	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = m_nBaudRate;
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.XonLim = 64;
	dcb.XoffLim = 64;
	dcb.fTXContinueOnXoff = TRUE;

	::SetCommState(m_hCommPort, &dcb);
	::SetupComm(m_hCommPort, 8192, 8192);

	return TRUE;
}

void CSerialPort::Close()
{
	if (m_bOpened)
	{
		CloseHandle(m_hCommPort);
		m_bOpened = FALSE;
	}
}

BOOL CSerialPort::Write(int nLen, const BYTE* pchData)
{
	if (!m_bOpened)
		return FALSE;

	int nTx = 0;
	DWORD dw;
	while (nTx < nLen)
	{
		WriteFile(m_hCommPort, pchData + nTx, nLen - nTx, &dw, NULL);
		if (dw == 0)
			return FALSE;
		nTx += dw;
	}

	return TRUE;
}

int CSerialPort::GetInQueue()
{
	COMSTAT comstat;
	DWORD dwError = 0xffffff;
	ClearCommError(m_hCommPort, &dwError, &comstat);
	return comstat.cbInQue;
}

int CSerialPort::Read(int nLen, BYTE* pchData)
{
	if (!GetInQueue())
		return 0;

	DWORD dwBytesRead = 0;
	ReadFile(m_hCommPort, pchData, nLen, &dwBytesRead, NULL);
	return dwBytesRead;
}

void CSerialPort::Flush()
{
	BYTE c;

	while (GetInQueue())
	{
		DWORD dwBytesRead = 0;
		ReadFile(m_hCommPort, &c, 1, &dwBytesRead, NULL);
	}
}
