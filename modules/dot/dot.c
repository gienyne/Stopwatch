/**
 * @file dot.c
 * @author Dimitry Ntofeu Nyatcha
 * @brief Modul zur Steuerung des DOT-Segments mit PWM, Dimming, Zeitmessung und Rundenzähler
 *
 * @verbatim
 ============================================================
 ### Genutzte Ressourcen ###
 GPIO:
   - GPIOE: DOT-Segment
   - GPIOD: 7-Segment-Anzeige
   - GPIOA / GPIOG: USER_TASTE / JOY_GPIO_PORT
 TIMER:
   - TIM1: Für DOT-Blinken, Dimming, Stoppuhr
   - TIM2: Für Treppenhausfunktion
 ADC:
   - Extern über potis_DMA zur Steuerung von Frequenz und Helligkeit

 ============================================================
 ### Funktionsübersicht ###

 1. ***Initialisierung***
   (#) dot_gpio_init():
       - Initialisiert GPIOE und GPIOD für DOT- und 7-Segment-Anzeige

 2. ***PWM-Steuerung für DOT-Blinkfrequenz***
   (#) blink_dot_timer_init():
       - Startet Timer1 mit 10 kHz für Basisfrequenz
   (#) blink_dot_oc_init():
       - Konfiguriert PWM (OC) für Blinkfunktion mit 50% Duty Cycle
   (#) set_blink_Dot_freq(poti_num, mv):
       - Dynamische Frequenzanpassung über Potentiometer (1–10 Hz)
   (#) get_freq():
       - Liefert aktuelle Blinkfrequenz in mHz

 3. ***DOT-Dimming über PWM***
   (#) dimming_dot_timer_init():
       - Startet Timer1 mit 1 MHz Basis für feines Dimming
   (#) dimming_dot_oc_init():
       - Initialisiert PWM für Helligkeitssteuerung
   (#) dimming_intensitat_Dot(poti_num):
       - Passt Duty Cycle an Potentiometerwert an

 4. ***Treppenhausfunktion***
   (#) treppenhaus_init():
       - Initialisiert GPIO, Timer2 und Taster
       - DOT bleibt 59s aktiv nach Tastendruck (JOY_PIN_PRESS)

 5. ***Stoppuhr mit Rundenmessung***
   (#) stopwatch_timer_init():
       - Initialisiert Timer1 für 1 ms Tick (Interrupt)
   (#) stopwatch_gpio_init():
       - Konfiguriert USER-TASTE als EXTI0-Interrupt
   (#) HAL_TIM_PeriodElapsedCallback():
       - Inkrementiert zeitstand jede Millisekunde
   (#) HAL_GPIO_EXTI_Callback():
       - Setzt taste_gedrueckt-Flag bei Tastendruck (USER-TASTE)
   (#) taste_verarbeiten():
       - Startet Chrono oder speichert aktuelle Rundenzeit
   (#) gesamtdauer_anzeigen():
       - Zeigt laufende Gesamtzeit + aktuelle Runde (wenn aktiv)
   (#) zeit_anzeigen_at():
       - Formatiert Zeit als MM:SS.CC und zeigt sie an

 6. ***Grafische Benutzeroberfläche (LCD)***
   (#) dessine_boutons():
       - Zeichnet "Reset"- und "Start"-Buttons auf dem LCD

 ============================================================
 @endverbatim
 */

/**
 * Globale Variablen
 */
#include "stm32f4xx.h"
#include <stdio.h>
#include <lcd/lcd.h>
#include "dot/dot.h"
#include "potis_DMA/potis_DMA.h"
#include "utils/utils.h"
#include "median/median.h"
#include "env_sensor/env_sensor.h"

//TIM_HandleTypeDef tim_handle_struct;
TIM_OC_InitTypeDef tim_oc_handle_struct;


uint32_t freq_mHz; // Dot frequenz

/**
 * Private variablen
 */
volatile uint32_t zeitstand = 0; // aktuelle zähler
volatile uint32_t last_zeitstand = 0; // letzer Zähler-Wert
volatile uint8_t chrono_laeuft= 0; //Zeigt an, ob die Stoppuhr läuft
volatile uint32_t rundenzeiten[MAXIMALE_RUNDEN]; // Aufgezeichnete Zeiten für jede Runde
volatile uint8_t runde_index = 0; // index aktueller Runde
volatile uint32_t startzeit = 0; // Startzeit der Stoppuhr
volatile uint32_t runden_startzeit = 0; // startzeit für jede Runde
volatile uint8_t taste_gedrueckt = 0; // sagt ob die button gedrückt ist


/**
 * @brief Initialisiert GPIOs für das DOT-Segment und 7-Segment-Anzeige
 * @param 	None
 * @retval  None
 */
void dot_gpio_init(){

	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	 HAL_GPIO_WritePin(GPIOE, DOT, GPIO_PIN_RESET);
	GPIO_InitTypeDef gpio_init_struct;

	gpio_init_struct.Pin = DOT;
	gpio_init_struct.Mode =  GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_MEDIUM;
	gpio_init_struct.Alternate = GPIO_AF1_TIM1;

	HAL_GPIO_Init(GPIOE, &gpio_init_struct);

	    gpio_init_struct.Pin = ALLE_CNTL | ALLE_SEG_OHNE_G | SEG_G;
		gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
		gpio_init_struct.Pull = GPIO_NOPULL;
		gpio_init_struct.Speed = GPIO_SPEED_MEDIUM;

		HAL_GPIO_Init(GPIOD, &gpio_init_struct);

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, SET);

		/*ALLE 7 SEGMENTS ausschalten*/
		HAL_GPIO_WritePin(GPIOD, ALLE_SEG_OHNE_G, GPIO_PIN_SET); // GPIO_PIN_SET denn  segmente low aktiv sind

		/*SEGMENT G ausschalten */
		HAL_GPIO_WritePin(GPIOE, SEG_G, GPIO_PIN_SET);



}

/**
 * @brief Initialisiert den Timer1 für das Blinken des DOT-Segments
 * @param 	None
 * @retval  None
 */
void blink_dot_timer_init(){

	 __HAL_RCC_TIM1_CLK_ENABLE();

	 tim_handle_struct.Instance = TIM1;
	 tim_handle_struct.Init.Prescaler = (SystemCoreClock / 10000) - 1;// 10khz
	 tim_handle_struct.Init.Period = 10000 - 1;
	 tim_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
	 tim_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	 tim_handle_struct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	 tim_handle_struct.Init.RepetitionCounter = 0;

	 HAL_TIM_Base_Init(&tim_handle_struct);
	 HAL_TIM_Base_Start(&tim_handle_struct);

}

/**
 * @brief  Konfiguriert die Output-Compare Einheit 2 für das PWM-Signal zur Steuerung der Blink-frequenz von dem Dot
 * @param 	None
 * @retval  None
 */
void blink_dot_oc_init(){

	tim_oc_handle_struct.OCMode = TIM_OCMODE_PWM1;

	tim_oc_handle_struct.Pulse = 5000 - 1; // 50% (AN 0.5ms, AUS 0.5ms)
	tim_oc_handle_struct.OCIdleState = TIM_OCIDLESTATE_SET;
	tim_oc_handle_struct.OCPolarity = TIM_OCPOLARITY_LOW;
	tim_oc_handle_struct.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	tim_oc_handle_struct.OCNPolarity = TIM_OCNPOLARITY_LOW;
	tim_oc_handle_struct.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_OC_ConfigChannel(&tim_handle_struct, &tim_oc_handle_struct,TIM_CHANNEL_2);
	HAL_TIM_OC_Start(&tim_handle_struct, TIM_CHANNEL_2);

}

/**
 * @brief Setzt die PWM-Frequenz basierend auf ADC-Messung
 * @param poti_num Nummer des Potentiometers (siehe potis.h)
 * @param mv ADC-Wert in Millivolt
 * @retval None
 */
void set_blink_Dot_freq(int poti_num, uint32_t mv){

	// freq en mHz : 1000 à 10000 (1Hz a 10Hz)
	freq_mHz = 1000 + (mv * 9000) / POTIS_VREF_MV;

	    // umwandlung in hz
	      uint32_t freq_Hz = freq_mHz / 1000;


    // berechnung des neuen Prescalller-Wert basierend auf die neue Frequenz
    uint32_t prescaler = freq_Hz * DOT_TIMER_FREQ_HZ  ;


    __HAL_TIM_SET_PRESCALER(&tim_handle_struct,(SystemCoreClock / prescaler) - 1); // set den neuen Prescaller-Wert

    __HAL_TIM_ENABLE(&tim_handle_struct);

}

/**
 * @brief Gibt die aktuelle DOT-Frequenz in mHz zurück
 * @retval freq_mHz aktuelle frequenz mit der dot blink
 */
int get_freq(){
	return freq_mHz;
}

/**
 * @brief Initialisiert Timer 1 für Dimming zur Steuerung der Dot-Helligkeit
 * @param None
 * @retval None
 */
void dimming_dot_timer_init(){

	 __HAL_RCC_TIM1_CLK_ENABLE();

	 tim_handle_struct.Instance = TIM1;
	 tim_handle_struct.Init.Prescaler = (SystemCoreClock / 1000000) - 1;
	 tim_handle_struct.Init.Period = 1000 - 1;
	 tim_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
	 tim_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	 tim_handle_struct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	 tim_handle_struct.Init.RepetitionCounter = 0;

	 HAL_TIM_Base_Init(&tim_handle_struct);

}

/**
 * @brief  Konfiguriert die Output-Compare Einheit 2 für das PWM-Signal zur Steuerung der Dot-Helligkeit
 * @param None
 * @retval None
 */
void dimming_dot_oc_init(){

	tim_oc_handle_struct.OCMode = TIM_OCMODE_PWM1;

	tim_oc_handle_struct.Pulse = 500 - 1; // 50% (ON 0.5ms, OFF 0.5ms)
	tim_oc_handle_struct.OCIdleState = TIM_OCIDLESTATE_SET;
	tim_oc_handle_struct.OCPolarity = TIM_OCPOLARITY_LOW;
	tim_oc_handle_struct.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	tim_oc_handle_struct.OCNPolarity = TIM_OCNPOLARITY_LOW;
	tim_oc_handle_struct.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_OC_ConfigChannel(&tim_handle_struct, &tim_oc_handle_struct,TIM_CHANNEL_2);
	HAL_TIM_OC_Start(&tim_handle_struct, TIM_CHANNEL_2);

}

/**
 * @brief Stellt Helligkeit des DOT je nach Potentiometerwert ein
 * @param poti_num Nummer des Potentiometers (siehe potis.h)
 * @retval None
 */
void dimming_intensitat_Dot(int poti_num){

	uint32_t mv = potis_DMA_get_average_val_mv(poti_num); // [0-3300] mV

	uint32_t duty_cycle = (mv * 100) / POTIS_VREF_MV; // [0-100] %

	uint32_t pwm_period = __HAL_TIM_GET_AUTORELOAD(&tim_handle_struct) + 1; //  ARR + 1 = freq_timer / freq_pwm

	uint32_t pulse = (duty_cycle * pwm_period) / 100;

	 __HAL_TIM_SET_COMPARE(&tim_handle_struct, TIM_CHANNEL_2, pulse);



}

/**
 * @brief Initialisiert die Treppenhausfunktion (DOT + Taste) (hier geht darum auf die Taste:JOY_PIN_PRESS zu drücken und das Dot-Led für 59s leuchten lassen)
 * @param None
 * @retval None
 */
void treppenhaus_init(){

	__HAL_RCC_TIM2_CLK_ENABLE();

	__HAL_RCC_GPIOE_CLK_ENABLE();

	__HAL_RCC_GPIOG_CLK_ENABLE();

	__HAL_RCC_GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef gpio_init;
		gpio_init.Pin = DOT ;
		gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
		gpio_init.Pull = GPIO_NOPULL;
		gpio_init.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOE, &gpio_init);

	GPIO_InitTypeDef gpio_initdef;
		gpio_initdef.Pin = ALLE_CNTL ;
		gpio_initdef.Mode = GPIO_MODE_OUTPUT_PP;
		gpio_initdef.Pull = GPIO_NOPULL;
		gpio_initdef.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOD, &gpio_initdef);


	GPIO_InitTypeDef joystick_gpio_init;
	    joystick_gpio_init.Pin = JOY_PIN_PRESS; // für User-taste (GPIOA Pin 0 benutzen)
		joystick_gpio_init.Mode = GPIO_MODE_INPUT;
		joystick_gpio_init.Pull = GPIO_PULLUP;

		 	HAL_GPIO_Init(GPIOG, &joystick_gpio_init);

		 	 HAL_GPIO_WritePin(GPIOE, DOT, GPIO_PIN_RESET);
		 	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, SET);


		 tim_handle_struct.Instance = TIM2;
		 tim_handle_struct.Init.Prescaler = (SystemCoreClock / 10000) - 1; // 10.000
		 tim_handle_struct.Init.Period = 295000 - 1;                       //590000 / 2
		 tim_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
		 tim_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		 tim_handle_struct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		 tim_handle_struct.Init.RepetitionCounter = 0;

		 HAL_TIM_Base_Init(&tim_handle_struct);



}

/**
 * @brief Initialisiert Timer1 (und setzt seine Priority-Gruppe und subpriority) für Stoppuhr und aktiviert die Interrupt
 * @param None
 * @retval None
 */
void stopwatch_timer_init(){

	 __HAL_RCC_TIM1_CLK_ENABLE();

	 tim_handle_struct.Instance = TIM1;
	 tim_handle_struct.Init.Prescaler = (SystemCoreClock / 10000) - 1; // 10khz
	 tim_handle_struct.Init.Period = 100 - 1; //1ms
	 tim_handle_struct.Init.CounterMode = TIM_COUNTERMODE_UP;
	 tim_handle_struct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	 tim_handle_struct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	 tim_handle_struct.Init.RepetitionCounter = 0;

	 HAL_TIM_Base_Init(&tim_handle_struct);

	 // startet den Timer mit Interrupt
	 HAL_TIM_Base_Start_IT(&tim_handle_struct);
	 HAL_NVIC_SetPriority( TIM1_UP_TIM10_IRQn, 1, 1);
	 HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

/**
 * @brief Initialisiert GPIO für die USER-TASTE und setzt die Priority-Group und subpriority für den Interrunpt
 * @param None
 * @retval None
 */
void stopwatch_gpio_init(){

	 __HAL_RCC_GPIOA_CLK_ENABLE(); //------------------- nur falls man einen andderen Buttons benutzen will stellen mit //----- ändern

		GPIO_InitTypeDef gpio_init_struct;

		gpio_init_struct.Pin = USER_TASTE; // -------------
		gpio_init_struct.Mode = GPIO_MODE_IT_RISING;
		gpio_init_struct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(USER_PORT, &gpio_init_struct); // ------------------

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
	HAL_NVIC_SetPriority(EXTI0_IRQn , 0, 0); //------------
	HAL_NVIC_EnableIRQ(EXTI0_IRQn); // --------------------

}

/**
 * @brief Timer-Interrupt Callback – zählt "Zähler" für Zeitmessung hoch
 * @param htim pointer auf eine struktur von typ TIM_HandleTypeDef
 * @retval None
 */
/*void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance == TIM1 ){

		zeitstand++;

	}
}*/

/**
 *  @brief EXTI Callback bei Usertaste – aktiviert das Flag taste_gedrueckt
 *  @param GPIO_Pin Pin der GPIO-PORT
 *  @retval None
 */
/*void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

    if (GPIO_Pin == USER_TASTE) {
    	taste_gedrueckt = 1;
    }


}*/

/**
 *  @brief Behandelt Tastendruck (USER_TASTE) – startet und speichert Rundenzeit
 *  @param None
 *  @retval None
 */
void taste_verarbeitung() {
    if (taste_gedrueckt) {
        taste_gedrueckt = 0;

        uint32_t jetzt = zeitstand;

        if (!chrono_laeuft) {
            chrono_laeuft = 1;
            startzeit = jetzt;
            runde_index = 0;
            runden_startzeit  = jetzt;
        } else if (runde_index < MAXIMALE_RUNDEN) {
            uint32_t rundenzeit = jetzt - runden_startzeit;
            rundenzeiten[runde_index] = rundenzeit;

            char label[16];
            snprintf(label, sizeof(label), "Runde %d:", runde_index + 1);
            lcd_draw_text_at_coord(label, 10, 30 + runde_index * 20, BLACK, 2, WHITE);
            zeit_anzeigen_at(rundenzeiten[runde_index], 95, 30 + runde_index * 20);

            runde_index++;
            runden_startzeit  = jetzt;
        }
    }
}



/**
 * @brief Zeigt fortlaufend die verstrichene Gesamtzeit auf dem Display an
 * @param None
 * @retval None
 */
void gesamtdauer_anzeigen() {
    if (chrono_laeuft) {
        uint32_t jetzt = zeitstand;
        uint32_t vergangene_zeit = jetzt - startzeit;

        lcd_draw_text_at_coord("Gesamt:", 10, 150, BLACK, 2, WHITE);
        zeit_anzeigen_at(vergangene_zeit, 90, 150);

        if (runde_index < MAXIMALE_RUNDEN) {
            char label[16];
            snprintf(label, sizeof(label), "Runde %d:", runde_index + 1);
            lcd_draw_text_at_coord(label, 10, 30 + runde_index * 20, BLACK, 2, WHITE);

            uint32_t aktuelle_rundenzeit = jetzt - runden_startzeit ;
            zeit_anzeigen_at(aktuelle_rundenzeit, 95, 30 + runde_index * 20);
        }
    }
}


/**
 * @brief Zeigt die Zeit im Format Minuten:Sekunden:Zehntelsekunden auf dem LCD an.
 *
 * @param hundertstel Gesamtzeit in Hundertstelsekunden (1 Hundertstel = 10 ms)
 * @param x X-Koordinate auf dem Bildschirm, wo die Zeit angezeigt werden soll
 * @param y Y-Koordinate auf dem Bildschirm, wo die Zeit angezeigt werden soll
 *
 * Diese Funktion wandelt eine gegebene Zeit in Hundertstelsekunden in ein
 * lesbares Format "MM:SS.CC" um (Minuten, Sekunden, Hundertstel) und zeigt
 * dieses Format an der angegebenen Position auf dem Bildschirm an.
 *
 * Wenn der Chronometer aktiv ist (chrono_laeuft == 1), wird zusätzlich der Text
 * "Chrono läuft" oben auf dem Bildschirm angezeigt.
 */
void zeit_anzeigen_at(uint32_t hundertstel, uint16_t x, uint16_t y) {
    uint32_t minuten = hundertstel / 6000;
    uint32_t sekunden = (hundertstel % 6000) / 100;
    uint32_t zehntelsek = hundertstel % 100;

    char ausgabe[16];
    snprintf(ausgabe, sizeof(ausgabe), ": %02lu:%02lu.%02lu", minuten, sekunden, zehntelsek);
    lcd_draw_text_at_coord(ausgabe, x, y, BLACK, 2, WHITE);

    if (chrono_laeuft) {
        lcd_draw_text_at_coord("Chrono laeuft", 10, 10, BLACK, 2, WHITE);
    }
}

/**
 * @brief Zeichnet die Steuerungsschaltflächen (Reset und Start) auf dem LCD-Bildschirm.
 *
 * Diese Funktion zeichnet zwei farbige Rechtecke auf dem LCD, die die "Reset"- (gelb)
 * und "Start"-Schaltflächen (grün) darstellen.
 */
void dessine_boutons(){

	 // Bouton Reset (jaune)
	    lcd_draw_rect(14, 220, 118, 260, YELLOW , 1); // Jaune
	    lcd_draw_text_at_coord("Reset", 40, 230, WHITE, 2, YELLOW );

	    // Bouton Start (vert)
	    lcd_draw_rect(140, 220, 234, 260, GREEN, 1); // Vert
	    lcd_draw_text_at_coord("Start", 160, 230, WHITE, 2, GREEN);


}
