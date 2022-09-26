// defines pins numbers
const int trigPin1 = 10; //trig1 - white
const int echoPin1 = 9; //echo1 - white
const int trigPin2 = 13; //blue
const int echoPin2 = 12; //blue
// defines variables
long duration;
int distance;
int x;

//Declare pin functions on Redboard
#define step1 3
#define dir1 2

#define step2 6
#define dir2 7

void setup() {
  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input

  pinMode(step1, OUTPUT);
  pinMode(dir1, OUTPUT);

  pinMode(step2, OUTPUT);
  pinMode(dir2, OUTPUT);

  resetEDPins(dir1, step1);//Set step, direction, microstep and enable pins to default states
  resetEDPins(dir2, step2);

  Serial.begin(9600); // Starts the serial communication
}

void loop() {

  SonarSensor(trigPin1, echoPin1);
  Serial.print("White Sensor: ");
  Serial.println(distance);
  
  SonarSensor(trigPin2, echoPin2);
  Serial.print("Blue Sensor: ");
  Serial.println(distance);
  
  StepForwardDefault(dir1, step1);
  resetEDPins(dir1, step1);

  StepForwardDefault(dir2, step2);
  resetEDPins(dir2, step2);

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

//Reset Easy Driver pins to default states
void resetEDPins(int dir, int step) {
  digitalWrite(step, LOW);
  digitalWrite(dir, LOW);
}

//Default microstep mode function
void StepForwardDefault(int dir, int step){
  // Serial.println("Moving forward at default step mode.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  for(x= 0; x<50; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(step,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(step,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
}