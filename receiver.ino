//Code for Available PWM frequency for D3 & D11:
//TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz

//TCCR2B = TCCR2B & B11111000 | B00000010; // for PWM frequency of 3921.16 Hz

//TCCR2B = TCCR2B & B11111000 | B00000011; // for PWM frequency of 980.39 Hz

//TCCR2B = TCCR2B & B11111000 | B00000100; // for PWM frequency of 490.20 Hz (The DEFAULT)

//TCCR2B = TCCR2B & B11111000 | B00000101; // for PWM frequency of 245.10 Hz

//TCCR2B = TCCR2B & B11111000 | B00000110; // for PWM frequency of 122.55 Hz

//TCCR2B = TCCR2B & B11111000 | B00000111; // for PWM frequency of 30.64 Hz


//   Code for Available PWM frequency for D5 & D6:
//TCCR0B = TCCR0B & B11111000 | B00000001; // for PWM frequency of 62500.00 Hz

//TCCR0B = TCCR0B & B11111000 | B00000010; // for PWM frequency of 7812.50 Hz

//TCCR0B = TCCR0B & B11111000 | B00000011; // for PWM frequency of 976.56 Hz (The DEFAULT)

//TCCR0B = TCCR0B & B11111000 | B00000100; // for PWM frequency of 244.14 Hz

//TCCR0B = TCCR0B & B11111000 | B00000101; // for PWM frequency of 61.04 Hz


//  Code for Available PWM frequency for D9 & D10:
//TCCR1B = TCCR1B & B11111000 | B00000001; // set timer 1 divisor to 1 for PWM frequency of 31372.55 Hz

//TCCR1B = TCCR1B & B11111000 | B00000010; // for PWM frequency of 3921.16 Hz

//TCCR1B = TCCR1B & B11111000 | B00000011; // for PWM frequency of 490.20 Hz (The DEFAULT)

//TCCR1B = TCCR1B & B11111000 | B00000100; // for PWM frequency of 122.55 Hz

//TCCR1B = TCCR1B & B11111000 | B00000101; // for PWM frequency of 30.64 Hz
// Library: TMRh20/RF24, https://github.com/tmrh20/RF24/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";

char receivedData[32] = "";
int  xAxis, yAxis, potValue;

#define enA 10   // Note: Pin 9 in previous video ( pin 10 is used for the SPI communication of the NRF24L01)
#define in1 4
#define in2 5
#define enB 3   // Note:  Pin 10 in previous video
#define in3 6
#define in4 7



int motorSpeedA = 0;
int motorSpeedB = 0;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}


void loop() {
  if (radio.available()) {   // If the NRF240L01 module received data
    radio.read(&receivedData, sizeof(receivedData)); // Read the data and put it into character array
    xAxis = atoi(&receivedData[0]); // Convert the data from the character array (received X value) into integer
    delay(10);
    radio.read(&receivedData, sizeof(receivedData));
    yAxis = atoi(&receivedData[0]);

    radio.read(&receivedData, sizeof(receivedData));
    potValue = atoi(&receivedData[0]);
    delay(10);
    Serial.print("X : ");
    Serial.println(xAxis);
    Serial.print("Y : ");
    Serial.println(yAxis);
    Serial.print("PotValue : ");
    Serial.println(potValue);
  }


  // X-axis used for forward and backward control
  if (xAxis < 100) {
    // Set Motor A backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(potValue, 0, 1023, 50, 255);
    motorSpeedB = map(potValue, 0, 1023, 50, 255);
  }
  else if (xAxis > 800) {
    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(potValue, 0, 1023, 50, 255);
    motorSpeedB = map(potValue, 0, 1023, 50, 255);
  }
  // If joystick stays in middle the motors are not moving
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }


  // Y-axis used for left and right control
  //this is left
  if (yAxis < 100) {

    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(potValue, 0, 1023, 50, 255);
    motorSpeedB = map(potValue, 0, 1023, 50, 255);
  }


  if (yAxis > 800) {

    // Set Motor A forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(potValue, 0, 1023, 50, 255);
    motorSpeedB = map(potValue, 0, 1023, 50, 255);
  }
  // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
  if (motorSpeedA < 50) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 50) {
    motorSpeedB = 0;
  }
  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B
}
