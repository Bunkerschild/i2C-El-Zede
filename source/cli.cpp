/******************************************************************************    
* This file is part of the i2C-El-Zede project.                               *
* See https://github.com/Bunkerschild/i2C-El-Zede for details                 *
*                                                                             *
*   Copyright (C) 2018 Oliver Welter <info@bunkerschild.de>                   *
*                                                                             *
*   This program is free software: you can redistribute it and/or modify      *
*   it under the terms of the GNU General Public License as published by      *
*   the Free Software Foundation, either version 3 of the License, or         *
*   (at your option) any later version.                                       *
*                                                                             *
*   This program is distributed in the hope that it will be useful,           *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
*   GNU General Public License for more details.                              *
*                                                                             *
*   You should have received a copy of the GNU General Public License         *
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.    *
******************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "include/i2c_device.h"
#include "include/el_zede.h"

bool initialized = false;
bool connected = false;
bool debug = false;

unsigned int i2c_address = 0x27;
unsigned int i2c_dev_id = 1;

unsigned int position_row = 0;
unsigned int position_col = 0;

el_zede *lcd_util = new el_zede;

struct Table 
{
    long long tab[128];
  
    constexpr Table() : tab {} 
    {
      tab['1'] = 1;
      tab['2'] = 2;
      tab['3'] = 3;
      tab['4'] = 4;
      tab['5'] = 5;
      tab['6'] = 6;
      tab['7'] = 7;
      tab['8'] = 8;
      tab['9'] = 9;
      tab['a'] = 10;
      tab['A'] = 10;
      tab['b'] = 11;
      tab['B'] = 11;
      tab['c'] = 12;
      tab['C'] = 12;
      tab['d'] = 13;
      tab['D'] = 13;
      tab['e'] = 14;
      tab['E'] = 14;
      tab['f'] = 15;
      tab['F'] = 15;
    }
    
    constexpr long long operator[](char const idx) const 
    { 
     return tab[(std::size_t) idx]; 
    } 
} constexpr table;

constexpr int hextoint(char number) 
{
    return table[(std::size_t)number];
}

unsigned int hextoint(const char *token)
{
    unsigned int retval = 0;
    unsigned int tokval = 0;
    unsigned int factor = 0;
    unsigned int multiplier = 0;
    
    for (int i = strlen(token); i > 0; i--)
    {
      tokval = hextoint(token[i-1]);
      multiplier = pow(16, factor);
      
      if (multiplier == 0)
        multiplier = 1;
        
      factor++;
      
      retval += (tokval * multiplier);
    }
    
    return retval;
}

void copyright()
{
    printf("i2C-El-Zede - Copyright (C) 2018 Oliver Welter <info@bunkerschild.de>\n");
}

int help()
{
    printf("CURRENTLY NO HELP IMPLEMENTED, YET :-(\n");
    
    return 2;
}

void gpl()
{
    printf("This program is free software: you can redistribute it and/or modify\n");
    printf("it under the terms of the GNU General Public License as published by\n");
    printf("the Free Software Foundation, either version 3 of the License, or\n");
    printf("(at your option) any later version.\n\n");
    printf("This program is distributed in the hope that it will be useful,\n");
    printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
    printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
    printf("GNU General Public License for more details.\n\n");
    printf("You should have received a copy of the GNU General Public License\n");
    printf("along with this program.  If not, see <https://www.gnu.org/licenses/>.\n\n");
    
    copyright();
}

int usage(const char *name)
{
    printf("Usage: %s --help\n", name);
    return 1;
}

int argument_error(const char *arg, const char *name)
{
    printf("Missing or invalid argument to %s\n", arg);
    usage(name);
    
    return 3;
}

int info()
{
    copyright();
    
    printf("\nI2C-address: 0x%02x (/dev/i2c-%i)\n", i2c_address, i2c_dev_id);
    
    // Currently hardcoded - todo: make this flexible
    printf("LCD-Cols: %i\n", ELZEDE_COLS);
    printf("LCD-Rows: %i\n", ELZEDE_ROWS);
    
    return 4;
}

void create()
{
  i2c_device *lcd_device = new i2c_device;

  lcd_device->i2c_create(i2c_dev_id);

  if (lcd_device->i2c_get_error_code() > 0)
  {
    printf("I2C-ERROR: %s\n", lcd_device->i2c_get_error_msg());
    exit(lcd_device->i2c_get_error_code());
  }
  
  lcd_util->elzede_create(i2c_address, lcd_device);

  if (lcd_util->elzede_get_error_code() > 0)
  {
    printf("LCD-ERROR: %s\n", lcd_util->elzede_get_error_msg());
    exit(lcd_util->elzede_get_error_code());
  }  
}

void init()
{  
  position_row = 1;
  position_col = 1;
  
  lcd_util->elzede_init();
  
  initialized = true;
}

int uninitialized()
{
  printf("ERROR: You cannot use this argument, if LCD is uninitialized\n");
  
  return 6;
}

int main (int argc, char **argv)
{
    if (argc < 2)
    {
        gpl();

        return usage(argv[0]);
    }
    
    int i;
        
    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--help"))
        {
           copyright();
           
           return help();
        }
        else if (!strcmp(argv[i], "--license"))
        {
           gpl();
           
           return 0;
        }
        else if (!strcmp(argv[i], "--info"))
        {
           return info();
        }
        else if (!strcmp(argv[i], "--debug"))
        {
           debug = true;
        }
        else if (!strcmp(argv[i], "--device-id"))
        {
          i++;
          
          if (i >= argc)
            return argument_error(argv[i-1], argv[0]);
            
          i2c_dev_id = (unsigned int)atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "--init-override"))
        {
          initialized = true;
        }
    }
    
    if (debug)
        printf("DEBUGGING ENABLED\n=================\nI2C address is 0x%02x (/dev/i2c-%i)\n", i2c_address, i2c_dev_id);
        
    create();
    
    if (debug && initialized)          
        printf("Initialization override - marked as initialized\n");
          
    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--initialize"))
        {
           if (!initialized)
           {
              if (debug) printf("Initializing hardware\n");
              
              init();
           }
           else
           {
              if (debug) printf("Hardware already initialized - no further initialization required\n");
           }
        }
        else if (!strcmp(argv[i], "--i2c-address"))
        {
          i++;
          
          if (i >= argc)
            return argument_error(argv[i-1], argv[0]);
          
          char *tok = strtok(argv[i], "x");
          tok = strtok(NULL, "x");
            
          if (!tok)
          {
            i2c_address = (unsigned int)atoi(argv[i]);
          }
          else
          {
            i2c_address = (unsigned int)hextoint(tok);
          }
          
          if (debug) printf("Set I2C address to 0x%02x (/dev/i2c-%i)\n", i2c_address, i2c_dev_id);
        }
        else if (!strcmp(argv[i], "--set-cursor-position"))
        {
          if (!initialized)
            return uninitialized();
          
          unsigned int row = 0;
          unsigned int col = 0;
          
          i++;
          
          if (i >= argc)
            return argument_error(argv[i-1], argv[0]);
            
          row = (unsigned int)atoi(argv[i]);

          i++;
          
          if (i >= argc)
            return argument_error(argv[i-2], argv[0]);
            
          col = (unsigned int)atoi(argv[i]);
          
          if (col > ELZEDE_COLS)
            return argument_error(argv[i-2], argv[0]);
          
          if (row > ELZEDE_ROWS)
            return argument_error(argv[i-2], argv[0]);
            
          if (debug) printf("Set cursor position to row %i and col %i\n", row, col);
          
          position_col = col;
          position_row = row;
          
          lcd_util->elzede_set_cursor_position(position_row, position_col-1);
        }
        else if (!strcmp(argv[i], "--entry-mode"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set entry mode\n");
            
          lcd_util->elzede_entry_mode();
        }
        else if (!strcmp(argv[i], "--backlight-off"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set backlight off\n");
          
          lcd_util->elzede_backlight_off();
        }
        else if (!strcmp(argv[i], "--backlight-on"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set backlight on\n");
          
          lcd_util->elzede_backlight_on();
        }
        else if (!strcmp(argv[i], "--scroll-lcd-right"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set scroll LCD right\n");
          
          lcd_util->elzede_scroll_lcd_right();
        }
        else if (!strcmp(argv[i], "--scroll-lcd-left"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set scroll LCD left\n");
          
          lcd_util->elzede_scroll_lcd_left();
        }
        else if (!strcmp(argv[i], "--cursor-pos-reset"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set cursor position reset\n");
          
          position_row = 1;
          position_col = 1;
          
          lcd_util->elzede_cursor_pos_reset();
        }
        else if (!strcmp(argv[i], "--cursor-move-left"))
        {
          if (!initialized)
            return uninitialized();
            
          unsigned int col = position_col;
          unsigned int row = position_row;
          
          col--;
          
          if (col == 0)
          {
            col = ELZEDE_COLS;
            row--;
            
            if (row == 0)
            {
              row = ELZEDE_ROWS;
            }
          }
            
          if (debug) printf("Set cursor move left to col %i and row %i\n", col, row);
          
          position_row = row;
          position_col = col;
          
          lcd_util->elzede_cursor_move_left();
        }
        else if (!strcmp(argv[i], "--cursor-move-right"))
        {
          if (!initialized)
            return uninitialized();
            
          unsigned int col = position_col;
          unsigned int row = position_row;
          
          col++;
          
          if (col == (ELZEDE_COLS + 1))
          {
            col = 1;
            row++;
            
            if (row == (ELZEDE_ROWS + 1))
            {
              row = 1;
            }
          }
            
          if (debug) printf("Set cursor move left to col %i and row %i\n", col, row);
          
          position_row = row;
          position_col = col;
          
          lcd_util->elzede_cursor_move_right();
        }
        else if (!strcmp(argv[i], "--cursor-off"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set cursor off\n");
          
          lcd_util->elzede_cursor_off();
        }
        else if (!strcmp(argv[i], "--cursor-underline"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set cursor underline\n");
          
          lcd_util->elzede_cursor_underline();
        }
        else if (!strcmp(argv[i], "--cursor-blink"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set cursor blink\n");
          
          lcd_util->elzede_cursor_blink();
        }
        else if (!strcmp(argv[i], "--mode-8-bit"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set mode 8 bit\n");
          
          lcd_util->elzede_mode_8_bit();
        }
        else if (!strcmp(argv[i], "--mode-4-bit"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set mode 4 bit\n");
          
          lcd_util->elzede_mode_4_bit();
        }
        else if (!strcmp(argv[i], "--clear-lcd-only"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set clear LCD only\n");
          
          lcd_util->elzede_clear_lcd_only();
        }
        else if (!strcmp(argv[i], "--clear-lcd-and-ram"))
        {
          if (!initialized)
            return uninitialized();
            
          if (debug) printf("Set clear LCD and RAM\n");
          
          lcd_util->elzede_clear_lcd_and_ram();
        }
        else if (!strcmp(argv[i], "--string"))
        {
          if (!initialized)
            return uninitialized();
            
          i++;
          
          if (i >= argc)
            return argument_error(argv[i-1], argv[0]);
            
          const char *str = (char *)argv[i];
            
          if (debug) printf("Set string: %s\n", str);
          
          unsigned int row = position_row;
          unsigned int col = position_col;
          
          for (int i = 0; i < strlen(str); i++)
          {
            col++;
            
            if (col == (ELZEDE_COLS + 1))
            {
              col = 1;
              row++;
              
              if (row == (ELZEDE_ROWS + 1))
              {
                row = 1;
              }
            }
          }
          
          if (debug) printf("New cursor position is at col %i and row %i\n", col, row);
          
          position_row = row;
          position_col = col;
          
          lcd_util->elzede_string(argv[i]);
        }
    }
}
