#ifndef I2C_H
#define I2C_H

#include<avr/io.h>
#include<avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>

//---------------------I2C-------------------------------
#define START 			0x08
#define	MT_SLA_ACK		0x18
#define	MT_SLA_READ_ACK	0x40
#define MT_DATA_ACK		0x28
#define	START_2			0x10
#define	READ_ACK		0x50
#define	READ_NACK		0x58

#define SLA_ADDR		0xA0
#define TMR_ADDR		0xD0
//-------------------------------------------------------
typedef struct 
{
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	unsigned char year;
} Clock;//��������� �������
//-------------------------------------------------------  
void INIT_I2C(void);//�������������
void Write_I2C(unsigned char address,unsigned char address_reg, unsigned char data);//�������� ����
unsigned char Read_I2C(unsigned char address,unsigned char address_reg);//������� ����
unsigned char I2C_Read_Byte(unsigned char address,unsigned int address_mem);//�������� ���� � eeprom
void I2C_Write_Byte(unsigned char address,unsigned char address_mem, unsigned char data);//������� ���� �� eeprom

void ReadTime(Clock *clk,unsigned char read_type);//������ ����� FM30C256
void ClockInit(void);//������������� �����, ���������
void StoreTime(Clock *clk, unsigned char save_type);//��������� ������� � ��� ���������(�����, ���� ���..)
#endif
