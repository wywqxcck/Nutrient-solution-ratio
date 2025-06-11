#include "stm32f10x.h"                  // Device header

// 定义引脚相关宏
#define ENCODER_PORT GPIOA
#define EN_A_PIN GPIO_Pin_0
#define EN_B_PIN GPIO_Pin_1

// 定义编码器定时器
#define ENCODER_TIM TIM2
void Encoder_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    // 使能相关时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 配置PA0、PA1为浮空输入，用于读取编码器信号
    GPIO_InitStructure.GPIO_Pin = EN_A_PIN | EN_B_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(ENCODER_PORT, &GPIO_InitStructure);


    // 时基单元初始化
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = 3;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(ENCODER_TIM, &TIM_TimeBaseStructure);

    // 输入捕获初始化通道1（对应EN_A）
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(ENCODER_TIM, &TIM_ICInitStructure);

    // 输入捕获初始化通道2（对应EN_B）
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(ENCODER_TIM, &TIM_ICInitStructure);

    // 配置编码器接口模式
    TIM_EncoderInterfaceConfig(ENCODER_TIM, TIM_EncoderMode_TI12, 
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    // 使能定时器
    TIM_Cmd(ENCODER_TIM, ENABLE);
}

int8_t Get_EC11(void)
{
	int8_t data = -1;
	if(TIM_GetCounter(ENCODER_TIM) != 0){
		char Dir = ((ENCODER_TIM->CR1 & TIM_CR1_DIR) == TIM_CR1_DIR); //读取转动方向：0为正、1为负
		
		if(Dir){
			data = 0;
		}
		else{
			data = 1;
		}
		
		TIM2->CNT = 0;
	}
	return data;
}
