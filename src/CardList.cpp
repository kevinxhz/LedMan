//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "CardList.h"
#include "SysConfig.h"
#include "AddCards.h"
#include <inifiles.hpp>
#include <registry.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

__fastcall TfrmCardList::TfrmCardList(TComponent* Owner)
	: TForm(Owner)
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "显示屏(控制卡)管理";
		btnNewGroup->Hint = "新建显示屏(控制卡)组";
		btnNewCard->Hint = "新建显示屏(控制卡)";
		btnDelete->Hint = "删除";
		vt->Header->Columns[0].Items[0]->Text = "名称/描述";
		vt->Header->Columns[0].Items[2]->Text = "网络通信编码";
		btnAddCards->Caption = "以选择的显示屏(控制卡)为模板批量加入其它显示屏(控制卡)";
	}
	else
	{
		Caption = "Organize LED Panels(Cards)";
		btnNewGroup->Hint = "Add a New Group";
		btnNewCard->Hint = "Add a New LED Panel(Card)";
		btnDelete->Hint = "Delete";
		vt->Header->Columns[0].Items[0]->Text = "Name/Descriptions";
		vt->Header->Columns[0].Items[2]->Text = "Network ID";
		btnAddCards->Caption = "Use Selected One as Template to Add More";
	}
	m_bChanged = false;
}
//---------------------------------------------------------------------------

void TfrmCardList::LoadCardTree(UnicodeString sSection, PVirtualNode NodeRoot)
{
	TCustomIniFile* ini = new TRegistryIniFile(sSection);
	int nIndex = 0;

	for (;;)
	{
		UnicodeString sName = ini->ReadString("List", UnicodeString(nIndex), "");
		if (sName == "")
			break;

		PVirtualNode Node = vt->AddChild(NodeRoot);
		Node->CheckType = ctTriStateCheckBox;
		Node->CheckState = csUncheckedNormal;
		CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
		dtu->m_sName = sName;
		dtu->m_sSection = sSection + "\\LedPanels";
		dtu->LoadConfig();
		if (dtu->m_nNodeType == NODE_GROUP)
			LoadCardTree(dtu->m_sSection + "\\" + dtu->m_sName, Node);

		nIndex++;
	}

	delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::FormCreate(TObject *Sender)
{
	vt->NodeDataSize = sizeof(CDtu);

	PVirtualNode NodeRoot = vt->AddChild(NULL);
	CDtu* dtu = (CDtu*)(vt->GetNodeData(NodeRoot));
	dtu->m_nNodeType = NODE_ALL_CARD_ROOT;
	dtu->m_sName = sysConfig.m_sRootName;
	dtu->m_sSection = REG_ROOT_NAME;
	LoadCardTree(dtu->m_sSection + "\\" + dtu->m_sName, NodeRoot);

	vt->FullExpand(NodeRoot);
	vt->Selected[NodeRoot] = true;
	vt->FocusedNode = NodeRoot;
}
//---------------------------------------------------------------------------

void TfrmCardList::WriteGroupList(PVirtualNode pParentNode)
{
	CDtu* dtu = (CDtu*)(vt->GetNodeData(pParentNode));
	TCustomIniFile* ini = new TRegistryIniFile(dtu->m_sSection + "\\" + dtu->m_sName);
	int nIndex = 0;

	ini->EraseSection("List");
	PVirtualNode Node = vt->GetFirstChild(pParentNode);
	unsigned int nLevel = vt->GetNodeLevel(Node);
	while (Node != NULL)
	{
		dtu = (CDtu*)(vt->GetNodeData(Node));
		if (vt->GetNodeLevel(Node) == nLevel)
			ini->WriteString("List", UnicodeString(nIndex++), dtu->m_sName);
		if (Node == vt->GetLastChild(pParentNode))
			break;
		Node = vt->GetNext(Node, false);
	}

	delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::btnNewGroupClick(TObject *Sender)
{
	TVTNodeAttachMode attachMode = amAddChildLast;

	PVirtualNode pParentNode = vt->GetFirstSelected(false);
	if (pParentNode != NULL)
	{
		CDtu* dtu = (CDtu*)(vt->GetNodeData(pParentNode));
		if (dtu->m_nNodeType == NODE_CARD)
			attachMode = amInsertAfter;
	}
	else
		pParentNode = vt->GetFirstChild(NULL);

	PVirtualNode Node = vt->InsertNode(pParentNode, attachMode, NULL);

	CDtu* dtuParent = (CDtu*)(vt->GetNodeData(pParentNode));
	while (dtuParent->m_nNodeType == NODE_CARD)
	{
		pParentNode = pParentNode->Parent;
		dtuParent = (CDtu*)(vt->GetNodeData(pParentNode));
	}
	CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
	dtu->Create();
	dtu->m_nNodeType = NODE_GROUP;
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
		dtu->m_sName = "显示屏(控制卡)组" + UnicodeString(FindUniqueId());
	else
		dtu->m_sName = "Group of LED Panels(Cards)" + UnicodeString(FindUniqueId());
	dtu->m_sSection = dtuParent->m_sSection + "\\" + dtuParent->m_sName + "\\LedPanels";
	dtu->SaveConfig();
	WriteGroupList(pParentNode);
	vt->Selected[Node] = true;
	vt->FocusedNode = Node;
	vt->Refresh();
	m_bChanged = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::btnNewCardClick(TObject *Sender)
{
	TVTNodeAttachMode attachMode = amAddChildLast;

	PVirtualNode pParentNode = vt->GetFirstSelected(false);
	if (pParentNode != NULL)
	{
		CDtu* dtu = (CDtu*)(vt->GetNodeData(pParentNode));
		if (dtu->m_nNodeType == NODE_CARD)
			attachMode = amInsertAfter;
	}
	else
		pParentNode = vt->GetFirstChild(NULL);

	PVirtualNode Node = vt->InsertNode(pParentNode, attachMode, NULL);

	CDtu* dtuParent = (CDtu*)(vt->GetNodeData(pParentNode));
	while (dtuParent->m_nNodeType == NODE_CARD)
	{
		pParentNode = pParentNode->Parent;
		dtuParent = (CDtu*)(vt->GetNodeData(pParentNode));
	}
	CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
	dtu->Create();
	dtu->m_nNodeType = NODE_CARD;
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
		dtu->m_sName = "显示屏(控制卡)" + UnicodeString(FindUniqueId());
	else
		dtu->m_sName = "LED Panel(Card)" + UnicodeString(FindUniqueId());
	dtu->m_sSection = dtuParent->m_sSection + "\\" + dtuParent->m_sName + "\\LedPanels";
	dtu->SaveConfig();
	WriteGroupList(pParentNode);
	vt->Selected[Node] = true;
	vt->Expanded[Node] = true;
	vt->Expanded[pParentNode] = true;
	vt->FocusedNode = Node;
	vt->Refresh();
	m_bChanged = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::vtGetNodeDataSize(TBaseVirtualTree *Sender,
	  int &NodeDataSize)
{
	NodeDataSize = sizeof(CDtu);
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::vtGetText(TBaseVirtualTree *Sender,
	  PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
	  UnicodeString &CellText)
{
	CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
	if (Column == 0)
	{
		if (TextType == ttNormal)
			CellText = dtu->m_sName;
	}
	else if (Column == 2)
	{
		if (TextType == ttNormal)
		{
			if (dtu->m_nNodeType == NODE_CARD)
			{
				CellText = UnicodeString(dtu->m_sCode);
			}
			else
				CellText = "";
		}

	}
	else
		CellText = "";
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::vtPaintText(TBaseVirtualTree *Sender,
	  const TCanvas *TargetCanvas, PVirtualNode Node, TColumnIndex Column,
	  TVSTTextType TextType)
{
	CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
	if (dtu->m_nNodeType == NODE_GROUP)
		TargetCanvas->Font->Style = TargetCanvas->Font->Style << fsBold;
	else
		TargetCanvas->Font->Style = TargetCanvas->Font->Style >> fsBold;

	if (Node == Sender->HotNode)
		TargetCanvas->Font->Color = clRed;
	else
	{
		if (TextType == ttStatic)
			TargetCanvas->Font->Color = clBlue;
		else
			TargetCanvas->Font->Color = clWindowText;
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::vtInitNode(TBaseVirtualTree *Sender,
	  PVirtualNode ParentNode, PVirtualNode Node,
	  TVirtualNodeInitStates &InitialStates)
{
	CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
	if (dtu->m_nNodeType == NODE_GROUP)
		InitialStates = InitialStates<<ivsHasChildren;
	else
		InitialStates = InitialStates>>ivsHasChildren;
	if ((Node->ChildCount == 0) || (dtu->m_nNodeType == NODE_CARD))
		InitialStates = InitialStates<<ivsExpanded;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::btnDeleteClick(TObject *Sender)
{
	if (vt->FocusedNode != NULL)
	{
		CDtu* dtu = (CDtu*)(vt->GetNodeData(vt->FocusedNode));
		if (dtu->m_nNodeType != NODE_ALL_CARD_ROOT)
		{
			PVirtualNode ParentNode = vt->FocusedNode->Parent;
			dtu->DeleteRegistry();
			PVirtualNode Node = vt->GetPrevious(vt->FocusedNode);
			vt->DeleteNode(vt->FocusedNode);
			vt->FocusedNode = Node;
			vt->Selected[Node] = true;
			WriteGroupList(ParentNode);
			m_bChanged = true;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::vtFocusChanging(TBaseVirtualTree *Sender,
	  PVirtualNode OldNode, PVirtualNode NewNode, TColumnIndex OldColumn,
	  TColumnIndex NewColumn, bool &Allowed)
{
	if ((NewColumn == 0) || (NewColumn == 2))
		Allowed = true;
	else
		Allowed = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::vtGetImageIndex(TBaseVirtualTree *Sender,
	  PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column, bool &Ghosted,
	  int &ImageIndex)
{
	if (Column == 0)
	{
		CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
		if (dtu->m_nNodeType == NODE_CARD)
			ImageIndex = 0;
		else
			ImageIndex = 1;
	}
	else if (Column == 1)
	{
		if (Sender->FocusedNode == Node)
			ImageIndex = 2;
		else
			ImageIndex = -1;
	}
	else
		ImageIndex = -1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::vtDragAllowed(TBaseVirtualTree *Sender,
	  PVirtualNode Node, TColumnIndex Column, bool &Allowed)
{
	CDtu* dtu = (CDtu*)(Sender->GetNodeData(Node));
	if (dtu->m_nNodeType != NODE_ALL_CARD_ROOT)
	{
		m_dtuDragDrop = dtu;
		m_nodeDragDrop = Node;
		Allowed = true;
	}
	else
		Allowed = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::vtDragDrop(TBaseVirtualTree *Sender,
	  TObject *Source, IDataObject *DataObject, TFormatArray Formats,
	  TShiftState Shift, TPoint &Pt, int &Effect, TDropMode Mode)
{
	TVTNodeAttachMode AttachMode;

	PVirtualNode NodeTarget = Sender->DropTargetNode;
	if (NodeTarget == NULL)
		NodeTarget = vt->GetFirstChild(NULL);
	CDtu* dtu = (CDtu*)(Sender->GetNodeData(NodeTarget));

	if (Formats.Length > 0)
	{
		for (int i = 0; i < Formats.High; i++)
		{
			if (Formats[i] == CF_VIRTUALTREE)
			{
				if (Mode == dmAbove)
				{
					if (dtu->m_nNodeType == NODE_ALL_CARD_ROOT)
						AttachMode = amAddChildLast;
					else
						AttachMode = amInsertBefore;
				}
				else if (Mode == dmOnNode)
				{
					if ((dtu->m_nNodeType == NODE_GROUP) || (dtu->m_nNodeType == NODE_ALL_CARD_ROOT))
						AttachMode = amAddChildLast;
					else
						AttachMode = amInsertAfter;
				}
				else if (Mode == dmBelow)
				{
					if (dtu->m_nNodeType == NODE_ALL_CARD_ROOT)
						AttachMode = amAddChildLast;
					else
						AttachMode = amInsertAfter;
				}
				else
				{
					PVirtualNode OldParentNode = m_nodeDragDrop->Parent;
					NodeTarget = vt->GetFirstChild(NULL);
					Sender->ProcessDrop(DataObject, NodeTarget, Effect, amAddChildLast);
					dtu = (CDtu*)vt->GetNodeData(NodeTarget);
					if (dtu != m_dtuDragDrop)
					{
						if (OldParentNode != NodeTarget)
						{
							m_dtuDragDrop->CopyTo(dtu->m_sSection + "\\" + dtu->m_sName + "\\LedPanels");
							WriteGroupList(OldParentNode);
						}
						WriteGroupList(NodeTarget);
						ChangTreeSection(dtu, m_nodeDragDrop);
						m_bChanged = true;
					}
					return;
				}
				PVirtualNode OldParentNode = m_nodeDragDrop->Parent;
				Sender->ProcessDrop(DataObject, NodeTarget, Effect, AttachMode);
				dtu = (CDtu*)vt->GetNodeData(NodeTarget);
				if (dtu->m_nNodeType == NODE_CARD)
				{
					NodeTarget = NodeTarget->Parent;
					dtu = (CDtu*)vt->GetNodeData(NodeTarget);
				}
				if (dtu != m_dtuDragDrop)
				{
					if (OldParentNode != NodeTarget)
					{
						m_dtuDragDrop->CopyTo(dtu->m_sSection + "\\" + dtu->m_sName + "\\LedPanels");
						WriteGroupList(OldParentNode);
					}
					WriteGroupList(NodeTarget);
					ChangTreeSection(dtu, m_nodeDragDrop);
					m_bChanged = true;
				}
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::vtDragOver(TBaseVirtualTree *Sender,
	  TObject *Source, TShiftState Shift, TDragState State, TPoint &Pt,
	  TDropMode Mode, int &Effect, bool &Accept)
{
	Accept = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::vtNewText(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, UnicodeString NewText)
{
	CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
	if (Column == 0)
	{
		if (dtu->m_nNodeType != NODE_ALL_CARD_ROOT)
		{
			if (CheckNameUnique(Node, NewText))
			{
				dtu->Rename(NewText);
				WriteGroupList(Node->Parent);
				m_bChanged = true;
			}
			else
			{
				if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
					MessageBox(this->Handle,  L"该名称已被使用！", L"错误", MB_OK | MB_ICONWARNING);
				else
					MessageBox(this->Handle,  L"This name has been used！", L"Error", MB_OK | MB_ICONWARNING);
			}
		}
	}
	else if (Column == 2)
	{
		dtu->m_sCode = NewText;
		dtu->SaveConfig();
		m_bChanged = true;
	}
}
//---------------------------------------------------------------------------

bool __fastcall TfrmCardList::CheckNameUnique(PVirtualNode CurrentNode, UnicodeString &NewText)
{
	PVirtualNode Node = vt->GetFirst();
	while (Node != NULL)
	{
		CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
		if (Node != CurrentNode)
		{
			if (dtu->m_sName == NewText)
				return false;
		}
		Node = vt->GetNext(Node);
	}

	return true;
}
//---------------------------------------------------------------------------

int __fastcall TfrmCardList::FindUniqueId()
{
	UnicodeString sCard, sGroup;

	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		sCard = "显示屏(控制卡)";
		sGroup = "显示屏(控制卡)组";
	}
	else
	{
		sCard = "LED Panel(Card)";
		sGroup = "Group of LED Panels(Cards)";
	}

	bool bFound;
	int nIndex = 1;
	for (;;)
	{
		bFound = false;
		PVirtualNode Node = vt->GetFirst();
		while (Node != NULL)
		{
			CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
			if (dtu->m_sName == sCard + UnicodeString(nIndex))
			{
				bFound = true;
				break;
			}
			if (dtu->m_sName == sGroup + UnicodeString(nIndex))
			{
				bFound = true;
				break;
			}
			Node = vt->GetNext(Node);
		}

		if (!bFound)
			break;
		nIndex++;
	}

	return nIndex;
}
//---------------------------------------------------------------------------

void TfrmCardList::ChangTreeSection(CDtu* dtuNewParent, PVirtualNode NodeRoot)
{
	CDtu* dtu = (CDtu*)(vt->GetNodeData(NodeRoot));
	dtu->m_sSection = dtuNewParent->m_sSection + "\\" + dtuNewParent->m_sName + "\\LedPanels";
	if (dtu->m_nNodeType == NODE_GROUP)
	{
		PVirtualNode Node = vt->GetFirstChild(NodeRoot);
		unsigned int nLevel = vt->GetNodeLevel(Node);
		while (Node != NULL)
		{
			if (vt->GetNodeLevel(Node) == nLevel)
				ChangTreeSection(dtu, Node);
			if (Node == vt->GetLastChild(NodeRoot))
				break;
			Node = vt->GetNext(Node, false);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::btnAddCardsClick(TObject *Sender)
{
	TfrmAddCards* frmAddCards = new TfrmAddCards(this);
	if (frmAddCards->ShowModal() != mrOk)
	{
		delete frmAddCards;
		return;
	}

	int nCount = frmAddCards->editCount->Text.ToInt();
	delete frmAddCards;

	for (int i = 0; i < nCount; i++)
	{
		PVirtualNode Node = vt->InsertNode(vt->FocusedNode, amInsertAfter, NULL);
		CDtu* dtuSelected = (CDtu*)(vt->GetNodeData(vt->FocusedNode));
		CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
		dtu->Create(dtuSelected);
		if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
			dtu->m_sName = "显示屏(控制卡)" + UnicodeString(FindUniqueId());
		else
			dtu->m_sName = "LED Panel(Card)" + UnicodeString(FindUniqueId());
		dtu->SaveConfig();
		WriteGroupList(vt->FocusedNode->Parent);
		vt->Selected[Node] = true;
		vt->Expanded[Node] = true;
		vt->FocusedNode = Node;
	}
	vt->Refresh();
	m_bChanged = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmCardList::vtFocusChanged(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column)
{
	CDtu* dtu = (CDtu*)(vt->GetNodeData(Node));
	btnAddCards->Enabled = (dtu != NULL) && (dtu->m_nNodeType == NODE_CARD);
}
//---------------------------------------------------------------------------

