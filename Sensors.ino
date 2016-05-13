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

  pinMode(POT1_Pin, INPUT);
  pinMode(POT2_Pin, INPUT);
}

//////////////////////////////////////////////////////////////
//
void loop_Sensors(){

//  delay(dht.getMinimumSamplingPeriod());      // 2016-05-13 delay = 2000 (2 seconds). Disregarding this important parameter to match existing QCA software.  It seems to work...

  humidity = 10*dht.getHumidity();
  temperature = 10*dht.getTemperature();
  POT1_Value = analogRead(POT1_Pin);
  POT2_Value = analogRead(POT2_Pin);

  if (serialdebug&& !linegraph) {
    Serial.print("getStatusString() = "); Serial.print(dht.getStatusString());
    Serial.print("\tHumidity = ");        Serial.print(humidity, 1);
    Serial.print("\t\tTemp (C) = ");      Serial.print(temperature, 1);
    Serial.print("\t\tTemp (F) = ");      Serial.println(dht.toFahrenheit(temperature), 1);
    Serial.print("POT1_Value = ");        Serial.print(POT1_Value);
    Serial.print("\t\tPOT2_Value = ");    Serial.println(POT2_Value);
  }
  
  if (!latchEndMissionSwitch) outputSensorValue(Command_Time, systemTime/1000); else outputSensorValue(Command_Time, 1000);
  outputSensorValue(Command_Sensor1, humidity);
  outputSensorValue(Command_Sensor2, temperature);
  outputSensorValue(Command_Sensor3, POT1_Value);
  outputSensorValue(Command_Sensor4, POT2_Value);

  if (serialdebug && linegraph) {
    cleargraphline();
    if (!latchEndMissionSwitch) markgraphlie(512, '|'); else markgraphlie(512, '-');
    markgraphlie(humidity, 'H');
    markgraphlie(temperature, 'T');
    markgraphlie(POT1_Value, '1');
    markgraphlie(POT2_Value, '2');
    printgraphline();
  }
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
//  if (serialdebug && !linegraph) print8bits(hi);
//  if (serialdebug && !linegraph) print8bits(lo);
  if (serialdebug && !linegraph) Serial.print(hi);
  if (serialdebug && !linegraph) Serial.print(lo);
  if (serialdebug && !linegraph) Serial.println(" ");
}
