# 1 "MACROS_RUTINAS.s"
# 1 "<built-in>" 1
# 1 "MACROS_RUTINAS.s" 2

CAMBIO_DE_MODOS MACRO
    CAMBIO1:
    MOVLW 9
    SUBWF PORTE,W
    BTFSS STATUS,Z
    GOTO CAMBIO2
    MOVLW 00000001B
    MOVWF CAMBIOMODO
    CAMBIO2:
    MOVLW 10
    SUBWF PORTE,W
    BTFSS STATUS,Z
    GOTO CAMBIO3
    MOVLW 00000010B
    MOVWF CAMBIOMODO
    CAMBIO3:
    MOVLW 11
    SUBWF PORTE,W
    BTFSS STATUS,Z
    GOTO CAMBIO4
    MOVLW 00000100B
    MOVWF CAMBIOMODO
    CAMBIO4:
    MOVLW 12
    SUBWF PORTE,W
    BTFSS STATUS,Z
    GOTO CAMBIO5
    MOVLW 00001000B
    MOVWF CAMBIOMODO
    CAMBIO5:
    MOVLW 13
    SUBWF PORTE,W
    BTFSS STATUS,Z
    GOTO VALOR_INICIAL
    MOVLW 00010000B
    MOVWF CAMBIOMODO
    VALOR_INICIAL:
    MOVLW 14
    SUBWF PORTE,W
    BTFSS STATUS,Z
    GOTO EJECUCION_DE_LOS_MODOS
    MOVLW 0X01
    MOVWF PORTE

 ENDM

MODO_NORMAL MACRO
D1V1:
    BTFSS PORTA,0
    GOTO D2V1
    MOVF DECENA_TEMP,W
    CALL TABLAS
    MOVWF PORTC


D2V1:
    BTFSS PORTA,1
    GOTO D1V2
    MOVF UNIDAD_TEMP,W
    CALL TABLAS
    MOVWF PORTC

D1V2:
    BTFSS PORTA,2
    GOTO D2V2
    MOVF DECENA_TEMP,W
    CALL TABLAS
    MOVWF PORTC

D2V2:
    BTFSS PORTA,3
    GOTO D1V3
    MOVF UNIDAD_TEMP,W
    CALL TABLAS
    MOVWF PORTC

D1V3:
    BTFSS PORTA,4
    GOTO D2V3
    MOVF DECENA_TEMP,W
    CALL TABLAS
    MOVWF PORTC

D2V3:
    BTFSS PORTA,5
    GOTO D1C
    MOVF UNIDAD_TEMP,W
    CALL TABLAS
    MOVWF PORTC


D1C:
    BTFSS PORTA,6
    GOTO D2C
    MOVF CONTADOR_CONFIG_DECENA,W
    CALL TABLAS
    MOVWF PORTC

D2C:
    BTFSS PORTA,7
    GOTO LOOP
    MOVF CONTADOR_CONFIG_UNIDAD,W
    CALL TABLAS
    MOVWF PORTC

ENDM

SEMAFOROS MACRO
 GOTO CONTADOR_PRINCIPAL_VIA1

VIA1:
    CONTADOR_PRINCIPAL_VIA1:
    MOVLW 0
    SUBWF TEMP_TIEMPOVIA1,W
    BTFSC STATUS,Z
    GOTO OPERACION1_VIA1

    MOVLW 0
    SUBWF CONTADOR_VIAS,W
    BTFSS STATUS,Z
    GOTO $+4
    MOVF TEMP_TIEMPOVIA1,W
    MOVWF CONTADOR_VIAS

    CLRF TEMP_TIEMPOVIA1

    OPERACION1_VIA1:
    MOVLW 0
    SUBWF TEMP_VERDEVIA1,W
    BTFSC STATUS,Z
    GOTO OPERACION2_VIA1
    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO $+4
    MOVF TEMP_VERDEVIA1,W
    MOVWF CONTADOR
    CLRF TEMP_VERDEVIA1
    MOVLW 10001001B
    MOVWF PORTD
    BCF PORTB,7
    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO LOOP
    CLRF PORTD
    GOTO OPERACION2_VIA1

    OPERACION2_VIA1:
    MOVLW 0
    SUBWF TEMP_VERDETITILANTEVIA1,W
    BTFSC STATUS,Z
    GOTO OPERACION3_VIA1
    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO $+4
    MOVF TEMP_VERDETITILANTEVIA1,W
    MOVWF CONTADOR
    CLRF TEMP_VERDETITILANTEVIA1

    MOVLW 01001100B
    MOVWF PORTD
    BCF PORTB,7

    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO LOOP
    CLRF PORTD
    GOTO OPERACION3_VIA1

    OPERACION3_VIA1:
    MOVLW 0
    SUBWF TEMP_AMARILLOVIA1,W
    BTFSC STATUS,Z
    GOTO VIA2
    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO $+4
    MOVF TEMP_AMARILLOVIA1,W
    MOVWF CONTADOR
    CLRF TEMP_AMARILLOVIA1

    BTFSS TITILANTE,0
    GOTO $+3
    BCF PORTD,2
    GOTO $+2
    BSF PORTD,2

    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO LOOP
    CLRF PORTD
    GOTO VIA2

VIA2:
    CONTADOR_PRINCIPAL_VIA2:
    MOVLW 0
    SUBWF TEMP_TIEMPOVIA2,W
    BTFSC STATUS,Z
    GOTO OPERACION1_VIA2

    MOVLW 0
    SUBWF CONTADOR_VIAS,W
    BTFSS STATUS,Z
    GOTO $+4
    MOVF TEMP_TIEMPOVIA2,W
    MOVWF CONTADOR_VIAS

    CLRF TEMP_TIEMPOVIA2

    OPERACION1_VIA2:
    MOVLW 0
    SUBWF TEMP_VERDEVIA2,W
    BTFSC STATUS,Z
    GOTO OPERACION2_VIA2
    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO $+4
    MOVF TEMP_VERDEVIA2,W
    MOVWF CONTADOR
    CLRF TEMP_VERDEVIA2
    MOVLW 01001010B
    MOVWF PORTD
    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO LOOP
    CLRF PORTD
    GOTO OPERACION2_VIA2

    OPERACION2_VIA2:

    MOVLW 0
    SUBWF TEMP_VERDETITILANTEVIA2,W
    BTFSC STATUS,Z
    GOTO OPERACION3_VIA2
    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO $+4
    MOVF TEMP_VERDETITILANTEVIA2,W
    MOVWF CONTADOR
    CLRF TEMP_VERDETITILANTEVIA2

    MOVLW 01100001B
    MOVWF PORTD

    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO LOOP
    CLRF PORTD
    GOTO OPERACION3_VIA2

    OPERACION3_VIA2:

    MOVLW 0
    SUBWF TEMP_AMARILLOVIA2,W
    BTFSC STATUS,Z
    GOTO VIA3
    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO $+4
    MOVF TEMP_AMARILLOVIA2,W
    MOVWF CONTADOR
    CLRF TEMP_AMARILLOVIA2

    BTFSS TITILANTE,0
    GOTO $+3
    BCF PORTD,5
    GOTO $+2
    BSF PORTD,5

    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO LOOP
    CLRF PORTD
    GOTO VIA3


VIA3:
    CONTADOR_PRINCIPAL_VIA3:
    MOVLW 0
    SUBWF TEMP_TIEMPOVIA3,W
    BTFSC STATUS,Z
    GOTO OPERACION1_VIA3

    MOVLW 0
    SUBWF CONTADOR_VIAS,W
    BTFSS STATUS,Z
    GOTO $+4
    MOVF TEMP_TIEMPOVIA3,W
    MOVWF CONTADOR_VIAS

    CLRF TEMP_TIEMPOVIA3

    OPERACION1_VIA3:

    MOVLW 0
    SUBWF TEMP_VERDEVIA3,W
    BTFSC STATUS,Z
    GOTO OPERACION2_VIA3
    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO $+4
    MOVF TEMP_VERDEVIA3,W
    MOVWF CONTADOR
    CLRF TEMP_VERDEVIA3
    MOVLW 01010001B
    BCF PORTB,7
    MOVWF PORTD
    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO LOOP
    CLRF PORTD
    GOTO OPERACION2_VIA3

    OPERACION2_VIA3:

    MOVLW 0
    SUBWF TEMP_VERDETITILANTEVIA3,W
    BTFSC STATUS,Z
    GOTO OPERACION3_VIA3
    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO $+4
    MOVF TEMP_VERDETITILANTEVIA3,W
    MOVWF CONTADOR
    CLRF TEMP_VERDETITILANTEVIA3

    MOVLW 00001001B
    MOVWF PORTD
    BSF PORTB,7

    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO LOOP
    CLRF PORTD
    GOTO OPERACION3_VIA3

    OPERACION3_VIA3:

    MOVLW 0
    SUBWF TEMP_AMARILLOVIA3,W
    BTFSC STATUS,Z
    GOTO FINALINAR_VIAS
    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO $+4
    MOVF TEMP_AMARILLOVIA3,W
    MOVWF CONTADOR
    CLRF TEMP_AMARILLOVIA3

    BTFSS TITILANTE,0
    GOTO $+3
    BCF PORTB,7
    GOTO $+2
    BSF PORTB,7

    BTFSS PORTD,0
    GOTO $+3
    BCF PORTB,7
    GOTO $+2
    BSF PORTB,7

    MOVLW 0
    SUBWF CONTADOR,W
    BTFSS STATUS,Z
    GOTO LOOP
    CLRF PORTD
    GOTO FINALINAR_VIAS

  FINALINAR_VIAS:
    CALL REINICIO_DE_VALORES

 ENDM

 REINICIAR_TMR0 MACRO
    MOVLW 250
    MOVWF TMR0
    BCF T0IF; Limpiamos bandera de Timer0
    ENDM

    REINICIAR_TMR1 MACRO

    MOVLW 0XF0
    MOVWF TMR1H
    MOVLW 0XBD
    MOVWF TMR1L
    BCF PIR1,0; TMR1IF Limpiamos bandera de Timer1
    ENDM

    RUTINA_BCD MACRO
 CLRF DECENA
 CLRF CENTENA
 CLRF UNIDAD
 MOVF CONTADOR_VIAS,W
 MOVWF UNIDAD

BCD_0:
 MOVLW 0X0A
 SUBWF UNIDAD,W
 BTFSS STATUS,0
 GOTO BCD_Fin
BCD_1:
 MOVWF UNIDAD
 INCF DECENA,F
 MOVLW 0X0A
 SUBWF DECENA,W
 BTFSS STATUS,0
 GOTO BCD_0
BCD_2:
 CLRF DECENA
 INCF CENTENA,F
 GOTO BCD_0
BCD_Fin:

    MOVF UNIDAD,W
    MOVWF UNIDAD_TEMP
    MOVF DECENA,W
    MOVWF DECENA_TEMP

    ENDM

    RUTINA_BCD_CONFIG MACRO
 CLRF DECENA
 CLRF CENTENA
 CLRF UNIDAD
 MOVF CONTADOR_CONFIG,W
 MOVWF UNIDAD

BCD1_0:
 MOVLW 0X0A
 SUBWF UNIDAD,W
 BTFSS STATUS,0
 GOTO BCD1_Fin
BCD1_1:
 MOVWF UNIDAD
 INCF DECENA,F
 MOVLW 0X0A
 SUBWF DECENA,W
 BTFSS STATUS,0
 GOTO BCD1_0
BCD1_2:
 CLRF DECENA
 INCF CENTENA,F
 GOTO BCD1_0
BCD1_Fin:

    MOVF UNIDAD,W
    MOVWF CONTADOR_CONFIG_UNIDAD
    MOVF DECENA,W
    MOVWF CONTADOR_CONFIG_DECENA

    ENDM

    MODO_NORMAL_SEMAFOROS MACRO
    SEMAFOROS
ENDM