/*
 *
 * HiT0M1 Pin Config File
 *  
*/
#ifndef PINCONFIG
#define PINCONFIG

// 74HC595
#define SRCLK       (13)
#define RCLK        (10)
#define SER         (11)

// 74HC139
#define CS_A        (14)
#define CS_B        (15)
#define CS_E        (8)

#define WR_A        (16)
#define WR_B        (17)
#define WR_E        (9)
// Tone Generator
#define IC_PIN      (18)
#define A0_PIN      (19)

// Status LEDs
#define PSG1_LED    (3)    
#define PSG2_LED    (4)
#define PSG3_LED    (5)
#define FMR_LED     (6)
#define FMM_LED     (7)

#endif