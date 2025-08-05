/**
 **************************************************
 * @file        utils.c
 * @author      Dimitry Ntofeu Nyatcha
 * @version     v1.0
 * @date        19.04.2025
 * @brief       Modul für allgemeine Operationen auf die GPIO-PORT
 @verbatim
 ==================================================
            ### Verwendete Ressourcen ###
    HAL-Delay-Funktion, GPIO-Strukturen (ODR, IDR)
 ==================================================
            ###     Verwendung      ###
    (#) Aufruf von 'utils_delay_ms()', um eine zeitliche Verzögerung zu erzeugen.
    (#) Aufruf von 'utils_gpio_port_write()', um ein Bitmuster auf einen Port zu schreiben.
    (#) Aufruf von 'utils_gpio_port_read()', um den aktuellen Wert eines Ports auszulesen.
 ==================================================
 @endverbatim
 **************************************************
 */

#include "stm32f4xx.h"
#include "utils.h"

void utils_delay_ms(uint32_t t){
   // HAL_Delay(t);

	__HAL_RCC_TIM2_CLK_ENABLE();


	TIM_HandleTypeDef tim_handle_struct;
	tim_handle_struct.Instance = TIM2;
	tim_handle_struct.Init.Prescaler = (SystemCoreClock  / 1000) - 1; // on s'assure que chaque tics dure 1ms
	tim_handle_struct.Init.Period = 0xFFFFFFFF; // le nombre de tics a attendre
	tim_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_handle_struct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	tim_handle_struct.Init.RepetitionCounter = 0;

	 HAL_TIM_Base_Init(&tim_handle_struct);

	 // demare le timer
	 HAL_TIM_Base_Start(&tim_handle_struct);

	 uint32_t start = __HAL_TIM_GET_COUNTER(&tim_handle_struct);

	 while ((__HAL_TIM_GET_COUNTER(&tim_handle_struct) - start) < t ) {
	         //  warten
	     }

	 //Arreter le timer apres usage (optionnel mais propre)
	 HAL_TIM_Base_Stop(&tim_handle_struct);
}

void  utils_gpio_port_write(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
	GPIOx->ODR =  GPIO_PIN;
}

uint16_t utils_gpio_port_read(GPIO_TypeDef *GPIOx){
	return (uint16_t)GPIOx->IDR;
}

