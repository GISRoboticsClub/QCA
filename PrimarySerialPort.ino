//
// Code for setting up and controlling the Primary Serial Port.
//
//     This is the Arduino Hardware USB port.

//////////////////////////////////////////////////////////////////////////
void setup_PrimarySerialPort() {

// Initialize for serial window debugging and communicating to the ESampler
    Serial.begin(9600);
    while (!Serial) { ; } // wait for serial port to connect. Needed for Leonardo only

    if (serialdebug) Serial.println("Setting up Arduino System");         // Notice this debug is for all debugging...
}


//////////////////////////////////////////////////////////////////////////
void loop_PrimarySerialPort() {

}

