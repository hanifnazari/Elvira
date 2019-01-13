#include <mcp_can.h>
#include <mcp_can_dfs.h>
#define maxAnglesServo 270
// CAN-Library from Seeed-Studio (link below)
//https://github.com/Seeed-Studio/CAN_BUS_Shield

// CAN-BUS Shield, receive data with interrupt mode
// CAN-bus and servomotor, PD-regulator
// Author: Hanif Nazari & Tobias Hansson, 2018-12-01

#include <SPI.h>
#include "mcp_can.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);    // Set CS pin

unsigned char flagRecv = 0;
unsigned char len = 0; //message length
unsigned char buf[8];
int target = 0; // 
int val = 0; // current value
int e = 0; // error = target(desired value) - val(acutal Value)
int oldE = 0;
int p = 8;
int reg = 0;// (p * e) + ((e - oldE) / (deltaT / 1000));
long currentMillis;
long previousMillis;
long deltaT = 0;
unsigned int messageType; // messagetype indicated with 0 in position buf[0] from the user interface
int upperLim = 255;
int lowerLim = 10;
unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // using this when sending back the angles with stmp array. 


void setup()
{
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(A0, INPUT);
  
  Serial.begin(9600);

  while (CAN_OK != CAN.begin(CAN_125KBPS))              // init can bus : baudrate = 125k
  {
    Serial.println("CAN BUS Shield init fail");
    Serial.println(" Init CAN BUS Shield again");
    delay(100);
  }
  Serial.println("CAN BUS Shield init ok!");

  attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt
  previousMillis = millis();
}

void MCP2515_ISR()
{
  flagRecv = 1;
}

void loop()  {

  previousMillis = currentMillis;
  currentMillis = millis();
  deltaT = (currentMillis - previousMillis);

  if (flagRecv) {
    // check if get data

    unsigned int Myid = 1;
    // iterate over all pending messages
    // If either the bus is saturated or the MCU is busy,
    // both RX buffers may be in use and reading a single
    // message does not clear the IRQ conditon.
    if (CAN_MSGAVAIL == CAN.checkReceive()) {
      flagRecv = 0;// clear flag
      // read data,  len: data length, buf: data buf
      CAN.readMsgBuf(&len, buf);
      // checks if the messages ID matches the Processor ID wich is Myid = 1;
      if (CAN.getCanId() == Myid) {

        messageType = buf[0];
        switch (messageType) {
          case 0:    // update target
            target = (int)buf[1];
            //testflag = 1;
            break;
          case 1:    //send back angle
            stmp[0] = Myid;
            stmp[1] = val;
            CAN.sendMsgBuf(0x00, 0, 2, stmp);
            Serial.print(val, HEX);
            delay(100);   // send data per 100ms
            break;
          case 2:     //set upper Limit
            if (upperLim <= 255) {
              upperLim = (int)buf[1];
              Serial.print("upperLim");
              Serial.println(upperLim);
            }
            break;
          case 3: // set lower limit
            if (lowerLim >= 10) {
              lowerLim = (int)buf[1];
              Serial.print("lowerLim");
              Serial.println(lowerLim);
            }
            break;
            
          case 4: // update p value
          
            p = (int)buf[1];
            Serial.println(p);
            break;
        }  

      }

    }

  }
    // val is the value from the potentiometer, connected to analog pin A0 as input. 
  val = map(analogRead(A0), 0, 1023, 0, 255);
  
if(target!=0){
  e = target - val;
  reg = p*e + (e - oldE)/deltaT/1000;
  if(reg > 70){ reg = 70;}
  else if(reg < -70){ reg = -70;}
  oldE = e;

  if (reg > 0 ) {
    digitalWrite(5, LOW);
    analogWrite(6, reg);
  } else {
    digitalWrite(6, LOW);
    analogWrite(5, - reg);
  }
}

}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
