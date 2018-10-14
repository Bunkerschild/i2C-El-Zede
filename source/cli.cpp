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
#include "include/LiquidCrystal.h"

bool initialized = false;
bool connected = false;
bool debug = false;

unsigned int i2c_address = 0x27;
unsigned int i2c_dev_id = 1;

unsigned int display_cols = 20;
unsigned int display_rows = 4;
unsigned int display_dotsize = LCD_5x8DOTS;

LiquidCrystal *lcd_util = new LiquidCrystal;

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
  
  lcd_util->create(i2c_address, lcd_device, display_cols, display_rows, display_dotsize);
}

void init()
{  
  lcd_util->init();
  
  initialized = true;
}

int uninitialized()
{
  printf("ERROR: You cannot use this argument, if LCD is uninitialized\n");
  
  return 6;
}

int help(const char *name)
{    
  printf("This tool is designed to be an interface for use with\n");
  printf("I2C DOT matrix displays, like 20x4, typically used in\n");
  printf("conjunction with arduino like boards. It makes it possible\n");
  printf("to control compatible displays from the command line,\n");
  printf("directly attached to the computers I2C bus.\n\n");
  printf("The following switches are used for informational purposes:\n");
  printf("===========================================================\n");
  printf(" --help                              Show this help context\n");
  printf(" --license                           Show license context\n");
  printf(" --info                              Show information about configured defaults\n");
  printf(" --debug                             Enable debugging information to STDOUT\n\n");
  printf("Only one of the following switches is required as the first argument:\n");
  printf("=====================================================================\n");
  printf(" --initialize                        Initialize LCD after connecting to I2C bus\n");
  printf(" --no-init                           Do not initialize LCD after connection\n\n");
  printf("The following switches are used to set I2C device ID and LCD address:\n");
  printf("=====================================================================\n");
  printf(" --device-id <ID>                    Set the I2C device ID - defaults to 1\n");
  printf(" --i2c-address <VAL>                 I2C address as integer or hex - defaults to 0x27\n\n");
  printf("The following switches are sequencable used to control the LCD behaviour:\n");
  printf("=========================================================================\n");
  printf(" --wait <ms>                         Waittime in ms until next switch is used\n");
  printf(" --clear                             Clear LCD screen\n");
  printf(" --home                              Set cursor to row 1 and column 1\n");
  printf(" --display                           Display content in RAM\n");
  printf(" --no-display                        Hide display content\n");
  printf(" --blink                             Let the cursor blink\n");
  printf(" --no-blink                          Stop cursor blinking\n");
  printf(" --cursor                            Display cursor\n");
  printf(" --no-cursor                         Hide cursor\n");
  printf(" --scroll-display-left               Scroll display content to the left\n");
  printf(" --scroll-display-right              Scroll display content to the right\n");
  printf(" --left-to-right                     Character direction left to right\n");
  printf(" --right-to-left                     Character direction right to left\n");
  printf(" --backlight                         Enable display backlight\n");
  printf(" --no-backlight                      Disable display backlight\n");
  printf(" --autoscroll                        Enable autoscrolling of content\n");
  printf(" --no-autoscroll                     Disable autoscrolling of content\n");
  printf(" --create-char <ID> <VAL,VAL,...>    Create a character in hex or integer values at ID\n");
  printf(" --set-cursor <row> <column>         Place cursor for next character at row and column\n");
  printf(" --write <VAL>                       Write character as integer or hex at current position\n");
  printf(" --print-str <string>                Print a string to current cursor position\n\n");
  printf("Examples:\n");
  printf("=========\n");
  printf("$ %s --initialize --print-str \"I love Hello World examples\"\n\n", name);
  printf("On a 20x4 LCD it will result in:\n");
  printf("+--------------------+\n");
  printf("|I love Hello World e|\n");
  printf("|xamples             |\n");
  printf("|                    |\n");
  printf("|                    |\n");
  printf("+--------------------+\n\n");
  printf("$ %s --no-init --set-cursor 4 3 --print-str \"STUFF\" --set-cursor 3 6 --print-str \"COOL\"\n\n", name);
  printf("as a second command will result in:\n");
  printf("+--------------------+\n");
  printf("|I love Hello World e|\n");
  printf("|xamples             |\n");
  printf("|     COOL           |\n");
  printf("|  STUFF             |\n");
  printf("+--------------------+\n\n");  
  copyright();
  return 2;
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
           return help(argv[0]);
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
        else if (!strcmp(argv[i], "--no-init"))
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
        else if (!strcmp(argv[i], "--wait"))
        {
          unsigned int ms = 0;
          
          i++;
          
          if (i >= argc)
            return argument_error(argv[i-1], argv[0]);
            
          ms = (unsigned int)atoi(argv[i]);
          
          if (debug) printf("Wait %i ms\n", ms);
          
          usleep(ms);
        }
        else if (!strcmp(argv[i], "--clear"))
        {
          lcd_util->clear();
          
          if (debug) printf("Clear display\n");
        }
        else if (!strcmp(argv[i], "--home"))
        {
          lcd_util->home();
          
          if (debug) printf("Home position\n");
        }
        else if (!strcmp(argv[i], "--display"))
        {
          lcd_util->display();
          
          if (debug) printf("Display content in RAM\n");
        }
        else if (!strcmp(argv[i], "--no-display"))
        {
          lcd_util->noDisplay();
          
          if (debug) printf("Hide display content\n");
        }
        else if (!strcmp(argv[i], "--blink"))
        {
          lcd_util->blink();
          
          if (debug) printf("Set blink on\n");
        }
        else if (!strcmp(argv[i], "--no-blink"))
        {
          lcd_util->noBlink();
          
          if (debug) printf("Set blink off\n");
        }
        else if (!strcmp(argv[i], "--cursor"))
        {
          lcd_util->cursor();
          
          if (debug) printf("Set cursor on\n");
        }
        else if (!strcmp(argv[i], "--no-cursor"))
        {
          lcd_util->noCursor();
          
          if (debug) printf("Set cursor off\n");
        }
        else if (!strcmp(argv[i], "--scroll-display-left"))
        {
          lcd_util->scrollDisplayLeft();
          
          if (debug) printf("Scroll display to the left\n");
        }
        else if (!strcmp(argv[i], "--scroll-display-right"))
        {
          lcd_util->scrollDisplayRight();
          
          if (debug) printf("Scroll display to the right\n");
        }
        else if (!strcmp(argv[i], "--left-to-right"))
        {
          lcd_util->leftToRight();
          
          if (debug) printf("Left to right mode\n");
        }
        else if (!strcmp(argv[i], "--right-to-left"))
        {
          lcd_util->rightToLeft();
          
          if (debug) printf("Right to left mode\n");
        }
        else if (!strcmp(argv[i], "--backlight"))
        {
          lcd_util->backlight();
          
          if (debug) printf("Backlight on\n");
        }
        else if (!strcmp(argv[i], "--no-backlight"))
        {
          lcd_util->noBacklight();
          
          if (debug) printf("Backlight off\n");
        }
        else if (!strcmp(argv[i], "--autoscroll"))
        {
          lcd_util->autoscroll();
          
          if (debug) printf("Autoscroll on\n");
        }
        else if (!strcmp(argv[i], "--no-autoscroll"))
        {
          lcd_util->noAutoscroll();
          
          if (debug) printf("Autoscroll off\n");
        }
        else if (!strcmp(argv[i], "--create-char"))
        {
          unsigned int id = 0;
          
          if (debug) printf("Create character at ID %i\n", id);
        }
        else if (!strcmp(argv[i], "--set-cursor"))
        {
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
          
          lcd_util->setCursor(row, col);
          
          if (debug) printf("Set cursor to row %i and column %i\n", row, col);
        }
        else if (!strcmp(argv[i], "--write"))
        {
          unsigned int chr = 0;
          
          i++;
          
          if (i >= argc)
            return argument_error(argv[i-1], argv[0]);
          
          char *tok = strtok(argv[i], "x");
          tok = strtok(NULL, "x");
            
          if (!tok)
          {
            chr = (unsigned int)atoi(argv[i]);
          }
          else
          {
            chr = (unsigned int)hextoint(tok);
          }
          
          lcd_util->write(chr);
                    
          if (debug) printf("Print character 0x%02x\n", chr);
        }
        else if (!strcmp(argv[i], "--print-str"))
        {
          char *str = {0};
          unsigned int chr = 0;
          
          i++;
          
          if (i >= argc)
            return argument_error(argv[i-1], argv[0]);
            
          str = (char *)argv[i];
          
          for (int i = 0; i < strlen(str); i++)
          {
            chr = (unsigned int)str[i];
            lcd_util->write(chr);
          }
          
          if (debug) printf("Print string: '%s'\n", str);
        }
    }
}
