/*
 * main.c
 *
 * Created: 14.04.2017 18:28:46
 *  Author: Marcin Osi�ski
 */ 


 #include "functions.h"


 int menuState;
 
 int tempToSet1;
 int tempToSet2;
 
 int timer;
 int timerON;


 int temperature1;
 unsigned char ds18b20_scratchpad1[9];
 int temperature2;
 unsigned char ds18b20_scratchpad2[9];
 char tempBuf1[5];
 char tempBuf2[5];
 unsigned char romCode1[9];
 unsigned char romCode2[9];

 int main()
 {
	config();

	LCD_Initalize();

	welcomeScreen();

	menuState = menuMainScreen();

	while(1)
	{
		menuState = checkBtns(menuState);
		
		showValues(menuState);
		
		setOutputs();
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
	 OneWireWriteByte(romCode1[n]);

	 OneWireWriteByte(0xbe); // READ SCRATCHPAD

	 for(unsigned int i=0; i<9; i++)
	 ds18b20_scratchpad1[i] = OneWireReadByte();

	 temperature1 = ((ds18b20_scratchpad1[1] << 8) + ds18b20_scratchpad1[0]) / 16;
	 
	 OneWireReset();
	 
	 OneWireWriteByte(0x55); // MATCH ROM
	 
	 for(unsigned int j=0; j<8; j++)
	 OneWireWriteByte(romCode2[j]);
	 
	 OneWireWriteByte(0xbe); // READ SCRATCHPAD

	 for(unsigned int k=0; k<9; k++)
	 ds18b20_scratchpad2[k] = OneWireReadByte();

	 temperature2 = ((ds18b20_scratchpad2[1] << 8) + ds18b20_scratchpad2[0]) / 16;


	 if(timerON == 1) timer++;
 }
