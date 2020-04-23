#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//
// Hardware configuration
//
 
// Set up nRF24L01 radio on SPI bus plus pins 7 & 8
RF24 radio(8, 7);
 
//
// Topology
//
 
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
 
void setup(void)
{
 
    Serial.begin(9600);
  //
  // Setup and configure rf radio
  //
 
  radio.begin();
 
  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15, 15);
 
  // optionally, reduce the payload size.  seems to
  // improve reliability
  radio.setPayloadSize(16);
 
  //
  // Open pipes to other nodes for communication
  //
 
  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)
 
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
 
  //
  // Start listening
  //
 
  radio.startListening();
 
}
 
 
void receiveOrder()
{
    // if there is data ready
    if ( radio.available() )
    {
      Serial.println("Available");
      // Dump the payloads until we've gotten everything
       unsigned long message;
       char *msg = "";
       
       char text[20] = {0};
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        //done = radio.read( &msg, 6 );
        
        done = radio.read(&text, sizeof(text));;
 
        // Use it
        Serial.print("Receive : ");
        Serial.println(text);
      }
 
     
      
    
      //Serial.println(text);
    
      // First, stop listening so we can talk
      radio.stopListening();
 
      unsigned long response = 120;
     Serial.print("Sending : ");
     Serial.println(response);
     

      // Send the final one back.
      radio.write( &response, sizeof(unsigned long) );
 
      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }
    
    //Serial.println("No Radio");
}
 
void loop(void)
{
  receiveOrder(); 
}
