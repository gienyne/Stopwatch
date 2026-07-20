/**
 **************************************************
 * @file        my_lcd.h
 * @author      Dimitry Ntofeu Nyatcha
 * @version     v1.0
 * @date        20.04.2025
 * @brief       Header-Datei für das LCD-Hilfsmodul (Balkendiagramm und Kreuz)
 **************************************************
 */

#ifndef MY_LCD_MY_LCD_H_
#define MY_LCD_MY_LCD_H_

void my_lcd_draw_baargraph(uint16_t x, uint16_t y, uint16_t width,uint16_t height, uint16_t value, uint16_t Colour, uint16_t bg_colour);

void draw_diag_cross(uint16_t x, uint16_t y, uint16_t color);
#endif /* MY_LCD_MY_LCD_H_ */
