//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RootName.h"
#include "SysConfig.h"
#include "dtu.h"
#include "CustMsgDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRootName *frmRootName;
//---------------------------------------------------------------------------
__fastcall TfrmRootName::TfrmRootName(TComponent* Owner)
	: TForm(Owner)
{
	m_frmMain = (TfrmMain*)Owner;
	m_nResult = -1;
	m_bDeleted = false;

	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "该软件管理的所有显示屏的根节点名称";
		lbOldName->Caption = "当前根接点名称:";
		lbNewName->Caption = "新名称:";
		btnJump->Caption = "跳转";
		btnChange->Caption = "修改";
		btnCreateNew->Caption = "新建";
		btnCancel->Caption = "取消";
	}
	else
	{
		Caption = "Root Name of All LED Signs";
		lbOldName->Caption = "Current Name:";
		lbNewName->Caption = "New Name:";
		btnJump->Caption = "Jump to";
		btnChange->Caption = "Change";
		btnCreateNew->Caption = "Create New";
		btnCancel->Caption = "Cancel";
	}
}

//---------------------------------------------------------------------------
void __fastcall TfrmRootName::FormCreate(TObject *Sender)
{
	wchar_t szName[256];

	TRegistry *reg = new TRegistry();
	reg->RootKey = HKEY_CURRENT_USER;
	if (reg->OpenKeyReadOnly(REG_ROOT_NAME))
	{
		DWORD dwIndex = 0;
		while (RegEnumKey(reg->CurrentKey, dwIndex++, szName, 256) == ERROR_SUCCESS)
			comboRootName->Items->Add(szName);
	}

	delete reg;

	editRootName->Text = sysConfig.m_sRootName;
	comboRootName->Text = editRootName->Text;
}

//---------------------------------------------------------------------------
void __fastcall TfrmRootName::btnChangeClick(TObject *Sender)
{
	UnicodeString sNewName;

	sNewName = comboRootName->Text;

	HKEY hKey;
	if (RegOpenKey(HKEY_CURRENT_USER, UnicodeString(REG_ROOT_NAME + UnicodeString("\\") + sNewName).c_str(), &hKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		Application->MessageBox(L"新名称已经存在。请选择另外的根节点名称。", L"修改根节点名称", MB_OK | MB_ICONWARNING);
		return;
	}

	LSTATUS _stdcall (*SHCopyKey)(HKEY hkeySrc,	wchar_t* szSrcSubKey, HKEY hkeyDest,	DWORD fReserved);

	HINSTANCE hInst = LoadLibraryW(L"shlwapi.dll");
	if (hInst)
	{
		SHCopyKey = (LSTATUS (__stdcall*)(HKEY,	wchar_t*, HKEY, DWORD))GetProcAddress(hInst, "SHCopyKeyW");

		HKEY hKey = NULL;
		HKEY hKeyDst = NULL;
		RegCreateKeyW(HKEY_CURRENT_USER, UnicodeString(REG_ROOT_NAME).c_str(), &hKey);
		RegCreateKeyW(HKEY_CURRENT_USER, UnicodeString(REG_ROOT_NAME + UnicodeString("\\") + sNewName).c_str(), &hKeyDst);
		SHCopyKey(hKey, sysConfig.m_sRootName.c_str(), hKeyDst, 0);
		RegCloseKey(hKey);
		RegCloseKey(hKeyDst);

		FreeLibrary(hInst);

		TCustomIniFile* sysini = new TRegistryIniFile(REG_ROOT_NAME);
		sysini->EraseSection(sysConfig.m_sRootName);
		delete sysini;

		sysConfig.m_sRootName = sNewName;

		sysini = new TRegistryIniFile("Software\\Yuepoch");
		sysini->WriteString("LedPanelManV3", "RootName", sysConfig.m_sRootName);
		delete sysini;

		PVirtualNode Node = m_frmMain->vt->GetFirst();
		CDtu* dtuRoot = (CDtu*)(m_frmMain->vt->GetNodeData(Node));
		dtuRoot->m_sName = sysConfig.m_sRootName;

		Application->MessageBox(L"修改根节点名称成功！", L"修改根节点名称", MB_OK | MB_ICONINFORMATION);
		m_nResult = 0;

		Close();
	}
	else
	{
		Application->MessageBox(L"修改根节点名称失败！如果您正在使用注册表编辑工具，请关闭后重试。", NULL, MB_OK | MB_ICONWARNING);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRootName::btnCreateNewClick(TObject *Sender)
{
	UnicodeString sNewName;

	sNewName = comboRootName->Text;

	HKEY hKey;
	if (RegOpenKey(HKEY_CURRENT_USER, UnicodeString(REG_ROOT_NAME + UnicodeString("\\") + sNewName).c_str(), &hKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		Application->MessageBox(L"新名称已经存在。请选择另外的根节点名称。", L"新建根节点名称", MB_OK | MB_ICONWARNING);
		return;
	}

	sysConfig.m_sRootName = sNewName;

	TCustomIniFile* sysini = new TRegistryIniFile("Software\\Yuepoch");
	sysini->WriteString("LedPanelManV3", "RootName", sysConfig.m_sRootName);
	delete sysini;

	m_nResult = 1;

	Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRootName::btnJumpClick(TObject *Sender)
{
	UnicodeString sNewName;

	sNewName = comboRootName->Text;

	HKEY hKey;
	if (!RegOpenKey(HKEY_CURRENT_USER, UnicodeString(REG_ROOT_NAME + UnicodeString("\\") + sNewName).c_str(), &hKey) == ERROR_SUCCESS)
	{
		Application->MessageBox(L"新名称不存在。请在新名称列表中选择已有的的根节点名称。", L"跳转到新的根节点", MB_OK | MB_ICONWARNING);
		return;
	}

	RegCloseKey(hKey);

	sysConfig.m_sRootName = sNewName;

	TCustomIniFile* sysini = new TRegistryIniFile("Software\\Yuepoch");
	sysini->WriteString("LedPanelManV3", "RootName", sysConfig.m_sRootName);
	delete sysini;

	m_nResult = 2;

	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmRootName::btnDeleteClick(TObject *Sender)
{
	UnicodeString sName = comboRootName->Text;

	if (sysConfig.m_sRootName == sName)
	{
		Application->MessageBox(L"您不能删除正在使用中的根节点！", L"删除根节点", MB_OK | MB_ICONWARNING);
		return;

	}

	HKEY hKey;
	if (!RegOpenKey(HKEY_CURRENT_USER, UnicodeString(REG_ROOT_NAME + UnicodeString("\\") + sName).c_str(), &hKey) == ERROR_SUCCESS)
	{
		Application->MessageBox(L"新名称不存在。请在新名称列表中选择已有的的根节点名称。", L"删除根节点", MB_OK | MB_ICONWARNING);
		return;
	}

	RegCloseKey(hKey);

	if (Application->MessageBox((UnicodeString("您确定要删除根节点: <") + sName + UnicodeString(">?")).c_str(), L"删除根节点", MB_YESNO | MB_ICONQUESTION) != IDYES)
		return;

	TCustomIniFile* ini = new TRegistryIniFile(REG_ROOT_NAME);
	ini->EraseSection(sName);
	delete ini;

	comboRootName->Items->Delete(comboRootName->Items->IndexOf(sName));
	comboRootName->Text = editRootName->Text;
	m_bDeleted = true;
}
//---------------------------------------------------------------------------

