/**
 **************************************************
 * @file        P1_Fan.h
 * @author      Dimitry Ntofeu Nyatcha, Sidal Güray
 * @version     v1.0
 * @date        13.06.2025
 * @brief       Modul zur Initialisierung und Regelung des Lüfters
 * @verbatim
*/

 /* Public Präprozessor Definitionen */
#ifndef P1_FAN_P1_FAN_H_
#define P1_FAN_P1_FAN_H_

#define FAN_PIN (GPIO_PIN_5) //PE5 -> TIM9_CH1
#define FAN_PORT (GPIOE)
#define TACHO_PIN (GPIO_PIN_6) // PF6 -> TIM10_CH1
#define TACHO_PORT (GPIOF)

#define PWM_SAFE_MIN 25
#define RPM_TIMEOUT_MS 2000
#define RPM_TIMEOUT_COUNT_MAX 10
#define PWM_START_THRESHOLD 5


//P_I Regler Konstanten
#define K_P 0.12f  //0.5
#define K_I 0.02f  //0.05
#define T_A 0.1f  //0.1s also 100ms


// Limite PWM
#define PWM_MIN 0
#define PWM_MAX 100


extern TIM_HandleTypeDef htim10;

extern volatile uint32_t last_tacho_impuls;
extern volatile uint8_t pi_update_flag;

/* Public Funktionen (Prototypen) */
void gpio_fan_init(void);
void timer_init(void);
void fan_oc_init(void);
void gpio_tacho_init(void);
void timer_rpm_init(void);
void set_fan_speed_percent(uint8_t percent);
void Ta_pid_timer_init();
void pi_regler_update(void);
uint32_t fan_get_soll_wert(void);
uint32_t fan_get_speed(void);

#endif /* P1_FAN_P1_FAN_H_ */
