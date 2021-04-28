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

#define _XTAL_FREQ 125000

// VARIABLES GLOBALES

uint8_t Banderas;
uint8_t ADRESH_TEMP1; // canal 12
uint8_t ADRESH_TEMP2; // canal 10

// PROTOTIPO DE FUNCIONES
void setup(void);

// INTERRUPCION
void __interrupt() isr (void)
{
    // INTERRUPCION PARA ADC
    if (ADIF == 1){
        switch(Banderas){
            case(1): 
                ADRESH_TEMP1 = ADRESH;
                ADCON0bits.CHS = 0x00;//Seleccionamos canal 10 (SEGUNDO POTENCIOMETRO)
                Banderas = 2;
                ADIF = 0;// LIMPIAMOS BANDERA 
                break;
            case(2): 
                ADRESH_TEMP2 = ADRESH;
                ADCON0bits.CHS = 0x01;//Seleccionamos canal 12 (PRIMER POTENCIOMETRO)
                Banderas = 1;
                ADIF = 0;// LIMPIAMOS BANDERAA
                break;
        }        
    }
} 
void main(void){
    // CONFIGURACION INICIAL
setup();
// VALORES INICIALES
Banderas = 1; // valores iniciales para que haga conversión
while(1)// LOOP PRINCIPAL
{   
    ADCON0bits.GO_DONE = 1;
    CCPR1L = (ADRESH_TEMP1>>3)-3;// SALIDA A PRIMER SERVO
    CCPR2L = (ADRESH_TEMP2>>3)-3;// SALIDA A SEGUNDO SERVO
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
    PORTA = 0X03;
    PORTC = 0X00;

    ANSEL = 0X03;
    ANSELH = 0X00;
    TRISC = 0X00;
    TRISA = 0X03;

    // INTERRUPCIONES
    INTCONbits.GIE = 1;// HABILITAR INTERRUPCIONES GLOBALES
    INTCONbits.PEIE= 1;// HABILITAR INTERRUPCIONES PERIFERICOS
    
    // HABILITAR INTERRUPCION DEL ADC
    PIE1bits.ADIE = 1;
    
    //CONFIGURAMOS EL CONVERSOR ADC
    ADCON1bits.ADFM = 0;// JUSTIFICAMOS A LA IZQUIERDA-
    ADCON0bits.ADCS = 0b00;// AJUSTAMOS FUENTE DE RELOJ Fosc/2
    PIR1bits.ADIF = 0;// BAJAMOS BANDERA DE LA INTERRUPCION DEL ADC 
    ADCON0bits.ADON = 1;//ACTIVAMOS LAS CONVERSIONES CON EL ADON

    //CONFIGURACION DE PWM
    TRISB = 0XFF;
    
    PR2 = 157;//157
    CCP1CONbits.P1M = 0x00; // SINGLE OUTPUT 
    CCP1CONbits.CCP1M = 0x0C; // PWM MODE; P1A,P1C active-high; P1B,P1D active-high  
    
    CCP2CONbits.CCP2M = 0x0F; // PWM MODE; P1A,P1C active-high; P1B,P1D active-high  
    
    CCPR1L = 0x0F;  // ALGUNOS VALORES INICIALES MÁS SIGNIFICATIVOS
    CCPR2L = 0x0F;
    CCP1CONbits.DC1B = 0x00; // ALGUNOS VALORES INICIALES MENOS SIGNIFICATIVOS
    CCP2CONbits.DC2B0 = 0; 
    CCP2CONbits.DC2B1 = 0; 
    
    //CONFIGURACION TIMER2
    PIR1bits.TMR2IF = 0; // BANDERA DEL TIMER2
    T2CONbits.T2CKPS = 0x01;// PRESCALER EN 4
    T2CONbits.TOUTPS = 0x00;// POSTCALER EN 1:1
    T2CONbits.TMR2ON = 0x01;// ENCENDEMOS TIMER2
    
    while(PIR1bits.TMR2IF == 0); // Comenzar hasta que el segundo ciclo empiece
    TRISC = 0;   // activamos las salidas pwm
    
}





   
