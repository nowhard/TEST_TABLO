#ifndef TIMER1_H
#define TIMER1_H

#include<avr/io.h>
#include<avr/interrupt.h>

void Timer1_Init(void);//������������� timer1
void Timer1_Run(void);//������ timer1
void Timer1_Stop(void);//������� timer1
void Timer1_Reset(void);//����� timer1
#define MID_MAS_SIZE 4

#endif
