/**
  ******************************************************************************
  * @file    	main.c
  * @author		Dimitry Ntofeu Nyatcha
  * @version 	V1.0
  * @date		20.04.2025
  ******************************************************************************
*/


#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "joystick/joystick.h"
#include "esd/esd.h"
#include "utils/utils.h"

int main(void)
{
	HAL_Init();
	esd_init();
	joystick_init();

	esd_digit_t digit_zahl = 0;
	esd_position_t position = 0;

	//esd_show_digit(digit_zahl, ESD_POSITION_1);


	while(1) {

		 if((utils_gpio_port_read(JOY_GPIO_PORT) & JOY_PIN_UP ) == 0){
			digit_zahl = (digit_zahl + 1) % (ESD_DIGIT_9 + 1);
			//esd_show_digit(digit_zahl, ESD_POSITION_1);
		}

		else if((utils_gpio_port_read(JOY_GPIO_PORT) & JOY_PIN_DOWN ) == 0){
			digit_zahl = (digit_zahl + ESD_DIGIT_9) % (ESD_DIGIT_9 + 1);
		}

		else if((utils_gpio_port_read(JOY_GPIO_PORT) & JOY_PIN_RIGHT) == 0){
					position = (position + 1) % (ESD_POSITION_4 + 1);
				}

		else if((utils_gpio_port_read(JOY_GPIO_PORT) & JOY_PIN_LEFT) == 0){
							position = (position + ESD_POSITION_4) % (ESD_POSITION_4 + 1);
						}

		else if((utils_gpio_port_read(JOY_GPIO_PORT) & JOY_PIN_PRESS) == 0){
			int zaehler = digit_zahl;
			        while(zaehler >= 0){
							esd_show_digit(zaehler, position);
							zaehler--;
							utils_delay_ms(1000);

			}

						}

		 esd_show_digit(digit_zahl, position);
		 		utils_delay_ms(200);
	}
}
