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
  int vals1, vals2, vals3, vals4;
  
  outputSensorValue(Command_Time, systemTime/1000);
  vals1 = sensor1();
  vals2 = sensor2();
  vals3 = sensor3();
  vals4 = sensor4();

  cleargraphline();
  markgraphlie(512, '|');
  markgraphlie(vals1, '1');
  markgraphlie(vals2, '2');
  markgraphlie(vals3, '3');
  markgraphlie(vals4, '4');
  printgraphline();
  
}

int sensor1(){
  int squarevalue;
  if ((systemTime/1000)%60 > 30){
      squarevalue = 1023;
    }
    else {
      squarevalue = 0;
    }
  outputSensorValue(Command_Sensor1, squarevalue);
  return squarevalue;
}

int sensor2(){
  int triangularvalue;
  int sTime;

  sTime = (systemTime/1000)%60;
  
  if (sTime < 30) {triangularvalue = sTime * 1023 / 30;}
    else          {triangularvalue = 1023 - (sTime-30) * 1023 / 30;}
  outputSensorValue(Command_Sensor2, triangularvalue);
  return triangularvalue;
}

int sensor3(){
  int sawtoothvalue;
  sawtoothvalue = (systemTime/1000)%60 * (1023/60);
  outputSensorValue(Command_Sensor3, sawtoothvalue);
  return sawtoothvalue;
}

int sensor4(){
  float svalue;
  int sTime, sinvalue;

  sTime = (systemTime/1000)%60;

  svalue = sin(2*3.14159 * ((float) sTime)/60.0);
  svalue = (1.0 + svalue) * 512.0;
  sinvalue = (int) svalue;
  
  outputSensorValue(Command_Sensor4, sinvalue);
  return sinvalue;
}

void outputSensorValue(unsigned int sensorNumber, unsigned int sensorValue){      // 16-bit integers
  unsigned char hi,lo;
  
  sensorValue = sensorValue   & 0B0000001111111111;                               // 10 bits are for data
  sensorNumber = sensorNumber & 0B0000000000000111;                               // 3 bits are for sensorNumbers = commands
  sensorNumber = sensorNumber << 13;
  sensorValue = sensorValue | sensorNumber;
  
  lo = byte(sensorValue & 0B0000000011111111);
  hi = byte(sensorValue >> 8);
//  Serial.write(hi);
//  Serial.write(lo);
//  print8bits(hi);
//  print8bits(lo);
//  Serial.println(" ");

}
