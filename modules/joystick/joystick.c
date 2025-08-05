/**
 **************************************************
 * @file        joystick.c
 * @author      Dimitry Ntofeu Nyatcha
 * @version     v1.0
 * @date        20.04.2025
 * @brief       Modul zur Initialisierung des Joysticks
 @verbatim
 ==================================================
            ### Verwendete Ressourcen ###
    GPIO -> (PORTG): PG6, PG9, PG10, PG11, PG12 (Joystick Pins)
 ==================================================
            ###     Verwendung      ###
    (#) Aufruf von 'joystick_init()', um die Joystick-Ports als Eingänge zu initialisieren.
 ==================================================
 @endverbatim
 **************************************************
 */

/**
 * Include
 */
#include "stm32f4xx.h"
#include "joystick.h"

/**
 * @brief  Initialisiert die GPIOs-PORT bzw PIN für die Joystick-Eingaben
 * @param  Keine
 * @return Keine
 */
void joystick_init(void){

	__HAL_RCC_GPIOG_CLK_ENABLE();

	GPIO_InitTypeDef joystick_gpio_init;
	joystick_gpio_init.Pin = JOY_ALL_DIRECTION;
	joystick_gpio_init.Mode = GPIO_MODE_INPUT;
	joystick_gpio_init.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(GPIOG, &joystick_gpio_init);

}



