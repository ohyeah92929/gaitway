// Lab 5 - Music player and Audio amp
// Hyejun Im (hi956), Sarthak Gupta(sg38368)
// TA: William Glanton


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
extern int save_inds;
extern int inst2;
extern int inst3;


// ***************** Switch_Init ****************
// Initialize switch interface on PB4 and PC6
// Inputs:  pointer to a function to call on touch (falling edge),
//          pointer to a function to call on release (rising edge)
// Outputs: none 
void Switch_Init(void);

// ***************** Switch_WaitPress ****************
// Wait for switch to be pressed 
// There will be minimum time delay from touch to when this function returns
// Inputs:  none
// Outputs: none 
void Switch_WaitPress(void);

// ***************** Switch_WaitRelease ****************
// Wait for switch to be released 
// There will be minimum time delay from release to when this function returns
// Inputs:  none
// Outputs: none 
void Switch_WaitRelease(void);

// ***************** Switch_Input ****************
// Return current value of the switch 
// Repeated calls to this function may bounce during touch and release events
// If you need to wait for the switch, use WaitPress or WaitRelease
// Inputs:  none
// Outputs: false if switch currently pressed, true if released 
unsigned long Switch_Input(void);

