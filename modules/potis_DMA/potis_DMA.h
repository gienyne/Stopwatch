/**
 **************************************************
 * @file        potis_DMA.h
 * @author      Dimitry Ntofeu Nyatcha
 * @version     v1.0
 * @date        29.04.2025
 * @brief       Modul zur Erfassung von Potentiometerwerten über ADC mit DMA und Mittelwertbildung.
 *
 * @verbatim
 * ==================================================
 * ### Verwendete Ressourcen ###
 * GPIO:  PA6 (ADC12_IN6), PA7 (ADC12_IN7)
 * ADC:   ADC1 (12-Bit Auflösung, 2 Kanäle)
 * DMA:   DMA2, Stream4, Channel 0
 *
 * ### Beschreibung ###
 * Dieses Modul liest die Werte zweier Potentiometer kontinuierlich per DMA aus.
 * Die ADC-Werte werden in einem Ringpuffer gespeichert. Mittels Mittelwertbildung
 * über 100 Messwerte werden stabilere Ergebnisse erzielt.
 *
 * Funktionen:
 *  - potis_DMA_init()        : Initialisiert den DMA für den ADC
 *  - potis_ADC_init()        : Initialisiert ADC1 und GPIOs
 *  - potis_DMA_get_average_val_mv() : Gibt Mittelwert (in mV) eines Potis zurück
 * ==================================================
 * @endverbatim
 **************************************************
 */
#ifndef POTIS_DMA_POTIS_DMA_H_
#define POTIS_DMA_POTIS_DMA_H_

#include "stm32f4xx.h"

#define RING_PUFFER_SIZE 100

/*Initialisiert den DMA für den ADC*/
void potis_DMA_init(void);

/* Initialisiert ADC1 und GPIOs*/
void potis_ADC_init(void);

/*Gibt Mittelwert (in mV) eines Potis zurück*/
uint32_t potis_DMA_get_average_val_mv(uint8_t poti_num);

//uint32_t potis_DMA_get_val_mv(uint8_t poti_num);
//uint32_t potis_DMA_get_val(uint8_t poti_num);
#endif /* POTIS_DMA_POTIS_DMA_H_ */
