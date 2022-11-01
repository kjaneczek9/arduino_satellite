// defines pins numbers
//sensor 2a
const int trigPin2a =3;  //green
const int echoPin2a = 5; //green

//sensor 2b
const int trigPin2b = 2;  //red
const int echoPin2b = 4; //red

// laser
const int laserPin2 = 6;

const int step2 = 9; //base
const int dir2 = 8; //base

const int step_top = 13;
const int dir_top = 12;

const int echo_top = 11;
const int trig_top = 10;


// defines variables
long duration;
int distance_1a;
int distance_1b;
int distance_2a;
int distance_2b;
int x;
bool move;
bool outputDistance = false;


void setup() {
  pinMode(trigPin2a, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2a, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2b, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2b, INPUT); // Sets the echoPin as an Input

  // Stepper motors:
  pinMode(step2, OUTPUT);
  pinMode(dir2, OUTPUT);

  //lasers:
  pinMode(laserPin2, OUTPUT);

  digitalWrite(dir2, LOW);
  delayMicroseconds(100);
  resetEDPins(dir2, step2);

  Serial.begin(9600); // Starts the serial communication
  
}

void loop() {
 

  if(move){
    StepForwardDefault(dir2, step2);
    resetEDPins(dir2, step2);
  } 

  if(checkForDebris()) {
      stop_scan();
      laser2_on();
      start_scan_top();
    }
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

void start_scan_top(){
  StepForwardDefault(dir_top, step_top)
}


void updateDistances(){
  distance_2a = SonarSensor(trigPin2a, echoPin2a);
  distance_2b = SonarSensor(trigPin2b, echoPin2b);

  if(outputDistance){
    Serial.print("2A: ");
    Serial.println(distance_2a);
    Serial.print("2B: "); 
    Serial.println(distance_2b);
  }
}

void laser2_on(){
  digitalWrite(laserPin2, HIGH);
  delay(10);
  digitalWrite(laserPin2, LOW);
}

bool checkForDebris(){
  updateDistances();
  return (abs(distance_2a - distance_2b) <= 5 && distance_2b < 10);
}


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
  delayMicroseconds(10);
  //digitalWrite(dir, LOW);
}

//Default microstep mode function
void StepForwardDefault(int dir, int step){
  //digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  for(x= 0; x<16000; x++){  //Loop the forward stepping enough times for motion to be visible
    Serial.println("Moving forward...");
    digitalWrite(step,HIGH); //Trigger one step forward
    delayMicroseconds(100);
    digitalWrite(step,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(100);
    if(checkForDebris()){
      resetEDPins(dir2,step2);
      move = false;
      break;
    }
  }
}