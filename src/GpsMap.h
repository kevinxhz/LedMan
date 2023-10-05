//---------------------------------------------------------------------------

#ifndef GpsMapH
#define GpsMapH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <OleCtrls.hpp>
#include <SHDocVw.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include "IdBaseComponent.hpp"
#include "IdComponent.hpp"
#include "IdUDPBase.hpp"
#include "IdUDPClient.hpp"
#include "dtu.h"
//---------------------------------------------------------------------------
class TfrmGpsMap : public TForm
{
__published:	// IDE-managed Components
	TWebBrowser *wb;
	TToolBar *ToolBar1;
	TTimer *timerQueryPosition;
	TSpeedButton *btnRefresh;
	TIdUDPClient *udpSocket;
	TSpeedButton *btnStop;
	void __fastcall btnRefreshClick(TObject *Sender);
	void __fastcall timerQueryPositionTimer(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btnStopClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmGpsMap(TComponent* Owner);

	BYTE ucGpsData[18];
//	BYTE ucWeidu[4];
//	BYTE ucJingdu[4];
//	BYTE ucSpeed[2];
//	BYTE ucDegree[2];
	CDtu* m_dtu;
	void StartBrowse();
	bool OpenCommPort();
	void ReceiveUdpStatusPack();
	void SendCommand_QueryPosition(CDtu* dtu);
	bool WaitForCmdEcho(BYTE ucCmd, CDtu* dtu, int nDataLen);
	BYTE CalcChecksum(int nLen, BYTE* p);
	void SendData_UDP(CDtu* dtu, int nLen, BYTE* p);
	AnsiString GetSpeedString();
	AnsiString GetDegreeString();
	AnsiString GetTimeString();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmGpsMap *frmGpsMap;
//---------------------------------------------------------------------------
#endif
