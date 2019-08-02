#include <Wire.h>
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

//  Variables
const int PulseWire = 2;
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
int umd;
int tempBody;
int temp;                      // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
int myBPM;                      // Otherwise leave the default "550" value.
int pulse = 0;
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


void setup() {

  Serial.begin(9600);
  Wire.begin(7); // join i2c bus with address #7
  Wire.onRequest(requestEvent);
  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
 

  // Double-check the "pulseSensor" object was created and "began" seeing a signal.
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.
  }
}



void loop() {

  myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".


   Serial.println(temp);
  //Serial.println(umd);
  Serial.println(tempBody);
  

  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
    pulse = myBPM;
  }
  delay(20);
}

void requestEvent() {
  Serial.println("OK");
  inv(pulse);
  Wire.write("*");
  temp = readTempInCelsius(10, 0);
  umd = readUmd(1);
  tempBody = readTempInCelsius(10, 3);

  inv(temp);
  Wire.write("*");
  inv(umd);
  Wire.write("*");
  inv(tempBody);
  pulse = 0;
}

float readTempInCelsius(int count, int pin) {
  float temperaturaMediata = 0;
  float sumaTemperatura = 0;
  for (int i = 0; i < count; i++) {
    int reading = analogRead(pin);
    float voltage = reading * 5.0;
    voltage /= 1024.0;
    float temperatureCelsius = (voltage - 0.5) * 100 ;
    sumaTemperatura = sumaTemperatura + temperatureCelsius;
  }
  return sumaTemperatura / (float)count;
}

int readUmd(int pin) {
  int umd = analogRead(pin);
  while (umd > 1000) {
    umd--;
  }
  return umd / 10;
}

void inv(int n) {
  int inv = 0, i = 0;
  char c[3];
  while (n) {
    inv = n % 10 + (int)'0';
    n /= 10;
    c[i] = inv;
    i++;
  }
  while (i >= 0) {
    Wire.write(c[i]);
    i--;
  }
}
