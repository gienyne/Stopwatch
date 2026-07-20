/**
 ******************************************************************************
 * @file           : potis.h
 * @author         : Dimitry Ntofeu Nyatcha
 * @date           : 27.04.2025
 * @brief          : Modul zur Verwendung von zwei Potentiometern über ADC1.
 *
 *
 * @verbatim
 ==============================================================================
 Ressourcen:
 ------------------------------------------------------------------------------
 GPIO: PA6 (ADC12_IN6) und PA7 (ADC12_IN7)
 ADC : ADC1, 12-Bit Auflösung
 ------------------------------------------------------------------------------
 Funktionen:
 ------------------------------------------------------------------------------
 (#) potis__ADC_init(void)
     -> Initialisiert ADC1 und die GPIOs für die Potentiometer.

 (#) potis_ADC_get_val(uint8_t poti_num)
     -> Führt eine Wandlung durch und gibt den ADC-wert zurück.

 (#) potis_ADC_get_val_mv(uint8_t poti_num)
     -> Wandelt den ADC-Wert  in Millivolt um.

 Hinweis:
 Die Funktion nutzt keinen DMA, sondern arbeitet mit Polling-Modus.
 ==============================================================================
@endverbatim
 ******************************************************************************
 */

#ifndef POTIS_POTIS_H_
#define POTIS_POTIS_H_

/*HAL-Bibliothek für STM32*/
#include "stm32f4xx.h"

/* Define */
#define ADC_1 GPIO_PIN_7
#define ADC_2 GPIO_PIN_6

#define ADC_PORT GPIOA

#define ADC_chanel_7  0 // Index für Poti an PA7
#define ADC_chanel_6  1 // Index für Poti an PA6

#define POTIS_MAX_ADC_WERT  4095U    // Maximaler Wert für 12-bit ADC
#define POTIS_VREF_MV        3300U    // 3.3V reference Spannung in millivolts

/**
 * @brief Initialisiert ADC1 sowie die zugehörigen GPIO-Pins
 */
void potis__ADC_init(void);

/**
 * @brief Liest den aktuellen ADC-Wert eines Potentiometers
 * @param poti_num  Interner Kanalindex (0 für PA7, 1 für PA6)
 * @return Wert zwischen 0 und 4095
 */
uint32_t potis_ADC_get_val(uint8_t poti_num);

/**
 * @brief Gibt den ADC-Wert eines Potentiometers in Millivolt zurück
 * @param poti_num  Interner Kanalindex (0 für PA7, 1 für PA6)
 * @return Wert in Millivolt (0 - 3300 mV)
 */
uint32_t potis_ADC_get_val_mv(uint8_t poti_num);


#endif /* POTIS_POTIS_H_ */
