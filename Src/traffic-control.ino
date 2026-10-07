#include
#include
#include 
const int trigPins[] = {3, 4, 6, 8}; 
const int echoPins[] = {2, 5, 7, 9};
int servoPins[] = {28, 26, 24, 22};
const int SS_PIN = 53;
const int RST_PIN = 10;
MFRC522 mfrc522(SS_PIN, RST_PIN); 
Servo servos[4];
unsigned long previousMillis = 0; 
const long interval = 5000;
const int distanceThreshold = 4;
boolean objectPresent = false; 
void setup() 
{ 
Serial.begin(9600); 
SPI.begin(); 
mfrc522.PCD_Init();
for (int i = 0; i < 4; i++) 
{
pinMode(trigPins[i], OUTPUT);
pinMode(echoPins[i], INPUT);18
servos[i].attach(servoPins[i]); servos[i].write(90); 
} 
} 
void loop() 
{ 
boolean objectDetected = false; 
if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) 
{ 
Serial.println("RFID Tag Detected");
openGate(0);
mfrc522.PICC_HaltA();
mfrc522.PCD_StopCrypto1();
}
for (int i = 0; i < 4; i++) 
{ 
long duration, distance; 
digitalWrite(trigPins[i], LOW); 
delayMicroseconds(2);
digitalWrite(trigPins[i], HIGH);
delayMicroseconds(10); 
digitalWrite(trigPins[i], LOW); 
duration = pulseIn(echoPins[i], HIGH);
distance = duration * 0.034 / 2; 
Serial.print("Sensor "); 
Serial.print(i + 1);
Serial.print(": ");
Serial.print(distance); 
Serial.println(" cm"); 
if (distance > 1 && distance < distanceThreshold) 
{ 
objectDetected = true; 
if (!objectPresent) 19
{ 
objectPresent = true; 
openGate(i); 
} 
} } 
if (!objectDetected && objectPresent) 
{ 
objectPresent = false; 
closeGates(); 
} 
if (!objectPresent) {
gateSequence();
} 
} 
void openGate(int gateIndex) 
{ 
for (int i = 0; i < 4; i++) 
{ 
if (i == gateIndex) 
{ servos[i].write(0);
} Else {
servos[i].write(90); }}}
void closeGates() {
for (int i = 0; i < 4; i++) {20
servos[i].write(90);
} 
} 
void gateSequence()
{ 
unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval)
{ 
previousMillis = currentMillis;
static int currentGate = 0;
closeGates();
servos[currentGate].write(0);
currentGate = (currentGate + 1) % 4; 
}
}
