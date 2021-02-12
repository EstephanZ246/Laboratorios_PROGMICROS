


PROCESSOR 16F887
#include <xc.inc>
    
CONFIG FOSC=XT     // Oscilador Interno Sin salidas
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

    

PSECT resVect, class=CODE, abs, delta=2

;_______________Vector Reset_________________
ORG 00h         ;posición 0000h para el Reset
resetVec:; configuramos el vector de reset
    PAGESEL main
    goto main


PSECT code, delta=2, abs
ORG 100h
 ;________________________CONFIGURACION DE REGISTROS____________________________
 main: 
      banksel TRISA
      clrf  TRISA   ; Para poner RA en Salida
      clrf  TRISB   ; Para poner RB en Salida
      clrf  TRISC; Para poner RC en salida
      bcf TRISE,0; LED desborde
      bsf TRISD,0; Sumar contador 1
      bsf TRISD,1; Restar Contador 1
      bsf TRISD,2;Sumar contador 2
      bsf TRISD,3; Restar contador 2
      bsf TRISD,4  ; Hacer Suma
     
      banksel ANSEL
      clrf  ANSEL; poner para salida digital
      clrf  ANSELH; poner para salida digital
      banksel PORTA; Seleccionamos el registro PORTA
      clrf PORTA; Configuramos el PORTA como salida
      clrf PORTB; Configuramos el PORTB como salida
      clrf PORTC; Configuramos el PORTC como salida
      clrf PORTE; Configuramos el PORTE como salida
      
      ;____________________________CICLO INFITO________________________________
      loop:   
    btfsc PORTD,0;Si el bit 0 de PORTD está encendido llamamos al incrementar del contador 1
    call contador11; llamamos subrutina 
    btfsc PORTD,1;Si el bit 1 de PORTD está encendido llamamos al decrementar del contador 1
    call contador12; llamamos subrutina 
    btfsc PORTD,2;Si el bit 2 de PORTD está encendido llamamos al incrementar del contador 2
    call contador21; llamamos subrutina 
    btfsc PORTD,3;Si el bit 3 de PORTD está encendido llamamos al decrementar del contador 2
    call contador22; llamamos subrutina 
    btfsc PORTD,4;Si el bit 4 de PORTD está encendido llamamos a la suma de los contadores
    call hacersuma; llamamos subrutina 
    goto loop; Regresamos al inicio para el loop infinito
    
    ;_______________________________SUBRUTINAS___________________________________
    
    //////////// Configuración para contador 1
    
    contador11:; Subrutina incrementar contador 1
    
    btfss PORTD,0; Comprobar pin está en 1 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    btfsc PORTD,0 ; Comprobar pin está en 0 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    incf PORTA;Incrementar valor de puerto
    btfsc PORTA,4; ; Comprobar pin está en 0 para saltar una instrucción y si está en 1 borra puerto para iniciar de nuevo
    clrf PORTA;limpiar puerto para que no haya desborde
    clrf PORTC;Limpiamos el Resultado
    bcf PORTE,0;Limpiamos el LED de desborde
    return
    
    contador12: ; Resta al contador
    btfss PORTD,1; Comprobar pin está en 1 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    btfsc PORTD,1; Comprobar pin está en 0 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    decf PORTA;Decrementar valor del puerto
    clrf PORTC;Limpiamos el Resultado
    bcf PORTE,0;Limpiamos el LED de desborde
    return
    
        
////////////// configuración para contador 2
  
contador21:
    
    btfss PORTD,2; Comprobar pin está en 1 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    btfsc PORTD,2; Comprobar pin está en 0 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    incf PORTB; Incrementar valor del puerto
    btfsc PORTB,4; Comprobar pin está en 0 para saltar una instrucción, si está en 1 borra puerto
    clrf PORTB; limpiar puerto
    clrf PORTC;Limpiamos el Resultado
    bcf PORTE,0;Limpiamos el LED de desborde
    return
    
contador22:
    
    btfss PORTD,3; Comprobar pin está en 1 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    btfsc PORTD,3; Comprobar pin está en 0 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    decf PORTB; Decrementar valor en puerto
    clrf PORTC;Limpiamos el Resultado
    bcf PORTE,0;Limpiamos el LED de desborde
    return
    
hacersuma:
    //clrf PORTC ; limpiar PORTC para 
    bcf PORTE,0; limpiamos el led de desborde
    btfss PORTD,4; Comprobamos que el bit del puerto esté en 1 para saltar una instrucción
    goto $-1; Mantener en la instrucción anterior
    movf PORTB,W; Movemos el valor del puerto en W
    andlw 00001111B;Nos aceguramos que solo sean 4 bits
    movwf PORTC; Movemos W al PORTC
    movf PORTA,W;Movemos puerto a W
    andlw 00001111B; nos aseguramos que solo sean 4 bits
    addwf PORTC;Sumamos W al puerto de resultado
    btfsc PORTC,4;Para activar el led de desborde 
    bsf PORTE,0; Activamos el Desborde
    return     
END
      
      
      
 