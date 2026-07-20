/**
 **************************************************
 * @file        my_lcd.c
 * @author      Dimitry Ntofeu Nyatcha
 * @version     v1.0
 * @date        20.04.2025
 * @brief       Modul für unsere eigene LCD-Funktionen (Balkendiagramm, Kreuz)
 @verbatim
 ==================================================
            ### Verwendete Ressourcen ###
    LCD: ILI9341 Controller aslo: ILI9341_Draw_Filled_Rectangle_Coord()
 ==================================================
            ###     Verwendung      ###
    (#) Aufruf von 'my_lcd_draw_baargraph()', um ein horizontales Balkendiagramm zu zeichnen.
    (#) Aufruf von 'draw_diag_cross()', um ein diagonales Kreuz auf das Display zu zeichnen.
 ==================================================
 @endverbatim
 **************************************************
 */

#include <lcd/lcd.h>
#include "my_lcd.h"

/**
 * @brief  Zeichnet ein horizontales Balkendiagramm basierend auf Wert und Hintergrundfarbe
 * @param  x         Startposition x
 * @param  y         Startposition y
 * @param  width     Breite des gesamten Balkens
 * @param  height    Höhe des Balkens
 * @param  value     Wert zwischen 0 und 1000 (Promille)
 * @param  Color     Füllfarbe des aktiven Bereichs
 * @param  bg_color  Hintergrundfarbe des inaktiven Bereichs
 * @return Keine
 */
void my_lcd_draw_baargraph(uint16_t x, uint16_t y, uint16_t width,uint16_t height, uint16_t value, uint16_t Color, uint16_t bg_color){

	if (value > 1000){value = 1000;}

	int fill = (width * value) / 1000;

	ILI9341_Draw_Filled_Rectangle_Coord(x + fill, y - height, x + width, y, Color);

	ILI9341_Draw_Filled_Rectangle_Coord( x, y - height, x + fill, y, bg_color);



}

/**
 * @brief  Zeichnet ein diagonales Kreuz auf das Display
 * @param  x     Startposition x
 * @param  y     Startposition y
 * @param  color Farbe der Linien
 * @return Keine
 */
void draw_diag_cross(uint16_t x, uint16_t y, uint16_t color){
for (int i = 0; i < 100; i++){
	 lcd_draw_pixel( x + 50 , y + i, color);
	 lcd_draw_pixel( x + i , y + 50, color);
}
}


