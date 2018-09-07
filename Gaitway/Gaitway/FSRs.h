// EE445L Project : GaitWay
// Hyejun Im (hi956), Sarthak Gupta(sg38368)
// TA: William Glanton


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>


#define MODE_MAIN 0
#define MODE_CALIBPREP 1
#define MODE_CALIB 2
#define MODE_TEST 3

extern int test_mode;
extern int fsrError[4];
// ***************** Timer3A_Init ****************
// Activate TIMER3 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer3A_Init(void);

// ***************** Timer4A_Init ****************
// Activate TIMER4 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer4A_Init(void);


// ***************** FSR_Init ****************
// Initialize everything regarding with FSRs(Timer, ADC etc)
// Inputs:  none
// Outputs: none
void FSR_Init(void);

// ***************** FSR_Calibration ****************
// Do everything about Calibration function
// Set timer, measure boundaries and then stop the timer.
// Inputs:  none
// Outputs: none
void FSR_Calibration(void);

// ***************** FSR_Test ****************
// Do everything about Testing function
// Check voltage every time period, detect error type and make actions
// Inputs:  none
// Outputs: none
void FSR_Test(void);

// ***************** ADC0_InSeq3 ****************
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t FSR_InSeq3(void);
    
