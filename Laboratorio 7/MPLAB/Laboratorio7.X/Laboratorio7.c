/* 
 * File:   Laboratorio7.c  parte 1
 * Author: ESTEPHAN PORTALES
 *

 */

//#include <stdio.h>
//#include <stdlib.h>


// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
// VALOR DE TIMER 0
#define _tmr0_value 177 //177 Para 5ms

// VARIABLES

//uint8_t contador1;



// PROTOTIPO DE FUNCIONES
void setup(void);

// INTERRUPCION
void __interrupt() isr (void)
{
    // INTERRUPCION TIMER0
    if (T0IF)
    { 
        PORTC++;
        TMR0 = _tmr0_value;
        T0IF = 0;
    }
    // INTERRUPCION PUERTOB
    if (RBIF)
    {
        if (RB0 == 0){
            PORTD++;
        }
        if (RB1 == 0){
            PORTD--;
        }  
        
        RBIF = 0;   
    } 
          
}

void main(void){
    // CONFIGURACION INICIAL
setup();

while(1)
{
// NO HAY NADA POR EL MOMENTO

}
}


void setup (void)
{
    // CONFIGURACION DE RELOJ
    OSCCONbits.SCS = 1;
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.IRCF1 = 0; // 125KHztenemos  Hz
    OSCCONbits.IRCF0 = 1;
    
    
    //CONFIGURACION IO
    
    PORTC = 0X00;
    PORTB = 0X03;
    PORTD = 0X00;

            
    ANSEL = 0X00;
    ANSELH = 0X00;
    TRISC = 0X00;
    TRISB = 0X03;
    TRISD = 0X00;


    //ENABLE PULLUP PORTB
    OPTION_REGbits.nRBPU = 0;
    IOCB = 0X03;
    
    //CONFIGURACION DE PULLUP PORTB
    WPUB = 0X03;

    // CONFIGURAR TMR0
    OPTION_REGbits.T0CS = 0;// Configuramos (Fosc/4)
    OPTION_REGbits.PSA = 0; // PSA asignamos el prescaler al Timer0
    OPTION_REGbits.PS2 = 0; // prescaler 1:2
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;
    TMR0 = _tmr0_value;
    
    // HABILITAR INTERRUPCIONES TMR0 Y PORTB
    //INTCONbits.RBIF = 0;
    INTCONbits.T0IF = 0;

    //INTCONbits.RBIE = 1;
    INTCONbits.T0IE = 1;
    INTCONbits.GIE = 1;
}

