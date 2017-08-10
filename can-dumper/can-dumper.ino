#include <Arduino.h>
#include <SPI.h>
#include <avr/pgmspace.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "mcp_can.h"
#include "mcp_can_dfs.h"

const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);
unsigned char flagRecv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];

void attachCAN()
{
  #if defined(__AVR_ATmega32U4__) // Arduino Pro Micro
    pinMode(7, INPUT);
    attachInterrupt(digitalPinToInterrupt(7), MCP2515_ISR, FALLING); // start interrupt
  #else // Other Arduinos (Nano in my case)
    pinMode(2, INPUT);
    attachInterrupt(digitalPinToInterrupt(2), MCP2515_ISR, FALLING); // start interrupt
  #endif
}

void setup()
{
  delay(3000);
    Serial.begin(115200);
START_INIT:
  if(CAN_OK == CAN.begin(MCP_STDEXT, CAN_125KBPS, MCP_8MHZ)) {
    Serial.println(F("CAN ok!"));
  } else {
    Serial.println(F("CAN fail"));
    delay(100);
    goto START_INIT;
  }

  attachCAN();

  CAN.setMode(MCP_NORMAL);

}

void MCP2515_ISR()
{
    flagRecv = 1;
}

void loop()
{
    if(flagRecv)
    {
        flagRecv = 0;
        while (CAN_MSGAVAIL == CAN.checkReceive())
        {
            // read data,  len: data length, buf: data buf
            CAN.readMsgBuf(&len, buf);
            Serial.print(CAN.getCanId(),HEX);
            Serial.print("\t");
            // print the data
            for(int i = 0; i<len; i++)
            {
                Serial.print(buf[i],HEX);Serial.print("\t");
            }
            Serial.println();
        }
    }
}
