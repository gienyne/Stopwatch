/**
 * @file main.c
 * @author Dimitry Ntofeu Nyatcha
 * @brief  Funktion zur Steuerung der Blink-Frequenz des Dot-Segments
 */

#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include  <stdio.h>
#include "dot/dot.h"
#include "potis_DMA/potis_DMA.h"
#include "my_lcd/my_lcd.h"



int main(void){

	HAL_Init();

    lcd_init();

	potis_DMA_init();

	potis_ADC_init();

	dot_gpio_init();

	blink_dot_timer_init();

	blink_dot_oc_init();


	char buffer[32];


	while(1) {

		uint32_t mv = potis_DMA_get_average_val_mv(0);

		set_blink_Dot_freq(0, mv);
		snprintf(buffer, sizeof(buffer), "freq: %d mili_hz", get_freq());
		 lcd_draw_text_at_line(buffer, 2, BLACK, 2, WHITE);

	}
}



