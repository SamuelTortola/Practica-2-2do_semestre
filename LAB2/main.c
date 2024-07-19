//******************************************************************************
// Universidad Del Valle De Guatemala
// IE2023: Electrónica digital 2
// Autor: Samuel Tortola - 22094
// Proyecto: Laboratorio 2
// Hardware: Atmega238p
// Creado: 18/07/2024
//Última modificación: 19/07/2024
//******************************************************************************

/*
El presente programa consiste en la implementación de la pantalla LCD 16x02, la cual muestra el valor de
dos potenciómetros y ademas tiene por medio de UART, que cuando reciba los comando + y - sume o reste el 
contador. 
Pines: Puerto D desde PD2, conectado de D2 a D7 de LCD. Puerto B, PB0 conectado a RS de LCD, PB1 a E, PB2 a D0, y PB3 a D1.
  Potenciómetros conectados a PC0 y PC1 de atemga328P.
*/



#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "LCD/LCD.h"   //Incluir libreria de LCD
#include "ADC/ADC.h"   //Incluir libreria de ADC
#include "UART/UART.h"  //Incluir libreria de UART

uint8_t Val1 = 0, Val2 = 0, caso = 0, activa = 0, menu2 = 0;    //Variables de procesamiento
int  cambio = 0;    //Varible del contador 3

int millares = 0;
int centenas = 0;
int decenas = 0; 
int unidades = 0;

char lista[10] = {'0','1','2','3','4','5','6','7','8','9'}; //Lista de numeros a mostrar
char lista1[4]; // 3 caracteres + 1 para el carácter nulo, lista de potenciómetro 1
char lista2[4]; // 3 caracteres + 1 para el carácter nulo, lista de potenciómetro 2
char lista3[4] = {'0', '0', '0'}; // 3 caracteres + 1 para el carácter nulo, lista de contador 3

volatile char receivedChar = 0;    //Variable que almacena el valor del UART


void setup(void);
void setup(void){
	cli();  //Apagar interrupciones
	DDRD = 0xFF;  //Puerto D como salida
	DDRB = 0xFF;  //Puerto B como salida
	DDRC =0;  //Puerto C como entrada
	
	UCSR0B = 0;  //Usar los pines TX y RX como digitales
	
	Lcd_Init8bits();   //Iniciar pantalla LCD
	Lcd_Clear();
	
	initADC(); //Iniciar ADC
	
	initUART9600();  //Iniciar UART
	
	sei(); //Activar interrupciones
}



int main(void)
{
	setup();
	
    while (1) 
    {               //Ejemplo:
		/*
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
		 Lcd_Clear();*/
		
		//************MENU*********************************
	if(activa == 0 && menu2 == 0){
	writeTextUART("\n\r     **************Hola como esta****************");   //Mostrar inicio 
	writeUART(10);
	writeUART(13);
	writeUART(10);
	writeTextUART("          Coloque 1 para leer potenciometros");   //Mostrar inicio
	writeUART(10);
	writeUART(13);
	writeTextUART("          Coloque 2 para incrementar contador\n\r");   //Mostrar inicio
	writeUART(10);
	writeUART(13);
	activa = 1;   //Salir del menu 
	}
	
		

		ADCSRA |=(1<<ADSC);  //Leer ADC
	
		Lcd_Set_Cursor(0,2);  //fila y columna
		Lcd_Write_String("S1:");
		Lcd_Set_Cursor(0,7);  //fila y columna
		Lcd_Write_String("S2:");
		Lcd_Set_Cursor(0,12);  //fila y columna
		Lcd_Write_String("S3:");
		
		
		Lcd_Set_Cursor(1,2);  //fila y columna
		Lcd_Write_String(lista1);
		Lcd_Set_Cursor(1,7);  //fila y columna
		Lcd_Write_String(lista2);
		Lcd_Set_Cursor(1,12);  //fila y columna
		Lcd_Write_String(lista3);
		

			//Obtener el dato del potenciometro descompuesto, y volverlo char y guardarlo en una lista
			millares = Val2/1000;
			centenas = (Val2-(millares*1000))/100;
			decenas = (Val2- (millares*1000 + centenas*100))/10;
			unidades = Val2-(millares*1000 + centenas*100 + decenas*10 );
			
			
			switch(centenas){   //Mostrar centenas
				case 0:
					lista1[0] = lista[0];   //almacenar cada valor, en la lista a mostrar en la pantalla
				break;
				
				
				case 1:
					lista1[0] = lista[1];
				break;
				
				case 2:
					lista1[0] = lista[2];
				break;
				
				
				case 3:
					lista1[0] = lista[3];
				break;
				
				case 4:
					lista1[0] = lista[4];
				break;
				
				
				case 5:
					lista1[0] = lista[5];
				break;
				
				
				case 6:
					lista1[0] = lista[6];
				break;
				
				
				case 7:
					lista1[0] = lista[7];
				break;
				
				case 8:
					lista1[0] = lista[8];
				break;
				
				
				case 9:
					lista1[0] = lista[9];
				break;
				
			}
			
			
			switch(decenas){   //Mostrar decenas
				case 0:
					lista1[1] = lista[0];
				break;
				
				
				case 1:
					lista1[1] = lista[1];
				break;
				
				case 2:
					lista1[1] = lista[2];
				break;
				
				
				case 3:
					lista1[1] = lista[3];
				break;
				
				case 4:
				lista1[1] = lista[4];
				break;
				
				
				case 5:
					lista1[1] = lista[5];
				break;
				
				
				case 6:
					lista1[1] = lista[6];
				break;
				
				
				case 7:
					lista1[1] = lista[7];
				break;
				
				case 8:
					lista1[1] = lista[8];
				break;
				
				
				case 9:
					lista1[1] = lista[9];
				break;
				
			}
			
			switch(unidades){   //Mostrar unidades
				case 0:
					lista1[2] = lista[0];
				break;
				
				
				case 1:
					lista1[2] = lista[1];
				break;
				
				case 2:
					lista1[2] = lista[2];
				break;
				
				
				case 3:
					lista1[2] = lista[3];
				break;
				
				case 4:
					lista1[2] = lista[4];
				break;
				
				
				case 5:
					lista1[2] = lista[5];
				break;
				
				
				case 6:
					lista1[2] = lista[6];
				break;
				
				
				case 7:
					lista1[2] = lista[7];
				break;
				
				case 8:
					lista1[2] = lista[8];
				break;
				
				
				case 9:
					lista1[2] = lista[9];
				break;
				
			}
			
			
		
			//Obtener el dato del potenciometro descompuesto, y volverlo char y guardarlo en una lista
			millares = Val1/1000;
			centenas = (Val1-(millares*1000))/100;
			decenas = (Val1- (millares*1000 + centenas*100))/10;
			unidades = Val1-(millares*1000 + centenas*100 + decenas*10 );
			
			
			switch(centenas){   //Mostrar centenas
				case 0:
				lista2[0] = lista[0];    //almacenar cada valor, en la lista a mostrar en la pantalla
				break;
				
				
				case 1:
				lista2[0] = lista[1];
				break;
				
				case 2:
				lista2[0] = lista[2];
				break;
				
				
				case 3:
				lista2[0] = lista[3];
				break;
				
				case 4:
				lista2[0] = lista[4];
				break;
				
				
				case 5:
				lista2[0] = lista[5];
				break;
				
				
				case 6:
				lista2[0] = lista[6];
				break;
				
				
				case 7:
				lista2[0] = lista[7];
				break;
				
				case 8:
				lista2[0] = lista[8];
				break;
				
				
				case 9:
				lista2[0] = lista[9];
				break;
				
			}
			
			
			switch(decenas){   //Mostrar decenas
				case 0:
				lista2[1] = lista[0];
				break;
				
				
				case 1:
				lista2[1] = lista[1];
				break;
				
				case 2:
				lista2[1] = lista[2];
				break;
				
				
				case 3:
				lista2[1] = lista[3];
				break;
				
				case 4:
				lista2[1] = lista[4];
				break;
				
				
				case 5:
				lista2[1] = lista[5];
				break;
				
				
				case 6:
				lista2[1] = lista[6];
				break;
				
				
				case 7:
				lista2[1] = lista[7];
				break;
				
				case 8:
				lista2[1] = lista[8];
				break;
				
				
				case 9:
				lista2[1] = lista[9];
				break;
				
			}
			
			switch(unidades){   //Mostrar unidades
				case 0:
				lista2[2] = lista[0];
				break;
				
				
				case 1:
				lista2[2] = lista[1];
				break;
				
				case 2:
				lista2[2] = lista[2];
				break;
				
				
				case 3:
				lista2[2] = lista[3];
				break;
				
				case 4:
				lista2[2] = lista[4];
				break;
				
				
				case 5:
				lista2[2] = lista[5];
				break;
				
				
				case 6:
				lista2[2] = lista[6];
				break;
				
				
				case 7:
				lista2[2] = lista[7];
				break;
				
				case 8:
				lista2[2] = lista[8];
				break;
				
				
				case 9:
				lista2[2] = lista[9];
				break;
		}
		
		        //Separar los valores numericos, convertilos en char y guardarlos en una lista
				
		
		
		
			if (receivedChar == '1' && menu2 == 0){   //Si se elige ver los potenciómetros 
				writeTextUART("Potenciómetro 1: ");   
				writeTextUART(lista1);        //Mostrar potenciómetro 1
				writeUART(10);
				writeUART(13);
				writeTextUART("Potenciómetro 2: ");
				writeTextUART(lista2);               //Mostrar potenciómetro 2
				writeUART(10);
				writeUART(13);
				writeUART(10);
				writeUART(13);
				receivedChar = 0;
				activa = 0;
			}
			

			
			if (receivedChar == '2' ){   //Si se elige modificar el valor del contador
				menu2 = 1;
				writeTextUART("Presione + para incrementar, - para decrementar, e para menu principal \n\r");
				writeUART(10);
				writeUART(13);
				writeUART(10);
				writeUART(13);
				
				receivedChar = 0;
			}
			
			if (receivedChar == '+' && menu2 == 1){
				if (cambio >= 255)    //si el contador es mayor de 255, dejarlo en 255
				{
					cambio = 255;
				}
				
				millares = cambio/1000;
				centenas = (cambio-(millares*1000))/100;
				decenas = (cambio- (millares*1000 + centenas*100))/10;
				unidades = cambio-(millares*1000 + centenas*100 + decenas*10 );
				
				
				switch(centenas){   //Mostrar centenas
					case 0:
					lista3[0] = lista[0];    //almacenar cada valor, en la lista a mostrar en la pantalla
					break;
					
					
					case 1:
					lista3[0] = lista[1];
					break;
					
					case 2:
					lista3[0] = lista[2];
					break;
					
					
					case 3:
					lista3[0] = lista[3];
					break;
					
					case 4:
					lista3[0] = lista[4];
					break;
					
					
					case 5:
					lista3[0] = lista[5];
					break;
					
					
					case 6:
					lista3[0] = lista[6];
					break;
					
					
					case 7:
					lista3[0] = lista[7];
					break;
					
					case 8:
					lista3[0] = lista[8];
					break;
					
					
					case 9:
					lista3[0] = lista[9];
					break;
					
				}
				
				
				switch(decenas){   //Mostrar decenas
					case 0:
					lista3[1] = lista[0];
					break;
					
					
					case 1:
					lista3[1] = lista[1];
					break;
					
					case 2:
					lista3[1] = lista[2];
					break;
					
					
					case 3:
					lista3[1] = lista[3];
					break;
					
					case 4:
					lista3[1] = lista[4];
					break;
					
					
					case 5:
					lista3[1] = lista[5];
					break;
					
					
					case 6:
					lista3[1] = lista[6];
					break;
					
					
					case 7:
					lista3[1] = lista[7];
					break;
					
					case 8:
					lista3[1] = lista[8];
					break;
					
					
					case 9:
					lista3[1] = lista[9];
					break;
					
				}
				
				switch(unidades){   //Mostrar unidades
					case 0:
					lista3[2] = lista[0];   //Almacenar los valores del contador
					break;
					
					
					case 1:
					lista3[2] = lista[1];
					break;
					
					case 2:
					lista3[2] = lista[2];
					break;
					
					
					case 3:
					lista3[2] = lista[3];
					break;
					
					case 4:
					lista3[2] = lista[4];
					break;
					
					
					case 5:
					lista3[2] = lista[5];
					break;
					
					
					case 6:
					lista3[2] = lista[6];
					break;
					
					
					case 7:
					lista3[2] = lista[7];
					break;
					
					case 8:
					lista3[2] = lista[8];
					break;
					
					
					case 9:
					lista3[2] = lista[9];
					break;
				}
					writeTextUART("Contador: ");    //Mostrar el valor del contador 3
					writeTextUART(lista3);
					writeUART(10);
					writeUART(13);
				receivedChar = 0;
				cambio ++;         //Incrementar el tercer contador
			}
			
			
			if (receivedChar == '-' && menu2 == 1){
			
				if (cambio <= 0)    //si el contador es menor de 0, dejarlo en 0
				{
					cambio = 0;
				}
				
				millares = cambio/1000;
				centenas = (cambio-(millares*1000))/100;
				decenas = (cambio- (millares*1000 + centenas*100))/10;
				unidades = cambio-(millares*1000 + centenas*100 + decenas*10 );
				
				
				switch(centenas){   //Mostrar centenas
					case 0:
					lista3[0] = lista[0];    //almacenar cada valor, en la lista a mostrar en la pantalla
					break;
					
					
					case 1:
					lista3[0] = lista[1];
					break;
					
					case 2:
					lista3[0] = lista[2];
					break;
					
					
					case 3:
					lista3[0] = lista[3];
					break;
					
					case 4:
					lista3[0] = lista[4];
					break;
					
					
					case 5:
					lista3[0] = lista[5];
					break;
					
					
					case 6:
					lista3[0] = lista[6];
					break;
					
					
					case 7:
					lista3[0] = lista[7];
					break;
					
					case 8:
					lista3[0] = lista[8];
					break;
					
					
					case 9:
					lista3[0] = lista[9];
					break;
					
				}
				
				
				switch(decenas){   //Mostrar decenas
					case 0:
					lista3[1] = lista[0];
					break;
					
					
					case 1:
					lista3[1] = lista[1];
					break;
					
					case 2:
					lista3[1] = lista[2];
					break;
					
					
					case 3:
					lista3[1] = lista[3];
					break;
					
					case 4:
					lista3[1] = lista[4];
					break;
					
					
					case 5:
					lista3[1] = lista[5];
					break;
					
					
					case 6:
					lista3[1] = lista[6];
					break;
					
					
					case 7:
					lista3[1] = lista[7];
					break;
					
					case 8:
					lista3[1] = lista[8];
					break;
					
					
					case 9:
					lista3[1] = lista[9];
					break;
					
				}
				
				switch(unidades){   //Mostrar unidades
					case 0:
					lista3[2] = lista[0];   //Almacenar los valores del contador
					break;
					
					
					case 1:
					lista3[2] = lista[1];
					break;
					
					case 2:
					lista3[2] = lista[2];
					break;
					
					
					case 3:
					lista3[2] = lista[3];
					break;
					
					case 4:
					lista3[2] = lista[4];
					break;
					
					
					case 5:
					lista3[2] = lista[5];
					break;
					
					
					case 6:
					lista3[2] = lista[6];
					break;
					
					
					case 7:
					lista3[2] = lista[7];
					break;
					
					case 8:
					lista3[2] = lista[8];
					break;
					
					
					case 9:
					lista3[2] = lista[9];
					break;
				}
				
				writeTextUART("Contador: ");    //Mostrar el valor del contador 3
				writeTextUART(lista3);
				writeUART(10);
				writeUART(13);
				receivedChar = 0;
				cambio --;  //Decrementar el valor del contador
			}
			
			
			
			if (receivedChar == 'e' && menu2 == 1){  //Volver al menu principal 
				receivedChar = 0;
				activa = 0;    //Regresar al menu principal  
				menu2 = 0;   //Activaar el menu principal
				
			}
		 
    }
}



ISR(ADC_vect){
	
	switch (caso){
		case 0:
		ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0)); //Borrar configuracion actual y poner ADC1
		Val1 = ADCH;
		caso = 1;
		break;
		
		case 1:
		ADMUX &= ~((1<<MUX2)|(1<<MUX1)|(1<<MUX0));   //Borrar configuracion actual y poner ADC0
		ADMUX |= (1<<MUX0);
		Val2 = ADCH;
		caso = 0;
		break;
	}
	
	ADCSRA |= (1<<ADIF); //Se borra la bandera de interrupción

}



ISR(USART_RX_vect)
{
	receivedChar = UDR0; // Almacena el carácter recibido
	
	while(!(UCSR0A & (1<<UDRE0)));    //Mientras haya caracteres
}

