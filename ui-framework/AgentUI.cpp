#include "Arduino.h"
#include "SPI.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_TFTLCD.h"
#include "TouchScreen.h"
#include "AgentUI.h"

int16_t tempPositionX = 0;
uint8_t currentScreen = HOME_SCREEN;
boolean isDrawing = false;

TouchScreen touch = TouchScreen( XP, YP, XM, YM, 300 );

AgentUI::AgentUI( ) : Adafruit_TFTLCD( LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET )
{

}

void AgentUI::init()
{
	reset();
	uint16_t identifier = readID();
	begin( identifier );
	setRotation( 3 );
}

void AgentUI::sync()
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
      currentPosition.x = map( currentPosition.y, TS_MINX, TS_MAXX, width(), 0 );
      currentPosition.y = map( tempPositionX, TS_MINY, TS_MAXY, 0, height() );

      onTouchEvent( currentPosition.x, currentPosition.y );
    }
  }
}

uint8_t AgentUI::getCurrentScreenID()
{
	return currentScreen;
}

void AgentUI::setHomeScreenData( HomeScreenData data )
{
	homeScreenData = data;
}

void AgentUI::setPlantAgentScreenData( uint8_t agentNum, PlantAgentScreenData data )
{
	plantAgentScreenDataList[agentNum - 1] = data;
}

void AgentUI::setCFPScreenData( CFPScreenData data )
{
	cfpScreenData = data;
}

// Generate Header Panel UI
void AgentUI::showHeaderPanel()
{
  // draw outer rect
  fillRect( 0, 0, 320, 20, C_HEADER_BACKGROUND);

  // lcd agent nanem
  setCursor( 2, 2 );   setTextSize( 1 );
  setTextColor( C_HEADER_FOREGROUND );
  print( "AGENT UI v1.0" );

  setCursor( 2, 11 );
  print( "DASHBOARD" );

  // lcd date and time
  //setCursor( 192, 2 );
  //print( "12-DEC-2016" );

  //setCursor( 210, 11 );
  //print( "12:00:00" );

  // communication icons
  drawFastVLine( 300, 0, 20, C_HEADER_SEPARATOR);

  drawFastVLine( 280, 0, 20, C_HEADER_SEPARATOR);

  drawFastVLine( 260, 0, 20, C_HEADER_SEPARATOR);
}

// Update Date Time Section
void AgentUI::updateTime( TimeInfo timeInfo )
{
	// Clear Text
	fillRect( 210, 11, 47, 7, C_HEADER_BACKGROUND);
	
	setCursor( 227, 11 ); setTextSize( 1 );
	setTextColor( C_HEADER_FOREGROUND );
	
	String timeString = "";
	if ( timeInfo.hour < 10 )
	{
		timeString.concat("0");
	}
	timeString.concat( timeInfo.hour );
	timeString.concat(":");
	if ( timeInfo.min < 10 )
	{
		timeString.concat("0");
	}
	timeString.concat( timeInfo.min );
	//timeString.concat(":");
	//if ( timeInfo.sec < 10 )
	//{
	//	timeString.concat("0");
	//}
	//timeString.concat( timeInfo.sec );
	println( timeString );
}

void AgentUI::updateDate( DateInfo dateInfo )
{
	setCursor( 192, 2 );
	String dateString = "2016-DEC-";
	dateString.concat( dateInfo.day );
	print( dateString );
}
	
// Generate Main Panel
void AgentUI::showMainPanel()
{
  // draw outer rect
  drawRoundRect( 5, 25, 200, 160, 5, C_AGENT_MAIN_PANEL_BORDER);

	// Draw water level indicator
	uint8_t levelHeight = ( waterLevelInd / 100.0 ) * 150;
	drawRoundRect( 10, 30, 5, 150, 1, C_CFP_PANEL_AGENT_HUM_FOREGROUND);
	fillRoundRect( 10, 30 + ( 150 - levelHeight ), 5, levelHeight, 1, C_CFP_PANEL_AGENT_HUM_FOREGROUND);

	uint8_t indX = 15;
	uint8_t indY = min(175 , 180 - levelHeight);
	fillTriangle( indX, indY, indX + 5, indY - 5, indX + 5, indY + 5, C_CFP_PANEL_WATER_LEVEL_BACKGROUND );

	// Draw water level text
	uint8_t yPos = min( 131, 180 - levelHeight - 25 );
	yPos = max( 26, yPos );
	showTotalWaterLevel( 20, yPos, waterLevelInd );

	// Draw Full Button
	fillRoundRect( 140, 35, 60, 40, 3, C_AGENT_MAIN_PANEL_BUTTON);
	setCursor( 155, 40 );   setTextSize( 1 );
	setTextColor( C_AGENT_MAIN_PANEL_BUTTON_FOREGROUND );
	print( "REFILL" );
	setCursor( 158, 50 ); print( "WATER" );
	setCursor( 162, 60 ); print( "TANK" );
  
	// Draw + Button
	fillRoundRect( 140, 85, 60, 40, 3, C_AGENT_MAIN_PANEL_BUTTON);
	fillRoundRect( 155, 100, 30, 10, 3, C_AGENT_MAIN_PANEL_BUTTON_FOREGROUND);
	fillRoundRect( 165, 90, 10, 30, 3, C_AGENT_MAIN_PANEL_BUTTON_FOREGROUND);
		
	// Draw - Button
	fillRoundRect( 140, 135, 60, 40, 3, C_AGENT_MAIN_PANEL_BUTTON);
	fillRoundRect( 155, 150, 30, 10, 3, C_AGENT_MAIN_PANEL_BUTTON_FOREGROUND);
}

// update water tank agent information in the home screen
void AgentUI::updateMainPanel( uint8_t waterLevel )
{
	// update values
	waterLevelInd = waterLevel;

	// clear the previous screen
	fillRect( 10, 26 , 80, 155, C_BLACK);

	// Draw water level indicator
	uint8_t levelHeight = ( waterLevelInd / 100.0 ) * 150;
	drawRoundRect( 10, 30, 5, 150, 1, C_CFP_PANEL_AGENT_HUM_FOREGROUND);
	fillRoundRect( 10, 30 + ( 150 - levelHeight ), 5, levelHeight, 1, C_CFP_PANEL_AGENT_HUM_FOREGROUND);

	uint8_t indX = 15;
	uint8_t indY = min(175 , 180 - levelHeight);
	fillTriangle( indX, indY, indX + 5, indY - 5, indX + 5, indY + 5, C_CFP_PANEL_WATER_LEVEL_BACKGROUND );

	// Draw water level text
	uint8_t yPos = min( 131, 180 - levelHeight - 25 );
	yPos = max( 26, yPos );
	showTotalWaterLevel( 20, yPos, waterLevelInd );
}

// Generate Icon / Buttons in Action Bar
// Action buttion will highligh based on the screen
void AgentUI::showActionPanel( uint8_t screenID )
{
  // draw outer rect
  //lcd.drawRect( 0, 190, 320, 50, WHITE);

  // draw action buttions
  // Home Button
  if ( screenID == HOME_SCREEN )
  {
    fillCircle( 41, 215, 23, C_ACTIONBAR_HOME_OUTER );
    fillCircle( 41, 215, 18, C_BLACK );
    drawCircle( 41, 215, 18, C_ACTIONBAR_HOME_OUTER );
	drawBitmap( 25, 196, iconHome, 32, 32, C_ACTIONBAR_ICON_COLOR );
  }
  else
  {
    drawCircle( 41, 215, 23, C_ACTIONBAR_HOME_OUTER );
    drawCircle( 41, 215, 18, C_ACTIONBAR_HOME_OUTER );
	drawBitmap( 25, 196, iconHome, 32, 32, C_ACTIONBAR_ICON_COLOR );
  }

  // Agent Network Button
  drawCircle( 101, 215, 23, C_ACTIONBAR_NETWORK_OUTER );
  drawCircle( 101, 215, 18, C_ACTIONBAR_NETWORK_OUTER );
	drawBitmap( 86, 200, iconNetwork, 32, 32, C_ACTIONBAR_ICON_COLOR );
	
  // CFP Button
  if ( screenID == CFP_SCREEN )
  {
    fillCircle( 164, 215, 23, C_ACTIONBAR_CFP_OUTER );
    fillCircle( 164, 215, 18, C_BLACK );
    drawCircle( 164, 215, 18, C_ACTIONBAR_CFP_OUTER );
	drawBitmap( 148, 198, iconCFP, 32, 32, C_ACTIONBAR_ICON_COLOR );
  }
  else
  {
    drawCircle( 164, 215, 23, C_ACTIONBAR_CFP_OUTER );
    drawCircle( 164, 215, 18, C_ACTIONBAR_CFP_OUTER );
	drawBitmap( 148, 198, iconCFP, 32, 32, C_ACTIONBAR_ICON_COLOR );
  }

  // Resource Button
  drawCircle( 224, 215, 23, C_ACTIONBAR_RESOURCE_OUTER );
  drawCircle( 224, 215, 18, C_ACTIONBAR_RESOURCE_OUTER );
	//drawBitmap( 106, 196, iconHome, 32, 32, C_ACTIONBAR_ICON_COLOR );

  // Log Button
  drawCircle( 284, 215, 23, C_ACTIONBAR_LOG_OUTER );
  drawCircle( 284, 215, 18, C_ACTIONBAR_LOG_OUTER );
  	drawBitmap( 268, 198, iconLog, 32, 32, C_ACTIONBAR_ICON_COLOR );
}

void AgentUI::showActionPanel( )
{
	showActionPanel( NO_SCREEN );
}

// Generate Agent Summary Panel in the Home screenID
// Only generate one Panel (call 3 times)
void AgentUI::showAgentInfoPanel( uint16_t x, uint16_t y, AgentConnectionInfo connection )
{
  // draw outer rect
  drawRect( x + 2, y, 106, 50, C_AGENT_SUM_PANEL_BORDER );

  // draw title bar
  fillRect( x, y, 110, 13, C_AGENT_SUM_PANEL_BORDER );
  setCursor( x + 2, y + 2 );   setTextSize( 1 );
  setTextColor( C_AGENT_SUM_PANEL_FOREGROUND );
  print( "PLANT AGENT 0" );
  println( connection.agentNum );

  // draw icon
  //drawCircle( x + 21, y + 30, 14, C_ICON_BORDER );
  drawBitmap( x + 21 - 14, y + 30 - 14, iconPlant, 32, 32, C_AGENT_SUM_PANEL_ICON_BACKGROUND );

  // draw connected string
  setCursor( x + 47, y + 36 );   setTextSize( 1 );
  setTextColor( C_AGENT_SUM_PANEL_CONNECT_FOREGROUND );

  if ( !connection.online )
  {
    fillRect( x + 45, y + 33, 58, 13, C_AGENT_SUM_PANEL_DISCONNECT_BACKGROUND );
    println( " OFFLINE " );
  }
  else
  {
    fillRect( x + 45, y + 33, 58, 13, C_AGENT_SUM_PANEL_CONNECT_BACKGROUND );
    println( "  ONLINE " );

    // draw comm channel string
    fillRect( x + 45, y + 16, 58, 13, C_AGENT_SUM_PANEL_COMTYPE_BACKGROUND );
    setCursor( x + 47, y + 19 );   setTextSize( 1 );
    setTextColor( C_AGENT_SUM_PANEL_CONNECT_FOREGROUND );

    switch( connection.comType )
    {
      case C_WIRED : println( "  WIRED  " ); break;
      case C_RF    : println( "    RF   " ); break;
      case C_WIFI  : println( "  WI-FI  " ); break;
      case C_BLE   : println( "   BLE   " ); break;
    }
  }
}

// Update Agent Summary Panel in the Home Screen
void AgentUI::updateAgentInfoPanel( uint16_t x, uint16_t y, AgentConnectionInfo connection )
{
  // draw connected string
  setCursor( x + 47, y + 36 );   setTextSize( 1 );
  setTextColor( C_AGENT_SUM_PANEL_CONNECT_FOREGROUND );

  if ( !connection.online )
  {
    fillRect( x + 45, y + 33, 58, 13, C_AGENT_SUM_PANEL_DISCONNECT_BACKGROUND );
    println( " OFFLINE " );

	fillRect( x + 45, y + 16, 58, 13, C_BLACK );
  }
  else
  {
    fillRect( x + 45, y + 33, 58, 13, C_AGENT_SUM_PANEL_CONNECT_BACKGROUND );
    println( "  ONLINE " );

	// draw comm channel string
    fillRect( x + 45, y + 16, 58, 13, C_AGENT_SUM_PANEL_COMTYPE_BACKGROUND );
    setCursor( x + 47, y + 19 );   setTextSize( 1 );
    setTextColor( C_AGENT_SUM_PANEL_CONNECT_FOREGROUND );

    switch( connection.comType )
    {
      case C_WIRED : println( "  WIRED  " ); break;
      case C_RF    : println( "    RF   " ); break;
      case C_WIFI  : println( "  WI-FI  " ); break;
      case C_BLE   : println( "   BLE   " ); break;
    }
  }
}

// Generate new Screen with Plant Agent Information
// Three Meters are used to display the temperature, humidity and Water Level
void AgentUI::showPlantAgentScreen( PlantAgentInfo data )
{
  //lcd.drawRoundRect( 5, 25, 200, 160, 5, WHITE);

  // Draw title bar
  fillRect( 0, 25, 320, 15, C_AGENT_PANEL_TITLE_BACKGROUND );
  setCursor( 120, 25 + 2 );   setTextSize( 1 );
  setTextColor( C_AGENT_PANEL_TITLE_FOREGROUND );
  print( "PLANT AGENT 0" );
  println( data.agentNum );

  // Draw temp value
  uint16_t x = 0;
  uint16_t y = 80;
	uint16_t color = getTempMeterColor( data.temperature );
  fillCircle( 55, 100, 50, color );
  fillCircle( 55, 100, 42, C_BLACK );

  // display temp value
  displayDigit(x + 25, y, data.temperature / 10, C_AGENT_PANEL_METER_FOREGROUND );
  displayDigit(x + 58, y, data.temperature % 10, C_AGENT_PANEL_METER_FOREGROUND );

  fillRect( x + 15, y + 75, 80, 15, C_AGENT_PANEL_METER_TITLE_BACKGROUND );
  setCursor( x + 20, y + 80 );   setTextSize( 1 );
  setTextColor( C_AGENT_PANEL_METER_TITLE_FOREGROUND );
  print( "  TEMP (.C)" );

  // Draw Hum value
  x = 105;
  y = 80;
	color = getHumMeterColor( data.humidity );
  fillCircle( 160, 100, 50, color );
  fillCircle( 160, 100, 42, C_BLACK );

  // display Hum value
  displayDigit(x + 25, y, data.humidity / 10, C_AGENT_PANEL_METER_FOREGROUND);
  displayDigit(x + 58, y, data.humidity % 10, C_AGENT_PANEL_METER_FOREGROUND);

  fillRect( x + 15, y + 75, 80, 15, C_AGENT_PANEL_METER_TITLE_BACKGROUND );
  setCursor( x + 20, y + 80 );   setTextSize( 1 );
  setTextColor( C_AGENT_PANEL_METER_TITLE_FOREGROUND );
  print( "REL. HUM (%)" );

  // Draw water value
  x = 210;
  y = 80;
	color = getWaterMeterColor( data.waterLevel );
  fillCircle( 265, 100, 50, color );
  fillCircle( 265, 100, 42, C_BLACK );

  // display water value
  displayDigit(x + 25, y, data.waterLevel / 10, C_AGENT_PANEL_METER_FOREGROUND );
  displayDigit(x + 58, y, data.waterLevel % 10, C_AGENT_PANEL_METER_FOREGROUND );

  fillRect( x + 15, y + 75, 83, 15, C_AGENT_PANEL_METER_TITLE_BACKGROUND );
  setCursor( x + 20, y + 80 );   setTextSize( 1 );
  setTextColor( C_AGENT_PANEL_METER_TITLE_FOREGROUND );
  print( "WATER LEV.(%)" );
}

// Update plat agent information in the screen
void AgentUI::updatePlantAgentScreen( PlantAgentInfo data )
{
  // Draw title bar
  // fillRect( 0, 25, 320, 15, C_AGENT_PANEL_TITLE_BACKGROUND );
  // setCursor( 120, 25 + 2 );   setTextSize( 1 );
  // setTextColor( C_AGENT_PANEL_TITLE_FOREGROUND );
  // print( "PLANT AGENT 0" );
  // println( data.agentNum );

  // Draw temp value
  uint16_t x = 0;
  uint16_t y = 80;
  // fillCircle( 55, 100, 50, getTempMeterColor( data.temperature ) );
  // fillCircle( 55, 100, 42, C_BLACK );

  // display temp value
  displayDigit(x + 25, y, data.temperature / 10, C_AGENT_PANEL_METER_FOREGROUND );
  displayDigit(x + 58, y, data.temperature % 10, C_AGENT_PANEL_METER_FOREGROUND );

  // fillRect( x + 15, y + 75, 80, 15, C_AGENT_PANEL_METER_TITLE_BACKGROUND );
  // setCursor( x + 20, y + 80 );   setTextSize( 1 );
  // setTextColor( C_AGENT_PANEL_METER_TITLE_FOREGROUND );
  // print( "  TEMP (.C)" );

  // Draw Hum value
  x = 105;
  y = 80;
  // fillCircle( 160, 100, 50, getHumMeterColor( data.humidity ) );
  // fillCircle( 160, 100, 42, C_BLACK );

  // display Hum value
  displayDigit(x + 25, y, data.humidity / 10, C_AGENT_PANEL_METER_FOREGROUND);
  displayDigit(x + 58, y, data.humidity % 10, C_AGENT_PANEL_METER_FOREGROUND);

  // fillRect( x + 15, y + 75, 80, 15, C_AGENT_PANEL_METER_TITLE_BACKGROUND );
  // setCursor( x + 20, y + 80 );   setTextSize( 1 );
  // setTextColor( C_AGENT_PANEL_METER_TITLE_FOREGROUND );
  // print( "REL. HUM (%)" );

  // Draw water value
  x = 210;
  y = 80;
  // fillCircle( 265, 100, 50, getWaterMeterColor( data.waterLevel ) );
  // fillCircle( 265, 100, 42, C_BLACK );

  // display water value
  displayDigit(x + 25, y, data.waterLevel / 10, C_AGENT_PANEL_METER_FOREGROUND );
  displayDigit(x + 58, y, data.waterLevel % 10, C_AGENT_PANEL_METER_FOREGROUND );

  // fillRect( x + 15, y + 75, 83, 15, C_AGENT_PANEL_METER_TITLE_BACKGROUND );
  // setCursor( x + 20, y + 80 );   setTextSize( 1 );
  // setTextColor( C_AGENT_PANEL_METER_TITLE_FOREGROUND );
  // print( "WATER LEV.(%)" );
}

uint16_t AgentUI::getTempMeterColor( uint8_t value )
{
  return C_AGENT_PANEL_METER_BACKGROUND_NORMAL;
}

uint16_t AgentUI::getHumMeterColor( uint8_t value )
{
  return C_AGENT_PANEL_METER_BACKGROUND_WARNING;
}

uint16_t AgentUI::getWaterMeterColor( uint8_t value )
{
  return C_AGENT_PANEL_METER_BACKGROUND_CRITICAL;
}

void AgentUI::displayDigit( uint16_t x, uint16_t y, uint8_t digit, uint16_t color )
{
	// clear previous digit
	fillRoundRect(x + 6, y, 15, 6, 2, C_BLACK);
	fillRoundRect(x + 6, y + 21, 15, 6, 2, C_BLACK);
	fillRoundRect(x + 6, y + 42, 15, 6, 2, C_BLACK);

	fillRoundRect(x, y + 6, 6, 15, 2, C_BLACK);
	fillRoundRect(x + 21, y + 6, 6, 15, 2, C_BLACK);
	fillRoundRect(x, y + 27, 6, 15, 2, C_BLACK);
	fillRoundRect(x + 21, y + 27, 6, 15, 2, C_BLACK);

  switch ( digit )
  {
    case 0 :
    {
      fillRoundRect(x + 6, y, 15, 6, 2, color);
      //lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2,color);
      fillRoundRect(x + 6, y + 42, 15, 6, 2,color);

      fillRoundRect(x, y + 6, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      fillRoundRect(x, y + 27, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  	case 1 :
    {
      //lcd.fillRoundRect(x + 6, y, 15, 6, 2, color);
      //lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2,color);
      //lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2,color);

      //lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  	case 2 :
    {
      fillRoundRect(x + 6, y, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      //lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      fillRoundRect(x, y + 27, 6, 15, 2, color);
      //lcd.fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  	case 3 :
    {
      fillRoundRect(x + 6, y, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      //lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  	case 4 :
    {
      //lcd.fillRoundRect(x + 6, y, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 21, 15, 6, 2,color);
      //lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2,color);

      fillRoundRect(x, y + 6, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  	case 5 :
    {
      fillRoundRect(x + 6, y, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      fillRoundRect(x, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  	case 6 :
    {
      fillRoundRect(x + 6, y, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      fillRoundRect(x, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      fillRoundRect(x, y + 27, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  	case 7 :
    {
      fillRoundRect(x + 6, y, 15, 6, 2, color);
      //lcd.fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      //lcd.fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      //lcd.fillRoundRect(x, y + 6, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  	case 8 :
    {
      fillRoundRect(x + 6, y, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      fillRoundRect(x, y + 6, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      fillRoundRect(x, y + 27, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  	case 9 :
    {
      fillRoundRect(x + 6, y, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 21, 15, 6, 2, color);
      fillRoundRect(x + 6, y + 42, 15, 6, 2, color);

      fillRoundRect(x, y + 6, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 6, 6, 15, 2, color);
      //lcd.fillRoundRect(x, y + 27, 6, 15, 2, color);
      fillRoundRect(x + 21, y + 27, 6, 15, 2, color);
    } break;
  }
}

// Generate CFP panel for single plant agent, need to call separatelly for each agent
void AgentUI::showAgentCFPPanel( uint16_t x, uint16_t y, PlantAgentInfo agentData, WaterInfo waterData )
{
  // draw outer rect
  drawRect( x, y, 240, 50, C_CFP_PANEL_AGENT_BORDER );

  // draw title bar
  fillRect( x + 1, y + 1, 123, 12, C_CFP_PANEL_AGENT_BORDER );
  fillTriangle( x + 123, y + 1, x + 123, y + 12, x + 135, y + 1, C_CFP_PANEL_AGENT_BORDER );
  setCursor( x + 3, y + 3 );   setTextSize( 1 );
  setTextColor( C_CFP_PANEL_AGENT_TITLE_FOREGROUND );
  print( "PLANT AGENT 0" );
  println( agentData.agentNum );

  // draw accept / reject icon
  drawBitmap( x + 20 - 14, y + 32 - 14, iconInfo, 32, 32, C_CFP_PANEL_AGENT_INFO_ICON );
  drawCircle( x + 20, y + 32, 14, C_ICON_BORDER );
  
  //drawBitmap( x + 20 - 14, y + 32 - 14, iconAccept, 32, 32, C_AGENT_SUM_PANEL_ICON_BACKGROUND );
  //drawBitmap( x + 20 - 14, y + 32 - 14, iconReject, 32, 32, C_AGENT_SUM_PANEL_ICON_BACKGROUND );
  //drawBitmap( x + 20 - 14, y + 32 - 14, iconInfo, 32, 32, C_AGENT_SUM_PANEL_ICON_BACKGROUND );
  //drawBitmap( x + 20 - 14, y + 32 - 14, iconRetry, 32, 32, C_AGENT_SUM_PANEL_ICON_BACKGROUND );
  
  // Temp
  setCursor( x + 45, y + 16 );
  setTextColor( C_CFP_PANEL_AGENT_TEMP_FOREGROUND );
  println( "TEMP" );
  fillRect( x + 90, y + 14, 45, 10, C_CFP_PANEL_AGENT_TEMP_BACKGROUND );
  setCursor( x + 92, y + 16 ); setTextSize( 1 );
  setTextColor( C_CFP_PANEL_AGENT_TEMP_FOREGROUND );

  String tempString = "  ";
  tempString.concat( agentData.temperature );
  tempString.concat(" C ");
  println( tempString );

  // Hum
  setCursor( x + 45, y + 28 );
  setTextColor( C_CFP_PANEL_AGENT_HUM_FOREGROUND );
  println( "R. HUM" );
  fillRect( x + 90, y + 26, 45, 10, C_CFP_PANEL_AGENT_HUM_BACKGROUND );
  setCursor( x + 92, y + 28 ); setTextSize( 1 );
  setTextColor( C_CFP_PANEL_AGENT_HUM_FOREGROUND );

  String humString = "  ";
  humString.concat( agentData.humidity );
  humString.concat(" % ");
  println( humString );

  // Water Level
  setCursor( x + 45, y + 40 );
  setTextColor( C_CFP_PANEL_AGENT_WATER_FOREGROUND );
  println( "WATER" );
  fillRect( x + 90, y + 38, 45, 10, C_CFP_PANEL_AGENT_WATER_BACKGROUND );
  setCursor( x + 92, y + 40 ); setTextSize( 1 );
  setTextColor( C_CFP_PANEL_AGENT_WATER_FOREGROUND );

  String waterLevelString = "  ";
  waterLevelString.concat( agentData.waterLevel );
  waterLevelString.concat(" % ");
  println( waterLevelString );

  // draw requested water units
  fillRect( x + 140, y + 5, 46, 40, C_CFP_PANEL_AGENT_WATER_REQUEST_BACKGROUND );

  fillRect( x + 141, y + 6, 44, 12, C_BLACK );
  setCursor( x + 142, y + 9 ); setTextSize( 1 );
  setTextColor( C_CFP_PANEL_AGENT_WATER_REQUEST_BACKGROUND );
  println( "REQUEST" );

  setCursor( x + 146, y + 20 ); setTextSize( 3 );
  setTextColor( C_CFP_PANEL_AGENT_WATER_REQUEST_FOREGROUND );
  println( waterData.requestUnits );

  // draw accepted water units
  x = x + 50;
  fillRect( x + 140, y + 5, 46, 40, C_CFP_PANEL_AGENT_WATER_ACCEPT_BACKGROUND );

  fillRect( x + 141, y + 6, 44, 12, C_BLACK );
  setCursor( x + 144, y + 9 ); setTextSize( 1 );
  setTextColor( C_CFP_PANEL_AGENT_WATER_ACCEPT_BACKGROUND );
  println( "ACCEPT" );

  setCursor( x + 146, y + 20 ); setTextSize( 3 );
  setTextColor( C_CFP_PANEL_AGENT_WATER_ACCEPT_FOREGROUND );
  println( waterData.acceptUnits );
}

// Update agent status in CFP panel
void AgentUI::updateAgentCFPPanel( uint16_t x, uint16_t y, uint8_t status )
{
  // draw accept / reject icon
	fillCircle( x + 20, y + 32, 14, C_BLACK );
	switch ( status )
	{
		case CFP_STATUS_NORMAL : drawBitmap( x + 20 - 14, y + 32 - 14, iconInfo, 32, 32, C_CFP_PANEL_AGENT_INFO_ICON ); break;
		case CFP_STATUS_ACCEPT : drawBitmap( x + 20 - 14, y + 32 - 14, iconAccept, 32, 32, C_CFP_PANEL_AGENT_ACCEPT_ICON ); break;
		case CFP_STATUS_REJECT : drawBitmap( x + 20 - 14, y + 32 - 14, iconReject, 32, 32, C_CFP_PANEL_AGENT_REJECT_ICON ); break;
		case CFP_STATUS_RETRY : drawBitmap( x + 20 - 14, y + 32 - 14, iconRetry, 32, 32, C_CFP_PANEL_AGENT_RETRY_ICON ); break;
	}
	drawCircle( x + 20, y + 32, 14, C_ICON_BORDER );
}

// Update agent base information in the CFP Panel
void AgentUI::updateAgentCFPPanel( uint16_t x, uint16_t y, PlantAgentInfo agentData, WaterInfo waterData )
{
  // draw outer rect
  //drawRect( x, y, 240, 50, C_CFP_PANEL_AGENT_BORDER );

  // draw title bar
  // fillRect( x + 1, y + 1, 123, 12, C_CFP_PANEL_AGENT_BORDER );
  // fillTriangle( x + 123, y + 1, x + 123, y + 12, x + 135, y + 1, C_CFP_PANEL_AGENT_BORDER );
  // setCursor( x + 3, y + 3 );   setTextSize( 1 );
  // setTextColor( C_CFP_PANEL_AGENT_TITLE_FOREGROUND );
  // print( "PLANT AGENT 0" );
  // println( agentData.agentNum );

  // draw accept / reject icon
  
  /* Icon will not be update by this method
  	fillCircle( x + 20, y + 32, 14, C_BLACK );
	switch ( status )
	{
		case CFP_STATUS_NORMAL : drawBitmap( x + 20 - 14, y + 32 - 14, iconInfo, 32, 32, C_CFP_PANEL_AGENT_INFO_ICON ); break;
		case CFP_STATUS_ACCEPT : drawBitmap( x + 20 - 14, y + 32 - 14, iconAccept, 32, 32, C_CFP_PANEL_AGENT_ACCEPT_ICON ); break;
		case CFP_STATUS_REJECT : drawBitmap( x + 20 - 14, y + 32 - 14, iconReject, 32, 32, C_CFP_PANEL_AGENT_REJECT_ICON ); break;
		case CFP_STATUS_RETRY : drawBitmap( x + 20 - 14, y + 32 - 14, iconRetry, 32, 32, C_CFP_PANEL_AGENT_RETRY_ICON ); break;
	}
	drawCircle( x + 20, y + 32, 14, C_ICON_BORDER ); */

  // Temp
  // setCursor( x + 45, y + 16 );
  // setTextColor( C_CFP_PANEL_AGENT_TEMP_FOREGROUND );
  // println( "TEMP" );
  fillRect( x + 90, y + 14, 45, 10, C_CFP_PANEL_AGENT_TEMP_BACKGROUND );
  setCursor( x + 92, y + 16 ); setTextSize( 1 );
  setTextColor( C_CFP_PANEL_AGENT_TEMP_FOREGROUND );

  String tempString = "  ";
  tempString.concat( agentData.temperature );
  //tempString.concat( "." );
  //tempString.concat( agentData.temperature );
  tempString.concat(" C ");
  println( tempString );

  // Hum
  // setCursor( x + 45, y + 28 );
  // setTextColor( C_CFP_PANEL_AGENT_HUM_FOREGROUND );
  // println( "R. HUM" );
  fillRect( x + 90, y + 26, 45, 10, C_CFP_PANEL_AGENT_HUM_BACKGROUND );
  setCursor( x + 92, y + 28 ); setTextSize( 1 );
  setTextColor( C_CFP_PANEL_AGENT_HUM_FOREGROUND );

  String humString = "  ";
  humString.concat( agentData.humidity );
  //humString.concat( "." );
  //humString.concat( agentData.humidity );
  humString.concat(" % ");
  println( humString );

  // Water Level
  // setCursor( x + 45, y + 40 );
  // setTextColor( C_CFP_PANEL_AGENT_WATER_FOREGROUND );
  // println( "WATER" );
  fillRect( x + 90, y + 38, 45, 10, C_CFP_PANEL_AGENT_WATER_BACKGROUND );
  setCursor( x + 92, y + 40 ); setTextSize( 1 );
  setTextColor( C_CFP_PANEL_AGENT_WATER_FOREGROUND );

  String waterLevelString = "  ";
  waterLevelString.concat( agentData.waterLevel );
  waterLevelString.concat(" % ");
  println( waterLevelString );

  // draw requested water units
  //fillRect( x + 140, y + 5, 46, 40, C_CFP_PANEL_AGENT_WATER_REQUEST_BACKGROUND );

  //fillRect( x + 141, y + 6, 44, 12, C_BLACK );
  //setCursor( x + 142, y + 9 ); setTextSize( 1 );
  //setTextColor( C_CFP_PANEL_AGENT_WATER_REQUEST_BACKGROUND );
  //println( "REQUEST" );

	fillRect( x + 140, y + 19, 46, 22, C_CFP_PANEL_AGENT_WATER_REQUEST_BACKGROUND );
  setCursor( x + 146, y + 20 ); setTextSize( 3 );
  setTextColor( C_CFP_PANEL_AGENT_WATER_REQUEST_FOREGROUND );
  println( waterData.requestUnits );

  // draw accepted water units
  x = x + 50;
  //fillRect( x + 140, y + 5, 46, 40, C_CFP_PANEL_AGENT_WATER_ACCEPT_BACKGROUND );

  //fillRect( x + 141, y + 6, 44, 12, C_BLACK );
  //setCursor( x + 144, y + 9 ); setTextSize( 1 );
  //setTextColor( C_CFP_PANEL_AGENT_WATER_ACCEPT_BACKGROUND );
  //println( "ACCEPT" );

	fillRect( x + 140, y + 19, 46, 22, C_CFP_PANEL_AGENT_WATER_ACCEPT_BACKGROUND );
  setCursor( x + 146, y + 20 ); setTextSize( 3 );
  setTextColor( C_CFP_PANEL_AGENT_WATER_ACCEPT_FOREGROUND );
  println( waterData.acceptUnits );
}


// Display total Water availability in the CFP Screen
void AgentUI::showTotalWaterLevel( uint16_t x, uint16_t y, uint8_t totalWaterLevel )
{
  fillRect( x, y , 70, 50, C_CFP_PANEL_WATER_LEVEL_BACKGROUND );

  fillRect( x + 1, y + 1, 68, 12, C_BLACK );
  setCursor( x + 8, y + 3 ); setTextSize( 1 );
  setTextColor( C_CFP_PANEL_WATER_LEVEL_BACKGROUND );
  println( "AVAILABLE" );

	if ( totalWaterLevel > 99 )
	{
		  setCursor( x + 10, y + 20 );
	}
	else
	{
		  setCursor( x + 20, y + 20 );
	}
  setTextColor( C_CFP_PANEL_WATER_LEVEL_FOREGROUND ); setTextSize( 3 );
  println( totalWaterLevel );
}

// update total Water availability in the CFP Screen
void AgentUI::updateTotalWaterLevel( uint16_t x, uint16_t y, uint8_t totalWaterLevel )
{
  // fillRect( x, y , 70, 50, C_CFP_PANEL_WATER_LEVEL_BACKGROUND );

  // fillRect( x + 1, y + 1, 68, 12, C_BLACK );
  // setCursor( x + 8, y + 3 ); setTextSize( 1 );
  // setTextColor( C_CFP_PANEL_WATER_LEVEL_BACKGROUND );
  // println( "AVAILABLE" );

	if ( totalWaterLevel > 99 )
	{
			setCursor( x + 10, y + 20 );
	}
	else
	{
			setCursor( x + 20, y + 20 );
	}
	fillRect( x, y + 20, 70, 30, C_CFP_PANEL_WATER_LEVEL_BACKGROUND );
  setTextColor( C_CFP_PANEL_WATER_LEVEL_FOREGROUND ); setTextSize( 3 );
  println( totalWaterLevel );
}

// Display Start button in the CFP Screen
void AgentUI::showStartButton(uint16_t x, uint16_t y)
{
  drawRoundRect( x, y , 70, 105, 10, C_CFP_PANEL_START_BUTTON_BORDER );
  drawCircle( x + 35, y + 35, 20, C_CFP_PANEL_START_BUTTON_BORDER ); // icon
	drawBitmap( x + 35 - 16, y + 35 - 16, iconWatering, 32, 32, C_CFP_PANEL_START_BUTTON_BORDER );
	

  setTextSize( 1 ); setTextColor( C_CFP_PANEL_START_BUTTON_FOREGROUND );
  setCursor( x + 18, y + 70 );println( "START" );
  setCursor( x + 10, y + 82 );println( "WATERING" );
  setCursor( x + 13, y + 94 );println( "PROCESS" );
}

// Display Log UI
void AgentUI::showLogPanel( uint16_t x, uint16_t y )
{
	// clear log area
	fillRect( x, y, 310, 160, C_BLACK );

	// draw border
	drawRoundRect( x, y, 310, 160, 5, C_LOG_PANEL_BORDER );

	// draw text
	y = y + 4;
	setTextSize( 1 ); setTextColor( C_LOG_PANEL_TEXT_FORGROUND );
	for ( int i = 0; i < 15; i ++ )
	{
		setCursor( x + 5, y + ( i * 10) );println( logBuffer[i] );
	}
}

void AgentUI::updateLogPanel( uint16_t x, uint16_t y, String logLine )
{
	// draw text
	y = y + 4;
	setTextSize( 1 ); setTextColor( C_LOG_PANEL_TEXT_FORGROUND );
	setCursor( x + 5, y + ( (logLineIndex - 1) * 10) );println( logLine );
}

/*
*
*/
void AgentUI::showHomeScreen( HomeScreenData data )
{
	isDrawing = true;
	fillScreen( C_BLACK );

	showHeaderPanel();
	showActionPanel( HOME_SCREEN );
	showMainPanel();

	showAgentInfoPanel( 210, 25, data.connectionList[0] );
	showAgentInfoPanel( 210, 80, data.connectionList[1] );
	showAgentInfoPanel( 210, 135, data.connectionList[2] );

	homeScreenData = data;
	currentScreen = HOME_SCREEN;
	isDrawing = false;
}

/*
*
*/
void AgentUI::showPlantAgentScreen( PlantAgentScreenData data )
{
	isDrawing = true;
	fillScreen( C_BLACK );

	showHeaderPanel();
	showPlantAgentScreen( data.plantAgentInfo );
	showActionPanel();

	plantAgentScreenDataList[data.plantAgentInfo.agentNum - 1] = data;
	currentScreen = PLANT_AGENT_SCREEN;
	isDrawing = false;
}

/*
*
*/
void AgentUI::showCFPScreen( CFPScreenData data )
{
	isDrawing = true;
	fillScreen( C_BLACK );

	showHeaderPanel();
	showActionPanel( CFP_SCREEN );

	showStartButton( 5, 25 );
	showTotalWaterLevel( 5, 135, data.totalWaterLevel );

	showAgentCFPPanel( 80, 25, data.plantAgentInfoList[0], data.waterInfoList[0] );
	showAgentCFPPanel( 80, 80, data.plantAgentInfoList[1], data.waterInfoList[1] );
	showAgentCFPPanel( 80, 135, data.plantAgentInfoList[2], data.waterInfoList[2] );

	cfpScreenData = data;
	currentScreen = CFP_SCREEN;
	isDrawing = false;
}

void AgentUI::showLogScreen()
{
	isDrawing = true;
	fillScreen( C_BLACK );

	showHeaderPanel();
	showActionPanel( LOG_SCREEN );

	showLogPanel( 5, 25 );

	currentScreen = LOG_SCREEN;
	isDrawing = false;
}

/* ************************************************************************
 * *************************** Update methods *****************************
 * ************************************************************************/
void AgentUI::updateWaterTankInformation( WaterTankInfo waterTankInfo )
{
	// update water level global variable
	waterLevelInd = waterTankInfo.waterLevel;

	if ( currentScreen == HOME_SCREEN )
	{
		updateMainPanel( waterTankInfo.waterLevel );
	}
}

void AgentUI::updateAgentConnectionStatus( AgentConnectionInfo info )
{
	if ( currentScreen == HOME_SCREEN )
	{
		switch ( info.agentNum )
		{
			case 1 : updateAgentInfoPanel( 210, 25, info ); break;
			case 2 : updateAgentInfoPanel( 210, 80, info ); break;
			case 3 : updateAgentInfoPanel( 210, 135, info ); break;
		}
	}
}

void AgentUI::updatePlantAgentInformation( PlantAgentInfo info )
{
	if ( currentScreen == PLANT_AGENT_SCREEN )
	{
		updatePlantAgentScreen( info );
	}
}

// Public method to update the water level UI
void AgentUI::updateTotalWaterLevel( uint8_t totalWaterLevel )
{
	if ( currentScreen == CFP_SCREEN )
	{
		updateTotalWaterLevel( 5, 135, totalWaterLevel );
	}
}

// puplic method to update the CFP agent values
void AgentUI::updateAgentCFPPanel( PlantAgentInfo agentData, WaterInfo waterData )
{
	if ( currentScreen == CFP_SCREEN )
	{
		switch ( agentData.agentNum )
		{
			case 1 : updateAgentCFPPanel( 80, 25, agentData, waterData ); break;
			case 2 : updateAgentCFPPanel( 80, 80, agentData, waterData ); break;
			case 3 : updateAgentCFPPanel( 80, 135, agentData, waterData ); break;
		}
	}
}

// puplic method to update the CFP agent status
void AgentUI::updateAgentStatusCFPPanel( uint8_t agentNum, uint8_t status )
{
	if ( currentScreen == CFP_SCREEN )
	{
		switch ( agentNum )
		{
			case 1 : updateAgentCFPPanel( 80, 25, status ); break;
			case 2 : updateAgentCFPPanel( 80, 80, status ); break;
			case 3 : updateAgentCFPPanel( 80, 135, status ); break;
		}
	}
}

// Public method to add log line to the Buffer
void AgentUI::addLogLine( String logLine )
{
	if ( logLineIndex == 15 )
	{
		logLineIndex = 0;
		for( int i = 0; i < 15; i++ )
		{
			logBuffer[i] = "";
		}

		if ( currentScreen == LOG_SCREEN )
		{
			// clear log area
			fillRect( 5, 25, 310, 160, C_BLACK );
			drawRoundRect( 5, 25, 310, 160, 5, C_LOG_PANEL_BORDER );
		}
	}

	// add log line to the Buffer
	logBuffer[logLineIndex] = logLine;
	logLineIndex++;

	// update the Screen if needed
	if ( currentScreen == LOG_SCREEN )
	{
		updateLogPanel( 5, 25, logLine );
	}
}

void AgentUI::onTouchEvent( int16_t x, int16_t y )
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
	else if (( x > 140 && x < 200 ) && ( y > 35 && y < 75 ))
	{
		onButtonPressed( BTN_WATER_FILL );
	}
	else if (( x > 140 && x < 200 ) && ( y > 85 && y < 125 ))
	{
		onButtonPressed( BTN_WATER_INC );
	}
	else if (( x > 140 && x < 200 ) && ( y > 135 && y < 175 ))
	{
		onButtonPressed( BTN_WATER_DEC );
	}	
  }
  else if ( currentScreen == CFP_SCREEN )
  {
	if (( x > 5 && x < 110 ) && ( y > 25 && y < 95 ) )
    {
      onButtonPressed( BTN_START_PROCESS );
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
        onButtonPressed( BTN_CFP_BACK );
      }
      else if ( x < 307 && x > 261 )
      {
        onButtonPressed( BTN_LOG );
      }
  }
}

void AgentUI::addButtonPressedEvent( ButtonPressedEvent event )
{
	_buttonPressedEvent = event;
}

void AgentUI::onButtonPressed( uint8_t buttonID )
{
	// Call back routing for the main program
	_buttonPressedEvent( buttonID );
	
  if ( buttonID == BTN_HOME )
  {
    if ( currentScreen != HOME_SCREEN )
    {
      showHomeScreen(  homeScreenData );
    }
  }
  else if ( buttonID == BTN_WATER_FILL )
  {
	WaterTankInfo waterTankInfo = { 100 };
	updateWaterTankInformation( waterTankInfo );
  }
  else if ( buttonID == BTN_WATER_INC )
  {
	uint8_t level = waterLevelInd + waterLevelStep;
	if ( level > 100 )
	{
		level = 100;
	}
	WaterTankInfo waterTankInfo = { level };
	updateWaterTankInformation( waterTankInfo );
  }
  else if ( buttonID == BTN_WATER_DEC )
  {
	uint8_t level = 0;
	if ( waterLevelInd > 10 )
	{
		level = waterLevelInd - waterLevelStep;
	}
	WaterTankInfo waterTankInfo = { level };
	updateWaterTankInformation( waterTankInfo );
  }
  else if ( buttonID == BTN_NETWORK )
  {

  }
  else if ( buttonID == BTN_CFP )
  {
	cfpScreenData.totalWaterLevel = waterLevelInd;
    showCFPScreen( cfpScreenData );
  }
  else if ( buttonID == BTN_CFP_BACK )
  {

  }
  else if ( buttonID == BTN_LOG )
  {
	showLogScreen();
  }
  else if ( buttonID == BTN_PLAGENT_1 )
  {
    showPlantAgentScreen( plantAgentScreenDataList[0] );
  }
  else if ( buttonID == BTN_PLAGENT_2 )
  {
    showPlantAgentScreen( plantAgentScreenDataList[1] );
  }
  else if ( buttonID == BTN_PLAGENT_3 )
  {
    showPlantAgentScreen( plantAgentScreenDataList[2] );
  }
  else if ( buttonID == BTN_START_PROCESS )
  {
	// TODO Call CFP start function
  }
}
