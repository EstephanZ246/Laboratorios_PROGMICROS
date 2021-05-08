/* 
 * File:   Laboratorio9.c
 * Author: ESTEPHAN PORTALES
 *
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
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
#include <xc.h>     // LIBRERIAS
#include <stdint.h>

#define _XTAL_FREQ 1000000

// VARIABLES GLOBALES

uint8_t Banderas;
uint8_t Entrada;


// PROTOTIPO DE FUNCIONES
void setup (void);
void print(unsigned char* palabra);
void enviar_letra(char letra);
void println(unsigned char *palabra);
void menu(void);

// INTERRUPCION
void __interrupt() isr (void)
{
    // INTERRUPCION DEL EUSART
    if (RCIF){
       Entrada =  RCREG;
       RCREG = 0; 
    }  
    }
    
void main(void){
    // CONFIGURACION INICIAL
setup();
menu();
while(1)// LOOP PRINCIPAL
{   
    /*TRMT PARA TSR 1 = VACIO Y 0= OCUPAO
     TXIF PARA TXREG 1 = VACIO Y 0= OCUPAO*/
    if (Entrada == 0x31){
        println("Este es el texto de la parte 1");
        println("");
        menu();
        Entrada = 0;
    }
    
    else if (Entrada == 0x32){
        println("Que valor quiere desplegar en el puerto A?");
        Entrada = 0;
        while(Entrada == 0);
        PORTA = Entrada;
        Entrada = 0;
        menu();
    }
    else if (Entrada == 0x33){
        println("Que valor quiere desplegar en el puerto b?");
        Entrada = 0;
        while(Entrada == 0);
        PORTB = Entrada;
        Entrada = 0;
        menu();
    }
 
}
}
void setup (void)
{
    // CONFIGURACION DE RELOJ
    OSCCONbits.SCS = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 0; // 1MHz
    OSCCONbits.IRCF0 = 0;

    //CONFIGURACION IO
    PORTA = PORTB = 0X00;
    ANSEL = ANSELH = 0X00;
    TRISB = TRISA = 0X00;
    

    // INTERRUPCIONES
    INTCONbits.GIE = 1;// HABILITAR INTERRUPCIONES GLOBALES
    INTCONbits.PEIE= 1;// HABILITAR INTERRUPCIONES PERIFERICOS
    
    // CONFIGURAR EUSART TRANSMITER

    SPBRGH = 0; //BYTE SUPERIOR 9600 BAUD RATE
    SPBRG = 25; //BYTE INFERIOR
    BRGH = 1;
    BRG16 = 1;
    
    TXSTAbits.SYNC= 0; // LO PONEMOS COMO ASINCRONO
    RCSTAbits.SPEN= 1; // HABILITA EL EUSART Y CONFIGURA LOS TINES TX/CK I/O COMO SALIDA
    TXSTAbits.TXEN= 1; // ACTIVA EL CIRCUITO PARA TRASMISOR DEL EUSART
    RCSTAbits.CREN = 1;// ACTIVA EL CIRCUITO PARA EL RECEPTOR DE EUSART
    //PIE1bits.TXIE = 1; //ES PARA INTERRUPCIONES TRANSMISOR
    PIE1bits.RCIE = 1; //ES PARA INTERRUPCIONES RECEPTOR  
}



void print(unsigned char *palabra){
    while((*palabra) != '\0'){
        while(!TXIF);
        TXREG = (*palabra);
        *palabra++;
          
  }
}
void println(unsigned char *palabra){
    while((*palabra) != '\0'){
        while(!TXIF);
        TXREG = (*palabra);
        *palabra++;
          
  }
    enviar_letra('\r');
}

void enviar_letra(char letra){
    while(!TXIF);
    TXREG = letra;   
}

void menu(void){
    println("");
    println("");
    println("Que accion desea ejecutar?");
    println("(1) Desplegar cadena de caracteres");
    println("(2) Cambiar PORTA");
    println("(3) Cambiar PORTB");
    println("");
    println("");
}
    
    
    
    
      
  
    
    
    
 
    
  






   
