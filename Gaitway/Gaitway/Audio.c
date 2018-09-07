// EE445L Project : GaitWay
// Hyejun Im (hi956), Sarthak Gupta(sg38368)
// TA: William Glanton

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Audio.h"
#include "Switch.h"
#include "FSRs.h"

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


uint8_t voiceidx; // index of a current voice
uint8_t notesidx; // index of a current note
uint8_t playsong; // title of the current song


SONG songRL;
SONG songF;
SONG songB;

SONG* songCurr;


// waves
unsigned short Bassoon[WAVE_SIZE]={1068, 1169, 1175, 1161, 1130, 1113, 1102, 1076, 1032, 985, 963, 987, 1082, 1343, 1737, 1863, 1575, 1031, 538, 309, 330, 472, 626, 807, 1038, 1270, 1420, 1461, 1375, 1201, 1005, 819, 658, 532, 496, 594, 804, 1055, 1248, 1323, 1233, 1049, 895, 826, 826, 850, 862, 861, 899, 961, 1006, 1023, 1046, 1092, 1177, 1224, 1186, 1133, 1098, 1102, 1109, 1076, 1027, 1003};
unsigned short Oboe[WAVE_SIZE]={1024, 1024, 1014, 1008, 1022, 1065, 1093, 1006, 858, 711, 612, 596, 672, 806, 952, 1074, 1154, 1191, 1202, 1216, 1236, 1255, 1272, 1302, 1318, 1299, 1238, 1140, 1022, 910, 827, 779, 758, 757, 782, 856, 972, 1088, 1177, 1226, 1232, 1203, 1157, 1110, 1067, 1028, 993, 958, 929, 905, 892, 900, 940, 1022, 1125, 1157, 1087, 965, 836, 783, 816, 895, 971, 1017};
unsigned short Trumpet[WAVE_SIZE]={987, 1049, 1090, 1110, 1134, 1160, 1139, 1092, 1070, 1042, 1035, 1029, 1008, 1066, 1150, 1170, 1087, 915, 679, 372, 151,  558, 1014, 1245, 1260, 1145, 1063, 984, 934, 960, 1027, 1077, 1081, 1074, 1064, 1042, 1010, 974, 968, 974, 994, 1039,  1094, 1129, 1125, 1092, 1056, 1056, 1082, 1059, 1046, 1058, 1061, 1045, 1034, 1050, 1094, 1112, 1092, 1063, 1053, 1065, 1052, 992};
unsigned short Flute[WAVE_SIZE] = {1007,1252,1374,1548,1698,1797,1825,1797,1675,1562,1383,1219,1092,1007,913,890,833,847,810,777,744,674,598,551,509,476,495,533,589,659,758,876,1007,1252,1374,1548,1698,1797,1825,1797,1675,1562,1383,1219,1092,1007,913,890,833,847,810,777,744,674,598,551,509,476,495,533,589,659,758,876};
unsigned short Sin[WAVE_SIZE] = {1024,1122,1215,1302,1378,1440,1486,1514,1524,1514,1486,1440,1378,1302,1215,1122,1024,926,833,746,670,608,562,534,524,534,562,608,670,746,833,926,1024,1122,1215,1302,1378,1440,1486,1514,1524,1514,1486,1440,1378,1302,1215,1122,1024,926,833,746,670,608,562,534,524,534,562,608,670,746,833,926};
unsigned short Horn[WAVE_SIZE] = {1063,1082,1119,1275,1678,1748,1275,755,661,661,703,731,769,845,1039,1134,1209,1332,1465,1545,1427,1588,1370,1086,708,519,448,490,566,684,802,992,1063,1082,1119,1275,1678,1748,1275,755,661,661,703,731,769,845,1039,1134,1209,1332,1465,1545,1427,1588,1370,1086,708,519,448,490,566,684,802,992};

//unsigned short ForLab[WAVE_SIZE] = {0, 512, 1024, 1536, 2048, 2560, 3072, 3584, 4096};
//unsigned short ForLab[WAVE_SIZE] = {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};
unsigned short ForLab[WAVE_SIZE] = {2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048};



// notes
unsigned short NoteRL[NOTE_SIZE] = {C_2, C_2, C_2, C_2, D_1, D_1, D_1, D_1};
unsigned short NoteF[NOTE_SIZE] = {C_2, C_2, C_2, C_2, D_1, D_1, D_1, D_1};
unsigned short NoteB[NOTE_SIZE] = {C_2, C_2, C_2, C_2, C_2, C_2, C_2, C_2};

const unsigned short* currentMusic;


#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))


// ***************** Timer1A_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer1A_Init(void){
   // long sr;
  //sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  //TIMER1_TAILR_R = period-1;    // 4) reload value
   TIMER1_TAILR_R = C-1;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x40000000; // 8) priority 2
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R |= 1<<21;           // 9) enable IRQ 21 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
  //EndCritical(sr);
}

void Timer1A_Handler(void){
    TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer1A timeout    
    
    if(songCurr->notes[note_idx] != 0)
        DAC_Out(songCurr->wave[inst_idx1]);

    inst_idx1++;
    if(inst_idx1 == WAVE_SIZE) 
        inst_idx1 = 0;

}



// ***************** Timer2A_Init ****************
// Activate TIMER2 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer2A_Init(void){
   // long sr;
  //sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate TIMER2
  TIMER2_CTL_R = 0x00000000;    // 1) disable TIMER2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = EIGHTH-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear TIMER2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x20000000; // 8) priority 1
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R |= 1<<23;           // 9) enable IRQ 21 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable TIMER1A
  //EndCritical(sr);
}

void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer1A timeout

    TIMER1_TAILR_R = songCurr->notes[note_idx];

    note_idx++;

    if(note_idx == NOTE_SIZE) 
        note_idx = 0;
}



//********Audio_Init*****************
// Initialize Max5353 12-bit DAC
// inputs:  initial voltage output (0 to 4095)
// outputs: none
// assumes: system clock rate less than 20 MHz
void Audio_Init(){
  SYSCTL_RCGCSSI_R |= 0x08;       // activate SSI3
  SYSCTL_RCGCGPIO_R |= 0x08;      // activate port D
  while((SYSCTL_PRGPIO_R&0x08) == 0){};// ready?
  GPIO_PORTD_AFSEL_R |= 0x0B;     // enable alt funct on PD 0,1,3
  GPIO_PORTD_DEN_R |= 0x0B;       // configure PD 0,1,3 as SSI
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0xFFFF0F00)+0x00001011; // SSI1 is 0x00002022
  GPIO_PORTD_AMSEL_R = 0;         // disable analog functionality on PD
  SSI3_CR1_R = 0x00000000;        // disable SSI, master mode
  SSI3_CPSR_R = 0x02;             // 8 MHz SSIClk 
  SSI3_CR0_R &= ~(0x0000FFF0);    // SCR = 0, SPH = 0, SPO = 0 Freescale
  SSI3_CR0_R |= 0x0F;             // DSS = 16-bit data
  SSI3_DR_R = 0xC000;               // load 'data' into transmit FIFO
  SSI3_CR1_R |= 0x00000002;       // enable SSI
      
      initSong(&songRL, 1, Flute, NoteRL);
      initSong(&songF, 2, Flute, NoteF);
      initSong(&songB, 3, Flute, NoteB);
      songCurr = &songRL;

      Timer1A_Init();
      Timer2A_Init();
      
}



////////////////////
//**FSR_INFO
// Black- Right - fsr_num 0
// Green - Left - fsr_num 1
// Yellow - Front - fsr_num 2
// Red - Back - fsr_num 3
/////////////////////

//********DAC_Out*****************
// Send data to Max5353 12-bit DAC
// inputs:  voltage output (0 to 4095)
// outputs: none
void DAC_Out(uint16_t code){
        while((SSI3_SR_R&0x00000002)==0){};// SSI Transmit FIFO Not Full
			 //SSI3_DR_R =  0x5000 + 0x0000;	//write a blank value to buffer so DAC B will be updated with nothing
       
					
				//the code below has been tested to only output to your right ear
				if(fsrError[0]){ // right
					SSI3_DR_R =  0x4000 + code; // DAC B, only right
				}
				
				else if(fsrError[1]){ // left
					SSI3_DR_R =  0x8000 + code; // only left
				}
					
				else if(fsrError[2]){ // front
					SSI3_DR_R =  0x2000 + code; // buff
					SSI3_DR_R =  0xC000 + code; // R
					SSI3_DR_R =  0x8000 + code; // L
				}
				else if(fsrError[3]){ // back
					SSI3_DR_R =  0x2000 + code; // buff
					SSI3_DR_R =  0xC000 + code; // R
					SSI3_DR_R =  0x8000 + code; // L
				}
//				else{
//					SSI3_DR_R =  0x8000 + 0; // only left
//				}
                
                
                
       //SSI3_DR_R =  0x1000 + code; // DAC B / left
       //SSI3_DR_R =  0x8000 + code; // DAC B / left
            
       //SSI3_DR_R =  0x4000 + code; // DAC A
       //SSI3_DR_R =  0x0000 + code; // DAC B
       
            // SSI0_DR_R =   code; 
}                // data out, no reply



//********initSong*****************
// Initialize SONG struct
// inputs:  song - SONG struct being initialized
//              stitle - number of the song (A member of SONG)
//              swave - wave of the song (A member of SONG)
//              snotes - notes of the song (A member of SONG)
// outputs: none
void initSong(SONG* song, int stitle, unsigned short* swave, unsigned short* snotes)
{
    
    song->title = stitle;
    
    for(int i = 0; i < WAVE_SIZE; i++)
    {
        song->wave[i] = swave[i];
    }
    
    for(int i = 0; i < NOTE_SIZE; i++)
    {
        song->notes[i] = snotes[i];
    }
    
}

