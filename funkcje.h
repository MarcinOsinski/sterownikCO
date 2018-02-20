/*
 * funkcje.h
 *
 * Created: 14.04.2017 18:29:42
 *  Author: Student235267
 */ 
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



/////////////////////////	HASLO	/////////////////////////
#define HASLO1	0
#define HASLO2	0
#define HASLO3	0
#define HASLO4	1
/////////////////////////////////////////////////////////////

////////////////////	GRANICE NASTAW	/////////////////////
#define TEMP1_DOL	35
#define TEMP1_GORA	60
#define TEMP2_DOL	35
#define TEMP2_GORA	70
/////////////////////////////////////////////////////////////


#define USTAW(PORT,NUMER)				PORT &= ~(1 << NUMER)
#define ZERUJ(PORT,NUMER)				PORT |=  (1 << NUMER)

#define SPRAWDZ_1_NA(PIN,NUMER)			PIN & (1 << NUMER)
#define SPRAWDZ_0_NA(PIN,NUMER)			!(PIN & (1 << NUMER))	

#define MENU_EKRAN_ROBOCZY			1
#define MENU_PROGRAMUJ_CZUJNIK		2
#define MENU_TEMP_ZADANA			3
#define MENU_WYBOR					4
#define MENU_PROG_CZUJNIK1			5
#define MENU_PROG_CZUJNIK2			6
#define MENU_TEMP1					7
#define MENU_TEMP2					8
#define MENU_HASLO_ERROR			9
#define MENU_HASLO_GOOD				10


void wyswietl_int(int dana, int rozmiar);

void ruski_debugger(int zmienna, int rozmiar);

void konfiguracja();
void ekran_powitalny();

unsigned int eeprom_odczyt();
unsigned int eeprom_zapisz();

void odczyt_temperatury();

int menu_wybor();
int menu_ekran_roboczy();
int menu_programuj_czujnik();
int menu_prog_czujnik1();
int menu_prog_czujnik2();
int menu_temp_zadana();
int menu_temp1();
int menu_temp2();
int menu_haslo();

int przycisk_enter(int ktore_menu);
int przycisk_plus(int ktore_menu);
int przycisk_minus(int ktore_menu);
int przycisk_menu(int ktore_menu);

int sprawdz_przyciski(int stanMenu);
void wyswietl_zmienne(int stanMenu);
void steruj_wyjsciami();

#endif
