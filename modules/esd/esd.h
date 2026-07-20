/**
 **************************************************
 * @file        esd.h
 * @author      Dimitry Ntofeu Nyatcha
 * @version     v1.0
 * @date        15.04.2025
 * @brief       Header-Datei für das Modul zur Steuerung des 8-Segment-Displays
 **************************************************
 */
#ifndef ESD_ESD_H_
#define ESD_ESD_H_

/* Includes */
#include "stm32f4xx.h" //Header Datei der STM32Cube HAL

/*Public define für 7-segments (Also 8 wenn man dazu noch der Punkt zählt)*/
// SEG A-F -> PORTD
#define SEG_A GPIO_PIN_7
#define SEG_B GPIO_PIN_4
#define SEG_C GPIO_PIN_5
#define SEG_D GPIO_PIN_6
#define SEG_E GPIO_PIN_12
#define SEG_F GPIO_PIN_11
#define ALLE_SEG_OHNE_G (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)

//CNTL1-4 PORTD
#define CNTL1 GPIO_PIN_14
#define CNTL2 GPIO_PIN_15
#define CNTL3 GPIO_PIN_0
#define CNTL4 GPIO_PIN_1

#define ALLE_CNTL (CNTL1 | CNTL2 | CNTL3 | CNTL4)

// PORTE
#define POINT GPIO_PIN_7
#define DOT GPIO_PIN_11
#define SEG_G GPIO_PIN_12

/* define für die Ziffer
 * Aus Grund Unterschiedlichem PORT wird SEG_G Bei der Implementierung von esd_show_digit(esd_digit_t digit, esd_position_t pos)  nocht später hinzugefügt
 * */
#define DIGIT_0 (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)
#define DIGIT_1 (SEG_B | SEG_C)
#define DIGIT_2_OHNE_G (SEG_A | SEG_B | SEG_E | SEG_D) //g
#define DIGIT_3_OHNE_G (SEG_A | SEG_B | SEG_C | SEG_D) //g
#define DIGIT_4_OHNE_G (SEG_F | SEG_C | SEG_B) //g
#define DIGIT_5_OHNE_G (SEG_A | SEG_F | SEG_C | SEG_D) //g
#define DIGIT_6_OHNE_G (SEG_A | SEG_F | SEG_E | SEG_D | SEG_C) //g
#define DIGIT_7 (SEG_A | SEG_B | SEG_C)
#define DIGIT_8_OHNE_G (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F) //g
#define DIGIT_9_OHNE_G (SEG_A | SEG_B | SEG_F | SEG_C | SEG_D) //g


/**
 * @brief Enumeration für die Zahlen (0 bis 9)
 */
typedef enum {
 ESD_DIGIT_0,
 ESD_DIGIT_1,
 ESD_DIGIT_2,
 ESD_DIGIT_3,
 ESD_DIGIT_4,
 ESD_DIGIT_5,
 ESD_DIGIT_6,
 ESD_DIGIT_7,
 ESD_DIGIT_8,
 ESD_DIGIT_9
 } esd_digit_t;

 /**
  * @brief Enumeration für die Positionen auf dem Display (1 bis 4 oder alle)
  */
 typedef enum {
 ESD_POSITION_1,
 ESD_POSITION_2,
 ESD_POSITION_3,
 ESD_POSITION_4,
 ESD_POSITION_ALL
 } esd_position_t;

 /*Prototypen von Public Funktionen */
 void esd_init(void);
 void esd_show_digit(esd_digit_t digit, esd_position_t pos);

#endif /* ESD_ESD_H_ */
