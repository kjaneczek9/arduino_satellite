// defines pins numbers
const int trigPin1 = 10; //trig1 - white
const int echoPin1 = 9; //echo1 - white
const int trigPin2 = 13; //blue
const int echoPin2 = 12; //blue
// defines variables
long duration;
int distance;

void setup() {
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input

  Serial.begin(9600); // Starts the serial communication
}

void loop() {

  SonarSensor(trigPin1, echoPin1);
  Serial.print("White Sensor: ");
  Serial.println(distance);
  delay(100);
  
  SonarSensor(trigPin2, echoPin2);
  Serial.print("Blue Sensor: ");
  Serial.println(distance);
  delay(100);
  }


void SonarSensor(int trigPin,int echoPin) {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = duration * 0.034/2;
}