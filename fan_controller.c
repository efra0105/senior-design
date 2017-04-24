/*
 * File:   fan_controller.c
 * Author: Efrain Magallon
 *Red Id : 814172067
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

int ADC_Val;

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
    INTCON = 0b11000001;           // Interrupt control register. Sets GIE and PEIE to 1
    PIE0 = 0b00100000;           //TMR0IE active
    TRISC = 0b11011111;         //LED on RC5
    //TMR0IE active
    //ADC Config
    ADCON0 = 0b00010101;
    ADCON1 = 0b01010000; 
      //PWM config.
    PR2 = 0xff;
    TMR2ON = 1;//timer 2 on
    PWM5CON = 0b10000000;//PWM control
    
    RC5PPS = 0b00010;//
    PPSLOCK = 1;

    ADGO = 1;
    
    PWM5DCH = 255;
    __delay_ms(3000);
	
    while (1)
    {
        if(ADC_Val < 44) PWM5DCH = 89; // idle speed 25% duty cycle @80F
	       else if (ADC_Val < 55) PWM5DCH = 100; //30%
           else if (ADC_Val < 67) PWM5DCH = 110; //34%
           else if (ADC_Val < 79) PWM5DCH = 120; //39%
           else if (ADC_Val < 90) PWM5DCH = 131; //44%
           else if (ADC_Val < 102) PWM5DCH = 141; //48%
           else if (ADC_Val < 114) PWM5DCH = 151; //53%
           else if (ADC_Val < 125) PWM5DCH = 162; //58%
           else if (ADC_Val < 137) PWM5DCH = 172; //62%
           else if (ADC_Val < 149) PWM5DCH = 182; //67%
           else if (ADC_Val < 161) PWM5DCH = 193; //72%
           else if (ADC_Val < 172) PWM5DCH = 203; //77%
           else if (ADC_Val < 184) PWM5DCH = 214; //81%
           else if (ADC_Val < 197) PWM5DCH = 224; //86%
           else if (ADC_Val < 207) PWM5DCH = 234; //91%
	       else if (ADC_Val < 221) PWM5DCH = 245; //95%
	       else if (ADC_Val < 231) PWM5DCH = 255; //100% @124.2F
           else PWM5DCH = 255; //else burst mode
           
        /*
         if(ADC_Val < 44) PWM5DCH = 89; // idle speed 35% duty cycle @80F
           else if (ADC_Val < 53) PWM5DCH = 98;// 38% duty cycle @83F
	       else if (ADC_Val < 55) PWM5DCH = 106; //42%
	       else if (ADC_Val < 65) PWM5DCH = 114; //45% 
	       else if (ADC_Val < 76) PWM5DCH = 122; //48% 
	       else if (ADC_Val < 86) PWM5DCH = 131; //51%
	       else if (ADC_Val < 96) PWM5DCH = 139; //55%
	       else if (ADC_Val < 107) PWM5DCH = 147; //58% 
	       else if (ADC_Val < 117) PWM5DCH = 156; //61% 
	       else if (ADC_Val < 128) PWM5DCH = 164; //64%
	       else if (ADC_Val < 138) PWM5DCH = 172; //68%
	       else if (ADC_Val < 148) PWM5DCH = 180; //71% 
	       else if (ADC_Val < 159) PWM5DCH = 189; //74%
	       else if (ADC_Val < 169) PWM5DCH = 197; //77%
	       else if (ADC_Val < 180) PWM5DCH = 205; //81% 
	       else if (ADC_Val < 193) PWM5DCH = 214; //84%
	       else if (ADC_Val < 197) PWM5DCH = 222; //87%
	       else if (ADC_Val < 200) PWM5DCH = 230; //90%
	       else if (ADC_Val < 203) PWM5DCH = 238; //94%
	       else if (ADC_Val < 206) PWM5DCH = 247; //97%
	       else if (ADC_Val < 210) PWM5DCH = 0xff; //100%
		   
	       else PWM5DCH = 0xff; //else burst mode
           //Burst @ 126.9 degrees

          */
   
      
    }
}

void interrupt my_isr (void)
{
  if (TMR0IF && TMR0IE) // Source Check for TMR0
{
      TMR0IF = 0;
      ADC_Val = ADRESH;
      ADGO = 1;
 }

}
