#include "APP.h"
#include "Serial.h"
#include "oled.h"
#include "Delay.h"
#include "my_usart1.h"
uint8_t Data_Arr[DATA_MAX] = {0x01,0,0,0,0,0,0,0,0};
uint32_t Flow_rateA = 0;//定义A流量值
uint32_t Flow_rateB = 0;//定义B流量值
uint32_t Flow_rateC = 0;//定义C流量值
uint8_t key_A = 0;
uint8_t key_B = 0;
uint8_t key_C = 0;

void Send_Data(void)
{
	Delay_ms(500); 
	if (Serial_GetRxFlag() == 1)	//如果接收到数据包
		{
			if (Serial_RxPacket[0] == 0x01)
			{
				Flow_rateA = (Serial_RxPacket[3] << 16)+(Serial_RxPacket[2] << 8)+Serial_RxPacket[1];
			}
			else if(Serial_RxPacket[0] == 0x02)
			{
				Flow_rateB = (Serial_RxPacket[3] << 16)+(Serial_RxPacket[2] << 8)+Serial_RxPacket[1];
			}
			else if(Serial_RxPacket[0] == 0x03)
			{
				Flow_rateC = (Serial_RxPacket[3] << 16)+(Serial_RxPacket[2] << 8)+Serial_RxPacket[1];
			}
		   if (Serial_RxPacket[1] == 0x01)
		   {
				key_A = Serial_RxPacket[1];
		   }
		   else if(Serial_RxPacket[1] == 0x02)
		   {
				key_A = Serial_RxPacket[1];
		   }
		   if(Serial_RxPacket[1] == 0x03)
		   {
				key_B = Serial_RxPacket[1];			   
		   }
		   else if(Serial_RxPacket[1] == 0x04)
		   {
				key_B = Serial_RxPacket[1];	
		   }
		   if(Serial_RxPacket[1] == 0x05)
		   {
				key_C = Serial_RxPacket[1];		
		   }
		   else if(Serial_RxPacket[1] == 0x06)
		   {
				key_C = Serial_RxPacket[1];	
		   }
	        Data_Arr[A1] = Flow_rateA>>8;
	        Data_Arr[A2] = (uint8_t)Flow_rateA;
		    Data_Arr[B1] = Flow_rateB>>8;
		    Data_Arr[B2] = (uint8_t)Flow_rateB;
		    Data_Arr[C1] = Flow_rateC>>8;
	        Data_Arr[C2] = (uint8_t)Flow_rateC;
			Data_Arr[KA1] = key_A>>8;
			Data_Arr[KA2] = (uint8_t)key_A;
		    Data_Arr[KB1] = key_B>>8;
			Data_Arr[KB2] = (uint8_t)key_B;
		    Data_Arr[KC1] = key_C>>8;
			Data_Arr[KC2] = (uint8_t)key_C;
			char tem[256] = { 0 };
			sprintf(tem,"A_DATA=%d|B_DATA=%d|C_DATA=%d|A_Switch=%d|B_Switch=%d|C_Switch=%d",(Data_Arr[A1]<<8)+Data_Arr[A2],(Data_Arr[B1]<<8)+Data_Arr[B2],(Data_Arr[C1]<<8)+Data_Arr[C2],
					(Data_Arr[KA1]<<8)+Data_Arr[KA2],(Data_Arr[KB1]<<8)+Data_Arr[KB2],(Data_Arr[KC1]<<8)+Data_Arr[KC2]);
			u3_printf("%s",tem);
			printf("%s\r\n",tem);
		}
		    OLED_ShowNum(2,1,(Data_Arr[A1]<<8)+Data_Arr[A2],4);
		    OLED_ShowNum(3,1,(Data_Arr[B1]<<8)+Data_Arr[B2],4);
		    OLED_ShowNum(4,1,(Data_Arr[C1]<<8)+Data_Arr[C2],4);

}
               
