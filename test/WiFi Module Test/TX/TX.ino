#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9, 10);
 
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
 
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1, pipes[1]);
 
  //
  // Start listening
  //
 
  radio.startListening();
}
 
void sendOrder()
{
  // First, stop listening so we can talk.
  radio.stopListening();
    
  int valueX = 150;
  char *msg = "Domini";
  
  Serial.print("Sending : ");
  //Serial.println(msg);
  
    // Take the time, and send it.  This will block until complete
    //bool ok = radio.write( &msg, strlen(msg) );

    const char text[] = "Domini Deliya";
    bool ok = radio.write(&text, sizeof(text));
    delay(100);
    
    if (ok)
    {
      Serial.println("Send Success");
    }
    else
    {
      Serial.println("Send Fail");
    }
    // Now, continue listening
    radio.startListening();
 
    // Wait here until we get a response, or timeout (10ms)
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while ( ! radio.available() && ! timeout )
      if (millis() - started_waiting_at > 1000 )
        timeout = true;
 
    // Describe the results
    if ( timeout )
    {
      // At this point the ACK did not arrived so the red LED has to be on
      Serial.println("Timeout");
    }
    else
    {
      // Grab the response, compare, and send to debugging spew
      unsigned int response;
      radio.read( &response, sizeof(int) );
 
     // At this point the ACK did arrived so the green LED has to be on
      Serial.print("Response : ");
      Serial.println(response);
    }
}
 
void loop(void)
{
    sendOrder();
}
