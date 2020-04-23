#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#include <Agent.h>

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ROZ     0xFBE0
#define GRI     0xBDF7

Adafruit_TFTLCD lcdScreen(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

String agentName = "M";
Agent motorAgent( agentName, 2 );

void setup()
{
  // initialize the LCD
  lcdScreen.reset();
  uint16_t identifier = lcdScreen.readID();
  lcdScreen.begin( identifier );
  initDraw();
  
  motorAgent.addLogEvent( logLine );
  motorAgent.registerAgent(0, SERIAL_1, "S1");
  motorAgent.registerAgent(1, SERIAL_2, "S2");
  
  motorAgent.addMessageReceivedEvent( onMessageReceive );

  // Setup CFP process - Negotiation Feature
  setupCFP();
}

void initDraw()
{
  lcdScreen.fillScreen(BLACK);
  lcdScreen.fillRect(0, 0, 320, 240, BLACK);
  
  lcdScreen.setRotation(45);
  lcdScreen.setCursor(0, 0);
  lcdScreen.setTextColor(WHITE);  
  lcdScreen.setTextSize(2);
  lcdScreen.println("Motor Agent - M");
  lcdScreen.setTextSize(1);
}

void loop()
{
  
}

// This method will initiate the CFP for the distance messurment
void setupCFP()
{
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
  delay( 10 );
  motorAgent.serialReceive( SERIAL_2 );
}

void onMessageReceive( ACLMessage aclMessage )
{
  logLine( "onMessageReceive " + aclMessage.sender + " " + aclMessage.content );
}

int status = 0; // status 0 - CFP is send, status 1 - accepted
int lowestDistance = 1000;
String acceptedAgent = "";
int proposalCount = 0;

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
            logLine( aclMessage.sender + " ACCEPTED" );
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
  if ( lineCount > 200 )
  {
    lineCount = 20;
    initDraw();
  }
  else
  {
    lineCount = lineCount + 10;
  }
  
  lcdScreen.setCursor( 0, lineCount );
  lcdScreen.setTextColor( GREEN );  
  lcdScreen.println( "> " + logString );
}
