//---------------------------------------------------------------------------

#ifndef SerialPortH
#define SerialPortH
#include <vcl.h>
#include <windows.h>
//---------------------------------------------------------------------------
class CSerialPort
{
public:
	CSerialPort();
	~CSerialPort();
	BOOL GetOpen();
	void SetPortName(AnsiString sPortName);
	void SetBaudRate(int nBaudRate);
	BOOL Open();
	void Close();
	BOOL Write(int nLen, const BYTE* pchData);
	int GetInQueue();
	int Read(int nLen, BYTE* pchData);
	void Flush();
	int m_nBaudRate;
	AnsiString m_sPortName;
	HANDLE m_hCommPort;
	BOOL m_bOpened;
};
#endif
