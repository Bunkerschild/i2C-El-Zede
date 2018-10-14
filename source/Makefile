elzede-cli:
	gcc i2c_device.cpp LiquidCrystal.cpp cli.cpp -o elzede-cli -lm -lstdc++

/usr/local/bin/elzede-cli: elzede-cli
	cp -f elzede-cli /usr/local/bin/elzede-cli

install: /usr/local/bin/elzede-cli

clean:
	rm -f elzede-cli >/dev/null 2>&1

all: elzede-cli install
	