/* 
 * File:   Laboratorio8.c
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

#define _tmr0_value 177 //177 Para 5ms

// VARIABLES GLOBALES
uint8_t Centena;
uint8_t Decena;
uint8_t Unidad;
uint8_t Banderas;
uint8_t ADRESH_TEMP1; // canal 12
uint8_t ADRESH_TEMP2; // canal 10
uint8_t muxeo;

// PROTOTIPO DE FUNCIONES
void setup(void);
void Decimales (void);
int Display(int Numero);

// INTERRUPCION
void __interrupt() isr (void)
{
    // INTERRUPCION PARA ADC
    if (ADIF == 1){
        switch(Banderas){
            case(1): 
                ADRESH_TEMP1 = ADRESH;
                Banderas = 2;
                break;
            case(2): 
                ADRESH_TEMP2 = ADRESH;
                Banderas = 1;
                break;
        }        
        ADIF = 0;// LIMPIAMOS BANDERA    
    }
    if (T0IF ==1){
        switch (muxeo){
            case(1): muxeo = 2; break;
            case(2): muxeo = 4; break;
            case(4): muxeo = 1; break;
        } 
        TMR0 = _tmr0_value;
        T0IF = 0;// BAJAMOS BANDERA
    }  
    } 
void main(void){
    // CONFIGURACION INICIAL
setup();
// VALORES INICIALES
Banderas = 1; // valores iniciales para que haga conversión
muxeo = 1;
while(1)// LOOP PRINCIPAL
{   
    Decimales();
    switch(Banderas){
        case(1):
            ADCON0bits.CHS = 0x0C;//Seleccionamos canal 12 (PRIMER POTENCIOMETRO)
            ADCON0bits.GO_DONE = 1;
            break;
            
        case(2):
            ADCON0bits.CHS = 0x0A;//Seleccionamos canal 10 (SEGUNDO POTENCIOMETRO)
            ADCON0bits.GO_DONE = 1;
            break;
    }
    PORTC = ADRESH_TEMP1;
    switch(muxeo){
        case(1):  PORTA = 0; PORTE = 0X01; Display(Centena); break;
        case(2):  PORTA = 0; PORTE = 0X02; Display(Decena); break;
        case(4):  PORTA = 0; PORTE = 0X04; Display(Unidad); break;
    } 
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
    

    PORTB = 0X03;
    PORTC = 0X00;
    PORTE = 0X00;
    PORTA = 0X00;
     
    ANSEL = 0X00;
    ANSELH = 0X14;
    TRISC = 0X00;
    TRISB = 0X03;
    TRISE = 0X00;
    TRISA = 0X00;


    //ENABLE PULLUP PORTB
    //OPTION_REGbits.nRBPU = 0;
    //IOCB = 0X0F;
    
    //CONFIGURACION DE PULLUP PORTB
    //WPUB = 0X0F;

    // CONFIGURAR TMR0
    OPTION_REGbits.T0CS = 0;// Configuramos (Fosc/4)
    OPTION_REGbits.PSA = 0; // PSA asignamos el prescaler al Timer0
    OPTION_REGbits.PS2 = 0; // prescaler 1:2
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;
    TMR0 = _tmr0_value;
    
    // HABILITAR INTERRUPCIONES TMR0 Y PORTB
    //INTCONbits.RBIF = 0;
    INTCONbits.T0IF = 0; // BANDERA TIMER0

    //INTCONbits.RBIE = 1;
    INTCONbits.T0IE = 1;// HABILITAR INTERRUPCIONES TIMER0
    INTCONbits.GIE = 1;// HABILITAR INTERRUPCIONES GLOBALES
    INTCONbits.PEIE= 1;// HABILITAR INTERRUPCIONES PERIFERICOS
    
    // HABILITAR INTERRUPCION DEL ADC
    PIE1bits.ADIE = 1;
    
    //CONFIGURAMOS EL CONVERSOR ADC
    ADCON1bits.ADFM = 0;// JUSTIFICAMOS A LA IZQUIERDA-
    ADCON0bits.ADCS = 0b00;// AJUSTAMOS FUENTE DE RELOJ Fosc/2
    PIR1bits.ADIF = 0;// BAJAMOS BANDERA DE LA INTERRUPCION DEL ADC 
    ADCON0bits.ADON = 1;//ACTIVAMOS LAS CONVERSIONES CON EL ADON
    
    
}
// CONVERSOR BCD
void Decimales (void){ 
    int Temporal;
    Centena = ADRESH_TEMP2 /100;
    Temporal = ADRESH_TEMP2 % 100;
    Decena = Temporal /10;
    Unidad = Temporal % 10; 
    
} 
// TABLAS PARA DISPLAY 7 SEGMENTOS
int Display(int Numero){
    switch (Numero){
        case(0): PORTA = (0b0111111);
        break;
        case(1): PORTA = (0b0000110);
        break;
        case(2): PORTA = (0b1011011);
        break;
        case(3): PORTA = (0b1001111);
        break;
        case(4): PORTA = (0b1100110);
        break;
        case(5): PORTA = (0b1101101);
        break;
        case(6): PORTA = (0b1111101);
        break;
        case(7): PORTA = (0b1111111);
        break;
        case(8): PORTA = (0b1111111);
        break;
        case(9): PORTA = (0b1100111);
        break;
    }  
}
