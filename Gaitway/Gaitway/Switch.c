// Lab 5 - Music player and Audio amp
// Hyejun Im (hi956), Sarthak Gupta(sg38368)
// TA: William Glanton

#include "Switch.h"
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Audio.h"
#include "Screen.h"
#include "FSRs.h"
#include "ST7735.h"


#define PD0                     (*((volatile uint32_t *)0x40007000))
#define PF4                     (*((volatile uint32_t *)0x40025040))
    
#define SWITCH_TEST 8    // PB3
#define SWITCH_CALIB 64        // PC6

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

int temp =0;
int port_num = 0;
int save_inds = 0;
int inst2 = 0;
int inst3 = 0;

volatile static unsigned long Touch;     // true on touch
volatile static unsigned long Release;   // true on release
volatile static unsigned long Last;      // previous
void (*TouchTask)(void);    // user function to be executed on touch
void (*ReleaseTask)(void);  // user function to be executed on release

static void Timer0Arm(void){
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x0000001;    // 3) 1-SHOT mode
  TIMER0_TAILR_R = 16000000;      // 4) 10ms reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R |= 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
	
	
}
static void GPIOArm(void){
  GPIO_PORTB_ICR_R = 0x08;      // (e) clear flag4
  GPIO_PORTB_IM_R |= 0x08;      // (f) arm interrupt on PB0 *** No IME bit as mentioned in Book ***
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF)|0x00002000; // (g) priority 1
  NVIC_EN0_R |= 1 << 1;      // (h) enable interrupt 1 in NVIC  
    
    
      // need to init Port C too!!!
  GPIO_PORTC_ICR_R = 0x40;      // (e) clear flag4
  GPIO_PORTC_IM_R |= 0x40;      // (f) arm interrupt on PB0 *** No IME bit as mentioned in Book ***
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF)|0x00200000; // (g) priority 1
  NVIC_EN0_R |= 1 << 2;      // (h) enable interrupt 2 in NVIC  
    
}


// ***************** Switch_Init ****************
// Initialize switch interface on PB4 and PC6
// Inputs:  pointer to a function to call on touch (falling edge),
//          pointer to a function to call on release (rising edge)
// Outputs: none 
void Switch_Init(){
  // **** general initialization ****
  SYSCTL_RCGCGPIO_R |= 0x00000002; // (a) activate clock for port B
  while((SYSCTL_PRGPIO_R & 0x00000002) == 0){};
  GPIO_PORTB_DIR_R &= ~0x08;    // (c) make PB3 in 
  GPIO_PORTB_AFSEL_R &= ~0x08;  //     disable alt funct on PB3
  GPIO_PORTB_DEN_R |= 0x08;     //     enable digital I/O on PB3   
  GPIO_PORTB_PCTL_R &= ~0x0000F000; // configure PB3 as GPIO
  GPIO_PORTB_AMSEL_R = 0;       //     disable analog functionality on PB
  GPIO_PORTB_IS_R &= ~0x08;     // (d) PB3 is edge-sensitive
      GPIO_PORTB_IBE_R |= 0x08;
  GPIO_PORTB_CR_R = 0x1F;           // allow changes to PF4-0
      
      // need to init Port C6 too!!!
      // comment to test portB
  SYSCTL_RCGCGPIO_R |= 0x00000004; // (a) activate clock for port C
  while((SYSCTL_PRGPIO_R & 0x00000004) == 0){};
  GPIO_PORTC_DIR_R &= ~0x40;    // (c) make PC6 in 
  GPIO_PORTC_AFSEL_R &= ~0x40;  //     disable alt funct on PC6
  GPIO_PORTC_DEN_R |= 0x40;     //     enable digital I/O on PC6   
  GPIO_PORTC_PCTL_R &= ~0x0F000000; // configure PC6 as GPIO
  GPIO_PORTC_AMSEL_R = 0;       //     disable analog functionality on PC
  GPIO_PORTC_IS_R &= ~0x40;     // (d) PC3 is edge-sensitive
  GPIO_PORTC_IEV_R |= 0x40;
      
      
      
  GPIOArm();

  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
 }

 
 // TEST button pressed
// Interrupt on rising or falling edge of PB3 (CCP0)
void GPIOPortB_Handler(void){
    port_num = GPIO_PORTB_DATA_R&0x08;

    if(port_num == SWITCH_TEST) // test button pressed
    {
        GPIO_PORTB_IM_R &= ~0x08;     // disarm interrupt on PB3
        
        if(test_mode == MODE_MAIN){ //arrow movement in the main screen
            Screen_MoveArrow(ST7735_YELLOW, 2);
        }
        else if(test_mode == MODE_TEST){ // button pressed during testing mode to go back to the main screen
            test_mode = MODE_MAIN;
            Screen_DrawMain();
        }
        Timer0Arm(); // start one shot
    }
}


// CALIB button pressed
// Interrupt on rising or falling edge of PB3 (CCP0)
void GPIOPortC_Handler(void){
    port_num = GPIO_PORTC_DATA_R&0x40;

    if(port_num == SWITCH_CALIB) // test button pressed
    {
        GPIO_PORTC_IM_R &= ~0x40;     // disarm interrupt on PC6
        
        
        if(test_mode == MODE_MAIN){ // select mode and start calibration
            test_mode = MODE_CALIBPREP; // show calibration preparation screen
            Screen_DrawCalibrationPrep();
        }
        Timer0Arm(); // start one shot
    }
}



// Interrupt 10 ms after rising edge of PF4
void Timer0A_Handler(void){
  TIMER0_IMR_R = 0x00000000;    // disarm timeout interrupt
  //Last = PD0;  // switch state
  GPIOArm();   // start GPIO
}

