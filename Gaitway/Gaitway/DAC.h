// Lab 5 - Music player and Audio amp
// Hyejun Im (hi956), Sarthak Gupta(sg38368)
// TA: William Glanton


#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>


//********DAC_Init*****************
// Initialize Max5353 12-bit DAC
// inputs:  initial voltage output (0 to 4095)
// outputs: none
// assumes: system clock rate less than 20 MHz
void DAC_Init(uint16_t data);

//********DAC_Out*****************
// Send data to Max5353 12-bit DAC
// inputs:  voltage output (0 to 4095)
// outputs: none
void DAC_Out(uint16_t code);            // data out, no reply
  
//********DAC_Out*****************
// Send data to Max5353 12-bit DAC
// inputs:  voltage output (0 to 4095)
// outputs: reply is returned
// send the 16-bit code to the SSI, return a reply
uint16_t DAC_Out2(uint16_t code);

void DAC_InitRaw(void);
//********DAC_Out*****************
// Send data to Max5353 12-bit DAC
// inputs:  voltage output (0 to 4095)
// outputs: none
// send the 16-bit code to the SSI, 
void DAC_OutRaw(uint16_t code);

#define WAVE_SIZE 64
#define NOTE_SIZE 248
//#define NOTE_SIZE 1

#define EIGHTH 4000000 //12500000



extern const unsigned short* currentMusic;

// waves
extern unsigned short Bassoon[WAVE_SIZE];
extern unsigned short Oboe[WAVE_SIZE];
extern unsigned short Trumpet[WAVE_SIZE];
extern unsigned short Flute[WAVE_SIZE];
extern unsigned short Sin[WAVE_SIZE];
extern  unsigned short Horn[WAVE_SIZE];

extern unsigned short ForLab[WAVE_SIZE];

// notes
extern unsigned short Note[NOTE_SIZE] ;
extern unsigned short Note1[NOTE_SIZE];
extern unsigned short Note2[NOTE_SIZE];
extern unsigned short Note3[NOTE_SIZE];


extern uint8_t voiceidx; // index of a current voice
extern uint8_t notesidx; // index of a current note
extern uint8_t playsong; // title of the current song

typedef struct {
    int title; // use enum playlist for this
    
    unsigned short wave[WAVE_SIZE]; // wave form
    
    unsigned short notes[NOTE_SIZE]; // frequency of a song {'A', 'B', 'D'}
    
}SONG;

extern SONG song1;
extern SONG song2;
extern SONG song3;
    
// Initialize Song structs
void initSong(SONG* song, int stitle, unsigned short swave[WAVE_SIZE], unsigned short snotes[NOTE_SIZE]);



#define BREAK 0 // rest
#define C_2 3822   // 65.406 Hz
#define DF_1 3608   // 69.296 Hz
#define D_1 3405   // 73.416 Hz
#define EF_1 3214   // 77.782 Hz
#define E_1 3034   // 82.407 Hz
#define F_1 2863   // 87.307 Hz
#define GF_1 2703   // 92.499 Hz
#define G_1 2551   // 97.999 Hz
#define AF_1 2408   // 103.826 Hz
#define A_1 2273   // 110.000 Hz
#define BF_1 2145   // 116.541 Hz
#define B_1 2025   // 123.471 Hz
#define C_1 1911   // 130.813 Hz
#define DF0 1804   // 138.591 Hz
#define D0 1703   // 146.832 Hz
#define EF0 1607   // 155.563 Hz
#define E0 1517   // 164.814 Hz
#define F0 1432   // 174.614 Hz
#define GF0 1351   // 184.997 Hz
#define G0 1276   // 195.998 Hz
#define AF0 1204   // 207.652 Hz
#define A0 1136   // 220.000 Hz
#define BF0 1073   // 233.082 Hz
#define B0 1012   // 246.942 Hz
#define C0 956   // 261.626 Hz
#define DF 902   // 277.183 Hz
#define D 851   // 293.665 Hz
#define EF 804   // 311.127 Hz
#define E 758   // 329.628 Hz
#define F 716   // 349.228 Hz
#define GF 676   // 369.994 Hz
#define G 638   // 391.995 Hz
#define AF 602   // 415.305 Hz
#define A 568   // 440.000 Hz
#define BF 536   // 466.164 Hz
#define B 506   // 493.883 Hz
#define C 478   // 523.251 Hz
#define DF1 451   // 554.365 Hz
#define D1 426   // 587.330 Hz
#define EF1 402   // 622.254 Hz
#define E1 379   // 659.255 Hz
#define F1 358   // 698.456 Hz
#define GF1 338   // 739.989 Hz
#define G1 319   // 783.991 Hz
#define AF1 301   // 830.609 Hz
#define A1 284   // 880.000 Hz
#define BF1 268   // 932.328 Hz
#define B1 253   // 987.767 Hz
#define C1 239   // 1046.502 Hz
#define DF2 225   // 1108.731 Hz
#define D2 213   // 1174.659 Hz
#define EF2 201   // 1244.508 Hz
#define E2 190   // 1318.510 Hz
#define F2 179   // 1396.913 Hz
#define GF2 169   // 1479.978 Hz
#define G2 159   // 1567.982 Hz
#define AF2 150   // 1661.219 Hz
#define A2 142   // 1760.000 Hz
#define BF2 134   // 1864.655 Hz
#define B2 127   // 1975.533 Hz
#define C2 119   // 2093.005 Hz



