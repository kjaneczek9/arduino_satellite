// defines pins numbers

// sensor 1a
const int trigPin1a = 11; //trig1 - white
const int echoPin1a = 10; //echo1 - white

//sensor 1b
const int trigPin1b = 12; //blue
const int echoPin1b = 13; //blue

//sensor 2a
const int trigPin2a = 4; //blue
const int echoPin2a = 5; //blue

//sensor 2b
const int trigPin2b = 6; //blue
const int echoPin2b = 7; //blue

// laser
const int laserPin = 2;


// defines variables
long duration;
int distance_1a;
int distance_1b;
int distance_2a;
int distance_2b;
int x;
bool move;

//Declare pin functions on Redboard
const int step1 = 35; //top 
const int dir1 = 41; //top

const int step2 = 37; //base
const int dir2 = 39; //base


void setup() {
  pinMode(trigPin1a, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1a, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin1b, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1b, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2a, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2a, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2b, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2b, INPUT); // Sets the echoPin as an Input

  // Stepper motors:
  pinMode(step1, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(step2, OUTPUT);
  pinMode(dir2, OUTPUT);

  //lasers:
  pinMode(laserPin, OUTPUT);

  resetEDPins(dir1, step1);//Set step, direction, microstep and enable pins to default states
  resetEDPins(dir2, step2);
  

  Serial.begin(9600); // Starts the serial communication
}

void loop() {

  distance_1a = SonarSensor(trigPin1a, echoPin1a);
  Serial.print("1A: ");
  Serial.println(distance_1a);

  distance_1b = SonarSensor(trigPin1b, echoPin1b);
  Serial.print("1B: ");
  Serial.println(distance_1b);
  
  distance_2a = SonarSensor(trigPin2a, echoPin2a);
  Serial.print("2A: ");
  Serial.println(distance_2a);

  distance_2b = SonarSensor(trigPin2b, echoPin2b);
  Serial.print("2B: ");
  Serial.println(distance_2b);
  
  
  if(move){
  StepForwardDefault(dir1, step1);
  resetEDPins(dir1, step1);

  StepForwardDefault(dir2, step2);
  resetEDPins(dir2, step2);
  }

  if(abs(distance_1b - distance_1a) <= 5 && distance_1b < 20){
    stop_scan();
    laser_on();
    
  }
  else if (abs(distance_2a - distance_2b) <= 5 && distance_1b < 20) {
    stop_scan();
    laser_on();
  }
  // // else if (wdistance <= 20 && bdistance != wdistance ){
  // //   start_backward_scan();
  // // }
  else{
    start_scan();
  }

  }

void stop_scan(){
  move = false;
}

void start_scan(){
  move = true;
}

void laser_on(){
  digitalWrite(laserPin, HIGH);
  delay(100);
  digitalWrite(laserPin, LOW);
}

// void start_backward_scan(){
//     move_dir = "HIGH";
// }


int SonarSensor(int trigPin,int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034/2;
}

//Reset Easy Driver pins to default states
void resetEDPins(int dir, int step) {
  digitalWrite(step, LOW);
  digitalWrite(dir, LOW);
}

//Default microstep mode function
void StepForwardDefault(int dir, int step){
  // Serial.println("Moving forward...");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  for(x= 0; x<100; x++){  //Loop the forward stepping enough times for motion to be visible
    digitalWrite(step,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(step,LOW); //Pull step pin low so it can be triggered again
  }
}
