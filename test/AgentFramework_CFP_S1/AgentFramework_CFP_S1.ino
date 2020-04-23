#include <Agent.h>
#include <NewPing.h>
#include <LiquidCrystal.h>

#define STATUS_LED_PIN  13

#define TRIGGER_PIN  51
#define ECHO_PIN  53
#define POWER_PIN  49

#define MAX_DISTANCE 2000
    
String agentName = "S1";
Agent sensorAgent( agentName, 1 );

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup()
{
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Init...");
  
  sensorAgent.addLogEvent( logLine );
  sensorAgent.registerAgent(0, SERIAL_1, "M");
  sensorAgent.addMessageReceivedEvent( onMessageReceive );
  
  // register new behaviour for that topic
  sensorAgent.addBehaviour( BEHAVIOUR_RECEIVER, 0, onReceiverBehaviour );
}

void loop()
{
}

void serialEvent1()
{
  sensorAgent.serialReceive( SERIAL_1 );
}

void onMessageReceive( ACLMessage aclMessage )
{
  logLine( "onMessageReceive " + aclMessage.sender + " " + aclMessage.content );
}

void onReceiverBehaviour( ACLMessage aclMessage )
{
  if ( aclMessage.topic == "Distance" )
  {
    logLine( "onReceiverBehaviour " + aclMessage.sender + " " + aclMessage.content );
   
    // Continue the CFP process
    if ( aclMessage.performative == P_CFP ) // We are getting CFP from the other agents
    {
      digitalWrite(STATUS_LED_PIN, LOW);
      String proposedDistance = String( getDistance() );
      ACLMessage proposalMessage = { agentName, aclMessage.sender, P_PROPOSE, "Distance", proposedDistance };
      sensorAgent.send( proposalMessage );
      logLine( proposalMessage.content + " Proposal Sent" );
    }
    else if ( aclMessage.performative == P_ACCEPT_PROPOSAL )
    {
      digitalWrite(STATUS_LED_PIN, HIGH);
      logLine( "Proposal Accepted by - " + aclMessage.sender);
    }
    else if ( aclMessage.performative == P_REJECT_PROPOSAL)
    {
      digitalWrite(STATUS_LED_PIN, LOW);
      logLine( "Proposal Rejected by - " + aclMessage.sender);
    }
  }
}

// This method will get the distance from HC-SR04
int getDistance()
{
    delay(200);
    int uS = sonar.ping();
    return ( uS / US_ROUNDTRIP_CM );
}

void logLine( String logString )
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print( logString );
  if ( logString.length() > 16 )
  {
    lcd.setCursor(0,1);
    lcd.print( logString.substring(16) );
  }
}
