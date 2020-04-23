#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <SPI.h>
#include <Wire.h>
#include <TouchScreen.h>
#include <AgentUI.h>

AgentUI agentUI;

void setup() 
{
  agentUI.init();

  // Generate data for home screen
  HomeScreenData homeData = { 100, {{ 1, true, C_WIRED }, { 2, false, C_NO }, { 3, true, C_RF }} };
  agentUI.showHomeScreen( homeData );
  
  //delay( 2000 );
  
  // Generate plant agent info panel
  PlantAgentInfo info = {1, 29, 72, 30};
  PlantAgentScreenData plantData = { info };
  agentUI.setPlantAgentScreenData(1, plantData);
  //agentUI.showPlantAgentScreen( plantData );

  //delay( 2000 );
  CFPScreenData cfpData = { {{1, 29, 72, 30}, {2, 39, 50, 90}, {3, 32, 60, 55}},
                            {{50, 30}, {20, 20}, {30, 10}},
                            88};
  agentUI.setCFPScreenData( cfpData );
  //agentUI.showCFPScreen( cfpData );

  agentUI.addLogLine("Test1");
  agentUI.addLogLine("Test2");
  

}

PlantAgentInfo infoTemp = {1, 29, 72, 30};
WaterInfo waterInfo = {0,0};
WaterTankInfo waterTankInfo = { 70 };

TimeInfo timeInfo = {1,0,0};
void loop()
{
  agentUI.sync();
  
//  delay( 1000 );
//  AgentConnectionInfo testInfo = {1, false, C_WIRED};
//  agentUI.updateAgentConnectionStatus(testInfo);
//  
//  delay( 1000 );
//  testInfo = {1, true, C_WIRED};
//  agentUI.updateAgentConnectionStatus(testInfo);
//  
//  delay( 1000 );
//  testInfo = {2, true, C_RF};
//  agentUI.updateAgentConnectionStatus(testInfo);
//  
//  delay( 1000 );
//  testInfo = {3, false, C_RF};
//  agentUI.updateAgentConnectionStatus(testInfo);

  delay(200);
  infoTemp.temperature += 1;
  agentUI.updatePlantAgentInformation( infoTemp );

  agentUI.updateTotalWaterLevel( infoTemp.temperature);

  infoTemp.agentNum = infoTemp.temperature % 3 + 1;
  waterInfo.requestUnits += 1;
  waterInfo.acceptUnits += 1;
  agentUI.updateAgentCFPPanel( infoTemp, waterInfo );

  agentUI.updateAgentStatusCFPPanel( 2, CFP_STATUS_ACCEPT );
  agentUI.addLogLine("A");

  //waterTankInfo.waterLevel += 1;
  //agentUI.updateWaterTankInformation( waterTankInfo ) ;
  timeInfo.hour++;
  timeInfo.min++;
  timeInfo.sec++;
  agentUI.updateTime( timeInfo );
}
