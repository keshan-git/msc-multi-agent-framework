#include <Agent.h>

String agentName = "M";
Agent motorAgent( agentName, 1 );

void setup()
{
  motorAgent.init( );
  motorAgent.registerAgent(0, SERIAL_1, "S1");
  motorAgent.addMessageReceivedEvent( onMessageReceive );

  // Setup CFP process - Negotiation Feature
  setupCFP();
}

void loop()
{
  
}

// This method will initiate the CFP for the distance messurment
void setupCFP()
{
  String topic = "Distance";
  
  // Initiate the process by sending ACL message - CFP
  ACLMessage cfpMessage = { agentName, "", P_CFP, topic, "US Distance" };
  motorAgent.sendToAll( cfpMessage );
  
  // register new behaviour for that topic
  motorAgent.addBehaviour( BEHAVIOUR_RECEIVER, 0, onReceiverBehaviour );
}

void serialEvent()
{
  motorAgent.serialReceive();
}

void onMessageReceive( ACLMessage aclMessage )
{
  Serial.println( "onMessageReceive " + aclMessage.sender + " " + aclMessage.content );
}

int status = 0; // status 0 - CFP is send, status 1 - accepted
int lowestDistance = 10;
String acceptedAgent = "";
int proposalCount = 0;

void onReceiverBehaviour( ACLMessage aclMessage )
{
  if ( aclMessage.topic == "Distance" )
  {
    Serial.println( "onReceiverBehaviour " + aclMessage.sender + " " + aclMessage.content );
   
    // Continue the CFP process
    switch ( status )
    {
      case 0 :
      {
        if ( aclMessage.performative == P_PROPOSE ) // We are getting proposals from the other agents
        {
          int proposedDistance = aclMessage.content.toInt();
          if ( proposedDistance < lowestDistance )
          {
            lowestDistance = proposedDistance;
            acceptedAgent = aclMessage.sender;
            Serial.println( "Lowest Distance From : " + acceptedAgent + " - " + lowestDistance);
          }
          else
          {
            // We can't accept this distance (It is larger than the current minimum)
            ACLMessage rejectMessage = { agentName, aclMessage.sender, P_REJECT_PROPOSAL, "Distance", "" };
            motorAgent.send( rejectMessage );
            Serial.println( aclMessage.sender + " REJECTED" );
          }
          
          proposalCount++;
          
          if ( proposalCount >= 3 && acceptedAgent != NULL )
          {
            // We have accepted distance - Send accepted ACL
            ACLMessage acceptMessage = { agentName, acceptedAgent, P_ACCEPT_PROPOSAL, "Distance", "" };
            motorAgent.send( acceptMessage );
            status++;
            Serial.println( aclMessage.sender + " ACCEPTED" );
          }
        }
        break;
      }
    }
  }
}
