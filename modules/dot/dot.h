/**
 * @file dot.h
 * @author Dimitry Ntofeu Nyatcha
 * @brief Modul zur Verwaltung des DOT-Segments mit PWM-Steuerung und zur Steuerung von einem Stopuhr
 */

#ifndef DOT_DOT_H_
#define DOT_DOT_H_

#include "stm32f4xx_hal.h"

//CNTL1-4 PORTD
#define CNTL1 GPIO_PIN_14
#define CNTL2 GPIO_PIN_15
#define CNTL3 GPIO_PIN_0
#define CNTL4 GPIO_PIN_1

// SEG A-F -> PORTD
#define SEG_A GPIO_PIN_7
#define SEG_B GPIO_PIN_4
#define SEG_C GPIO_PIN_5
#define SEG_D GPIO_PIN_6
#define SEG_E GPIO_PIN_12
#define SEG_F GPIO_PIN_11

#define ALLE_CNTL (CNTL1 | CNTL2 | CNTL3 | CNTL4) // Kontrol-leitungen für die 7Segments
#define ALLE_SEG_OHNE_G (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F)

// PORTE
#define DOT (GPIO_PIN_11)
#define SEG_G (GPIO_PIN_12)

// PORTA
#define USER_TASTE (GPIO_PIN_0)
#define USER_PORT (GPIOA)
//joypin
#define JOY_PIN_PRESS       (GPIO_PIN_12)
#define JOY_GPIO_PORT       (GPIOG)


#define DOT_TIMER_FREQ_HZ       10000U    // Timer Frequenz 10 kHz
#define POTIS_VREF_MV 3300U // referenz Spannung

#define MAXIMALE_RUNDEN 6 // Anzahl erlaubten Rundenzeiten kann verändert werden

/**
 * Externe Variablen
 */
extern volatile uint32_t zeitstand; // aktuelle zähler
extern volatile uint32_t last_zeitstand; // letzer Zähler-Wert
extern volatile uint8_t chrono_laeuft; //Zeigt an, ob die Stoppuhr läuft
extern volatile uint32_t runden_zeiten[MAXIMALE_RUNDEN]; // Aufgezeichnete Zeiten für jede Runde
extern volatile uint8_t runde_index; // index aktueller Runde
extern volatile uint32_t startzeit; // Startzeit der Stoppuhr
extern volatile uint32_t runden_startzeit; // startzeit für jede Runde
extern volatile uint8_t taste_gedrueckt; // sagt ob die button gedrückt ist

extern TIM_HandleTypeDef tim_handle_struct;

/**
 * Public Funktionen
 */
void dot_gpio_init();

void blink_dot_timer_init();
void blink_dot_oc_init();
void set_blink_Dot_freq(int poti_num, uint32_t mv);
int get_freq();

void dimming_dot_timer_init();
void dimming_dot_oc_init();
void dimming_intensitat_Dot();

void treppenhaus_init();


void stopwatch_timer_init();
void stopwatch_gpio_init();
void zeit_anzeigen_at(uint32_t total_centiemes, uint16_t x, uint16_t y);
void dessine_boutons();
void gesamtdauer_anzeigen();
void taste_verarbeitung();
#endif /* DOT_DOT_H_ */
