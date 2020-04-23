#include <Agent.h>

Agent agent( "A", 1 );

void setup()
{
  agent.init( );
  agent.registerAgent(0, SERIAL_1, "C");
  //agent.registerAgent(1, SERIAL_2, "E");
  //agent.registerAgent(2, SERIAL_3, "B");
  
  //agent.printAgentMap();
  
  agent.addMessageReceivedEvent( onMessageReceive );
  
  //agent.send( "E", "Hello, Welcome to the MAS" );
  //agent.sendToAll( "Hello All, Welcome to the MAS" );
  
  pinMode(13, OUTPUT);
  
  // register new behaviour
  agent.addBehaviour( BEHAVIOUR_RECEIVER, 0, onReceiverBehaviour );
}

void loop()
{
  
}

void serialEvent()
{
  agent.serialReceive();
}

void onMessageReceive( ACLMessage aclMessage )
{
  Serial.print( "onMessageReceive " + aclMessage.sender );
  Serial.println( " " + aclMessage.content );
  
//  if ( aclMessage.sender.equals("C"))
//  {
//    if ( aclMessage.content.equals("ON"))
//    {
//      digitalWrite(13, HIGH);
//    }
//    else
//    {
//      digitalWrite(13, LOW);
//    }
//  }
}

void onReceiverBehaviour( ACLMessage acl )
{
  Serial.print( "onReceiverBehaviour " + acl.sender );
  Serial.println( " " + acl.content );
}
