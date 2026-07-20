
/**
 **************************************************
 * @file        P1_Fan.c
 * @author      Dimitry Ntofeu Nyatcha, Sidal Güray
 * @version     v1.0
 * @date        13.06.2025
 * @brief       Modul zur Initialisierung und Regelung des Lüfters
 @verbatim
 ==================================================
            ### Verwendete Ressourcen ###
    GPIO : PE5, PF6
    Timer: TIM1, TIM9, TIM10
 ==================================================
            ###     Verwendung      ###
    (#) Aufruf von 'gpio_fan_init', um der GPIO-Pins für Lüfter zu initialisieren.
    (#) Aufruf von 'timer_init', um Timer 9 zu initialisieren, um ein PWM-Signal (Steuersignal) mit einer Frequenz von 25 kHz zu erzeugen.
    (#) Aufruf von 'fan_oc_init', um ein PWM-Signal mit einem Compare-Wert von 50 zu erzeugen, der mit dem Potentiometer gesteuert werden kann.
    (#) Aufruf von 'gpio_tacho_init', um GPIO-Pins für das Tachosignal  zu initialisieren.
    (#) Aufruf von 'timer_rpm_init, Timer 1 mit 1 MHz zu initialisiren, um die Zeit zwischen zwei Tachosignalen zu messen.
    (#) Aufruf von 'Ta_pid_timer_init', um Timer 10 mit 1kHz für den PI-Regler zu initialisieren.
    (#) Aufruf von 'set_fan_speed_percent', um die Umdrehungsanzahl von Lüftern zu steuern.
    (#) Aufruf von 'pi_regler_update', um die Umdrehungsanzahl der Lüfter zu regeln, um die gewünschte Lüftergeschwindigkeit (Sollwert) zu erreichen.
 	(#) Aufruf von 'fan_get_soll_wert', um Sollwert abhängig vom Potiwert zu berechnen.
 	(#) Aufruf von 'fan_get_speed' um Ist-Wert zu zurückgeben.
 ==================================================
 @endverbatim
 **************************************************
 =======
 */

/* Includes */
#include "stm32f4xx.h"
#include "env_sensor/env_sensor.h"
#include "P1_Fan/P1_Fan.h"
#include "median/median.h"
#include "potis_DMA/potis_DMA.h"


/* Globale Variablen */
TIM_HandleTypeDef tim_handle_struct_fan;
TIM_OC_InitTypeDef tim_oc_handle_struct_fan;
TIM_HandleTypeDef htim1;
//TIM_HandleTypeDef htim10;

volatile uint32_t last_tacho_impuls = 0;
volatile uint32_t rpm_filtered = 0;
volatile uint8_t pi_update_flag = 0;
float e_sum = 0;
uint8_t pwm_output = 0;




/*
 * @brief Funktion zum Initialisieren der GPIO-Pins für Lüfter
 *
 */
void gpio_fan_init(){

	__HAL_RCC_GPIOE_CLK_ENABLE();

	GPIO_InitTypeDef gpio_init_struct;

	gpio_init_struct.Pin = FAN_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_OD; // Alternate Function, Open-Drain
	gpio_init_struct.Pull = GPIO_NOPULL;
	gpio_init_struct.Speed = GPIO_SPEED_MEDIUM;
    gpio_init_struct.Alternate = GPIO_AF3_TIM9;

    HAL_GPIO_Init(GPIOE, &gpio_init_struct);


}


/*
 * @brief Funktion zum Initialisieren von Timer 9,
 * um ein PWM-Signal (Steuersignal) mit einer Frequenz von 25 kHz zu erzeugen
 */
void timer_init(){

	__HAL_RCC_TIM9_CLK_ENABLE();

	tim_handle_struct_fan.Instance = TIM9;  // TIM1 und TIM8 - 11 -> APB2 (90MHZ * 2 = 180Mhz)
	tim_handle_struct_fan.Init.Prescaler = (SystemCoreClock / 2500000) - 1; // Timer mit 2,5Mhz
	tim_handle_struct_fan.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_handle_struct_fan.Init.Period = 100 - 1;
	tim_handle_struct_fan.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim_handle_struct_fan.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim_handle_struct_fan);
	HAL_TIM_Base_Start(&tim_handle_struct_fan);

}


/*
 * @brief Funktion zum Erzeugen eines PWM-Signals mit einem Compare-Wert von 50,
 * der mit dem Potentiometer gesteuert werden kann.
 */
void fan_oc_init(){

	tim_oc_handle_struct_fan.OCMode = TIM_OCMODE_PWM1;

	tim_oc_handle_struct_fan.Pulse = 50 - 1; // 50% ON 50% AUS
	tim_oc_handle_struct_fan.OCIdleState = TIM_OCIDLESTATE_SET;
	tim_oc_handle_struct_fan.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim_oc_handle_struct_fan.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	tim_oc_handle_struct_fan.OCNPolarity = TIM_OCNPOLARITY_LOW;
	tim_oc_handle_struct_fan.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_OC_ConfigChannel(&tim_handle_struct_fan, &tim_oc_handle_struct_fan,TIM_CHANNEL_1);
	HAL_TIM_OC_Start(&tim_handle_struct_fan, TIM_CHANNEL_1); // PE5 -> TIM9_CH1
}


/*
 * @brief Funktion zum Initialisieren von GPIO-Pins für das Tachosignal
 */
void gpio_tacho_init() {

	__HAL_RCC_GPIOF_CLK_ENABLE();

	GPIO_InitTypeDef gpio_init_struct;

	gpio_init_struct.Pin = TACHO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_IT_RISING; // Alternate Function, Open-Drain
	gpio_init_struct.Pull = GPIO_PULLUP;
	gpio_init_struct.Speed = GPIO_SPEED_MEDIUM;


	HAL_GPIO_Init(TACHO_PORT, &gpio_init_struct);

	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
	HAL_NVIC_SetPriority( EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}


/*
 * @brief Funktion zum Initialisieren von Timer 1 mit 1 MHz,
 * um die Zeit zwischen zwei Tachosignalen zu messen.
 */
void timer_rpm_init(){

	__HAL_RCC_TIM1_CLK_ENABLE();
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = (SystemCoreClock / 1000000) - 1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 0xFFFF;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;


	HAL_TIM_Base_Init(&htim1);
	HAL_TIM_Base_Start(&htim1);
}


/*
 * @brief Diese Callback-Funktion wird aufgerufen,
 * wenn ein Tachoimpuls erkannt wurde, um die Anzahl der Umdrehungen pro Minute zu berechnen.
 * @param GPIO_PIN: Gibt an, welcher Pin den Interrupt ausgelöst hat.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN){
	if(GPIO_PIN == TACHO_PIN){
		uint32_t aktuelle_tacho_impuls = __HAL_TIM_GET_COUNTER(&htim1);
		__HAL_TIM_SET_COUNTER (&htim1, 0);
		if( aktuelle_tacho_impuls  > 0){
			uint32_t rpm = (60 * 1000000) / (2 * aktuelle_tacho_impuls);
			rpm_filtered = median_get_median(rpm);
			last_tacho_impuls = HAL_GetTick();
		}
	}
}


/*
 * @brief Funktion zur Initialisierung von Timer 10 mit 1kHz für den PI-Regler
 * Erzeugt pro 100ms einen NVIC Interrupt
 */
void Ta_pid_timer_init(){
	 __HAL_RCC_TIM10_CLK_ENABLE();
	 htim10.Instance = TIM10;
	 htim10.Init.Prescaler = (SystemCoreClock / 1000) - 1;
	 htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	 htim10.Init.Period = 100 - 1;
	 htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	 HAL_TIM_Base_Init(&htim10);
	 HAL_TIM_Base_Start_IT(&htim10);

	 HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 1);
	 HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

/*
 * @brief Callback-Funktion für den NVIC-Interrupt des PI-Reglers,
 * der alle 100 ms erzeugt wird.
 * Falls innerhalb von 800 Milisekunden kein Tachoimpuls angekommen ist,
 * wird die Umdrehungszahl auf 0 gesetzt. (Timeout)
 * @param htim: Timer Instanz, der den Interrupt ausgelöst.
 */
/*void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM10) {

    	pi_update_flag = 1;
    	if ((HAL_GetTick() - last_tacho_impuls) > 800) {  // 800 ms timeout
    	        rpm_filtered = 0;
    	    }
    }
}*/


/*
 * @brief Funktion zum Steuern der Umdrehungsanzahl von Lüftern
 * Der Compare-Wert für das PWM-Signal wird mit dem Poti-Wert gesteuert.
 * @param percent: Prozent für die Lüftergeschwindigkeit
 */
void set_fan_speed_percent(uint8_t percent){
	if(percent > 100){
		percent = 100;
	}
	else if(percent < 0){
		percent = 0;
	}

	__HAL_TIM_SET_COMPARE(&tim_handle_struct_fan, TIM_CHANNEL_1, percent);
}


/*
 * @brief Funktion zum Regeln der Umdrehungsanzahl der Lüfter,
 * um die gewünschte Lüftergeschwindigkeit (Sollwert) zu erreichen.
 */
void pi_regler_update(void){
	uint32_t adc_mv = potis_DMA_get_average_val_mv(0);
	float soll_wert = (adc_mv * 4000.0f) / (3300.0f); // 4000
	float ist_Wert = (float)rpm_filtered;
	float error = soll_wert - ist_Wert;

	e_sum += error;

	//PWM-Ausgang
	float u =  (K_P * error) + (K_I * T_A * e_sum);

	// Limitation
	if (u > PWM_MAX) {
	    u = PWM_MAX;
	} else if (u < PWM_MIN) {
	  u = PWM_MIN;
	}
	pwm_output = (int)u;
	set_fan_speed_percent(pwm_output);
}


/*
 * @brief Getter-Funktion zum Berechnen des Sollwerts abhängig vom Potiwert.
 * @return Auf 4000 skalierter Sollwert
 */
uint32_t fan_get_soll_wert(void) {
  uint32_t adc_mv = potis_DMA_get_average_val_mv(0);
  return (adc_mv * 4000) / 3300;
}

/*
 * @brief Getter-Funktion zum Zurückgeben des Ist-Werts
 * @return Gibt die gefilterte Umdrehungsanzahl des Lüfters (Ist-Wert) zurück.
 */
uint32_t fan_get_speed(void) {
  return rpm_filtered;
}
