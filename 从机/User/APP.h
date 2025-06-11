#ifndef __APP_H_
#define __APP_H_

#include "sys.h"
#include "oled.h"
#include "string.h"
#include "stdio.h"
#include "ds18b20.h"
#include "AD.h"
#include "Water_Pump.h"
#include "my_usart1.h"

#define SEND_WAIT		0 
#define SEND_OK			1
#define SEND_ING		2 

typedef struct
{
	uint8_t CurrentNum;	//当前索引序号:页码
	uint8_t Enter;		//确认键
	uint8_t Next;		//下一个
  uint8_t Last;		//上一个
	uint8_t Return;		//返回键
	void (*Current_Operation)(void);//当前操作(函数指针)
}Menu_table_t;

enum{//界面编号
	Main_Mune,
	Choice_A_Mune,
	Choice_B_Mune,
	Choice_C_Mune,
	Set_A_Mune,
	Set_B_Mune,
	Set_C_Mune,
};

extern Menu_table_t taskTable[];
extern uint8_t taskIndex,key;

#endif
