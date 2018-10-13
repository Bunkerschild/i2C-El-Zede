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

void copyright()
{
    printf("i2C-El-Zede - Copyright (C) 2018 Oliver Welter <info@bunkerschild.de>\n");
}

void help()
{

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

int main (int argc, char **argv)
{
    if (argc < 2)
    {
        gpl();

        return usage(argv[0]);
    }
        
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "--help"))
        {
           copyright();
           help();
           
           return 2;
        }
        else if (!strcmp(argv[i], "--license"))
        {
           gpl();
           
           return 0;
        }
        else if (!strcmp(argv[i], "--init"))
        {
           
        }
        else
        {
           printf("ERROR: Invalid argument '%s'\n\n", argv[i]);
           copyright();

           return usage(argv[0]);
        }
    }
}
