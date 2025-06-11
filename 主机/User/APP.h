#ifndef __APP_H_
#define __APP_H_

#include "stm32f10x.h"
#include "stdint.h"
#include "Delay.h"


#define RX_TIMEOUT_VALUE                            1000  // ���ճ�ʱ��ֵ����λms
#define BUFFER_SIZE                                 64    // ���ݸ��س��ȣ�ʹ��64bit���ݳ���
#define DATA_MAX 13

enum Data{
	A1 = 1,
	A2,
	B1,
	B2,
	C1,
	C2,
	KA1,
	KA2,
	KB1,
	KB2,
	KC1,
	KC2,
};


void Send_Data(void);
#endif

