#include "sys.h"
#include "delay.h"
#include "my_usart1.h"
#include "key.h"
#include "led.h"
#include "Water_Pump.h"
#include "lora.h"
#include "time3.h"
#include "usart3.h"
#include "Encoder.h"
#include "APP.h"
extern u16 Flow_A;
extern u16 Flow_B;
extern u16 Flow_C;    //流量计
uint16_t HMI_A,HMI_B,HMI_C;  //串口屏发的
uint16_t A_volume = 0;
uint16_t B_volume = 0;
uint16_t C_volume = 0;  //按键发的
uint16_t HighHMI_A,HighHMI_B,HighHMI_C;
extern uint32_t pulseCountPB3;
extern uint32_t pulseCountPB4;
extern uint32_t pulseCountPB14;
uint16_t temperature;
uint16_t TDS;
uint16_t EC;
void parseString(char* str, int value[]) {
    char* token = strtok(str, "|");
    int i = 0;   
    while (token != NULL){
        sscanf(token, "%*[^=]=%d", &value[i]);
        token = strtok(NULL, "|");
        i++;
    }
}
void Send_Data(void)
{
	char temp[256];
	int ec = EC,tds = TDS,t = temperature;
	sprintf(temp,"EC=%d|TDS=%d|t=%d",ec,tds,t);
	u3_printf(temp);	
}

int main(void)
 {		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级  
	My_USART1();
	usart3_init(115200);  //串口3初始化为115200 
	usart3_rx(1);//开启串口3接收
	GPIO_EXTI_Init();   //YF_S401初始化
	TIM4_Init(9,7199);
	KEY_GPIO_Init();    //EC11的按键初始化
	 Encoder_Init();
	button_init(&btn1, read_button1_GPIO, 0);
	button_attach(&btn1, PRESS_REPEAT,BTN1_PRESS_REPEAT_Handler);
	button_attach(&btn1, SINGLE_CLICK,BTN1_SINGLE_Click_Handler);
	button_attach(&btn1, DOUBLE_CLICK,BTN1_DOUBLE_Click_Handler);
	button_attach(&btn1, LONG_PRESS_START, BTN1_LONG_PRESS_START_Handler);
	button_start(&btn1);
	 Pump_Init();
	 DS18B20_Init();
	 AD_Init();
	TIM1_Init();
	OLED_Init();
	printf("LORA模块测试程序开始\r\n");
	 int data[6];
	 while(1)
	 {
		 key = Get_Key_Sta();
		 if(key != KEY_NO){
			switch(key){
				case KEY_ENTER:OLED_Clear();taskIndex = taskTable[taskIndex].Enter;break;
				case KEY_RETURN:OLED_Clear();taskIndex = taskTable[taskIndex].Return;break;
				case KEY_NEXT:
					if(taskIndex < Set_A_Mune){
						OLED_Clear();
					}
					taskIndex = taskTable[taskIndex].Next;
					break;
				case KEY_LAST:
					if(taskIndex < Set_A_Mune){
						OLED_Clear();
					}
					taskIndex = taskTable[taskIndex].Last;
					break;
			}
		 }
		 taskTable[taskIndex].Current_Operation();
		 Send_Data();
		LoRa_ReceData();
		 if(Get_Lora_Flage() == 1)
		{
			printf("USART3_RX_BUF = %s\r\n\r\n",USART3_RX_BUF);
			parseString(USART3_RX_BUF,data);
			for(uint8_t i = 0; i < 6;i++)
			{          
				printf("data[%d]=%d\r\n",i,data[i]);
			}
			HMI_A = data[0];
			HMI_B = data[1];
			HMI_C = data[2];
		}

		if(data[3] == 1)
		{
			PumpA_ON;
			
			data[3] = 0;
		}
		else if(Flow_A > HMI_A)
		{
			PumpA_OFF;
			pulseCountPB3 = 0;
		}
		if(data[4] == 3)
		{
			PumpB_ON;
		}
		else if(Flow_B > HMI_B)
		{
			PumpB_OFF;
			pulseCountPB4 = 0;
		}
		if(data[5] == 5)
		{
			PumpC_ON;
		}
		else if(Flow_C > HMI_C)
		{
			PumpC_OFF;
			pulseCountPB14 = 0;
		}
		Delay_ms(50);
	 }
	

} 

