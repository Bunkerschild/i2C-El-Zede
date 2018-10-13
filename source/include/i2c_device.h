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
 
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
 
class i2c_device
{
    private:
        int i2c_handle;
        unsigned int i2c_addr;
        const int i2c_read_length = 4;
        bool i2c_connected;
        char *i2c_error_msg;
        int i2c_error_code;
        unsigned char i2c_buffer[60];
        void i2c_init_buffer();
        void i2c_error(const char *error_msg, int error_code);
        bool i2c_set_address(unsigned char address);
        
    public:
        i2c_device(int i2c_device_id);
        int i2c_get_error_code();
        char *i2c_get_error_msg();
        unsigned char *i2c_get_buffer();
        bool i2c_is_connected();
        bool i2c_read();
        bool i2c_write(unsigned char address, unsigned char byte);
};

 