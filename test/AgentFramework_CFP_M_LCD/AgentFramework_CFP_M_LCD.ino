#include <Agent.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

String agentName = "M";
Agent motorAgent( agentName, 2 );

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const byte rows = 4; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[rows] = {A11, A10, A9, A8}; //connect to the row pinouts of the keypad
byte colPins[cols] = {A15, A14, A13, A12}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

int status = 0; // status 0 - CFP is send, status 1 - accepted
int lowestDistance = 1000;
String acceptedAgent = "";
int proposalCount = 0;

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Init...");
  
  motorAgent.addLogEvent( logLine );
  motorAgent.registerAgent(0, SERIAL_1, "S1");
  motorAgent.registerAgent(1, SERIAL_2, "S2");
  
  motorAgent.addMessageReceivedEvent( onMessageReceive );
  
  setupCFP();
}

void loop()
{
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    if ( key == '#' )
    {
      // Setup CFP process - Negotiation Feature
      setupCFP();
    }
  }
  
  int keyValue = getKeyValue();
  if ( keyValue == 1 )
  {
    // Setup CFP process - Negotiation Feature
    setupCFP();
  }
}

// This method will initiate the CFP for the distance messurment
void setupCFP()
{
  status = 0; // status 0 - CFP is send, status 1 - accepted
  lowestDistance = 1000;
  acceptedAgent = "";
  proposalCount = 0;
  
  String topic = "Distance";

  // Initiate the process by sending ACL message - CFP
  ACLMessage cfpMessage = { agentName, "-", P_CFP, topic, "CM" };
  motorAgent.sendToAll( cfpMessage );
  
  // register new behaviour for that topic
  motorAgent.addBehaviour( BEHAVIOUR_RECEIVER, 0, onReceiverBehaviour );
}

void serialEvent1()
{
  motorAgent.serialReceive( SERIAL_1 );
}

void serialEvent2()
{
  //delay( 10 );
  motorAgent.serialReceive( SERIAL_2 );
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
            logLine( "Lowest Distance From : " + acceptedAgent + " - " + lowestDistance);
          }
          else
          {
            // We can't accept this distance (It is larger than the current minimum)
            ACLMessage rejectMessage = { agentName, aclMessage.sender, P_REJECT_PROPOSAL, "Distance", "" };
            motorAgent.send( rejectMessage );
            logLine( aclMessage.sender + " REJECTED" );
          }
          
          proposalCount++;
          
          if ( proposalCount >= 2 && acceptedAgent != NULL )
          {
            // We have accepted distance - Send accepted ACL
            ACLMessage acceptMessage = { agentName, acceptedAgent, P_ACCEPT_PROPOSAL, "Distance", "" };
            motorAgent.send( acceptMessage );
            status++;
            logLine( acceptedAgent + " ACCEPTED" );
          }
        }
        break;
      }
    }
  }
}

int lineCount = 20;
void logLine( String logString )
{
  //delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print( logString );
  if ( logString.length() > 16 )
  {
    lcd.setCursor(0,1);
    lcd.print( logString.substring(16) );
  }
}

int getKeyValue()
{
  int x = analogRead (0);
  
  if (x < 60) 
  {
    return 4;
  }
  else if (x < 200)
  {
    return 1;
  }
  else if (x < 400)
  {
    return 3;
  }
  else if (x < 600)
  {
    return 2;
  }
  else if (x < 800)
  {
    return -1;
  }
  else
  {
    return -2;
  }
}
