#include "lora.h"
#include "sys.h"
#include "delay.h"
#include "usart3.h"
#include "string.h"
#include "stdio.h"
#include "my_usart1.h"
#include "led.h"
#include "key.h"


//�豸����ģʽ(���ڼ�¼�豸״̬)
u8 Lora_mode=0;//0:����ģʽ 
static uint8_t flage = 0;
extern u8 USART_RX_BUF[];     //���ջ���,���USART_REC_LEN���ֽ�.
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;


//LORAģ�鷢������
void LoRa_SendData(void)
{    
    	u8 temp[256] = "Hello Lora !!!";
			u3_printf("%s\r\n",temp);	
}
uint8_t Get_Lora_Flage(void)
{
	if(flage == 1){
		flage = 0;
		return 1; 
	}
	else{
		return 0;
	}
}

//Loraģ���������
void LoRa_ReceData(void)
{
		u16 len=0;  

  if(USART3_RX_STA&0x8000)
	{
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;//��ӽ�����
		USART3_RX_STA=0;
		flage =1;
//		printf("���յ�������Ϊ");
//		printf("%s\r\n",USART3_RX_BUF);
  }

}


