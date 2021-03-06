/*
 * File:   interrrupts.c
 * Author: lloyd20001
 *
 * Created on March 3, 2017, 4:21 PM
 */
#pragma config FEXTOSC = OFF    // FEXTOSC External Oscillator mode Selection bits->Oscillator not enabled
#pragma config RSTOSC = HFINT1    // Power-up default value for COSC bits->HFINTOSC
#pragma config CLKOUTEN = OFF    // Clock Out Enable bit->CLKOUT function is disabled; I/O or oscillator function on OSC2
#pragma config CSWEN = ON    // Clock Switch Enable bit->Writing to NOSC and NDIV is allowed
#pragma config FCMEN = ON    // Fail-Safe Clock Monitor Enable->Fail-Safe Clock Monitor is enabled

// CONFIG2
#pragma config MCLRE = ON    // Master Clear Enable bit->MCLR/VPP pin function is MCLR; Weak pull-up enabled
#pragma config PWRTE = OFF    // Power-up Timer Enable bit->PWRT disabled
#pragma config WDTE = OFF    // Watchdog Timer Enable bits->WDT disabled; SWDTEN is ignored
#pragma config LPBOREN = OFF    // Low-power BOR enable bit->ULPBOR disabled
#pragma config BOREN = ON    // Brown-out Reset Enable bits->Brown-out Reset enabled, SBOREN bit ignored
#pragma config BORV = LOW    // Brown-out Reset Voltage selection bit->Brown-out voltage (Vbor) set to 2.45V
#pragma config PPS1WAY = ON    // PPSLOCK bit One-Way Set Enable bit->The PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable bit->Stack Overflow or Underflow will cause a Reset
#pragma config DEBUG = OFF    // Debugger enable bit->Background debugger disabled

// CONFIG3
#pragma config WRT = OFF    // User NVM self-write protection bits->Write protection off
#pragma config LVP = ON    // Low Voltage Programming Enable bit->Low Voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored.

// CONFIG4
#pragma config CP = OFF    // User NVM Program Memory Code Protection bit->User NVM code protection disabled
#pragma config CPD = OFF    // Data NVM Memory Code Protection bit->Data NVM code protection disabled

//****************************************************************************
// Includes
//****************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

//****************************************************************************
// Defines
//****************************************************************************
#define _XTAL_FREQ  4000000

//****************************************************************************
// Global Variables
//****************************************************************************
uint16_t count = 0;

//****************************************************************************
// Main
//****************************************************************************
void main(void)
{     
    OSCCON1 = 0x60; // HFINTOSC   
    OSCFRQ = 0x03;  // HFFRQ 4_MHz;
    
 
    
    // TMR0 init
    T0CON0 = 0b10000000;            // Timer0 Control Register 0. Set T0EN to 1.
    T0CON1 = 0b01000110;            // Timer0 Control Register 1. Sets T0CS to 2. And T0CKPS to 1:64
    TMR0H = 155;                    // Timer0 Period Register. Sets period to 10ms. 4/4MHz * 64 *(?+1) = 10ms
   
    // Interrupt init
    //INTCON = 0b11000001;           // Interrupt control register. Sets GIE and PEIE to 1
    TMR0IE = 1;                   // TMR0 Overflow Interrupt Enable bit
    GIE = 1;                       // Turn on global interrupts
    PEIE = 1;                     // Peripheral Interrupt Enable.
    //PIE0 = 0b00100000;           //TMR0IE active
    TRISC = 0b11011111;         //LED on RC5
    
    
    
    //TMR0IE active
    //ADC Config
    ADCON0 = 0b00001001;
    ADCON1 = 0b01010000;
    
    
    
    
    
    //PWM config.
    
    PR2 = 0xff;
    TMR2ON = 1;
    //PWM5DCH = 0;
    //PWM5DCL = 0;
    PWM5CON = 0b10000000;
    
    

    
    RC5PPS = 0b00010;
    PPSLOCK = 1;

    ADGO = 1;
    while (1)
    {
     
    }
}

void interrupt my_isr (void)
{
  if (TMR0IF && TMR0IE) // Source Check for TMR0
{
      TMR0IF = 0;
      PWM5DCH = ADRESH;
      PWM5DCL = ADRESL;
      ADGO = 1;
 }

}
