//TTN:
//   Based mainly in Hello LoRa - ABP TTN Packet Sender (Multi-Channel) Adafruit module Feather 32u4
//   Copyright 2015, 2016 Ideetron B.V.
//   Modified by Brent Rubell for Adafruit Industries, 2018
//GPS
// Include library TinyGPS on Arduino repository.
//MIX:
//   David Medina EA3IES 
//   The Air and Space Box Program 2020
//   find us on https://airbox.space
//   GitHub repository: theairandspace
//   Any suggestion is welcomed! 
/************************** Configuration ***********************************/

#include <TinyLoRa.h> // adafruit library, download from: https://github.com/adafruit/TinyLoRa
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h> // download from Arduino Repository

SoftwareSerial Serialgps(6, 5); // RX6, TX5 

#define EU863 
#define irq 2
#define css 10
#define reset 9

// Visit your thethingsnetwork.org device console
// to create an account, or if you need your session keys.

// Network Session Key (MSB)
uint8_t NwkSkey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Application Session Key (MSB)
uint8_t AppSkey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Device Address (MSB)
uint8_t DevAddr[4] = { 0x00, 0x00, 0x00, 0x00 };

/************************** Example Begins Here ***********************************/

// Data Packet to Send to TTN two versions, version 14 bytes and version 12 bytes
//static uint8_t datos[14];
static uint8_t datos[12];

// define interval to send data in seconds, don't forget 1% rule!
const unsigned int sendInterval = 150; // 150 seconds

// define class and Pinout for SX127x
TinyLoRa TTN = TinyLoRa(irq, css, reset); //irq pin, chip select, reset

bool haydatosgps = false;

TinyGPS GPS;//define class gps
  
unsigned long startmillis = millis();

void setup()
{
 Serialgps.begin(9600); //Neo uses 9600 de baudrate UART

  delay(2000);// pausa de cortesía
  Serial.begin(115200);// defined for debug
  while (! Serial);
  
  // Initialize pin LED_BUILTIN as an output
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialize LoRa, find more info on README.md
  Serial.print("Starting TTN...");
  
  // uncomment to define multi-channel sending 
  //TTN.setChannel(MULTI);
  // uncoment to define one specific channel
  TTN.setChannel(CH0);
  
  // set datarate check specific spreading factor and bandwidth
  TTN.setDatarate(SF12BW125);
  if(!TTN.begin())
  {
    Serial.println("Failed");
    Serial.println("Check wires");
    while(true);
  }

  // Optional set transmit power. If not set default is +17 dBm.
  // Valid options are: -80, 1 to 17, 20 (dBm).
  
  // TTN.setPower(17);

  Serial.println("Setup: All OK");
}

void loop()
{
  leer_gps();
  if (millis() - startmillis > (sendInterval *1000)) // if its time to send
  {

   if (haydatosgps){
      preparar_trama_datos_gps();// prepare data to send

      TTN.sendData(datos, sizeof(datos), TTN.frameCounter);
      Serial.println("data sent!!");
      memset(datos,0,sizeof(datos));// clean datos array
   }
   else Serial.println("Nothing to send...");
  
   startmillis = millis();
  } 
}

//////////////////////////////////////////////////////
/////////////////       RUTINAS       ////////////////
//////////////////////////////////////////////////////

//rutinas del gps 

void leer_gps(void){

 if (Serialgps.available()){
   while (Serialgps.available())
   { 
     GPS.encode(Serialgps.read());
     //Serial.println(gps.read());// uncomment to check you are reading data from GPS
   }
   haydatosgps =true; // some data to send  
 }
}

void preparar_trama_datos_gps(void) 
{
  Serial.print("cooking data!");
  float flat, flon;
  unsigned long latitud,longitud, chars = 0;
  unsigned int entero;
  unsigned short sentences = 0, failed = 0;

  GPS.f_get_position(&flat, &flon);

  latitud = flat * 10000000; // multiply by 10 million to convert float to integer
  datos[0] = (byte) ((latitud & 0xFF000000) >> 24 );
  datos[1] = (byte) ((latitud & 0x00FF0000) >> 16 );
  datos[2] = (byte) ((latitud & 0x0000FF00) >> 8 );
  datos[3] = (byte) ((latitud & 0X000000FF));

  longitud = flon * 10000000; // the same as before
  datos[4] = (byte) ((longitud & 0xFF000000) >> 24 );
  datos[5] = (byte) ((longitud & 0x00FF0000) >> 16 );
  datos[6] = (byte) ((longitud & 0x0000FF00) >> 8 );
  datos[7] = (byte) ((longitud & 0X000000FF));

  entero = (unsigned int)GPS.f_altitude();//convert altitud float to integer and divide in two bytes
  datos[8] = (byte) ((entero & 0xFF00) >> 8 );
  datos[9] = (byte) ((entero & 0X00FF));

//uncomment only one of the two versions

//version 14 bytes
//  entero = (unsigned int)GPS.f_speed_knots();// read as integer and then use two bytes to send speed
//  datos[10] = (byte) ((entero & 0xFF00) >> 8 );
//  datos[11] = (byte) ((entero & 0X00FF));

//version 12 bytes
  entero = (unsigned int)GPS.f_speed_knots();//convert speed float to integer
  datos[10] = (byte) ((entero & 0X00FF)); //because its difficult to achieve 255 Nautical miles we can use just one byte

//version 14 bytes
//  entero = (unsigned int)GPS.f_course();//convert course float to integer values between 0-359 
//  datos[12] = (byte) ((entero & 0xFF00) >> 8 );
//  datos[13] = (byte) ((entero & 0X00FF));

//version 12 bytes
  entero = (unsigned int)GPS.f_course();//convertimos rumbo float en entero
  entero = entero/2; //divide course by 2, yes, we loose one degree of precission, we multiply again on destination.
  datos[11] = (byte) ((entero & 0X00FF));
  
  haydatosgps = false;
}
