int IRPin = 13;

void setup() {
  pinMode(IRPin, INPUT);
  Serial.begin(9600);
}

int getIR(){

  /*
   * Returns the frequency of the detected IR pulses (if one of the two defined ones).
   * Returns -1 if not one of the defined frequencies, or no IR signal
   */

  int totalSamples = 0;
  int samples353 = 0;
  int samples571 = 0;

  while (totalSamples < 10){
    int onTime = pulseIn(IRPin, HIGH, 30000);
    int offTime = pulseIn(IRPin, LOW, 30000);
    int timePeriod = onTime + offTime; // in microseconds, total time for the on and off period
    float frequency = 1 / (timePeriod / 1000000.0);

    //Serial.println(frequency);

    if (frequency > 250 && frequency < 450){
      samples353 += 1;
    }else if (frequency > 450 && frequency < 670){
      samples571 += 1;
    }

    totalSamples += 1;
  }
  
  // Identifies actual frequency
  if (samples353 >= 5){
    return 353;
  }else if (samples571 >= 5){
    return 571;
  }else{
    return -1;
  }
  
}

void loop() {
  Serial.println(getIR());
}
