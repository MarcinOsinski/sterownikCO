#ifndef FUNKCJE_H
#define FUNKCJE_H

#include <avr/io.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/eeprom.h>

#include "HD44780.h"
#include "ds18b20.h"



/////////////////////////	PASSWORD	/////////////////////////
#define PASSWORD1	0
#define PASSWORD2	0
#define PASSWORD3	0
#define PASSWORD4	1
/////////////////////////////////////////////////////////////

////////////////////	GRANICE NASTAW	/////////////////////
#define TEMP1_BOTTOM	35
#define TEMP1_TOP		60
#define TEMP2_BOTTOM	35
#define TEMP2_TOP		70
/////////////////////////////////////////////////////////////


#define SET(PORT,NUMER)					PORT &= ~(1 << NUMER)
#define CLEAR(PORT,NUMER)				PORT |=  (1 << NUMER)

#define CHECK_1_AT(PIN,NUMER)			PIN & (1 << NUMER)
#define CHECK_0_AT(PIN,NUMER)			!(PIN & (1 << NUMER))

#define MENU_MAIN_SCREEN			1
#define MENU_PROG_PROBE				2
#define MENU_TEMP_TO_SET			3
#define MENU_CHOICE					4
#define MENU_PROG_PROBE1			5
#define MENU_PROG_PROBE2			6
#define MENU_TEMP1					7
#define MENU_TEMP2					8
#define MENU_PASSWORD_ERROR			9
#define MENU_PASSWORD_GOOD			10

int howManyDigits(int number);
void showInt(int intToShow, int posx, int posy);

void config();
void welcomeScreen();

unsigned int eeprom_read();
unsigned int eeprom_write();

void readTemp();

int menuChoice();
int menuMainScreen();
int menuProgProbe();
int menuProgProbe1();
int menuProgProbe2();
int menuTempToSet();
int menuTemp1();
int menuTemp2();
int menuPassword();

int enterBtn(int whichMenu);
int plusBtn(int whichMenu);
int minusBtn(int whichMenu);
int menuBtn(int whichMenu);

int checkBtns(int menuState);
void showValues(int menuState);
void setOutputs();

#endif
