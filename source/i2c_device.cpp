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
 
#include "include/i2c_device.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

using namespace std;

void i2c_device::i2c_create(int i2c_device_id)
{
  char i2c_device_node[16] = {0};
  
  sprintf(i2c_device_node, "/dev/i2c-%i", i2c_device_id);

  char *i2c_filename = (char*)i2c_device_node;
  
  if ((this->i2c_handle = open(i2c_filename, O_RDWR)) < 0)
  {
    this->i2c_connected = false;
    i2c_error("Unable to open I2C device.", 1);
  }
  else
  {
    this->i2c_connected = true;
  }
}

void i2c_device::i2c_error(const char *error_msg, int error_code)
{
  this->i2c_error_msg = (char*)error_msg;
  this->i2c_error_code = error_code;
}

int i2c_device::i2c_get_error_code()
{
  return this->i2c_error_code;
}

char *i2c_device::i2c_get_error_msg()
{
  return this->i2c_error_msg;
}

void i2c_device::i2c_init_buffer()
{
  memcpy(this->i2c_buffer, "\0", sizeof this->i2c_buffer);
}

bool i2c_device::i2c_read()
{  
  i2c_init_buffer();
  
  if (!this->i2c_connected)
  {
    i2c_error("Unable to read, cause of closed I2C handle.", 2);
    return false;
  }
  
  if (read(this->i2c_handle, this->i2c_buffer, this->i2c_read_length) != this->i2c_read_length)
  {
    i2c_error("Error, while reading from I2C bus.", 3);
    return false;
  }
  
  return true;
}

unsigned char * i2c_device::i2c_get_buffer()
{
  unsigned char *i2c_return_buffer = (unsigned char*)this->i2c_buffer;
  
  return i2c_return_buffer;
}

bool i2c_device::i2c_is_connected()
{
  return this->i2c_connected;
}

bool i2c_device::i2c_set_address(unsigned char address)
{
  if (!this->i2c_connected)
  {
    i2c_error("Unable to set address, cause of closed I2C handle.", 4);
    return false;
  }
  
  if (ioctl(this->i2c_handle, I2C_SLAVE, address) < 0)
  {
    i2c_error("Bus access denied or invalid address error.", 5);
    return false;
  }
  
  this->i2c_addr = address;
  return true;
}

bool i2c_device::i2c_write(unsigned char address, unsigned char byte)
{
  if (address != this->i2c_addr)
  {
    if (!i2c_set_address(address))
    {
      return false;
    }
    
    return true;
  }
  
  if (!this->i2c_connected)
  {
    i2c_error("Unable to write, cause of closed I2C handle.", 6);
    return false;
  }
  
  unsigned char buffer[60] = {0};
  int length = 1;
  
  buffer[0] = byte;
  
  if (write(this->i2c_handle, buffer, length) != length)
  {
    i2c_error("Error, while writing to I2C bus.", 7);
    return false;
  }
  
  return true;
}
