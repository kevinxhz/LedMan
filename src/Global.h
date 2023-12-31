#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <windows.h>
#include <System.hpp>

//#define AUTO_SIZE_TEXT
//#define TWO_SOFT_DOG
//#define SUN_BO_NO_YledMan
#define GS2012A_PACK_SIZE 				1024
//#define GS2012A_PACK_SIZE 				512
//#define XINJIANG_AUTO_WEB_DATA			//新疆气象数据自动发布
//#define FIRST_ACT_DISABLED
//#define FIXED_IP 	"116.255.238.173"    	//郑州迅灵
#define GS7025H		//7025H替代9025

#ifdef SUN_BO_NO_YledMan
#define DEAFULT_DATA_CENTER 			"219.234.88.231"
#else
#define DEAFULT_DATA_CENTER 			"dtu.yuepoch.com"
//#define DEAFULT_DATA_CENTER 			"116.255.238.173"    	//郑州迅灵
#endif

//#define SOFTWARE_END_YEAR				2010
//#define SOFTWARE_END_MONTH			12
//#define SOFTWARE_END_DAY				29

#define LANGUAGE_CHINESE				0
#define LANGUAGE_ENGLISH				1

#define MAX_TX_DTU_COUNT_ONE_TIME		16

#define CARD_TYPE_GS7020B				0		//GS7020B GS7020E CS7020B
#define CARD_TYPE_GS7030B				1		//GS7030B CS7030B
#define CARD_TYPE_YL2010A				2
#define CARD_TYPE_GS7020BF				3		//GS7020EF
#define CARD_TYPE_GS2012A				4
#define CARD_TYPE_YL2010AF				5
#define CARD_TYPE_YL2010AR				6
#define CARD_TYPE_PD102B				7
#define CARD_TYPE_YL91A					8
#define CARD_TYPE_GS7020				9
#define CARD_TYPE_GS7020F				10
#define CARD_TYPE_GS7020M				11
#define CARD_TYPE_GS7020A				12
#define CARD_TYPE_YL2010C				13
#define CARD_TYPE_GS7020B_GPS			14
#define CARD_TYPE_GS7020BW				15
#define CARD_TYPE_YL2010T				16
#define CARD_TYPE_GS7020BX				17		//GS7020EX 出租车用多时段支持插播 循环冠名报时
#define CARD_TYPE_GS7020EC				18
#define CARD_TYPE_GS7020BF_HEB			19		//哈尔滨专用, 带通信加密
#define CARD_TYPE_GS7020BF_W			20		//武汉公交专用, 支持报站器
#define CARD_TYPE_GS7020BT				21		//按钮开始显示屏测试
#define CARD_TYPE_GS7030BV2				22
#define CARD_TYPE_GS7022B				23		//同时支持GPRS/SMS
#define CARD_TYPE_GS7020BQ				24		//控制卡软件同GS7020B, 发送软件支持4个16*16的气象预警图标，占用节目 101-104
#define CARD_TYPE_GS7020BP				25		//停车场专用
#define CARD_TYPE_GS2012A_S				26
#define CARD_TYPE_GS9026				27
#define CARD_TYPE_GS7024E				28
#define CARD_TYPE_WF2012B				29
#define CARD_TYPE_GS2012AS_64			30
#define CARD_TYPE_GS2012B				31
#define CARD_TYPE_GS9020				32
#define CARD_TYPE_YL2010A_GPS_JTX		33
#define CARD_TYPE_GS9020S				34
#define CARD_TYPE_GS7020H				35		//GS7020H CS7020H
#define CARD_TYPE_GS9025				36		//公交车站用文字屏
#define CARD_TYPE_GS9023				37		//公交车站用文字屏
#define CARD_TYPE_GS7020HS				38
#define CARD_TYPE_GS9020F_GPS			39
#define CARD_TYPE_GS7020HFA				40
#define CARD_TYPE_SN2P					41

//#define SUPPORT_PLAYTIME_TX				//深圳苓贯 YL2010AF 节目发送结束后发送总播放时间

#define MAX_ACT_COUNT  					255

#define MAX_EDIT_SCREEN_X_COUNT	32
#define MAX_EDIT_SCREEN_Y_COUNT	16

#define ACTIVE_DATA_WSD			0
#define ACTIVE_DATA_FSY			1
#define ACTIVE_DATA_WSD1		2
#define ACTIVE_DATA_TYPE 		ACTIVE_DATA_WSD

#define REG_ROOT_NAME			"Software\\Yuepoch\\LedPanelManV3"

#endif	//_GLOBAl_H_
