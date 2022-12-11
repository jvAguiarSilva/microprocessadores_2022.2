/* 
 * File:   main.c
 * Author: joaov
 *
 * Created on 1 de Dezembro de 2022, 18:48
 */


// CONFIG1H
#pragma config FOSC = XT_XT     // Oscillator Selection bits (XT oscillator (XT))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

#include "lcd.h"
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>


#define _XTAL_FREQ 4000000


// 1 se o botão foi pressionado 0 se não foi
int mediador = 0;               
int botao_azul = 0;             
int botao_vermelho = 0;        


// Variavel que conta o tempo em Ms dos jogadores correspondetes 
int tempo_azul = 0; 
int tempo_vermelho = 0; 

// Escreve caracteres no lcd com printf()
void putch(char data){
    escreve_lcd(data);   
}  


// Interrupcao para quando os botoes sao apertados
void __interrupt(high_priority) isr() {
    if (INT0IF) { // Trata o evento produzido pelo push button ligado ao pino RB0/INT0
        if((botao_azul==0)&&(botao_vermelho==0)) {
            mediador=1;
            PORTAbits.RA0=1;
            INT1IE=1;// ativa interrupcoes int1
            INT2IE=1;// ativa interrupcoes int2
            tempo_azul = 0;
            tempo_vermelho = 0;
            PORTAbits.RA3=1;
             __delay_ms(500); // liga o buzzer por 0.5 segundos
             PORTAbits.RA3 = 0;
             PORTAbits.RA0 = 0;
        }else if((botao_azul==1)&&(botao_vermelho==1)) {
            mediador = 0;
            botao_azul = 0;
            botao_vermelho = 0;
            PORTAbits.RA1 = 0;
            PORTAbits.RA2 = 0;
            PORTAbits.RA0 = 0;
            limpa_lcd();     
        }
        INT0IF = 0;
        return;
    }
    if (INT1IF) {
        // Atendimento do evento produzido pelo push button ligado ao pino RB1/INT1
        botao_azul = 1;
        INT1IF = 0;
        return;
    }
    if(INT2IF){
        botao_vermelho = 1;
        INT2IF = 0;
        return;
    }
    return;
}

void main(void) {
    // Entradas
    TRISBbits.TRISB0 = 1;     // RD0: mediador
    TRISBbits.TRISB1 = 1;     // RD1: azul
	TRISBbits.TRISB2 = 1;     // RD2: vermelho
    
    
    // Saídas
    TRISAbits.RA0 = 0 ; // RA0: led mediador 
	TRISAbits.RA1 = 0 ; // RA1: led Azul
	TRISAbits.RA2 = 0 ; // RA2: led Vermelho
    TRISAbits.RA3 = 0 ; // RA3: BUZZER
    
    PORTD = 0;          // Definida como saída para o LCD
    TRISD = 0x00;
    ADCON1 = 0x0F;

    T2CONbits.T2CKPS1 = 0; // Pre scaler 1:4
    T2CONbits.T2CKPS0 = 1; // Pre scaler 1:4
    PR2 = 250;             // 250 Para estourar a cada 1 ms T2OUTPS3 a T2OUTPS0 controlam quantos estouros de timer2 precisam pra levar TMR2IF pra 1     
    T2CONbits.T2OUTPS3 = 0;
    T2CONbits.T2OUTPS2 = 0;
    T2CONbits.T2OUTPS1 = 0;
    T2CONbits.T2OUTPS0 = 1;
    INT0IE = 1;            // habilita interrupição int0
    
    PEIE = 1; // habilita interrupcoes dos perifericos
    GIE = 1;  // habilita interrupcoes globalmente
    TMR2=0;
      
    T2CONbits.TMR2ON = 1; // Coloca timer pra contar
    int printa_na_tela = 0; // controla quando a tela pode ser utilizada
    int empate = 0;
    
    while (1){
        while(mediador == 1) {
            if (TMR2IF) {
                TMR2IF = 0;
            if (botao_azul == 0)
                tempo_azul++;
            if (botao_vermelho == 0)
                tempo_vermelho++;
            }
            if((botao_azul == 1) && (botao_vermelho == 1)) {
                mediador=0;
                if(tempo_azul<tempo_vermelho){
                    PORTAbits.RA1 = 1;
                }else if(tempo_azul > tempo_vermelho){
                    PORTAbits.RA2 = 1;
                } else if (tempo_azul == tempo_vermelho){
                    PORTAbits.RA0 = 1;
                    empate = 1;
                }
                printa_na_tela = 1;
            }
        }
        
        if(printa_na_tela){ 
            if (empate) {
               inicializa_lcd();
               limpa_lcd();
               caracter_inicio(1,1);  // primeira linha   
               printf("Empate = %d ms",tempo_azul);
               empate = 0;
            } else { 
                inicializa_lcd();
                limpa_lcd();
                caracter_inicio(1,1);  // primeira linha   
                printf("Blue = %d ms",tempo_azul); 
                caracter_inicio(2,1);  // segunda linha   
                printf("Red  = %d ms",tempo_vermelho);
            }
            printa_na_tela = 0;
        }
    }
}
