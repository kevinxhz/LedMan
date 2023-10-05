//---------------------------------------------------------------------------

#ifndef CardListH
#define CardListH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "VirtualTrees.hpp"
#include <ImgList.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include "sysconfig.h"
#include "dtu.h"

//---------------------------------------------------------------------------
class TfrmCardList : public TForm
{
__published:	// IDE-managed Components
	TImageList *TreeImages;
	TVirtualStringTree *vt;
	TToolBar *ToolBar1;
	TSpeedButton *btnDelete;
	TSpeedButton *btnNewCard;
	TSpeedButton *btnNewGroup;
	TSpeedButton *btnAddCards;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnNewGroupClick(TObject *Sender);
	void __fastcall vtGetNodeDataSize(TBaseVirtualTree *Sender, int &NodeDataSize);
	void __fastcall vtGetText(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText);
	void __fastcall btnNewCardClick(TObject *Sender);
	void __fastcall vtPaintText(TBaseVirtualTree *Sender,
          const TCanvas *TargetCanvas, PVirtualNode Node, TColumnIndex Column,
          TVSTTextType TextType);
	void __fastcall vtInitNode(TBaseVirtualTree *Sender, PVirtualNode ParentNode,
          PVirtualNode Node, TVirtualNodeInitStates &InitialStates);
	void __fastcall btnDeleteClick(TObject *Sender);
	void __fastcall vtFocusChanging(TBaseVirtualTree *Sender, PVirtualNode OldNode,
		  PVirtualNode NewNode, TColumnIndex OldColumn, TColumnIndex NewColumn,
          bool &Allowed);
	void __fastcall vtGetImageIndex(TBaseVirtualTree *Sender, PVirtualNode Node,
          TVTImageKind Kind, TColumnIndex Column, bool &Ghosted,
          int &ImageIndex);
	void __fastcall vtDragAllowed(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, bool &Allowed);
	void __fastcall vtDragDrop(TBaseVirtualTree *Sender, TObject *Source,
		  IDataObject *DataObject, TFormatArray Formats, TShiftState Shift,
		  TPoint &Pt, int &Effect, TDropMode Mode);
	void __fastcall vtDragOver(TBaseVirtualTree *Sender, TObject *Source,
		  TShiftState Shift, TDragState State, TPoint &Pt, TDropMode Mode,
		  int &Effect, bool &Accept);
	void __fastcall vtNewText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
		  UnicodeString NewText);
	void __fastcall btnAddCardsClick(TObject *Sender);
	void __fastcall vtFocusChanged(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column);

private:	// User declarations
	CDtu* m_dtuDragDrop;
	PVirtualNode m_nodeDragDrop;
	bool __fastcall CheckNameUnique(PVirtualNode CurrentNode, UnicodeString &NewText);
	int __fastcall FindUniqueId();
	void LoadCardTree(UnicodeString sSection, PVirtualNode NodeRoot);
	void WriteGroupList(PVirtualNode pParentNode);
	void ChangTreeSection(CDtu* dtuNewParent, PVirtualNode Node);
public:		// User declarations
	__fastcall TfrmCardList(TComponent* Owner);
	bool m_bChanged;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCardList *frmCardList;
//---------------------------------------------------------------------------
#endif
