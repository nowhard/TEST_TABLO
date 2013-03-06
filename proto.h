#ifndef PROTO_H
#define PROTO_H

#include<avr/io.h>
#include<avr/interrupt.h>
#include<avr/pgmspace.h>
#include "timer1.h" 

#include <stdio.h>

#define HI(x) ((x)>>8)
#define LO(x) ((x)&0xFF)


#define ADRESS_DEV 			0x1 //����� ����������
#define DEV_NAME   			"GENERIC DEVICE"

#define DEV_ID				0x10
#define DEV_SN				0x1
//--------------------------------------------------------------------------------
#define MAX_LENGTH_REC_BUF 	25 //������������ ����� ������������ �����
#define MIN_LENGTH_REC_BUF	5 //����������� ����� ������������ �����

#define MAX_LENGTH_TR_BUF  	25 //������������ ����� ������������� �����
#define CRC_LEN				2 //����� ���� CRC
//-------------------------��� ��������-------------------------------------------
#define GET_DEV_INFO 		17 //�������� ���������� �� ����������
#define SET_SINGLE_REG		6 //���������� �������� ������ ��������
#define SET_MULTIPLE_REG	16 //���������� �������� ���������� ���������
#define READ_HOLDING_REG	3 //������ ��������� ��������
#define READ_INPUT_REG		4 //������ ������� ���������
//--------------------------------------------------------------------
#define REG_ADDR_MIN		0//������ ����� ���������
#define REG_ADDR_MAX		4//������� ����� ���������
//--------------------------��������� ��������� ��������--------------
#define MD_RESTART		0	//������� ����� � ���������
#define MD_WAIT 		1	//�������� ������
#define	MD_PLACE_BUF	2	//����� ������ � �����
#define	MD_ADDR_CHECK	3	//�������� ������
#define	MD_CRC_CHECK	4	//�������� ����������� �����	
#define	MD_BUF_HANDLING	5	//��������� �������
#define MD_BUF_TRANSFER	6	//�������� ������ ��������
#define	MD_ERR_HANDLING	7	//��������� ������
//--------------------------------------------------------------------
unsigned char 	MD_STATE;//������� ���������
unsigned char	volatile  buf_count;//������� ������
unsigned char	transf_count;//������� ������������ ������
unsigned char	buf_len;//����� ������������� ������
unsigned char	cnt_15; //������� 1.5 �������
unsigned char	cnt_35; //������� 3.5 �������
//------------------------����� ������--------------------------------
unsigned char CRC_ERR;	//������ crc
unsigned char COMMAND_ERR;//���������������� �������
unsigned char TIMEOUT_15; //������� 1.5
unsigned char TIMEOUT_35; //������� 3.5
//--------------------------------------------------------------------
unsigned char  RecieveBuf[MAX_LENGTH_REC_BUF] ; //����� ����������� ������
unsigned char  TransferBuf[MAX_LENGTH_TR_BUF] ; //����� ������������ ������
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//���������
ISR(TIMER1_OVF_vect);
void Protocol_Init(void); //������������� ���������
void Send_Info(void);     //������� ���������� �� ����������
void SetSingleRegister(void);//���������� �������� ������ ��������
void SetMultipleRegisters(unsigned char *regs);//���������� �������� ���������� ���������
void ReadHoldingReg(void);//������ ��������� ��������
//void ReadInputReg(unsigned char reg);//������ ������� ���������
//unsigned char ErrorMessage(unsigned char Error);//��������� �� ������
void ModBusBufHandling(void); //������� ��������� ��������� �������
void ModBusProcess(void);//������� ������� ��������� modbus
void Timer3_Interrupt(void);//��������� ���������� �������
unsigned int CRC16( unsigned char * pucFrame, unsigned int usLen );//������ CRC16

void stub(void);//�������� ��� ��������


//--------------------------------------------------------------------
unsigned int controller_reg[5];
#define REG_0 0
#define REG_1 1
#define REG_2 2
#define REG_3 3
#define REG_4 4
//--------------------------------------------------------------------
#endif
