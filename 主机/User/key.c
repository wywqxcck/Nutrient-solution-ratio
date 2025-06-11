#include "sys.h"

u8 key1,key2,key3,key4;//������־λ

void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//����ʱ��
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_2|GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//����Ϊ����ģʽ
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
}

/*����ɨ�躯��*/
void KEY_Scan()
{
	if(PAin(0) == 0)
	{
		delay_ms(5);
		if(PAin(0) == 0)
		{
			key4=1;
		}
		while(!PAin(0));
	}
	
	if(PAin(5) == 0)
	{
		delay_ms(5);
		if(PAin(5) == 0)
		{
			key3=1;
		}
		while(!PAin(5));
	}
	
	if(PAin(6) == 0)
	{
		delay_ms(5);
		if(PAin(6) == 0)
		{
			key2=1;
		}
		while(!PAin(6));
	}
	
	if(PAin(7) == 0)
	{
		delay_ms(5);
		if(PAin(7) == 0)
		{
			key1=1;
		}
		while(!PAin(7));
	}
}

u8 KEY_Scan_123(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return 1;
	}else if(KEY0==1)key_up=1; 	    
 	return 0;// �ް�������
}

