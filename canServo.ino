#include <mcp_can.h>
#include <mcp_can_dfs.h>
#define maxAnglesServo 270
// CAN-Library from Seeed-Studio
//https://github.com/Seeed-Studio/CAN_BUS_Shield

// CAN-BUS Shield, receive data with interrupt mode
// CAN-bus and servomotor, PD-regulator
// Author: Hanif Nazari & Tobias Hansson, 2018-12-01

#include <SPI.h>
#include "mcp_can.h"

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

unsigned char flagRecv = 0;
unsigned char len = 0;
unsigned char buf[8];
int target = 0;
int val = 0;
int e = 0; // error = target(desired value) - val(acutal Value)
int oldE = 0;
float p = 1;
int reg = 0;// (p * e) + ((e - oldE) / (deltaT / 1000));
long currentMillis;
long previousMillis;
long deltaT = 0;
unsigned int messageType;
int upperLim = 255;
int lowerLim = 10;
unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};

String StringOne;
void setup()
{
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(A0, INPUT);
  
  Serial.begin(9600);

  while (CAN_OK != CAN.begin(CAN_125KBPS))              // init can bus : baudrate = 500k
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

  Serial.print(" previousMillis ");
  Serial.println(previousMillis);

  Serial.print(" deltaT ");
  Serial.println(deltaT);

  Serial.print(" currentMillis ");
  Serial.println(currentMillis);

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

      if (CAN.getCanId() == Myid) {

        messageType = buf[0];
        switch (messageType) {
          case 0:    // update target
            target = (int)buf[1];
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

      } else {
        Serial.println(" ID doesn't match");
      }

    }

  }

  val = map(analogRead(A0), 0, 1023, 0, 255);
  Serial.print(" val ");
  Serial.println(val);

  Serial.print(" target ");
  Serial.println(target);
  Serial.println();

  Serial.print(" error ");
  Serial.println(e);
  Serial.println();

  e = target - val;
  reg = (p * e) + ((e - oldE) / (deltaT / 1000));
  oldE = e;

  if (reg > 0 ) {
    digitalWrite(5, LOW);
    analogWrite(6, reg);
  } else {
    digitalWrite(6, LOW);
    analogWrite(5, abs(reg));
  }

}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
