
/*  Purpose: Implementation of smart lighting systems
 *  Read Complete ReadMe.md at my GitHub Profile
 *  
 *  Created by Manish Joshi, https://github.com/manishjoshi394
 * 
 * Ultrasonic Sensor HC-SR04
 *
 *   Ultrasonic sensor Pins:
 *      VCC: +5VDC
 *      Trig : Trigger (INPUT) - Pin10 and Pin 12
 *      Echo: Echo (OUTPUT) - Pin 11 and 13
 *      GND: GND
 */
 
int trigPin1 = 10;    
int echoPin1 = 11;    

int trigPin2 = 12;
int echoPin2 = 13;

long duration, cm,  inches;

unsigned long int prevMili = 0;
unsigned long int TIMEOUT = 5 * 1000;

void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

void loop()
{
  
  if (thresholdExceed(0, 200, 3, trigPin1, echoPin1))// || thresholdExceed(3, 200, 3, trigPin2, echoPin2))   // best sensitive setting 200 timegap 3 trials---min stable dis. of wall ~150.... moderately sensitive 500 gap 3 trials
  {
    turnOnLED(2, 3, 2000);
    delay(500);
    turnOnLED(4, 5, 2000);
    delay(500);
    turnOnLED(6, 7, 2000);
    delay(500);
    turnOnLED(8, 9, 2000);
    
    Serial.println("**************************Threshold exceeded********************LED ON********");
  }
  if ((millis() - prevMili) > TIMEOUT)
    {
      delay(100);
      diffuseLED(2, 3); // if time is out
      delay(700);
      diffuseLED(4, 5); // if time is out
      delay(700);
      diffuseLED(6, 7); // if time is out
      delay(700);
      diffuseLED(8, 9); // if time is out shut down Lights
      delay(700);
      
    }
  
}


/**************************************************
 * Utility Functions
 * @author: Manish Joshi
 * 
 **************************************************/

bool thresholdExceed(long threshold, int timeGap, int trials, int trigPin, int echoPin)
{
  long change, minPrevDis = 30000, minNewDis = 30000;
  int trials_1 = trials;
  while (trials--)
  {
    delay(10); // I used 10 ms here initially but that was a significant time wastage
    long dis = getDistance(trigPin, echoPin);
    if (minPrevDis > dis)
      minPrevDis = dis;
  }
  Serial.print("Minimum Previous   :    ");
  Serial.println(minPrevDis);
  
  delay(timeGap);
  
  while (trials_1--)
  {
    delay(10);
    long dis = getDistance(trigPin, echoPin);
    if (dis < minNewDis)
      minNewDis = dis;
  }
  change = abs(minPrevDis - minNewDis);
  Serial.print("Min NEW  &  change   :     ");
    Serial.print(minNewDis);
    Serial.print(" ");
    Serial.println(change);
  if (change > threshold)
    return true;
  
  return false;
}

void turnOnLED(int pinP, int pinN, int timeout)
{
  pinMode(pinP, OUTPUT);
  pinMode(pinN, OUTPUT);
  
  if (digitalRead(pinP) == HIGH)
    return;
  
  digitalWrite(pinN, LOW);
  digitalWrite(pinP, HIGH);
  
  prevMili = millis();
  TIMEOUT = timeout;
  
}

void diffuseLED(int pinP, int pinN)
{
  digitalWrite(pinP, LOW);
  digitalWrite(pinN, LOW);
}
long getDistance(int trigPin, int echoPin)
{
 // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // convert the time into a distance
  cm = (duration/2) / (29.1); // SONAR Formula @author: Manish Joshi

  return cm;
}

