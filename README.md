# TTN Tracker 

LMIC libraries are a good choice but requires too much space and could be a problem using 32kb controllers, this code save space but can be used only in ABP and non confirming packets.

## Preliminary:

### Semtech SX1276/77/78/79 wiring

| Semtech SX1276/77/78/79 | Arduino |
| :---------------------: | :------:|
| VCC | 3.3V |
| GND | GND |
| SCK | SCK |
| MISO | MISO |
| MOSI | MOSI |
| NSS | 10 |
| NRESET | 9 |
| DIO0 | 2 |


`NSS`, `RESET`, and `IRQ` pins can be changed when defining class `TinyLoRa(irq,css,reset)`. The `irq` pin is optional, it is only needed for receive callback mode.

**NOTES**:
 * Some boards (like the Arduino Nano), cannot supply enough current for the SX127x and GPS together. This will cause lockups when sending, be sure to use an external 3.3V supply when using these boards.

## Installation TinyLoRa

### Using GitHub Repository

1. Download the Adafruit library: https://github.com/adafruit/TinyLoRa
2. Click to "Clone or Download" button.
3. Choose `Sketch` -> `Include Library` -> `include library .ZIP`
4. Click the library on the folder you download it.

## Installation GPS

### Using the Arduino IDE Library Manager

1. Choose `Sketch` -> `Include Library` -> `Manage Libraries...`
2. Type `tinyGPS` into the search box.
3. Click the row to select the library.
4. Click the `Install` button to install the library.


## FAQ

**1) Initilizating the LoRa radio is failing**

Please check the wiring you are using matches what's listed in [Semtech SX1276/77/78/79 wiring](#semtech-sx1276777879-wiring). You can also use `tinyLoRa(irq,css,reset)` to change the default pins used.


## LoRaWAN

  The main code is part of Adafruit's TinyLoRa library for the Arduino platform. 
  
  It is designed specifically to work with the Adafruit Feather 32u4 RFM95 LoRa https://www.adafruit.com/product/3078 but it work with any other arduino 32x. Even that there is no warranty.


## Default Region

The default region of this library is US902 Used in USA, Canada and South America. To change for European frecuencies use:

`#define EU863` 

before the setup()

see more information at: https://airbox.space/ESP-Com-Lorawanbasico.html
## Keys
visit thethingsnetwork.org device console account to find your session keys. 

**IMPORTANT**: 
* Write key in MSB direction.
### NwkSkey[16]; 
Network Session Key 
### AppSkey[16];
Application Session Key
### DevAddr[4];
Device Address

## setChannel(Code) 
define LoRaWAN Channels (only EUR863 are described below)

| EUR Channels |Code| Frequency |
| :----------: | :-----:|:------:|
|Channel 0 |CH0 |868.100 MHz|
|Channel 1 |CH1|868.300 MHz|
|Channel 2 |CH2|868.500 MHz|
|Channel 3 |CH3|867.100 MHz|
|Channel 4 |CH4|867.300 MHz|
|Channel 5 |CH5|867.500 MHz|
|Channel 6 |CH6|867.700 MHz|
|Channel 7 |CH7|867.900 MHz|
|Random|MULTI |All|

## setDatarate(Code) 

Spreading factors: 7 to 12

Bandwidth: 125 to 250 Khz

The possible combinations are:

* SF7BW125
* SF7BW250
* SF8BW125
* SF9BW125
* SF10BW125
* SF11BW125
* SF12BW125

## Payload Format

to receive properly the data into thethingsnetwork console define the payload format as below.


`function Decoder(bytes, port) {`
`  // Decode an uplink message from a buffer`
`  // (array) of bytes to an object of fields.`
`var decoded = {};`

`lat_decode = ((bytes[0]) << 24)`
`+ ((bytes[1]) << 16)`
`+ ((bytes[2]) << 8)`
`+ ((bytes[3]));`

`lon_decode = ((bytes[4]) << 24)`
`+ ((bytes[5]) << 16)`
`+ ((bytes[6]) << 8)`
`+ ((bytes[7]));`

`decoded.latitude = lat_decode / 10000000;`
`decoded.longitude = lon_decode / 10000000;`

`decoded.altitude =((((bytes[8]) << 8) + ((bytes[9])))*3.28084);//convertimos metros a pies`
`decoded.groundspeed = bytes[10];`
`decoded.truecourse = bytes[11]*2;// para ocupar solo un byte se envía el rumbo/2`

`return decoded;`
`}`

more info at: https://airbox.space/

The Air and Space Box Program 2020.
