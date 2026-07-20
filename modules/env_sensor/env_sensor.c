
/**
 **************************************************
 * @file        P1_Fan.c
 * @author      Dimitry Ntofeu Nyatcha, Sidal Güray
 * @version     v1.0
 * @date        07.07.2025
 * @brief       Modul zur Initialisierung von BME280 Sensor und Can Bus
 @verbatim
 ==================================================
            ### Verwendete Ressourcen ###
    - GPIO : PB6 (I2C_SCL), PB7 (I2C_SDA), PB8 (CAN1_RX), PB9 (CAN1_TX)

    - I2C: I2C1 für BME280 Sensor

    - CAN :
       - CAN1 für Senden/Empfangen von Sensor-Daten
       - Filterbank 0-2 für TEMP, PRESS, HUM

    Timer : TIM2 für Senden der Sensordaten mit Sekundentakt
 ==================================================
 ===========================================
            ### Verwendung ###
    (#) Aufruf von 'sensor_gpio_init', um die GPIO-Pins für I2C1 (BME280) und CAN1 zu initialisieren.
    (#) Aufruf von 'sensor_i2c_init', um den I2C1-Bus für die Kommunikation mit dem Sensor zu initialisieren.
    (#) Aufruf von 'sensor_bme280_init', um den BME280-Sensor zu initialisieren.
    (#) Aufruf von 'sensor_read', um aktuelle Messwerte (Temperatur, Druck, Luftfeuchtigkeit) auszulesen.
    (#) Aufruf von 'sensor_read_struct', um aktuelle Messwerte (Temperatur, Druck, Luftfeuchtigkeit) von einem Struct "Data" auszulesen.
    (#) Aufruf von 'init_can', um den CAN-Bus (CAN1) zu initialisieren und Filter zu konfigurieren.
    (#) Aufruf von 'SendSensorData', um Sensordaten über CAN zu versenden.
    (#) Implementierung von 'HAL_CAN_RxFifo0MsgPendingCallback', um eingehende CAN-Nachrichten zu empfangen und auszuwerten.
    (#) Aufruf von 'timer_sensor_init', um Timer 2 für die zyklische Senden von Daten mit Sekundentakt zu initialisieren.
    (#) Aufruf von 'HAL_TIM_PeriodElapsedCallback', um das Flag für das Senden der Daten bei jeder Sekunde zu setzen.
    (#) Aufruf von 'knoten_wechsel_mitJoystick', um mit Joystick zwischen Gruppen-IDs zu wechseln.
 ==================================================
 @endverbatim
 **************************************************
 =======
 */

/* Includes */

#include "stm32f4xx.h"
#include "env_sensor.h"
#include "bme280.h"
#include "bme280_defs.h"
#include "stm32f4xx_hal_can.h"
#include <stdio.h>
#include <lcd/lcd.h>
#include "my_lcd/my_lcd.h"
#include "joystick/joystick.h"
#include "utils/utils.h"

/* Globale Variablen */
I2C_HandleTypeDef handle_i2c1;
CAN_FilterTypeDef can_filter;
TIM_HandleTypeDef tim_handle_struct;
TIM_HandleTypeDef htim2;
CAN_HandleTypeDef can_handle;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

volatile uint8_t temp_received = 0;
volatile uint8_t press_received = 0;
volatile uint8_t hum_received = 0;
volatile uint8_t flag_send_data = 0;

volatile int16_t temp_value = 0;
volatile uint32_t press_value = 0;
volatile uint16_t hum_value = 0;
volatile uint8_t eigene_daten = 0;
volatile groupe_id joystick_groupId = 0;



uint8_t TxData[8];
uint8_t RxData[8];
uint32_t TxMailbox;



float temperature = 0.0f;
float pressure = 0.0f;
float humidity = 0.0f;


/* Static Variablen */
static struct bme280_dev bme;
static struct bme280_data sensor_data;

static uint8_t bme280_addr = BME280_ADDR;
static struct bme280_settings settings;

/* Static Funktionen */

/**
 * @brief Liest Daten von I2C-Register des BME280-Sensors.
 * @param reg_addr Adresse des Registers, von dem gelesen werden soll.
 * @param reg_data Zeiger auf den Puffer, in dem die gelesenen Daten gespeichert werden.
 * @param len Anzahl der zu lesenden Bytes.
 * @param intf_ptr Zeiger auf die I2C-Handle-Struktur.
 * @retval HAL-Statuscode (HAL_OK bei Erfolg, sonst Fehlercode).
 */
static int8_t user_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len,
		void *intf_ptr) {

	return HAL_I2C_Mem_Read((I2C_HandleTypeDef*) intf_ptr, bme280_addr,
			reg_addr, I2C_MEMADD_SIZE_8BIT, reg_data, len, 100);

}


/**
 * @brief Schreibt Daten in ein I2C-Register des BME280-Sensors.
 * @param reg_addr Adresse des Registers, in das geschrieben werden soll.
 * @param reg_data Zeiger auf den Puffer mit den zu schreibenden Daten.
 * @param len Anzahl der zu schreibenden Bytes.
 * @param intf_ptr Zeiger auf die I2C-Handle-Struktur.
 * @retval HAL-Statuscode (HAL_OK bei Erfolg, sonst Fehlercode).
 */
static int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
		uint32_t len, void *intf_ptr) {

	return HAL_I2C_Mem_Write((I2C_HandleTypeDef*) intf_ptr, bme280_addr,
			reg_addr, I2C_MEMADD_SIZE_8BIT, (uint8_t*) reg_data, len, 100);

}
/**
 * @brief Führt eine Verzögerung in Mikrosekunden aus.
 * @param period Verzögerungszeit in Mikrosekunden.
 */
static void user_delay_us(uint32_t period, void *intf_ptr) {
	uint32_t start = HAL_GetTick();
	while ((HAL_GetTick() - start) * 1000 < period)
		;
}

/* Public Funktionen */

/*
 * @brief Funktion zum Initialisieren der GPIO-Pins für I2C1 (BME280) und CAN1.
 */
void sensor_gpio_init(void) {

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_CAN1_CLK_ENABLE();

	GPIO_InitTypeDef gpio_InitStruct;

	gpio_InitStruct.Pin = I2C_SDA | I2C_SCL;
	gpio_InitStruct.Mode = GPIO_MODE_AF_OD;
	gpio_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	gpio_InitStruct.Pull = GPIO_PULLUP;
	gpio_InitStruct.Alternate = GPIO_AF4_I2C1;

	HAL_GPIO_Init(I2C_PORT, &gpio_InitStruct);

	// CAN gpio_pin init
	gpio_InitStruct.Pin = CAN1_RX | CAN1_TX;
	gpio_InitStruct.Mode = GPIO_MODE_AF_OD;
	gpio_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_InitStruct.Pull = GPIO_PULLUP;
	gpio_InitStruct.Alternate = GPIO_AF9_CAN1;

	HAL_GPIO_Init(CAN_PORT, &gpio_InitStruct);

	//CAN1 Interrupt
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0); // fifo0
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
}


/*
 * @brief Funktion zum Initialisieren I2C1-Bus für die Kommunikation mit dem Sensor.
 */
void sensor_i2c_init() {

	handle_i2c1.Instance = I2C1;
	handle_i2c1.Init.ClockSpeed = 100000; //  I2C-Bustakt (SCL-speed)
	handle_i2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	handle_i2c1.Init.OwnAddress1 = 0; //setzt den Mikrocontroller als Master
	handle_i2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT; // adressierungsmodus (7-bit adress)
	handle_i2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE; // aktiviert eine zeite Adresse für der gleiche Sklave
	handle_i2c1.Init.OwnAddress2 = 0; // Adresse secondaire benutzt nur wenn DualAddressMode ENABLE ist
	handle_i2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; //Die Adresse 0x00 wird vom I2C reserviert, um eine Nachricht an alle Slaves gleichzeitig zu senden.
	handle_i2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE; //Clock stretching: Das Gerät kann SCL auf LOW halten, um den Master zum Warten zu zwingen. ( Der Slave darf SCL halten wenn DISABLE steht)

	HAL_I2C_Init(&handle_i2c1);

	int8_t res = HAL_I2C_Init(&handle_i2c1);
	if (res != HAL_OK) {

	}

}

/*
 * @brief Funktion zum Initialisieren der BME280-Sensor
 * @retval Gibt den Statuscode der BME280-Funktion zurück (BME280_OK  bei erfolgreicher Initialisierung, sonst Fehlercode)
 */
int sensor_bme280_init(void) {

	int8_t rslt;

	bme.intf = BME280_I2C_INTF; // bestimmt die peripherie hier I2C
	bme.intf_ptr = &handle_i2c1;
	bme.read = user_i2c_read;
	bme.write = user_i2c_write;
	bme.delay_us = user_delay_us;

	rslt = bme280_init(&bme);

	if (rslt != BME280_OK) {
		return rslt;
	}

	settings.osr_t = BME280_OVERSAMPLING_2X;
	settings.osr_p = BME280_OVERSAMPLING_4X;
	settings.osr_h = BME280_OVERSAMPLING_1X;
	settings.filter = BME280_FILTER_COEFF_2;

	uint8_t settings_Set = BME280_SEL_OSR_PRESS |
	BME280_SEL_OSR_TEMP |
	BME280_SEL_OSR_HUM |
	BME280_SEL_FILTER;

	rslt = bme280_set_sensor_settings(settings_Set, &settings, &bme);

	return rslt;
}

/**
 * @brief Funktion zum Auslesen aktuelle Messwerte (Temperatur, Druck, Luftfeuchtigkeit) vom BME280-Sensor.
 * @param temperature Zeiger auf float, in dem die Temperatur gespeichert wird [°C].
 * @param pressure Zeiger auf float, in dem der Luftdruck gespeichert wird [hPa].
 * @param humidity Zeiger auf float, in dem die Luftfeuchtigkeit gespeichert wird [%].
 * @retval Gibt den Statuscode der BME280-Funktion zurück (BME280_OK bei Erfolg und sonst Fehlercode).
 */
int sensor_read(float *temperature, float *pressure, float *humidity) {

	int8_t rslt;

	rslt = bme280_set_sensor_mode(BME280_POWERMODE_FORCED, &bme);
	uint32_t delay_max_us = 40000;
	int8_t delay = bme280_cal_meas_delay(&delay_max_us, &settings);
	bme.delay_us(delay * 1000, NULL);
	rslt = bme280_get_sensor_data(BME280_ALL, &sensor_data, &bme);

	if (rslt == BME280_OK) {
		*temperature = sensor_data.temperature;
		*pressure = sensor_data.pressure / 100.0f;
		*humidity = sensor_data.humidity;
	}

	return rslt;

}

/**
 * @brief Funktion zum Lesen aktuelle Messwerte vom BME280-Sensor aus und speichert sie in Data Struktur.
 * @param data Zeiger auf eine Data-Struktur, in der Messdaten einzeln gespeichert werden.
 * @retval Gibt den Statuscode der BME280-Funktion zurück (BME280_OK bei Erfolg, sonst Fehlercode).
 */

int sensor_read_struct(Data *data) {
	return sensor_read(&data->temperature, &data->pressure, &data->humidity);
}

/**
 * @brief Funktion zum Initialisieren CAN1 und 3 Filtern (0-2).
 */
void init_can(void) {

	can_handle.Instance = CAN1;
	can_handle.Init.Mode = CAN_MODE_NORMAL; //CAN_MODE_NORMAL; CAN_MODE_LOOPBACK
	can_handle.Init.Prescaler = 21;
	can_handle.Init.SyncJumpWidth = CAN_SJW_1TQ;
	can_handle.Init.TimeSeg1 = CAN_BS1_13TQ;
	can_handle.Init.TimeSeg2 = CAN_BS2_2TQ;
	can_handle.Init.TimeTriggeredMode = DISABLE;
	can_handle.Init.AutoBusOff = DISABLE;
	can_handle.Init.AutoWakeUp = DISABLE; // Ermöglicht es dem CAN, aus dem Schlafmodus/sleep-modus aufzuwachen, wenn eine Nachricht eintrifft.
	can_handle.Init.AutoRetransmission = ENABLE; // Wenn ein Frame verloren wird, wird er automatisch neu gesendet.
	can_handle.Init.ReceiveFifoLocked = DISABLE; // Wenn aktiviert, werden neue Nachrichten ignoriert, wenn der FIFO voll ist...Hier: deaktiviert -> die alten werden überschrieben.
	can_handle.Init.TransmitFifoPriority = DISABLE; // Wenn aktiviert, haben Frames mit der kleinsten ID im FIFO Vorrang. Hier deaktiviert bedeutet: Priorität nach Reihenfolge der Einsendung.

	if (HAL_CAN_Init(&can_handle) != HAL_OK) {

	}

	for (uint8_t sensor_id = 0; sensor_id < 3; sensor_id++) {

		can_filter.FilterActivation = ENABLE;
		can_filter.FilterBank = sensor_id;
		can_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
		can_filter.FilterMode = CAN_FILTERMODE_IDMASK;
		can_filter.FilterScale = CAN_FILTERSCALE_32BIT;

		can_filter.FilterIdHigh = (sensor_id << 5);
		can_filter.FilterMaskIdHigh = (0x000F << 5);
		can_filter.FilterIdLow = 0x0000;
		can_filter.FilterMaskIdLow = 0x0000;

		HAL_CAN_ConfigFilter(&can_handle, &can_filter);

	}

	HAL_CAN_Start(&can_handle);
	HAL_CAN_ActivateNotification(&can_handle, CAN_IT_RX_FIFO0_MSG_PENDING);
}

/**
 * @brief Funktion zum Senden Sensor Daten über CAN.
 * @param sensor_id: ID von Sensor (0-2)
 * @param value: Wert, welche über CAN versendet werden.
 */
void SendSensorData(uint8_t sensor_id, uint32_t value) {

	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.StdId = (GROUP_ID << 4) | sensor_id;

	switch (sensor_id) {
	case TEMP:
		TxHeader.DLC = 2;
		TxData[0] = (value >> 8) & 0xFF;
		TxData[1] = value & 0xFF;
		break;
	case PRESS:
		TxHeader.DLC = 4;
		TxData[0] = (value >> 24) & 0xFF;
		TxData[1] = (value >> 16) & 0xFF;
		TxData[2] = (value >> 8) & 0xFF;
		TxData[3] = value & 0xFF;
		break;
	case HUM:
		TxHeader.DLC = 2;
		TxData[0] = (value >> 8) & 0xFF;
		TxData[1] = value & 0xFF;
		break;
	}

	if (HAL_CAN_GetTxMailboxesFreeLevel(&can_handle) > 0) {
		HAL_CAN_AddTxMessage(&can_handle, &TxHeader, TxData, &TxMailbox);
	}

}

/**
 * @brief Callback Funktion zum Empfangen und Auswertung eingehender CAN-Nachrichten.
 * @param hcan: Zeiger auf CAN_HandleTypeDef
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);

	uint32_t received_groupId = RxHeader.StdId >> 4;

	if (joystick_groupId == received_groupId) {

		switch (RxHeader.FilterMatchIndex) {

		case TEMP: // TEMP
			temp_value = (RxData[0] << 8) | RxData[1];
			temp_received = 1;
			break;

		case PRESS: // PRESS
			press_value = (RxData[0] << 24) | (RxData[1] << 16)
					| (RxData[2] << 8) | RxData[3];
			press_received = 1;
			break;

		case HUM: // HUM
			hum_value = (RxData[0] << 8) | RxData[1];
			hum_received = 1;
			break;

		default:
			break;
		}

	} else {

		return;
	}
}

/**
 * @brief Funktion zum Initialisieren von Timer2.
 * @param hcan: Zeiger auf CAN_HandleTypeDef
 */
void timer_sensor_init() {

	__HAL_RCC_TIM2_CLK_ENABLE();
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = (SystemCoreClock / 100000) - 1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 100000 - 1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim2);
	HAL_TIM_Base_Start_IT(&htim2);

	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/**
 * @brief Callback-Funktion, die bei Ablauf des Timers aufgerufen wird. Setzt 'flag_send_data', wenn der Timer 2 ausgelöst wurde.
 * @param htim Zeiger auf die Timer-Handle-Struktur.
 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		flag_send_data = 1;
	}
}

/**
 * @brief Funktion zum Erkennen von Joystick Inputs um Knoten (0-5) zu wechseln.
 */
bool knoten_wechsel_mitJoystick() {

	char buffer1[25];
	sprintf(buffer1, "GruppeID = %d\n", joystick_groupId);
	lcd_draw_text_at_coord(buffer1, 5, 2, BLACK, 3, WHITE);
	utils_delay_ms(200);

	if ((utils_gpio_port_read(JOY_GPIO_PORT) & JOY_PIN_UP) == 0) {
		joystick_groupId = (joystick_groupId + 1) % 6;
		return true;
	}

	else if ((utils_gpio_port_read(JOY_GPIO_PORT) & JOY_PIN_DOWN) == 0) {
		joystick_groupId = (joystick_groupId + group_5) % (group_5 + 1);
		return true;
	}
	return false;
}
