//
// Code for setting up and controlling the Secondary Serial Port.
//
//     This is the hardware shield that is run by the SoftwareSerial library.

//////////////////////////////////////////////////////////////////////////
void setup_SecondarySerialPort() {
  int write_count;
  
   SecondarySerialPort.begin(9600);

// initialize the output digital pins
  pinMode(ReadingSecondarySerialPortPin, OUTPUT);    
  digitalWrite(ReadingSecondarySerialPortPin, LOW);

  pinMode(WritingSecondarySerialPortPin, OUTPUT);    
  digitalWrite(WritingSecondarySerialPortPin, LOW);

  SSP_c = 0; // make sure pointer set up correctly
  SSP_f = 0;
  SSP_charcount = 0; 
}

//////////////////////////////////////////////////////////////////////////
void loop_SecondarySerialPort() {
  char c;
  
// data received
  c = ReadSecondarySerialPort();
  if (c >= 0) {
// received data from raspberry pi    
  }

// check for data to send
  if (SSP_charcount > 0){
    WriteSecondarySerialPort(SSPBufferOut());
  }
}


//////////////////////////////////////////////////////////////////////////
char ReadSecondarySerialPort(){

  char c;
  
  digitalWrite(ReadingSecondarySerialPortPin, HIGH);
  c = SecondarySerialPort.read();
  digitalWrite(ReadingSecondarySerialPortPin, LOW);
  return c;
}


//////////////////////////////////////////////////////////////////////////
char WriteSecondarySerialPort(char c){

  int bytessent;
  
  digitalWrite(WritingSecondarySerialPortPin, HIGH);
  bytessent = SecondarySerialPort.write(c);
  digitalWrite(WritingSecondarySerialPortPin, LOW);

  return bytessent;
}


//////////////////////////////////////////////////////////////////////////
char SSPBufferIn(char char2send){

  if (SSP_charcount >= SSP_bufferSize) {
      // BIG TROUBLE!  Buffer full and we need to do something.  Right now just ignore the error and kill the old data
      SSP_charcount = 0;
    }
  SSP_buffer[SSP_c]= char2send;
  SSP_c++;
  SSP_c = SSP_c & (SSP_bufferSize - 1);
  SSP_charcount++;
}


//////////////////////////////////////////////////////////////////////////
char SSPBufferOut(){
    
  char char2send;
  char2send = SSP_buffer[SSP_f];
  SSP_f++;
  SSP_f = SSP_f & (SSP_bufferSize - 1);
  --SSP_charcount;
  return char2send;
}
///////////////////////////////////////////////////////////////////////////
//
// This routine converts a binary BYTE into a printed (Serial.write) value
//   Probably used for testing...
//
void print8bits(byte bits2print) {
  int i;
  for (i=0; i<=7; i++){
    if (bits2print & 0B10000000){
      Serial.write("1");
    }
    else{
      Serial.write("0");
    }
    bits2print = bits2print << 1;
  }
}

///////////////////////////////////////////////////////////////////////////
//
// This routine converts a binary INTEGER into a printed (Serial.write) value
//   Probably used for testing...
//
void print16bits(int int2print) {
//  Serial.print("hi = ");
  print8bits(int2print >> 8);
//  Serial.print(" lo = ");
  print8bits(int2print & 0B11111111);
}
