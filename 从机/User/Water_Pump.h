#ifndef __Water_Pump_H
#define __Water_Pump_H


#define PumpA_ON    GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define PumpB_ON    GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define PumpC_ON    GPIO_SetBits(GPIOB,GPIO_Pin_9)


#define PumpA_OFF   GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define PumpB_OFF   GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define PumpC_OFF   GPIO_ResetBits(GPIOB,GPIO_Pin_9)


void Pump_Init(void);


#endif
