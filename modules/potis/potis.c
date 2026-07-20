/**
 ******************************************************************************
 * @file           : potis.c
 * @author         : Dimitry Ntofeu Nyatcha
 * @date           : 27.04.2025
 * @brief          : Modul zur Verwendung von zwei Potentiometern über ADC1.
 *
 * Dieses Modul stellt grundlegende Funktionen zum Initialisieren des ADCs
 * sowie zum Auslesen und Umrechnen der Analogwerten der beiden Potentiometer bereit.
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
     -> Wandelt den ADC-Wert in Millivolt um.

 Hinweis:
 Die Funktion nutzt keinen DMA, sondern arbeitet mit Polling-Modus.
 ==============================================================================
@endverbatim
 ******************************************************************************
 */

/*Include*/
#include "stm32f4xx.h"
#include "potis/potis.h"
#include "median/median.h"


/*globale variable für ADC-Konfiguration*/
 static ADC_HandleTypeDef ADC_handle_structure;

 /**
  * @brief  Initialisiert ADC1 und die GPIO-Pins PA6 und PA7.
  *         Konfiguriert den ADC für eine einzelne Konvertierung.
  */
 void potis__ADC_init(void){

	 __HAL_RCC_ADC1_CLK_ENABLE(); // versorgt die Instace ADC1 vom ADC mit einem Takt bzw Strom
	 __HAL_RCC_GPIOA_CLK_ENABLE(); // versorgt Port A mit einem Takt

	 //GPIO Konfiguration für analoge Signale
	 GPIO_InitTypeDef gpio_init_struct;

	 	gpio_init_struct.Pin = ADC_1 | ADC_2;
	 	gpio_init_struct.Mode = GPIO_MODE_ANALOG;
	 	gpio_init_struct.Pull = GPIO_NOPULL;
	 	gpio_init_struct.Speed = GPIO_SPEED_MEDIUM;

	 HAL_GPIO_Init(ADC_PORT, &gpio_init_struct);


	 /*Konfiguration des ADCs*/
	    ADC_handle_structure.Instance = ADC1;
	    ADC_handle_structure.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	    ADC_handle_structure.Init.Resolution = ADC_RESOLUTION_12B;
	    ADC_handle_structure.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	    ADC_handle_structure.Init.ScanConvMode = DISABLE; // ein Kanal pro Konvertierung
	    ADC_handle_structure.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	    ADC_handle_structure.Init.ContinuousConvMode = DISABLE;
	    ADC_handle_structure.Init.NbrOfConversion = 1;
	    ADC_handle_structure.Init.ExternalTrigConv = ADC_SOFTWARE_START;


	 HAL_ADC_Init(&ADC_handle_structure);


 }

 /**
  * @brief  Führt eine ADC-Wandlung auf dem angegebenen Kanal durch (PA6 oder PA7).
  * @param  poti_num: Kanalnummer -> ADC_chanel_6 (PA6) oder ADC_chanel_7 (PA7)
  * @retval ADC-wert zwischen 0 und 4095
  */
 uint32_t potis_ADC_get_val(uint8_t poti_num){

	 ADC_ChannelConfTypeDef ADC_channel_structure;

	 /*Auswahl der richtigen Kanal*/
	 if(poti_num == ADC_chanel_7){
		 ADC_channel_structure.Channel = ADC_CHANNEL_7;
	 }
	 else if(poti_num == ADC_chanel_6){
		 ADC_channel_structure.Channel = ADC_CHANNEL_6;
	 }
	 else {
		 return 0;
	 }

	 ADC_channel_structure.Rank = 1;
	 ADC_channel_structure.SamplingTime = ADC_SAMPLETIME_84CYCLES;

	 /*Konfiguration der Kanal*/
	 HAL_ADC_ConfigChannel(&ADC_handle_structure, &ADC_channel_structure);

	 /*Konversion starten*/
	 HAL_ADC_Start(&ADC_handle_structure);

	 /*Warten bis die Konvertierung fertig ist*/

	 if( HAL_ADC_PollForConversion(&ADC_handle_structure, 1000) != HAL_TIMEOUT){

		 uint32_t adc_wert = HAL_ADC_GetValue(&ADC_handle_structure); // speichert den ADC-WERT
		 return adc_wert;
	 }

	 //Falls = HAL_TIMEOUT, 0 als Error zurückgeben
	 return 0;
 }


 /**
  * @brief  Führt eine ADC-Wandlung durch und rechnet den ADC-wert in Millivolt um.
  * @param  poti_num: Kanalnummer (ADC_chanel_6 oder ADC_chanel_7)
  * @retval Spannung in mV (wobei Vref = 3300 mV)
  */
 uint32_t potis_ADC_get_val_mv(uint8_t poti_num){

	 uint32_t adc_wert = potis_ADC_get_val(poti_num);

	 uint32_t mv_val = (POTIS_VREF_MV * adc_wert) / POTIS_MAX_ADC_WERT;

	 return mv_val;
 }
