/**
 **************************************************
 * @file        env_sensor.h
 * @author      Dimitry Ntofeu Nyatcha, Sidal Güray
 * @version     v1.0
 * @date        07.07.2025
 * @brief       Modul zur Initialisierung von BME280 Sensor und Can Bus
 * @verbatim
*/

#ifndef ENV_SENSOR_ENV_SENSOR_H_
#define ENV_SENSOR_ENV_SENSOR_H_

/* Includes */
#include <stdbool.h>



/* Public Präprozessor Definitionen */
#define I2C_SDA		GPIO_PIN_7
#define I2C_SCL		GPIO_PIN_6
#define I2C_PORT	GPIOB

#define CAN1_RX  GPIO_PIN_8
#define CAN1_TX  GPIO_PIN_9
#define CAN_PORT GPIOB

#define BME280_ADDR	(0x76 << 1)

#define GROUP_ID 5
#define TEMP 0
#define PRESS 1
#define HUM 2

/* Public Variablen */
extern TIM_HandleTypeDef tim_handle_struct;
extern TIM_HandleTypeDef htim2;
extern CAN_HandleTypeDef can_handle;

extern volatile uint8_t temp_received;
extern volatile uint8_t press_received;
extern volatile uint8_t hum_received;
extern volatile uint8_t flag_send_data;
extern volatile uint8_t eigene_daten;

extern volatile int16_t temp_value;
extern volatile uint32_t press_value;
extern volatile uint16_t hum_value;

/* Type Definitionen */
typedef struct {
	float temperature;
	float pressure;
	float humidity;
} Data;

typedef enum {

	group_1 = 1, group_2 = 2, group_3 = 3, group_4 = 4, group_5 = 5

} groupe_id;

/* Variable von selbstdefinierten Type */
extern volatile groupe_id joystick_groupId;

/* Public Funktionen (Prototypen) */
void sensor_gpio_init(void);
void sensor_i2c_init(void);

int sensor_bme280_init(void);
int sensor_read(float *temperature, float *pressure, float *humidity);
int sensor_read_struct(Data *data);

void timer_sensor_init();
void SendSensorData(uint8_t sensor_id, uint32_t value);
void init_can(void);
bool knoten_wechsel_mitJoystick();
#endif /* ENV_SENSOR_ENV_SENSOR_H_ */
