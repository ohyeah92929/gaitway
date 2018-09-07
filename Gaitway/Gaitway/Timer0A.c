// Timer0A.c
// Runs on LM4F120/TM4C123
// Use Timer0A in periodic mode to request interrupts at a particular
// period.
// Daniel Valvano
// September 11, 2013

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers"
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015
   Volume 1, Program 9.8

  "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014
   Volume 2, Program 7.5, example 7.6

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Timer0A.h"
#include "DAC.h"
#include "Music.h"
#include "Switch.h"
int inst_idx1 = 0;
int inst_idx2 = 0;
int inst_idx3 = 0;
int note_idx = 0;
uint32_t period;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void (*PeriodicTask)(void);   // user function




#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))


// ***************** Timer1A_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer1A_Init(void(*task)(void)){
   // long sr;
  //sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  PeriodicTask = task;          // user function
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  //TIMER1_TAILR_R = period-1;    // 4) reload value
   TIMER1_TAILR_R = C2-1;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0xE0000000; // 8) priority 7
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R |= 1<<21;           // 9) enable IRQ 21 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
  //EndCritical(sr);
}

void Timer1A_Handler(void){
    PF2 ^= 1;
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer1A timeout
    if(!save_inds)
    {
        if(song1.notes[note_idx] != 0)
            DAC_Out(song1.wave[inst_idx1]);
    
        inst_idx1++;
        if(inst_idx1 == WAVE_SIZE) inst_idx1 = 0;
    }
    
    PF2 ^= 1;
}



// ***************** Timer2A_Init ****************
// Activate TIMER2 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer2A_Init(void(*task)(void)){
   // long sr;
  //sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate TIMER2
  PeriodicTask = task;          // user function
  TIMER2_CTL_R = 0x00000000;    // 1) disable TIMER2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = EIGHTH-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear TIMER2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R |= 1<<23;           // 9) enable IRQ 21 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable TIMER1A
  //EndCritical(sr);
}

void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer1A timeout

    if(!save_inds){
        TIMER1_TAILR_R = song1.notes[note_idx];
        TIMER3_TAILR_R = song2.notes[note_idx];
        TIMER4_TAILR_R = song3.notes[note_idx];
        
        note_idx ++;
        
        if(note_idx == NOTE_SIZE) note_idx = 0;
    }
}



// ***************** Timer3A_Init ****************
// Activate TIMER3 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer3A_Init(void(*task)(void)){
   // long sr;
  //sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate TIMER3
  PeriodicTask = task;          // user function
  TIMER3_CTL_R = 0x00000000;    // 1) disable TIMER3A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER3_TAILR_R = period-1;    // 4) reload value
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0xC0000000; // 8) priority 6
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN1_R |= 1<<3;           // 9) enable IRQ 35 in NVIC
  TIMER3_CTL_R = 0x00000001;    // 10) enable TIMER3A
  //EndCritical(sr);
}

void Timer3A_Handler(void){
  TIMER3_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer1A timeout

    if(!save_inds)
    {
        if(inst2 == 1)
        {
            if(song2.notes[note_idx] != 0)
            {
                if(inst3 == 1)
                    DAC_Out((song1.wave[inst_idx1] + song2.wave[inst_idx2] + song3.wave[inst_idx3])/3);
                else
                    DAC_Out((song1.wave[inst_idx1] + song2.wave[inst_idx2])/2);
            }
        
            inst_idx2++;
            if(inst_idx2 == WAVE_SIZE) inst_idx2 = 0;
        }
        else
            inst_idx2 = 0;
    }
    
}








// ***************** Timer4A_Init ****************
// Activate TIMER4 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer4A_Init(void(*task)(void)){
   // long sr;
  //sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x10;   // 0) activate TIMER4
  PeriodicTask = task;          // user function
  TIMER4_CTL_R = 0x00000000;    // 1) disable TIMER4A during setup
  TIMER4_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER4_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER4_TAILR_R = period-1;    // 4) reload value
  TIMER4_TAPR_R = 0;            // 5) bus clock resolution
  TIMER4_ICR_R = 0x00000001;    // 6) clear TIMER4A timeout flag
  TIMER4_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI17_R = (NVIC_PRI17_R&0xFF00FFFF)|0x00800000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN2_R |= 1<<6;           // 9) enable IRQ 70 in NVIC
  TIMER4_CTL_R = 0x00000001;    // 10) enable TIMER4A
  //EndCritical(sr);
}

void Timer4A_Handler(void){
  TIMER4_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer1A timeout

    if(!save_inds)
    {
        if(inst3 == 1)
        {
            if(song3.notes[note_idx] != 0)
            {
                if(inst2 == 1)
                    DAC_Out((song1.wave[inst_idx1] + song2.wave[inst_idx2] + song3.wave[inst_idx3])/3);
                else
                    DAC_Out((song1.wave[inst_idx1] + song3.wave[inst_idx3])/2);
            }
        
            inst_idx3++;
            if(inst_idx3 == WAVE_SIZE) inst_idx3 = 0;
        }
        else
            inst_idx3 = 0;
    }
    
}



