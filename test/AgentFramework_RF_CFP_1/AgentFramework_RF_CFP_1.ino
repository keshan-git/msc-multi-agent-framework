#include <Agent.h>

#define STATUS_LED_PIN  13
#define RF_TX_PIN 2
#define RF_RX_PIN 3
  
#define TOPIC 0

String agentName = "A1";
Agent agent1( agentName, 1 );

void setup()
{
  Serial.begin( 9600 );
  Serial.println( "init" );
    
  pinMode(STATUS_LED_PIN, OUTPUT);
  
  agent1.addLogEvent( logLine );
  agent1.registerAgent(0, COMM_RF, "A2", RF_TX_PIN, RF_RX_PIN );
  agent1.addMessageReceivedEvent( onMessageReceive );
  
  // Setup CFP process - Negotiation Feature
  setupCFP();
}

void loop()
{
  agent1.sync();
}

// This method will initiate the CFP for the distance messurment
void setupCFP()
{
  // register new behaviour for that topic
  agent1.addBehaviour( BEHAVIOUR_RECEIVER, TOPIC, onReceiverBehaviour );
  
  // Initiate the process by sending ACL message - CFP
  ACLMessage cfpMessage = { agentName, "-", P_CFP, String( TOPIC ) , "CM" };
  agent1.sendToAll( cfpMessage );
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
    if ( aclMessage.performative == P_PROPOSE ) // We are getting proposals from the other agents
    {
      int proposedDistance = aclMessage.content.toInt();
      if ( proposedDistance > 10 )
      {
        // We have accepted distance - Send accepted ACL
        ACLMessage acceptMessage = { agentName, aclMessage.sender, P_ACCEPT_PROPOSAL, String( TOPIC ), "" };
        agent1.send( acceptMessage );
        logLine( aclMessage.sender + " ACCEPTED" );
      }
      else
      {
        // We can't accept this distance
        ACLMessage rejectMessage = { agentName, aclMessage.sender, P_REJECT_PROPOSAL, String( TOPIC ), "" };
        agent1.send( rejectMessage );
        logLine( aclMessage.sender + " REJECTED" );
      }
    }
  }
}

void logLine( String logString )
{
  Serial.println( logString );
}
