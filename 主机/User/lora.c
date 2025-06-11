#include "lora.h"
#include "sys.h"
#include "delay.h"
#include "usart3.h"
#include "string.h"
#include "stdio.h"
#include "led.h"
#include "key.h"
#include "usart3.h"
#include "Serial.h"
#include "oled.h"
//#include "bh1750.h"

//设备工作模式(用于记录设备状态)
u8 Lora_mode=0;//0:配置模式 1:接收模式 2:发送模式
static u8 Int_mode=0;//0:关闭 1:上升沿 2:下降沿

EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;


//lora发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//其他,期待应答结果的位置(str的位置)
u8* lora_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

//lora发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 lora_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while((USART3->SR&0X40)==0);//等待上一次数据发送完成  
		USART3->DR=(u32)cmd;
	}else u3_printf("%s\r\n",cmd);//发送命令
	
	if(ack&&waittime)		//需要等待应答
	{
	   while(--waittime)	//等待倒计时
	   { 
		  Delay_ms(10);
		  if(USART3_RX_STA&0X8000)//接收到期待的应答结果
		  {
			  if(lora_check_cmd(ack))break;//得到有效数据 
			  USART3_RX_STA=0;
		  } 
	   }
	   if(waittime==0)res=1; 
	}
	return res;
} 




u8 LoRa_Init(void)
{
   u8 retry=0;
	 u8 temp=1;
	
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能PA端口时钟
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//禁止JTAG,从而PA15可以做普通IO使用,否则PA15不能做普通IO!!!	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	    		 //LORA_MD0
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LORA_AUX
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		     //下拉输入
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.4
	
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	
	   EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
  	 EXTI_InitStructure.EXTI_LineCmd = DISABLE;              //中断线关闭(先关闭后面再打开)
  	 EXTI_Init(&EXTI_InitStructure);//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	   NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//LORA_AUX
  	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2， 
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		//子优先级3
  	 NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; //关闭外部中断通道（后面再打开）
   	 NVIC_Init(&NVIC_InitStructure); 
	 
		LORA_MD0=0;
		LORA_AUX=0;
	
	 while(LORA_AUX)//确保LORA模块在空闲状态下(LORA_AUX=0)
	 {
		 Delay_ms(500);
         
	 }
	 usart3_init(115200);//初始化串口3
	 
	 LORA_MD0=1;//进入AT模式
	 Delay_ms(40);
	 retry=3;
	 while(retry--)
	 {
		 if(!lora_send_cmd("AT","OK",70))
		 {
			 temp=0;//检测成功
			 break;
		 }	
	 }
	 if(retry==0) temp=1;//检测失败
	 return temp;
}


//配置LORA的信道、速率、串口、发射功率、传输模式等
void Lora_set(void)
{
    usart3_init(115200);
		usart3_rx(1);//开启串口3接收
		while(LORA_AUX);//等待模块空闲
		LORA_MD0=1; //进入配置模式
		Delay_ms(40);
		Lora_mode=0;//标记"配置模式"
   
	  lora_send_cmd((u8 *)"AT+CWMODE=0",(u8 *)"OK",200); //设置为一般模式
	  printf("\r\nAT+CWMODE=0  OK\r\n");
	  lora_send_cmd((u8 *)"AT+TMODE=0",(u8 *)"OK",200); //设置为透明传输
	  printf("\r\nAT+TMODE=0  OK\r\n");
	  lora_send_cmd((u8 *)"AT+TPOWER=3",(u8 *)"OK",200); //设置发射功率20db
	  printf("\r\nAT+TPOWER=3  OK\r\n");
	  lora_send_cmd((u8 *)"AT+WLRATE=24,5",(u8 *)"OK",200); //设置信道为24，空中速率2.4
	   printf("\r\nAT+WLRATE=24,5  OK\r\n");
      lora_send_cmd((u8 *)"AT+WLTIME=0",(u8 *)"OK",200); //设置休眠时间为1S
	    printf("\r\nAT+WLTIME=0  OK\r\n");
	  lora_send_cmd((u8 *)"AT+UART=7,0",(u8 *)"OK",200);//设置波特率115200，无校验 
	printf("\r\nAT+UART=7,0  OK\r\n");
	   
		LORA_MD0=0;//退出配置,进入通信
	  Delay_ms(40);
		while(LORA_AUX);//判断是否空闲(模块会重新配置参数)
		USART3_RX_STA=0;
		Lora_mode=1;//标记"接收模式"
	  usart3_init(115200);
		Aux_Int(1);//设置LORA_AUX上升沿中断	
	
}

//AUX中断设置
//mode:配置的模式 0:关闭 1:上升沿 2:下降沿
void Aux_Int(u8 mode)
{
    if(!mode)
	{
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//关闭中断
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;	
	}else
	{
		if(mode==1)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿
	    else if(mode==2)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿
		
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	}
	Int_mode = mode;//记录中断模式
	EXTI_Init(&EXTI_InitStructure);
	NVIC_Init(&NVIC_InitStructure);  
    
}

//LORA_AUX中断服务函数
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4))
	{  
	   if(Int_mode==1)//上升沿(发送:开始发送数据 接收:数据开始输出)     
	   {
		  if(Lora_mode==1)//接收模式
		  {
			 USART3_RX_STA=0;//数据计数清0
		  }
		  Int_mode=2;//设置下降沿触发
	   }
       else if(Int_mode==2)//下降沿(发送:数据已发送完 接收:数据输出结束)	
	   {
		  if(Lora_mode==1)//接收模式
		  {
			 USART3_RX_STA|=1<<15;//数据计数标记完成
		  }else if(Lora_mode==2)//发送模式(串口数据发送完毕)
		  {
			 Lora_mode=1;//进入接收模式
		  }
		  Int_mode=1;//设置上升沿触发
  
	   }
       Aux_Int(Int_mode);//重新设置中断边沿
	   EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE4上的中断标志位  
	}	
}


//LORA模块发送数据
void LoRa_SendData(void)
{    
	u8 temp[100];
	sprintf((char*)temp,"光照强度");
	u3_printf("%s\r\n",temp);
}
static uint8_t flage = 0;
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

//Lora模块接收数据
void LoRa_ReceData(void)
{
		u16 len=0;
  if(USART3_RX_STA&0x8000)
	{
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;//添加结束符
		USART3_RX_STA=0;
		flage = 1;
  }
}








