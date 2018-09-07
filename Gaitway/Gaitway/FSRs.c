// EE445L Project : GaitWay
// Hyejun Im (hi956), Sarthak Gupta(sg38368)
// TA: William Glanton


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "FSRs.h"
#include "Screen.h"
#include "Audio.h"




#define CALIBRATION_PERIOD 160000 // 2/1000 seconds(BUS freq*2/1000)
#define CALIBRATION_IDX 1000
#define FSR_NUM 4
int fsr[FSR_NUM][CALIBRATION_IDX] = {0,}; // fsr[0][]: fsr front
int fsr_avg[FSR_NUM] = {0,};
int fsr_num = 0, calibration_idx = 0;


int fsrError[4] = {0,0,0,0};
#define TEST_PERIOD 32000 // 2/1000 seconds(BUS freq*2/1000)

int test_mode = MODE_MAIN; // 0: Calibration mode, 1: Test mode

#define SND_LEV1 100
#define SND_LEV2 300

int count = 0;



// ***************** Timer3A_Init ****************
// Activate TIMER3 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer3A_Init(){
  SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate TIMER3
  TIMER3_CTL_R = 0x00000000;    // 1) disable TIMER3A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER3_TAILR_R = CALIBRATION_PERIOD-1;    // 4) reload value CHECK THIS VALUE
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear TIMER3A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x00000000; // 8) priority 6       *change back to C*
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN1_R |= 1<<3;           // 9) enable IRQ 35 in NVIC
  //TIMER3_CTL_R = 0x00000001;    // 10) enable TIMER3A
  //EndCritical(sr);
}

void Timer3A_Handler(void){
    TIMER3_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer31A timeout

    fsr[fsr_num][calibration_idx] = FSR_InSeq3();
    
    fsr_num++;
    
    if(fsr_num == FSR_NUM)
    {
        fsr_num = 0;
        calibration_idx++;
        if(calibration_idx == CALIBRATION_IDX) // time to end calibration
        {
                int average = 0;
            // calculate average of FSR values
            for(int j = 0; j < FSR_NUM; j++)
            {
                for(int i = 0; i < CALIBRATION_IDX; i++)
                    average += fsr[j][i];
                average /= CALIBRATION_IDX;
                fsr_avg[j] = average;
            }
            
            /////////////////////////////////////////////////////////////////
            // A temporary code to restart calibration during debug mode
            // This code should be removed
            //calibration_idx = 0;
            /////////////////////////////////////////////////////////////////
						
            test_mode = MODE_TEST; // change to test mode
            
        }
				
    }
}



// ***************** Timer4A_Init ****************
// Activate TIMER4 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer4A_Init(){
   // long sr;
  //sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x10;   // 0) activate TIMER4
  TIMER4_CTL_R = 0x00000000;    // 1) disable TIMER4A during setup
  TIMER4_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER4_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER4_TAILR_R = TEST_PERIOD-1;    // 4) reload value CHECK THIS VALUE
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

    
}


// ***************** FSR_Init ****************
// Initialize everything regarding with FSRs(Timer, ADC etc)
// Inputs:  none
// Outputs: none
void FSR_Init(void)
{
    SYSCTL_RCGCADC_R |= 0x0001;   // 7) activate ADC0 
                                  // 1) activate clock for Port E
    SYSCTL_RCGCGPIO_R |= 0x10;
    while((SYSCTL_PRGPIO_R&0x10) != 0x10){};
      
    GPIO_PORTE_DIR_R &= ~0x2E;      // 2) make PE1,2,3,5 input
    GPIO_PORTE_AFSEL_R |= 0x2E;     // 3) enable alternate function on PE1,2,3,5
    GPIO_PORTE_DEN_R &= ~0x2E;      // 4) disable digital I/O on PE1,2,3,5
    GPIO_PORTE_AMSEL_R |= 0x2E;     // 5) enable analog functionality on PE1,2,3,5
      
    GPIO_PORTE_DIR_R &= ~0x10;      // 2) make PE4 input
    GPIO_PORTE_AFSEL_R |= 0x10;     // 3) enable alternate function on PE4
    GPIO_PORTE_DEN_R &= ~0x10;      // 4) disable digital I/O on PE4
    GPIO_PORTE_AMSEL_R |= 0x10;     // 5) enable analog functionality on PE4
    


    ADC0_PC_R &= ~0xF;              // 7) clear max sample rate field
    ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
    ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
    ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
//    ADC0_SSMUX3_R &= ~0x000F;       // 11) clear SS3 field
//    ADC0_SSMUX3_R += 9;             //    set channel
    ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
    ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
    ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
   
    //////////////////////////////////
    // Initialize a timer for calibration
    Timer3A_Init();
}


// ***************** FSR_Calibration ****************
// Do everything about Calibration function
// Set timer, measure boundaries and then stop the timer.
// Inputs:  none
// Outputs: none
void FSR_Calibration(void)
{
    // enable timer3A
		SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate TIMER3
		TIMER3_CTL_R = 0x00000001;    // 10) enable TIMER3A
	
	  // set parameters
	  fsr_num = 0;
		calibration_idx = 0;
	
    // show messages on screen
    Screen_DrawCalibration();
	
    // Wait for 8 seconds
    while(test_mode == MODE_CALIB)
    {}
        
    // Turn off the timer
    TIMER3_CTL_R = 0x00000000;    // 1) disable TIMER3A
    SYSCTL_RCGCTIMER_R &= 0xF7;   // 0) deactivate TIMER3
    
		// Draw testing screen 
			Screen_DrawTest();
}

int FSR_Mesaure(void);
// ***************** FSR_Test ****************
// Do everything about Testing function
// Check voltage every time period, detect error type and make actions
// Inputs:  none
// Outputs: none
void FSR_Test(void)
{
    //////////////////////////////////
    // Initialize a timer for testing
    //Timer4A_Init();
    //FSR_Measure();
		for(int i = 0; i < 4; i++){
			fsr_num = i;
			fsrError[i] = 1;
			while(fsrError[i] == 1){
				int fsr_now = FSR_Mesaure(); // measure current pressure
                
                if(arrow_pos == ONEFOOT_Y) // testing one foot mode
                {
                    if((fsr_now - fsr_avg[i]) > (fsr_avg[i]) * 0.06){
                        //Screen_GPTest(0, i, fsr_now); // For debugging
                        
                        // set error flag
                        fsrError[i] = 1;
                        
                        // change music
                        if(i==0 || i==1) songCurr = &songRL;
                        else if(i==2) songCurr = &songF;
                        else if(i==3) songCurr = &songB;
                            
                        TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
                    }else{
                        fsrError[i] = 0;
                        TIMER1_CTL_R = 0x00000000;    // 10) disable TIMER1A
                    }
				}
                else if(arrow_pos == WALKING_Y) // testing walking mode
                {
                    if(i == 0) // right
                    {
                        if((fsr_now - fsr_avg[i]) > (fsr_avg[i]) * 0.28){
                            //Screen_GPTest(0, i, fsr_now); // For debugging
                            
                            // set error flag
                            fsrError[i] = 1;
                            
                            // change music
                            songCurr = &songRL;
                                
                            
                            TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
                        }
                        else{
                            fsrError[i] = 0;
                            TIMER1_CTL_R = 0x00000000;    // 10) disable TIMER1A
                        }
                    }
                    else if(i==1) // left
                    {
                        if((fsr_now - fsr_avg[i]) > (fsr_avg[i]) * 0.24){
                            //Screen_GPTest(0, i, fsr_now); // For debugging
                            
                            // set error flag
                            fsrError[i] = 1;
                            
                            // change music
                            songCurr = &songRL;
                                
                            
                            TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
                        }
                        else{
                            fsrError[i] = 0;
                            TIMER1_CTL_R = 0x00000000;    // 10) disable TIMER1A
                        }
                    }
                    else if (i==2) // front
                    {
                        
                        if((fsr_now - fsr_avg[i]) > (fsr_avg[i]) * 0.39){
                            //Screen_GPTest(0, i, fsr_now); // For debugging
                            
                            // set error flag
                            fsrError[i] = 1;
                            
                            // change music
                            songCurr = &songF;
                                
                            
                            TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
                        }else{
                            fsrError[i] = 0;
                            TIMER1_CTL_R = 0x00000000;    // 10) disable TIMER1A
                        }
                    }
                    else if(i==3) // back
                    {
                        if((fsr_now - fsr_avg[i]) > (fsr_avg[i]) * 0.45){
                            //Screen_GPTest(0, i, fsr_now); // For debugging
                            
                            // set error flag
                            fsrError[i] = 1;
                            
                            // change music
                            songCurr = &songB;
                                
                            
                            TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
                        }else{
                            fsrError[i] = 0;
                            TIMER1_CTL_R = 0x00000000;    // 10) disable TIMER1A
                        }
                    }
				}
			}
		}
    // show messages on screen
    
}



// ***************** ADC0_InSeq3 ****************
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t FSR_InSeq3(void){  
    uint32_t result;

    /* PE1 : AIN2
        PE2 : AIN1
        PE3 : AIN0
        PE5 : AIN8
    */
    ADC0_SSMUX3_R &= ~0x000F;       // 11) clear SS3 field
    if(fsr_num == 0)
        ADC0_SSMUX3_R += 2;             //    set channel
    else if(fsr_num == 1)
        ADC0_SSMUX3_R += 1;             //    set channel
    else if(fsr_num == 2)
        ADC0_SSMUX3_R += 0;             //    set channel
    else if(fsr_num == 3)
        ADC0_SSMUX3_R += 8;             //    set channel
    
    ADC0_PSSI_R = 0x0008;            // 1) initiate SS3
    while((ADC0_RIS_R&0x08)==0){};   // 2) wait for conversion done
    // if you have an A0-A3 revision number, you need to add an 8 usec wait here
    result = ADC0_SSFIFO3_R&0xFFF;   // 3) read result

    ADC0_ISC_R = 0x0008;             // 4) acknowledge completion
    return result;
}

//this function will be used to measure the current values of the FSRs to see if there is error

int FSR_Mesaure(void){
	int avg = 0;
	for(int i = 0; i<100; i++)
	{
		avg += FSR_InSeq3();
	}
	avg /= 100;
		return avg;
//	for(int i = 0; i < 4; i++){
//		fsr_num = i;
//		if((FSR_InSeq3() - fsr_avg[i]) > (fsr_avg[i] - 1700) * 0.1){
//			fsrError[i] = 1;
//		}else{
//			fsrError[i] = 0;
//		}
//	}
	
}
