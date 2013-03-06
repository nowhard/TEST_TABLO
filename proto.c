#include "proto.h"
//-----------------------------------------------------------------------------------
ISR(TIMER1_OVF_vect) //���������� ���������� �������3 
{	
	cnt_15++;
	cnt_35++;

	if(cnt_15>=3)//������ 1.5 �������� �����
	{
		TIMEOUT_15=0x1;
		cnt_15=0x0;
	}
	
	if(cnt_35>=7) //������ 3.5 �������� �����
	{
		Timer1_Stop();//��������� ������

		PORTB++;
	
		TIMEOUT_35=0x1;//���� �������� 
		cnt_35=0x0;

		if(buf_count<MIN_LENGTH_REC_BUF)
		{
			MD_STATE=MD_RESTART;		//������� � ���� ����� �������
			//�������� ��������� ������
						
		}
		else
		{
			MD_STATE=MD_ADDR_CHECK;//���� ������, ��������� �����	
		}
	}
		
	TIFR&=~(1<<TOV1);
	TCNT1H=0xFE;
	TCNT1L=0x5E;
}
//------------------------------------------------------------------------------
ISR(USART_RXC_vect)
{
	
	Timer1_Reset();
	if(TIMEOUT_15 || buf_count>MAX_LENGTH_REC_BUF)
	{
		buf_count=0x0;//�������� ����� �������� ������
	}
	
	RecieveBuf[buf_count]=UDR;//��������� �������� ������ � �����

	buf_count++;
	
	cnt_15=0x0;
	cnt_35=0x0;		
	TIMEOUT_15=0x0;//�������� ��������
	TIMEOUT_35=0x0;

   MD_STATE=MD_WAIT;		// ���� ����� �������

}
//------------------------------------------------------------------------------
ISR(USART_TXC_vect)//������� �������� ������
{
			if(transf_count<buf_len)
			{
				UDR=TransferBuf[transf_count];
				transf_count++;
			}
			else
			{
				transf_count=0x0;		//�������� �������
				MD_STATE=MD_RESTART;
			}
			//MD_STATE=MD_WAIT;		// ���� ����� �������
}
//------------------------------------------------------------------------------
 unsigned char aucCRCHi[256] PROGMEM = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

 unsigned char  aucCRCLo[256] PROGMEM = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};
//--------------------------------------------------------------------
//unsigned char  string[]={DEV_NAME};

void Protocol_Init(void)
{
    UCSRA&=~(1<<RXC);//���� ������ ����� �� UART
    UCSRA&=~(1<<TXC);//���� �������� ����� �� UART

	MD_STATE=MD_RESTART;//������� ���������
	buf_count=0x0;//������� ������ ������
	transf_count=0x0;//������� ������������ ������
	buf_len=0x0;//����� ������������� ������
	cnt_15=0x0; //������� 1.5 �������
	cnt_35=0x0; //������� 3.5 �������
	//------------------------����� ������--------------------------------
	CRC_ERR=0x0;	//������ crc
	COMMAND_ERR=0x0;//���������������� �������
	TIMEOUT_15=0x0; //������� 1.5
	TIMEOUT_35=0x0; //������� 3.5
}
//-----------------------------------------------------------------------------
void Send_Info(void)//���������� �� ����������
{
	unsigned int CRC=0x0;
	
	TransferBuf[0]=ADRESS_DEV;
	TransferBuf[1]=0x11;//�������� ����������
	TransferBuf[2]=0x4; //������� ����
	TransferBuf[3]=HI(DEV_ID);//id
	TransferBuf[4]=LO(DEV_ID);//id
	TransferBuf[5]=HI(DEV_SN);//SN
	TransferBuf[6]=LO(DEV_SN);//SN	


	CRC=CRC16(&TransferBuf,7);

	TransferBuf[7]=HI(CRC);
	TransferBuf[8]=LO(CRC);

	buf_len=0x9;				 		
}
//-----------------------------------------------------------------------------
void SetSingleRegister(void)//���������� �������� ������ ��������
{
	unsigned char i=0;
	unsigned int CRC=0x0;
	unsigned int addr;//����� ��������
	unsigned int num;//�������� ��������

	//-----------��������� ������� ��������------------
	addr=(((unsigned int)RecieveBuf[2])|((unsigned int)RecieveBuf[3]<<8));
	num=(((unsigned int)RecieveBuf[4]<<8)|((unsigned int)RecieveBuf[5]));

	if(addr>=REG_ADDR_MIN && addr<=REG_ADDR_MAX)
	{
			controller_reg[addr]=num;

			for (i=0;i<(buf_count-CRC_LEN);i++)
			{
				TransferBuf[i]=RecieveBuf[i];		
			}
			buf_len=buf_count;

			CRC=CRC16(&TransferBuf,buf_len-CRC_LEN);

			TransferBuf[buf_len-2]=HI(CRC);
			TransferBuf[buf_len-1]=LO(CRC);	
	}
	else
	{
		//��������� ������
		MD_STATE=MD_ERR_HANDLING;
	}
	//-------------------------------------------------	
}
//-----------------------------------------------------------------------------
void SetMultipleRegisters(unsigned char *regs)//���������� �������� ���������� ���������
{
}
//-----------------------------------------------------------------------------
void ReadHoldingReg(void)//������ ��������� ��������
{
#define HEAD_LEN	3
//	
//[addr|func|n_regs|....|crc|crc]-���� ������
//[		head	   |............]	
	unsigned char i=0,count=0;
	unsigned int CRC=0x0;
	unsigned int addr;//����� ���������� ��������
	unsigned int len;//���������� ����������� ���������

	TransferBuf[0]=ADRESS_DEV;
	TransferBuf[1]=READ_HOLDING_REG;//��������� �������� ��������

	//-----------������ ���������� ������ � ���������� ���������------------
	addr=(((unsigned int)RecieveBuf[2]<<8)|(unsigned int)RecieveBuf[3]);
	len= (((unsigned int)RecieveBuf[4]<<8)|(unsigned int)RecieveBuf[5]);

	if(addr>=REG_ADDR_MIN && addr<=REG_ADDR_MAX && (len+addr)<=REG_ADDR_MAX)
	{
		for(i=addr;i<(addr+len);i++)
		{
			TransferBuf[count+HEAD_LEN]=HI(controller_reg[i]);	
			TransferBuf[count+HEAD_LEN+1]=LO(controller_reg[i]);
			count+=2;
		}		
		TransferBuf[2]=count; //������� ����
		buf_len=HEAD_LEN+count+CRC_LEN; //����� ������������� ������

		CRC=CRC16(&TransferBuf,buf_len-CRC_LEN);

		TransferBuf[buf_len-2]=HI(CRC);
		TransferBuf[buf_len-1]=LO(CRC);		                     
	}
	else
	{
		//��������� ������
		MD_STATE=MD_ERR_HANDLING;
	}
	
	//-------------------------------------------------
}
//-----------------------------------------------------------------------------
/*void ReadInputReg(unsigned char reg)//������ ������� ���������
{
}*/
//-----------------------------------------------------------------------------
void ModBusBufHandling(void) //������� ��������� ��������� �������
{
  switch(RecieveBuf[1]){
//-------------�������� ���������� �� ����������----------------------------
  	case GET_DEV_INFO:
	{
		Send_Info();
	}	
	break;
//--------------���������� �������� ������ ��������-------------------------
  	case SET_SINGLE_REG:
	{
		SetSingleRegister();	
	}
	break;
//--------------���������� �������� ���������� ���������--------------------
  	case SET_MULTIPLE_REG:
	{
		
	}
	break;
//---------------������ ��������� ��������----------------------------------
  	case READ_HOLDING_REG:
	{
		//stub();
		ReadHoldingReg();	
	}
	break;
//---------------������ ������� ���������-----------------------------------
  	case READ_INPUT_REG:
	{
			
	}
	break;
//--------------------------------------------------------------------------
    default:
	{
       COMMAND_ERR=0x1;//�������������� �������
	   MD_STATE=MD_ERR_HANDLING;//�� ���������� ������
    }
  }
}
//-----------------------------------------------------------------------------------
void ModBusProcess(void)//������� ������� modbus
{
	switch(MD_STATE)
	{
//------------------------------------------
		case(MD_RESTART):
		{
		//	buf_count=0x0;//?? ��� ��������� ����� ���� �����������, ������� ����������
			Timer1_Reset();
			Timer1_Stop();
			
			cnt_15=0x0;
			cnt_35=0x0;

			TIMEOUT_15=0x0; //������� 1.5
			TIMEOUT_35=0x0; //������� 3.5
		
			MD_STATE=MD_WAIT;//��������� � ��������� �������� 
		}break;
//------------------------------------------
		case(MD_WAIT)://�������� ������
		{
			//�����
		}break;
//------------------------------------------
		case(MD_ADDR_CHECK):// �������� ������
		{			
			if(RecieveBuf[0]==ADRESS_DEV)//���� ����� ������
			{
				MD_STATE=MD_CRC_CHECK;	//��������� crc
			}
			else
			{
				MD_STATE=MD_RESTART;		//����� ���� ����� �������
			}
			
		}break;
//------------------------------------------
		case(MD_CRC_CHECK):// �������� CRC
		{
		//��� ���������� crc//���������� CRC ���������
		
			unsigned int CRC=0x0;
			CRC|=((RecieveBuf[buf_count-CRC_LEN]<<8)|(RecieveBuf[buf_count-CRC_LEN+1]));

					
			if(CRC16(&RecieveBuf,buf_count-CRC_LEN)==CRC)
			{
				MD_STATE=MD_BUF_HANDLING;
			}
			else
			{		
				CRC_ERR=0x1;//������������ CRC
	   			MD_STATE=MD_ERR_HANDLING;//�� ���������� ������ 
			}	
		}break;
//------------------------------------------
		case(MD_BUF_HANDLING):// ��������� ��������� ������
		{	
				ModBusBufHandling();//��������� ��������� ���������	
				MD_STATE=MD_BUF_TRANSFER;//����� ��������� �������� ���������		
		}break;
//------------------------------------------
		case(MD_BUF_TRANSFER):// �������� ������
		{
					UDR=TransferBuf[transf_count];//�������� �����, ��������� �������� �������
					transf_count++;	
					MD_STATE=MD_WAIT;//��������� ������ � ������� � ����		
		}break;
//------------------------------------------
		case(MD_ERR_HANDLING):// ��������� ������ // ����� �� �������������
		{
		
		//---��������� ������----
			CRC_ERR=0x0;	//������ crc
			COMMAND_ERR=0x0;//���������������� �������	

			MD_STATE=MD_RESTART;		//����� ������� � ���� ����� �������
		}break;
//------------------------------------------
	}
}
//-----------------------������ CRC16------------------------------------------------------------
unsigned int CRC16( unsigned char * pucFrame, unsigned int usLen )
{
    unsigned char           ucCRCHi = 0xFF;
    unsigned char           ucCRCLo = 0xFF;
    int             iIndex;

    while( usLen-- )
    {
        iIndex = ucCRCLo ^ *( pucFrame++ );
        ucCRCLo = ( unsigned char )( ucCRCHi ^ pgm_read_byte(&aucCRCHi[iIndex])/*aucCRCHi[iIndex]*/ );
        ucCRCHi = pgm_read_byte(&aucCRCLo[iIndex]);//aucCRCLo[iIndex];
    }
    return ( unsigned int )( ucCRCHi  | ucCRCLo<< 8 );
}
//---------------------------------------------------------------------------------------------
void stub(void)//�������� ��� ��������
{
unsigned int CRC=0;
	TransferBuf[0]=ADRESS_DEV;
	TransferBuf[1]=0x3;//read reg
	TransferBuf[2]=0x2;
	TransferBuf[3]=0x1;
	TransferBuf[4]=0x1;
	//TransferBuf[5]=0x1;
	
	CRC=CRC16(&TransferBuf,5);

	TransferBuf[5]=HI(CRC);
	TransferBuf[6]=LO(CRC);

	buf_len=0x7;
}
