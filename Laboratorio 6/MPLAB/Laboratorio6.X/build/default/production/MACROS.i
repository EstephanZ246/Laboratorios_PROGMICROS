# 1 "MACROS.s"
# 1 "<built-in>" 1
# 1 "MACROS.s" 2






; MACROS DE CONFIGURACION


CONFIGURAR_IO MACRO
    BANKSEL ANSEL
    CLRF ANSEL
    CLRF ANSELH

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
    BSF INTCON,7
    BSF INTCON,5
    BCF INTCON,2
    BSF INTCON,6
    BANKSEL PIE1
    BSF PIE1,0
    BANKSEL PORTA
    ENDM


   CONFIGURAR_RELOJ MACRO
    BANKSEL OSCCON
    BSF OSCCON,6
    BCF OSCCON,5
    BCF OSCCON,4
    BCF SCS
    BANKSEL PORTA
    ENDM

; ___________________________ Configuramos el prescaler_______________________
    CONFIGURAR_TMR0 MACRO
    BANKSEL TRISA
    BCF T0CS
    BCF PSA
    BSF PS2
    BSF PS1
    BSF PS0
    BANKSEL PORTA
    REINICIAR_TMR0
    ENDM

    CONFIGURAR_TMR1 MACRO

    BANKSEL T1CON
    BSF T1CON,5
    BSF T1CON,4

    BCF T1CON,1
    BSF T1CON,0
    BANKSEL PORTA
    REINICIAR_TMR1
    ENDM

    REINICIAR_TMR0 MACRO
    BANKSEL TMR0
    MOVLW 240
    MOVWF TMR0
    BCF T0IF; Limpiamos bandera de Timer0
    BANKSEL PORTA
    ENDM

    REINICIAR_TMR1 MACRO
    BANKSEL TMR1L

    MOVLW 0X37

    MOVWF TMR1L
    BANKSEL TMR1H
    MOVLW 0XFC

    MOVWF TMR1H
    BANKSEL PIR1
    BCF PIR1,0; TMR1IF Limpiamos bandera de Timer1
    BANKSEL PORTA
    ENDM
