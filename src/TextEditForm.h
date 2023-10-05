//---------------------------------------------------------------------------

#ifndef TextEditFormH
#define TextEditFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <Clipbrd.hpp>
#include <ExtCtrls.hpp>

#define MAX_ROW		64
#define MAX_COL		1024

#define ALIGN_LEFT		0
#define ALIGN_CENTER	1
#define ALIGN_RIGHT		2

//---------------------------------------------------------------------------
class TwndTextEdit : public TForm
{
__published:	// IDE-managed Components
	TPopupMenu *mnuPopupEdit;
	TImageList *ImageList_Edit;
	TMenuItem *mnuCopy;
	TMenuItem *mnuPaste;
	TMenuItem *mnuCut;
	TMenuItem *mnuDelete;
	TMenuItem *mnuSelectAll;
	TMenuItem *N2;
	TImage *ImageText;
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall WinMsgProc(TMessage &msg);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall mnuPopupEditPopup(TObject *Sender);
	void __fastcall mnuDeleteClick(TObject *Sender);
	void __fastcall mnuCopyClick(TObject *Sender);
	void __fastcall mnuCutClick(TObject *Sender);
	void __fastcall mnuPasteClick(TObject *Sender);
	void __fastcall mnuSelectAllClick(TObject *Sender);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y);
	void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);

private:	// User declarations
	BYTE m_ucTextColor;
public:		// User declarations
	__fastcall TwndTextEdit(TComponent* Owner);

	TStrings *m_wsListText;
	TStrings *m_wsListActive;
	TStrings *m_wsListColor;
	int m_nRow, m_nCol;
	void SetCaret();
	int m_nSelectStartRow, m_nSelectStartCol, m_nSelectEndRow, m_nSelectEndCol;
	bool m_bSelect;
	void AdjustSelectArea();
	bool CheckInSelectArea(int nRow, int nCol);
	bool DeleteSelectedText();
	TClipboard* m_ClipBoard;
	bool m_bMouseStartSelect;
	void FormatSelctedArea();
	void InvalidateChar(int nRow, int nCol);
	void InvalidateRow(int nRow);
	void InvalidateSelectedArea();
	int m_nOldMoveRow, m_nOldMoveCol;
	void DrawWindowsText();
	bool IsSelectedRow(int nRow);
	int GetRowPosInWindow(int nRow);
	void GetCharMatrixPos(int X, int Y);
	UnicodeString GetText();
	UnicodeString GetTextColor();
	int m_nAlignStyle;
	bool m_bAlignVertCenter;
	int GetOffsetX(int nRow);
	void ChangeTextColor(BYTE ucColor);
	void ChangeBackColor(BYTE ucColor);
	int m_nEditScreenWidth;
	int m_nEditScreenHeight;
	bool m_bDirty;
	void Clear();
	void SetText(UnicodeString ws);
	void SetTextColor(UnicodeString s);
	TPicture *pic;
	bool IsAnyTextSelected();
	UnicodeString GetSelectedText();
	void SetTextActive(int nActiveType);
	UnicodeString GetActiveText();
	void SetActiveText(UnicodeString ws);
	void SetFont(TFont* pFont, int nDisplayAreaHeight);
	int m_nFontHeight;
	UnicodeString TrimRightEmptyLines(UnicodeString& wsSrc);
	bool m_bDotFont;
	int m_nDotFontSize;
	void SetDotFont(int nFontSize);
	void DrawDotFontText();
	int GetTextWidth(UnicodeString ws, int nCount);
	bool m_bRefreshEnabled;
	void EnabledRefresh(bool bEnable);
	void ResetCursor();
	void ChangeLanguage();
	void DrawCicleClock();
#ifdef AUTO_SIZE_TEXT
	void AutoSizeText();
#endif
};

//---------------------------------------------------------------------------
#endif
