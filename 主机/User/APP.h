#ifndef __APP_H_
#define __APP_H_

#include "stm32f10x.h"
#include "stdint.h"
#include "Delay.h"


#define RX_TIMEOUT_VALUE                            1000  // 接收超时的值，单位ms
#define BUFFER_SIZE                                 64    // 数据负载长度，使用64bit数据长度
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

