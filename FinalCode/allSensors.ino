// Radio
int radioPin = 7; // Output of radio circuit

// Accoustic
int echoPin = 5; // Echo pin of HC-SR04
int trigPin = 9; // Trig pin of HC-SR04

// Magnetic
int magneticPin = A1; // Output of hall sensor circuit

// IR
int IRPin = 6;

void setup() {
  pinMode(IRPin, INPUT);
  pinMode(magneticPin,INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(radioPin, INPUT);
  Serial.begin(9600);
}


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

void loop() {

  Serial.println(scan());
  Serial.println("");
  
}
