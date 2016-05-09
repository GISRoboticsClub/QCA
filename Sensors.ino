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

  do {
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
    AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  } while (!(AcX != -1) & (AcX != 0) & (AcY != -1) & (AcY != 0) & (AcZ != -1) & (AcZ != 0) & (Tmp != -1) &(Tmp != 0) & (GyX != -1) & (GyX != 0) & (GyY != -1) & (GyY != 0) & (GyZ != -1) & (GyZ != 0));

  if (!latchEndMissionSwitch) outputSensorValue(Command_Time, systemTime/1000); else outputSensorValue(Command_Time, 1000);
  outputSensorValue(Command_Sensor1, AcX/32+512);
  outputSensorValue(Command_Sensor2, AcY/32+512);
  outputSensorValue(Command_Sensor3, AcZ/32+512);
  outputSensorValue(Command_Sensor4, Tmp/32+512);

  if (serialdebug && linegraph) {
    cleargraphline();
    if (!latchEndMissionSwitch) markgraphlie(512, '|'); else markgraphlie(512, '-');
    markgraphlie(AcX/32+512, '1');
    markgraphlie(AcY/32+512, '2');
    markgraphlie(AcZ/32+512, '3');
    markgraphlie(Tmp/32+512, '4');
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
  if (serialdebug && !linegraph) print8bits(hi);
  if (serialdebug && !linegraph) print8bits(lo);
  if (serialdebug && !linegraph) Serial.println(" ");
}
