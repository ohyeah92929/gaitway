// EE445L Project : GaitWay
// Hyejun Im (hi956), Sarthak Gupta(sg38368)
// TA: William Glanton


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"
#include "FSRs.h"
#include "Audio.h"
#include "Screen.h"
#include "Switch.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


// if desired interrupt frequency is f, Timer0A_Init parameter is busfrequency/f
#define F16HZ (50000000/16)
#define F20KHZ (50000000/20000)

//debug code
int main(void){ 
    PLL_Init(Bus80MHz);              // bus clock at 16 MHz
    DisableInterrupts();

    FSR_Init(); // timer3,4
    Audio_Init(); // timer1,2
    Switch_Init(); // PB3, PC6, timer0
    Screen_Init(); // timer5
    EnableInterrupts();

// Temporaty code for debugging


    while(1){
        if(test_mode == MODE_MAIN) ;
        else if(test_mode == MODE_CALIBPREP) ;
        else if(test_mode == MODE_CALIB)
          FSR_Calibration();
        else if(test_mode == MODE_TEST)
          FSR_Test();
    }
}
