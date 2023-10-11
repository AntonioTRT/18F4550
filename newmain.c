#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Definiciones de pines para el LCD
#define RS LATD0
#define EN LATD1
#define D4 LATD2
#define D5 LATD3
#define D6 LATD4
#define D7 LATD5

// Declara las funciones de las que dependes
void LCD_comando(char cmd);
void LCD_caracter(char data);

#pragma config FOSC = INTOSCIO_EC
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config MCLRE = OFF
#define _XTAL_FREQ 8000000


////////////////////////////////
///////////funciones///////////
///////////////////////////////
///
// Función para mostrar un mensaje en el LCD



void mostrarMensaje(char *mensaje) {
    LCD_comando(0xC0); 
      int len = strlen(mensaje);
    for (int i = 0; i < len; i++) {
        LCD_caracter(mensaje[i]);
        __delay_ms(100);
    }
}


void main() {
    OSCCON = 0x60;
    
    //Push
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    //ADC IN
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 1;
    TRISAbits.TRISA6 = 1;
    
    //LEd
    TRISB = 0x00;
    //lcd
    TRISD = 0x00;
    
    
    TRISE = 0x00;
    unsigned char contador = 75; // Cambiado a unsigned char
    // Configurar RC2 PWM
    TRISCbits.TRISC2 = 0;
    PR2 = 255;
    CCPR1L = 0; // Cambiado a unsigned char
    CCP1CON = 0x0C;
    T2CON = 0x05;
    // Inicializar el LCD
    LCD_comando(0x02); // Modo de 4 bits
    LCD_comando(0x28); // 2 líneas, 5x7 matriz de puntos
    LCD_comando(0x0E); // Enciende la pantalla, el cursor y el cursor parpadeante
    LCD_comando(0x01); // Borra la pantalla
    __delay_ms(2); 
    LATB = 0b01111111;
    __delay_ms(100);
    // comandos lcd
    LCD_comando(0x80); // Establecer la posición del cursor en la primera línea
    char mensaje[] = "iniciando ....";
    for (int i = 0; i < 14; i++) {
        LCD_caracter(mensaje[i]);
        __delay_ms(100);
    }
      LATB = 0b0000000;  
      //borrar pantalla
    LCD_comando(0x01); // Envía el comando para borrar la pantalla
    __delay_ms(2); // Pequeña pausa para dar tiempo al LCD a borrar la pantalla
    
////////////////////////////////
///////////loop principal///////////
///////////////////////////////
    while (1) {
        int reiniciar = 0; // Inicialmente no reiniciar
         if (reiniciar==0) {
        mostrarMensaje("selecciona");// Realiza alguna acción antes de volver al inicio del bucle
       // reiniciar = 0; // Restablece la variable para evitar un reinicio continuo
        continue; // Salta de nuevo al inicio del bucle
    }

        
        
        if (contador > 1) {
            contador = contador - 1;
        }
        __delay_ms(50);

        CCPR1L = contador;
        
        if (PORTAbits.RA0 == 1) {
            LCD_comando(0x80);
            char mensaje[] = " ....";
            for (int i = 0; i < 14; i++) {
                LCD_caracter(mensaje[i]);
                __delay_ms(1000);
            }
            contador = 255;
            __delay_ms(500);
        }
       //  if (alguna_condicion) {
        // Si se cumple una condición, establece reiniciar a 1 para volver al inicio del bucle
        reiniciar = 1;
   // }
        ///// fin de while
    }
}




void LCD_comando(char cmd) {
    RS = 0;
    PORTD = (cmd & 0xF0) | (PORTD & 0x0F);
    EN = 1;
    __delay_ms(1);
    EN = 0;
    __delay_ms(1);

    PORTD = ((cmd << 4) & 0xF0) | (PORTD & 0x0F);
    EN = 1;
    __delay_ms(1);
    EN = 0;
    __delay_ms(1);
}

void LCD_caracter(char data) {
    RS = 1;
    PORTD = (data & 0xF0) | (PORTD & 0x0F);
    EN = 1;
    __delay_ms(1);
    EN = 0;
    __delay_ms(1);
    PORTD = ((data << 4) & 0xF0) | (PORTD & 0x0F);
    EN = 1;
    __delay_ms(1);
    EN = 0;
    __delay_ms(1);
}
