/*
 * File:   main.c
 * Author: dasNeves
 *
 * Desenvolver um cronometro digital, con?gurar o DsPic para acionar um display de 7
 *  segmentos para realizar a contagem de 0 a 9. Depois adicionar um botão para 
 * introduzir a função PAUSE/ 
 * Fosc = FRC 
 * Período do Timer 1 = 1s 
 * MPLAB (XC16) e PROTEUS 
 * 
 * Created on 28 June 2019, 23:41
 */

#include "xc.h"
_FOSCSEL(FNOSC_FRC ) //7.37MHz
_FOSC(POSCMD_XT & OSCIOFNC_OFF)
        
int a = 0;        
int tab[10] = {  // Tabela com os valores de saida
    0b00111111,  // Onde o display 7 segmentos foi
    0b00000110,  // conectado. Vai de 0 a 9.
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111100,
    0b00000111,
    0b01111111,
    0b01100111  
};

void config_cn(){
    CNEN1bits.CN0IE = 1;  // Ativa CN no pino CN0
    IFS1bits.CNIF = 0; //LIMPANDO FLAG DE INTERRUPCAO
    IEC1bits.CNIE= 1; //HABILITANDO A INTERUPCAO DO change notification
}

void config_timer1(){
    T1CON = 0; // DESLIGANDO TUDO POR PRECAUCAO
    T1CONbits.TCKPS = 0b11; //PRESCALE DE 256
    TMR1 = 0; // LIMPANDO O CONTADOR DO TIMER
    PR1 = 19531; // FORMULA NO DATASHEET
    IPC0bits.T1IP = 1; //SETANDO A PRIORIDADE
    IFS0bits.T1IF = 0; //LIMPANDO FLAG DE INTERRUPCAO
    IEC0bits.T1IE = 1; //HABILITANDO A INTERUPCAO DO TIMER
    T1CONbits.TON = 1;//ligando o timer
}

void __attribute__((__interrupt__, __auto_psv__)) _CNInterrupt(void){
    IFS1bits.CNIF= 0; // LIMPANDO A FLAG DA INTERRUPCAO
    T1CONbits.TON = !T1CONbits.TON;
    
} 

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void){
    IFS0bits.T1IF = 0; // LIMPANDO A FLAG DA INTERRUPCAO
    a = a+1;
    if(a==10){
        a=0;
    }
    PORTB = tab[a];
} 

void main(void) {
    TRISB = 0; // SETANDO TODOS OS PINOS DAS PORTA B COMO SAIDA
    config_cn();
    config_timer1();
    PORTB = tab[a];
    
    while(1);
}
