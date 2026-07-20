/**
 * @file main.c
 * @author Dimitry Ntofeu Nyatcha
 * @brief  Funktion zur Steuerung von einem Stopuhr
 */


#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "utils/utils.h"
#include "dot/dot.h"
#include <stdio.h>

/* denken Sie daran in die Datei : stm32f4xx_it.c von Stopwatch sowie in die dot.c die callbackfunktion:
 * EXTI0_IRQHandler bzw void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) und die HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 * sowie TIM1_UP_TIM10_IRQHandler(void) in die stm32f4xx_it.c von Stopwatch   Auszukommentieren
 * aber dann die in Fan kommentieren denn beide werden auch in die P1_Fan.c implemetiert bzw aufgerunfen
 * und das Problem ist, dass es zu fehler multiple definition of.... wenn man das mehrmals implemetiert
 *
 * Also einfach auf Callbackfunktion von funktion in der Source datei selbst und in der stm32f4xx_it.c der jeweilligen datei achten

 */
int main(void)
{
	    HAL_Init();

	    lcd_init();
	    stopwatch_gpio_init();
	    stopwatch_timer_init();


	    lcd_draw_text_at_coord("Chrono Ready", 10, 10, BLACK, 2, WHITE);

	    dessine_boutons();



	    while (1) {

	    	taste_verarbeitung();
	    	gesamtdauer_anzeigen();

	    }
}


