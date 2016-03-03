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


//////////////////////////////////////////////////////////////////////////
// Debug Definitions
#define serialdebug                     false    // Echos debug info out the Hardware Serial Port
#define timerdebug                      false    // Enables the Timer Chain debug output
#define statemachinedebug               false    // Enables the State Machine debug output
#define primaryserialdebug              false   // Enables the Primary Serial Port debug output
#define secondaryserialdebug            false   // Enables the Primary Serial Port debug output

// Ardauino Pin Definations
#define Pin0                            0       // Pin0 – Arduino Hardware USB Port (Primary) - RX
#define Pin1                            1       // Pin1 - Arduino Hardware USB Port (Primary) - TX
#define SecondarySerialPortRXPin        2       // Pin2 - Software Serial Port (Secondary) - RX       
#define SecondarySerialPortTXPin        3       // Pin3 - Software Serial Port (Secondary) - TX    
#define ReadingSecondarySerialPortPin   4       // Pin4 – LED showing when reading from the Secondary Serial Port
#define WritingSecondarySerialPortPin   5       // Pin5 - LED showing when writing to the Secondary Serial Port
#define Pin6                            6       // Pin6 - 
#define Pin7                            7       // Pin7 - 
#define Pin8                            8       // Pin8 -
#define Pin9                            9       // Pin9 -
#define Pin10                           10      // Pin10 - 
#define Pin11                           11      // Pin11 - 
#define Pin12                           12      // Pin12 - 
#define Pin13                           13      // Pin13 - 

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

//////////////////////////////////////////////////////////////////////////
// General Global Variables

// Timer Chain Variables
unsigned long systemTime;                                 // Valid system time for the current "loop"
unsigned long quarterSecondTime = quarterSecond;
unsigned long halfSecondTime    = halfSecond;
unsigned long oneSecondTime     = oneSecond;
unsigned long fiveSecondTime    = fiveSecond;
unsigned long tenSecondTime     = tenSecond;
unsigned long sixtySecondTime   = sixtySecond;

// Primary Serial Port Variables

// Secondary Serial Port Variables
SoftwareSerial SecondarySerialPort(SecondarySerialPortRXPin, SecondarySerialPortTXPin); // RX, TX
int SSP_c = 0;   // next place to store data
int SSP_f = 0;   // next char to send
int SSP_charcount = 0;  // counts
char SSP_buffer[SSP_bufferSize];

//////////////////////////////////////////////////////////////////////////
void setup() {
  setup_TimerChain();
  setup_PrimarySerialPort();
  setup_SecondarySerialPort();
  setup_Sensors();
}   

//////////////////////////////////////////////////////////////////////////
void loop() {

  systemTime = millis();
  
  loop_TimerChain();
  loop_PrimarySerialPort();
  loop_SecondarySerialPort();
}

