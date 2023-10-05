//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SysConfig.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CustomerFrameView"
#pragma link "LEDBoardFrameView"
#pragma resource "*.dfm"
TMasterForm *MasterForm;
//---------------------------------------------------------------------------
__fastcall TMasterForm::TMasterForm(TComponent* Owner)
  : TForm(Owner)
{
  ADOQuery->ConnectionString = DBCONNSTR;
}
//---------------------------------------------------------------------------

void __fastcall TMasterForm::LeftPageControlChange(TObject *Sender)
{
  TPageControl * pPageCtrl = (TPageControl *)Sender;
  if (pPageCtrl->ActivePageIndex == 0)
  {
    CustomerFrame->Visible = true;
    LEDBoardFrame->Visible = false;
  }
  else
  {
    CustomerFrame->Visible = false;
    LEDBoardFrame->Visible = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMasterForm::CustomerTabSheetShow(TObject *Sender)
{
  LoadUserTreeView(0);
}
//---------------------------------------------------------------------------
void TMasterForm::LoadUserTreeView(int CustID)
{
  unsigned int UserID;
  AnsiString UserName, UserDesc;
  TTreeNode *Root, *UserNode, *SelNode;

  UserTreeView->Items->Clear();
  UserTreeView->Items->BeginUpdate();

  Root = UserTreeView->Items->Add(NULL, "ȫ���ͻ�");
  Root->ImageIndex = 0;
  Root->SelectedIndex = 0;
  Root->StateIndex = -1;
  Root->Data = NULL;
  if (CustID == 0)
    SelNode = Root;

  // ��ȡ��Ծ�ͻ��б�
  try {
    ADOQuery->Close();
    ADOQuery->SQL->Clear();
    ADOQuery->SQL->Add("SELECT ID, NAME ");
    ADOQuery->SQL->Add("FROM CUSTOMER WHERE STATUS=0");
    ADOQuery->Open();
    for (int i = 0; i < ADOQuery->RecordCount; i++)
    {
      UserID = ADOQuery->Fields->Fields[0]->AsInteger;
      UserName = ADOQuery->Fields->Fields[1]->AsString;

      UserNode = UserTreeView->Items->AddChild(Root, UserName.c_str());
      UserNode->ImageIndex = 1;
      UserNode->SelectedIndex = 2;
      UserNode->StateIndex = -1;
      UserNode->Data = (void *)UserID;
      if (UserID == CustID)
        SelNode = UserNode;

      ADOQuery->Next();
    }
    ADOQuery->Close();
  }
  catch(Exception &ex)
  {
    if (ADOQuery != NULL)
    {
      ADOQuery->Close();
    }
  }

  UserTreeView->Items->EndUpdate();
  UserTreeView->Items->GetFirstNode()->Expand(false);
  UserTreeView->SetFocus();
  SelNode->Selected = true;
}
//---------------------------------------------------------------------------
void __fastcall TMasterForm::LEDBoardTabSheetShow(TObject *Sender)
{
  unsigned int ID;
  AnsiString Name;
  TTreeNode *Root, *BoardGrpNode, *BoardNode;
  wchar_t tmp[10];

  LEDBoardTreeView->Items->Clear();
  LEDBoardTreeView->Items->BeginUpdate();

  Root = LEDBoardTreeView->Items->Add(NULL, "ȫ����ʾ��");
  Root->ImageIndex = 0;
  Root->SelectedIndex = 0;
  Root->StateIndex = -1;

  // ��ȡ��ʾ�����б�
  try {
    ADOQuery->Close();
    ADOQuery->SQL->Clear();
    ADOQuery->SQL->Add("SELECT ID, NAME ");
    ADOQuery->SQL->Add("FROM BOARDGROUP");
    ADOQuery->Open();
    for (int i = 0; i < ADOQuery->RecordCount; i++)
    {
      ID = ADOQuery->Fields->Fields[0]->AsInteger;
      Name = ADOQuery->Fields->Fields[1]->AsString;

      BoardGrpNode = LEDBoardTreeView->Items->AddChild(Root, Name.c_str());
      BoardGrpNode->ImageIndex = 1;
      BoardGrpNode->SelectedIndex = 2;
      BoardGrpNode->StateIndex = -1;
      BoardGrpNode->Data = (void *)ID;

      ADOQuery->Next();
    }

    // ����ÿ���������ʾ��
    BoardGrpNode = Root->getFirstChild();
    while (BoardGrpNode != NULL)
    {
      // ��ȡ���ڱ���ʾ�������ʾ���б�
      ADOQuery->Close();
      ADOQuery->SQL->Clear();
      ADOQuery->SQL->Add("SELECT LEDBOARD.ID, LEDBOARD.NAME ");
      ADOQuery->SQL->Add("FROM LEDBOARD, BOARDSINGROUPS WHERE ");
      ADOQuery->SQL->Add("(BOARDSINGROUPS.GROUPID = ");
      ADOQuery->SQL->Add(::_itow((int)BoardGrpNode->Data, tmp, 10));
      ADOQuery->SQL->Add(" AND BOARDSINGROUPS.BOARDID = LEDBOARD.ID)");
      ADOQuery->Open();
      for (int i = 0; i < ADOQuery->RecordCount; i++)
      {
        ID = ADOQuery->Fields->Fields[0]->AsInteger;
        Name = ADOQuery->Fields->Fields[1]->AsString;

        BoardNode = LEDBoardTreeView->Items->AddChild(BoardGrpNode, Name.c_str());
        BoardNode->ImageIndex = 1;
        BoardNode->SelectedIndex = 2;
        BoardNode->StateIndex = -1;
        BoardNode->Data = (void *)ID;

        ADOQuery->Next();
      }

      BoardGrpNode = Root->GetNextChild(BoardGrpNode);
    }

    ADOQuery->Close();
  }
  catch(Exception &ex)
  {
    if (ADOQuery != NULL)
    {
      ADOQuery->Close();
    }
  }

  LEDBoardTreeView->Items->EndUpdate();  

  LEDBoardTreeView->Items->GetFirstNode()->Expand(false);
  LEDBoardTreeView->SetFocus();
  Root->Selected = true;
}
//---------------------------------------------------------------------------
void __fastcall TMasterForm::UserTreeViewChange(TObject *Sender,
      TTreeNode *Node)
{
  unsigned int UserID;
  AnsiString Name;
  wchar_t tmp[10];

  UserID = (unsigned int)Node->Data;
  if (UserID != NULL)
  { // ѡ��ĳ���û�
    ADOQuery->Close();
    ADOQuery->SQL->Clear();
    ADOQuery->SQL->Add("SELECT NAME ");
    ADOQuery->SQL->Add("FROM CUSTOMER WHERE ID=");
    ADOQuery->SQL->Add(::_itow(UserID, tmp, 10));
    ADOQuery->Open();
    ADOQuery->First();
    Name = ADOQuery->Fields->Fields[0]->AsString;
    ADOQuery->Close();
    typeCust* c = new typeCust;
    c->CustID = UserID;
    c->Name = Name;
    CustomerFrame->ChangeCust(c);
    delete c;
  }
  else
  { // ѡ�зǿͻ��ڵ�
    CustomerFrame->ChangeCust(NULL);
  }
}
//---------------------------------------------------------------------------
void TMasterForm::UpdateSendingTaskStatus(typeTask *pTask)
{
  for (int i = 0; i < SendingTaskListView->Items->Count; i++)
  {
    TListItem* item = SendingTaskListView->Items->Item[i];
    if (item->Data == NULL) // �ѷ��ͳɹ�������
      continue;

    typeTask * task = (typeTask *)(item->Data);
    if (task == pTask)
    {
      switch (task->State)
      {
      case TASK_STATE_R:
        item->SubItems->Strings[2] = "�ȴ�����";
        break;
      case TASK_STATE_W:
        item->SubItems->Strings[2] = "���ڷ���";
        break;
      case TASK_STATE_T:
        item->SubItems->Strings[2] = "����ʱ";
        UpdateMsgInBoardStatus(pTask->BoardID, pTask->ucSlot, MSG_PUB_STATE_OUTDATE);
        break;
      case TASK_STATE_S:
        item->SubItems->Strings[2] = "���ͳɹ�";
        UpdateMsgInBoardStatus(pTask->BoardID, pTask->ucSlot, MSG_PUB_STATE_SHOWING);
        item->Data = NULL;
        break;
      case TASK_STATE_F:
        item->SubItems->Strings[2] = "����ʧ��";
        UpdateMsgInBoardStatus(pTask->BoardID, pTask->ucSlot, MSG_PUB_STATE_OUTDATE);
        break;
      }
    }
  }
}
//---------------------------------------------------------------------------
BOOL TMasterForm::UpdateMsgInBoardStatus(int BoardID, BYTE ucSlot, int status)
{
  // �������ݿ��е���Ϣ����״̬
  AnsiString UpdateSQL = "UPDATE MSGSINBOARDS SET STATUS=";
  UpdateSQL += status;
  UpdateSQL += " WHERE BOARDID=";
  UpdateSQL += BoardID;
  UpdateSQL += " AND SLOT=";
  UpdateSQL += ucSlot;
  UpdateSQL += " AND STATUS=";
  UpdateSQL += MSG_PUB_STATE_SENDING;
  try {
    ADOQuery->Close();
    ADOQuery->SQL->Clear();
    ADOQuery->SQL->Add(UpdateSQL);
    ADOQuery->ExecSQL();
    ADOQuery->Close();
  }
  catch(Exception &ex)
  {
    if (ADOQuery != NULL)
    {
      ADOQuery->Close();
      return FALSE;
    }
  }

  return TRUE;
}
//---------------------------------------------------------------------------
