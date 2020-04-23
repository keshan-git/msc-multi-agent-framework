#include <Agent.h>
//#include <NewPing.h>
//#include <LiquidCrystal.h>

#define STATUS_LED_PIN  13
#define RF_TX_PIN 2
#define RF_RX_PIN 3
    
String agentName = "R2";
Agent radioAgent1( agentName, 1 );

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup()
{
  Serial.begin( 9600 );
  Serial.println( "init" );
    
  pinMode(STATUS_LED_PIN, OUTPUT);
  //pinMode(POWER_PIN, OUTPUT);
  //digitalWrite(POWER_PIN, HIGH);
  
  // set up the LCD's number of columns and rows:
  //lcd.begin(16, 2);
  //lcd.setCursor(0,0);
  //lcd.print("Init...");
  
  radioAgent1.addLogEvent( logLine );
  radioAgent1.registerAgent(0, COMM_RF, "R1", RF_TX_PIN, RF_RX_PIN );
  radioAgent1.addMessageReceivedEvent( onMessageReceive );
  
  // register new behaviour for that topic
  //sensorAgent.addBehaviour( BEHAVIOUR_RECEIVER, 0, onReceiverBehaviour );
}

void loop()
{
  radioAgent1.sync();
}

void onMessageReceive( ACLMessage aclMessage )
{
  logLine( "onMessageReceive " + aclMessage.sender + " " + aclMessage.content );
}

void onReceiverBehaviour( ACLMessage aclMessage )
{
//  if ( aclMessage.topic == "Distance" )
//  {
//    logLine( "onReceiverBehaviour " + aclMessage.sender + " " + aclMessage.content );
//   
//    // Continue the CFP process
//    if ( aclMessage.performative == P_CFP ) // We are getting CFP from the other agents
//    {
//      digitalWrite(STATUS_LED_PIN, LOW);
//      String proposedDistance = String( getDistance() );
//      ACLMessage proposalMessage = { agentName, aclMessage.sender, P_PROPOSE, "Distance", proposedDistance };
//      sensorAgent.send( proposalMessage );
//      logLine( proposalMessage.content + " Proposal Sent" );
//    }
//    else if ( aclMessage.performative == P_ACCEPT_PROPOSAL )
//    {
//      digitalWrite(STATUS_LED_PIN, HIGH);
//      logLine( "Proposal Accepted by - " + aclMessage.sender);
//    }
//    else if ( aclMessage.performative == P_REJECT_PROPOSAL)
//    {
//      digitalWrite(STATUS_LED_PIN, LOW);
//      logLine( "Proposal Rejected by - " + aclMessage.sender);
//    }
//  }
}

// This method will get the distance from HC-SR04
//int getDistance()
//{
//    delay(200);
//    int uS = sonar.ping();
//    return ( uS / US_ROUNDTRIP_CM );
//}

void logLine( String logString )
{
  Serial.println( logString );
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print( logString );
  //if ( logString.length() > 16 )
  //{
    //lcd.setCursor(0,1);
    //lcd.print( logString.substring(16) );
  //}
}
