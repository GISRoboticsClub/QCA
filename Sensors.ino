//////////////////////////////////////////////////////////////
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
//
void setup_Sensors(){
}

//////////////////////////////////////////////////////////////
//
void loop_Sensors(){
  int vals1, vals2, vals3, vals4;
  
  vals1 = sensor1();
  vals2 = sensor2();
  vals3 = sensor3();
  vals4 = sensor4();

  outputSensorValue(Command_Time, systemTime/1000);
  outputSensorValue(Command_Sensor1, vals1);
  outputSensorValue(Command_Sensor2, vals2);
  outputSensorValue(Command_Sensor3, vals3);
  outputSensorValue(Command_Sensor4, vals4);

  if (serialdebug && linegraph) {
    cleargraphline();
    markgraphlie(512, '|');
    markgraphlie(vals1, '1');
    markgraphlie(vals2, '2');
    markgraphlie(vals3, '3');
    markgraphlie(vals4, '4');
    printgraphline();
  }
}

//////////////////////////////////////////////////////////////
//
// TRM35 Temperature Sensor
//

int sensor1(){
  float sensor1_tempC;
  
  sensor1_tempC = analogRead(TM35_Temperature_Sensor_Pin);
  sensor1_tempC = (5.0 * sensor1_tempC * 100.0)/1024.0;
  
  if (serialdebug) Serial.print(sensor1_tempC);
  if (serialdebug) Serial.println();
 
  return sensor1_tempC;
}

//////////////////////////////////////////////////////////////
//
int sensor2(){
  int triangularvalue;
  int sTime;

  sTime = (systemTime/1000)%60;
  
  if (sTime < 30) {triangularvalue = sTime * 1023 / 30;}
    else          {triangularvalue = 1023 - (sTime-30) * 1023 / 30;}
  return triangularvalue;
}

//////////////////////////////////////////////////////////////
//
int sensor3(){
  int sawtoothvalue;
  sawtoothvalue = (systemTime/1000)%60 * (1023/60);
  return sawtoothvalue;
}

//////////////////////////////////////////////////////////////
//
int sensor4(){
  float svalue;
  int sTime, sinvalue;

  sTime = (systemTime/1000)%60;

  svalue = sin(2*3.14159 * ((float) sTime)/60.0);
  svalue = (1.0 + svalue) * 512.0;
  sinvalue = (int) svalue;
  return sinvalue;
}

//////////////////////////////////////////////////////////////
//
void outputSensorValue(unsigned int commandNumber, unsigned int sensorValue){     // 16-bit integers
  unsigned char hi,lo;
  
  sensorValue = sensorValue   & 0B0000001111111111;                               // 10 bits are for data
  commandNumber = commandNumber & 0B0000000000000111;                             // 3 bits are for commandNumber
  commandNumber = commandNumber << 13;
  sensorValue = sensorValue | commandNumber;
  
  lo = byte(sensorValue & 0B0000000011111111);
  hi = byte(sensorValue >> 8);

  if (!serialdebug) Serial.write(hi);
  if (!serialdebug) Serial.write(lo);
  if (serialdebug && !linegraph) print8bits(hi);
  if (serialdebug && !linegraph) print8bits(lo);
  if (serialdebug && !linegraph) Serial.println(" ");
}
