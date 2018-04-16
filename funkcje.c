/*
 * funkcje.c
 *
 * Created: 14.04.2017 18:29:27
 *  Author: Marcin Osi�ski
 */ 

 #include "funkcje.h"


 extern int stan_menu;
 extern int tempZadana1;
 extern int tempZadana2;
 
 extern int timer;
 extern int timerON;

 extern double temperatura_1;
 extern double temperatura_2;
 extern unsigned char ds18b20_pad_1[9];
 extern unsigned char ds18b20_pad_2[9];
 extern char bufor_temp_1[5];
 extern char bufor_temp_2[5];
 extern unsigned char rom_code_1[9];
 extern unsigned char rom_code_2[9];
 

 void wyswietl_int(int dana, int rozmiar)
 {
	 char wynik[rozmiar];
	 
	 sprintf(wynik, "%d", dana);
	 
	 for (unsigned int i = 0; i < (rozmiar - 1); i++)
		LCD_WriteData(wynik[i]);
 }

 void ruski_debugger(int zmienna, int rozmiar)
 {
	LCD_GoTo(13,1); 
	
	wyswietl_int(zmienna, rozmiar);
 }



 void konfiguracja()
 {
	DDRA = 0x00;
	DDRB = 0x00;
	DDRC = 0xff;

	PORTC = 0xff;

	TCCR1B |= (1 << WGM12); // Ustawia timer w tryb CTC
	OCR1A = 62500; // Ustawia warto�� po��dan� na 1/4Hz dla preskalera 256
	TCCR1B |= (1 << CS12); // Ustawia timer z preskalerem Fcpu/256
	TIMSK  = (1<<OCIE1A);  // Odblokowanie przerwa�
	
	tempZadana1 = eeprom_read_byte((uint8_t*) 11);
	tempZadana2 = eeprom_read_byte((uint8_t*) 12);
	
	for(unsigned int i=0; i<8; i++) rom_code_1[i] = eeprom_read_byte((uint8_t*) 20 + i);
	for(unsigned int j=0; j<8; j++) rom_code_2[j] = eeprom_read_byte((uint8_t*) 40 + j);
	
	sei();
 }

 void ekran_powitalny()
 {
	LCD_WriteText("   UNIWERSALNY  ");
	LCD_GoTo(0,1);
	LCD_WriteText("    STEROWNIK   ");

	_delay_ms(2000);
 }



 int menu_ekran_roboczy()
 {
	LCD_GoTo(0,0);
	LCD_WriteText("ZASOBNIK.       ");
	LCD_GoTo(0,1);
	LCD_WriteText("BUFOR.          ");

	return MENU_EKRAN_ROBOCZY;
 }

 int menu_programuj_czujnik()
 {
	LCD_GoTo(0,0);
	LCD_WriteText("PROG. CZUJNIKOW ");
	LCD_GoTo(0,1);
	LCD_WriteText("ZASOB.-1  BUF.-2");
	
	return MENU_PROGRAMUJ_CZUJNIK;
 }

 int menu_prog_czujnik1()
 {
	LCD_GoTo(0,0);
	LCD_WriteText("PROG. CZUJNIKA 1 ");
	
	OneWireReset();
	OneWireWriteByte(0x33); // READ ROM
	for(unsigned int i=0; i<8; i++) rom_code_1[i] = OneWireReadByte();
	
	LCD_GoTo(0,1);
	LCD_WriteText("                 ");
	LCD_GoTo(4,1);
	for(unsigned int k=0; k<8; k++) LCD_WriteData(rom_code_1[k] + 48);

	return MENU_PROG_CZUJNIK1;
 }

 int menu_prog_czujnik2()
 {
	LCD_GoTo(0,0);
	LCD_WriteText("PROG. CZUJNIKA 2 ");

	OneWireReset();
	OneWireWriteByte(0x33); // READ ROM
	for(unsigned int j=0; j<8; j++) rom_code_2[j] = OneWireReadByte();
	
	LCD_GoTo(0,1);
	LCD_WriteText("                 ");
	LCD_GoTo(4,1);
	for(unsigned int k=0; k<8; k++) LCD_WriteData(rom_code_2[k] + 48);
 
	return MENU_PROG_CZUJNIK2;
 }

int menu_temp_zadana()
{
	LCD_GoTo(0,0);
	LCD_WriteText("  TEMP ZADANA   ");
	LCD_GoTo(0,1);
	LCD_WriteText("ZASOB.-1  BUF.-2");

	return MENU_TEMP_ZADANA;
}

int menu_temp1()
{
	LCD_GoTo(0,0);
	LCD_WriteText(" TEMP ZASOBNIKA ");
	LCD_GoTo(0,1);
	LCD_WriteText("                ");

	return MENU_TEMP1;
}

int menu_temp2()
{
	LCD_GoTo(0,0);
	LCD_WriteText("   TEMP BUFORA  ");
	LCD_GoTo(0,1);
	LCD_WriteText("                ");

	return MENU_TEMP2;
}

int menu_wybor()
{
	LCD_GoTo(0,0);
	LCD_WriteText("1 - nastawy     ");
	LCD_GoTo(0,1);
	LCD_WriteText("2 - prog. czuj. ");

	return MENU_WYBOR;
}

int menu_haslo()
{
	int haslo1 = 0;
	int haslo2 = 0;
	int haslo3 = 0;
	int haslo4 = 0;

	LCD_GoTo(0,0);
	LCD_WriteText("  PODAJ HASLO   ");
	LCD_GoTo(0,1);
	LCD_WriteText("                ");
	
	while((haslo1 != HASLO1) || (haslo2 != HASLO2) || (haslo3 != HASLO3) || (haslo4 != HASLO4))
	{
		LCD_GoTo(5,1);
	
		LCD_WriteData(haslo1 + 48);
		LCD_WriteData(haslo2 + 48);
		LCD_WriteData(haslo3 + 48);
		LCD_WriteData(haslo4 + 48);

		if(SPRAWDZ_1_NA(PINA,1) && haslo1 < 10)
		{	haslo1 ++;		_delay_ms(500);	}

		else if(SPRAWDZ_1_NA(PINA,0) && haslo2 < 10)
		{	haslo2 ++;		_delay_ms(500);	}

		else if(SPRAWDZ_1_NA(PINA,3) && haslo3 < 10)
		{	haslo3 ++;		_delay_ms(500);	}

		else if(SPRAWDZ_1_NA(PINA,2) && haslo4 < 10)
		{	haslo4 ++;		_delay_ms(500);	}

		if(PINA & ((1 << 3) | (1 << 2)))
			return MENU_HASLO_ERROR;		
	}

	return MENU_HASLO_GOOD;
}


int przycisk_enter(int ktore_menu)
{
	int stan_menu;

	switch(ktore_menu)
	{
		case MENU_EKRAN_ROBOCZY:									
			stan_menu = menu_wybor();

			return stan_menu;
			break;

		case MENU_PROGRAMUJ_CZUJNIK:	
			return MENU_PROGRAMUJ_CZUJNIK;								
			break;

		case MENU_TEMP_ZADANA:
			return MENU_TEMP_ZADANA;
			break;

		case MENU_WYBOR:
			return MENU_WYBOR;
			break;

		case MENU_TEMP1:
			eeprom_write_byte((uint8_t*)11, tempZadana1);
			stan_menu = menu_temp_zadana();

			return stan_menu;
			break;

		case MENU_TEMP2:
			eeprom_write_byte((uint8_t*) 12, tempZadana2);
		
			stan_menu = menu_temp_zadana();

			return stan_menu;
			break;

		case MENU_PROG_CZUJNIK1:
			for(unsigned int i=0; i<8; i++) eeprom_write_byte((uint8_t*) 20 + i, rom_code_1[i]);
		
			stan_menu = menu_programuj_czujnik();
			
			return stan_menu;
			break;

		case MENU_PROG_CZUJNIK2:
			for(unsigned int j=0; j<8; j++) eeprom_write_byte((uint8_t*) 40 + j, rom_code_2[j]);
		
			stan_menu = menu_programuj_czujnik();
			
			return stan_menu;
			break;

		case MENU_HASLO_ERROR:
			return MENU_HASLO_ERROR;
			break;

		case MENU_HASLO_GOOD:
			stan_menu = menu_programuj_czujnik();

			return stan_menu;
			break;
	}
	
	return 0;
}

int przycisk_menu(int ktore_menu)
{
	int stan_menu;

	switch(ktore_menu)
	{
		case MENU_EKRAN_ROBOCZY:
			return MENU_EKRAN_ROBOCZY;
			break;

		case MENU_PROGRAMUJ_CZUJNIK:
			stan_menu = menu_ekran_roboczy();

			return stan_menu;
			break;

		case MENU_TEMP_ZADANA:
			stan_menu = menu_ekran_roboczy();

			return stan_menu;
			break;

		case MENU_WYBOR:
			stan_menu = menu_ekran_roboczy();

			return stan_menu;
			break;
			
		case MENU_TEMP1:
			stan_menu = menu_ekran_roboczy();

			return stan_menu;
			break;

		case MENU_TEMP2:
			stan_menu = menu_ekran_roboczy();

			return stan_menu;
			break;

		case MENU_PROG_CZUJNIK1:
			stan_menu = menu_ekran_roboczy();

			return stan_menu;
			break;

		case MENU_PROG_CZUJNIK2:
			stan_menu = menu_ekran_roboczy();

			return stan_menu;
			break;

		case MENU_HASLO_ERROR:
			stan_menu = menu_ekran_roboczy();
			
			return stan_menu;
			break;

		case MENU_HASLO_GOOD:
			return MENU_HASLO_GOOD;
			break;
	}

	return 0;
}

int przycisk_plus(int ktore_menu)
{
	int stan_menu;

	switch(ktore_menu)
	{
		case MENU_EKRAN_ROBOCZY:
			return MENU_EKRAN_ROBOCZY;
			break;

		case MENU_PROGRAMUJ_CZUJNIK:
			stan_menu = menu_prog_czujnik2();

			return stan_menu;
			break;

		case MENU_TEMP_ZADANA:
			stan_menu = menu_temp2();

			return stan_menu;
			break;

		case MENU_WYBOR:
			stan_menu = menu_haslo();

			return stan_menu;
			break;
		
		case MENU_TEMP1:
			if(tempZadana1 < TEMP1_GORA)
				tempZadana1 ++;
		
			return MENU_TEMP1;
			break;

		case MENU_TEMP2:
			if(tempZadana2 < TEMP2_GORA)
				tempZadana2 ++;
		
			return MENU_TEMP2;
			break;

		case MENU_PROG_CZUJNIK1:
			return MENU_PROG_CZUJNIK1;
			break;

		case MENU_PROG_CZUJNIK2:
			return MENU_PROG_CZUJNIK2;
			break;

		case MENU_HASLO_ERROR:
			return MENU_HASLO_ERROR;
			break;

		case MENU_HASLO_GOOD:
			return MENU_HASLO_GOOD;
			break;
	}

	return 0;
}

int przycisk_minus(int ktore_menu)
{
	int stan_menu = 0;

	switch(ktore_menu)
	{
		case MENU_EKRAN_ROBOCZY:
			return MENU_EKRAN_ROBOCZY;
			break;

		case MENU_PROGRAMUJ_CZUJNIK:
			stan_menu = menu_prog_czujnik1();
			
			return stan_menu;
			break;

		case MENU_TEMP_ZADANA:
			stan_menu = menu_temp1();

			return stan_menu;
			break;

		case MENU_WYBOR:
			stan_menu = menu_temp_zadana();

			return stan_menu;
			break;
		
		case MENU_TEMP1:
			if(tempZadana1 > TEMP1_DOL)
				tempZadana1 --;
		
			return MENU_TEMP1;
			break;

		case MENU_TEMP2:
			if(tempZadana2 > TEMP2_DOL)		
				tempZadana2 --;
		
			return MENU_TEMP2;
			break;

		case MENU_PROG_CZUJNIK1:
			return MENU_PROG_CZUJNIK1;
			break;

		case MENU_PROG_CZUJNIK2:
			return MENU_PROG_CZUJNIK2;
			break;

		case MENU_HASLO_ERROR:
			return MENU_HASLO_ERROR;
			break;

		case MENU_HASLO_GOOD:
			return MENU_HASLO_GOOD;
			break;
	}

	return 0;
}

int sprawdz_przyciski(int stanMenu)
{
	if(SPRAWDZ_1_NA(PINA,0))
	{
		_delay_ms(500);
		stanMenu = przycisk_plus(stan_menu);
	}


	else if(SPRAWDZ_1_NA(PINA,3))
	{
		_delay_ms(500);
		stanMenu = przycisk_menu(stan_menu);
	}


	else if(SPRAWDZ_1_NA(PINA,1))
	{
		_delay_ms(500);
		stanMenu = przycisk_minus(stan_menu);
	}


	else if(SPRAWDZ_1_NA(PINA,2))
	{
		_delay_ms(500);
		stanMenu = przycisk_enter(stan_menu);
	}
	
	return stanMenu;
}

void wyswietl_zmienne(int stanMenu)
{
	switch(stanMenu)
	{
		case MENU_EKRAN_ROBOCZY:
			LCD_GoTo(13, 0);
			wyswietl_int(temperatura_1, 3);
			
			LCD_GoTo(13, 1);
			wyswietl_int(temperatura_2, 3);
		
			break;
		
	case MENU_TEMP1:
			LCD_GoTo(7, 1);
			wyswietl_int(tempZadana1, 3);
			
			break;
			
		case MENU_TEMP2:
			LCD_GoTo(7, 1);
			wyswietl_int(tempZadana2, 3);
		
			break;
	}
}

void steruj_wyjsciami()
{
		////////////////////////WYJSCIE 1//////////////////////////////////////////////////
		
		if(((SPRAWDZ_0_NA(PINB, 3)) && (temperatura_2 > tempZadana2)) || (temperatura_2 > 80))
			USTAW(PORTC, 5);
		
		else if(!(((SPRAWDZ_0_NA(PINB, 3)) && (temperatura_2 > tempZadana2 )) || (temperatura_2 > 80)))
			ZERUJ(PORTC, 5);
		
		///////////////////////WYJSCIE 2////////////////////////////////////////////////
		
		if(((temperatura_1 < tempZadana1) && ( temperatura_2 > ( temperatura_1 + 5))) || (temperatura_2 > 80))
			USTAW(PORTC, 6);
		
		else if(!(((temperatura_1 < tempZadana1) && ( temperatura_2 > ( temperatura_1 + 5))) || (temperatura_2 > 80)))
			ZERUJ(PORTC, 6);
		
		////////////////////////WYJSCIE 3////////////////////////////////////////////////
		
		if(((temperatura_1 > tempZadana1) && (SPRAWDZ_0_NA(PINB, 4))) || (temperatura_2 > 80))
		{
			timerON = 1;

			if(timer <= 100)
				USTAW(PORTC,4);

			if(timer > 100)
				ZERUJ(PORTC,4);

			if(timer > 300)
				timer = 0;
		}
		
		else if(!(((temperatura_1 > tempZadana1) && (SPRAWDZ_0_NA(PINB, 4))) || (temperatura_2 > 80)))
		{
			timerON = 0;
			timer = 0;

			ZERUJ(PORTC,4);
		}
            
               ////////////////////////WYJSCIE pk/////////////////////////////////////////////////
		
	    if(((SPRAWDZ_0_NA(PINB, 3)) && (temperatura_2 <= tempZadana2)))
	    	USTAW(PORTC, 7);
		
	    else if(!(((SPRAWDZ_0_NA(PINB, 3)) && (temperatura_2 <= tempZadana2 ))))
	    	ZERUJ(PORTC, 7);
		

}

	
