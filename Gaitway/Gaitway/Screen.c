// EE445L Project : GaitWay
// Hyejun Im (hi956), Sarthak Gupta(sg38368)
// TA: William Glanton

#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "Screen.h"
#include "ST7735.h"
#include "FSRs.h"

void Timer5A_Init(void);

void Delay1ms(uint32_t n);
void Screen_FSRTest(void);

void Screen_DrawLogoTop(void);
void Screen_DrawArrow(int16_t x, int16_t y, int16_t color, uint8_t size);


int arrow_pos = ONEFOOT_Y;
int calib_sec = 5;

// ***************** Screen_MoveArrow ****************
// Move arrow to select between two modes.
// Used to show which menu will be selected.
// Input: color 	16-bit color, which can be produced by ST7735_Color565()
// Output: none
void Screen_MoveArrow(int16_t color, int size)
{
    Screen_DrawArrow(ARROW_X, arrow_pos, ST7735_BLACK, 3); // remove original arrow
    if (arrow_pos == ONEFOOT_Y)
    {
        arrow_pos = WALKING_Y;
    }
    else if(arrow_pos == WALKING_Y)
    {
        arrow_pos = ONEFOOT_Y;
    }
    
    Screen_DrawArrow(ARROW_X, arrow_pos, color, 3); // draw a new arrow
}

// ***************** Screen_DrawArrow ****************
// Draw an arrow in the chosen position.
// Used to show which menu will be selected.
// Input: x     	horizontal position of the start of the arrow, columns from the left edge
//        y     	vertical position of the start of the arrow, rows from the top edge
//        color 	16-bit color, which can be produced by ST7735_Color565()
//		  thickness	thickness of the arrow
// Output: none
void Screen_DrawArrow(int16_t x, int16_t y, int16_t color, uint8_t size)
{
    ST7735_DrawChar(x , y, '>', color, ST7735_BLACK, size);
}

void Screen_DrawLogoTop(void)
{
    ST7735_FillScreen(ST7735_BLACK);                 // set screen to white
//    ST7735_FillScreen(ST7735_WHITE);                 // set screen to white
    
    // Display ImTa logo lol
    
    ST7735_DrawCharS(74, 1, 'I', ST7735_RED, ST7735_BLACK, 1);
    ST7735_DrawCharS(80, 1, 'M', ST7735_WHITE, ST7735_BLACK, 1);
    ST7735_DrawCharS(86, 1, 'T', ST7735_RED, ST7735_BLACK, 1);
    ST7735_DrawCharS(92, 1, 'A', ST7735_WHITE, ST7735_BLACK, 1);

    ST7735_DrawCharS(102, 1, 'T', ST7735_WHITE, ST7735_BLACK, 1);
    ST7735_DrawCharS(108, 1, 'E', ST7735_WHITE, ST7735_BLACK, 1);
    ST7735_DrawCharS(114, 1, 'C', ST7735_WHITE, ST7735_BLACK, 1);
    ST7735_DrawCharS(120, 1, 'H', ST7735_WHITE, ST7735_BLACK, 1);
}

void Screen_DrawModeTop()
{
    // Display walking/one foot mode
    if(arrow_pos == ONEFOOT_Y)
    {
        ST7735_DrawCharS(5, 1, 'O', ST7735_WHITE, ST7735_BLACK, 1);
        ST7735_DrawCharS(11, 1, 'n', ST7735_WHITE, ST7735_BLACK, 1);
        ST7735_DrawCharS(17, 1, 'e', ST7735_WHITE, ST7735_BLACK, 1);

        ST7735_DrawCharS(29, 1, 'F', ST7735_WHITE, ST7735_BLACK, 1);
        ST7735_DrawCharS(35, 1, 'o', ST7735_WHITE, ST7735_BLACK, 1);
        ST7735_DrawCharS(41, 1, 'o', ST7735_WHITE, ST7735_BLACK, 1);
        ST7735_DrawCharS(47, 1, 't', ST7735_WHITE, ST7735_BLACK, 1);
    }
    else if (arrow_pos == WALKING_Y)
    {
        ST7735_DrawCharS(5, 1, 'W', ST7735_WHITE, ST7735_BLACK, 1);
        ST7735_DrawCharS(11, 1, 'a', ST7735_WHITE, ST7735_BLACK, 1);
        ST7735_DrawCharS(17, 1, 'l', ST7735_WHITE, ST7735_BLACK, 1);
        ST7735_DrawCharS(23, 1, 'k', ST7735_WHITE, ST7735_BLACK, 1);
        ST7735_DrawCharS(29, 1, 'i', ST7735_WHITE, ST7735_BLACK, 1);
        ST7735_DrawCharS(35, 1, 'n', ST7735_WHITE, ST7735_BLACK, 1);
        ST7735_DrawCharS(41, 1, 'g', ST7735_WHITE, ST7735_BLACK, 1);
    }
        
}

// ***************** Screen_Init ****************
// Initialize screen
// Inputs:  none
// Outputs: none
void Screen_Init()
{
    ST7735_InitR(none);
    
	Timer5A_Init();

    // Draw logo
    Screen_DrawGreeting();
    
    Delay1ms(5000);
    
    // Display Main screen
    Screen_DrawMain();
}


// ***************** Screen_DrawGreeting ****************
// Draw a greeting screen
// Inputs:  none
// Outputs: none
void Screen_DrawGreeting(void)
{
    ST7735_FillScreen(ST7735_BLACK);                 // set screen to white
//    ST7735_FillScreen(ST7735_WHITE);                 // set screen to white
    
    // Display ImTa logo lol
    ST7735_DrawCharS(20, 54, 'I', ST7735_RED, ST7735_BLACK, 4);
    ST7735_DrawCharS(40, 54, 'M', ST7735_WHITE, ST7735_BLACK, 4);
    ST7735_DrawCharS(64, 54, 'T', ST7735_RED, ST7735_BLACK, 4);
    ST7735_DrawCharS(88, 54, 'A', ST7735_WHITE, ST7735_BLACK, 4);

    ST7735_DrawCharS(30, 84, 'T', ST7735_WHITE, ST7735_BLACK, 4);
    ST7735_DrawCharS(50, 84, 'E', ST7735_WHITE, ST7735_BLACK, 4);
    ST7735_DrawCharS(74, 84, 'C', ST7735_WHITE, ST7735_BLACK, 4);
    ST7735_DrawCharS(98, 84, 'H', ST7735_WHITE, ST7735_BLACK, 4);
    
//    ST7735_DrawCharS(20, 54, 'I', ST7735_Color565(191,87,0), ST7735_WHITE, 4);
//    ST7735_DrawCharS(40, 54, 'M', ST7735_Color565(191,87,0), ST7735_WHITE, 4);
//    ST7735_DrawCharS(64, 54, 'T', ST7735_Color565(191,87,0), ST7735_WHITE, 4);
//    ST7735_DrawCharS(88, 54, 'A', ST7735_Color565(191,87,0), ST7735_WHITE, 4);

//    ST7735_DrawCharS(30, 84, 'T', ST7735_Color565(191,87,0), ST7735_WHITE, 4);
//    ST7735_DrawCharS(50, 84, 'E', ST7735_Color565(191,87,0), ST7735_WHITE, 4);
//    ST7735_DrawCharS(74, 84, 'C', ST7735_Color565(191,87,0), ST7735_WHITE, 4);
//    ST7735_DrawCharS(98, 84, 'H', ST7735_Color565(191,87,0), ST7735_WHITE, 4);
}


// ***************** Screen_DrawMain ****************
// Draw a main screen
// Inputs:  none
// Outputs: none
void Screen_DrawMain(void)
{
    ST7735_FillScreen(ST7735_BLACK);                 // set screen to white
//    ST7735_FillScreen(ST7735_WHITE);                 // set screen to white
	Screen_DrawLogoTop();
    Screen_DrawArrow(ARROW_X, arrow_pos, ST7735_YELLOW, 2);
    
    // title
    ST7735_DrawCharS(5, 20, 'S', ST7735_CYAN, ST7735_BLACK, 2);
    ST7735_DrawCharS(17, 20, 'e', ST7735_CYAN, ST7735_BLACK, 2);
    ST7735_DrawCharS(29, 20, 'l', ST7735_CYAN, ST7735_BLACK, 2);
    ST7735_DrawCharS(41, 20, 'e', ST7735_CYAN, ST7735_BLACK, 2);
    ST7735_DrawCharS(53, 20, 'c', ST7735_CYAN, ST7735_BLACK, 2);
    ST7735_DrawCharS(68, 20, 't', ST7735_CYAN, ST7735_BLACK, 2);
    
    ST7735_DrawCharS(12, 45, 'T', ST7735_CYAN, ST7735_BLACK, 2);
    ST7735_DrawCharS(24, 45, 'e', ST7735_CYAN, ST7735_BLACK, 2);
    ST7735_DrawCharS(36, 45, 's', ST7735_CYAN, ST7735_BLACK, 2);
    ST7735_DrawCharS(48, 45, 't', ST7735_CYAN, ST7735_BLACK, 2);
    
    ST7735_DrawCharS(72, 45, 'M', ST7735_CYAN, ST7735_BLACK, 2);
    ST7735_DrawCharS(84, 45, 'o', ST7735_CYAN, ST7735_BLACK, 2);
    ST7735_DrawCharS(96, 45, 'd', ST7735_CYAN, ST7735_BLACK, 2);
    ST7735_DrawCharS(108, 45, 'e', ST7735_CYAN, ST7735_BLACK, 2);
    
    // one foot
    ST7735_DrawCharS(20+3, ONEFOOT_Y+1, 'O', ST7735_GREEN, ST7735_BLACK, 2);
    ST7735_DrawCharS(32+3, ONEFOOT_Y+1, 'N', ST7735_GREEN, ST7735_BLACK, 2);
    ST7735_DrawCharS(44+3, ONEFOOT_Y+1, 'E', ST7735_GREEN, ST7735_BLACK, 2);
    
    ST7735_DrawCharS(68+3, ONEFOOT_Y+1, 'F', ST7735_GREEN, ST7735_BLACK, 2);
    ST7735_DrawCharS(80+3, ONEFOOT_Y+1, 'O', ST7735_GREEN, ST7735_BLACK, 2);
    ST7735_DrawCharS(92+3, ONEFOOT_Y+1, 'O', ST7735_GREEN, ST7735_BLACK, 2);
    ST7735_DrawCharS(104+3, ONEFOOT_Y+1, 'T', ST7735_GREEN, ST7735_BLACK, 2);
    
    // walking
    ST7735_DrawCharS(20+3, WALKING_Y+1, 'W', ST7735_GREEN, ST7735_BLACK, 2);
    ST7735_DrawCharS(32+3, WALKING_Y+1, 'A', ST7735_GREEN, ST7735_BLACK, 2);
    ST7735_DrawCharS(44+3, WALKING_Y+1, 'L', ST7735_GREEN, ST7735_BLACK, 2);
    ST7735_DrawCharS(56+3, WALKING_Y+1, 'K', ST7735_GREEN, ST7735_BLACK, 2);
    ST7735_DrawCharS(68+3, WALKING_Y+1, 'I', ST7735_GREEN, ST7735_BLACK, 2);
    ST7735_DrawCharS(80+3, WALKING_Y+1, 'N', ST7735_GREEN, ST7735_BLACK, 2);
    ST7735_DrawCharS(92+3, WALKING_Y+1, 'G', ST7735_GREEN, ST7735_BLACK, 2);
    
//    ST7735_DrawCharS(5, 20, 'S', ST7735_BLUE, ST7735_WHITE, 2);
//    ST7735_DrawCharS(17, 20, 'e', ST7735_BLUE, ST7735_WHITE, 2);
//    ST7735_DrawCharS(29, 20, 'l', ST7735_BLUE, ST7735_WHITE, 2);
//    ST7735_DrawCharS(41, 20, 'e', ST7735_BLUE, ST7735_WHITE, 2);
//    ST7735_DrawCharS(53, 20, 'c', ST7735_BLUE, ST7735_WHITE, 2);
//    ST7735_DrawCharS(68, 20, 't', ST7735_BLUE, ST7735_WHITE, 2);
//    
//    ST7735_DrawCharS(12, 45, 'T', ST7735_BLUE, ST7735_WHITE, 2);
//    ST7735_DrawCharS(24, 45, 'e', ST7735_BLUE, ST7735_WHITE, 2);
//    ST7735_DrawCharS(36, 45, 's', ST7735_BLUE, ST7735_WHITE, 2);
//    ST7735_DrawCharS(48, 45, 't', ST7735_BLUE, ST7735_WHITE, 2);
//    
//    ST7735_DrawCharS(72, 45, 'M', ST7735_BLUE, ST7735_WHITE, 2);
//    ST7735_DrawCharS(84, 45, 'o', ST7735_BLUE, ST7735_WHITE, 2);
//    ST7735_DrawCharS(96, 45, 'd', ST7735_BLUE, ST7735_WHITE, 2);
//    ST7735_DrawCharS(108, 45, 'e', ST7735_BLUE, ST7735_WHITE, 2);
//    
    
}

// ***************** Screen_DrawCalibrationPrep ****************
// Draw a preparation message before calibrating
// Inputs:  none
// Outputs: none
void Screen_DrawCalibrationPrep(void)
{
    ST7735_FillScreen(ST7735_BLACK);                 // set screen to white
//    ST7735_FillScreen(ST7735_WHITE);                 // set screen to white
	Screen_DrawLogoTop();
    Screen_DrawModeTop();

    
    ST7735_DrawCharS(10, 15, 'P', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(22, 15, 'r', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(34, 15, 'e', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(46, 15, 'p', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(58, 15, 'a', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(70, 15, 'r', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(82, 15, 'e', ST7735_MAGENTA, ST7735_BLACK, 2);
    
    ST7735_DrawCharS(16, 32, 'f', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(28, 32, 'o', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(40, 32, 'r', ST7735_MAGENTA, ST7735_BLACK, 2);
    
    ST7735_DrawCharS(64, 32, 't', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(76, 32, 'h', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(88, 32, 'e', ST7735_MAGENTA, ST7735_BLACK, 2);
    
    ST7735_DrawCharS(10, 54, 'C', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(22, 54, 'a', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(34, 54, 'l', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(46, 54, 'i', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(58, 54, 'b', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(70, 54, 'r', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(82, 54, 'a', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(94, 54, '-', ST7735_MAGENTA, ST7735_BLACK, 2);
    
    ST7735_DrawCharS(28, 72, 't', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(40, 72, 'i', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(52, 72, 'o', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(64, 72, 'n', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(76, 72, '!', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(88, 72, '!', ST7735_MAGENTA, ST7735_BLACK, 2);

    
    // Count-down 5 seconds on the screen
    TIMER5_CTL_R = 0x00000001;    // 10) enable TIMER5A
}


// ***************** Screen_DrawCalibration ****************
// Draw a screen while calibrating
// Inputs:  none
// Outputs: none
void Screen_DrawCalibration(void)
{
    ST7735_FillScreen(ST7735_BLACK);                 // set screen to white
//    ST7735_FillScreen(ST7735_WHITE);                 // set screen to white
	Screen_DrawLogoTop();
    Screen_DrawModeTop();
    
    ST7735_DrawCharS(10, ONEFOOT_Y, 'C', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(22, ONEFOOT_Y, 'a', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(34, ONEFOOT_Y, 'l', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(46, ONEFOOT_Y, 'i', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(58, ONEFOOT_Y, 'b', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(70, ONEFOOT_Y, 'r', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(82, ONEFOOT_Y, 'a', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(94, ONEFOOT_Y, '-', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(28, ONEFOOT_Y+25, 't', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(40, ONEFOOT_Y+25, 'i', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(52, ONEFOOT_Y+25, 'n', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(64, ONEFOOT_Y+25, 'g', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(76, ONEFOOT_Y+25, '.', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(88, ONEFOOT_Y+25, '.', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(100, ONEFOOT_Y+25, '.', ST7735_MAGENTA, ST7735_BLACK, 2);
}


// ***************** Screen_DrawTest ****************
// Draw a screen while testing
// Inputs:  none
// Outputs: none
void Screen_DrawTest(void)
{
    ST7735_FillScreen(ST7735_BLACK);                 // set screen to white
//    ST7735_FillScreen(ST7735_WHITE);                 // set screen to white
	Screen_DrawLogoTop();
    Screen_DrawModeTop();
    
    ST7735_DrawCharS(10, ONEFOOT_Y, 'T', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(22, ONEFOOT_Y, 'e', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(34, ONEFOOT_Y, 's', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(46, ONEFOOT_Y, 't', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(58, ONEFOOT_Y, 'i', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(70, ONEFOOT_Y, 'n', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(82, ONEFOOT_Y, 'g', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(94, ONEFOOT_Y, '.', ST7735_MAGENTA, ST7735_BLACK, 2);
    ST7735_DrawCharS(106, ONEFOOT_Y, '.', ST7735_MAGENTA, ST7735_BLACK, 2);
}





// ***************** Screen_GPTest ****************
// Draw a screen to test for general purposes (For debug)
// Inputs:  none
// Outputs: none
void Screen_GPTest(int button, int fsr_num, int fsr_val)
{
    //uint32_t fsr = FSR_InSeq3();
    ST7735_SetCursor(0, 3);
    ST7735_OutString("FSR: ");
    ST7735_OutUDec(fsr_num);
		ST7735_OutString(", value: ");
    ST7735_OutUDec(fsr_val);
    ST7735_SetCursor(0, 5);
    if(button == 0) 
        ST7735_OutString("Switch: x");
    if(button == 6) 
        ST7735_OutString("Switch: PortC");
    if(button == 3) 
        ST7735_OutString("Switch: PortB");
}



// ***************** Timer3A_Init ****************
// Activate TIMER5 interrupts to run user task every second.
// This is used to count down 5 seconds before the calibration
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer5A_Init(){
  SYSCTL_RCGCTIMER_R |= 0x20;   // 0) activate TIMER5
  TIMER5_CTL_R = 0x00000000;    // 1) disable TIMER5A during setup
  TIMER5_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER5_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER5_TAILR_R = 80000000-1;    // 4) reload value CHECK THIS VALUE
  TIMER5_TAPR_R = 0;            // 5) bus clock resolution
  TIMER5_ICR_R = 0x00000001;    // 6) clear TIMER5A timeout flag
  TIMER5_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI23_R = (NVIC_PRI23_R&0xFFFFFF00)|0x000000C0; // 8) priority 6
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN2_R |= 1<<28;           // 9) enable IRQ 92 in NVIC
  //TIMER5_CTL_R = 0x00000001;    // 10) enable TIMER5A
  //EndCritical(sr);
}

void Timer5A_Handler(void){
    TIMER5_ICR_R = TIMER_ICR_TATOCINT;// acknowledge time5A timeout

    if(calib_sec >= 0)
    {
        ST7735_DrawCharS(55, 110, 48+calib_sec, ST7735_MAGENTA, ST7735_BLACK, 4);
        calib_sec--;
    }
    else // Preparation done. Switch to actual calibration mode.
    {
        TIMER5_CTL_R = 0x00000000;    // 1) disable TIMER5A during setup
        calib_sec = 5;
        
		test_mode = MODE_CALIB; // calibration mode
        Screen_DrawCalibration();
    }
    
}


