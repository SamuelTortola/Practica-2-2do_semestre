//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electrónica digital 2
// Autor: Samuel Tortola - 22094
// Proyecto: Laboratorio 2
// Hardware: Atmega238p
// Creado: 18/07/2024
//Última modificación: 18/07/2024
//******************************************************************************


#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "LCD/LCD.h"


void setup(void);
void setup(void){
	DDRD = 0xFF;  //Puerto D como salida
	DDRB = 0x03;  //PB0 y PB1 como salida
	
	UCSR0B = 0;  //Usar los pines TX y RX como digitales
	
	Lcd_Init8bits();   //Iniciar pantalla LCD
	Lcd_Set_Cursor(0,0);  //fila y columna
	
}



int main(void)
{
	setup();
	
    while (1) 
    {               //Ejemplo:
		
		 Lcd_Write_String("Hola UVG");
		 _delay_ms(1000);
		 for(int i=0;i<15;i++){
			 Lcd_Shift_Left();
			 _delay_ms(500);
		 }
		 for(int i=0;i<15;i++){
			  Lcd_Shift_Right();
			 _delay_ms(500);
		 }
		 Lcd_Clear();
    }
}

