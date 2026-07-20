/**
 **************************************************
 * @file        my_lcd.h
 * @author      Dimitry Ntofeu Nyatcha
 * @version     v1.0
 * @date        20.04.2025
 **************************************************
 */


#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "my_lcd/my_lcd.h"
#include  <stdio.h>
#include "utils/utils.h"


int main(void)
{
	HAL_Init();

	/* Initialization of the LCD */
	lcd_init();


	while(1) {

		char buf[16];

		    for (int i = 10; i >= 0; i--) {

		        snprintf(buf, sizeof(buf), "Count: %2d", i);
		        lcd_draw_text_at_line(buf, 2, WHITE, 2, BLACK);


		        uint16_t promille = i * 100;
		        my_lcd_draw_baargraph(20, 200, 200, 20, promille, RED, GREEN);

		        utils_delay_ms(1000);
		    }

		    // Kreuz zeichnen
		    draw_diag_cross(40, 80 , BLACK);
	}
}
