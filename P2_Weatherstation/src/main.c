/**
 ******************************************************************************
 * @file    	main.c
 * @author		Dimitry Ntofeu Nyatcha, Sidal Güray
 * @version 	V1.0
 * @date		07.07.2025
 * @brief		Main Funktion für die Aufgabe Wetterstation
 ******************************************************************************
 */

#include <lcd/lcd.h>
#include "my_lcd/my_lcd.h"
#include <lcd/lcd.h>
#include "stm32f4xx.h"
#include "env_sensor/env_sensor.h"
#include <stdio.h>
#include "env_sensor/bme280.h"
#include "env_sensor/bme280_defs.h"
#include "stm32f4xx_hal_can.h"
#include "joystick/joystick.h"

int main(void) {
	HAL_Init();

	/* Initialization of the LCD */
	lcd_init();
	//lcd_draw_text_at_line("BME280 Sensor Monitor", 0, BLACK, 2, WHITE);

	sensor_gpio_init();
	sensor_i2c_init();
	sensor_bme280_init();
	timer_sensor_init();
	init_can();
	joystick_init();
	float temp;
	float hum;
	float press;
	while (1) {
		if (knoten_wechsel_mitJoystick()) {

			eigene_daten = (joystick_groupId == GROUP_ID);
			// LCD löschen
			lcd_draw_rect(140, 62, 240, 320, WHITE, 1);
			lcd_draw_rect(140, 112, 240, 320, WHITE, 1);
			lcd_draw_rect(140, 162, 240, 320, WHITE, 1);

			my_lcd_draw_baargraph(20, 60, 110, 20, 0, WHITE, WHITE);
			my_lcd_draw_baargraph(20, 110, 110, 20, 0, WHITE, WHITE);
			my_lcd_draw_baargraph(20, 160, 110, 20, 0, WHITE, WHITE);

			// Flags zurücksetzen
			temp_received = 0;
			press_received = 0;
			hum_received = 0;
			temp = 0;
			press = 0;
			hum = 0;
		}
		if (flag_send_data) {
			Data data;
			if (sensor_read_struct(&data) == BME280_OK) {

				// Conversion
				int16_t temp100 = (int16_t) (data.temperature * 100);
				uint32_t press100 = (uint32_t) (data.pressure * 100);
				uint16_t hum100 = (uint16_t) (data.humidity * 100);

				// Envoi CAN
				SendSensorData(TEMP, temp100);

				SendSensorData(PRESS, press100);
				SendSensorData(HUM, hum100);

				if (!eigene_daten) {
					if (temp_received) {
						temp = temp_value / 100.0f;
					}
					if (press_received) {
						press = press_value / 100.0f;
					}
					if (hum_received) {
						hum = hum_value / 100.0f;
					}
				} else {
					temp = data.temperature;
					press = data.pressure;
					hum = data.humidity;
				}

				char temp_buffer[16];
				uint16_t temp_promille =
						(temp >= 50.0f) ?
								1000 : (uint16_t) ((temp * 1000.0f) / 50.0f);
				sprintf(temp_buffer, "%.2f C", temp);
				lcd_draw_text_at_coord(temp_buffer, 140, 62, RED, 2, WHITE);
				my_lcd_draw_baargraph(20, 60, 110, 20, temp_promille, RED,
						YELLOW);

				char press_buffer[32];
				uint16_t press_promille =
						(press >= 1100.0f) ?
								1000 : (uint16_t) ((press * 1000.0f) / 1100.0f);
				my_lcd_draw_baargraph(20, 110, 110, 20, press_promille, BLUE,
						YELLOW);
				sprintf(press_buffer, "%.2f hPa", press);
				lcd_draw_text_at_coord(press_buffer, 140, 112, BLUE, 2, WHITE);

				char hum_buffer[32];
				uint16_t hum_promille =
						(hum >= 100.0f) ? 1000 : (uint16_t) (hum * 10.0f);
				my_lcd_draw_baargraph(20, 160, 110, 20, hum_promille, GREEN,
						YELLOW);
				sprintf(hum_buffer, "%.2f %%", hum);
				lcd_draw_text_at_coord(hum_buffer, 140, 162, GREEN, 2, WHITE);
			}

			flag_send_data = 0; // reset flag

		}
	}
}
