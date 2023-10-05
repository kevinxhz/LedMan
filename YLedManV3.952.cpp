#include <vcl.h>
#pragma hdrstop
#include <atl\atlmod.h>
#pragma comment(lib,"ws2_32.lib")
#include <winsock2.h>
#include <registry.hpp>
#include "global.h"
#include "SysConfig.h"
#include "Login.h"

//---------------------------------------------------------------------------
USEFORM("src\SignCheck.cpp", frmCheckResult);
USEFORM("src\Login.cpp", LoginForm);
USEFORM("src\CardList.cpp", frmCardList);
USEFORM("src\SelectFont.cpp", frmFontSelect);
USEFORM("src\FlashTest.cpp", frmFlashTest);
USEFORM("src\AdjustClockSetup.cpp", frmAdjustClockSetup);
USEFORM("src\CustMsgDlg.cpp", frmMain);
USEFORM("src\DialogReTx.cpp", frmAutoReTx);
USEFORM("src\AdjustLightness.cpp", frmAdjustLightness);
USEFORM("src\BrightnessSetup.cpp", frmBrightnessSetup);
USEFORM("src\GpsMap.cpp", frmGpsMap);
USEFORM("src\ReTxSetup.cpp", frmReTxSetup);
USEFORM("src\CardType.cpp", frmCardType);
USEFORM("src\SearchCard.cpp", frmSearchCard);
USEFORM("src\TextEditForm.cpp", wndTextEdit);
USEFORM("src\Weather.cpp", frmWeather);
USEFORM("src\About.cpp", frmAbout);
USEFORM("src\CommConfig.cpp", frmCommConfig);
USEFORM("src\RemoteControl.cpp", frmRemoteControl);
USEFORM("src\TxLogView.cpp", frmTxLogView);
USEFORM("src\Password.cpp", frmPassword);
USEFORM("src\ActDefine.cpp", frmActDefine);
USEFORM("src\Warnning.cpp", frmWarnning);
USEFORM("src\RootName.cpp", frmRootName);
USEFORM("src\ActiveType.cpp", frmActiveType);
USEFORM("src\ConfigForm.cpp", frmConfig);
USEFORM("src\ParkDigit.cpp", frmParkDigit);
USEFORM("src\SearchAct.cpp", frmSearchAct);
USEFORM("src\AddCards.cpp", frmAddCards);
USEFORM("src\Weather_16x16_C1.cpp", frmWeather_16x16_C1);
USEFORM("src\Weather_64x64.cpp", frmWeather64x64);
USEFORM("src\LedDigit.cpp", frmLedDigit);
//---------------------------------------------------------------------------
BOOL CheckPassword()
{
	if (!sysConfig.m_sSystemPassword.IsEmpty())
	{
		TLoginForm *frmLogin = new TLoginForm(Application);
		if (frmLogin->ShowModal() != mrOk)
		{
			delete frmLogin;
			return FALSE;
		}

		delete frmLogin;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
int TestDog()
{
	int _stdcall (*NTFindFirst)(char*);
	int _stdcall (*NTGetHardwareID)(char*);
	char szAppName[] = "YuepochGS7020EF";
	char *GUID = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";

	HINSTANCE hInst = LoadLibrary(L"NT77.dll");
	if (hInst)
	{
		NTFindFirst = (int (__stdcall*)(char*))GetProcAddress(hInst, "NTFindFirst");
		int nRet = NTFindFirst(szAppName);
		if (nRet != 0)
		{
			FreeLibrary(hInst);
			return -1;
		}

		NTGetHardwareID = (int (__stdcall*)(char*))GetProcAddress(hInst, "NTGetHardwareID");

		nRet = NTGetHardwareID(GUID);
		if (nRet != 0)
		{
			FreeLibrary(hInst);
			return -1;
		}

		if (strcmp(GUID, "edf5f29bbec744b7aa20c54cc17b8e89") != 0)
		{
			FreeLibrary(hInst);
			return -1;
		}

		FreeLibrary(hInst);
	}
	else
		return -1;

	return 0;
}

//---------------------------------------------------------------------------
WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	Application->Initialize();

	//if (TestDog())
	//	return -1;

	if (!CheckPassword())
		return -1;

#ifdef SUN_BO_NO_YledMan
	Application->Title = L"Wireless 显示屏管理";
#else
	Application->Title = L"YledMan 显示屏管理";
#endif
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->CreateForm(__classid(TfrmCheckResult), &frmCheckResult);
		Application->CreateForm(__classid(TfrmGpsMap), &frmGpsMap);
		Application->Run();

	return 0;
}

