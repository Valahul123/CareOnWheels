#include <Wire.h>
int MOTOR1_PIN1 = 6;
int MOTOR1_PIN2 = 9;
int MOTOR2_PIN1 = 3;
int MOTOR2_PIN2 = 5;
int currentSpeed = 0;
int currentAction = 0;
int iterations = 0;



void setup() {
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(7 , INPUT);
  pinMode(8 , INPUT);
  Wire.begin(8); // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop() {
  int objectAhead = digitalRead(8);
  int autoBalance = digitalRead(7);
  Serial.println(objectAhead);
  Serial.println("OK");
  Serial.println(autoBalance);
  if (iterations > 0 && (currentAction > 0 || currentAction <= 3)) {
    iterations--;
    int maxSpeed =  (currentSpeed * 100 + 25  );
    int analogSpeed  = maxSpeed < 255 ? maxSpeed : 255;
    if (objectAhead == 0 || autoBalance == 1) {
      analogWrite(MOTOR1_PIN1, 0);
      analogWrite(MOTOR1_PIN2, 0);
      analogWrite(MOTOR2_PIN1, 0);
      analogWrite(MOTOR2_PIN2, 0);

    }
    if (currentAction == 1 && objectAhead == 1 && autoBalance == 0) {

      analogWrite(MOTOR1_PIN1, analogSpeed);
      analogWrite(MOTOR1_PIN2, 0);
      analogWrite(MOTOR2_PIN1, analogSpeed);
      analogWrite(MOTOR2_PIN2, 0);
      
    }



    if (currentAction == 2) {
 
      analogWrite(MOTOR1_PIN1, 0);
      analogWrite(MOTOR1_PIN2, analogSpeed);
      analogWrite(MOTOR2_PIN1, 0);
      analogWrite(MOTOR2_PIN2, analogSpeed);

  
    }

    if (currentAction == 3 && objectAhead == 1 && autoBalance == 0) {
    
      analogWrite(MOTOR1_PIN1, 0);
      analogWrite(MOTOR1_PIN2, analogSpeed);
      analogWrite(MOTOR2_PIN1, analogSpeed);
      analogWrite(MOTOR2_PIN2, 0);

    
    }

    if (currentAction == 4 && objectAhead == 1 && autoBalance == 0) {
      analogWrite(MOTOR1_PIN1, analogSpeed);
      analogWrite(MOTOR1_PIN2, 0);
      analogWrite(MOTOR2_PIN1, 0);
      analogWrite(MOTOR2_PIN2, analogSpeed);


    }

  } else {
    analogWrite(MOTOR1_PIN1, 0);
    analogWrite(MOTOR1_PIN2, 0);
    analogWrite(MOTOR2_PIN1, 0);
    analogWrite(MOTOR2_PIN2, 0);
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {

  int x = Wire.read();    // receive byte as an integer
  //Serial.println(x);
  if (x != -1) {
   // Serial.println(x);
    currentSpeed = x % 10;
    currentAction =  (x / 10) % 10;
    iterations = 50;
  }

}
