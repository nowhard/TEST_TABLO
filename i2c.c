#include "i2c.h"

static volatile unsigned char second, minute, hour,day,month,year, read_time_state=0;
static unsigned int i2c_err_tim=0;
//---------------------I2C_INIT------------------------------------
void INIT_I2C(void)
{
	TWBR=0xF;
}
//---------------------I2C_WRITE------------------------------------

void Write_I2C(unsigned char address,unsigned char address_reg, unsigned char data)
{
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//START
	
	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=START)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}

	TWDR=address;//address+write
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=MT_SLA_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}
	
	TWDR=(unsigned char)address_reg;//address+write
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=MT_DATA_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}

	TWDR=data;//address+write
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=MT_DATA_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}

	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP

}
//---------------------I2C_READ------------------------------------

unsigned char Read_I2C(unsigned char address,unsigned char address_reg)
{

unsigned char second;

TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//START
	
	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=START)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return 0;
	}

	TWDR=address;//address+write
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=MT_SLA_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return 0;
	}
	
	TWDR=address_reg;//
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=MT_DATA_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return 0;
	}


//------------------read---------------------
TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//START2
	
	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=START_2)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return 0;
	}


	
	TWDR=address+1;//address+read
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	while(!(TWCR&(1<<TWINT)));//...wait for start

	if((TWSR&0xF8)!=MT_SLA_READ_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP	
		return 0;
	}

    
	

	TWCR=(1<<TWINT)|(1<<TWEN);//read & nack

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}
	
	if((TWSR&0xF8)!=READ_NACK)
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return 0;
		
	}
	second=TWDR;
    
	
	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP

	return second;

}


//--------------------------EEPROM READ---------------------------
unsigned char I2C_Read_Byte(unsigned char address,unsigned int address_mem)
{
	unsigned char byte;
	
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//START
	
	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=START)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}

	TWDR=address;//address+write
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	while(!(TWCR&(1<<TWINT)));//...wait for start

	if((TWSR&0xF8)!=MT_SLA_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}
	

	TWDR=(address_mem>>8)&0xFF;//address+write
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=MT_DATA_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}

    TWDR=(address_mem)&0xFF;//address+write
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=MT_DATA_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}

///	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
//------------------read---------------------
TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//START2
	
	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=START_2)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return 0;
	}


	
	TWDR=address+1;//address+read
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=MT_SLA_READ_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP	
		return 0;
	}

    
	

	TWCR=(1<<TWINT)|(1<<TWEN);//read & nack

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}
	
	if((TWSR&0xF8)!=READ_NACK)
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return 0;
		
	}
	byte=TWDR;
    
	
	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP

	return byte;
	
}
//---------------------EEPROM WRITE-------------------------------
void I2C_Write_Byte(unsigned char address,unsigned char address_mem, unsigned char data)
{
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//START
	
	while(!(TWCR&(1<<TWINT)));//...wait for start

	if((TWSR&0xF8)!=START)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}

	TWDR=address;//address+write
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=MT_SLA_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}
	
	TWDR=(address_mem>>8)&0xFF;//address+write
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=MT_DATA_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}

    TWDR=(address_mem)&0xFF;//address+write
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=MT_DATA_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}

	TWDR=data;//address+write
	
	TWCR=(1<<TWINT)|(1<<TWEN);//transfer address

	i2c_err_tim=1000;//выставим тайминг
	while(!(TWCR&(1<<TWINT)))//...wait for start
	{
		i2c_err_tim--;
		if(i2c_err_tim==0)
		{
			return 0;
		}
	}

	if((TWSR&0xF8)!=MT_DATA_ACK)//result
	{
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP
		return;
	}

	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//STOP

}
//------------------------------------------------------------
void ReadTime(Clock *clk,unsigned char read_type)//чтение часов FM30C256
{	
	switch(read_time_state)
	{
		case 0:
		{
			Write_I2C(TMR_ADDR,0x0,0x1);//считать 
			read_time_state=1;
		}
		break;

		case 1:
		{
			Write_I2C(TMR_ADDR,0x0,0x0);//считать
			read_time_state=2;
		}
		break;

		case 2:
		{
			switch(read_type)
			{
				case 0://time
				{
					clk->minute=Read_I2C(TMR_ADDR,0x3);
					clk->minute=((clk->minute&0xF0)>>4)*10+(clk->minute&0x0F);
					
					if(clk->minute>59)
					{
						clk->minute=59;
					}					

					clk->hour=Read_I2C(TMR_ADDR,0x4);
					clk->hour=((clk->hour&0xF0)>>4)*10+(clk->hour&0x0F);

					if(clk->hour>23)
					{
						clk->hour=23;
					}
				}
				break;

				case 1://date
				{
					clk->day=Read_I2C(TMR_ADDR,0x6);
					clk->day=((clk->day&0xF0)>>4)*10+(clk->day&0x0F);
				
					if(clk->day>31)
					{
						clk->day=31;
					}
									
					clk->month=Read_I2C(TMR_ADDR,0x7);
					clk->month=((clk->month&0xF0)>>4)*10+(clk->month&0x0F);
				
					if(clk->month>12)
					{
						clk->hour=12;
					}
				}
				break;

				case 2://year
				{
					clk->year=Read_I2C(TMR_ADDR,0x8);
					clk->year=(((clk->year)>>4)&0x0F)*10+(clk->year&0x0F);
				
					if(clk->year>99)
					{
						clk->year=99;
					}
				}
				break;

				default:
				{
				}
				break;
			}
			read_time_state=0;
		}
		break;

		default:
		{
			read_time_state=0;
		}

	}
}

//----------------------------------------------------------
void ClockInit(void)//инициализация часов, установка
{

uint8_t reg;

//Write_I2C(TMR_ADDR,0x0,0x4);//CAL 
//_delay_us(1000);

Write_I2C(TMR_ADDR,0x1,0x3F);//CAL 
_delay_us(1000);


reg=Read_I2C(TMR_ADDR,0x1);
_delay_us(1000);

//reg=Read_I2C(TMR_ADDR,0x0);

if((reg&0x80)==0)
	return;

Write_I2C(TMR_ADDR,0x0,0x2);// 
_delay_us(100);

//printf("write on\n");

Write_I2C(TMR_ADDR,0x4,(1<<4)+2);// 
_delay_us(100);
//printf("hour ok\n");

Write_I2C(TMR_ADDR,0x3,(0<<4)+0);// 
_delay_us(100);
//printf("minute ok\n");

Write_I2C(TMR_ADDR,0x2,0);// 
_delay_us(100);
//printf("second ok\n");

Write_I2C(TMR_ADDR,0x0,0x0);// 
_delay_us(100);
//printf("write off\n");

Write_I2C(TMR_ADDR,0x1,0x0);
_delay_us(100);
}
//---------------------------------------------------
void StoreTime(Clock *clk, unsigned char save_type)//структура времени и что сохранять(время, дату год..)
{
	Write_I2C(TMR_ADDR,0x0,0x2);// 
	_delay_us(100);

	switch(save_type)
	{
		case 0://time
		{
			Write_I2C(TMR_ADDR,0x3,((clk->minute/10)<<4)|((clk->minute%10)&0xF));// 
			_delay_us(100);	
			Write_I2C(TMR_ADDR,0x4,((clk->hour/10)<<4)|((clk->hour%10)&0xF));// 
		}
		break;

		case 1://data
		{
			Write_I2C(TMR_ADDR,0x6,((clk->day/10)<<4)|((clk->day%10)&0xF));// 
			_delay_us(100);	
			Write_I2C(TMR_ADDR,0x7,((clk->month/10)<<4)|((clk->month%10)&0xF));// 
		}
		break;

		case 2://year
		{
			Write_I2C(TMR_ADDR,0x8,((clk->year/10)<<4)|((clk->year%10)&0xF));
		}
		break;

		default:
		{
		}
		break;
	}
	_delay_us(100);
	Write_I2C(TMR_ADDR,0x0,0x0);// 
	_delay_us(100);


	Write_I2C(TMR_ADDR,0x1,0x0);
	_delay_us(100);	
}
//--------------------------------------------------------
