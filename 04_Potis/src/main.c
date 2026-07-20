/**
 **************************************************
 * @file        main.c
 * @author      Dimitry Ntofeu Nyatcha
 * @version     v1.0
 * @date        27.04.2025
 * @brief       Ablesen von Potentiometern und Anzeige auf LCD
 **************************************************
 */

#include <stdio.h>
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "my_lcd/my_lcd.h"
#include "potis/potis.h"
#include "utils/utils.h"


int main(void)
{
	HAL_Init();
    lcd_init();
    potis__ADC_init();

    char buffer[64];
    uint32_t val_poti_chanel7;
    uint32_t val_poti_chanel6;

	while(1) {

		 val_poti_chanel7 = potis_ADC_get_val_mv(0);
		 val_poti_chanel6 = potis_ADC_get_val_mv(1);

		//lcd_fill_screen(WHITE);

		// 3. Afficher les valeurs en mV
		        sprintf(buffer, "Poti1: %lu mV", val_poti_chanel7);
		        lcd_draw_text_at_line(buffer, 2, BLACK, 2, WHITE);

		        sprintf(buffer, "Poti2: %lu mV", val_poti_chanel6);
		        lcd_draw_text_at_line(buffer, 4, BLACK, 2, WHITE);

		        // 4. Dessiner les bargraphs
		        my_lcd_draw_baargraph(10, 150, 200, 20, (val_poti_chanel7 * 1000) / 3300, BLACK, YELLOW);
		        my_lcd_draw_baargraph(10, 200, 200, 20, (val_poti_chanel6 * 1000) / 3300, BLUE, YELLOW);


	}
}

