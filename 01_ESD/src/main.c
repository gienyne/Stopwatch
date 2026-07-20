/**
  ******************************************************************************
  * @file    	main.c
  * @author		Dimitry Ntofeu nyatcha
  * @version 	V1.0
  * @date		15.04.2025
  ******************************************************************************
*/

#include "esd/esd.h"
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "utils/utils.h"



int main(void)
{
	HAL_Init();

		esd_init();

		esd_digit_t digit_number[10] = {
				 ESD_DIGIT_0,
				 ESD_DIGIT_1,
				 ESD_DIGIT_2,
				 ESD_DIGIT_3,
				 ESD_DIGIT_4,
				 ESD_DIGIT_5,
				 ESD_DIGIT_6,
				 ESD_DIGIT_7,
				 ESD_DIGIT_8,
				 ESD_DIGIT_9
		};

		esd_position_t digit_position[5] = {
				 ESD_POSITION_1,
				 ESD_POSITION_2,
				 ESD_POSITION_3,
				 ESD_POSITION_4,
				 ESD_POSITION_ALL
		};


		while(1){

           for(int i = 0; i < 5; i++){
        	   for(int j = 9; j >= 0; j--){
        		   esd_show_digit(digit_number[j], digit_position[i]);
        		   utils_delay_ms(1000);
        	   }
           }

		}
}
