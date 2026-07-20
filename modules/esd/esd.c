/**
 **************************************************
 * @file        esd.c
 * @author      Dimitry Ntofeu Nyatcha
 * @version     v1.0
 * @date        15.04.2025
 * @brief       Modul zur Steuerung des 8-Segment-Displays
 @verbatim
 ==================================================
            ### Verwendete Ressourcen ###
    GPIO -> (GPIOE, und GPIOD): PD0, PD1, PD4, PD5, PD6, PD7, PD11, PD12, PD14, PD15
          PE7, PE11, PE12
 ==================================================
            ###     Verwendung      ###
    (#) Aufruf von 'esd_init()', um die notwendigen Pheripherie bzw GPIO-Port zu initialisieren.
    (#) Aufruf von 'esd_show_digit(digit, pos)', um eine bestimmte Zahl an einer bestimmten Position anzuzeigen.
 ==================================================
 @endverbatim
 **************************************************
 */

/* Includes */
#include "stm32f4xx.h"
#include "esd/esd.h"


/**
 * @brief  Initialisiert die benötigten GPIO-PORT bzw PINS für das 8-Segment-Display
 * @param  Keine
 * @return Keine
 */
 void esd_init(void){

	 __HAL_RCC_GPIOD_CLK_ENABLE(); // versorgt GPIO - PORT D mit einem Takt
	 __HAL_RCC_GPIOE_CLK_ENABLE(); // versorgt GPIO - PORT E mit einem Takt

	 /* Anlegung der GPIO-Struktur für die 7-SEGMENT*/
	 GPIO_InitTypeDef gpio_init;

	  gpio_init.Pin = ALLE_CNTL | ALLE_SEG_OHNE_G; // auf port D
	  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	  gpio_init.Pull = GPIO_NOPULL;
	  gpio_init.Speed = GPIO_SPEED_MEDIUM;

	  HAL_GPIO_Init(GPIOD, &gpio_init);

	      gpio_init.Pin = (SEG_G | DOT | POINT);
		  gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
		  gpio_init.Pull = GPIO_NOPULL;
		  gpio_init.Speed = GPIO_SPEED_MEDIUM;

	  HAL_GPIO_Init(GPIOE, &gpio_init);

 }

 /**
  * @brief  Zeigt eine zahl an der angegebenen Position an
  * @param  digit : Die anzuzeigende Zahl
  * @param  pos   : Die Position, an der die Ziffer angezeigt werden soll
  * @return Keine
  */
 void esd_show_digit(esd_digit_t digit, esd_position_t pos){


	 /*ALLE 7 SEGMENTS ausschalten*/
	 HAL_GPIO_WritePin(GPIOD, ALLE_SEG_OHNE_G, GPIO_PIN_SET); // GPIO_PIN_SET denn  segmente low aktiv sind

	 /*SEGMENT G ausschalten */
	 HAL_GPIO_WritePin(GPIOE, SEG_G, GPIO_PIN_SET);

	 /*Alle Leitungen Zur ansteuerung der 7 Segments deaktivieren*/
	 HAL_GPIO_WritePin(GPIOD, ALLE_CNTL, GPIO_PIN_RESET);

	 switch(pos){
	 /*Jedes Mal werden die entsprechenden Leitungen für die betrofene Position gesetzt und Punkt und doppelpunkt ungesezt */
	 case ESD_POSITION_1:
		 HAL_GPIO_WritePin(GPIOD, CNTL1, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOE, POINT | DOT, GPIO_PIN_SET);
		 break;
	 case ESD_POSITION_2:
		 HAL_GPIO_WritePin(GPIOD, CNTL2, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOE, POINT | DOT, GPIO_PIN_SET);
	     break;

	 case ESD_POSITION_3:
		 HAL_GPIO_WritePin(GPIOD, CNTL3, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOE, POINT | DOT, GPIO_PIN_SET);
		 break;

	 case ESD_POSITION_4:
	     HAL_GPIO_WritePin(GPIOD, CNTL4, GPIO_PIN_SET);
	 	 HAL_GPIO_WritePin(GPIOE, POINT | DOT, GPIO_PIN_SET);
	 	 break;

	 case ESD_POSITION_ALL:
		 HAL_GPIO_WritePin(GPIOD, CNTL1 | CNTL2 | CNTL3 | CNTL4, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOE, DOT | POINT, GPIO_PIN_SET);
		 break;

	 }



	switch(digit) {
    /*Jedes Mal werden die entsprechenden Segments für die betrofene Ziffer gesetzt */
	case ESD_DIGIT_0:
		HAL_GPIO_WritePin(GPIOD,  DIGIT_0, GPIO_PIN_RESET);
		break;

	case ESD_DIGIT_1:
		HAL_GPIO_WritePin(GPIOD, DIGIT_1, GPIO_PIN_RESET);
		break;

	case ESD_DIGIT_2:
		HAL_GPIO_WritePin(GPIOD, DIGIT_2_OHNE_G, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, SEG_G, GPIO_PIN_RESET);
		break;

	case ESD_DIGIT_3:
			HAL_GPIO_WritePin(GPIOD, DIGIT_3_OHNE_G, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, SEG_G, GPIO_PIN_RESET);
			break;

	case ESD_DIGIT_4:
			HAL_GPIO_WritePin(GPIOD, DIGIT_4_OHNE_G, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, SEG_G, GPIO_PIN_RESET);
			break;

	case ESD_DIGIT_5:
			HAL_GPIO_WritePin(GPIOD, DIGIT_5_OHNE_G, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, SEG_G, GPIO_PIN_RESET);
			break;

	case ESD_DIGIT_6:
			HAL_GPIO_WritePin(GPIOD, DIGIT_6_OHNE_G, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, SEG_G, GPIO_PIN_RESET);
			break;

	case ESD_DIGIT_7:
			HAL_GPIO_WritePin(GPIOD, DIGIT_7, GPIO_PIN_RESET);
			break;

	case ESD_DIGIT_8:
			HAL_GPIO_WritePin(GPIOD, DIGIT_8_OHNE_G, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, SEG_G, GPIO_PIN_RESET);
			break;

	case ESD_DIGIT_9:
			HAL_GPIO_WritePin(GPIOD, DIGIT_9_OHNE_G, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, SEG_G, GPIO_PIN_RESET);
			break;
	}
 }


