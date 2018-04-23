/*
 * funktions.c
 *
 * Created: 14.04.2017 18:29:27
 *  Author: Marcin Osinski
 */

 #include "functions.h"

 extern int tempToSet1;
 extern int tempToSet2;

 extern int timer;
 extern int timerON;

 extern int temperature1;
 extern int temperature2;
 extern unsigned char romCode1[9];
 extern unsigned char romCode2[9];

 void showInt(int intToShow, int size)
 {
	 char wynik[size];

	 sprintf(wynik, "%d", intToShow);

	 for (unsigned int i = 0; i < (size - 1); i++)
		LCD_WriteData(wynik[i]);
 }


 void config()
 {
	DDRA = 0x00;
	DDRB = 0x00;
	DDRC = 0xff;

	PORTC = 0xff;

	TCCR1B |= (1 << WGM12);
	OCR1A = 62500;
	TCCR1B |= (1 << CS12);
	TIMSK  = (1<<OCIE1A);

	tempToSet1 = eeprom_read_byte((uint8_t*) 11);
	tempToSet2 = eeprom_read_byte((uint8_t*) 12);

	for(unsigned int i=0; i<8; i++) romCode1[i] = eeprom_read_byte((uint8_t*) 20 + i);
	for(unsigned int j=0; j<8; j++) romCode2[j] = eeprom_read_byte((uint8_t*) 40 + j);

	sei();
 }

 void welcomeScreen()
 {
	LCD_WriteText("   REKRUTACJA   ");
	LCD_GoTo(0,1);
	LCD_WriteText("     SKN MOS    ");

	_delay_ms(5000);
 }



 int menuMainScreen()
 {
	LCD_GoTo(0,0);
	LCD_WriteText("ZASOBNIK.       ");
	LCD_GoTo(0,1);
	LCD_WriteText("BUFOR.          ");

	return MENU_MAIN_SCREEN;
 }

 int menuProgProbe()
 {
	LCD_GoTo(0,0);
	LCD_WriteText("PROG. CZUJNIKOW ");
	LCD_GoTo(0,1);
	LCD_WriteText("ZASOB.-1  BUF.-2");

	return MENU_PROG_PROBE;
 }

 int menuProgProbe1()
 {
	LCD_GoTo(0,0);
	LCD_WriteText("PROG. CZUJNIKA 1 ");

	OneWireReset();
	OneWireWriteByte(0x33); // READ ROM
	for(unsigned int i=0; i<8; i++) romCode1[i] = OneWireReadByte();

	LCD_GoTo(0,1);
	LCD_WriteText("                 ");
	LCD_GoTo(4,1);
	for(unsigned int k=0; k<8; k++) LCD_WriteData(romCode1[k] + 48);

	return MENU_PROG_PROBE1;
 }

 int menuProgProbe2()
 {
	LCD_GoTo(0,0);
	LCD_WriteText("PROG. CZUJNIKA 2 ");

	OneWireReset();
	OneWireWriteByte(0x33); // READ ROM
	for(unsigned int j=0; j<8; j++) romCode2[j] = OneWireReadByte();

	LCD_GoTo(0,1);
	LCD_WriteText("                 ");
	LCD_GoTo(4,1);
	for(unsigned int k=0; k<8; k++) LCD_WriteData(romCode2[k] + 48);

	return MENU_PROG_PROBE2;
 }

int menuTempToSet()
{
	LCD_GoTo(0,0);
	LCD_WriteText("  TEMP ZADANA   ");
	LCD_GoTo(0,1);
	LCD_WriteText("ZASOB.-1  BUF.-2");

	return MENU_TEMP_TO_SET;
}

int menuTemp1()
{
	LCD_GoTo(0,0);
	LCD_WriteText(" TEMP ZASOBNIKA ");
	LCD_GoTo(0,1);
	LCD_WriteText("                ");

	return MENU_TEMP1;
}

int menuTemp2()
{
	LCD_GoTo(0,0);
	LCD_WriteText("   TEMP BUFORA  ");
	LCD_GoTo(0,1);
	LCD_WriteText("                ");

	return MENU_TEMP2;
}

int menuChoice()
{
	LCD_GoTo(0,0);
	LCD_WriteText("1 - nastawy     ");
	LCD_GoTo(0,1);
	LCD_WriteText("2 - prog. czuj. ");

	return MENU_CHOICE;
}

int menuPassword()
{
	int password1 = 0;
	int password2 = 0;
	int password3 = 0;
	int password4 = 0;

	LCD_GoTo(0,0);
	LCD_WriteText("  PODAJ HASLO   ");
	LCD_GoTo(0,1);
	LCD_WriteText("                ");

	while((password1 != PASSWORD1) || (password2 != PASSWORD2)
			|| (password3 != PASSWORD3) || (password4 != PASSWORD4))
	{
		LCD_GoTo(5,1);

		LCD_WriteData(password1 + 48);
		LCD_WriteData(password2 + 48);
		LCD_WriteData(password3 + 48);
		LCD_WriteData(password4 + 48);

		if((CHECK_1_AT(PINA,1)) && password1 < 10)
		{	password1 ++;		_delay_ms(500);	}

		else if((CHECK_1_AT(PINA,0)) && password2 < 10)
		{	password2 ++;		_delay_ms(500);	}

		else if((CHECK_1_AT(PINA,3)) && password3 < 10)
		{	password3 ++;		_delay_ms(500);	}

		else if((CHECK_1_AT(PINA,2)) && password4 < 10)
		{	password4 ++;		_delay_ms(500);	}

		if(PINA & ((1 << 3) | (1 << 2)))
			return MENU_PASSWORD_ERROR;
	}

	return MENU_PASSWORD_GOOD;
}


int enterBtn(int whichMenu)
{
	int menuState;

	switch(whichMenu)
	{
		case MENU_MAIN_SCREEN:
			menuState = menuChoice();

			return menuState;
			break;

		case MENU_PROG_PROBE:
			return MENU_PROG_PROBE;
			break;

		case MENU_TEMP_TO_SET:
			return MENU_TEMP_TO_SET;
			break;

		case MENU_CHOICE:
			return MENU_CHOICE;
			break;

		case MENU_TEMP1:
			eeprom_write_byte((uint8_t*)11, tempToSet1);
			menuState = menuTempToSet();

			return menuState;
			break;

		case MENU_TEMP2:
			eeprom_write_byte((uint8_t*) 12, tempToSet2);

			menuState = menuTempToSet();

			return menuState;
			break;

		case MENU_PROG_PROBE1:
			for(unsigned int i=0; i<8; i++) eeprom_write_byte((uint8_t*) 20 + i, romCode1[i]);

			menuState = menuProgProbe();

			return menuState;
			break;

		case MENU_PROG_PROBE2:
			for(unsigned int j=0; j<8; j++) eeprom_write_byte((uint8_t*) 40 + j, romCode2[j]);

			menuState = menuProgProbe();

			return menuState;
			break;

		case MENU_PASSWORD_ERROR:
			return MENU_PASSWORD_ERROR;
			break;

		case MENU_PASSWORD_GOOD:
			menuState = menuProgProbe();

			return menuState;
			break;
	}

	return 0;
}

int menuBtn(int whichMenu)
{
	int menuState;

	switch(whichMenu)
	{
		case MENU_MAIN_SCREEN:
			return MENU_MAIN_SCREEN;
			break;

		case MENU_PROG_PROBE:
			menuState = menuMainScreen();

			return menuState;
			break;

		case MENU_TEMP_TO_SET:
			menuState = menuMainScreen();

			return menuState;
			break;

		case MENU_CHOICE:
			menuState = menuMainScreen();

			return menuState;
			break;

		case MENU_TEMP1:
			menuState = menuMainScreen();

			return menuState;
			break;

		case MENU_TEMP2:
			menuState = menuMainScreen();

			return menuState;
			break;

		case MENU_PROG_PROBE1:
			menuState = menuMainScreen();

			return menuState;
			break;

		case MENU_PROG_PROBE2:
			menuState = menuMainScreen();

			return menuState;
			break;

		case MENU_PASSWORD_ERROR:
			menuState = menuMainScreen();

			return menuState;
			break;

		case MENU_PASSWORD_GOOD:
			return MENU_PASSWORD_GOOD;
			break;
	}

	return 0;
}

int plusBtn(int whichMenu)
{
	int menuState;

	switch(whichMenu)
	{
		case MENU_MAIN_SCREEN:
			return MENU_MAIN_SCREEN;
			break;

		case MENU_PROG_PROBE:
			menuState = menuProgProbe2();

			return menuState;
			break;

		case MENU_TEMP_TO_SET:
			menuState = menuTemp2();

			return menuState;
			break;

		case MENU_CHOICE:
			menuState = menuPassword();

			return menuState;
			break;

		case MENU_TEMP1:
			if(tempToSet1 < TEMP1_TOP)
				tempToSet1 ++;

			return MENU_TEMP1;
			break;

		case MENU_TEMP2:
			if(tempToSet2 < TEMP2_TOP)
				tempToSet2 ++;

			return MENU_TEMP2;
			break;

		case MENU_PROG_PROBE1:
			return MENU_PROG_PROBE1;
			break;

		case MENU_PROG_PROBE2:
			return MENU_PROG_PROBE2;
			break;

		case MENU_PASSWORD_ERROR:
			return MENU_PASSWORD_ERROR;
			break;

		case MENU_PASSWORD_GOOD:
			return MENU_PASSWORD_GOOD;
			break;
	}

	return 0;
}

int minusBtn(int whichMenu)
{
	int menuState = 0;

	switch(whichMenu)
	{
		case MENU_MAIN_SCREEN:
			return MENU_MAIN_SCREEN;
			break;

		case MENU_PROG_PROBE:
			menuState = menuProgProbe1();

			return menuState;
			break;

		case MENU_TEMP_TO_SET:
			menuState = menuTemp1();

			return menuState;
			break;

		case MENU_CHOICE:
			menuState = menuTempToSet();

			return menuState;
			break;

		case MENU_TEMP1:
			if(tempToSet1 > TEMP1_BOTTOM)
				tempToSet1 --;

			return MENU_TEMP1;
			break;

		case MENU_TEMP2:
			if(tempToSet2 > TEMP2_BOTTOM)
				tempToSet2 --;

			return MENU_TEMP2;
			break;

		case MENU_PROG_PROBE1:
			return MENU_PROG_PROBE1;
			break;

		case MENU_PROG_PROBE2:
			return MENU_PROG_PROBE2;
			break;

		case MENU_PASSWORD_ERROR:
			return MENU_PASSWORD_ERROR;
			break;

		case MENU_PASSWORD_GOOD:
			return MENU_PASSWORD_GOOD;
			break;
	}

	return 0;
}

int checkBtns(int menuState)
{
	if(CHECK_1_AT(PINA,0))
	{
		_delay_ms(500);
		menuState = plusBtn(menuState);
	}


	else if(CHECK_1_AT(PINA,3))
	{
		_delay_ms(500);
		menuState = menuBtn(menuState);
	}


	else if(CHECK_1_AT(PINA,1))
	{
		_delay_ms(500);
		menuState = minusBtn(menuState);
	}


	else if(CHECK_1_AT(PINA,2))
	{
		_delay_ms(500);
		menuState = enterBtn(menuState);
	}

	return menuState;
}

void showValues(int menuState)
{
	switch(menuState)
	{
		case MENU_MAIN_SCREEN:
			LCD_GoTo(13, 0);
			showInt(temperature1, 3);

			LCD_GoTo(13, 1);
			showInt(temperature2, 3);

			break;

		case MENU_TEMP1:
			LCD_GoTo(7, 1);
			showInt(tempToSet1, 3);

			break;

		case MENU_TEMP2:
			LCD_GoTo(7, 1);
			showInt(tempToSet2, 3);

			break;
	}
}

void setOutputs()
{
		////////////////////////OUTPUT 1//////////////////////////////////////////////////

		if(((CHECK_0_AT(PINB, 3)) && (temperature2 > tempToSet2)) || (temperature2 > 80))
			SET(PORTC, 5);

		else if(!(((CHECK_0_AT(PINB, 3)) && (temperature2 > tempToSet2 )) || (temperature2 > 80)))
			CLEAR(PORTC, 5);

		///////////////////////WYJSCIE 2////////////////////////////////////////////////

		if(((temperature1 < tempToSet1) && ( temperature2 > ( temperature1 + 5))) || (temperature2 > 80))
			SET(PORTC, 6);

		else if(!(((temperature1 < tempToSet1) && ( temperature2 > ( temperature1 + 5))) || (temperature2 > 80)))
			CLEAR(PORTC, 6);

		////////////////////////WYJSCIE 3////////////////////////////////////////////////

		if(((temperature1 > tempToSet1) && (CHECK_0_AT(PINB, 4))) || (temperature2 > 80))
		{
			timerON = 1;

			if(timer <= 100)
				SET(PORTC,4);

			if(timer > 100)
				CLEAR(PORTC,4);

			if(timer > 300)
				timer = 0;
		}

		else if(!(((temperature1 > tempToSet1) && (CHECK_0_AT(PINB, 4))) || (temperature2 > 80)))
		{
			timerON = 0;
			timer = 0;

			CLEAR(PORTC,4);
		}

               ////////////////////////WYJSCIE pk/////////////////////////////////////////////////

	    if(((CHECK_0_AT(PINB, 3)) && (temperature2 <= tempToSet2)))
	    	SET(PORTC, 7);

	    else if(!(((CHECK_0_AT(PINB, 3)) && (temperature2 <= tempToSet2 ))))
	    	CLEAR(PORTC, 7);


}


