// Lab 5 - Music player and Audio amp
// Hyejun Im (hi956), Sarthak Gupta(sg38368)
// TA: William Glanton

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "DAC.h"



//********DAC_Init*****************
// Initialize Max5353 12-bit DAC
// inputs:  initial voltage output (0 to 4095)
// outputs: none
// assumes: system clock rate less than 20 MHz
void DAC_Init(uint16_t data){
  SYSCTL_RCGCSSI_R |= 0x01;       // activate SSI0
  SYSCTL_RCGCGPIO_R |= 0x01;      // activate port A
  while((SYSCTL_PRGPIO_R&0x01) == 0){};// ready?
  GPIO_PORTA_AFSEL_R |= 0x2C;     // enable alt funct on PA2,3,5
  GPIO_PORTA_DEN_R |= 0x2C;       // configure PA2,3,5 as SSI
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFF0F00FF)+0x00202200;
  GPIO_PORTA_AMSEL_R = 0;         // disable analog functionality on PA
  SSI0_CR1_R = 0x00000000;        // disable SSI, master mode
  SSI0_CPSR_R = 0x02;             // 8 MHz SSIClk 
  SSI0_CR0_R &= ~(0x0000FFF0);    // SCR = 0, SPH = 0, SPO = 0 Freescale
  SSI0_CR0_R |= 0x0F;             // DSS = 16-bit data
  SSI0_DR_R = data;               // load 'data' into transmit FIFO
  SSI0_CR1_R |= 0x00000002;       // enable SSI

}



//********DAC_Out*****************
// Send data to Max5353 12-bit DAC
// inputs:  voltage output (0 to 4095)
// outputs: none
void DAC_Out(uint16_t code){
        while((SSI0_SR_R&0x00000002)==0){};// SSI Transmit FIFO Not Full
       SSI0_DR_R =  0xC000 + code; 
            // SSI0_DR_R =   code; 
        //SSI0_DR_R |= 0xC0000000;
}                // data out, no reply
  
//********DAC_Out*****************
// Send data to Max5353 12-bit DAC
// inputs:  voltage output (0 to 4095)
// outputs: reply is returned
// send the 16-bit code to the SSI, return a reply
uint16_t DAC_Out2(uint16_t code){   uint16_t receive;
  while((SSI0_SR_R&0x00000002)==0){};// SSI Transmit FIFO Not Full
  SSI0_DR_R = code;                  // data out
  while((SSI0_SR_R&0x00000004)==0){};// SSI Receive FIFO Not Empty
  receive = SSI0_DR_R;               // acknowledge response
  return receive;
}

void DAC_InitRaw(void){
  SYSCTL_RCGCSSI_R |= 0x01;       // activate SSI0
  SYSCTL_RCGCGPIO_R |= 0x01;      // activate port A
  while((SYSCTL_PRGPIO_R&0x01) == 0){};// ready?
  GPIO_PORTA_AFSEL_R |= 0x24;     // enable alt funct on PA2,5, regular GPIO on PA3
  GPIO_PORTA_DEN_R |= 0x2C;       // configure PA2,5 as SSI, PA3 is regular GPIO
  GPIO_PORTA_DATA_R |= 0x08;      // PA3 FSS high
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFF0F00FF)+0x00200200;
  GPIO_PORTA_AMSEL_R = 0;         // disable analog functionality on PA
  SSI0_CR1_R = 0x00000000;        // disable SSI, master mode
  SSI0_CPSR_R = 0x02;             // 8 MHz SSIClk 
  SSI0_CR0_R &= ~(0x0000FFF0);    // SCR = 0, SPH = 0, SPO = 0 Freescale
  SSI0_CR0_R |= 0x0F;             // DSS = 16-bit data
  SSI0_CR1_R |= 0x00000002;       // enable SSI

}
//********DAC_Out*****************
// Send data to Max5353 12-bit DAC
// inputs:  voltage output (0 to 4095)
// outputs: none
// send the 16-bit code to the SSI, 
void DAC_OutRaw(uint16_t code){  
  GPIO_PORTA_DATA_R &= ~0x08;        // PA3 FSS low
  
  while((SSI0_SR_R&0x00000002)==0){};// SSI Transmit FIFO Not Full
  SSI0_DR_R = code;                  // data out

  GPIO_PORTA_DATA_R |= 0x08;         // PA3 FSS high
}

const unsigned short* currentMusic;

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
//unsigned short Note[NOTE_SIZE] = {C};
unsigned short Note1[NOTE_SIZE] = {0,0, DF,DF,F,F,DF,DF,F,F,DF,DF,F,F,GF,F,EF,DF,C0,C0,EF,EF,C0,C0,EF,EF,C0,C0,EF,EF,F,EF,DF,C0,DF,DF,F,F,DF,DF,F,F,DF,EF,F,GF,AF,AF,AF,AF,GF,F,EF,DF,F,EF,DF,C0,DF,DF,DF,DF,DF,DF,0,0,DF,DF,DF,DF,EF,DF,C0,DF,EF,EF,EF,EF,F,EF,DF,EF,F,F,F,F,GF,F,EF,DF,C0,C0,EF,DF,DF,DF,0,0,F,F,F,F,GF,F,EF,F,GF,GF,GF,GF,GF,GF,0,0,F,F,F,F,GF,F,EF,DF,EF,EF,EF,EF,EF,EF,0,0,AF,AF,AF,AF,BF,AF,GF,F,GF,GF,GF,GF,AF,GF,F,EF,F,F,F,F,GF,F,EF,DF,C0,C0,EF,DF,DF,DF,0,0,BF,C,DF1,DF1,C,BF,AF,AF,GF,GF,F,F,GF,GF,AF,AF,BF,BF,BF,C,DF1,DF1,C,BF,AF,AF,GF,GF,F,GF,F,GF,EF,EF,0,0,BF,C,DF1,DF1,C,BF,AF,AF,GF,GF,F,F,GF,GF,AF,AF,0,0,GF,F,EF,DF,F,EF,DF,C0,DF,DF,DF,DF,DF,DF,0,0};
unsigned short Note2[NOTE_SIZE] = {0,0,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,C0,C0,C0,C0,C0,C0,C0,C0,C0,C0,C0,C0,C0,C0,C0,C0,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,GF,GF,GF,GF,GF,GF,GF,GF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,DF,EF,EF,EF,EF,EF,EF,EF,EF,F,F,F,F,F,F,F,F,C0,C0,C0,C0,C0,C0,C0,C0,F,F,F,F,F,F,F,F,GF,GF,GF,GF,GF,GF,GF,GF,F,F,F,F,F,F,F,F,EF,EF,EF,EF,EF,EF,EF,EF,AF,AF,AF,AF,AF,AF,AF,AF,GF,GF,GF,GF,GF,GF,GF,GF,F,F,F,F,F,F,F,F,C0,C0,C0,C0,C0,C0,C0,C0,DF1,DF1,DF1,DF1,DF1,DF1,DF1,DF1,F,F,F,F,F,F,F,F,DF1,DF1,DF1,DF1,DF1,DF1,DF1,DF1,F,F,F,F,F,F,F,F,DF1,DF1,DF1,DF1,DF1,DF1,DF1,DF1,F,F,F,F,F,F,F,F,GF,GF,GF,GF,GF,GF,GF,GF,DF,DF,DF,DF,DF,DF,DF,DF};

unsigned short Note3[NOTE_SIZE] = {0,0,
DF,DF,0,0,DF,DF,0,0,
DF,DF,0,0,DF,DF,0,0,
C0,C0,0,0,C0,C0,0,0,
C0,C0,0,0,C0,C0,0,0,
DF,DF,0,0,DF,DF,0,0,
DF,DF,0,0,DF,DF,0,0,
GF,GF,0,0,GF,GF,0,0,
DF,DF,0,0,DF,DF,0,0,
DF,DF,0,0,DF,DF,0,0,
EF,EF,0,0,EF,EF,0,0,
F,F,0,0,F,F,0,0,
C0,C0,0,0,C0,C0,0,0,
F,F,0,0,F,F,0,0,
GF,GF,0,0,GF,GF,0,0,
F,F,0,0,F,F,0,0,
EF,EF,0,0,EF,EF,0,0,
AF,AF,0,0,AF,AF,0,0,
GF,GF,0,0,GF,GF,0,0,
F,F,0,0,F,F,0,0,
C0,C0,0,0,C0,C0,0,0,
0,0,DF1,DF1,0,0,DF1,DF1,0,0,
F,F,0,0,F,F,0,0,
0,0,DF1,DF1,0,0,DF1,DF1,0,0,
F,F,0,0,F,F,0,0,
0,0,DF1,DF1,0,0,DF1,DF1,0,0,
F,F,0,0,F,F,0,0,
GF,GF,0,0,GF,GF,0,0,
DF,DF,DF,DF,DF,DF,0,0,};

unsigned short Note4[NOTE_SIZE] = {0,0,
DF,DF,0,0,0,0,0,0,
DF,DF,0,0,0,0,0,0,
C0,C0,0,0,0,0,0,0,
C0,C0,0,0,0,0,0,0,
DF,DF,0,0,0,0,0,0,
DF,DF,0,0,0,0,0,0,
GF,GF,0,0,0,0,0,0,
DF,DF,0,0,0,0,0,0,
DF,DF,0,0,0,0,0,0,
EF,EF,0,0,0,0,0,0,
F,F,0,0,0,0,0,0,
C0,C0,0,0,0,0,0,0,
F,F,0,0,0,0,0,0,
GF,GF,0,0,0,0,0,0,
F,F,0,0,0,0,0,0,
EF,EF,0,0,0,0,0,0,
AF,AF,0,0,0,0,0,0,
GF,GF,0,0,0,0,0,0,
F,F,0,0,0,0,0,0,
C0,C0,0,0,0,0,0,0,
0,0,DF1,DF1,0,0,0,0,0,0,
F,F,0,0,0,0,0,0,
0,0,DF1,DF1,0,0,0,0,0,0,
F,F,0,0,0,0,0,0,
0,0,DF1,DF1,0,0,0,0,0,0,
F,F,0,0,0,0,0,0,
GF,GF,0,0,0,0,0,0,
DF,DF,DF,DF,DF,DF,0,0};

uint8_t voiceidx; // index of a current voice
uint8_t notesidx; // index of a current note
uint8_t playsong; // title of the current song


SONG song1;
SONG song2;
SONG song3;

// Initialize Song structs
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

