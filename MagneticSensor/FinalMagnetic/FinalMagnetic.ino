#define Hall_Sensor A0          //A0 used with analog output, D2 with digital output
 
 
double Val1 = 0;             //Here you can store both values, the Val2 can be boolean
 
 
void setup() {
  
  pinMode(Hall_Sensor,INPUT);
  Serial.begin(9600);
 
}
 
void loop() {
 
   Val1=analogRead(Hall_Sensor);            //We read both values and display them raw on the serial monitor
   //Serial.println(Val1);
   //Serial.print("\t");
   if (Val1 < 55){
    Serial.println("UP");
   }
   else if (Val1 > 75){
    Serial.println("down");
   }
   else{
    Serial.println("0");
   }
}
