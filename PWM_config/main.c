/*
* File: main.c
* Author: dasNeves
* Created on 12 June 2019, 17:06
*/
#include "xc.h"

_FOSCSEL(FNOSC_PRI)
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);

#define FCY 5000000
#define baud 9600
#define BRG ((FCY/baud)/16)-1

void main(void) {
TRISB =0;

PWM1CON1 = 0;

P1TCONbits.PTMOD = 0b00;
P1TCONbits.PTCKPS = 0b00;
P1TCONbits.PTOPS = 0b00;

P1TPER = 999;

P1TMR = 0;

PWM1CON1bits.PMOD1 = 1;
PWM1CON1bits.PMOD2 = 1;
PWM1CON1bits.PMOD3 = 1;


// habilitando pinos para saida pwm

PWM1CON1bits.PEN1H = 1;
PWM1CON1bits.PEN2H = 1;
PWM1CON1bits.PEN3H = 1;

P1DC1 = 2*P1TPER*0.75;
P1DC2 = 2*P1TPER*0.5;
P1DC3 = 2*P1TPER*0.25;

P1TCONbits.PTEN = 1;


while(1);
}