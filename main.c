/*
 * main.c
 *
 * Created: 14.04.2017 18:28:46
 *  Author: Marcin Osi�ski
 */ 


 #include "funkcje.h"


 int stan_menu;
 
 int tempZadana1 = 20;
 int tempZadana2 = 20;
 
 int timer;
 int timerON;

 double temperatura_1;
 unsigned char ds18b20_pad_1[9];
 double temperatura_2;
 unsigned char ds18b20_pad_2[9];
 char bufor_temp_1[5];
 char bufor_temp_2[5];
 unsigned char rom_code_1[9];
 unsigned char rom_code_2[9];

 int main()
 {
	konfiguracja();

	LCD_Initalize();

	ekran_powitalny();

	stan_menu = menu_ekran_roboczy();

	while(1)
	{
		stan_menu = sprawdz_przyciski(stan_menu);
		
		wyswietl_zmienne(stan_menu);
		
		steruj_wyjsciami();

	}
 }


 ISR (TIMER1_COMPA_vect)
 {
	 OneWireReset();
	 OneWireWriteByte(0xCC); // SKIP ROM
	 OneWireWriteByte(0x44); // CONVERT T
	 _delay_ms(750);
	 
	 OneWireReset();
	 
	 OneWireWriteByte(0x55); // MATCH ROM
	 
	 for(unsigned int n=0; n<8; n++)
	 OneWireWriteByte(rom_code_1[n]);

	 OneWireWriteByte(0xbe); // READ SCRATCHPAD

	 for(unsigned int i=0; i<9; i++)
	 ds18b20_pad_1[i] = OneWireReadByte();

	 temperatura_1 = ((ds18b20_pad_1[1] << 8) + ds18b20_pad_1[0]) / 16;
	 
	 OneWireReset();
	 
	 OneWireWriteByte(0x55); // MATCH ROM
	 
	 for(unsigned int j=0; j<8; j++)
	 OneWireWriteByte(rom_code_2[j]);
	 
	 OneWireWriteByte(0xbe); // READ SCRATCHPAD

	 for(unsigned int k=0; k<9; k++)
	 ds18b20_pad_2[k] = OneWireReadByte();

	 temperatura_2 = ((ds18b20_pad_2[1] << 8) + ds18b20_pad_2[0]) / 16;


	 if(timerON == 1) timer++;
 }
