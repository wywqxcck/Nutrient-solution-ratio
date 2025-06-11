#include "stm32f10x.h"                  // Device header
#include "my_usart1.h"
// 定义每升水对应的脉冲数，根据传感器实际规格调整
#define PULSES_PER_LITER 5400
// 定义判断脉冲结束的时间间隔（毫秒）
#define NO_PULSE_INTERVAL 2000  

// 存储每个引脚的脉冲计数
uint32_t pulseCountPB3;
uint32_t pulseCountPB4;
uint32_t pulseCountPB14;

// 流量变量
float totalFlowRatePB3 = 0; 
float totalFlowRatePB4 = 0;
float totalFlowRatePB14 = 0;
u16 Flow_A;
u16 Flow_B;
u16 Flow_C;
// 标记是否开始计数
volatile uint8_t isCountingPB3 = 0;
volatile uint8_t isCountingPB4 = 0;
volatile uint8_t isCountingPB14 = 0;

// 记录上次脉冲时间
volatile uint32_t lastPulseTimePB3 = 0;
volatile uint32_t lastPulseTimePB4 = 0;
volatile uint32_t lastPulseTimePB14 = 0;

float calculateFlow(uint32_t pulseCount);

void GPIO_EXTI_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能 GPIOB 和 AFIO 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    // 配置 PB3、PB4、PB14 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 解除 PB3、PB4 的调试功能锁定
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    // 将 PB3、PB4、PB14 连接到外部中断线
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);

    // 配置外部中断线 3、4、14
    EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4 | EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 配置 NVIC
    // 外部中断线 3
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 外部中断线 4
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 外部中断线 10 - 15
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 外部中断线 3 中断服务函数
void EXTI3_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
        if (!isCountingPB3) {
            isCountingPB3 = 1;
        }
        pulseCountPB3++;
        lastPulseTimePB3 = TIM1->CNT;
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

//外部中断线 4 中断服务函数
void EXTI4_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
        if (!isCountingPB4) {
            isCountingPB4 = 1;
        }
        pulseCountPB4++;
        lastPulseTimePB4 = TIM1->CNT;
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}

// 外部中断线 10 - 15 中断服务函数
void EXTI15_10_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line14) != RESET) {
        if (!isCountingPB14) {
            isCountingPB14 = 1;
        }
        pulseCountPB14++;
        lastPulseTimePB14 = TIM1->CNT;
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
} 
void TIM1_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能定时器 1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // 定时器 1 基本配置
    // 假设系统时钟为 72MHz
    TIM_TimeBaseStructure.TIM_Period = 72000 - 1; // 自动重载值，1ms 计数一次
    TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1; // 预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // 使能定时器 1 更新中断
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    // 配置 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能定时器 1
    TIM_Cmd(TIM1, ENABLE);
}

void TIM1_UP_IRQHandler(void) {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        uint32_t currentTime = TIM1->CNT;

        // 检查 PB3 是否长时间没有脉冲
        if (isCountingPB3 && (currentTime - lastPulseTimePB3) >= NO_PULSE_INTERVAL) {
            totalFlowRatePB3 = calculateFlow(pulseCountPB3);
			Flow_A = totalFlowRatePB3 * 1000;
            printf("a = %d\n",Flow_A);
//            pulseCountPB3 = 0;
            isCountingPB3 = 0;
        }

        // 检查 PB4 是否长时间没有脉冲
        if (isCountingPB4 && (currentTime - lastPulseTimePB4) >= NO_PULSE_INTERVAL) {
            totalFlowRatePB4 = calculateFlow(pulseCountPB4);
			Flow_B = totalFlowRatePB4 * 1000;
//            pulseCountPB4 = 0;
            isCountingPB4 = 0;
         }

        // 检查 PB14 是否长时间没有脉冲
        if (isCountingPB14 && (currentTime - lastPulseTimePB14) >= NO_PULSE_INTERVAL) {
            totalFlowRatePB14 = calculateFlow(pulseCountPB14);
			Flow_C = totalFlowRatePB14 * 1000;
//            pulseCountPB14 = 0;
            isCountingPB14 = 0;
        }

        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

// 计算流量（升）
float calculateFlow(uint32_t pulseCount) {
    return (float)pulseCount / (float)PULSES_PER_LITER;
}
