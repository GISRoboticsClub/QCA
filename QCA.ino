//
// This sketch builds a serial interface between two different serial devices.
//
//     One of the serial devices is the Arduino Hardware USB port.  This is called the Primary Serial Port.
//     The other is the software serial port using the Arduino
//          software serial port software.  This is called the Secondary Serial Port
//

// Watch Hitchhiker's Guide to the Galaxy!!!!!!!!!!

//////////////////////////////////////////////////////////////////////////
// Includes Files
#include <SoftwareSerial.h>
#include "DHT.h"

//Wiring to DHT22 module
//    Data - Blue  - Arduino Analog Pin 7
//    VCC  - Red   - Arduino 5V
//    GND  - Brown - Arduino GND

// Wiring to Pots
//    VCC  - Red   - Arduino 5V
//    GND  - Brown - Arduino GND
//    INP1 - Blue  - Arduino Analog 2
//    INP2 - Blue  - Arduino Analog 3

//Wiring to End of Mission Switch
//    GND - Yellow - Arduino GND
//    INP - Yellow - Arduino Digital Pin 6

//////////////////////////////////////////////////////////////////////////
// Debug Definitions
#define serialdebug                     true    // If TRUE echos debug info out the Hardware Serial Port, FALSE sends binary output to R PI
#define timerdebug                      false    // Enables the Timer Chain debug output
#define statemachinedebug               false    // Enables the State Machine debug output
#define primaryserialdebug              false   // Enables the Primary Serial Port debug output
#define secondaryserialdebug            false   // Enables the Primary Serial Port debug output
#define linegraph                       true    // Enables the Arduino-only line graph output


// Ardauino Digital Pin Definations
#define Pin0                            0       // Pin0 – Arduino Hardware USB Port (Primary) - RX
#define Pin1                            1       // Pin1 - Arduino Hardware USB Port (Primary) - TX
#define SecondarySerialPortRXPin        2       // Pin2 - Software Serial Port (Secondary) - RX       
#define SecondarySerialPortTXPin        3       // Pin3 - Software Serial Port (Secondary) - TX    
#define ReadingSecondarySerialPortPin   4       // Pin4 – LED showing when reading from the Secondary Serial Port
#define WritingSecondarySerialPortPin   5       // Pin5 - LED showing when writing to the Secondary Serial Port
#define EndMissionSwitch                6       // Pin6 - Switch closure to mark end of mission
#define DHT22InputPin                   7       // Pin7 - DHT22 Data Input Pin
#define Pin8                            8       // Pin8 -
#define Pin9                            9       // Pin9 -
#define Pin10                           10      // Pin10 - 
#define Pin11                           11      // Pin11 - 
#define Pin12                           12      // Pin12 - 
#define Pin13                           13      // Pin13 - 

// Ardauino Analog Pin Definations
#define TM35_Temperature_Sensor_Pin     A0      // A0 – TM35 Temperature Sensor
#define POT1_Pin                        A1      // Potentiometer #1
#define POT2_Pin                        A2      // Potentiometer #2
#define A3                              A3
#define A4                              A4
#define A5                              A5

// Primary Serial Port Definations

// Secondary Serial Port Definations
#define SSP_bufferSize                  32      // MUST BE BINARY NUMBERS!

// Timer Chain Definations - all timing in msec
#define quarterSecond                   250
#define halfSecond                      500
#define oneSecond                       1000
#define fiveSecond                      5000
#define tenSecond                       10000
#define sixtySecond                     60000

// Sensor Communication Commands
#define Command_Time                 0B000
#define Command_Sensor1              0B001
#define Command_Sensor2              0B010
#define Command_Sensor3              0B011
#define Command_Sensor4              0B100
#define Command5                     0B101
#define Command6                     0B110
#define Command7                     0B111

// Arduino Serial Graphics
#define graphlinemax                  101

//////////////////////////////////////////////////////////////////////////
// General Global Variables

// DHT22 Variables

DHT dht;
float humidity;
float temperature;

// Other Sensors

int POT1_Value;
int POT2_Value;

// Timer Chain Variables
unsigned long systemTime;                                 // Valid system time for the current "loop"
unsigned long quarterSecondTime = quarterSecond;
unsigned long halfSecondTime    = halfSecond;
unsigned long oneSecondTime     = oneSecond;
unsigned long fiveSecondTime    = fiveSecond;
unsigned long tenSecondTime     = tenSecond;
unsigned long sixtySecondTime   = sixtySecond;

boolean latchEndMissionSwitch   = false;

// Primary Serial Port Variables

// Secondary Serial Port Variables
SoftwareSerial SecondarySerialPort(SecondarySerialPortRXPin, SecondarySerialPortTXPin); // RX, TX
int SSP_c = 0;   // next place to store data
int SSP_f = 0;   // next char to send
int SSP_charcount = 0;  // counts
char SSP_buffer[SSP_bufferSize];

// Arduino Serial Graphics
char graphline[graphlinemax];

//////////////////////////////////////////////////////////////////////////
void setup() {

  dht.setup(DHT22InputPin);

  setup_TimerChain();
  setup_PrimarySerialPort();
  setup_SecondarySerialPort();
  setup_Sensors();
  
  pinMode(EndMissionSwitch, INPUT_PULLUP);
}   

//////////////////////////////////////////////////////////////////////////
void loop() {

// Timer CHain

  systemTime = millis();

//Serial.println(digitalRead(EndMissionSwitch));
  if (latchEndMissionSwitch | digitalRead(EndMissionSwitch) == LOW) latchEndMissionSwitch = true; 

// Process Kernal Loops

  loop_TimerChain();
  loop_PrimarySerialPort();
  loop_SecondarySerialPort();
}

