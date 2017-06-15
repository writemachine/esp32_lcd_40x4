/* 40 x 4 LCD Library
  
   Author: scandox@gmail.com

   This example code is in the Public Domain

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdlib.h>
#include <lcd.h>
#include <lcd_dual.h>

/************** INTERNAL FUNCTIONS **********************/

/* Takes a position in a 40 x 4 grid and translates it
   into the real position across the two lcd displays

   Displays take this shape:

   ---0--4---------------------------------------39--
   -0------------Disp1/Row0-----------Disp1/Row2-----
   -1------------Disp1/Row1-----------Disp1/Row3-----
   -2------------Disp2/Row0-----------Disp2/Row2-----
   -3------------Disp2/Row1-----------Disp2/Row3-----
   --------------------------------------------------

   Note - that's not to scale :) in any sense

*/
struct lcd_screen_position translate_position(int row, int col) {
  struct lcd_screen_position position;

  if (row < 0 || row > 3 || col < 0 || col > 39) return position;

  // Establish which display we're on
  if (row < 2) {
    position.displayNum = 0;
  }else{
    position.displayNum = 1;
  }

  // Establish column which is independent of row calc
  if (col <= 19) {
    position.col = col;
  }else{
    position.col = col - 20;
  }

  // Row depends on column
  if (row%2 == 0) {
    if (col <= 19) {
      position.row = 0;
    }else{
      position.row = 2;
    }
  }else{
    if (col <= 19) {
      position.row = 1;
    }else{
      position.row = 3;
    }
  }

  return position;
}

/* Writes a single character at current cursor position

*/
void write_char(struct lcd_screen * screen, int col, int row, char letter) {
  struct lcd_screen_position position = set_screen_position(screen, col, row);
  lcd_write_char(screen->display[position.displayNum], letter); 
}

/************** PUBLIC API **********************/

/* Initializes Dual display LCD Screen (40 x 4)

   Initializes 2 displays
   Sets cursor to 0,0
   Clears the display of any contents

   Default is cursor to non-blinking visible
*/
struct lcd_screen * lcd_screen_init(struct lcd_screen_pins p) {
 
  struct lcd_pins pins = { p.rs, p.en, p.d0, p.d1, p.d2, p.d3 }; 
  struct lcd_display * display1 = lcd_init(&pins);
 
  // Note: only difference is en2
  struct lcd_pins pins2 = { p.rs, p.en2, p.d0, p.d1, p.d2, p.d3 };
  struct lcd_display * display2 = lcd_init(&pins2);
  
  struct lcd_screen * screen = malloc(sizeof(struct lcd_screen));

  screen->display[0] = display1;
  screen->display[1] = display2;
  screen->currentCol = screen->currentRow = 0; 
  
  lcd_screen_clear(screen);

  // TODO: should I use our internal set_screen_position instead?
  lcd_set_cursor(screen->display[0], 0, 0); 

  return screen;
}

/* Frees memory associated with screen 

   Note: this does not clear physical display which remains in last state
*/
void lcd_screen_destroy(struct lcd_screen * screen) {
  lcd_destroy(screen->display[0]);
  lcd_destroy(screen->display[1]);
  free(screen);
}

/* Clears the screen 

*/
void lcd_screen_clear(struct lcd_screen * screen) {
  lcd_clear(screen->display[0]);
  lcd_clear(screen->display[1]);
}

/* Sets the cursor position on the screen

   Returns the translated position 
*/
struct lcd_screen_position set_screen_position(struct lcd_screen * screen, int col, int row) {
  struct lcd_screen_position position = translate_position(row, col);
  lcd_set_cursor(screen->display[position.displayNum], position.row, position.col);
  return position;
}

/* Writes a single character and determines and sets new cursor position

*/
void lcd_screen_write_char(struct lcd_screen * screen, char letter) {
  // Determine current display so we can handle change
  struct lcd_screen_position currentPosition = translate_position(screen->currentRow, screen->currentCol);
  
  // Determine col and row after char inserted
  int newCol = screen->currentCol + 1;
  int newRow;
  
  if (newCol > 39) {
    newCol = 0;
    newRow = screen->currentRow + 1;
  }else{ 
    newRow = screen->currentRow;
  }

  // Put the actual char
  write_char(screen, screen->currentCol, screen->currentRow, letter);

  screen->currentCol = newCol;
  screen->currentRow = newRow;
}

/* Writes a string

*/
void lcd_screen_write_text(struct lcd_screen * screen, char * text) {
  int i;
  for(i=0; text[i]!='\0'; i++) {
    lcd_screen_write_char(screen, text[i]);
  }
}
