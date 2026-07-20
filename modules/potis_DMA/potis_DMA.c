/**
 **************************************************
 * @file        potis_DMA.c
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

#include "stm32f4xx.h"
#include "potis_DMA/potis_DMA.h"
#include "potis/potis.h"
#include "median/median.h"

/*globale Variablen*/
static DMA_HandleTypeDef DMA_handle_structure;
static ADC_HandleTypeDef ADC_handle_structure;

//uint16_t sample[] = {0, 0};
uint16_t ring_puffer[RING_PUFFER_SIZE * 2]; // für die 2 kanäle

/**
 * @brief Initialisiert ADC1 mit zwei Kanälen für die Potis und startet DMA-Übertragung.
 */
void potis_ADC_init(void){

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
	    ADC_handle_structure.Init.ScanConvMode = ENABLE; // ein Kanal pro Konvertierung
	    ADC_handle_structure.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	    ADC_handle_structure.Init.ContinuousConvMode = ENABLE;
	    ADC_handle_structure.Init.NbrOfConversion = 2;
	    ADC_handle_structure.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	    ADC_handle_structure.DMA_Handle = &DMA_handle_structure;
	    ADC_handle_structure.Init.DMAContinuousRequests = ENABLE;

	 HAL_ADC_Init(&ADC_handle_structure);

	  ADC_ChannelConfTypeDef ADC_channel7_structure;
	    ADC_channel7_structure.Channel = ADC_CHANNEL_7;
	    ADC_channel7_structure.Rank = 1;
	    ADC_channel7_structure.SamplingTime = ADC_SAMPLETIME_84CYCLES;

	 HAL_ADC_ConfigChannel(&ADC_handle_structure, &ADC_channel7_structure);

	  ADC_ChannelConfTypeDef ADC_channel6_structure;
	    ADC_channel6_structure.Channel = ADC_CHANNEL_6;
		ADC_channel6_structure.Rank = 2;
		ADC_channel6_structure.SamplingTime = ADC_SAMPLETIME_84CYCLES;

     HAL_ADC_ConfigChannel(&ADC_handle_structure, &ADC_channel6_structure);


     HAL_ADC_Start_DMA(&ADC_handle_structure, (uint32_t*)ring_puffer,  RING_PUFFER_SIZE * 2);


 }

/**
 * @brief Initialisiert den DMA-Stream zur Übertragung von ADC1-Daten in den Speicher (Ringpuffer).
 */
void potis_DMA_init(void){

	  /*Strom versorgunt*/
	 //__HAL_RCC_DMA1_CLK_ENABLE();
	 __HAL_RCC_DMA2_CLK_ENABLE();

	//Konfiguration des DMA-Controllers für ADC1
	DMA_handle_structure.Instance = DMA2_Stream4;
	DMA_handle_structure.Init.Channel = DMA_CHANNEL_0;
	DMA_handle_structure.Init.Direction = DMA_PERIPH_TO_MEMORY;;
	DMA_handle_structure.Init.PeriphInc = DMA_PINC_DISABLE; // a revoir
	DMA_handle_structure.Init.MemInc = DMA_MINC_ENABLE; // für das Array
	DMA_handle_structure.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	DMA_handle_structure.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	DMA_handle_structure.Init.Mode = DMA_CIRCULAR;
	DMA_handle_structure.Init.Priority = DMA_PRIORITY_HIGH; // a revoir
	DMA_handle_structure.Init.FIFOMode = DMA_FIFOMODE_DISABLE;


 HAL_DMA_Init(&DMA_handle_structure);

}


/**
 * @brief Gibt den Mittelwert der letzten RING_PUFFER_SIZE Messungen eines Potis in mV zurück.
 * @param poti_num  0 für ADC_CHANNEL_7 , 1 für ADC_CHANNEL_6
 * @return Mittelwert in Millivolt (mV)
 */
uint32_t potis_DMA_get_average_val_mv(uint8_t poti_num){

	uint32_t sum = 0;
	uint32_t mittelwert = 0;


	for(int i = 0; i < RING_PUFFER_SIZE ; i++){
		sum += ring_puffer[i * 2 + poti_num];
	}

	mittelwert = (sum / (RING_PUFFER_SIZE));



	return (uint32_t)((mittelwert * POTIS_VREF_MV) /  POTIS_MAX_ADC_WERT);
}
















/*Nicht sehr wichtig denn funktioniert nicht über den Mittelwert will trotzdem nicht löschen
 * denn stellt eine andere Lösungsmöglichkeit dar*/

/*uint32_t potis_DMA_get_val_mv(uint8_t poti_num){

	uint32_t adc_wert = potis_DMA_get_val(poti_num);
	uint32_t mv_wert = ((adc_wert * POTIS_VREF_MV) /  POTIS_MAX_ADC_WERT);

	return mv_wert;

}
 * */
/*uint32_t potis_DMA_get_val(uint8_t poti_num){

	if (poti_num == ADC_chanel_7){
		return sample[0];
	}
	else if(poti_num == ADC_chanel_6){
		return sample[1];
	}
	else return;

}
 * */

