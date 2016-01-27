#include <SPI.h>

#include <mcp_can.h>
#include <mcp_can_dfs.h>



INT32U canId = 0x000;
unsigned char len = 0;
unsigned char buf[8];
char str[20];

const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN); 

void printHex(int num, int precision) {
     char tmp[16];
     char format[128];

     sprintf(format, "0x%%.%dX", precision);

     sprintf(tmp, format, num);
     Serial.print(tmp);
}

void setup()
{    
Serial.begin(115200);
START_INIT:
if(CAN_OK == CAN.begin(CAN_125KBPS, MCP_8MHz))
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }
}
void loop()
{
    if(CAN_MSGAVAIL == CAN.checkReceive())
    {
        CAN.readMsgBuf(&len, buf);
        canId = CAN.getCanId();
        Serial.print(millis());Serial.print(": ");
        printHex(canId, 3);Serial.print(" ");
        for(int i = 0; i<len; i++)
  {
    printHex(buf[i], 2);Serial.print(" ");
  }
  Serial.println();
   }
}
