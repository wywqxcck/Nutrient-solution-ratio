#include "sys.h"

extern u8 key1,key2,key3;
/*GPIO��ʼ��*/
void LED3_GPIO_Config(void)
{
	//GPIO_InitTypeDef GPIO_InitStruct;  //ʾ����һ��GPIO���󣨱�����������������棩
	
	//����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//��������
	GPIO_InitTypeDef GPIO_InitStruct;  //ʾ����һ��GPIO���󣨱�����������������棩/��Ҳ����ֱ�ӷ������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  //�������ģʽΪ�������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8;        //ѡ����������Ϊ568����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�����������
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);  //��������˼Ϊ�����üĴ����������д����Ҫ��
}

/*��ƿ���*/
void RED_LED_ON(void)
{
	//����͵�ƽ
	GPIO_ResetBits(GPIOB,GPIO_Pin_5 );//����͵�ƽ����
}
/*��ƹص�*/
void RED_LED_OFF(void)
{
	//����ߵ�ƽ
	GPIO_SetBits(GPIOB,GPIO_Pin_5);//����ߵ�ƽϨ��
}


/*�̵ƿ���*/
void GREEN_LED_ON(void)
{
	//����͵�ƽ
	GPIO_ResetBits(GPIOB,GPIO_Pin_6 );//����͵�ƽ����
}
/*�̵ƹص�*/
void GREEN_LED_OFF(void)
{
	//����ߵ�ƽ
	GPIO_SetBits(GPIOB,GPIO_Pin_6);//����ߵ�ƽϨ��
}

/*�Ƶƿ���*/
void YELLOW_LED_ON(void)
{
	//����͵�ƽ
	GPIO_ResetBits(GPIOB,GPIO_Pin_8 );//����͵�ƽ����
}
/*�Ƶƹص�*/
void YELLOW_LED_OFF(void)
{
	//����ߵ�ƽ
	GPIO_SetBits(GPIOB,GPIO_Pin_8);//����ߵ�ƽϨ��
}

/*���״̬��ת*/
void RED_LED_Change(void)
{
	static u8 i =0;
	if(key1 == 1)
	{
		key1 = 0;	
		switch (i)
		{
			case 0:PBout(5) = 1;i++;break;
			case 1:PBout(5) = 0;i=0;break;
		}	
	}	
}

/*�̵�״̬��ת*/
void GREEN_LED_Change(void)
{
	static u8 i =0;
	if(key2 == 1)
	{
		key2 = 0;	
		switch (i)
		{
			case 0:PBout(6) = 1;i++;break;
			case 1:PBout(6) = 0;i=0;break;
		}	
	}	
}

/*�Ƶ�״̬��ת*/
void YELLOW_LED_Change(void)
{
	static u8 i =0;
	if(key3 == 1)
	{
		key3 = 0;	
		switch (i)
		{
			case 0:PBout(8) = 1;i++;break;
			case 1:PBout(8) = 0;i=0;break;
		}	
	}	
}

