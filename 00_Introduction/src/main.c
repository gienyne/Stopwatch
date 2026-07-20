/**
  ******************************************************************************
  * @file    	main.c
  * @author		Ntofeu nyatcha dimitry
  * @version 	V1.0
  * @date		13.04.2025
  * @brief  	Aufgabe 1: LED_on -> Antwort auf Fragen
  ******************************************************************************
*/



#include <lcd/lcd.h>
#include "stm32f4xx.h"


int main(void)
{
	HAL_Init();

 /*Konfiguration GPIO-Ports für den Betrieb beider LEDs PG13 et PG14 */

	__HAL_RCC_GPIOG_CLK_ENABLE(); // versorgt GPIO- PORT G mit einem Takt


   // Anlegung der Struktur GPIO für Gruene und rote Led

	GPIO_InitTypeDef gpio_init;
	gpio_init.Pin = GPIO_PIN_13 | GPIO_PIN_14;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOG, &gpio_init);


	while(1) {
		   //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
		   //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);

		   /* Aktivieren Sie nun die grüne LED.*/
		 // HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);

		 /*4. Aktivieren Sie mithilfe eines einzigen Funktionsaufrufs beide LEDs*/
		  // HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_SET);

		 /*5. Aktivieren Sie beide LEDs und deaktivieren Sie anschließend die rote LED wieder.*/
		  // HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_SET);
		  // HAL_GPIO_WritePin(GPIOG,  GPIO_PIN_14, GPIO_PIN_RESET);

		 /* Deaktivieren Sie mit nur einem Funktionsaufruf beide LEDs.*/
		   //HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_RESET);

		/*Aktivieren Sie die grüne LED und togglen Sie daraufhin beide LEDs mit der Toggle Funktion.
           Rufen Sie diesen Befehl mehrmals auf, um den Effekt zu sehen: Wegen dem Effekt soll die PIN getrennt getoogle werden*/
		  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
		  HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14 | GPIO_PIN_13);
		  /*Bonus um effekt zu beseitigen*/
		 // HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
		 // HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
		  HAL_Delay(1000);
	}
}
