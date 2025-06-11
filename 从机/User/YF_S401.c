#include "stm32f10x.h"                  // Device header
#include "my_usart1.h"
// ����ÿ��ˮ��Ӧ�������������ݴ�����ʵ�ʹ�����
#define PULSES_PER_LITER 5400
// �����ж����������ʱ���������룩
#define NO_PULSE_INTERVAL 2000  

// �洢ÿ�����ŵ��������
uint32_t pulseCountPB3;
uint32_t pulseCountPB4;
uint32_t pulseCountPB14;

// ��������
float totalFlowRatePB3 = 0; 
float totalFlowRatePB4 = 0;
float totalFlowRatePB14 = 0;
u16 Flow_A;
u16 Flow_B;
u16 Flow_C;
// ����Ƿ�ʼ����
volatile uint8_t isCountingPB3 = 0;
volatile uint8_t isCountingPB4 = 0;
volatile uint8_t isCountingPB14 = 0;

// ��¼�ϴ�����ʱ��
volatile uint32_t lastPulseTimePB3 = 0;
volatile uint32_t lastPulseTimePB4 = 0;
volatile uint32_t lastPulseTimePB14 = 0;

float calculateFlow(uint32_t pulseCount);

void GPIO_EXTI_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // ʹ�� GPIOB �� AFIO ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

    // ���� PB3��PB4��PB14 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ��� PB3��PB4 �ĵ��Թ�������
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    // �� PB3��PB4��PB14 ���ӵ��ⲿ�ж���
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);

    // �����ⲿ�ж��� 3��4��14
    EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4 | EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // ���� NVIC
    // �ⲿ�ж��� 3
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // �ⲿ�ж��� 4
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // �ⲿ�ж��� 10 - 15
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// �ⲿ�ж��� 3 �жϷ�����
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

//�ⲿ�ж��� 4 �жϷ�����
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

// �ⲿ�ж��� 10 - 15 �жϷ�����
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

    // ʹ�ܶ�ʱ�� 1 ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // ��ʱ�� 1 ��������
    // ����ϵͳʱ��Ϊ 72MHz
    TIM_TimeBaseStructure.TIM_Period = 72000 - 1; // �Զ�����ֵ��1ms ����һ��
    TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1; // Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // ʹ�ܶ�ʱ�� 1 �����ж�
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    // ���� NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ʹ�ܶ�ʱ�� 1
    TIM_Cmd(TIM1, ENABLE);
}

void TIM1_UP_IRQHandler(void) {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        uint32_t currentTime = TIM1->CNT;

        // ��� PB3 �Ƿ�ʱ��û������
        if (isCountingPB3 && (currentTime - lastPulseTimePB3) >= NO_PULSE_INTERVAL) {
            totalFlowRatePB3 = calculateFlow(pulseCountPB3);
			Flow_A = totalFlowRatePB3 * 1000;
            printf("a = %d\n",Flow_A);
//            pulseCountPB3 = 0;
            isCountingPB3 = 0;
        }

        // ��� PB4 �Ƿ�ʱ��û������
        if (isCountingPB4 && (currentTime - lastPulseTimePB4) >= NO_PULSE_INTERVAL) {
            totalFlowRatePB4 = calculateFlow(pulseCountPB4);
			Flow_B = totalFlowRatePB4 * 1000;
//            pulseCountPB4 = 0;
            isCountingPB4 = 0;
         }

        // ��� PB14 �Ƿ�ʱ��û������
        if (isCountingPB14 && (currentTime - lastPulseTimePB14) >= NO_PULSE_INTERVAL) {
            totalFlowRatePB14 = calculateFlow(pulseCountPB14);
			Flow_C = totalFlowRatePB14 * 1000;
//            pulseCountPB14 = 0;
            isCountingPB14 = 0;
        }

        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

// ��������������
float calculateFlow(uint32_t pulseCount) {
    return (float)pulseCount / (float)PULSES_PER_LITER;
}
