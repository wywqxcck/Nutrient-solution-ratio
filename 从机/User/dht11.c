#include "stm32f10x.h"
#include "dht11.h"
#include "delay.h"

uint8_t dat[5]={0x00,0x00,0x00,0x00,0x00};    //�洢��ȡ����ʪ����Ϣ
uint32_t sum=0;         //���У��ʱ�����

/****************************************
����PB12�˿�Ϊ���
*****************************************/
void DHT11_PortOutput(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;

 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;//�������
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//���50MHz
 GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/****************************************
����PB12�˿�Ϊ����
*****************************************/
void DHT11_PortInput(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;

 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//���50MHz
 GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/****************************************
�������ƣ�void DHT_Read_Byte()
�������ܣ���DHT�������е�һ���ֽڶ�ȡ����
�����������
����ֵ����
*****************************************/
uint8_t DHT_Read_Byte(void)
{
  uint8_t temp;      //��Ŷ�ȡ����λ����
	uint8_t ReadDat = 0;
  uint8_t i;
	uint8_t retry = 0;
  for(i=0;i<8;i++)
  {
    while(DHT11 ==0 && retry < 100)     //�ȴ�DHT11����ߵ�ƽ
		{
			delay_us(1);
			retry++;
		}
		retry = 0;
    delay_us(30);          //��ʱ30us�����ڡ�0������ߵ�ƽʱ��26~28us��'1'����ߵ�ƽʱ��70us,��ʱ30us���жϳ���1,����0
    temp = 0;             //�Ƚ��Ĵ�������
    if(DHT11 ==1)        //��ʱ30us֮��������Ǹߵ�ƽ��֤��Ϊ1����
      temp = 1;            //��1�洢
    while(DHT11 ==1 && retry < 100)      //�ȴ��źű����ͣ�����
		{
			delay_us(1);
			retry++;			
		}
		retry = 0;
    ReadDat<<=1;           //��������1λ������µõ�������
    ReadDat|= temp;         //�µõ������ݷŵ����1λ
  }
	return ReadDat;
}

/*********************************************
�������ƣ�unsigned char DHT_Read()
�������ܣ���ȡDHT11����ʪ��
�����������
����ֵ�� flag--���ݶ�ȡ��У��ɹ���־
*********************************************/
uint8_t DHT_Read(void)
{
  uint8_t i;
	uint8_t retry = 0;
  DHT11_PortOutput();           //�˿ڷ�������Ϊ���
  DHT11_LOW;           //�˿���������
  delay_ms(18);          //��ʱ18ms��ʱ��Ҫ��
  DHT11_HIGH;           //�˿���������
  delay_us(40);          //��ʱ40us,
  DHT11_PortInput();           //��������Ϊ����
  delay_us(20);          //��ʱ20us
  if(DHT11 ==0)          //�����ȡ���͵�ƽ��֤��DHT11��Ӧ
  {
    while(DHT11 ==0 && retry < 100)  //�ȴ���ߵ�ƽ
		{
			delay_us(1);
			retry++;				
		}
		retry = 0;
    while(DHT11 ==1 && retry < 100) //�ȴ���͵�ƽ
		{
			delay_us(1);
			retry++;	
		}
		retry = 0;
    for(i=0;i<5;i++)    //ѭ��5�ν�40λ����
     {
      dat[i] = DHT_Read_Byte();    //����1���ֽ�
     }
    delay_us(50);         //�����ʱ�ȴ�50us
  }
  sum=dat[0]+dat[1]+dat[2]+dat[3];     //ǰ4���ֽ����ݵĺ�
  if(dat[4]==(u8)(sum))     //ǰ4�����ݺ͵�ĩ8λҪ�͵�5��������ȣ������ȡ��ȷ
    {
      return 1;                  //У����ȷ������1
    }
  else
     return 0;                  //У����󣬷���0
}




