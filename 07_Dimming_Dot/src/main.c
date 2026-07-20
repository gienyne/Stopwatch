/**
 * @file main.c
 * @author Dimitry Ntofeu Nyatcha
 * @brief Modul zur Verwaltung der DOT-Segments helligkeit
 */

#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include  <stdio.h>
#include "dot/dot.h"
#include "potis_DMA/potis_DMA.h"
#include "my_lcd/my_lcd.h"


int main(void)
{
	HAL_Init();

	lcd_init();

	potis_DMA_init();

	potis_ADC_init();

	dot_gpio_init();

	dimming_dot_timer_init();

	dimming_dot_oc_init();

	char buffer[32];

	while(1) {



		uint32_t mv = potis_DMA_get_average_val_mv(0);
		uint32_t percent = (mv * 100) / 3300;

		dimming_intensitat_Dot(0);
		snprintf(buffer, sizeof(buffer), "percent: %3lu%%", percent);
		lcd_draw_text_at_line(buffer, 2, BLACK, 2, WHITE);

		 my_lcd_draw_baargraph(10, 180, 200, 20, (mv * 1000)/3300, RED, YELLOW);

	}
}
