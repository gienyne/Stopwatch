/**
 **************************************************
 * @file        joystick.h
 * @author      Dimitry Ntofeu Nyatcha
 * @version     v1.0
 * @date        20.04.2025
 * @brief       Header-Datei für das Joystick-Modul
 **************************************************
 */

#ifndef JOYSTICK_JOYSTICK_H_
#define JOYSTICK_JOYSTICK_H_

#include "stm32f4xx.h"

/*Define*/
#define JOY_PIN_UP          (GPIO_PIN_11)
#define JOY_PIN_DOWN        (GPIO_PIN_6)
#define JOY_PIN_LEFT        (GPIO_PIN_9)
#define JOY_PIN_RIGHT       (GPIO_PIN_10)
#define JOY_PIN_PRESS       (GPIO_PIN_12)

#define JOY_PIN_UP_LEFT     (JOY_PIN_UP | JOY_PIN_LEFT)
#define JOY_PIN_UP_RIGHT    (JOY_PIN_UP | JOY_PIN_RIGHT)
#define JOY_PIN_DOWN_LEFT   (JOY_PIN_DOWN | JOY_PIN_LEFT)
#define JOY_PIN_DOWN_RIGHT  (JOY_PIN_DOWN | JOY_PIN_RIGHT)

#define JOY_ALL_DIRECTION   (JOY_PIN_UP | JOY_PIN_DOWN | JOY_PIN_LEFT | JOY_PIN_RIGHT | JOY_PIN_PRESS)

#define JOY_GPIO_PORT       (GPIOG)

/*Public Prototyp Funktion*/
/**
 * @brief  Initialisiert die GPIOs für den Joystick
 * @param  Keine
 * @return Keine
 */
void joystick_init(void);

#endif /* JOYSTICK_JOYSTICK_H_ */
