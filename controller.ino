// Library: TMRh20/RF24, https://github.com/tmrh20/RF24/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";

char xyData[32] = "";
String xAxis, yAxis, potValue;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
void loop() {
  potValue = analogRead(A2); // Read Joysticks X-axis
  xAxis = analogRead(A0); // Read Joysticks X-axis
  yAxis = analogRead(A1); // Read Joysticks Y-axis
  // X value
  xAxis.toCharArray(sendingData, 5); // Put the String (X Value) into a character array
  radio.write(&sendingData, sizeof(sendingData)); // Send the array data (X value) to the other NRF24L01 modile
  // Y value
  yAxis.toCharArray(xyData, 5);
  radio.write(&xyData, sizeof(xyData));
    // Pot value
  potValue.toCharArray(sendingData, 5);
  radio.write(&sendingData, sizeof(sendingData));
  delay(20);

  Serial.print("Y: ");
  Serial.println(yAxis);
    Serial.print("X: ");
  Serial.println(xAxis);
    Serial.print("PotValue: ");
  Serial.println(potValue);
}
