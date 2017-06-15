/* 40 x 4 LCD Library 
  
   Author: scandox@gmail.com

   This example code is in the Public Domain

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#ifndef LCD_DUAL
#define LCD_DUAL

#include "lcd.h"

struct lcd_screen_position {
  int col;
  int row;
  int displayNum;
};

struct lcd_screen {
  struct lcd_display * display[2];
  int currentCol;
  int currentRow;
};

/* This pin structure is based on 4 bit communication with the LCD
   which is dictated by the lcd library (lcd.h) above

   A 40x4 LCD is basically 2 20x4 LCDs joined together
   
   The RS and DATA pins are the same for both. However the second display requires
   a separate enable PIN

*/
struct lcd_screen_pins {
  int rs;
  int en;
  int d0; // remember could be d4
  int d1; // ... d5
  int d2; // ... d6
  int d3; // ... d7

  // The second Enable for second display
  int en2;
};

struct lcd_screen * lcd_screen_init(struct lcd_screen_pins p);
void lcd_screen_destroy(struct lcd_screen * screen);
void lcd_screen_clear(struct lcd_screen * screen);
struct lcd_screen_position set_screen_position(struct lcd_screen * screen, int col, int row);
void lcd_screen_write_char(struct lcd_screen * screen, char letter);
void lcd_screen_write_text(struct lcd_screen * screen, char * text);

#endif
