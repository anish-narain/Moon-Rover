#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#include <WiFiWebServer.h>
#include <WiFiClient.h>
//#include <WiFi101.h>

// Radio
int radioPin = 7; // Output of radio circuit

// Accoustic
int echoPin = 5; // Echo pin of HC-SR04
int trigPin = 9; // Trig pin of HC-SR04

// Magnetic
int magneticPin = A1; // Output of hall sensor circuit

// IR
int IRPin = 6;

// Network
const char ssid[] = "EEERover";
const char pass[] = "exhibition";
const int groupNumber = 27; // Set your group number to make the IP address constant - only do this on the EEERover network

String command;
String rockType;
bool sentType = false;

WiFiWebServer server(80);

// Motion
int SpeedRight = 250;
int SpeedLeft = 250;

int enLeft = 8; //previously 3
int inLeft = 10; //previously 4

int enRight = 11; //previously 9
int inRight = 12;//previously 8

// **********************************************************************************************
void HTTP_handleRoot(void) {

  String s;
  if(server.hasArg("State")){
    s = server.arg("State");
    Serial.println(s);
  }

  if (s != "SC"){
    server.send ( 200, "text/html", "");
  }
  delay(1);
  
}
void setup() {
  
  Serial.begin(9600);
  Serial.println("starting");
  delay(1000);
  // Set pin modes
  pinMode(IRPin, INPUT);
  pinMode(magneticPin,INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(radioPin, INPUT);

  pinMode(enLeft, OUTPUT);
  pinMode(enRight, OUTPUT);
  pinMode(inLeft, OUTPUT);
  pinMode(inRight, OUTPUT);

  // Configure server
  Serial.println("Starting Web Server");

  if (groupNumber){
    WiFi.config(IPAddress(192,168,0,groupNumber+1));
  }

  Serial.print(F("Connecting to WPA SSID: "));
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED)
  {
    delay(500);
    Serial.print('.');
  } 

  server.on ( "/", HTTP_handleRoot );
  server.onNotFound ( HTTP_handleRoot );
  
  server.begin();
  
  Serial.print(F("HTTP server started @ "));
  Serial.println(static_cast<IPAddress>(WiFi.localIP()));
  
}

// **********************************************************************************************

void moving_around()
{
  analogWrite(enLeft, SpeedLeft);
  analogWrite(enRight, SpeedRight);
}

void stop()
{
  analogWrite(enLeft, 0);
  analogWrite(enRight, 0);
}

void forward()
{
  digitalWrite(inLeft, HIGH);
  digitalWrite(inRight, LOW);
  moving_around();
}

void backward()
{
  digitalWrite(inLeft, LOW);
  digitalWrite(inRight, HIGH);
  moving_around();
}

void left()
{
  digitalWrite(inLeft, HIGH);
  digitalWrite(inRight, HIGH);
  moving_around();
}

void orbit_left_forward()
{
  digitalWrite(inLeft, HIGH);
  digitalWrite(inRight, LOW);
  analogWrite(enLeft, 140);
  analogWrite(enRight, 70);
}

void orbit_left_backward()
{
  digitalWrite(inLeft, LOW);
  digitalWrite(inRight, HIGH);
  analogWrite(enLeft, 140);
  analogWrite(enRight, 70);
}

void right()
{
  digitalWrite(inLeft, LOW);
  digitalWrite(inRight, LOW);
  moving_around();
}

void orbit_right_forward()
{
  digitalWrite(inLeft, HIGH);
  digitalWrite(inRight, LOW);
  analogWrite(enLeft, 70);
  analogWrite(enRight, 140);
}

void orbit_right_backward()
{
  digitalWrite(inLeft, LOW);
  digitalWrite(inRight, HIGH);
  analogWrite(enLeft, 70);
  analogWrite(enRight, 140);
}

// **********************************************************************************************

int getRadio(){

  /*
   * Returns the frequency of the detected radio modulating wave (if one of the two defined ones).
   * Returns -1 if not one of the defined frequencies, or no radio signal
   */

  const int SAMPLES = 40;
   
  int totalSamples = 0;
  int samples151 = 0;
  int samples239 = 0;

  while (totalSamples < SAMPLES){
    int onTime = pulseIn(radioPin, HIGH, 10000);
    int offTime = 0;

    if (onTime != 0){
    
      offTime = pulseIn(radioPin, LOW, 10000);

      if (offTime != 0){
        int timePeriod = onTime + offTime; // in microseconds, total time for the on and off period
        float frequency = 1 / (timePeriod / 1000000.0);
    
        if (frequency > 70 && frequency < 190){
          samples151 += 1;
        }else if (frequency > 200 && frequency < 300){
          samples239 += 1;
        }

      }

    }

    totalSamples += 1;
    
  }
  
  // Identifies actual frequency
  
  if (samples151 >= SAMPLES / 2){
    return 151;
  }else if (samples239 >= SAMPLES / 2){
    return 239;
  }
   
   return -1;
    
}

int getUltrasonic(){

  /*
   * Returns 1 if accoustic waves detected. -1 otherwise.
   */

  const int SAMPLES = 10;

  int totalSamples = 0;
  int samplesDetected = 0;

  while (totalSamples < SAMPLES){
    // Clear the trigPin condition 
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 
    
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds 'tricking' sensor
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW); 
    
    // Reads the echoPin 
    int duration = pulseIn(echoPin, HIGH); 

    if(duration < 5000){ 
      samplesDetected += 1; // Ultrasonic detected
    } 

    totalSamples += 1;
  }
  
  if (samplesDetected >= SAMPLES / 2){
    return 1;
  }

  return -1;
  
}

int getMagnetic(){

  /*
   * Returns 1 for upwards field, 0 for downwards, and -1 for no field
   */

  const int SAMPLES = 20;

  int totalSamples = 0;
  int samplesUp = 0;
  int samplesDown = 0;

  while (totalSamples < SAMPLES){

    double val = analogRead(magneticPin);

    //Serial.println(val);

    if (val > 50){
      samplesUp += 1; // Up
    }
    else if (val < 30){
      samplesDown += 1; // Down
    }

    totalSamples += 1;
    
  }
  
  if (samplesUp >= SAMPLES/2){
    return 1;
  }else if (samplesDown >= SAMPLES/2){
    return 0;
  }
  
  return -1; // No field detected
   
}

int getIR(){

  /*
   * Returns the frequency of the detected IR pulses (if one of the two defined ones).
   * Returns -1 if not one of the defined frequencies, or no IR signal
   */

  const int SAMPLES = 40;

  int totalSamples = 0;
  int samples353 = 0;
  int samples571 = 0;

  while (totalSamples < SAMPLES){
    int onTime = pulseIn(IRPin, HIGH, 30000);
    int offTime = 0;

    if (onTime != 0){
    
      offTime = pulseIn(IRPin, LOW, 30000);

      if (offTime != 0){
      
        int timePeriod = onTime + offTime; // in microseconds, total time for the on and off period
        float frequency = 1 / (timePeriod / 1000000.0);
    
        //Serial.println(frequency);
    
        if (frequency > 250 && frequency < 450){
          samples353 += 1;
        }else if (frequency > 450 && frequency < 670){
          samples571 += 1;
        }

      }

    }

    totalSamples += 1;
    
  }
  
  // Identifies actual frequency
  if (samples353 >= SAMPLES / 2){
    return 353;
  }else if (samples571 >= SAMPLES / 2){
    return 571;
  }

  return -1;
  
}

String scan() {

  /*
   * Reads all sensors and compares to table of properties to return rock type
   */

  
  int radioFreq = getRadio();
  int ultrasonic = getUltrasonic();
  int magnetic = getMagnetic();
  int IRFreq = getIR();

  Serial.println("Data");
  Serial.print("Radio: ");
  Serial.println(radioFreq);
  Serial.print("Ultrasonic: ");
  Serial.println(ultrasonic);
  Serial.print("Magnetic: ");
  Serial.println(magnetic);
  Serial.print("IR: ");
  Serial.println(IRFreq);
  
  if (radioFreq == 151 && ultrasonic == 1 && magnetic == -1 && IRFreq == -1){
    return "Gaborium";
  }else if (radioFreq == 239 && ultrasonic == -1 && magnetic == -1 && IRFreq == -1){
    return "Lathwaite";
  }else if (radioFreq == 151 && ultrasonic == -1 && magnetic == 1 && IRFreq == -1){
    return "Adamantine";
  }else if (radioFreq == 239 && ultrasonic == -1 && magnetic == 0 && IRFreq == -1){
    return "Xirang";
  }else if (radioFreq == -1 && ultrasonic == -1 && magnetic == -1 && IRFreq == 353){
    return "Thiotimoline";
  }else if (radioFreq == -1 && ultrasonic == 1 && magnetic == -1 && IRFreq == 571){
    return "Netherite";
  }

  return "Unknown";
  
}


// **********************************************************************************************

void loop() {

  server.handleClient();
    
  command = server.arg("State");
  Serial.println(command);
  if (command == "F") forward();
  else if (command == "B") backward();
  else if (command == "L") left();
  else if (command == "R") right();
  else if (command == "I") orbit_right_forward();
  else if (command == "G") orbit_left_forward();
  else if (command == "J") orbit_right_backward();
  else if (command == "H") orbit_left_backward();
  else if (command == "S"){
    sentType = false;
    stop();
  }
  else if (command == "SC"){
    if (sentType == false){
      rockType = scan();
      Serial.println(rockType);
      server.send(200, "text/html", rockType);
      sentType = true;
    }else{
      server.send(200, "text/html", rockType);
      Serial.println("skipped 2nd scan");
    }
  }
  
}

// **********************************************************************************************
