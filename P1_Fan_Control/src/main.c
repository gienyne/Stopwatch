/*
 * P1_Fan.c
 *
 *  Created on: May 27, 2025
 *      Author: Dimitry Ntofeu nyatcha, Sidal Güray
 */




#include "stm32f4xx.h"
#include <lcd/lcd.h>
#include "P1_Fan/P1_Fan.h"
#include <stdio.h>
#include "potis_DMA/potis_DMA.h"
#include "my_lcd/my_lcd.h"


int main (void){

	 HAL_Init();

	 potis_DMA_init();
	 potis_ADC_init();

	 gpio_fan_init();
	 timer_init();
	 fan_oc_init();
	 gpio_tacho_init();
	 timer_rpm_init();
	 Ta_pid_timer_init();
	 lcd_init();

	 char ist_Wert[64];
	 char soll_Wert[64];
	 char buffer[32];

	 while (1) {
	    if (pi_update_flag) {
	        pi_update_flag = 0;
	        pi_regler_update();

	        uint32_t soll = fan_get_soll_wert();
	        uint32_t ist = fan_get_speed();

	        sprintf(soll_Wert, "Soll Wert: %4ld", soll);
	        lcd_draw_text_at_line(soll_Wert, 1, BLACK, 2, WHITE);

	        sprintf(ist_Wert, "Ist Wert: %4ld", ist);
	        lcd_draw_text_at_line(ist_Wert, 5, BLACK, 2, WHITE);

	        // Konvertieren von Soll und Ist Werte in promill
	        uint32_t MAX_RPM = 4000;
	        uint16_t soll_promille = (soll > MAX_RPM) ? 1000 : (soll * 1000) / MAX_RPM;
	        uint16_t ist_promille  = (ist  > MAX_RPM) ? 1000 : (ist  * 1000) / MAX_RPM;

	        // Darstellen von Soll und Ist Werte als Bargraph
	        my_lcd_draw_baargraph(10, 70, 200, 20, soll_promille, RED, BLACK);
	        my_lcd_draw_baargraph(10, 135, 200, 20, ist_promille, BLUE, BLACK);


	        sprintf(buffer, "ADC Wert = %lu mV", potis_DMA_get_average_val_mv(0));
	        lcd_draw_text_at_line(buffer, 10, BLACK, 2, WHITE);
	       }
	  }
}
