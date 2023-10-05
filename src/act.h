#ifndef _ACT_H_
#define _ACT_H_

#include "global.h"

///////////////////////////////////////////
//��Ŀ����
#define ACT_TYPE_DIRECT_SHOW			0			//ֱ��������ʾ���� ����: ��ʾ���� ��ʾʱ��
#define ACT_TYPE_MOVE_LEFT				1			//����һ����ʾ���� ����: ͣ��ʱ��
#define ACT_TYPE_MOVE_RIGHT				2			//����һ����ʾ���� ����: ͣ��ʱ��
#define ACT_TYPE_MOVE_UP				3			//����һ����ʾ���� ����: ͣ��ʱ��
#define ACT_TYPE_MOVE_DOWN				4			//����һ����ʾ���� ����: ͣ��ʱ��
#define ACT_TYPE_CLEAR					5			//�����ʾ���� ����: ͣ��ʱ��
#define ACT_TYPE_MAX_ID					5

///////////////////////////////////////////
//��ʾ�������Ͷ���
#define OBJ_TYPE_NULL						0			//��, ��ʾ��Ŀ���ݽ�β
#define OBJ_TYPE_SINGLE_COLOR_IMAGE			1			//��ɫͼ��
#define OBJ_TYPE_DOUBLE_COLOR_CLOCK			2			//˫ɫʱ��
#define OBJ_TYPE_DOUBLE_COLOR_IMAGE			3			//˫ɫͼ��
#define OBJ_TYPE_SINGLE_COLOR_CLOCK			4			//��ɫʱ��
#define OBJ_TYPE_START_REPEAT				5			//��ʼ�ظ���� ����: �ظ�����
#define OBJ_TYPE_END_REPEAT					6			//�����ظ���� ��
#define OBJ_TYPE_4BIT_COLOR_IMAGE			7			//4bit 16ɫͼ��
#define OBJ_TYPE_THREE_COLOR_IMAGE			8			//����ɫͼ��
#define OBJ_TYPE_THREE_COLOR_CLOCK			9			//����ɫʱ���ද̬����
#define OBJ_TYPE_MAX_ID						9

///////////////////////////////////////////
//��ɫ����
#define COLOR_BLACK						0			//��
#define COLOR_RED						0x01		//��
#define COLOR_GREEN						0x02		//��
#define COLOR_YELLOW					0x03		//��
#define COLOR_BLUE						0x10		//��
#define COLOR_PINK						0x11		//�ۺ�
#define COLOR_AQUA						0x12		//ǳ��
#define COLOR_WHITE						0x13		//��

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
	WORD usLen;	//���ݲ��ֳ���
} MMI_CMD_HEAD;

typedef struct
{
	WORD usHeadTag;
	BYTE ucCardId;
	BYTE ucCmd;
	BYTE ucKey1;
	BYTE ucKey2;
	WORD usLen;	//���ݲ��ֳ���
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
#define SCAN_MODE_4_1_A					2	//P10 P12.5(ʹ��R1, 4�ֽ�˳�����ϵ���)
#define SCAN_MODE_4_1_B					3
#define SCAN_MODE_4_1_C					4
#define SCAN_MODE_16_1_B				5
#define SCAN_MODE_16_1_C				6
#define SCAN_MODE_4_1_D					7
#define SCAN_MODE_1_1_A					8
#define SCAN_MODE_1_1_B					9
#define SCAN_MODE_8_1_B					10

#define SCANLINE_REVERSE				0x80000000	//ABCDɨ��������

//��ɫͼ��
typedef struct
{
	BYTE ucObjectType;		//��������
	BYTE ucActType;			//��������
	WORD usRepeatCount;		//�����ظ�����
	DWORD ulPara;			//��������
	WORD usDisplayPosX;		//��ʾ�������Ͻ�X����
	WORD usDisplayPosY;		//��ʾ�������Ͻ�Y����
	WORD usDisplayWidth;	//��ʾ������
	WORD usDisplayHeight;	//��ʾ����߶�
	WORD usImageWidth;		//ͼ����
	WORD usImageHeight;		//ͼ��߶�
} IMAGE_OBJECT_HEAD;

//ʱ����, ͣ��ʱ��Ϊ1��
typedef struct
{
	BYTE ucObjectType;		//��������
	BYTE ucItemType;		//������: 0-��ʮλ, 1-���λ, 2-��ʮλ, 3-���λ, 4-��ʮλ, 5-�ո�λ...6,7,8,9,10,11, 12-������~��
	WORD usRepeatCount;		//����
	DWORD ulPara;			//ͣ��ʱ��
	WORD usDisplayPosX;		//��ʾ�������Ͻ�X����
	WORD usDisplayPosY;		//��ʾ�������Ͻ�Y����
	WORD usDisplayWidth;	//��ʾ������
	WORD usDisplayHeight;	//��ʾ����߶�
	WORD usImageWidth;		//ͼ����
	WORD usImageHeight;		//ͼ��߶�
} COLOR_OBJECT_HEAD;

//��Ŀͷ����Ϣ
typedef struct
{
	DWORD ulTag;											//��Ŀ��Ч��� ��28bitӦΪ0x9ABCDEF0,��4λ�Ǹý�Ŀ��������
	BYTE ucStartYear;										//��Ŀ��ʼ��������-��
	BYTE ucStartMonth;										//��Ŀ��ʼ��������-��
	BYTE ucStartDay;										//��Ŀ��ʼ��������-��
	BYTE ucStartHour;
	BYTE ucStartMin;
	BYTE ucEndYear;											//��Ŀֹͣ��������-��
	BYTE ucEndMonth;										//��Ŀֹͣ��������-��
	BYTE ucEndDay;											//��Ŀֹͣ��������-��
	BYTE ucEndHour;
	BYTE ucEndMin;
	BYTE ucRepeatCount;
	BYTE ucRefActIndex;
	DWORD ulStartSecondEveryDay[ACT_TIME_PERIOD_COUNT_24];		//ÿ�ղ���ʱ�ο�ʼʱ��-�����
	DWORD ulEndSecondEveryDay[ACT_TIME_PERIOD_COUNT_24];		//ÿ�ղ���ʱ��ֹͣʱ��-�����
	DWORD ulPlayTime;										//��Ŀ����һ�������ʱ��
} ACT_HEAD_GS7020BF;

typedef struct
{
	DWORD ulTag;					  	//��Ŀ��Ч��� ��28bitӦΪ0x9ABCDEF0,��4λ�Ǹý�Ŀ��������
	BYTE ucStartYear;					//��Ŀ��ʼ��������-��
	BYTE ucStartMonth;					//��Ŀ��ʼ��������-��
	BYTE ucStartDay;					//��Ŀ��ʼ��������-��
	BYTE ucStartHour;
	BYTE ucStartMin;
	BYTE ucEndYear;						//��Ŀֹͣ��������-��
	BYTE ucEndMonth;					//��Ŀֹͣ��������-��
	BYTE ucEndDay;						//��Ŀֹͣ��������-��
	BYTE ucEndHour;
	BYTE ucEndMin;
	BYTE ucRepeatCount;
	BYTE ucRefActIndex;
	WORD usStartMinuteEveryDay[ACT_TIME_PERIOD_COUNT_4];		//ÿ�ղ���ʱ�ο�ʼʱ��-���Ӽ���
	WORD usEndMinuteEveryDay[ACT_TIME_PERIOD_COUNT_4];		//ÿ�ղ���ʱ��ֹͣʱ��-���Ӽ���
} ACT_HEAD_STANDARD;

typedef struct
{
	DWORD ulTag;					  	//��Ŀ��Ч��� ��28bitӦΪ0x9ABCDEF0,��4λ�Ǹý�Ŀ��������
	BYTE ucStartYear;					//��Ŀ��ʼ��������-��
	BYTE ucStartMonth;					//��Ŀ��ʼ��������-��
	BYTE ucStartDay;					//��Ŀ��ʼ��������-��
	BYTE ucStartHour;
	BYTE ucStartMin;
	BYTE ucEndYear;						//��Ŀֹͣ��������-��
	BYTE ucEndMonth;					//��Ŀֹͣ��������-��
	BYTE ucEndDay;						//��Ŀֹͣ��������-��
	BYTE ucEndHour;
	BYTE ucEndMin;
	BYTE ucRepeatCount;
	BYTE ucRefActIndex;
	WORD usStartMinuteEveryDay[ACT_TIME_PERIOD_COUNT_24];		//ÿ�ղ���ʱ�ο�ʼʱ��-���Ӽ���
	WORD usEndMinuteEveryDay[ACT_TIME_PERIOD_COUNT_24];		//ÿ�ղ���ʱ��ֹͣʱ��-���Ӽ���
} ACT_HEAD_GS7024E;

typedef struct
{
	DWORD ulTag;					  	//��Ŀ��Ч��� ��28bitӦΪ0x9ABCDEF0,��4λ�Ǹý�Ŀ��������
} ACT_HEAD_PD102B;

//��Ŀͷ����Ϣ
typedef struct
{
	DWORD ulTag;											//��Ŀ��Ч��� ��28bitӦΪ0x9ABCDEF0,��4λ�Ǹý�Ŀ��������
	WORD usStartYear;										//��Ŀ��ʼ��������-��
	BYTE ucStartMonth;										//��Ŀ��ʼ��������-��
	BYTE ucStartDay;										//��Ŀ��ʼ��������-��
	BYTE ucStartHour;
	BYTE ucStartMinute;
	WORD usEndYear;											//��Ŀֹͣ��������-��
	BYTE ucEndMonth;										//��Ŀֹͣ��������-��
	BYTE ucEndDay;											//��Ŀֹͣ��������-��
	BYTE ucEndHour;
	BYTE ucEndMinute;
	BYTE ucRepeatCount;
	BYTE ucRes1;
	BYTE ucRes2;
	BYTE ucRes3;
	DWORD ulStartSecondInDay[ACT_TIME_PERIOD_COUNT_4];		//ÿ�ղ���ʱ�ο�ʼʱ��-�����
	DWORD ulEndSecondInDay[ACT_TIME_PERIOD_COUNT_4];		//ÿ�ղ���ʱ��ֹͣʱ��-�����
} ACT_HEAD_GS2012AS_64;

#endif


