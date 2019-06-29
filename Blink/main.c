/*
 * File:   main.c
 * Author: dasNeves
 *
 * Desenvolver um programa para a familia 33F.
 * No qual utilizaremos o Timer 1 para comandar um LED (pisca-led). 
 * Fosc 10MHz (sem PLL) 
 * Periodo do Timer 1 = 1s 
 * 
 *  MPLAB (XC16) e PROTEUS 
 * 
 * Created on 28 June 2019, 00:11
 */
#include "xc.h"
_FOSCSEL(FNOSC_PRI)
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT)

void config_timer1(){
    T1CON = 0;
    T1CONbits.TON = 0; // DESLIGA PARA CONFIGURAR
    T1CONbits.TCKPS = 0b11;// prescale de valor 1:256
    TMR1 = 0x10; // LIMPANDO O CONTADOR DO TIMER
    PR1 = 19531 ; //Ftimer = FCY/[PRESCALE * PR1]
    IPC0bits.T1IP = 0x01; // setando a prioridade da interrupcao
    IFS0bits.T1IF = 0; // limpando a flag da interrupcao
    IEC0bits.T1IE = 1; // habilitando a interrupcao
    T1CONbits.TON = 1;
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void){
    IFS0bits.T1IF = 0; // limpando a flag da interrupcao
    LATBbits.LATB0 = !LATBbits.LATB0; // mudando o estado do LED
} 

void main(void) {
    config_timer1();
    TRISBbits.TRISB0 = 0; //configura a porta rb0 como saida
    LATBbits.LATB0 = 1;
    
    while(1);
}
