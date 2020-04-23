#include <Agent.h>

#define STATUS_LED_PIN  13
#define RF_TX_PIN 2
#define RF_RX_PIN 3
    
#define TOPIC 0
    
String agentName = "A2";
Agent agent2( agentName, 1 );

void setup()
{
  Serial.begin( 9600 );
  Serial.println( "init" );
    
  pinMode(STATUS_LED_PIN, OUTPUT);
  
  agent2.addLogEvent( logLine );
  agent2.registerAgent(0, COMM_RF, "A1", RF_TX_PIN, RF_RX_PIN );
  agent2.addMessageReceivedEvent( onMessageReceive );
  
  // register new behaviour for that topic
  agent2.addBehaviour( BEHAVIOUR_RECEIVER, TOPIC, onReceiverBehaviour );
  agent2.addBehaviour( BEHAVIOUR_TICKER, 0, onReceiverTickerBehaviourA, 2 );
}

void onReceiverTickerBehaviourA( )
{
  logLine( "onReceiverTickerBehaviourA" );
}

void loop()
{
  agent2.sync();
}

void onMessageReceive( ACLMessage aclMessage )
{
  logLine( "onMessageReceive " + aclMessage.sender + " " + aclMessage.content );
}

void onReceiverBehaviour( ACLMessage aclMessage )
{
  if ( aclMessage.topic == String( TOPIC ) )
  {
    logLine( "onReceiverBehaviour " + aclMessage.sender + " " + aclMessage.content );
   
    // Continue the CFP process
    if ( aclMessage.performative == P_CFP ) // We are getting CFP from the other agents
    {
      digitalWrite(STATUS_LED_PIN, LOW);
      String proposedDistance = String( getDistance() );
      ACLMessage proposalMessage = { agentName, aclMessage.sender, P_PROPOSE, String( TOPIC ), proposedDistance };
      agent2.send( proposalMessage );
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
  return 9;
}

void logLine( String logString )
{
  Serial.println( logString );
}
