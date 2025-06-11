#ifndef __LORA_H
#define __LORA_H
#include "sys.h"


u8 LoRa_Init(void);
void Lora_set(void);
void Aux_Int(u8 mode);
void LoRa_Process(void);
void LoRa_SendData(void);
void LoRa_ReceData(void);
uint8_t Get_Lora_Flage(void);
#define LORA_AUX  PAin(4)    //LORA模块状态引脚
#define LORA_MD0  PAout(15)  //LORA模块控制引脚

#endif


