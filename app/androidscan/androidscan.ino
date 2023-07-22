#define USE_WIFI_NINA         false
#define USE_WIFI101           true
#include <WiFiWebServer.h>
#include <WiFiClient.h> 

const char ssid[] = "EEERover";
const char pass[] = "exhibition";
const int groupNumber = 30; // Set your group number to make the IP address constant - only do this on the EEERover network

String command;

int SpeedRight = 250;
int SpeedLeft = 250;

int enLeft = 3;
int inLeft = 4;

int enRight = 9;
int inRight = 8;



WiFiWebServer server(80);

//Return the web page

//Switch LED on and acknowledge

//Switch LED on and acknowledge

//Generate a 404 response with details of the failed request
/*void sendResult(){
  Client.println("HTTP/1.1 200 OK");
  Client.println("Content-Type: text/html");
  Client.println(""); //  Comillas importantes.
  
  Client.println("scan");
  
  Serial.print("Cliente desconectado: ");
  Serial.println(client.remoteIP());
  Client.flush();

  
  //client.stop();
}*/
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


void setup()
{
  Serial.begin(115200);
  //  Attach the servos on Arduino PWM pin 3 and PWM pin 5
  //  Set servos to mid-range, ie, default stop
  pinMode(enLeft, OUTPUT);
  pinMode(enRight, OUTPUT);
  pinMode(inLeft, OUTPUT);
  pinMode(inRight, OUTPUT);

  Serial.println(F("\nStarting Web Server"));

  //Check WiFi shield is present

  //Configure the static IP address if group number is set
  if (groupNumber)
    WiFi.config(IPAddress(192,168,0,groupNumber+1));
    
  //WiFi.mode(WIFI_AP);

  // attempt to connect to WiFi network
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



//Call the server polling function in the main loop
void loop()
{
      server.handleClient();
    
      command = server.arg("State");
      if (command == "F") forward();
      else if (command == "B") backward();
      else if (command == "L") left();
      else if (command == "R") right();
      else if (command == "I") orbit_right_forward();
      else if (command == "G") orbit_left_forward();
      else if (command == "J") orbit_right_backward();
      else if (command == "H") orbit_left_backward();
      else if (command == "S") stop();
      else if (command == "SC"){
        server.send(200, "text/html", "scan");
      }
}

//HTTP_Handlroot

void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
