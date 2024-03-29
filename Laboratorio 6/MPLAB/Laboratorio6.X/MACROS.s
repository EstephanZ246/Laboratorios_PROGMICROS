





; MACROS DE CONFIGURACION
 

CONFIGURAR_IO MACRO
    BANKSEL ANSEL // SALIDA DIGITALES
    CLRF ANSEL// SALIDA DIGITALES
    CLRF ANSELH// SALIDA DIGITALES
    
    BANKSEL TRISA
    CLRF TRISA; SALIDA PARA CONFIGURACION TIEMPO DISPLAYS
    CLRF TRISC
    CLRF TRISE
    CLRF TRISB
    BANKSEL PORTA
    
    CLRF PORTA
    CLRF PORTC
    CLRF PORTD
    CLRF PORTE
    CLRF PORTB
    ENDM


    CONFIGURAR_INTERRUPCIONES MACRO
    BANKSEL INTCON
    BSF INTCON,7//GIE
    BSF INTCON,5//T0IE HABILITA INTERRUPCION TMR0
    BCF INTCON,2//T0IF ESTA ES LA BANDERA DE TMR0
    BSF INTCON,6 // HABILITAMOS PERIPHERIAL INTERRUPT PARA TIMER 1 Y TIMER 2
    BANKSEL PIE1 // HABILITAMOS INTERRUPCIÓN
    BSF PIE1,0 // TIMER1 OVERFLOW INTERRUPT
    BANKSEL PORTA
    ENDM
    

   CONFIGURAR_RELOJ MACRO
    BANKSEL OSCCON// Seleccionamos el banco OSCCON 
    BSF OSCCON,6// IRCF2
    BCF OSCCON,5// IRCF1 Configuramos la velocidad a 31kHz
    BCF OSCCON,4// IRCF0
    BCF SCS//OSCCON,0 // SCS // Colocamos el oscilador interno como reloj del sistema
    BANKSEL PORTA // Vamos a banco 0
    ENDM
    
; ___________________________ Configuramos el prescaler_______________________
    CONFIGURAR_TMR0 MACRO
    BANKSEL TRISA // Vamos a banco 1
    BCF T0CS // Configuramos (Fosc/4)
    BCF PSA// PSA asignamos el prescaler al Timer0
    BSF PS2//
    BSF PS1// configuramos el prescaler a 1:256
    BSF PS0//
    BANKSEL PORTA// Vamos al banco 0
    REINICIAR_TMR0 // Reiniciamos el timer0 con la subrutina
    ENDM
    
    CONFIGURAR_TMR1 MACRO
    //PRESCALER
    BANKSEL T1CON
    BSF T1CON,5//T1CKPS1 // PREESCALER DE 1:8
    BSF T1CON,4//T1CKPS0
    
    BCF T1CON,1//TMR1CS
    BSF T1CON,0//TMR1ON
    BANKSEL PORTA
    REINICIAR_TMR1
    ENDM
    
    REINICIAR_TMR0 MACRO 
    BANKSEL TMR0
    MOVLW 240 //248.25 Colocamos valor obtenido con formula para tener 2ms=4*(1/31000)*(256-TMR0)*(2)
    MOVWF TMR0 // movemos el valor a timer0
    BCF T0IF; Limpiamos bandera de Timer0 
    BANKSEL PORTA
    ENDM

    REINICIAR_TMR1 MACRO
    BANKSEL TMR1L
    /////// TIEMPO ES 3036
    MOVLW 0X37
    //MOVLW 0XEF
    MOVWF TMR1L
    BANKSEL TMR1H
    MOVLW 0XFC
    //MOVLW 0XFF
    MOVWF TMR1H
    BANKSEL PIR1
    BCF PIR1,0; TMR1IF Limpiamos bandera de Timer1
    BANKSEL PORTA
    ENDM
    

   