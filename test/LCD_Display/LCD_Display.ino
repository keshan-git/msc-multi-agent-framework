#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4

// http://stackoverflow.com/questions/13720937/c-defined-16bit-high-color
// http://wiibrew.org/wiki/U16_colors

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Define color schema for the UI
#define C_HEADER_BACKGROUND 0x0000 // MAGENTA
#define C_HEADER_FOREGROUND 0x0000  // WHITE
#define C_HEADER_SEPARATOR 0x0000  // WHITE
#define C_HEADER_ICON 0x0000       // WHITE

// action bar colors
#define C_ACTIONBAR_HOME_OUTER 0x0000 // WHITE
#define C_ACTIONBAR_NETWORK_OUTER 0x0000 // WHITE
#define C_ACTIONBAR_CFP_OUTER 0x0000 // WHITE
#define C_ACTIONBAR_RESOURCE_OUTER 0x0000 // WHITE
#define C_ACTIONBAR_LOG_OUTER 0x0000 // WHITE

// main panel colors
#define C_AGENT_SUM_PANEL_BORDER 0x0000 // BLUE
#define C_AGENT_SUM_PANEL_FOREGROUND 0x0000 // WHITE
#define C_AGENT_SUM_PANEL_CONNECT_BACKGROUND 0x0000 // GREEN
#define C_AGENT_SUM_PANEL_CONNECT_FOREGROUND 0x0000 // WHITE
#define C_AGENT_SUM_PANEL_DISCONNECT_BACKGROUND 0x0000 // RED
#define C_AGENT_SUM_PANEL_COMTYPE_BACKGROUND 0x0000 // MAGENTA

// agent panel colors
#define C_AGENT_PANEL_TITLE_BACKGROUND 0x0000 // BLUE
#define C_AGENT_PANEL_TITLE_FOREGROUND 0x0000 // WHITE
#define C_AGENT_PANEL_METER_FOREGROUND 0x0000 // WHITE
#define C_AGENT_PANEL_METER_BACKGROUND_NORMAL 0x0000 // GREEN
#define C_AGENT_PANEL_METER_BACKGROUND_WARNING 0x0000 // YELLOW
#define C_AGENT_PANEL_METER_BACKGROUND_CRITICAL 0x0000 // RED
#define C_AGENT_PANEL_METER_TITLE_BACKGROUND 0x0000 // RED
#define C_AGENT_PANEL_METER_TITLE_FOREGROUND 0x0000 // WHITE

// CFP panel
#define C_CFP_PANEL_AGENT_BORDER 0x0000 // BLUE
#define C_CFP_PANEL_AGENT_TITLE_FOREGROUND 0x0000 // WHITE
#define C_CFP_PANEL_AGENT_TEMP_BACKGROUND 0x0000 // RED
#define C_CFP_PANEL_AGENT_TEMP_FOREGROUND 0x0000 // WHITE
#define C_CFP_PANEL_AGENT_HUM_BACKGROUND 0x0000 // RED
#define C_CFP_PANEL_AGENT_HUM_FOREGROUND 0x0000 // WHITE
#define C_CFP_PANEL_AGENT_WATER_BACKGROUND 0x0000 // RED
#define C_CFP_PANEL_AGENT_WATER_FOREGROUND 0x0000 // WHITE
#define C_CFP_PANEL_AGENT_WATER_REQUEST_BACKGROUND 0x0000 // GREEN
#define C_CFP_PANEL_AGENT_WATER_REQUEST_FOREGROUND 0x0000 // WHITE
#define C_CFP_PANEL_AGENT_WATER_ACCEPT_BACKGROUND 0x0000 // GREEN
#define C_CFP_PANEL_AGENT_WATER_ACCEPT_FOREGROUND 0x0000 // WHITE
#define C_CFP_PANEL_WATER_LEVEL_BACKGROUND 0x0000 // MAGENTA
#define C_CFP_PANEL_WATER_LEVEL_FOREGROUND 0x0000 // WHITE
#define C_CFP_PANEL_START_BUTTON_BORDER 0x0000 // RED
#define C_CFP_PANEL_START_BUTTON_FOREGROUND 0x0000 // RED

// Touch Panel Configurations
#define YP A1
#define XM A2
#define YM 7
#define XP 6
  
#define TS_MINX 130
#define TS_MAXX 930
#define TS_MINY 150
#define TS_MAXY 900

#define MINPRESSURE 5
#define MAXPRESSURE 1000

#define C_WIRED 1
#define C_RF 2
#define C_WIFI 3
#define C_BLE 4

#define HOME_SCREEN 1
#define PLANT_AGENT_SCREEN 2
#define CFP_SCREEN 3
#define LOG_SCREEN 4
#define AGENT_NETWORK_SCREEN 5

#define BTN_HOME 1
#define BTN_NETWORK 2
#define BTN_CFP 3
#define BTN_ 4
#define BTN_LOG 5
#define BTN_PLAGENT_1 6
#define BTN_PLAGENT_2 7
#define BTN_PLAGENT_3 8

Adafruit_TFTLCD lcd(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen touch = TouchScreen(XP, YP, XM, YM, 300);

int16_t tempPositionX = 0;
uint8_t currentScreen = HOME_SCREEN;
uint8_t currentPlantAgent = 0;
boolean isDrawing = false;

void setup()
{
  lcd.reset();
  uint16_t identifier = lcd.readID();
  lcd.begin(identifier);
  lcd.setRotation(3);

  loadScreen( HOME_SCREEN );
  Serial.begin(9600);
}


void loop()
{
  if ( !isDrawing )
  {
    digitalWrite( 13, HIGH );
    TSPoint currentPosition = touch.getPoint();
    digitalWrite( 13, LOW );
    pinMode( XM, OUTPUT );
    pinMode( YP, OUTPUT );

    if ( currentPosition.z > MINPRESSURE && currentPosition.z < MAXPRESSURE )
    {
      tempPositionX = currentPosition.x;
      currentPosition.x = map( currentPosition.y, TS_MINX, TS_MAXX, lcd.width(), 0 );
      currentPosition.y = map( tempPositionX, TS_MINY, TS_MAXY, 0, lcd.height() );

      onTouchEvent( currentPosition.x, currentPosition.y );
    }
  }
}

void lcdHeaderPanel()
{
  // draw outer rect
  lcd.fillRect( 0, 0, 320, 20, C_HEADER_BACKGROUND);

  // lcd agent nanem
  lcd.setCursor( 2, 2 );   lcd.setTextSize( 1 );
  lcd.setTextColor( C_HEADER_FOREGROUND );
  lcd.print( "WATER TANK AGENT" );

  lcd.setCursor( 2, 11 );
  lcd.print( "DASHBOARD" );

  // lcd date and time
  lcd.setCursor( 192, 2 );
  lcd.print( "12-DEC-2016" );

  lcd.setCursor( 228, 11 );
  lcd.print( "12:00" );

  // communication icons
  lcd.drawFastVLine( 300, 0, 20, C_HEADER_SEPARATOR);

  lcd.drawFastVLine( 280, 0, 20, C_HEADER_SEPARATOR);

  lcd.drawFastVLine( 260, 0, 20, C_HEADER_SEPARATOR);
}

void lcdMainPanel()
{
  // draw outer rect
  lcd.drawRoundRect( 5, 25, 200, 160, 5, WHITE);

  // draw unit sections
  //lcd
}

void showActionPanel( )
{
  showActionPanel( 0 );
}

void showActionPanel( uint8_t screenID )
{
  // draw outer rect
  //lcd.drawRect( 0, 190, 320, 50, WHITE);

  // draw action buttions
  // Home Button
  if ( screenID == HOME_SCREEN )
  {
    lcd.fillCircle( 41, 215, 23, C_ACTIONBAR_HOME_OUTER );
    lcd.fillCircle( 41, 215, 18, BLACK );
    lcd.drawCircle( 41, 215, 18, C_ACTIONBAR_HOME_OUTER );
  }
  else
  {
    lcd.drawCircle( 41, 215, 23, C_ACTIONBAR_HOME_OUTER );
    lcd.drawCircle( 41, 215, 18, C_ACTIONBAR_HOME_OUTER );
  }

  // Agent Network Button
  lcd.drawCircle( 101, 215, 23, C_ACTIONBAR_NETWORK_OUTER );
  lcd.drawCircle( 101, 215, 18, C_ACTIONBAR_NETWORK_OUTER );

  // CFP Button
  if ( screenID == CFP_SCREEN )
  {
    lcd.fillCircle( 164, 215, 23, C_ACTIONBAR_CFP_OUTER );
    lcd.fillCircle( 164, 215, 18, BLACK );
    lcd.drawCircle( 164, 215, 18, C_ACTIONBAR_CFP_OUTER );
  }
  else
  {
    lcd.drawCircle( 164, 215, 23, C_ACTIONBAR_CFP_OUTER );
    lcd.drawCircle( 164, 215, 18, C_ACTIONBAR_CFP_OUTER );
  }

  // Resource Button
  lcd.drawCircle( 224, 215, 23, C_ACTIONBAR_RESOURCE_OUTER );
  lcd.drawCircle( 224, 215, 18, C_ACTIONBAR_RESOURCE_OUTER );

  // Log Button
  lcd.drawCircle( 284, 215, 23, C_ACTIONBAR_LOG_OUTER );
  lcd.drawCircle( 284, 215, 18, C_ACTIONBAR_LOG_OUTER );
}

void lcdAgentInfoPanel( uint16_t x, uint16_t y, uint8_t agentNum, boolean con )
{
  lcdAgentInfoPanel( x, y, agentNum, con, 0 );
}

void lcdAgentInfoPanel( uint16_t x, uint16_t y, uint8_t agentNum, boolean con, uint8_t comType )
{
  // draw outer rect
  lcd.drawRect( x + 2, y, 106, 50, C_AGENT_SUM_PANEL_BORDER );

  // draw title bar
  lcd.fillRect( x, y, 110, 13, C_AGENT_SUM_PANEL_BORDER );
  lcd.setCursor( x + 2, y + 2 );   lcd.setTextSize( 1 );
  lcd.setTextColor( C_AGENT_SUM_PANEL_FOREGROUND );
  lcd.print( "PLANT AGENT 0" );
  lcd.println( agentNum );

  // draw icon
  lcd.drawCircle( x + 21, y + 30, 14, WHITE );

  // draw connected string
  lcd.setCursor( x + 47, y + 36 );   lcd.setTextSize( 1 );
  lcd.setTextColor( C_AGENT_SUM_PANEL_CONNECT_FOREGROUND );

  if ( !con )
  {
    lcd.fillRect( x + 45, y + 33, 58, 13, C_AGENT_SUM_PANEL_DISCONNECT_BACKGROUND );
    lcd.println( " OFFLINE " );
  }
  else
  {
    lcd.fillRect( x + 45, y + 33, 58, 13, C_AGENT_SUM_PANEL_CONNECT_BACKGROUND );
    lcd.println( "  ONLINE " );

    // draw comm channel string
    lcd.fillRect( x + 45, y + 16, 58, 13, C_AGENT_SUM_PANEL_COMTYPE_BACKGROUND );
    lcd.setCursor( x + 47, y + 19 );   lcd.setTextSize( 1 );
    lcd.setTextColor( C_AGENT_SUM_PANEL_CONNECT_FOREGROUND );

    switch( comType )
    {
      case C_WIRED : lcd.println( "  WIRED  " ); break;
      case C_RF    : lcd.println( "    RF   " ); break;
      case C_WIFI  : lcd.println( "  WI-FI  " ); break;
      case C_BLE   : lcd.println( "   BLE   " ); break;
    }
  }
}

void lcdPlantAgentPanel( uint8_t agentNum )
{
  //lcd.drawRoundRect( 5, 25, 200, 160, 5, WHITE);

  // Draw title bar
  lcd.fillRect( 0, 25, 320, 15, C_AGENT_PANEL_TITLE_BACKGROUND );
  lcd.setCursor( 120, 25 + 2 );   lcd.setTextSize( 1 );
  lcd.setTextColor( C_AGENT_PANEL_TITLE_FOREGROUND );
  lcd.print( "PLANT AGENT 0" );
  lcd.println( agentNum );

  uint8_t tempI = 88;
  uint8_t humI = 88;
  uint8_t waterI = 47;

  // Draw temp value
  uint16_t x = 0;
  uint16_t y = 80;
  lcd.fillCircle( 55, 100, 50, getTempMeterColor( tempI ) );
  lcd.fillCircle( 55, 100, 42, BLACK );

  // display temp value
  displayDigit(x + 25, y, tempI / 10, C_AGENT_PANEL_METER_FOREGROUND );
  displayDigit(x + 58, y, tempI % 10, C_AGENT_PANEL_METER_FOREGROUND );

  lcd.fillRect( x + 15, y + 75, 80, 15, C_AGENT_PANEL_METER_TITLE_BACKGROUND );
  lcd.setCursor( x + 20, y + 80 );   lcd.setTextSize( 1 );
  lcd.setTextColor( C_AGENT_PANEL_METER_TITLE_FOREGROUND );
  lcd.print( "  TEMP (.C)" );

  // Draw Hum value
  x = 105;
  y = 80;
  lcd.fillCircle( 160, 100, 50, getHumMeterColor( humI ) );
  lcd.fillCircle( 160, 100, 42, BLACK );

  // display Hum value
  displayDigit(x + 25, y, humI / 10, C_AGENT_PANEL_METER_FOREGROUND);
  displayDigit(x + 58, y, humI % 10, C_AGENT_PANEL_METER_FOREGROUND);

  lcd.fillRect( x + 15, y + 75, 80, 15, C_AGENT_PANEL_METER_TITLE_BACKGROUND );
  lcd.setCursor( x + 20, y + 80 );   lcd.setTextSize( 1 );
  lcd.setTextColor( C_AGENT_PANEL_METER_TITLE_FOREGROUND );
  lcd.print( "REL. HUM (%)" );

  // Draw water value
  x = 210;
  y = 80;
  lcd.fillCircle( 265, 100, 50, getWaterMeterColor( waterI ) );
  lcd.fillCircle( 265, 100, 42, BLACK );

  // display water value
  displayDigit(x + 25, y, waterI / 10, C_AGENT_PANEL_METER_FOREGROUND );
  displayDigit(x + 58, y, waterI % 10, C_AGENT_PANEL_METER_FOREGROUND );

  lcd.fillRect( x + 15, y + 75, 83, 15, C_AGENT_PANEL_METER_TITLE_BACKGROUND );
  lcd.setCursor( x + 20, y + 80 );   lcd.setTextSize( 1 );
  lcd.setTextColor( C_AGENT_PANEL_METER_TITLE_FOREGROUND );
  lcd.print( "WATER LEV.(%)" );
}

uint16_t getTempMeterColor( uint8_t value )
{
  return C_AGENT_PANEL_METER_BACKGROUND_NORMAL;
}

uint16_t getHumMeterColor( uint8_t value )
{
  return C_AGENT_PANEL_METER_BACKGROUND_WARNING;
}

uint16_t getWaterMeterColor( uint8_t value )
{
  return C_AGENT_PANEL_METER_BACKGROUND_CRITICAL;
}

void displayDigit( uint16_t x, uint16_t y, uint8_t digit, uint16_t color )
{
  switch ( digit )
  {
    case 0 :
    {
      lcd.fillRoundRect(x + 6, y, 15, 6, 2, color);
      //lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2,color);
      lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2,color);

      lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  case 1 :
    {
      //lcd.fillRoundRect(x + 6, y, 15, 6, 2, color);
      //lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2,color);
      //lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2,color);

      //lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  case 2 :
    {
      lcd.fillRoundRect(x + 6, y, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      //lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      //lcd.fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  case 3 :
    {
      lcd.fillRoundRect(x + 6, y, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      //lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  case 4 :
    {
      //lcd.fillRoundRect(x + 6, y, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2,color);
      //lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2,color);

      lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  case 5 :
    {
      lcd.fillRoundRect(x + 6, y, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  case 6 :
    {
      lcd.fillRoundRect(x + 6, y, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  case 7 :
    {
      lcd.fillRoundRect(x + 6, y, 15, 6, 2, color);
      //lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      //lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      //lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  case 8 :
    {
      lcd.fillRoundRect(x + 6, y, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  case 9 :
    {
      lcd.fillRoundRect(x + 6, y, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      lcd.fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  }
}

void lcdAgentCFPPanel( uint16_t x, uint16_t y, uint8_t agentNum )
{
  // draw outer rect
  lcd.drawRect( x, y, 240, 50, C_CFP_PANEL_AGENT_BORDER );

  // draw title bar
  lcd.fillRect( x + 1, y + 1, 123, 12, C_CFP_PANEL_AGENT_BORDER );
  lcd.fillTriangle( x + 123, y + 1, x + 123, y + 12, x + 135, y + 1, BLUE );
  lcd.setCursor( x + 3, y + 3 );   lcd.setTextSize( 1 );
  lcd.setTextColor( C_CFP_PANEL_AGENT_TITLE_FOREGROUND );
  lcd.print( "PLANT AGENT 0" );
  lcd.println( agentNum );

  // draw accept / reject icon
  lcd.drawCircle( x + 20, y + 32, 14, WHITE );

  // Temp
  lcd.setCursor( x + 45, y + 16 );
  lcd.setTextColor( C_CFP_PANEL_AGENT_TEMP_FOREGROUND );
  lcd.println( "TEMP" );
  lcd.fillRect( x + 90, y + 14, 45, 10, C_CFP_PANEL_AGENT_TEMP_BACKGROUND );
  lcd.setCursor( x + 92, y + 16 ); lcd.setTextSize( 1 );
  lcd.setTextColor( C_CFP_PANEL_AGENT_TEMP_FOREGROUND );
  lcd.println( "29.2 C" );

  // Hum
  lcd.setCursor( x + 45, y + 28 );
  lcd.setTextColor( C_CFP_PANEL_AGENT_HUM_FOREGROUND );
  lcd.println( "R. HUM" );
  lcd.fillRect( x + 90, y + 26, 45, 10, C_CFP_PANEL_AGENT_HUM_BACKGROUND );
  lcd.setCursor( x + 92, y + 28 ); lcd.setTextSize( 1 );
  lcd.setTextColor( C_CFP_PANEL_AGENT_HUM_FOREGROUND );
  lcd.println( "100 %" );

  // Water Level
  lcd.setCursor( x + 45, y + 40 );
  lcd.setTextColor( C_CFP_PANEL_AGENT_WATER_FOREGROUND );
  lcd.println( "WATER" );
  lcd.fillRect( x + 90, y + 38, 45, 10, C_CFP_PANEL_AGENT_WATER_BACKGROUND );
  lcd.setCursor( x + 92, y + 40 ); lcd.setTextSize( 1 );
  lcd.setTextColor( C_CFP_PANEL_AGENT_WATER_FOREGROUND );
  lcd.println( "100 %" );

  // draw requested water units
  lcd.fillRect( x + 140, y + 5, 46, 40, C_CFP_PANEL_AGENT_WATER_REQUEST_BACKGROUND );

  lcd.fillRect( x + 141, y + 6, 44, 12, BLACK );
  lcd.setCursor( x + 142, y + 9 ); lcd.setTextSize( 1 );
  lcd.setTextColor( C_CFP_PANEL_AGENT_WATER_REQUEST_BACKGROUND );
  lcd.println( "REQUEST" );

  lcd.setCursor( x + 146, y + 20 ); lcd.setTextSize( 3 );
  lcd.setTextColor( C_CFP_PANEL_AGENT_WATER_REQUEST_FOREGROUND );
  lcd.println( "88" );

  // draw accepted water units
  x = x + 50;
  lcd.fillRect( x + 140, y + 5, 46, 40, C_CFP_PANEL_AGENT_WATER_ACCEPT_BACKGROUND );

  lcd.fillRect( x + 141, y + 6, 44, 12, BLACK );
  lcd.setCursor( x + 144, y + 9 ); lcd.setTextSize( 1 );
  lcd.setTextColor( C_CFP_PANEL_AGENT_WATER_ACCEPT_BACKGROUND );
  lcd.println( "ACCEPT" );

  lcd.setCursor( x + 146, y + 20 ); lcd.setTextSize( 3 );
  lcd.setTextColor( C_CFP_PANEL_AGENT_WATER_ACCEPT_FOREGROUND );
  lcd.println( "88" );
}

void showTotalWaterLevel( uint16_t x, uint16_t y )
{
  lcd.fillRect( x, y , 70, 50, C_CFP_PANEL_WATER_LEVEL_BACKGROUND );

  lcd.fillRect( x + 1, y + 1, 68, 12, BLACK );
  lcd.setCursor( x + 8, y + 3 ); lcd.setTextSize( 1 );
  lcd.setTextColor( C_CFP_PANEL_WATER_LEVEL_BACKGROUND );
  lcd.println( "AVAILABLE" );

  lcd.setCursor( x + 10, y + 20 ); lcd.setTextSize( 3 );
  lcd.setTextColor( C_CFP_PANEL_WATER_LEVEL_FOREGROUND );
  lcd.println( "888" );
}

void showStartButton(uint16_t x, uint16_t y)
{
  lcd.drawRoundRect( x, y , 70, 105, 10, C_CFP_PANEL_START_BUTTON_BORDER );
  lcd.drawCircle( x + 35, y + 35, 20, C_CFP_PANEL_START_BUTTON_BORDER ); // icon

  lcd.setTextSize( 1 ); lcd.setTextColor( C_CFP_PANEL_START_BUTTON_FOREGROUND );
  lcd.setCursor( x + 18, y + 70 );lcd.println( "START" );
  lcd.setCursor( x + 10, y + 82 );lcd.println( "WATERING" );
  lcd.setCursor( x + 13, y + 94 );lcd.println( "PROCESS" );
}

void onTouchEvent( int16_t x, int16_t y )
{
  if ( currentScreen == HOME_SCREEN )
  {
    if ( x > 210 && ( y > 25 && y < 75 ) )
    {
      onButtonPressed( BTN_PLAGENT_1 );
    }
    else if ( x > 210 && ( y > 80 && y < 130 ) )
    {
      onButtonPressed( BTN_PLAGENT_2 );
    }
    else if ( x > 210 && ( y > 135 && y < 185 ) )
    {
      onButtonPressed( BTN_PLAGENT_3 );
    }
  }

    if ( y < 238 && y > 192 )
    {
      if ( x < 64 && x > 18 )
      {
        onButtonPressed( BTN_HOME );
      }
      else if ( x < 124 && x > 78 )
      {
        onButtonPressed( BTN_NETWORK );
      }
      else if ( x < 187 && x > 141 )
      {
        onButtonPressed( BTN_CFP );
      }
      else if ( x < 247 && x > 201 )
      {
        onButtonPressed( BTN_ );
      }
      else if ( x < 307 && x > 261 )
      {
        onButtonPressed( BTN_LOG );
      }
  }
}

void onButtonPressed( uint8_t buttonID )
{
  if ( buttonID == BTN_HOME )
  {
    if ( currentScreen != HOME_SCREEN )
    {
      loadScreen( HOME_SCREEN );
    }
  }
  else if ( buttonID == BTN_NETWORK )
  {

  }
  else if ( buttonID == BTN_CFP )
  {
    loadScreen( CFP_SCREEN );
  }
  else if ( buttonID == BTN_ )
  {

  }
  else if ( buttonID == BTN_LOG )
  {

  }
  else if ( buttonID == BTN_PLAGENT_1 )
  {
    currentPlantAgent = 1;
    loadScreen( PLANT_AGENT_SCREEN );
  }
  else if ( buttonID == BTN_PLAGENT_2 )
  {
    currentPlantAgent = 2;
    loadScreen( PLANT_AGENT_SCREEN );
  }
  else if ( buttonID == BTN_PLAGENT_3 )
  {
    currentPlantAgent = 3;
    loadScreen( PLANT_AGENT_SCREEN );
  }
}

void loadScreen( uint8_t screenID )
{
  isDrawing = true;
  lcd.fillScreen( BLACK );
  delay( 100 );

  if ( screenID == HOME_SCREEN )
  {
    lcdHeaderPanel();

    lcdAgentInfoPanel( 210, 25, 1, true, C_WIRED );
    lcdAgentInfoPanel( 210, 80, 2, true, C_RF );
    lcdAgentInfoPanel( 210, 135, 3, false );

    lcdMainPanel();

    showActionPanel( screenID );
  }
  else if ( screenID == PLANT_AGENT_SCREEN )
  {
    lcdHeaderPanel();
    lcdPlantAgentPanel( currentPlantAgent );
    showActionPanel();
  }
  else if ( screenID == CFP_SCREEN )
  {
    lcdHeaderPanel();

    showStartButton( 5, 25 );
    showTotalWaterLevel( 5, 135 );

    lcdAgentCFPPanel( 80, 25, 1 );
    lcdAgentCFPPanel( 80, 80, 2 );
    lcdAgentCFPPanel( 80, 135, 3 );

    showActionPanel( screenID );
  }
  else if ( screenID == LOG_SCREEN )
  {

  }
  else if ( screenID == AGENT_NETWORK_SCREEN )
  {

  }

  currentScreen = screenID;
  isDrawing = false;
}
