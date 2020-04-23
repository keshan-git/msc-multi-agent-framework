#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#include <Agent.h>

String agentName = "C";
Agent agentC( agentName, 2 );

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define BLACK   0x0000
#define GREEN   0x07E0
#define WHITE   0xFFFF

Adafruit_TFTLCD lcdScreen(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup()
{
  // initialize the LCD
  lcdScreen.reset();
  uint16_t identifier = lcdScreen.readID();
  lcdScreen.begin( identifier );
  initDraw();
  
  Serial.begin( 9600 );
  Serial.println( "--------------------init---------------------" );
  
  agentC.addLogEvent( logLine );
  agentC.registerAgent(0, SERIAL_1, "B");
//  agentC.registerAgent(1, SERIAL_2, "D");
    
  // Indirect Agents
  agentC.registerAgent(1, SERIAL_1, "A", true);
      
  agentC.addMessageReceivedEvent( onMessageReceive );
}

void initDraw()
{
  lcdScreen.fillScreen(BLACK);
  lcdScreen.fillRect(0, 0, 320, 240, BLACK);
  
  lcdScreen.setRotation(135);
  lcdScreen.setCursor(0, 0);
  lcdScreen.setTextColor(WHITE);  
  lcdScreen.setTextSize(2);
  lcdScreen.println("Agent C");
  lcdScreen.setTextSize(1);
}

void loop()
{

}

void serialEvent1()
{
  agentC.serialReceive( SERIAL_1 );
}

void serialEvent2()
{
  agentC.serialReceive( SERIAL_2 );
}

void onMessageReceive( ACLMessage aclMessage )
{
  logLine( "onMessageReceive " + aclMessage.sender + " " + aclMessage.content );
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
 
  Serial.println( logString );
}
