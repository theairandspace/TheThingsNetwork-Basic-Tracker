# TheThingsNetwork
TheThingsNetwork Basic ABP Tracker
Semtech SX1276/77/78/79 wiring
Semtech SX1276/77/78/79	Arduino
VCC	3.3V
GND	GND
SCK	SCK
MISO	MISO
MOSI	MOSI
NSS	10
NRESET	9
DIO0	2
NSS, NRESET, and DIO0 pins can be changed by using LoRa.setPins(ss, reset, dio0). DIO0 pin is optional, it is only needed for receive callback mode. If DIO0 pin is used, it must be interrupt capable via attachInterrupt(...).

NOTES:

Some boards (like the Arduino Nano), cannot supply enough current for the SX127x in TX mode. This will cause lockups when sending, be sure to use an external 3.3V supply that can provide at least 120mA's when using these boards.
If your Arduino board operates at 5V, like the Arduino Uno, Leonardo or Mega, you will need to use a level converter for the wiring to the Semtech SX127x module. Most Semtech SX127x breakout boards do not have logic level converters built-in.
