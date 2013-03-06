
//#include "globals.h"
#include "keyboard.h"
//#include "messages.h"
#include "core.h"
//#include <stdio.h>

#define KBD_PIN_LEFT		PD7
#define KBD_PIN_RIGHT		PD6
#define KBD_PIN_UP			PD5
#define KBD_PIN_DOWN		PB0
#define KBD_PIN_OK			PB1
#define KBD_PIN_GND			PB6

#define KBD_MASK (_BV(KBD_PIN_UP)|_BV(KBD_PIN_LEFT)|_BV(KBD_PIN_RIGHT))

#define KBD_MASK2 (_BV(KBD_PIN_DOWN)|_BV(KBD_PIN_OK)|_BV(KBD_PIN_GND))

#define KBD_PIN			PIND
#define KBD_PORT		PORTD
#define KBD_DDR			DDRD

#define KBD_PIN2		PINB
#define KBD_PORT2		PORTB
#define KBD_DDR2		DDRB

uint8_t lastKey,prevKey;
uint8_t kf1,kf2,kf3;



unsigned char getKeyCode(uint8_t ah,uint8_t ah2);


unsigned char getKeyCode(uint8_t ah,uint8_t ah2) {


	if (!(ah2 & _BV(KBD_PIN_OK))) {
		//printf("ok press");
		return KEY_OK;
	}
	if (!(ah & _BV(KBD_PIN_UP))) {
		//printf("up press");
		return KEY_UP;
	}
	if (!(ah2 & _BV(KBD_PIN_DOWN))) {
		//printf("down press");
		return KEY_DOWN;
	}
	if (!(ah & _BV(KBD_PIN_LEFT))) {
		//printf("left press");
		return KEY_LEFT;
	}
	if (!(ah2 & _BV(KBD_PIN_GND))) {
		//printf("gnd press");
		return KEY_GND;
	}
	if (!(ah & _BV(KBD_PIN_RIGHT))) {
		//printf("right press");
		return KEY_RIGHT;
	}
	return 0;
}
//----------------------------
void readKey() {

	lastKey=getKeyCode(KBD_PIN & KBD_MASK,KBD_PIN2 & KBD_MASK2);
	
	if (prevKey != lastKey) 
	{
		sendMessage(MSG_KEY_PRESS, lastKey);
	}
	prevKey = lastKey;
	setTimer(MSG_KEY_SCAN, 0, 70);
	return ;
}
//-------------------------------

//----------------------------------

void  KBD_init() {
	KBD_PORT |= KBD_MASK;
	KBD_DDR &= ~ KBD_MASK;

	KBD_PORT2 |= KBD_MASK2;
	KBD_DDR2 &= ~ KBD_MASK2;

	lastKey = 0;
	prevKey = 0;

	setHandler(MSG_KEY_SCAN, &readKey);


	setTimer(MSG_KEY_SCAN, 0, 70);
}
