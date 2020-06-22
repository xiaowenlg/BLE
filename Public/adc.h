#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"
void Init_All_Periph(void);
void filter(void);
u16 GetVolt(u16 advalue);
extern  u16 After_filter[3];
#endif /* __ADC_H */

