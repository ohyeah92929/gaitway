// EE445L Project : GaitWay
// Hyejun Im (hi956), Sarthak Gupta(sg38368)
// TA: William Glanton


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>


#define ARROW_X 5
#define TEXT_X 10
#define TITLE_Y 20
#define ONEFOOT_Y 80
#define WALKING_Y 120


extern int arrow_pos;


// ***************** Screen_MoveArrow ****************
// Move arrow to select between two modes.
// Used to show which menu will be selected.
// Input: color 	16-bit color, which can be produced by ST7735_Color565()
// Output: none
void Screen_MoveArrow(int16_t color, int size);

// ***************** Screen_Init ****************
// Initialize screen
// Inputs:  none
// Outputs: none
void Screen_Init(void);

// ***************** Screen_DrawGreeting ****************
// Draw a greeting screen
// Inputs:  none
// Outputs: none
void Screen_DrawGreeting(void);


// ***************** Screen_DrawMain ****************
// Draw a main screen
// Inputs:  none
// Outputs: none
void Screen_DrawMain(void);


// ***************** Screen_DrawCalibrationPrep ****************
// Draw a preparation message before calibrating
// Inputs:  none
// Outputs: none
void Screen_DrawCalibrationPrep(void);


// ***************** Screen_DrawCalibration ****************
// Draw a screen while calibrating
// Inputs:  none
// Outputs: none
void Screen_DrawCalibration(void);


// ***************** Screen_DrawTest ****************
// Draw a screen while testing
// Inputs:  none
// Outputs: none
void Screen_DrawTest(void);


// ***************** Screen_GPTest ****************
// Draw a screen to test for general purposes (For debug)
// Inputs:  none
// Outputs: none
void Screen_GPTest(int button, int fsr_num, int fsr_val);


