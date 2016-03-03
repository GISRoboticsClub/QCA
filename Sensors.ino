//
// Code for setting up and reading the Sensors
//
//     
// Arduino to Raspberry Pi data transfer
// 2 bytes = 16 bits
//
// The 2 bytes are organized this way:
//
//  0 \
//  1  \
//  2   \
//  3    \
//  4     \
//  5     /10 bit Data
//  6    /
//  7   /
//  8  /
//  9 /
// 10 \
// 11  +- Forced to 0 for error detection
// 12 / 
// 13 \
// 14  +- 3 bit Command
// 15 /
//
//
//////////////////////////////////////////////////////////////
void setup_Sensors(){
  
}

void loop_Sensors(){
  outputSensorValue(Command_Time, systemTime/1000);
  sensor1();
  sensor2();
  sensor3();
  sensor4();
}

void sensor1(){
  int squarevalue;
  if ((systemTime/1000)%60 > 30){
      squarevalue = 1023;
    }
    else {
      squarevalue = 0;
    }
  outputSensorValue(Command_Sensor1, squarevalue);
}

void sensor2(){
  int triangularvalue;
  if ((systemTime/1000)%60 < 30) {
      triangularvalue = systemTime%60 * (1023/30);
    }
    else {
      triangularvalue = 2046 - (systemTime%60 * (1023/30));
    }
  outputSensorValue(Command_Sensor2, triangularvalue);
}

void sensor3(){
  int sawtoothvalue;
  sawtoothvalue = (systemTime/1000)%60 * (1023/60);
  outputSensorValue(Command_Sensor3, sawtoothvalue);
}

void sensor4(){
  int sinvalue;
  sinvalue = 512*(1 + sin((2*3.14159)*systemTime/60));
  outputSensorValue(Command_Sensor4, sinvalue);
}

void outputSensorValue(unsigned int sensorNumber, unsigned int sensorValue){      // 16-bit integers
  unsigned char hi,lo;
  
  sensorValue = sensorValue   & 0B0000001111111111;                               // 10 bits are for data
  sensorNumber = sensorNumber & 0B0000000000000111;                               // 3 bits are for sensorNumbers = commands
  sensorNumber = sensorNumber << 13;
  sensorValue = sensorValue | sensorNumber;
  
  lo = byte(sensorValue & 0B0000000011111111);
  hi = byte(sensorValue >> 8);
  Serial.write(hi);
  Serial.write(lo);
//  print8bits(hi);
//  print8bits(lo);
//  Serial.println(" ");

}
