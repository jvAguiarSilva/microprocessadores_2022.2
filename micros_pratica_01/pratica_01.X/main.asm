#include "p18f4550.inc"
    
CONFIG FOSC = XT_XT
CONFIG WDT = OFF
CONFIG LVP = OFF

VARIAVEIS UDATA_ACS 0 ; 
    CONT RES 1
    KEY1 EQU .0 ; PINO RC0
    KEY2 EQU .1 ; PINO RC1
    CONTA2m   RES 1 ; Variavel auxiliar para contagem de 2 ms
    CONTA500m RES 1 ; Variavel auxiliar para contagem de 500 ms
    
RES_VECT  CODE    0x0000            ; processor reset vector
    GOTO    START                   ; go to beginning of program

; TODO ADD INTERRUPTS HERE IF USED

MAIN_PROG CODE                      ; let linker place main program

START
    CLRF CONT		;CONT = 0
    
    CLRF TRISD		;PORTD COMO SAÍDA
    BSF TRISC, KEY1	;KEY1 ENTRADA
    BSF TRISC, KEY2	;KEY2 ENTRADA
    CLRF PORTD		;ZERANDO PORTD
   
LOOP
    
    BTFSC PORTC, KEY1 
    BRA LOOP_A                     
    BRA LOOP_B				   
    ; LOOP_A: CHAVE 1 ATIVADA 
    ; LOOP_B: CHAVE 1 DESATIVADA
    
    ; LOOP_C: CHAVE 1 ATIVADA E CHAVE 2 ATIVADA, NÃO FAZ NADA
    ; LOOP_D: CHAVE 1 ATIVADA E CHAVE 2 DESATIVADA, INCREMENTO
    
    ; LOOP_E: CHAVE 1 DESATIVADA E CHAVE 2 ATIVADA, DECREMENTA
    ; LOOP_F: CHAVE 1 DESATIVADA E CHAVE 2 DESATIVADA, NÃO FAZ NADA
    
    LOOP_A 
	BTFSC PORTC, KEY2  
	BRA LOOP_C	
	BRA LOOP_D	
	
    LOOP_B	
	BTFSC PORTC, KEY2   ;V
	BRA LOOP_E	
	BRA LOOP_F	
    
    LOOP_C
	GOTO LOOP	

    LOOP_D
	CALL INCREMENT	
	GOTO LOOP	
	
    LOOP_E
	CALL DECREMENT	
	GOTO LOOP	

    LOOP_F
	GOTO LOOP
	
INCREMENT
    CALL ATRASO_500ms
    CALL ATRASO_500ms
    BTFSC PORTC, KEY1
    INCF CONT
    MOVFF CONT, PORTD  
    RETURN
    
DECREMENT
    CALL ATRASO_500ms
    CALL ATRASO_500ms
    BTFSC PORTC, KEY2
    DECF CONT
    MOVFF CONT, PORTD
    RETURN
    
ATRASO_2ms ; Subrotina que gera atraso de 2 ms
    MOVLW .152
    MOVWF CONTA2m ; Carrega contador para 200 interações
REPETE2      ; Inicio do loop    
    NOP      ; 10 NOPs -> 10 us
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    DECFSZ CONTA2m
    GOTO REPETE2  ; Repete interações até que CONTA2 = 0
    RETURN

ATRASO_500ms ; Subrotina que gera atraso de 500 ms
    MOVLW .250
    MOVWF CONTA500m ; Carrega contador para 250 interações
REPETE_500   ; Inicio d92o loop
    CALL ATRASO_2ms ; Espera 2 ms
    DECFSZ CONTA500m
    GOTO REPETE_500 ; Repete interações até que CONTA500 = 0
    RETURN
 
    
   END