//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#pragma comment(lib,"ws2_32.lib")

#include <winsock2.h>
#include <registry.hpp>
#include "SysConfig.h"
#include "Login.h"
//---------------------------------------------------------------------------
USEFORM("MainForm.cpp", MasterForm);
USEFORM("CustomerFrameView.cpp", CustomerFrame); /* TFrame: File Type */
USEFORM("LEDBoardFrameView.cpp", LEDBoardFrame); /* TFrame: File Type */
USEFORM("CustMsgDlg.cpp", CustMsgForm);
USEFORM("PicMsgEditDlg.cpp", PicMsgEditForm);
USEFORM("TermListX.cpp", TermListXFrame); /* TFrame: File Type */
USEFORM("Login.cpp", LoginForm);
USEFORM("Password.cpp", PasswordForm);
//---------------------------------------------------------------------------
BOOL IsFirstInstance();
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  if (!IsFirstInstance()) return -1;       // ֻ������һ��ʵ��

  try
  {
     WideString pwd;
     bool bNeedLogin = false;
     TRegistry * reg = new TRegistry();
     reg->RootKey = HKEY_LOCAL_MACHINE;
     if (reg->OpenKeyReadOnly(L"Software\\Yuepoch\\LEDManager"))
     {
       if (reg->ValueExists(L"Password"))
       {
         pwd = reg->ReadString(L"Password");
         bNeedLogin = true;
       }

       reg->CloseKey();
     }
     delete reg;
     if (bNeedLogin)
     {
       TLoginForm * lForm = new TLoginForm(Application);
       lForm->pwd = pwd;
       if (lForm->ShowModal() != mrOk)
         return -1;
       delete lForm;
     }

     Application->Initialize();

     // ��ʼ��ϵͳ��̨����
     if (!sysConfig.SysInit()) return -1;

     Application->Title = "GPRS��ʾ������";
     Application->CreateForm(__classid(TCustMsgForm), &CustMsgForm);
         Application->CreateForm(__classid(TLoginForm), &LoginForm);
         Application->CreateForm(__classid(TPasswordForm), &PasswordForm);
         Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  catch (...)
  {
     try
     {
       throw Exception("");
     }
     catch (Exception &exception)
     {
       Application->ShowException(&exception);
     }
  }

  sysConfig.SysClose();

  return 0;
}
//---------------------------------------------------------------------------

BOOL IsFirstInstance()
{
  //��������Ԫ.�������Ԫ�Ѿ�����,�����Ӧ�ó���ĵڶ�������.
  //ע��:��Ӧ�ó������ʱ,����Ԫ�Զ��ر�.
  if (::CreateMutex(NULL, TRUE, "__Yuepoch_LEDManager__") != NULL) {
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
      return FALSE;
    else
      return TRUE;
  }
  return FALSE;
}
//---------------------------------------------------------------------------

