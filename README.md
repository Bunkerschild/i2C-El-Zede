This tool is designed to be an interface for use with
I2C DOT matrix displays, like 20x4, typically used in
conjunction with arduino like boards. It makes it possible
to control compatible displays from the command line,
directly attached to the computers I2C bus.

The following switches are used for informational purposes:
===========================================================
<pre>
 --help                              Show this help context
 --license                           Show license context
 --info                              Show information about configured defaults
 --debug                             Enable debugging information to STDOUT

</pre>
Only one of the following switches is required as the first argument:
=====================================================================
<pre>
 --initialize                        Initialize LCD after connecting to I2C bus
 --no-init                           Do not initialize LCD after connection

</pre>
The following switches are used to set I2C device ID and LCD address:
=====================================================================
<pre>
 --device-id <ID>                    Set the I2C device ID - defaults to 1
 --i2c-address <VAL>                 I2C address as integer or hex - defaults to 0x27

</pre>
The following switches are used to set LCD dimensions:
=====================================================================
<pre>
 --lcd-rows <rows>                   Set rows of LCD - defaults to 4
 --lcd-cols <cols>                   Set cols of LCD - defaults to 20
 --lcd-dotsize-5x10                  Set dotsize of LCD to 5x10 - defaults to 5x8
 --lcd-8bit-mode                     Set LCD 8 bit mode - defaults to 4 bit

</pre>
The following switches are sequencable used to control the LCD behaviour:
=========================================================================
<pre>
 --wait <ms>                         Waittime in ms until next switch is used
 --clear                             Clear LCD screen
 --home                              Set cursor to row 1 and column 1
 --display                           Display content in RAM
 --no-display                        Hide display content
 --blink                             Let the cursor blink
 --no-blink                          Stop cursor blinking
 --cursor                            Display cursor
 --no-cursor                         Hide cursor
 --scroll-display-left               Scroll display content to the left
 --scroll-display-right              Scroll display content to the right
 --left-to-right                     Character direction left to right
 --right-to-left                     Character direction right to left
 --backlight                         Enable display backlight
 --no-backlight                      Disable display backlight
 --autoscroll                        Enable autoscrolling of content
 --no-autoscroll                     Disable autoscrolling of content
 --create-char <ID> <VAL,VAL,...>    Create a character in hex or integer values at ID
 --set-cursor <row> <column>         Place cursor for next character at row and column
 --write <VAL>                       Write character as integer or hex at current position
 --print-str <string>                Print a string to current cursor position

</pre>
Examples:
=========
<pre>
$ i2C-El-Zede/source/elzede-cli --initialize --print-str "I love Hello World examples"

On a 20x4 LCD it will result in:
+--------------------+
|I love Hello World e|
|xamples             |
|                    |
|                    |
+--------------------+

$ i2C-El-Zede/source/elzede-cli --no-init --set-cursor 4 3 --print-str "STUFF" --set-cursor 3 6 --print-str "COOL"

as a second command will result in:
+--------------------+
|I love Hello World e|
|xamples             |
|     COOL           |
|  STUFF             |
+--------------------+

</pre>
i2C-El-Zede - Copyright (C) 2018 Oliver Welter <info@bunkerschild.de>
