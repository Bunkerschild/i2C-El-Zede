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

#include "include/i2c_device.h"
#include "include/el_zede.h"

void el_zede::elzede_create(unsigned char i2c_addr, i2c_device *i2c_dev)
{
  this->i2c_addr = i2c_addr;
  this->i2c_dev = i2c_dev;
  
  elzede_check_connection();
}

bool el_zede::elzede_check_connection()
{
  if (!i2c_dev->i2c_is_connected())
  {
    elzede_error(i2c_dev->i2c_get_error_msg(), i2c_dev->i2c_get_error_code());
    this->elzede_initialized = false;
    
    return false;
  }
  
  return true;
}

void el_zede::elzede_init()
{
  if (!elzede_check_connection())
  {
    return;
  }
  else if (!this->elzede_initialized)
  {
    elzede_command(ELZEDE_INIT_SEQ1);
    elzede_command(ELZEDE_INIT_SEQ2);

    elzede_cursor_off();
    elzede_clear_lcd_and_ram();
    elzede_entry_mode();
    elzede_mode_4_bit();
    
    this->elzede_initialized = true;
    
    usleep(ELZEDE_DELAY);  
  }
}

unsigned int el_zede::elzede_get_end_addr(unsigned int start_addr)
{
  return (start_addr + (ELZEDE_COLS - 1));
}

void el_zede::elzede_toggle_enable(unsigned char bits)
{
  usleep(ELZEDE_DELAY);
  
  i2c_dev->i2c_write(this->i2c_addr, (bits | ELZEDE_BITS_ENABLE));
  usleep(ELZEDE_PULSE);
  
  i2c_dev->i2c_write(this->i2c_addr, (bits & ~ELZEDE_BITS_ENABLE));
  usleep(ELZEDE_DELAY);
}

void el_zede::elzede_byte(unsigned char bits, unsigned char mode)
{
  unsigned char high_bits;
  unsigned char low_bits;
  
  high_bits = mode | (bits & ELZEDE_BYTE_BITMASK) | ELZEDE_CMD_BACKLIGHT_ON;
  low_bits = mode | ((bits<<4) & ELZEDE_BYTE_BITMASK) | ELZEDE_CMD_BACKLIGHT_ON;
  
  i2c_dev->i2c_write(this->i2c_addr, high_bits);
  elzede_toggle_enable(high_bits);
  
  i2c_dev->i2c_write(this->i2c_addr, low_bits);
  elzede_toggle_enable(low_bits);
}

void el_zede::elzede_command(unsigned char cmd)
{
  elzede_byte(cmd, ELZEDE_SEND_CMD);
}

void el_zede::elzede_string(const char *str)
{
  for (int i = 0; i < strlen(str); i++)
  {
    elzede_byte(str[i], ELZEDE_SEND_DATA);
  }
}

void el_zede::elzede_error(const char *error_msg, int error_code)
{
  this->elzede_error_msg = (char*)error_msg;
  this->elzede_error_code = error_code;
}

int el_zede::elzede_get_error_code()
{
  return this->elzede_error_code;
}

char *el_zede::elzede_get_error_msg()
{
  return this->elzede_error_msg;
}

bool el_zede::elzede_is_initialized()
{
  return this->elzede_initialized;
}

void el_zede::elzede_clear_lcd_and_ram()
{
  elzede_command(ELZEDE_CMD_CLEAR_LCD_AND_RAM);
}

void el_zede::elzede_clear_lcd_only()
{
  elzede_command(ELZEDE_CMD_CLEAR_LCD_ONLY);
}

void el_zede::elzede_mode_4_bit()
{
  elzede_command(ELZEDE_CMD_MODE_4_BIT);
}

void el_zede::elzede_mode_8_bit()
{
  elzede_command(ELZEDE_CMD_MODE_8_BIT);
}

void el_zede::elzede_cursor_blink()
{
  elzede_command(ELZEDE_CMD_CURSOR_BLINK);
}

void el_zede::elzede_cursor_underline()
{
  elzede_command(ELZEDE_CMD_CURSOR_UNDERLINE);
}

void el_zede::elzede_cursor_off()
{
  elzede_command(ELZEDE_CMD_CURSOR_OFF);
}

void el_zede::elzede_cursor_move_right()
{
  elzede_command(ELZEDE_CMD_CURSOR_MOVE_RIGHT);
}

void el_zede::elzede_cursor_move_left()
{
  elzede_command(ELZEDE_CMD_CURSOR_MOVE_LEFT);
}

void el_zede::elzede_cursor_pos_reset()
{
  elzede_command(ELZEDE_CMD_CURSOR_POS_RESET);
}

void el_zede::elzede_scroll_lcd_left()
{
  elzede_command(ELZEDE_CMD_SCROLL_LCD_LEFT);
}

void el_zede::elzede_scroll_lcd_right()
{
  elzede_command(ELZEDE_CMD_SCROLL_LCD_RIGHT);
}

void el_zede::elzede_backlight_on()
{
  elzede_command(ELZEDE_CMD_BACKLIGHT_ON);
}

void el_zede::elzede_backlight_off()
{
  elzede_command(ELZEDE_CMD_BACKLIGHT_OFF);
}

void el_zede::elzede_entry_mode()
{
  elzede_command(ELZEDE_CMD_ENTRY_MODE);
}

bool el_zede::elzede_set_cursor_position(unsigned int row, unsigned int col)
{
  unsigned int start_position = 0;
  unsigned int end_position = 0;
  unsigned int cur_position = 0;
  
  if (col > ELZEDE_COLS)
  {
    elzede_error("Col position out of bounds.", 99);
    return false;
  }
  
  switch (row)
  {
    case 1:
      start_position = ELZEDE_START_ADDR_ROW1;
      break;
    case 2:
      start_position = ELZEDE_START_ADDR_ROW2;
      break;
    case 3:
      start_position = ELZEDE_START_ADDR_ROW3;
      break;
    case 4:
      start_position = ELZEDE_START_ADDR_ROW4;
      break;
    default:
      elzede_error("Row position out of bounds.", 100);
      return false;
  }
  
  end_position = elzede_get_end_addr(start_position);
  cur_position = (start_position+(col));
  
  if (cur_position > end_position)
  {
    elzede_error("Col exceeds end position.", 101);
    return false;
  }
  
  elzede_command(cur_position);
  return true;
}
