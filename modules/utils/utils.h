/*
 * utils.h
 *
 *  Created on: Apr 19, 2025
 *      Author: Dimitry Ntofeu nyatcha
 */

#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include "stm32f4xx.h"

 void utils_delay_ms(uint32_t t);

 void  utils_gpio_port_write(GPIO_TypeDef *GPIO, uint16_t GPIO_PIN);

 uint16_t utils_gpio_port_read(GPIO_TypeDef *GPIO);

#endif /* UTILS_UTILS_H_ */
