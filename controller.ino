#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(8,9); // CE, CSN

const uint64_t pipeOut = 0xB3B4B5B6A3LL;//0xE8E8F0F0E1LL; //IMPORTANT: The same as in the receiver

struct MyData {
  int   Xvalue;
  int   Yvalue;
  int   Potvalue;
};

MyData data;


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
}
void loop() {

  data.Potvalue = analogRead(A2);
  data.Yvalue = analogRead(A1);
  data.Xvalue = analogRead(A0);
Serial.println(data.Potvalue);
  

radio.write(&data, sizeof(MyData));
}
