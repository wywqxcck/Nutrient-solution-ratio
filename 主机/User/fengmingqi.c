#include "sys.h"

/*GPIO��ʼ��*/
void LED_GPIO_Config(void)
{
	//GPIO_InitTypeDef GPIO_InitStruct;  //ʾ����һ��GPIO���󣨱�����������������棩
	
	//����ʱ��
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK,ENABLE);
	
	//��������
	GPIO_InitTypeDef GPIO_InitStruct;  //ʾ����һ��GPIO���󣨱�����������������棩/��Ҳ����ֱ�ӷ������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  //�������ģʽΪ�������
	GPIO_InitStruct.GPIO_Pin = LED_GPIO_PIN;        //ѡ����������Ϊ13����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�����������
	
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);  //��������˼Ϊ�����üĴ����������д����Ҫ��
}

/*����*/
void LED_OFF(void)
{
	//����͵�ƽ
	GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);//����͵�ƽ����
}

/*�ص�*/
void LED_NO(void)
{
	//����ߵ�ƽ
	GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);//����ߵ�ƽϨ��
}
/*LED״̬�ı亯��*/
void LED_Change(void)
{
//	static u8 i =0;
//	if(flag == 1)
//	{
//		flag = 0;	
//		switch (i)
//		{
//			case 0:PCout(13) = 1;i++;break;
//			case 1:PCout(13) = 0;i=0;break;
//		}	
//	}	
}
