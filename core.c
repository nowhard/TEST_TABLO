#include "core.h"

// ��������� ������ �� ������ ������������
typedef struct{
  msg_num msg; // �������������� �������
  handler hnd; // ���������� ��� ����������
} iHandler;

// ��������� ������� �� ������ �������
typedef struct{
  msg_num msg; // ����� �������
  msg_par par; // ��������
} iMessage;

// ��������� �������
typedef struct{
  msg_num msg; // ����� ������������� ���������
  msg_par par; // ��� ��������
  unsigned int time; // ������ � �������� ����� (������ 1 ����)
} iTimer;

static volatile iTimer    lTimer[maxTimers]={0}; // ������ ��������
static volatile iHandler   lHandler[maxHandlers]={0}; // ������ ������������

static volatile iMessage  lMessage[maxMessages]={0}; // ����� ���������
static volatile unsigned int  lMesPointer=0,hMesPointer=0; // ��������� �� ������ � ����� ������

//------------------------------------------------------------------------------
// ��������� ����������� �������
// ����������: setHandler(MSG_KEY_PRESS, &checkKey);
void setHandler(msg_num msg, handler hnd) {
  unsigned char i,j;
  i=0; j=0;
  while (i<maxHandlers) {
    if (lHandler[i].msg==0) { // ���� ��������� �����
      lHandler[i].hnd = hnd; // � ����������� ����������
      lHandler[i].msg = msg;
      break;
    }
    i++;
  }
}
//------------------------------------------------------------------------------
// ������ ����������� �������
// ����������: killHandler(MSG_KEY_PRESS, &checkKey);
/*void killHandler(msg_num msg, handler hnd) {
  unsigned char i,j;
  i=0; j=0;
  while (i<maxHandlers) {
    
    if ((lHandler[i].msg==msg) && (lHandler[i].hnd==hnd)) {
      lHandler[i].msg = 0; // ���� ����� ������, �������
    }
    
    if (lHandler[i].msg != 0) {
      if (i != j) { // �������� ��� ������ � ������ ������, ����� ����� �� ����
        lHandler[j].msg = lHandler[i].msg;
        lHandler[j].hnd = lHandler[i].hnd;
        lHandler[i].msg = 0;
      }
      j++;
    }
    i++;
  }
}*/
//------------------------------------------------------------------------------
// ������� ������� � �������
// ������ ������: sendMessage(MSG_KEY_PRESS, KEY_MENU)
void sendMessage(msg_num msg, msg_par par) 
{
  hMesPointer = (hMesPointer+1) & (maxMessages-1); // �������� ��������� ������
  
  lMessage[hMesPointer].msg = msg; // ������� ������� � ��������
  lMessage[hMesPointer].par = par;
  if (hMesPointer == lMesPointer) 
  { // ������� ������ �������, ������� �������������� ��������
    lMesPointer = (lMesPointer+1) & (maxMessages-1);
  }
}
//------------------------------------------------------------------------------
// ��������� �������
void dispatchMessage(void) 
 {	//�������� ��������, ��������������
  char volatile i;
  unsigned char volatile res; //���������
  msg_num volatile msg;
  msg_par volatile par;
  
  if (hMesPointer == lMesPointer) 
  { // ���� ������ ������� - �������
    return;
	
  }
  
  lMesPointer = (lMesPointer+1) & (maxMessages-1); // ������� ���������
  
  msg = lMessage[lMesPointer].msg;
  par = lMessage[lMesPointer].par;
  
  if (msg==0)
    return;

  
  for(i=maxHandlers-1; i>=0; i--) 
  { 
  // ������������� ����������� � �����
    if (lHandler[i].msg==msg) 
	{ 
	// ��������� ���������� ����� ���������
      res = lHandler[i].hnd(par); // �������� ����������
		 
      if (res)
	   { // ���� ���������� ������ 1, �������� ��������� �������
        break;
       }
    }
  }
  
}
//------------------------------------------------------------------------------
// ���������� ������
// ������ ������: setTimer(MSG_LCD_REFRESH, 0, 50);


void setTimer(msg_num msg, msg_par par, unsigned int time) {
  unsigned char i,firstFree;
  firstFree = maxTimers+1;
  if (time == 0) 
  {
    sendMessage(msg, par);
  } 
  else 
  {
  
    for (i=0; i<maxTimers; i++) 
	{ 
	   // ���� ������������� ������
      if (lTimer[i].msg == 0) 
	  {
        firstFree = i;
      } 
	  else 
	  { 
	  // ���� ����� - ��������� �����
        if ((lTimer[i].msg == msg) && (lTimer[i].par == par))
	    {
          lTimer[i].time = time;
          return;
        }  
      }
    }
    if (firstFree <= maxTimers)
	{ // ����� - ������ ��������� �����
      lTimer[firstFree].msg = msg;
      lTimer[firstFree].par = par;
      lTimer[firstFree].time = time;
    }
  }
}
//------------------------------------------------------------------------------
// ����� ������
// ����������� - ������� ��� ������������� ������� �� ������ �������,
// �� �������� �� ��������� �������
/*void killTimer(msg_num msg) {
  unsigned char i;
  for (i=0; i<maxTimers; i++) {
    if (lTimer[i].msg == msg) {
      lTimer[i].msg = 0;
    }
  }
}*/
//------------------------------------------------------------------------------
// ��������� ��������
void dispatchTimer() {	 //������ ��������
  unsigned char i;
  msg_num msg;
  msg_par par;
  
  for (i=0; i<maxTimers; i++) 
  {    
		if (lTimer[i].msg == 0)
		{
	      continue;
		}
	    
	    if (lTimer[i].time == 0) 
		{ 
		// ���� ������ �����
	      msg = lTimer[i].msg;
	      par =lTimer[i].par;
	      lTimer[i].msg = 0;
		  	 
	      sendMessage(msg, par); // ������� �������
		  //LED1=~LED1; 
	    } 
	else 
	{
      lTimer[i].time--; // ����� ������ ��������� �����
    }
  }
}
//#pragma ot(0)
//------------------------------------------------------------------------------
//#pragma AREGS
ISR(TIMER0_OVF_vect) //���������� ���������� �������0 
{
	dispatchTimer(); 
	TCNT0=130;
	TIFR&=!(1<<TOV0);
	
	return;
}
//#pragma NOAREGS
