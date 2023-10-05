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
		Caption = "����������������ʾ���ĸ��ڵ�����";
		lbOldName->Caption = "��ǰ���ӵ�����:";
		lbNewName->Caption = "������:";
		btnJump->Caption = "��ת";
		btnChange->Caption = "�޸�";
		btnCreateNew->Caption = "�½�";
		btnCancel->Caption = "ȡ��";
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
		Application->MessageBox(L"�������Ѿ����ڡ���ѡ������ĸ��ڵ����ơ�", L"�޸ĸ��ڵ�����", MB_OK | MB_ICONWARNING);
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

		Application->MessageBox(L"�޸ĸ��ڵ����Ƴɹ���", L"�޸ĸ��ڵ�����", MB_OK | MB_ICONINFORMATION);
		m_nResult = 0;

		Close();
	}
	else
	{
		Application->MessageBox(L"�޸ĸ��ڵ�����ʧ�ܣ����������ʹ��ע���༭���ߣ���رպ����ԡ�", NULL, MB_OK | MB_ICONWARNING);
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
		Application->MessageBox(L"�������Ѿ����ڡ���ѡ������ĸ��ڵ����ơ�", L"�½����ڵ�����", MB_OK | MB_ICONWARNING);
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
		Application->MessageBox(L"�����Ʋ����ڡ������������б���ѡ�����еĵĸ��ڵ����ơ�", L"��ת���µĸ��ڵ�", MB_OK | MB_ICONWARNING);
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
		Application->MessageBox(L"������ɾ������ʹ���еĸ��ڵ㣡", L"ɾ�����ڵ�", MB_OK | MB_ICONWARNING);
		return;

	}

	HKEY hKey;
	if (!RegOpenKey(HKEY_CURRENT_USER, UnicodeString(REG_ROOT_NAME + UnicodeString("\\") + sName).c_str(), &hKey) == ERROR_SUCCESS)
	{
		Application->MessageBox(L"�����Ʋ����ڡ������������б���ѡ�����еĵĸ��ڵ����ơ�", L"ɾ�����ڵ�", MB_OK | MB_ICONWARNING);
		return;
	}

	RegCloseKey(hKey);

	if (Application->MessageBox((UnicodeString("��ȷ��Ҫɾ�����ڵ�: <") + sName + UnicodeString(">?")).c_str(), L"ɾ�����ڵ�", MB_YESNO | MB_ICONQUESTION) != IDYES)
		return;

	TCustomIniFile* ini = new TRegistryIniFile(REG_ROOT_NAME);
	ini->EraseSection(sName);
	delete ini;

	comboRootName->Items->Delete(comboRootName->Items->IndexOf(sName));
	comboRootName->Text = editRootName->Text;
	m_bDeleted = true;
}
//---------------------------------------------------------------------------

