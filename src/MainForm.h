//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include "CustomerFrameView.h"
#include "LEDBoardFrameView.h"
//---------------------------------------------------------------------------
class TMasterForm : public TForm
{
__published:	// IDE-managed Components
  TSplitter *MainVSplitter;
  TTreeView *UserTreeView;
  TStatusBar *StatusBar;
  TMainMenu *MainMenu;
  TMenuItem *N1;
  TMenuItem *N2;
  TMenuItem *N3;
  TMenuItem *N4;
  TMenuItem *N5;
  TMenuItem *N6;
  TADOQuery *ADOQuery;
  TPageControl *LeftPageControl;
  TTabSheet *CustomerTabSheet;
  TTabSheet *LEDBoardTabSheet;
  TTreeView *LEDBoardTreeView;
  TCustomerFrame *CustomerFrame;
  TLEDBoardFrame *LEDBoardFrame;
  TPanel *RightPanel;
  TPanel *RightBottomPanel;
  TSplitter *RightHSplitter;
  TLabel *SendTaskLabel;
  TButton *SendTaskAddButton;
  TButton *SendTaskEditButton;
  TButton *SendTaskDelButton;
  TPageControl *SendTaskPageControl;
  TTabSheet *SendingTaskTabSheet;
  TListView *SendingTaskListView;
  TTabSheet *SendingLogTabSheet;
  TListView *SendingLogListView;
  TPanel *SendingTaskTitlePanel;
  void __fastcall UserTreeViewChange(TObject *Sender, TTreeNode *Node);
  void __fastcall LeftPageControlChange(TObject *Sender);
  void __fastcall CustomerTabSheetShow(TObject *Sender);
  void __fastcall LEDBoardTabSheetShow(TObject *Sender);
private:	// User declarations
  // �����ݿ���Ϣ���ͼ�¼�ӷ���״̬��Ϊ����״̬
  BOOL UpdateMsgInBoardStatus(int BoardID, BYTE ucSlot, int status);
public:		// User declarations
  __fastcall TMasterForm(TComponent* Owner);
  void UpdateSendingTaskStatus(typeTask *pTask);
  // ���ؿͻ�������ѡ��ָ���ͻ�����CustIDΪ0��ѡ�и�
  void LoadUserTreeView(int CustID);
};
//---------------------------------------------------------------------------
extern PACKAGE TMasterForm *MasterForm;
//---------------------------------------------------------------------------
#endif
