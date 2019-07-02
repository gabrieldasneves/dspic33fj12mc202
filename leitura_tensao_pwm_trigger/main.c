/*
 * File:   main.c
 * Author: dasNeves
 *
 * Programar o DsPic para ler uma tensão em um canal AD e apresentar o valor no
 * Terminal Virtual. Utilizar o Special event do PWM como Gatilho da conversão. 
 *  
 * Created on 01 July 2019, 15:22
 */

#include "xc.h"

_FOSCSEL(FNOSC_PRI)
_FOSC(POSCMD_HS)

#define fcy 5000000
#define baud 9600
#define brgval ((fcy/baud)/16)-1

int leitura,x,value;
float res;
float voltagem;
int dez, c1, c2, aux1;

void __attribute__((interrupt, shadow, no_auto_psv)) _U1TXInterrupt(void){
    IFS0bits.U1TXIF = 0;
    while(!U1STAbits.TRMT){//Enquanto o buffer de transmissão estiver cheio não faça nad
        Nop();
    }  
}


void main(void){
    TRISA = 1;// setando todo mundo como entrada
    TRISB = 0;// setando todo mundo como saida
    RPOR0bits.RP0R = 3; // saida uart
    
    P1TCON = 0;// ZERANDO TUDO ANTES DE COMEÇAR
    PWM1CON1 = 0;// AQUI TAMBEM
    P1TCONbits.PTCKPS = 0b00; // PRESCALE DE 1:1
    P1TCONbits.PTMOD = 0b00; // MODO FREE (VER DATA SHEET))
    PTMR = 0; // ZERANDO O CONTADOR
    PTPER = 1999; // PERIODO CALCULADO POR Fpwm = (((Fcy/PTCKPS)/PTPER) - 1)
    P1SECMPbits.SEVTDIR = 1;// SELECIONANDO O SPECIAL EVENT
    PWM1CON1bits.PMOD1 = 1; // selecionando o modo INDEPENDENTE (VER DATASHET))
    PWM1CON1bits.PEN1H = 1; // LIGANDO O PINO DO PWM PARA A SAIDA DO SINAL (ABRINDO O PINO))
    P1DC1 = 2*P1TPER*(0.50); //CALCULO DO DUTY CYCLE
    P1TCONbits.PTEN = 1; // HABILITANDO O PWM
    
    
    AD1PCFGL = 0xFFFE; // todas as portas B sao digitais 
    AD1CON1 = 0; //
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CHS0 = 0;   
    AD1CON1bits.ASAM = 0;
    AD1CON1bits.SSRC = 0b011; // pwm 1
    AD1CON1bits.FORM = 0;
    AD1CON3bits.ADCS = 0b000010;
    AD1CON1bits.ADON = 1; //LIGA O adc
    
    U1MODE = 0;
    U1STA = 0;    
    U1STAbits.UTXISEL0 = 1; //Define a interrupção para quando todas as operações de transmissão tiverem completas    
    U1BRG = brgval;    
    IFS0bits.U1TXIF = 0;
    IEC0bits.U1TXIE = 1;
    IPC3bits.U1TXIP = 4;    
    U1MODEbits.UARTEN = 1;//Liga UART
    U1STAbits.UTXEN = 1;//Liga módulo TX
    
    
    
    while(1){
       
        res  = 0.0049; // 5/1023 resolucao do adc
        leitura = ADC1BUF0;
        voltagem = (leitura*res); // formula para voltagem
    
        
        aux1 = voltagem * 100;
        dez = (int)voltagem;
        c1 = (aux1 - (dez*100))/10;
        c2 = (aux1 - (dez*100))-(c1*10);
    
        U1TXREG = dez + '0';//Trasmissão do byte 1
        U1TXREG = '.';//Trasmissão do byte 2
        U1TXREG = c1 +'0';//Trasmissão do byte 3
        U1TXREG = c2 +'0';//Trasmissão do byte 4
        U1TXREG = 13;//quebra de linha
        
    }
}