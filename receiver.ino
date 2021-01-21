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

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define enB 5
#define enA 6

#define in1 7
#define in2 10
#define in3 4
#define in4 2

RF24 radio(8, 9); // CE, CSN
const uint64_t pipeIn =  0xB3B4B5B6A3LL;//0xE8E8F0F0E1LL64 bit encrypicted code

struct MyData {
  int   Xvalue;
  int   Yvalue;
  int   Potvalue;
};

MyData data;

int motorSpeedA = 0;
int motorSpeedB = 0;


void setup() {

  Serial.begin(9600);
  radio.begin();
  radio.setDataRate(RF24_250KBPS); // Both endpoints must have this set the same
  radio.setAutoAck(false);
  radio.openReadingPipe(1, pipeIn);
  radio.startListening();

//Set PWM frequency for pin 6 and 5 on ENA and ENB
TCCR0B = TCCR0B & B11111000 | B00000001; // for PWM frequency of 62500.00 Hz

  // Define Pins
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);


  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void loop() {

  if (radio.available()) {   // If the NRF240L01 module received data

    Serial.print("X:");
    Serial.print(data.Xvalue);
    Serial.print("      Y");
    Serial.println(data.Yvalue);
radio.read(&data, sizeof(MyData));
  }

  if (data.Xvalue < 500) {
    //Backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

    motorSpeedA = map(data.Potvalue, 0, 1023, 0, 255);
    motorSpeedB = map(data.Potvalue, 0, 1023, 0, 255);
  }
  //Forward
  else if (data.Xvalue > 515) {

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    motorSpeedA = map(data.Potvalue, 0, 1023, 0, 255);
    motorSpeedB = map(data.Potvalue, 0, 1023, 0, 255);
  }

  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }

  if (data.Yvalue < 450) {
    //THIS IS LEFT

    // Set Motor A backward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    // Set Motor B forward

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);


    motorSpeedA = map(data.Potvalue, 0, 1023, 0, 255);
    motorSpeedB = map(data.Potvalue, 0, 1023, 0, 255);

    //  Confine the range from 0 to 255
    if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
    if (motorSpeedB > 255) {
      motorSpeedB = 255;
    }
  }

  if (data.Yvalue > 550) {
    //THIS IS RIGHT

    // Set Motor A forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    // Set Motor B backward

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

    motorSpeedA = map(data.Potvalue, 0, 1023, 0, 255);
    motorSpeedB = map(data.Potvalue, 0, 1023, 0, 255);


    if (motorSpeedA > 255) {
      motorSpeedA = 255;
    }
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
  }

  //prevent noise
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }


  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B

 
 
}
