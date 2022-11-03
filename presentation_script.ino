// defines pins numbers
//sensor 2a
const int trig_base_1 =3;  //green
const int echo_base_1 = 5; //green

//sensor 2b
const int trig_base_2 = 2;  //red
const int echo_base_2 = 4; //red

// laser
const int laserPin = 6;
const int step_base = 9; //base
const int dir_base = 8; //base

const int step_top = 13;
const int dir_top = 12;

const int echo_top = 11;
const int trig_top = 10;


// defines variables
long duration;
int distance_top; //top
int distance_base_1; //base 1
int distance_base_2; //base 2
int x;
bool move_base;
bool move_top = true;
bool outputDistance = true;


void setup() {
  pinMode(trig_base_1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echo_base_1, INPUT); // Sets the echoPin as an Input
  pinMode(trig_base_2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echo_base_2, INPUT); // Sets the echoPin as an Input
  pinMode(trig_top, OUTPUT); // Sets the trigPin as an Output
  pinMode(echo_top, INPUT); // Sets the echoPin as an Input


  // Stepper motors:
  pinMode(step_base, OUTPUT);
  pinMode(dir_base, OUTPUT);
  pinMode(step_top, OUTPUT);
  pinMode(dir_top, OUTPUT);

  //lasers:
  pinMode(laserPin, OUTPUT);

  digitalWrite(dir_base, LOW);
  delayMicroseconds(100);
  digitalWrite(dir_top, LOW);
  delayMicroseconds(100);
  resetEDPins(dir_base, step_base);
  resetEDPins(dir_top, step_top);

  Serial.begin(9600); // Starts the serial communication
}

void loop() {

  if(move_base){
    move_base_motor();
    resetEDPins(dir_base, step_base);
  } 

  if(checkForDebris("BASE")) {
      stop_scan();
      laser_on();
      while(move_top){
        move_top_motor();
      }
    }

    
  if(!checkForDebris("BASE")){
    laser_off();
    start_scan();
  }

  if(!checkForDebris("TOP")){
    move_top = true;
  }

}


void stop_scan(){
  move_base = false;
}

void start_scan(){
  move_base = true;
}

void laser_on(){
  digitalWrite(laserPin, HIGH);
}

void laser_off(){
  digitalWrite(laserPin, LOW);
}

// void start_scan_top(){
//   move_top_motor();
// }


void updateDistances(){
    distance_top = SonarSensor(trig_top, echo_top);
    distance_base_1 = SonarSensor(trig_base_1, echo_base_1);
    distance_base_2 = SonarSensor(trig_base_2, echo_base_2);

  if(outputDistance){
    Serial.print("Base 1: ");
    Serial.println(distance_base_1);
    Serial.print("Base 2: "); 
    Serial.println(distance_base_2);
    Serial.print("Top: "); 
    Serial.println(distance_top);
    delay(200);

  }
}


bool checkForDebris(String location){
  updateDistances();

  if(location.equals("BASE")){
    return (abs(distance_base_1 - distance_base_2) <= 5 && distance_base_2 < 10);
  }
  else{
    return (distance_top <= 10);
  }
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
}


void move_base_motor(){
  for(x= 0; x<16000; x++){  //Loop the forward stepping enough times for motion to be visible
    Serial.println("Moving Base...");
    delay(300);
    digitalWrite(step_base,HIGH); //Trigger one step forward
    delayMicroseconds(100);
    digitalWrite(step_base,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(100);
    if(checkForDebris("BASE")){
      resetEDPins(dir_base,step_base);
      move_base = false;
      break;
    }
  }
}

void move_top_motor(){
  for(x= 0; x<16000; x++){  //Loop the forward stepping enough times for motion to be visible
    Serial.println("Moving Top...");
    delay(300);
    digitalWrite(step_top,HIGH); //Trigger one step forward
    delayMicroseconds(100);
    digitalWrite(step_top,LOW); //Pull step pin low so it can be triggered again
    delayMicroseconds(100);
    if(checkForDebris("TOP")){
      resetEDPins(dir_top,step_top);
      move_top = false;
      delay(1000);
      move_base = true;
      break;
    }
  }
}