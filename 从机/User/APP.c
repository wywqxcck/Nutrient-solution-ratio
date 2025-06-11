#include "APP.h"
extern uint16_t temperature;
uint16_t ADValue;
float voltage;
float temperature_calibrate;
float voltage_calibrate;
extern uint16_t TDS;
extern uint16_t EC;
extern uint16_t HMI_A,HMI_B,HMI_C;  //串口屏发的
static void Menu_Main(void);
static void Menu_Choice_A(void);
static void Menu_Choice_B(void);
static void Menu_Choice_C(void);
static void Menu_Set_A(void);
static void Menu_Set_B(void);
static void Menu_Set_C(void);

extern uint16_t A_volume;
extern uint16_t B_volume;
extern uint16_t C_volume;
uint8_t taskIndex = Main_Mune,key,send_sta = SEND_WAIT;	//任务调度序号
//界面调度表
Menu_table_t taskTable[] =
{
    //菜单界面函数 -- 一级界面
    {Main_Mune, Choice_A_Mune, Main_Mune, Main_Mune, Main_Mune, Menu_Main}, 
		
			//菜单界面函数 -- 二级界面
			{Choice_A_Mune, Set_A_Mune, Choice_B_Mune, Choice_C_Mune, Main_Mune, Menu_Choice_A}, 
			{Choice_B_Mune, Set_B_Mune, Choice_C_Mune, Choice_A_Mune, Main_Mune, Menu_Choice_B}, 
			{Choice_C_Mune, Set_C_Mune, Choice_A_Mune, Choice_B_Mune, Main_Mune, Menu_Choice_C}, 
		
				//菜单界面函数 -- 三级界面
				{Set_A_Mune, Set_A_Mune, Set_A_Mune, Set_A_Mune, Choice_A_Mune, Menu_Set_A}, 
				{Set_B_Mune, Set_B_Mune, Set_B_Mune, Set_B_Mune, Choice_A_Mune, Menu_Set_B}, 
				{Set_C_Mune, Set_C_Mune, Set_C_Mune, Set_C_Mune, Choice_A_Mune, Menu_Set_C}, 
				
};




static void Menu_Main(void)
{
	OLED_ShowCHinese(33,0,0);
	OLED_ShowCHinese(49,0,1);
	OLED_ShowCHinese(65,0,2);
	OLED_ShowCHinese(81,0,3);
	temperature = DS18B20_Get_Temp();
	ADValue = AD_GetValue();		//获取AD转换的值
	voltage = (float)ADValue / 4095 * 2.3;
    if(temperature >= 25)
	{
		temperature_calibrate = 1 + 0.02 * (temperature - 25);
			
	}
	else
	{
		temperature_calibrate = 1 - 0.02 * (25 - temperature);
	}
	voltage_calibrate = voltage * temperature_calibrate;
	TDS = (66.71*voltage_calibrate*voltage_calibrate*voltage_calibrate
					-127.93*voltage_calibrate*voltage_calibrate
					+428.7*voltage_calibrate) * 2.1;
	EC = TDS / 0.64;
	Delay_ms(200);
	char arr[100];
	sprintf(arr,"T=%d",temperature);
	OLED_ShowString(0,2,(u8*)arr,16);
	sprintf(arr,"EC=%d",EC);
	OLED_ShowString(0,4,(u8*)arr,16);
	sprintf(arr,"TDS=%d",TDS);
	OLED_ShowString(0,6,(u8*)arr,16);
	
	sprintf(arr,"A=%.4d",HMI_A);
	OLED_ShowString(70,2,(u8*)arr,16);
	sprintf(arr,"B=%.4d",HMI_B);
	OLED_ShowString(70,4,(u8*)arr,16);
	sprintf(arr,"C=%.4d",HMI_C);
	OLED_ShowString(70,6,(u8*)arr,16);
}


static void Menu_Choice_A(void)
{
	OLED_ShowCHinese(33,0,4);
	OLED_ShowCHinese(49,0,5);
	OLED_ShowCHinese(65,0,2);
	OLED_ShowCHinese(81,0,3);
	OLED_ShowString(0,2,(uint8_t*)"Choice_A    <<",16);
	OLED_ShowString(0,4,(uint8_t*)"Choice_B",16);
	OLED_ShowString(0,6,(uint8_t*)"Choice_C",16);
}


static void Menu_Choice_B(void)
{
	OLED_ShowCHinese(33,0,4);
	OLED_ShowCHinese(49,0,5);
	OLED_ShowCHinese(65,0,2);
	OLED_ShowCHinese(81,0,3);
	OLED_ShowString(0,2,(uint8_t*)"Choice_A",16);
	OLED_ShowString(0,4,(uint8_t*)"Choice_B    <<",16);
	OLED_ShowString(0,6,(uint8_t*)"Choice_C",16);
}


static void Menu_Choice_C(void)
{
	OLED_ShowCHinese(33,0,4);
	OLED_ShowCHinese(49,0,5);
	OLED_ShowCHinese(65,0,2);
	OLED_ShowCHinese(81,0,3);
	OLED_ShowString(0,2,(uint8_t*)"Choice_A",16);
	OLED_ShowString(0,4,(uint8_t*)"Choice_B",16);
	OLED_ShowString(0,6,(uint8_t*)"Choice_C    <<",16);
}

static void Menu_Set_A(void)
{
	char text_temp[20];
	
	OLED_ShowString(25,0,(uint8_t*)"SET Menu A",16);
	
	sprintf(text_temp,"A_volume = %.4d",A_volume);
	OLED_ShowString(0,3,(uint8_t*)text_temp,16);
	
	if(key == KEY_LAST){
		if(A_volume >= 50){
			A_volume -= 50;
		}
	}
	else if(key == KEY_NEXT){
		if(A_volume <= 4950){
			A_volume += 50;
		}
	}
	else if(key == KEY_SEND){
		HMI_A = A_volume;
		OLED_ShowString(30,6,(uint8_t*)"SET A OK",16);
			 PumpA_ON;
		printf("%d\r\n",A_volume);
//		if(send_sta == SEND_WAIT){
//			send_sta = SEND_ING;
//			OLED_ShowString(30,6,(uint8_t*)"SET A ING",16);
//		}
//		else if(send_sta == SEND_ING){
//			Delay_ms(2000);
//			send_sta = SEND_OK;
//			OLED_ShowString(30,6,(uint8_t*)"SET A O K",16);
//			Delay_ms(00);
//			send_sta = SEND_WAIT;
//		}
	}
	
	
	
}

static void Menu_Set_B(void)
{
	char text_temp[20];
	OLED_ShowString(25,0,(uint8_t*)"SET Menu B",16);
	
	sprintf(text_temp,"B_volume = %.4d",B_volume);
	OLED_ShowString(0,3,(uint8_t*)text_temp,16);
	
	
	if(key == KEY_LAST){
		if(B_volume >= 50){
			B_volume -= 50;
		}
	}
	else if(key == KEY_NEXT){
		if(B_volume <= 4950){
			B_volume += 50;
		}
	}
	else if(key == KEY_SEND){
		HMI_B = B_volume;
		OLED_ShowString(30,6,(uint8_t*)"SET B OK",16);
		PumpB_ON;
	}
}

static void Menu_Set_C(void)
{
	char text_temp[20];
	OLED_ShowString(25,0,(uint8_t*)"SET Menu C",16);
	
	sprintf(text_temp,"C_volume = %.4d",C_volume);
	OLED_ShowString(0,3,(uint8_t*)text_temp,16);

	if(key == KEY_LAST){
		if(C_volume >= 50){
			C_volume -= 50;
		}
	}
	else if(key == KEY_NEXT){
		if(C_volume <= 4950){
			C_volume += 50;
		}
	}
	else if(key == KEY_SEND){
		HMI_C = C_volume;
		OLED_ShowString(30,6,(uint8_t*)"SET C OK",16);
		PumpC_ON;
	}
}

