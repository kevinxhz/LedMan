#include <vcl.h>
#include "sysConfig.h"
#include "dtu.h"
#include "log.h"

void TLog::StartLogTx()
{
	char s[128];
	SYSTEMTIME st;
	::GetLocalTime(&st);

	TIniFile* ini = new TIniFile(ExtractFilePath(Application->ExeName) + "log\\TxLogSum.txt");
	int nLogTxCount = ini->ReadInteger("ACTLOG", "TxCount", 0);
	nLogTxCount++;
	ini->WriteInteger("ACTLOG", "TxCount", nLogTxCount);
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
		sprintf(s, "%04d年%02d月%02d日%02d时%02d分%02d秒", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	else
		sprintf(s, "%04d-%02d-%02d_%02d_%02d_%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	ini->WriteString("ACTLOG", "TX" + AnsiString(nLogTxCount), s);
	delete ini;

	m_hFile = FileCreate(ExtractFilePath(Application->ExeName) + "log\\" + s + ".txt", fmOpenWrite);
	FileWrite(m_hFile, "[ACT]\r\n", 7);

	m_nDtuIndex = 1;
}

void TLog::LogAct(CDtu* dtu, int nActIndex)
{
	AnsiString s = "ActDesc" + AnsiString(nActIndex + 1) + "=" + sysConfig.GetActDesc(dtu, nActIndex) + "\r\n" +
				   "ActText" + AnsiString(nActIndex + 1) + "=" + sysConfig.GetActText(dtu, nActIndex) + "\r\n";
	FileWrite(m_hFile, s.c_str(), s.Length());
}

void TLog::StartLogCard()
{
	FileWrite(m_hFile, "[CARD]\r\n", 8);
}

void TLog::LogTx(CDtu* dtu, bool bOk)
{
	AnsiString s = "CODE_" + AnsiString(m_nDtuIndex) + "=" + dtu->m_sCode + "\r\n" +
				   "NAME_" + AnsiString(m_nDtuIndex) + "=" + dtu->m_sName + "\r\n" +
				   "OK_" + AnsiString(m_nDtuIndex) + "=" + (bOk ? "1" : "0") + "\r\n";
	FileWrite(m_hFile, s.c_str(), s.Length());

	m_nDtuIndex++;
}

void TLog::EndLogTx()
{
	FileClose(m_hFile);
}

