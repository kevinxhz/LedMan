//---------------------------------------------------------------------------

#ifndef CustMsgDlgH
#define CustMsgDlgH
//---------------------------------------------------------------------------
#include "Global.h"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <ImgList.hpp>
#include <Graphics.hpp>
#include <Buttons.hpp>
#include "dtu.h"
#include <MPlayer.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <CategoryButtons.hpp>
#include <Outline.hpp>
#include <FileCtrl.hpp>
#include "VirtualTrees.hpp"
#include "IdUDPServer.hpp"
#include <OleCtrls.hpp>
#include <SHDocVw.hpp>
#include "SHDocVw_OCX.h"
#include "IdHTTP.hpp"
#include "IdTCPClient.hpp"
#include "IdTCPConnection.hpp"
#include <vector>
#include <list>
#include "act.h"
#include "log.h"
#include "TextEditForm.h"
#include "SerialPort.h"
#include "Weather_64x64.h"
using namespace std;
typedef list<void*> LIST_PTR;
//---------------------------------------------------------------------------

//typedef list<TListItem*> LIST_DTU;

class TWinControlW: public TWinControl
{
public:
	__property DefWndProc;
};

//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TOpenPictureDialog *OpenPictureDialog;
	TPanel *panelDtuList;
	TTimer *timerTxAct;
	TFontDialog *dlgFont;
	TImageList *imgListMenu;
	TPanel *panelEdit;
	TListView *listActs;
	TSplitter *Splitter3;
	TPanel *panelMisc;
	TLabel *lbEnterStyle;
	TComboBox *comboEnterStyle;
	TLabel *lbEnterSpeed;
	TLabel *lbStayTime;
	TEdit *editEnterSpeed;
	TLabel *lbSpeed1;
	TLabel *lbExitStyle;
	TComboBox *comboExitStyle;
	TLabel *lbExitSpeed;
	TEdit *editExitSpeed;
	TLabel *lbSpeed2;
	TLabel *lbStayTimeHint;
	TEdit *editShowTime;
	TLabeledEdit *editActDesc;
	TLabel *lbAreaOfAct;
	TComboBox *comboArea;
	TComboBox *comboEditAct;
	TLabel *lbEditingAct;
	TButton *btnLoadPicture;
	TLabel *lbPictureX;
	TEdit *editPicPosX;
	TLabel *lbPictureY;
	TEdit *editPicPosY;
	TPanel *panelScreen;
	TSpeedButton *btnAlignLeft;
	TSpeedButton *btnAlignCenter;
	TScrollBox *scrollboxEdit;
	TImage *imgScreen;
	TSpeedButton *btnAlignRight;
	TSpeedButton *btnAlignVertCenter;
	TSpeedButton *btnRed;
	TSpeedButton *btnGreen;
	TSpeedButton *btnYellow;
	TSpeedButton *btnBlack;
	TSpeedButton *btnRed_Back;
	TSpeedButton *btnGreen_Back;
	TSpeedButton *btnBlack_Back;
	TButton *btnFont;
	TMainMenu *mnuMain;
	TMenuItem *mnuSystem;
	TMenuItem *mnuAdminPassword;
	TMenuItem *N1;
	TMenuItem *mnuCardManage;
	TMenuItem *mnuCardConfig;
	TMenuItem *mnuSelect;
	TMenuItem *mnuCancelAllCard;
	TMenuItem *mnuSelectAllCard;
	TMenuItem *N3;
	TMenuItem *mnuCancelAllAct;
	TMenuItem *mnuSelectAllAct;
	TMenuItem *mnuEdit;
	TMenuItem *N4;
	TMenuItem *mnuPreviewAllAct;
	TMenuItem *mnuPreviewCurrentAct;
	TMenuItem *mnuCommunication;
	TMenuItem *mnuManualOpenScreen;
	TMenuItem *mnuManualCloseScreen;
	TMenuItem *mnuAdjustCardClock;
	TMenuItem *mnuCheckCardOnline;
	TMenuItem *N5;
	TMenuItem *mnuSendAct;
	TMenuItem *mnuHelp;
	TMenuItem *mnuAbout;
	TMenuItem *N2;
	TMenuItem *mnuUpdateProg;
	TTimer *timer_UpdateProg;
	TIdUDPClient *udpSocket;
	TTimer *timerCheckOnline;
	TTimer *timerWriteClock;
	TMenuItem *mnuReadCardClock;
	TTimer *timerReadClock;
	TTimer *timer_Execute;
	TLabel *lbShutterWidth;
	TEdit *editShutterWidth;
	TDateTimePicker *dateActEnd;
	TLabel *lbEnd;
	TGroupBox *groupBoxPeriod;
	TMenuItem *mnuSearchCard;
	TMenuItem *mnuSearchAct;
	TMenuItem *N6;
	TMenuItem *mnuDeleteAct;
	TLabel *lbStart;
	TDateTimePicker *dateActStart;
	TEdit *editFlashPeriod;
	TLabel *Label25;
	TDateTimePicker *timeActStart;
	TDateTimePicker *timeActEnd;
	TImage *imgPreview;
	TMenuItem *mnuDeleteText;
	TMenuItem *mnuDeletePicture;
	TLabel *lbDisplayTime;
	TLabel *lbPlaytime;
	TButton *btnCalcDisplayTime;
	TMenuItem *mnuSetTextActive;
	TMenuItem *N8;
	TMenuItem *mnuSetTextNormal;
	TSplitter *Splitter1;
	TImageList *TreeImages;
	TVirtualStringTree *vt;
	TToolBar *toolBar;
	TSpeedButton *btnSendAct;
	TSpeedButton *btnCardConfig;
	TSpeedButton *btnCardList;
	TSpeedButton *btnPreview;
	TSpeedButton *btnFindNextCard;
	TSpeedButton *btnFindNextAct;
	TSpeedButton *btnSelectAllCard;
	TSpeedButton *btnUnSelectAllCard;
	TSpeedButton *btnUnSelectAllAct;
	TSpeedButton *btnSelectAllAct;
	TSpeedButton *btnDeleteActContent;
	TSpeedButton *btnCheckOnline;
	TSpeedButton *btnReadCardClock;
	TSpeedButton *btnWriteCardClock;
	TMenuItem *N7;
	TMenuItem *mnuSaveActFile;
	TMenuItem *mnuOpenActFile;
	TSaveDialog *ActFileSaveDialog;
	TOpenDialog *ActFileOpenDialog;
	TTimer *timer_AutoWaitReTxAct;
	TMenuItem *mnuManualReTxAct;
	TMenuItem *mnuViewFailedTxAct;
	TMenuItem *mnuLog;
	TMenuItem *mnuQueryTxLogs;
	TMenuItem *mnuLanguage;
	TMenuItem *mnuChinese;
	TMenuItem *mnuEnglish;
	TMenuItem *N9;
	TMenuItem *mnuAutoCheckActChange;
	TTimer *timerCheckDynamicAct;
	TImage *imgActive;
	TTimer *timerAutoWriteClock;
	TLabel *lbRepeatCount;
	TEdit *editRepeatCount;
	TSpeedButton *btnTxWeatherWarnning;
	TGroupBox *groupWeekPeriod;
	TLabel *lbStartWeek;
	TComboBox *cbStartWeek;
	TLabel *lbEndWeek;
	TComboBox *cbEndWeek;
	TLabel *lbRefHint1;
	TComboBox *cbRefActIndex;
	TLabel *lbRefHint2;
	TScrollBox *scrAreas;
	TImage *imageAreas;
	TMenuItem *N10;
	TMenuItem *mnuSave;
	TMenuItem *mnuOpen;
	TSaveDialog *systemSaveDialog;
	TOpenDialog *systemOpenDialog;
	TMenuItem *mnuSelectAllSuccessed;
	TMenuItem *mnuSelectAllFailed;
	TMenuItem *N11;
	TMenuItem *mnuAutoWriteClockSetup;
	TMenuItem *mnuImportV3;
	TVirtualStringTree *vt1;
	TScrollBox *scrollboxTime;
	TSpeedButton *btnYellow_Back;
	TMenuItem *mnuReTxSetup;
	TMenuItem *mnuCommConfig;
	TSpeedButton *btnCommConfig;
	TLabel *lbPlayTimeAll;
	TLabel *lbAllDisplayTime;
	TMenuItem *mnuParkDigit;
	TTimer *timerParkDigit;
	TMenuItem *mnuCardType;
	TMenuItem *mnuFlashTest;
	TTimer *timerFlashTest;
	TTimer *Timer1;
	TSpeedButton *btnTest;
	TMenuItem *mnuTxWeatherWarnning;
	TMenuItem *mnuRootPath;
	TMenuItem *mnuCacelWarnning;
	TTimer *timer_CancelWarnning;
	TIdUDPServer *udpWarnning;
	TMenuItem *mnuDownloadDotFont;
	TTimer *timerDownloadDotFont;
	TMenuItem *mnuCheckResult;
	TMenuItem *N12;
	TMenuItem *mnuQueryCheckResult;
	TTimer *timer_CheckResult;
	TMenuItem *N13;
	TMenuItem *mnuShrink;
	TMenuItem *N14;
	TMenuItem *mnuActDefine;
	TSpeedButton *btnBlue;
	TSpeedButton *btnPink;
	TSpeedButton *btnAqua;
	TSpeedButton *btnWhite;
	TSpeedButton *btnBlue_Back;
	TSpeedButton *btnPink_Back;
	TSpeedButton *btnAqua_Back;
	TSpeedButton *btnWhite_Back;
	TMenuItem *mnuEraseAllAct;
	TTimer *timerClearAllActs;
	TMenuItem *mnuCircleClock;
	TTimer *Timer_CheckDog;
	TMenuItem *N15;
	TMenuItem *mnuRamText;
	TImage *Image1;
	TSpeedButton *btnGps;
	TTimer *timerQueryPosition;
	TMenuItem *mnuAdjustLightness;
	TTimer *timerAdjustLightness;
	TMenuItem *mnuBrightnessSetup;
	TTimer *timerBrightnessSetup;
	TTimer *timerWebData;
	TIdHTTP *IdHTTP1;
	TTimer *timerWeather;
	TMenuItem *mnuTxLedDigit;
	TTimer *timerLedDigit;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnLoadPicureClick(TObject *Sender);
	void __fastcall timerTxActTimer(TObject *Sender);
	void __fastcall btnFontClick(TObject *Sender);
	void __fastcall btnAlignClick(TObject *Sender);
	void __fastcall btnAlignVertCenterClick(TObject *Sender);
	void __fastcall btnRedClick(TObject *Sender);
	void __fastcall btnGreenClick(TObject *Sender);
	void __fastcall btnYellowClick(TObject *Sender);
	void __fastcall btnBlackClick(TObject *Sender);
	void __fastcall btnRed_BackClick(TObject *Sender);
	void __fastcall btnGreen_BackClick(TObject *Sender);
	void __fastcall btnWhite_BackClick(TObject *Sender);
	void __fastcall btnBlack_BackClick(TObject *Sender);
	void __fastcall mnuCardConfigClick(TObject *Sender);
	void __fastcall mnuAdminPasswordClick(TObject *Sender);
	void __fastcall mnuCardListClick(TObject *Sender);
	void __fastcall panelScreenResize(TObject *Sender);
	void __fastcall comboAreaChange(TObject *Sender);
	void __fastcall mnuSendActClick(TObject *Sender);
	void __fastcall comboEditActChange(TObject *Sender);
	void __fastcall mnuSelectAllActClick(TObject *Sender);
	void __fastcall mnuCancelAllActClick(TObject *Sender);
	void __fastcall mnuSelectAllCardClick(TObject *Sender);
	void __fastcall mnuCancelAllCardClick(TObject *Sender);
	void __fastcall editActDescChange(TObject *Sender);
	void __fastcall mnuAboutClick(TObject *Sender);
	void __fastcall listActsSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
	void __fastcall mnuUpdateProgClick(TObject *Sender);
	void __fastcall timer_UpdateProgTimer(TObject *Sender);
	void __fastcall mnuCheckCardOnlineClick(TObject *Sender);
	void __fastcall timerCheckOnlineTimer(TObject *Sender);
	void __fastcall mnuAdjustCardClockClick(TObject *Sender);
	void __fastcall timerWriteClockTimer(TObject *Sender);
	void __fastcall mnuReadCardClockClick(TObject *Sender);
	void __fastcall timerReadClockTimer(TObject *Sender);
	void __fastcall mnuManualCloseScreenClick(TObject *Sender);
	void __fastcall timer_ExecuteTimer(TObject *Sender);
	void __fastcall mnuSearchCardClick(TObject *Sender);
	void __fastcall mnuSearchActClick(TObject *Sender);
	void __fastcall btnCalcDisplayTimeClick(TObject *Sender);
	void __fastcall mnuDeleteActClick(TObject *Sender);
	void __fastcall editPicPosChange(TObject *Sender);
	void __fastcall mnuPreviewCurrentActClick(TObject *Sender);
	void __fastcall scrollboxEditResize(TObject *Sender);
	void __fastcall mnuPreviewAllActClick(TObject *Sender);
	void __fastcall mnuDeletePictureClick(TObject *Sender);
	void __fastcall mnuDeleteTextClick(TObject *Sender);
	void __fastcall mnuSetTextActiveClick(TObject *Sender);
	void __fastcall mnuEditClick(TObject *Sender);
	void __fastcall mnuSetTextNormalClick(TObject *Sender);
	void __fastcall vtGetNodeDataSize(TBaseVirtualTree *Sender, int &NodeDataSize);
	void __fastcall vtGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText);
	void __fastcall vtInitNode(TBaseVirtualTree *Sender, PVirtualNode ParentNode,
          PVirtualNode Node, TVirtualNodeInitStates &InitialStates);
	void __fastcall vtGetImageIndex(TBaseVirtualTree *Sender, PVirtualNode Node,
          TVTImageKind Kind, TColumnIndex Column, bool &Ghosted,
		  int &ImageIndex);
	void __fastcall vtPaintText(TBaseVirtualTree *Sender,
          const TCanvas *TargetCanvas, PVirtualNode Node, TColumnIndex Column,
          TVSTTextType TextType);
	void __fastcall btnFindNextCardClick(TObject *Sender);
	void __fastcall btnFindNextActClick(TObject *Sender);
	void __fastcall mnuSaveActFileClick(TObject *Sender);
	void __fastcall mnuOpenActFileClick(TObject *Sender);
	void __fastcall mnuViewFailedTxActClick(TObject *Sender);
	void __fastcall mnuManualReTxActClick(TObject *Sender);
	void __fastcall timer_AutoWaitReTxActTimer(TObject *Sender);
	void __fastcall mnuQueryTxLogsClick(TObject *Sender);
	void __fastcall mnuChineseClick(TObject *Sender);
	void __fastcall mnuEnglishClick(TObject *Sender);
	void __fastcall vtFocusChanged(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column);
	void __fastcall mnuAutoCheckActChangeClick(TObject *Sender);
	void __fastcall timerCheckDynamicActTimer(TObject *Sender);
	void __fastcall mnuManualOpenScreenClick(TObject *Sender);
	void __fastcall timerAutoWriteClockTimer(TObject *Sender);
	void __fastcall mnuSaveClick(TObject *Sender);
	void __fastcall mnuOpenClick(TObject *Sender);
	void __fastcall mnuSelectAllSuccessedClick(TObject *Sender);
	void __fastcall mnuSelectAllFailedClick(TObject *Sender);
	void __fastcall mnuAutoWriteClockSetupClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall mnuImportV3Click(TObject *Sender);
	void __fastcall vt1GetNodeDataSize(TBaseVirtualTree *Sender, int &NodeDataSize);
	void __fastcall vt1InitNode(TBaseVirtualTree *Sender, PVirtualNode ParentNode, PVirtualNode Node,
          TVirtualNodeInitStates &InitialStates);
	void __fastcall vt1LoadNode(TBaseVirtualTree *Sender, PVirtualNode Node, TStream *Stream);
	void __fastcall btnBlueClick(TObject *Sender);
	void __fastcall btnWhiteClick(TObject *Sender);
	void __fastcall btnAquaClick(TObject *Sender);
	void __fastcall btnBlue_BackClick(TObject *Sender);
	void __fastcall btnPink_BackClick(TObject *Sender);
	void __fastcall btnAqua_BackClick(TObject *Sender);
	void __fastcall btnPinkClick(TObject *Sender);
	void __fastcall btnYellow_BackClick(TObject *Sender);
	void __fastcall mnuReTxSetupClick(TObject *Sender);
	void __fastcall mnuCommConfigClick(TObject *Sender);
	void __fastcall mnuParkDigitClick(TObject *Sender);
	void __fastcall timerParkDigitTimer(TObject *Sender);
	void __fastcall mnuCardTypeClick(TObject *Sender);
	void __fastcall mnuFlashTestClick(TObject *Sender);
	void __fastcall timerFlashTestTimer(TObject *Sender);
	void __fastcall btnTestClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall btnTxWeatherWarnningClick(TObject *Sender);
	void __fastcall mnuRootPathClick(TObject *Sender);
	void __fastcall mnuCacelWarnningClick(TObject *Sender);
	void __fastcall timer_CancelWarnningTimer(TObject *Sender);
	void __fastcall udpWarnningUDPRead(TIdUDPListenerThread *AThread, TBytes AData,
          TIdSocketHandle *ABinding);
	void __fastcall mnuDownloadDotFontClick(TObject *Sender);
	void __fastcall timerDownloadDotFontTimer(TObject *Sender);
	void __fastcall mnuCheckResultClick(TObject *Sender);
	void __fastcall mnuQueryCheckResultClick(TObject *Sender);
	void __fastcall timer_CheckResultTimer(TObject *Sender);
	void __fastcall mnuShrinkClick(TObject *Sender);
	void __fastcall mnuActDefineClick(TObject *Sender);
	void __fastcall mnuEraseAllActClick(TObject *Sender);
	void __fastcall timerClearAllActsTimer(TObject *Sender);
	void __fastcall mnuCircleClockClick(TObject *Sender);
	void __fastcall Timer_CheckDogTimer(TObject *Sender);
	void __fastcall mnuRamTextClick(TObject *Sender);
	void __fastcall btnGpsClick(TObject *Sender);
	void __fastcall timerQueryPositionTimer(TObject *Sender);
	void __fastcall mnuAdjustLightnessClick(TObject *Sender);
	void __fastcall timerAdjustLightnessTimer(TObject *Sender);
	void __fastcall mnuBrightnessSetupClick(TObject *Sender);
	void __fastcall timerBrightnessSetupTimer(TObject *Sender);
	void __fastcall timerWebDataTimer(TObject *Sender);
	void __fastcall listActsChange(TObject *Sender, TListItem *Item, TItemChange Change);
	void __fastcall timerWeatherTimer(TObject *Sender);
	void __fastcall mnuTxLedDigitClick(TObject *Sender);
	void __fastcall timerLedDigitTimer(TObject *Sender);








private:	// User declarations
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
	__fastcall ~TfrmMain();

	TwndTextEdit *wndTextEdit;

	void OnRxDataPackge(char* szDtuCode, BYTE* p, DWORD dwLen);
	BOOL FindOkTag(BYTE* p, DWORD dwLen);
	void CreateTextImage(char* s);
	UnicodeString m_sText;
	void ShowTextPicture(int dx, int dy, int cx, int cy, int sx, int sy);
	void ClearTextPicture(int dx, int dy, int cx, int cy);
	void CreateTxData();
	void OnRxStatus(char* szDtuCode, BYTE ucStatus);
	void OnRxWarnningPackge(char* szDtuCode);
	unsigned int m_nWarnningMessageId;
	BOOL m_bPreview;
	BYTE CalcChecksum(int nLen, BYTE* p);
	CSerialPort m_ComPort;
	bool m_bDirty;
	int m_nActIndex;
	void UpdateActVol();
	void SaveDtuAct(CDtu* dtu, int nActIndex);
	void SaveAct();
	void EndTxAct();
	void CreateActImage();
	bool WaitForCmdEcho(BYTE ucCmd, CDtu* dtu, int nDataLen);
	bool WaitForCmdEcho_HEB(BYTE ucCmd, CDtu* dtu, int nDataLen);
	void SendData_UDP(CDtu* dtu, int nLen, BYTE* p);
	void SendData_UDP_Yuepoch(CDtu* dtu, int nLen, BYTE* p);
	void SendData_UDP_JTX(CDtu* dtu, int nLen, BYTE* p);
	void ReceiveUdpData();
	void ReceiveUdpStatusPack();
	void SendCommand_WriteFlash(CDtu* dtu);
	void SendCommand_WriteFlash_Normal(CDtu* dtu);
	void SendCommand_WriteFlash_HEB(CDtu* dtu);
	void SendCommand_UpdateProg(CDtu* dtu);
	void SendCommand_UpdateProg_Normal(CDtu* dtu);
	void SendCommand_UpdateProg_HEB(CDtu* dtu);
	void SendCommand_CheckOnline(CDtu* dtu);
	void SendCommand_CheckOnline_Normal(CDtu* dtu);
	void SendCommand_CheckOnline_HEB(CDtu* dtu);
	void SendCommand_WriteClock(CDtu* dtu);
	void SendCommand_WriteClock_Normal(CDtu* dtu);
	void SendCommand_WriteClock_HEB(CDtu* dtu);
	void SendCommand_ReadClock(CDtu* dtu);
	void SendCommand_ReadClock_Normal(CDtu* dtu);
	void SendCommand_ReadClock_HEB(CDtu* dtu);
	void SendCommand_OnOffScreen(BYTE ucCmd, CDtu* dtu);
	void SendCommand_OnOffScreen_Normal(BYTE ucCmd, CDtu* dtu);
	void SendCommand_OnOffScreen_HEB(BYTE ucCmd, CDtu* dtu);
	void SendCommand_RC(CDtu* dtu);
	bool VerifyAct(CDtu* dtu);

	///////////////////////////////
	DWORD GetFlashImageLen_ScrollLeft(int nAreaIndex);
	DWORD GetFlashImageLen_MoveLeft(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_MoveLeft(int nAreaIndex);
	DWORD GetFlashImageLen_MoveRight(int nAreaIndex);
	DWORD GetFlashImageLen_ScrollRight(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_MoveRight(int nAreaIndex);
	DWORD GetFlashImageLen_ScrollUp(int nAreaIndex);
	DWORD GetFlashImageLen_MoveUp(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_MoveUp(int nAreaIndex);
	DWORD GetFlashImageLen_ScrollDown(int nAreaIndex);
	DWORD GetFlashImageLen_MoveDown(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_MoveDown(int nAreaIndex);
	DWORD GetFlashImageLen_LeftToRightSpread(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_LeftToRightErase(int nAreaIndex);
	DWORD GetFlashImageLen_RightToLeftSpread(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_RightToLeftErase(int nAreaIndex);
	DWORD GetFlashImageLen_TopToBottomSpread(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_TopToBottomErase(int nAreaIndex);
	DWORD GetFlashImageLen_BottomToTopSpread(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_BottomToTopErase(int nAreaIndex);
	DWORD GetFlashImageLen_H_ToCenterSpread(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_H_ToCenterErase(int nAreaIndex);
	DWORD GetFlashImageLen_H_FromCenterSpread(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_H_FromCenterErase(int nAreaIndex);
	DWORD GetFlashImageLen_H_Shutter(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_H_Shutter(int nAreaIndex);
	DWORD GetFlashImageLen_V_Shutter(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_V_Shutter(int nAreaIndex);
	DWORD GetFlashImageLen_V_ToCenterSpread(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_V_ToCenterErase(int nAreaIndex);
	DWORD GetFlashImageLen_V_FromCenterSpread(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_V_FromCenterErase(int nAreaIndex);
	DWORD GetFlashImageLen_Direct(int nAreaIndex);
	DWORD GetFlashImageLen_Flash(int nAreaIndex);
	DWORD GetFlashImageLen_Exit_Direct(int nAreaIndex);
	BYTE GetBit(BYTE c, int x, int y, DWORD dwColorMask);
	BYTE* CreateFlashImage_Direct(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Direct_YL2010C(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p);
	BYTE* CreateFlashImage_Flash(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_Direct(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_ScrollLeft(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_MoveLeft(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_MoveLeft(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_ScrollRight(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_MoveRight(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_MoveRight(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_MoveUp(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_ScrollUp(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_MoveUp(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_ScrollDown(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_MoveDown(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_MoveDown(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_LeftToRightSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_LeftToRightErase(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_RightToLeftSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_RightToLeftErase(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_TopToBottomSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_TopToBottomErase(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_BottomToTopSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_BottomToTopErase(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_H_ToCenterSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_H_ToCenterErase(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_H_FromCenterSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_H_FromCenterErase(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_V_ToCenterSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_V_ToCenterErase(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_V_FromCenterSpread(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_V_FromCenterErase(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_H_Shutter(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_H_ShutterErase(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_V_Shutter(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* pRed);
	BYTE* CreateFlashImage_Exit_V_ShutterErase(int nActIndex, int nAreaIndex, BYTE* pRed);
	BYTE* CreateFlashImage_Flash_Dark(int nActIndex, int nAreaIndex, BYTE* p);
	DWORD GetDisplayTime_Direct();
	DWORD GetDisplayTime_Exit_Direct();
	DWORD GetDisplayTime_ScrollLeft();
	DWORD GetDisplayTime_MoveLeft();
	DWORD GetDisplayTime_Exit_MoveLeft();
	DWORD GetDisplayTime_ScrollRight();
	DWORD GetDisplayTime_MoveRight();
	DWORD GetDisplayTime_Exit_MoveRight();
	DWORD GetDisplayTime_ScrollUp();
	DWORD GetDisplayTime_MoveUp();
	DWORD GetDisplayTime_Exit_MoveUp();
	DWORD GetDisplayTime_ScrollDown();
	DWORD GetDisplayTime_MoveDown();
	DWORD GetDisplayTime_Exit_MoveDown();
	DWORD GetDisplayTime_LeftToRightSpread();
	DWORD GetDisplayTime_Exit_LeftToRightErase();
	DWORD GetDisplayTime_RightToLeftSpread();
	DWORD GetDisplayTime_Exit_RightToLeftErase();
	DWORD GetDisplayTime_TopToBottomSpread();
	DWORD GetDisplayTime_Exit_TopToBottomErase();
	DWORD GetDisplayTime_BottomToTopSpread();
	DWORD GetDisplayTime_Exit_BottomToTopErase();
	DWORD GetDisplayTime_H_ToCenterSpread();
	DWORD GetDisplayTime_Exit_H_ToCenterErase();
	DWORD GetDisplayTime_H_FromCenterSpread();
	DWORD GetDisplayTime_Exit_H_FromCenterErase();
	DWORD GetDisplayTime_V_ToCenterSpread();
	DWORD GetDisplayTime_Exit_V_ToCenterErase();
	DWORD GetDisplayTime_V_FromCenterSpread();
	DWORD GetDisplayTime_Exit_V_FromCenterErase();
	DWORD GetDisplayTime_H_Shutter();
	DWORD GetDisplayTime_Exit_H_ShutterErase();
	DWORD GetDisplayTime_V_Shutter();
	DWORD GetDisplayTime_Exit_V_ShutterErase();
	DWORD CalcPlayTime(int nActIndex);

	UnicodeString m_sSearchCardId;
	AnsiString m_sSearchCardCode;
	UnicodeString m_sSearchCardDesc;
	UnicodeString m_sSearchActText;
	UnicodeString m_sSearchActDesc;

	TPicture* m_picAct[MAX_ACT_COUNT];

	void PreviewAct(int nActIndex);
	void ProcPreview(int nActIndex, int nAreaIndex);
	void Preview_Direct(int nActIndex, int nAreaIndex);
	void Preview_Exit_Direct(int nActIndex, int nAreaIndex);
	void Preview_Exit_Null(int nActIndex, int nAreaIndex);
	void Preview_ScrollLeft(int nActIndex, int nAreaIndex);
	void Preview_MoveLeft(int nActIndex, int nAreaIndex);
	void Preview_Exit_MoveLeft(int nActIndex, int nAreaIndex);
	void Preview_ScrollRight(int nActIndex, int nAreaIndex);
	void Preview_Exit_MoveRight(int nActIndex, int nAreaIndex);
	void Preview_ScrollUp(int nActIndex, int nAreaIndex);
	void Preview_ScrollDown(int nActIndex, int nAreaIndex);
	void Preview_Exit_MoveUp(int nActIndex, int nAreaIndex);
	void Preview_Exit_MoveDown(int nActIndex, int nAreaIndex);
	void Preview_MoveRight(int nActIndex, int nAreaIndex);
	void Preview_MoveUp(int nActIndex, int nAreaIndex);
	void Preview_MoveDown(int nActIndex, int nAreaIndex);
	void Preview_LeftToRightSpread(int nActIndex, int nAreaIndex);
	void Preview_RightToLeftSpread(int nActIndex, int nAreaIndex);
	void Preview_TopToBottomSpread(int nActIndex, int nAreaIndex);
	void Preview_BottomToTopSpread(int nActIndex, int nAreaIndex);
	void Preview_H_ToCenterSpread(int nActIndex, int nAreaIndex);
	void Preview_H_FromCenterSpread(int nActIndex, int nAreaIndex);
	void Preview_V_ToCenterSpread(int nActIndex, int nAreaIndex);
	void Preview_V_FromCenterSpread(int nActIndex, int nAreaIndex);
	void Preview_H_Shutter(int nActIndex, int nAreaIndex);
	void Preview_V_Shutter(int nActIndex, int nAreaIndex);
	void Preview_Exit_LeftToRightErase(int nActIndex, int nAreaIndex);
	void Preview_Exit_RightToLeftErase(int nActIndex, int nAreaIndex);
	void Preview_Exit_TopToBottomErase(int nActIndex, int nAreaIndex);
	void Preview_Exit_BottomToTopErase(int nActIndex, int nAreaIndex);
	void Preview_Exit_H_ToCenterErase(int nActIndex, int nAreaIndex);
	void Preview_Exit_H_FromCenterErase(int nActIndex, int nAreaIndex);
	void Preview_Exit_V_ToCenterErase(int nActIndex, int nAreaIndex);
	void Preview_Exit_V_FromCenterErase(int nActIndex, int nAreaIndex);
	void Preview_Exit_H_ShutterErase(int nActIndex, int nAreaIndex);
	void Preview_Exit_V_ShutterErase(int nActIndex, int nAreaIndex);

	int m_nPreviewXScreenCount[MAX_ACT_COUNT];
	int m_nPreviewYScreenCount[MAX_ACT_COUNT];
	int m_nPreviewXScreen[MAX_ACT_COUNT];
	int m_nPreviewYScreen[MAX_ACT_COUNT];
	int m_nPreviewPos[MAX_ACT_COUNT];
	int m_nPreviewStaus[MAX_ACT_COUNT];
	int m_nActTime[MAX_ACT_COUNT];

	void PreviewAllAct();
	int FindNextAct(int nActIndex, int nAreaIndex);
	DWORD GetActiveFlashImageLen_Direct(int nAreaIndex);
	BYTE* CreateActiveFlashImage_Direct(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p);
	BYTE* CreateActiveFlashImage_Direct_1(int nActIndex, int nAreaIndex, int xStart, int yStart, BYTE* p, int nShowTime);
	BYTE* CreateActiveFlashImage(int nActIndex, wchar_t wcActiveType, wchar_t wcColor, BYTE* p);
	bool IsCurrentActActive();
	void MakeActiveTextColor(wchar_t wcColor);

	void LoadAllCard();
	void LoadCardTree(UnicodeString sSection, PVirtualNode NodeRoot, CDtu* dtuParent);
	int GetCheckedCardCount();
	void SaveChildDtuConfig(PVirtualNode NodeRoot);
	PVirtualNode GetFirstCheckedCard();
	PVirtualNode GetNextCheckedCard(PVirtualNode Node);
	void StartOperation();
	void EndOperation();
	void FindNextCard();
	void FindNextAct();
	bool OpenCommPort(int nTxAct, UnicodeString sTitle);
	int InitActTxStatus(BOOLEAN* pbActTx);
	void SaveCardTxStatus();
	bool CheckIfAnyActTxFailed();
	int GetReTxCardCount();
	void InitReTxCardStatus(BOOL bLog, UnicodeString s);
	int m_nWaitReTxTimerCount;
	void AutoReTxAct();

	TLog log;
	int m_nLanguage;
	void ChangeLanguage(int nLanguge);
	void InitActInputWindow();
	PVirtualNode m_Node;
	CDtu* m_dtu;
	void UpdateCardClock(CDtu* dtu);
	bool m_bStopUpdateProg;
	int GetCardCount_SameServerAndPort();
	int m_nLastAutoWriteClockMinute;
	int m_nAutoWriteClockHourCount;

	//动态更新节目
	UnicodeString m_sDynamicActText;
	void LoadDynamicActText(CDtu* dtu);
	int GetTextWidth(TCanvas* Canvas, bool bDotFont, int nDotFontSize, UnicodeString ws);
	void DrawChar(TCanvas* Canvas, int x, int y, wchar_t wc);

	void AutoSizeText(CDtu* dtu, int nActIndex);
	void DrawAreaImage();
	DWORD AdjustProcessToken(HANDLE hProcess, LPCTSTR lpPrivilegeName);

	void WriteGroupList(PVirtualNode pParentNode);
	TLabel* m_lbPeriod[ACT_TIME_PERIOD_COUNT_24];
	TLabel* m_lbTo[ACT_TIME_PERIOD_COUNT_24];
	TDateTimePicker* m_timeStart[ACT_TIME_PERIOD_COUNT_24];
	TDateTimePicker* m_timeEnd[ACT_TIME_PERIOD_COUNT_24];
	DWORD GetColorCount();
	int GetImageType();
	DWORD CalcAllPlayTime();
	bool m_bReTx;

	bool m_bParkDigit_DoubleColor;
	int m_nParkValue[8];
	TColor m_clParkColor[8];
	int m_nParkDigit_Lightvalue;
	void SendCommand_WriteParkValue(CDtu* dtu);

	void UpdateCaption();
	void CheckUniqueDtuName(CDtu* dtuCheck, PVirtualNode NodeCheck);
	void Encrypt(CDtu* dtu, int nLen, BYTE* p);

	BYTE m_ucTestData[3];
	void SendCommand_WriteFlashTestData(CDtu* dtu);
	void SendCommand_WriteFlashTestData_Normal(CDtu* dtu);
	void SendCommand_WriteFlashTestData_HEB(CDtu* dtu);

	void SendCommand_CancelWarnning(CDtu* dtu);
	void CreateAutoTimeHintImage();
	BYTE* CreateAutoHintFlashImage(BYTE* p);
	DWORD GetDisplayTime_AutoTimeHint();
	UnicodeString GetActIndexText(int nIndex);
	void udpWarnningUDPRead_HEB(TBytes AData);
	void udpWarnningUDPRead_Normal(TBytes AData);
	void SendCommand_AckWarnning(CDtu* dtu);
	void SendCommand_AckWarnning_HEB(CDtu* dtu);
	void MakeUnicodeTable();
	WORD* m_pucUnicodeTable;
	void SendCommand_WriteDotFont(CDtu* dtu);

	void SendCommand_QueryCheckResult(CDtu* dtu);
	void SendCommand_QueryCheckResult_Normal(CDtu* dtu);
	void SendCommand_QueryCheckResult_HEB(CDtu* dtu);
	void UpdateLedSignStatus(CDtu* dtu);

	TStringList* m_lstAllRootNodeName;
	LIST_PTR m_lstDtu;
	void LoadAllRootNodeName();
	void LoadAllCardName();
	void LoadCardNameTree(UnicodeString sSection);
	void ClearDtuList();
	void ClearAllTxOkStatus();
	void ReadFromFile(UnicodeString sFileName, int nPos, BYTE* buf, int nLen);
	void SendCommand_ReadWriteChipId(CDtu* dtu);
	void SendCommand_ReadWriteChipId_Normal(CDtu* dtu);
	void SendCommand_ReadWriteChipId_HEB(CDtu* dtu);
	void GetPseudoIp(UnicodeString wsSimCode, BYTE* pucIp);
	BYTE CalcCheckSum_JTX(BYTE* p, int nLen);
	void SendCommand_ClearAllActs(CDtu* dtu);
#ifdef AUTO_SIZE_TEXT
	void CreateActTxData(CDtu* dtu, int nActIndex);
#endif
	void CreateTxData1();

#ifdef TWO_SOFT_DOG
	bool TfrmMain::Dog_Check(int nIndex);
#endif
	bool bDogOk;
	BYTE* m_pucGS9025DotFont;
	void MakeGS9025TextActData();
	void MakeGS9025RamTextActData();
	void SendCommand_TextAct(CDtu* dtu);
	int MakeGS9025NullTextActData(int nActIndex, BYTE* p);
	void Create9025DotFont(BYTE* p);
	void Create9023DotFont(BYTE* p);
	void SendCommand_QueryPosition(CDtu* dtu);
	void UpdatePositionInfo(CDtu* dtu);
	void SendCommand_AdjustLightness(CDtu* dtu);
	int m_nTempLightValue;
	int m_nTempLightMinutes;
	void SendCommand_WriteBrightnessConfig(CDtu* dtu);
#ifdef XINJIANG_AUTO_WEB_DATA
	UnicodeString m_sWebData;
	UnicodeString m_sOldWebData;
	wchar_t* GetWebText(wchar_t* s, UnicodeString& sResult);
#endif
	bool m_bParkDigit_Init;
	bool m_bSelected[WEATHER_ICON_COUNT];
	void SendCommand_WeatherIco(CDtu* dtu);
	BYTE m_ucLedDigit[4];
	void SendCommand_WriteLedDigit(CDtu* dtu);
	UnicodeString m_sLedDigit;
};

#ifdef XINJIANG_AUTO_WEB_DATA
class TWebDataThread: public TThread
{
public:
	__fastcall TWebDataThread(void);
private:
	virtual void __fastcall Execute(void);
};
#endif
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
