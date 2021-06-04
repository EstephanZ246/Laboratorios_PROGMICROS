/* UNIVERSIDAD DEL VALLE DE GUATEMALA
 * File:   PROYECTO 2
 * Author: ESTEPHAN PORTALES
 *
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT disabled)
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

uint8_t ADRESH_CANAL1;//L
uint8_t ADRESH_CANAL2;//SERVO 2
uint8_t Banderas; // Para cambio de canales ADC
uint8_t Entrada ;
uint8_t velocidad ;
uint8_t temperatura ;
uint8_t luz ;
uint8_t bandera_monster ;
uint8_t bandera_luz ;

uint8_t mandar_datos ;
uint8_t Unidadt ;
uint8_t Decenat ;
uint8_t Centenat ;
uint8_t Unidadl ;
uint8_t Decenal ;
uint8_t Centenal ;


       


// PROTOTIPO DE FUNCIONES
void setup (void);
void print(unsigned char* palabra);
void enviar_letra(char letra);
void println(unsigned char *palabra);
void BCD_LUZ (int valor1);
void BCD_TEMPERATURA (int valor2);
int int_string(int valor);



// INTERRUPCION
void __interrupt() isr (void)
{
    // INTERRUPCION DEL EUSART
    if (RCIF){
       Entrada =  RCREG;
       RCREG = 0; 
    }  
    // INTERRUPCION ADC
    if (ADIF == 1){
        switch(Banderas){
            case(1): 
                ADRESH_CANAL1 = ADRESH;
                ADCON0bits.CHS = 0x01;//Seleccionamos canal 10 (SEGUNDO POTENCIOMETRO)
                Banderas = 2;
                ADIF = 0;// LIMPIAMOS BANDERA 
                break;
            case(2): 
                ADRESH_CANAL2 = ADRESH;
                ADCON0bits.CHS = 0x00;//Seleccionamos canal 12 (PRIMER POTENCIOMETRO)
                Banderas = 1;
                ADIF = 0;// LIMPIAMOS BANDERAA
                break;
        }        
    }// INTERRUPCION TIMER 1
    if (PIR1bits.TMR1IF){
        if (!mandar_datos){
            mandar_datos = 1;
        }
       TMR1H = 11;           
       TMR1L = 71; 
       PIR1bits.TMR1IF = 0;
    }
    }
    
void main(void){
    // CONFIGURACION INICIAL
setup();
Banderas = 1; // valores iniciales para que haga conversión
while(1)// LOOP PRINCIPAL
{   
    luz = ADRESH_CANAL1*(0.3921);
    temperatura = (ADRESH_CANAL2*(0.7058))-68;
    BCD_TEMPERATURA(temperatura);
    BCD_LUZ(luz);
    bandera_monster = PORTAbits.RA7 ;
    bandera_luz = PORTAbits.RA2;
    ADCON0bits.GO_DONE = 1;
    //Velocidades de carro 
    if(Entrada == 'V'){
        CCPR1L =63;//30
        velocidad = CCPR1L;
        Entrada = 0;
    }else if(Entrada == 'B'){
        CCPR1L = 126;//60
        velocidad = CCPR1L;
        Entrada = 0;
    }else if(Entrada == 'N'){
        CCPR1L = 189;//90
        velocidad = CCPR1L;
        Entrada = 0;
    }else if(Entrada == 'M'){
        CCPR1L = 255;//120
        velocidad = CCPR1L;
        Entrada = 0;
        //DIRECCION DEL CARRO 
    }else if(Entrada == 'S'){// HACIA ATRAS
        PORTAbits.RA3=1 ;
        PORTAbits.RA4=0 ;
        PORTAbits.RA5=0 ;
        PORTAbits.RA6=1 ;
        PORTBbits.RB0=0 ;
        Entrada = 0;
    }else if(Entrada == 'A'){//GIRAR IZQUIERDA
        PORTAbits.RA3=1 ;
        PORTAbits.RA4=0 ;
        PORTAbits.RA5=1 ;
        PORTAbits.RA6=0 ;
        PORTBbits.RB0=0 ;

        Entrada = 0;
    }else if(Entrada == 'W'){//HACIA DELANTE
        PORTAbits.RA3=0 ;
        PORTAbits.RA4=1 ;
        PORTAbits.RA5=1 ;
        PORTAbits.RA6=0 ;
        PORTBbits.RB0=0 ;
        Entrada = 0;
    }else if(Entrada == 'D'){//GIRAR DERECHA
        PORTAbits.RA3=0 ;
        PORTAbits.RA4=1 ;
        PORTAbits.RA5=0 ;
        PORTAbits.RA6=1 ;
        PORTBbits.RB0=0 ;
        Entrada = 0;
    }else if(Entrada == 'P'){// PARAR MOTORES
        PORTAbits.RA3=0 ;
        PORTAbits.RA4=0 ;
        PORTAbits.RA5=0 ;
        PORTAbits.RA6=0 ;
        PORTBbits.RB0=1 ;
        Entrada = 0;
    }else if(Entrada == 'G'){
        INTCONbits.GIE = 0;// HABILITAR INTERRUPCIONES GLOBALES
        INTCONbits.PEIE= 0;// HABILITAR INTERRUPCIONES PERIFERICOS
        EECON1bits.WREN = 1; // permite operar sobre eeprom
        PORTBbits.RB2 = 0;
        PORTBbits.RB1 = 1;
        
        EEDAT = velocidad; // GUARDAR VELOCIDAD EN EEPROM
        EEADR = 0;
        EECON2 = 0X55;
        EECON2 = 0XAA;
        EECON1bits.WR=1;
        while(EEIF ==0);
        EEIF = 0;
        
        EEDAT = temperatura;  // GUARDAR TEMPERATURA EN EEPROM
        EEADR = 1;
        EECON2 = 0X55;
        EECON2 = 0XAA;
        EECON1bits.WR=1;
        while(EEIF ==0);
        EEIF = 0;
        
        EEDAT = luz; // GUARDAR LUZ AMBIENTAL EN EEPROM
        EEADR = 2;
        EECON2 = 0X55;
        EECON2 = 0XAA;
        EECON1bits.WR=1;
        while(EEIF ==0);
        EEIF = 0;
        
        EEDAT = bandera_monster; // FUNCION NO IMPLEMENTADA (CARRO MONSTRUO))
        EEADR = 3;
        EECON2 = 0X55;
        EECON2 = 0XAA;
        EECON1bits.WR=1;
        while(EEIF ==0);
        EEIF = 0;
        
        EEDAT = bandera_luz; // GUARDAR ESTADO DEL LUZ EN EEPROM
        EEADR = 4;
        EECON2 = 0X55;
        EECON2 = 0XAA;
        EECON1bits.WR=1;
        while(EEIF ==0);
        EEIF = 0;
  
        EECON1bits.WREN = 0; // permite operar sobre eeprom
        INTCONbits.GIE = 1;// HABILITAR INTERRUPCIONES GLOBALES
        INTCONbits.PEIE= 1;// HABILITAR INTERRUPCIONES PERIFERICOS
        PORTBbits.RB2 = 1;
        PORTBbits.RB1 = 0;
        Entrada = 0;
        
    }else if(Entrada == 'L'){
        INTCONbits.GIE = 0;// HABILITAR INTERRUPCIONES GLOBALES
        INTCONbits.PEIE= 0;// HABILITAR INTERRUPCIONES PERIFERICOS
        EECON1bits.WREN = 1; // permite operar sobre eeprom
        
        EEADR = 0; // AQUÍ LEEMOS LOS DATOS GUARDADOS EN LA EEPROM Y LAS COLOCAMOS ES LAS VARIABLES PARA QUE SE EJECUTEN 
        EEPGD = 0;
        EECON1bits.RD = 1;
        CCPR1L = EEDAT;
        
        EEADR = 3;
        EEPGD = 0;
        EECON1bits.RD = 1;
        PORTAbits.RA2 = EEDAT;
        
        EEADR = 4;
        EEPGD = 0;
        EECON1bits.RD = 1;
        PORTAbits.RA7 = EEDAT;
        
        EECON1bits.WREN = 0; // permite operar sobre eeprom
        INTCONbits.GIE = 1;// HABILITAR INTERRUPCIONES GLOBALES
        INTCONbits.PEIE= 1;// HABILITAR INTERRUPCIONES PERIFERICOS
        Entrada = 0;
    }else if(Entrada == 'E'){   // ENCENDER LUZ DE ENFRENTE
        PORTAbits.RA2=1^PORTAbits.RA2;  
        Entrada = 0;
    }else if(Entrada == 'F'){ // ESTA ES UNA FUNCION NO IMPLEMENTADA
        PORTAbits.RA7=1^PORTAbits.RA7;
        Entrada = 0;
    }
    
    if (mandar_datos){ // AQUI MANDAMS LOS DATOS DE TEMPERATURA Y HUMEDAD A LA COMPUTADORA
       print("TEMPERATURA: "); 
       print(int_string(Centenat));
       print(int_string(Decenat));
       print(int_string(Unidadt));
       print("C  ");
       print("LUZ AMBIENTAL: ");
       print(int_string(Centenal));
       print(int_string(Decenal));
       print(int_string(Unidadl));
       print("\r");
       print("\n");
       mandar_datos = 0;
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
    PORTA = 0X03;// PARA ADC
    PORTB = 0X05;// SALIDAS A PUENTE EN H
    PORTCbits.RC1=0;// SALIDAS DE SERVO DIRECCIÓN DE CARRO, Y VELOCIDAD DE CARRO
    PORTCbits.RC2=0;// SALIDAS DE SERVO DIRECCIÓN DE CARRO, Y VELOCIDAD DE CARRO
    PORTD = 0X00;
    
    ANSEL = 0X03; // ENTRADAS PARA ADC
    ANSELH = 0X00;
    TRISB = 0X00;// SALIDAS A PUENTE EN H
    TRISA = 0X03;// PARA ADC, MOVER CARRO, LUZ DE CARRO EN FRENTE
    TRISD = 0X00;
    //TRISC = 0X06;
    TRISCbits.TRISC1=0;// SALIDAS DE SERVO DIRECCIÓN DE CARRO, Y VELOCIDAD DE CARRO
    TRISCbits.TRISC2=0;// SALIDAS DE SERVO DIRECCIÓN DE CARRO, Y VELOCIDAD DE CARRO
    
   
    // INTERRUPCIONES
    INTCONbits.GIE = 1;// HABILITAR INTERRUPCIONES GLOBALES
    INTCONbits.PEIE= 1;// HABILITAR INTERRUPCIONES PERIFERICOS
    PIE1bits.ADIE = 1;// HABILITAR INTERRUPCION DEL ADC
    
    // CONFIGURAR EUSART TRANSMITER Y RECEPTOR

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
    
    // PWM
    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 1;
    TRISCbits.TRISC3 = 1;

    PR2 = 255;//124 2ms
    CCP1CONbits.P1M = 0x00; // SINGLE OUTPUT 
    CCP1CONbits.CCP1M = 0x0C; // PWM MODE; P1A,P1C active-high; P1B,P1D active-high  
    
    CCP2CONbits.CCP2M = 0x0F; // PWM MODE; P1A,P1C active-high; P1B,P1D active-high  
    
    CCPR1L = 0;  // ALGUNOS VALORES INICIALES MÁS SIGNIFICATIVOS, ESCALA ES 0-120
    CCPR2L = 2;
    CCP1CONbits.DC1B = 0x00; // ALGUNOS VALORES INICIALES MENOS SIGNIFICATIVOS
    CCP2CONbits.DC2B0 = 0; 
    CCP2CONbits.DC2B1 = 0; 
    
    //ADC
    VCFG1 = 0; //VSS
    VCFG0 = 0; // VDD
    ADCON1bits.ADFM = 0;// JUSTIFICAMOS A LA IZQUIERDA-
    ADCON0bits.ADCS = 0b00;// AJUSTAMOS FUENTE DE RELOJ Fosc/2
    PIR1bits.ADIF = 0;// BAJAMOS BANDERA DE LA INTERRUPCION DEL ADC 
    ADCON0bits.ADON = 1;//ACTIVAMOS LAS CONVERSIONES CON EL ADON
    
    
    //TIMER 2
    //CONFIGURACION TIMER2
    PIR1bits.TMR2IF = 0; // BANDERA DEL TIMER2
    T2CONbits.T2CKPS = 0x01;// PRESCALER EN 4 01
    T2CONbits.TOUTPS = 0x00;// POSTCALER EN 1:1
    T2CONbits.TMR2ON = 0x01;// ENCENDEMOS TIMER2
    
    while(PIR1bits.TMR2IF == 0); // Comenzar hasta que el segundo ciclo empiece se habilita el pwm
    TRISC1 = 0;
    TRISC2 = 0;
    
   // TIMER1

T1CONbits.T1CKPS1 =1;
T1CONbits.T1CKPS0 =1;
T1CONbits.T1OSCEN =1;
T1CONbits.T1SYNC =1;
T1CONbits.TMR1CS =0;
T1CONbits.TMR1ON =1;
PIE1bits.TMR1IE = 1;
TMR1H = 11;           
TMR1L = 71;            
}

void print(unsigned char *palabra){ // FUNCION IMPRIMIR PALABRA SIN "ENTER"
    while((*palabra) != '\0'){
        while(!TXIF);
        TXREG = (*palabra);
        *palabra++;
          
  }
}
void println(unsigned char *palabra){//FUNCION IMPRIMIR PALABRA CON "ENTER"
    while((*palabra) != '\0'){
        while(!TXIF);
        TXREG = (*palabra);
        *palabra++;
          
  }
    enviar_letra('\r');
}

void enviar_letra(char letra){// ENVIAR SOLO LETRA
    while(!TXIF);
    TXREG = letra;   
}

void BCD_LUZ (int valor1){ // PARA PODER DESCOMPONER VALOR DE LUZ, PARA MANDARLO A LA PC
    int Temporal1;
    Centenal = valor1 /100;
    Temporal1 = valor1 % 100;
    Decenal = Temporal1 /10;
    Unidadl = Temporal1 % 10; 
    
} 
void BCD_TEMPERATURA (int valor2){ // PARA PODER DESCOMPONER VALOR DE TEMPERATURA, PARA MANDARLO A LA PC
    int Temporal2;
    Centenat = valor2 /100;
    Temporal2 = valor2 % 100;
    Decenat = Temporal2 /10;
    Unidadt = Temporal2 % 10; 
    
} 

int int_string(int valor){ // PARA PODER MANDAR EL NUMERO A LA PC, PORQUE SI SOLO LE MANDAMOS EL NUMERO, 
                             // QUE NO SEA STRING PUEDE MANDAR ALGO NADA QUE VER
    switch (valor){
        case(0): return("0"); break;
        case(1): return("1"); break;
        case(2): return("2"); break;
        case(3): return("3"); break;
        case(4): return("4"); break;
        case(5): return("5"); break;
        case(6): return("6"); break;
        case(7): return("7"); break;
        case(8): return("8"); break;
        case(9): return("9"); break; 
    }
}


    
    
    
      
  
    
    
    
 
    
  






   
