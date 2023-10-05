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
  if (!IsFirstInstance()) return -1;       // 只能启动一个实例

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

     // 初始化系统后台任务
     if (!sysConfig.SysInit()) return -1;

     Application->Title = "GPRS显示屏管理";
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
  //创建互斥元.如果互斥元已经存在,这就是应用程序的第二个事例.
  //注意:当应用程序结束时,互斥元自动关闭.
  if (::CreateMutex(NULL, TRUE, "__Yuepoch_LEDManager__") != NULL) {
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
      return FALSE;
    else
      return TRUE;
  }
  return FALSE;
}
//---------------------------------------------------------------------------

