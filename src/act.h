#ifndef _ACT_H_
#define _ACT_H_

#include "global.h"

///////////////////////////////////////////
//节目定义
#define ACT_TYPE_DIRECT_SHOW			0			//直接载入显示内容 参数: 显示内容 显示时间
#define ACT_TYPE_MOVE_LEFT				1			//左移一列显示内容 参数: 停留时间
#define ACT_TYPE_MOVE_RIGHT				2			//右移一列显示内容 参数: 停留时间
#define ACT_TYPE_MOVE_UP				3			//上移一行显示内容 参数: 停留时间
#define ACT_TYPE_MOVE_DOWN				4			//下移一行显示内容 参数: 停留时间
#define ACT_TYPE_CLEAR					5			//清除显示内容 参数: 停留时间
#define ACT_TYPE_MAX_ID					5

///////////////////////////////////////////
//显示对象类型定义
#define OBJ_TYPE_NULL						0			//空, 表示节目内容结尾
#define OBJ_TYPE_SINGLE_COLOR_IMAGE			1			//单色图像
#define OBJ_TYPE_DOUBLE_COLOR_CLOCK			2			//双色时钟
#define OBJ_TYPE_DOUBLE_COLOR_IMAGE			3			//双色图像
#define OBJ_TYPE_SINGLE_COLOR_CLOCK			4			//单色时钟
#define OBJ_TYPE_START_REPEAT				5			//开始重复标记 参数: 重复次数
#define OBJ_TYPE_END_REPEAT					6			//结束重复标记 无
#define OBJ_TYPE_4BIT_COLOR_IMAGE			7			//4bit 16色图像
#define OBJ_TYPE_THREE_COLOR_IMAGE			8			//三基色图像
#define OBJ_TYPE_THREE_COLOR_CLOCK			9			//三基色时钟类动态数据
#define OBJ_TYPE_MAX_ID						9

///////////////////////////////////////////
//颜色定义
#define COLOR_BLACK						0			//黑
#define COLOR_RED						0x01		//红
#define COLOR_GREEN						0x02		//绿
#define COLOR_YELLOW					0x03		//黄
#define COLOR_BLUE						0x10		//蓝
#define COLOR_PINK						0x11		//粉红
#define COLOR_AQUA						0x12		//浅绿
#define COLOR_WHITE						0x13		//白

#define BACKGROUND_COLOR_BLACK			0x00
#define BACKGROUND_COLOR_RED			0x04
#define BACKGROUND_COLOR_GREEN			0x08
#define BACKGROUND_COLOR_BLUE			0x20
#define BACKGROUND_COLOR_YELLOW			0x0C
#define BACKGROUND_COLOR_PINK			0x24
#define BACKGROUND_COLOR_AQUA			0x28
#define BACKGROUND_COLOR_WHITE			0x2C

#define MAX_IMAGE_WIDTH		2048
#define MAX_IMAGE_HEIGHT	2048

#define MMI_START_ID			0x90EB
#define MMI_START_ID1			0xEB
#define MMI_START_ID2			0x90

#define MMI_CMD_ECHO			0x51
#define MMI_CMD_STOP_DISPLAY	0x52
#define MMI_CMD_RESTART_DISPLAY	0x53
#define MMI_CMD_WRITE_FLASH		0x54
#define MMI_CMD_WRITE_FLASH_M	0x63
#define MMI_CMD_WRITE_FLASH_F	0x64
#define MMI_CMD_READ_CONFIG		0x55
#define MMI_CMD_WRITE_CONFIG	0x56
#define MMI_CMD_READ_CLOCK		0x57
#define MMI_CMD_WRITE_CLOCK		0x58
#define MMI_CMD_READ_FLASH		0x59
#define MMI_CMD_STOP_ACT_PROC	0x5B
#define MMI_CMD_IAP_START		0x81
#define MMI_CMD_IAP_START_F		0x82
#define MMI_CMD_TEXT_ACT		0xC1
#define MMI_CMD_RAM_TEXT_ACT	0xC9
#define MMI_CMD_UNLOCK_JTAG		0xFE
#define MMI_CMD_PARK_DIGIT		0x31
#define MMI_CMD_PARK_DIGIT_EXT		0x32
#define MMI_CMD_READ_BUS_CONFIG		0x91
#define MMI_CMD_WRITE_BUS_CONFIG	0x92
#define MMI_CMD_RC					0x49
#define MMI_CMD_PLAYTIME			0x72
#define MMI_CMD_WARNNING			0x60
#define MMI_CMD_CANCEL_WARNNING		0x61
#define MMI_CMD_QUERY_CHECK			0x62
#define MMI_CMD_QUERY_POSITION		0x68
#define MMI_TEMP_ADJUST_LIGHT		0x69
#define MMI_WRITE_LIGHT_CONFIG		0x6A
#define MMI_CMD_READ_CHIP_ID		0xF6
#define MMI_CMD_WRITE_CHIP_ID		0xF7
#define MMI_CMD_ERASE_ALL_ACTS		0x41
#define MMI_CMD_WEATHER_ICON		0x5D
#define MMI_CMD_LED_DIGIT			0x33

#define ACT_TIME_PERIOD_COUNT_24	24
#define ACT_TIME_PERIOD_COUNT_4		4

#pragma pack(1)
typedef struct
{
	WORD usHeadTag;
	BYTE ucCardId;
	BYTE ucCmd;
	WORD usLen;	//数据部分长度
} MMI_CMD_HEAD;

typedef struct
{
	WORD usHeadTag;
	BYTE ucCardId;
	BYTE ucCmd;
	BYTE ucKey1;
	BYTE ucKey2;
	WORD usLen;	//数据部分长度
} MMI_CMD_HEAD_HEB;
#pragma pack()

#define SOFT_VER							0x100
#define MAX_AREA_COUNT						8
#define DEVICE_ID_LEN						12
#define NET_NAME_LEN						32
#define IP_ADDR_STR_LEN						16
#define APN_LEN								32
#define MAX_LIGHT_CONTROL_COUNT				8

#define SCAN_MODE_16_1_A				0
#define SCAN_MODE_8_1_A					1
#define SCAN_MODE_4_1_A					2	//P10 P12.5(使用R1, 4字节顺序由上到下)
#define SCAN_MODE_4_1_B					3
#define SCAN_MODE_4_1_C					4
#define SCAN_MODE_16_1_B				5
#define SCAN_MODE_16_1_C				6
#define SCAN_MODE_4_1_D					7
#define SCAN_MODE_1_1_A					8
#define SCAN_MODE_1_1_B					9
#define SCAN_MODE_8_1_B					10

#define SCANLINE_REVERSE				0x80000000	//ABCD扫描线逆序

//单色图像
typedef struct
{
	BYTE ucObjectType;		//对象类型
	BYTE ucActType;			//动作类型
	WORD usRepeatCount;		//动作重复次数
	DWORD ulPara;			//动作参数
	WORD usDisplayPosX;		//显示区域坐上角X坐标
	WORD usDisplayPosY;		//显示区域坐上角Y坐标
	WORD usDisplayWidth;	//显示区域宽度
	WORD usDisplayHeight;	//显示区域高度
	WORD usImageWidth;		//图像宽度
	WORD usImageHeight;		//图像高度
} IMAGE_OBJECT_HEAD;

//时钟项, 停留时间为1秒
typedef struct
{
	BYTE ucObjectType;		//对象类型
	BYTE ucItemType;		//项类型: 0-年十位, 1-年个位, 2-月十位, 3-年个位, 4-日十位, 5-日个位...6,7,8,9,10,11, 12-星期日~六
	WORD usRepeatCount;		//保留
	DWORD ulPara;			//停留时间
	WORD usDisplayPosX;		//显示区域坐上角X坐标
	WORD usDisplayPosY;		//显示区域坐上角Y坐标
	WORD usDisplayWidth;	//显示区域宽度
	WORD usDisplayHeight;	//显示区域高度
	WORD usImageWidth;		//图像宽度
	WORD usImageHeight;		//图像高度
} COLOR_OBJECT_HEAD;

//节目头部信息
typedef struct
{
	DWORD ulTag;											//节目有效标记 高28bit应为0x9ABCDEF0,底4位是该节目所属区号
	BYTE ucStartYear;										//节目开始播放日期-年
	BYTE ucStartMonth;										//节目开始播放日期-月
	BYTE ucStartDay;										//节目开始播放日期-日
	BYTE ucStartHour;
	BYTE ucStartMin;
	BYTE ucEndYear;											//节目停止播放日期-年
	BYTE ucEndMonth;										//节目停止播放日期-月
	BYTE ucEndDay;											//节目停止播放日期-日
	BYTE ucEndHour;
	BYTE ucEndMin;
	BYTE ucRepeatCount;
	BYTE ucRefActIndex;
	DWORD ulStartSecondEveryDay[ACT_TIME_PERIOD_COUNT_24];		//每日播放时段开始时间-秒计数
	DWORD ulEndSecondEveryDay[ACT_TIME_PERIOD_COUNT_24];		//每日播放时段停止时间-秒计数
	DWORD ulPlayTime;										//节目播放一遍所需的时间
} ACT_HEAD_GS7020BF;

typedef struct
{
	DWORD ulTag;					  	//节目有效标记 高28bit应为0x9ABCDEF0,底4位是该节目所属区号
	BYTE ucStartYear;					//节目开始播放日期-年
	BYTE ucStartMonth;					//节目开始播放日期-月
	BYTE ucStartDay;					//节目开始播放日期-日
	BYTE ucStartHour;
	BYTE ucStartMin;
	BYTE ucEndYear;						//节目停止播放日期-年
	BYTE ucEndMonth;					//节目停止播放日期-月
	BYTE ucEndDay;						//节目停止播放日期-日
	BYTE ucEndHour;
	BYTE ucEndMin;
	BYTE ucRepeatCount;
	BYTE ucRefActIndex;
	WORD usStartMinuteEveryDay[ACT_TIME_PERIOD_COUNT_4];		//每日播放时段开始时间-分钟计数
	WORD usEndMinuteEveryDay[ACT_TIME_PERIOD_COUNT_4];		//每日播放时段停止时间-分钟计数
} ACT_HEAD_STANDARD;

typedef struct
{
	DWORD ulTag;					  	//节目有效标记 高28bit应为0x9ABCDEF0,底4位是该节目所属区号
	BYTE ucStartYear;					//节目开始播放日期-年
	BYTE ucStartMonth;					//节目开始播放日期-月
	BYTE ucStartDay;					//节目开始播放日期-日
	BYTE ucStartHour;
	BYTE ucStartMin;
	BYTE ucEndYear;						//节目停止播放日期-年
	BYTE ucEndMonth;					//节目停止播放日期-月
	BYTE ucEndDay;						//节目停止播放日期-日
	BYTE ucEndHour;
	BYTE ucEndMin;
	BYTE ucRepeatCount;
	BYTE ucRefActIndex;
	WORD usStartMinuteEveryDay[ACT_TIME_PERIOD_COUNT_24];		//每日播放时段开始时间-分钟计数
	WORD usEndMinuteEveryDay[ACT_TIME_PERIOD_COUNT_24];		//每日播放时段停止时间-分钟计数
} ACT_HEAD_GS7024E;

typedef struct
{
	DWORD ulTag;					  	//节目有效标记 高28bit应为0x9ABCDEF0,底4位是该节目所属区号
} ACT_HEAD_PD102B;

//节目头部信息
typedef struct
{
	DWORD ulTag;											//节目有效标记 高28bit应为0x9ABCDEF0,底4位是该节目所属区号
	WORD usStartYear;										//节目开始播放日期-年
	BYTE ucStartMonth;										//节目开始播放日期-月
	BYTE ucStartDay;										//节目开始播放日期-日
	BYTE ucStartHour;
	BYTE ucStartMinute;
	WORD usEndYear;											//节目停止播放日期-年
	BYTE ucEndMonth;										//节目停止播放日期-月
	BYTE ucEndDay;											//节目停止播放日期-日
	BYTE ucEndHour;
	BYTE ucEndMinute;
	BYTE ucRepeatCount;
	BYTE ucRes1;
	BYTE ucRes2;
	BYTE ucRes3;
	DWORD ulStartSecondInDay[ACT_TIME_PERIOD_COUNT_4];		//每日播放时段开始时间-秒计数
	DWORD ulEndSecondInDay[ACT_TIME_PERIOD_COUNT_4];		//每日播放时段停止时间-秒计数
} ACT_HEAD_GS2012AS_64;

#endif


