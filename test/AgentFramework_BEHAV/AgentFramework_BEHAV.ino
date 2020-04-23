#include <Agent.h>

String agentName = "M";
Agent motorAgent( agentName, 1 );

void setup()
{
    Serial.begin(9600);
    
  motorAgent.addLogEvent( logLine );
  motorAgent.registerAgent(0, SERIAL_1, "S1");
  motorAgent.addMessageReceivedEvent( onMessageReceive );
  
  // register new behaviour for that topic
  motorAgent.addBehaviour( BEHAVIOUR_TICKER, 0, onReceiverTickerBehaviourA, 2 );
  motorAgent.addBehaviour( BEHAVIOUR_TICKER, 1, onReceiverTickerBehaviourB, 5 );

}

void loop()
{
  motorAgent.updateTimer();
}

void serialEvent()
{
  motorAgent.serialReceive( SERIAL_1 );
}

void onMessageReceive( ACLMessage aclMessage )
{
  logLine( "onMessageReceive " + aclMessage.sender + " " + aclMessage.content );
}

void onReceiverTickerBehaviourA( )
{
  logLine( "onReceiverTickerBehaviourA" );
}

void onReceiverTickerBehaviourB()
{
  logLine( "onReceiverTickerBehaviourB" );
}

void logLine( String logString )
{
  Serial.println(logString);
}
