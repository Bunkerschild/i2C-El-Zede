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
 
#define ELZEDE_ROWS			4
#define ELZEDE_COLS			20

#define ELZEDE_START_ADDR_ROW1		0x80
#define ELZEDE_START_ADDR_ROW2		0xC0
#define ELZEDE_START_ADDR_ROW3		0x94
#define ELZEDE_START_ADDR_ROW4		0xD4

#define ELZEDE_SEND_DATA		0x01
#define ELZEDE_SEND_CMD			0x00

#define ELZEDE_CMD_CLEAR_LCD_AND_RAM	0x01
#define ELZEDE_CMD_CLEAR_LCD_ONLY	0x08
#define ELZEDE_CMD_MODE_4_BIT		0x28
#define ELZEDE_CMD_MODE_8_BIT		0x38
#define ELZEDE_CMD_CURSOR_BLINK		0x0F
#define ELZEDE_CMD_CURSOR_UNDERLINE	0x0E
#define ELZEDE_CMD_CURSOR_OFF		0x0C
#define ELZEDE_CMD_CURSOR_MOVE_RIGHT	0x14
#define ELZEDE_CMD_CURSOR_MOVE_LEFT	0x10
#define ELZEDE_CMD_CURSOR_POS_RESET	0x20
#define ELZEDE_CMD_SCROLL_LCD_LEFT	0x1E
#define ELZEDE_CMD_SCROLL_LCD_RIGHT	0x18
#define ELZEDE_CMD_BACKLIGHT_ON		0x08
#define ELZEDE_CMD_BACKLIGHT_OFF	0x00
#define ELZEDE_CMD_ENTRY_MODE		0x06

#define ELZEDE_BITS_ENABLE		0b00000100
#define ELZEDE_BITS_DISABLE		0b00000010
#define ELZEDE_BITS_REGISTER_SELECT	0b00000001

#define ELZEDE_INIT_SEQ1		0x33
#define ELZEDE_INIT_SEQ2		0x32

#define ELZEDE_DELAY			500
#define ELZEDE_PULSE			500

#define ELZEDE_BYTE_BITMASK		0xF0

class el_zede
{
  private:
    unsigned char i2c_addr;
    i2c_device *i2c_dev;
    bool elzede_initialized;
    void elzede_error(const char* error_msg, int error_code);
    char *elzede_error_msg;
    int elzede_error_code;
    void elzede_toggle_enable(unsigned char bits);
    unsigned int elzede_get_end_addr(unsigned int start_addr);
    void elzede_byte(unsigned char bits, unsigned char mode);
    void elzede_command(unsigned char cmd);
    bool elzede_check_connection();
    
  public:
    void elzede_create(unsigned char i2c_addr, i2c_device *i2c_dev);
    void elzede_init();
    int elzede_get_error_code();
    char *elzede_get_error_msg();
    bool elzede_is_initialized();
    void elzede_string(const char *str);
    void elzede_clear_lcd_and_ram();
    void elzede_clear_lcd_only();
    void elzede_mode_4_bit();
    void elzede_mode_8_bit();
    void elzede_cursor_blink();
    void elzede_cursor_underline();
    void elzede_cursor_off();
    void elzede_cursor_move_right();
    void elzede_cursor_move_left();
    void elzede_cursor_pos_reset();
    void elzede_scroll_lcd_left();
    void elzede_scroll_lcd_right();
    void elzede_backlight_on();
    void elzede_backlight_off();
    void elzede_entry_mode();
    bool elzede_set_cursor_position(unsigned int row, unsigned int col);
};
