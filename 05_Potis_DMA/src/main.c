/**
 **************************************************
 * @file        main.c
 * @author      Dimitry Ntofeu Nyatcha
 * @version     v1.0
 * @date        27.04.2025
 * @brief       Ablesen von Potentiometern auf Basis des DMA-Prinzips und Anzeige auf LCD
 **************************************************
 */

#include <stdio.h>
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "my_lcd/my_lcd.h"
#include "potis/potis.h"
#include "potis_DMA/potis_DMA.h"
#include "utils/utils.h"

int main(void){

	HAL_Init();
	lcd_init();

	potis_DMA_init();
	potis_ADC_init();

	uint32_t val_Adc1_mv;
	uint32_t val_Adc2_mv;

	char buf[64];
	while(1){

		val_Adc1_mv = potis_DMA_get_average_val_mv(ADC_chanel_7);
		val_Adc2_mv = potis_DMA_get_average_val_mv(ADC_chanel_6);

		        snprintf(buf, sizeof(buf), "Poti1 = %lu mV", (val_Adc1_mv * 3300) / 4095);
		        lcd_draw_text_at_line(buf, 2, BLACK, 2, WHITE);

		        snprintf(buf, sizeof(buf), "Poti2 = %lu mV", (val_Adc2_mv * 3300) / 4095);
		        lcd_draw_text_at_line(buf, 3, BLACK, 2, WHITE);

		        // Bargraph (entre 0 et 1000)
		        my_lcd_draw_baargraph(10, 180, 200, 20, (val_Adc1_mv * 1000) / 3300, RED, YELLOW);
		        my_lcd_draw_baargraph(10, 220, 200, 20, (val_Adc2_mv * 1000) / 3300, BLUE, YELLOW);

		       // HAL_Delay(100);

	}

}
