//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SearchAct.h"
#include "SysConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSearchAct *frmSearchAct;
//---------------------------------------------------------------------------
__fastcall TfrmSearchAct::TfrmSearchAct(TComponent* Owner)
	: TForm(Owner)
{
	if (sysConfig.m_nLanguage == LANGUAGE_CHINESE)
	{
		Caption = "���ҽ�Ŀ";
		lbText->Caption = "��Ŀ�������ݵ�ȫ���򲿷֣�*��ʾ�������а�����Ч���ݵĽ�Ŀ:";
		lbDescription->Caption = "��Ŀ˵����ȫ���򲿷֣�*��ʾ����������ע��˵���Ľ�Ŀ:";
		btnSearch->Caption = "����(&S)";
		btnCancel->Caption = "ȡ��(&C)";
	}
	else
	{
		Caption = "Search Acts";
		lbText->Caption = "(part of ) Act's text. * means all acts which has any text";
		lbDescription->Caption = "(part of ) Act's description. * means all acts which has any description";
		btnSearch->Caption = "&Search";
		btnCancel->Caption = "&Cancel";
	}
}
//---------------------------------------------------------------------------
