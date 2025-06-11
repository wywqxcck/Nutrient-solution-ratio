#include "sys.h"
#include "oled.h"
#include "APP.h"
extern u8 Lora_mode;
uint16_t EC = 0;
uint16_t TDS = 0;
uint16_t TEMP = 0;

void parseString(char* str, int value[], char fen) {
    char* token = strtok(str, &fen);
    int i = 0;
    
    while (token != NULL){
        sscanf(token, "%*[^=]=%d", &value[i]);
        token = strtok(NULL, &fen);
        i++;
    }
}



int main(void)
{
	OLED_Init();
	My_USART1(); //串口1初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	LED_GPIO_Config();
	Serial_Init();
	LoRa_Init();
	Lora_set();
	printf("init ok\r\n");
	int data[3];
	while(1)
	{
		Send_Data();
		LoRa_ReceData();
		if(Get_Lora_Flage() == 1){
			parseString(USART3_RX_BUF,data,'|');
//			for(uint8_t i = 0; i < 3;i++)
//			{          
//				printf("data[%d]=%d\r\n",i,data[i]);
//			}
		}
			EC = data[0];
			TDS = data[1];
			TEMP = data[2];
			char tjcstr[100];
		    sprintf(tjcstr, "page2.n0.val=%d", EC);
		    HMISends(tjcstr);
			HMISendb(0xff);
		    sprintf(tjcstr, "page2.n1.val=%d", TDS);
		    HMISends(tjcstr);
			HMISendb(0xff);
		    sprintf(tjcstr, "page2.n2.val=%d", TEMP);
		    HMISends(tjcstr);
			HMISendb(0xff);
//		    sprintf(tjcstr, "page0.n4.val=%d\xff\xff\xff", TDS);
//		    HMISends(tjcstr);
//		    sprintf(tjcstr, "page0.n5.val=%d\xff\xff\xff", TEMP);
//		    HMISends(tjcstr);
//			printf("ec = %d\ttds = %d\r\n",data[0],data[1]);
//		}
	}
	
}

