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
	uint8_t CurrentNum;	//��ǰ�������:ҳ��
	uint8_t Enter;		//ȷ�ϼ�
	uint8_t Next;		//��һ��
  uint8_t Last;		//��һ��
	uint8_t Return;		//���ؼ�
	void (*Current_Operation)(void);//��ǰ����(����ָ��)
}Menu_table_t;

enum{//������
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
