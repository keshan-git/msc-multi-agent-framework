#include <Agent.h>

String agentName = "A";
Agent agentA( agentName, 2 );

void setup()
{
  Serial.begin( 9600 );
  Serial.println( "--------------------init---------------------" );
  
  agentA.addLogEvent( logLine );
  agentA.registerAgent(0, SERIAL_2, "B");
  
  // Indirect Agents
  agentA.registerAgent(1, SERIAL_2, "C", true);
      
  agentA.addMessageReceivedEvent( onMessageReceive );
}

void loop()
{
  ACLMessage infoMessage1 = { agentName, "B", P_INFORM, COMMON_TOPIC, "Test1" };
  agentA.send( infoMessage1 );
  logLine( "Message Sent" );
    delay ( 1000 );
    
  ACLMessage infoMessage2 = { agentName, "C", P_INFORM, COMMON_TOPIC, "Test2" };
  agentA.send( infoMessage2 );
  logLine( "Message Sent" );
  
  delay ( 6000 );
}

void serialEvent1()
{
  agentA.serialReceive( SERIAL_1 );
}

void serialEvent2()
{
  agentA.serialReceive( SERIAL_2 );
}

void onMessageReceive( ACLMessage aclMessage )
{
  logLine( "onMessageReceive " + aclMessage.sender + " " + aclMessage.content );
}

void logLine( String logString )
{
  Serial.println( logString );
}
