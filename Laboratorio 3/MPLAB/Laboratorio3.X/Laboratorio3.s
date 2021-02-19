
PROCESSOR 16F887
#include <xc.inc>

CONFIG FOSC=INTRC_NOCLKOUT     // Oscilador Interno Sin salidas
CONFIG WDTE=OFF                 // WDT disabled (reinicio repetitivo del pic)
CONFIG PWRTE=ON                 // PWRT enabled (espera de 72ms al iniciar)
CONFIG MCLRE=OFF                // El pin de MCLR se utiliza como I/0
CONFIG CP=OFF                   // Sin protección de código
CONFIG CPD=OFF                  // Sin protección de datos

CONFIG BOREN=OFF   // Sin reinicio cuándo el voltaje de alimentación baja de 4V
CONFIG IESO=OFF             // Reinicio sin cambio de reloj de interno a externo
CONFIG FCMEN=OFF        // Cambio de reloj de externo a interno en caso de fallo
CONFIG LVP=ON                 // programación en bajo voltaje permitida

;Configuration word 2

CONFIG WRT=OFF      // Protección de autoescritura por el programa desactivada
CONFIG BOR4V=BOR40V         // Reinicio abajo de 4V, (BOR21V=2.1V)

PSECT udata_bank0; common memory
    ContadorDisplay: DS 1 ;1 byte
    cont:   DS 1 // contador para delay pequeño
    cont1:  DS 1 // delay que utiliza el delay pequeño



PSECT resVect, class=CODE, abs, delta=2

;_______________Vector Reset_________________
ORG 00h         ;posición 0000h para el Reset
resetVec:; configuramos el vector de reset
    PAGESEL main
    goto main




PSECT code, delta=2, abs
ORG 100h
 ; Algunas constantes para facilitar código
;______________________________ Tabla para display 7 segmentos_______________
 Tablas:
    clrf PCLATH
    bsf PCLATH,0
    addwf PCL ; PC+PCLATH + PCL + W
    
    NOP // No hace nada, solo llena un espacio en la memoria 
    ;     abcdefg      del display 7 segmentos
    retlw 0110000B;1
    retlw 1101101B;2
    retlw 1111001B;3
    retlw 0110011B;4
    retlw 1011011B;5
    retlw 1011111B;6
    retlw 1110001B;7
    retlw 1111111B;8
    retlw 1110011B;9
    retlw 1110111B;A
    retlw 0011111B;b
    retlw 0001101B;c
    retlw 0111101B;d
    retlw 1001111B;E
    retlw 1000111B;F
   

    ;______________________ Configuraciones de puertos___________________

configurar_IO:
     banksel TRISA
    bsf TRISA,0;Boton que aumenta el valor del display
    bsf TRISA,1;Boton decrementa el valor del display
    clrf TRISC;Puerto para Display
    clrf TRISD; Display 7 segmentos
    clrf TRISE; LED alarma

    banksel ANSEL
    clrf ANSEL;Puertos digitales
    clrf ANSELH;Puertos digitales

    banksel PORTA
    clrf PORTA  /// Limpiamos Puertos
    clrf PORTC	/// Limpiamos Puertos
    clrf PORTD	/// Limpiamos Puertos
    clrf PORTE	/// Limpiamos Puertos
    clrf ContadorDisplay// limpiamos Display
    return

    ;____________________ Configuramos velocidad del oscilador interno_________
    configurar_reloj:

    banksel OSCCON// Seleccionamos el banco OSCCON 
    bcf OSCCON,6// IRCF2
    bcf OSCCON,5// IRCF1 Configuramos la velocidad a 125KHz
    bsf OSCCON,4// IRCF0
    bsf SCS//OSCCON,0 // SCS // Colocamos el oscilador interno como reloj del sistema
    banksel PORTA // Vamos a banco 0
   return
; ___________________________ Configuramos el prescaler_______________________
    configurar_tmr0:

    banksel TRISA // Vamos a banco 1
    bcf T0CS // Configuramos (Fosc/4)
    bcf PSA// PSA asignamos el prescaler al Timer0
    bsf PS2//
    bsf PS1// configuramos el prescaler a 1:128
    bcf PS0//
    banksel PORTA// Vamos al banco 0
    call reiniciar_tmr0 // Reiniciamos el timer0 con la subrutina
   return

    reiniciar_tmr0:
    movlw 134 // Colocamos valor obtenido con formula para tener 500ms => 500ms=4*(1/125000)*(256-TMR0)*(128)
    movwf TMR0 // movemos el valor a timer0
    bcf T0IF; Limpiamos bandera de Timer0 
    return

main:
   call configurar_IO// Llamamos subrutina de configuración de puertos
   call configurar_reloj// llamamos subrutina de config del reloj
   call configurar_tmr0// Llamamos subrutina de conf del Timer0 
   banksel PORTA// Vamor a banco 0 como precaución


loop:
    
    bcf PORTE,0// limpiarmos alarma
    bcf STATUS,2 // reiniciamos valor del status Z
    btfsc PORTA,0;Si el bit 0 de PORTA está encendido llamamos al aumentar contador (Display)
    call aumentar_contador; llamamos subrutina
    btfsc PORTA,1;Si el bit 1 de PORTA está encendido llamamos al decrementar contador (Display)
    call decrementar_contador// llamamos subrutina
   btfsc T0IF // Verificamos la bandera y saltamos si está en 0
   call contador_binario  // llamamos la subrutina de contador binario
   goto loop
 


 aumentar_contador:

    btfsc PORTA,0; Comprobar pin está en 1 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    btfsc PORTA,0 ; Comprobar pin está en 0 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    incf ContadorDisplay; Incrementar el valor de nuestra variable para contador 7 segmentos
    movlw 00001111B; apicar AND para estar seguros que solo tenemos 4 bits menos significativos
    andwf ContadorDisplay; apicar AND para estar seguros que solo tenemos 4 bits menos significativos
    movf ContadorDisplay,W; mover el valor de la variable a nuestro registro de trabajo
    call Tablas; Buscar el valor que tiene el ContadorDisplay en nuestra tabla
    movwf PORTD // Pasamos el valor de la tabla al display
    bcf PORTE,0// Limpiamos la alarma
    return


 decrementar_contador:

    btfss PORTA,1; Comprobar pin está en 1 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    btfsc PORTA,1; Comprobar pin está en 0 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    decf ContadorDisplay;Decrementar valor del puerto
    movlw 00001111B// aplicar AND para obtener solo nible menos significativo
    andwf ContadorDisplay// aplicar AND para obtener solo nible menos significativo
    movf ContadorDisplay,W// mover valor al registro de trabajo
    call Tablas // Buscar valor en las tablas
    movwf PORTD// aplicar valor al display
    bcf PORTE,0// limpiamos alarma

    return

    contador_binario:
    call reiniciar_tmr0 // reiniciamos el timer 0
    incf PORTC// incrementamos el valor del contador binario
    movlw 00001111B// aplicamos AND para solo tener nible menos significativo
    andwf PORTC// Aplicamos AND para solo tener nible menos significativo
    
   
    /// verificar valor
    
    movf PORTC,W // Pasamos el valor del contador al registro de trabajo
    subwf ContadorDisplay,W // Restamos el valor de puerto c al valor de display y lo guardamos en el registro de trabajo
    btfsc STATUS,2 /// Comprobamos si la operacion de resta fue 0
    call alarma// Si el resultado es igual a 0 entonces el valor de Z es 1 y por lo tanto llamamos a la alarma
    return

    alarma:
    
    clrf PORTC // limpiamos contador binario
    bsf PORTE,0 // Limpiamos bandera
    movlw 25 // tenemos un pequeño delay para poder visualizar la alarma
    movwf cont1 // movemos el valor anterior al contador 1 extra para delay
    call small_delay // llamamos subrutina del delay pequeño
    decfsz cont1 // decrementamos el valor del contador extra 1 
    goto $-2 // regresamos 2 intrucciones
    
    return
    
    small_delay: // delay pequeño parte del delay de la alarma
    movlw 127 // delay para poder visualizar la alarma
    movwf cont// movemos el valor al cont extra para delay
    decfsz cont,f // decramentamos valor del cont extra, delay
    goto $-1// regresamos a la instruccion anterior
    return
 END


