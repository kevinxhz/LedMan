#ifndef _LOG_H_
#define _LOG_H_

class TLog
{
public:
	//AnsiString m_sPath;
	int m_nDtuIndex;
	int m_hFile;

	void StartLogTx();
	void LogAct(CDtu* dtu, int nActIndex);
	void StartLogCard();
	void LogTx(CDtu* dtu, bool bOk);
	void EndLogTx();
};

#endif

