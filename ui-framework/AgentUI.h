#ifndef AgentUI_h
#define AgentUI_h

#include "Arduino.h"
#include "Adafruit_TFTLCD.h"

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4

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

// Define color schema for the UI
#define C_BLACK 0x0000 // BLACK

#define C_ICON_BORDER 0xFFFF // WHITE
#define C_HEADER_BACKGROUND 0xF81F // MAGENTA
#define C_HEADER_FOREGROUND 0xFFFF  // WHITE
#define C_HEADER_SEPARATOR 0xFFFF  // WHITE
#define C_HEADER_ICON 0xFFFF       // WHITE

// action bar colors
#define C_ACTIONBAR_HOME_OUTER 0xFFFF // WHITE
#define C_ACTIONBAR_NETWORK_OUTER 0xFFFF // WHITE
#define C_ACTIONBAR_CFP_OUTER 0xFFFF // WHITE
#define C_ACTIONBAR_RESOURCE_OUTER 0xFFFF // WHITE
#define C_ACTIONBAR_LOG_OUTER 0xFFFF // WHITE
#define C_ACTIONBAR_ICON_COLOR 0xF81F // MAGENTA

// main panel colors
#define C_AGENT_MAIN_PANEL_BORDER 0x001F // BLUE
#define C_AGENT_MAIN_PANEL_BUTTON 0x001F // BLUE
#define C_AGENT_MAIN_PANEL_BUTTON_FOREGROUND 0xFFFF // BLUE

#define C_AGENT_SUM_PANEL_BORDER 0x001F // BLUE
#define C_AGENT_SUM_PANEL_FOREGROUND 0xFFFF // WHITE
#define C_AGENT_SUM_PANEL_CONNECT_BACKGROUND 0x07E0 // GREEN
#define C_AGENT_SUM_PANEL_CONNECT_FOREGROUND 0xFFFF // WHITE
#define C_AGENT_SUM_PANEL_DISCONNECT_BACKGROUND 0xF800 // RED
#define C_AGENT_SUM_PANEL_COMTYPE_BACKGROUND 0xF81F // MAGENTA
#define C_AGENT_SUM_PANEL_ICON_BACKGROUND 0x07E0 // GREEN

// agent panel colors
#define C_AGENT_PANEL_TITLE_BACKGROUND 0x001F // BLUE
#define C_AGENT_PANEL_TITLE_FOREGROUND 0xFFFF // WHITE
#define C_AGENT_PANEL_METER_FOREGROUND 0xFFFF // WHITE
#define C_AGENT_PANEL_METER_BACKGROUND_NORMAL 0x07E0 // GREEN
#define C_AGENT_PANEL_METER_BACKGROUND_WARNING 0xFFE0 // YELLOW
#define C_AGENT_PANEL_METER_BACKGROUND_CRITICAL 0xF800 // RED
#define C_AGENT_PANEL_METER_TITLE_BACKGROUND 0xF800 // RED
#define C_AGENT_PANEL_METER_TITLE_FOREGROUND 0xFFFF // WHITE

// CFP panel
#define C_CFP_PANEL_AGENT_BORDER 0x001F // BLUE
#define C_CFP_PANEL_AGENT_TITLE_FOREGROUND 0xFFFF // WHITE
#define C_CFP_PANEL_AGENT_TEMP_BACKGROUND 0xF800 // RED
#define C_CFP_PANEL_AGENT_TEMP_FOREGROUND 0xFFFF // WHITE
#define C_CFP_PANEL_AGENT_HUM_BACKGROUND 0xF800 // RED
#define C_CFP_PANEL_AGENT_HUM_FOREGROUND 0xFFFF // WHITE
#define C_CFP_PANEL_AGENT_WATER_BACKGROUND 0xF800 // RED
#define C_CFP_PANEL_AGENT_WATER_FOREGROUND 0xFFFF // WHITE
#define C_CFP_PANEL_AGENT_WATER_REQUEST_BACKGROUND 0x07E0 // GREEN
#define C_CFP_PANEL_AGENT_WATER_REQUEST_FOREGROUND 0xFFFF // WHITE
#define C_CFP_PANEL_AGENT_WATER_ACCEPT_BACKGROUND 0x07E0 // GREEN
#define C_CFP_PANEL_AGENT_WATER_ACCEPT_FOREGROUND 0xFFFF // WHITE

#define C_CFP_PANEL_AGENT_INFO_ICON 0x001F // BLUE
#define C_CFP_PANEL_AGENT_ACCEPT_ICON 0x07E0 // GREEN
#define C_CFP_PANEL_AGENT_REJECT_ICON 0xF800 // RED
#define C_CFP_PANEL_AGENT_RETRY_ICON 0x001F // BLUE

#define C_CFP_PANEL_WATER_LEVEL_BACKGROUND 0xF81F // MAGENTA
#define C_CFP_PANEL_WATER_LEVEL_FOREGROUND 0xFFFF // WHITE
#define C_CFP_PANEL_START_BUTTON_BORDER 0xF800 // RED
#define C_CFP_PANEL_START_BUTTON_FOREGROUND 0xF800 // RED

#define C_LOG_PANEL_BORDER 0x07E0 // GREEN
#define C_LOG_PANEL_TEXT_FORGROUND 0x07E0 // GREEN

#define C_NO 0
#define C_WIRED 1
#define C_RF 2
#define C_WIFI 3
#define C_BLE 4

#define NO_SCREEN -1
#define HOME_SCREEN 1
#define PLANT_AGENT_SCREEN 2
#define CFP_SCREEN 3
#define LOG_SCREEN 4
#define AGENT_NETWORK_SCREEN 5

#define BTN_HOME 1
#define BTN_NETWORK 2
#define BTN_CFP 3
#define BTN_CFP_BACK 4
#define BTN_LOG 5
#define BTN_PLAGENT_1 6
#define BTN_PLAGENT_2 7
#define BTN_PLAGENT_3 8
#define BTN_WATER_FILL 9
#define BTN_WATER_INC 10
#define BTN_WATER_DEC 11
#define BTN_START_PROCESS 12

#define CFP_STATUS_NORMAL 1
#define CFP_STATUS_ACCEPT 2
#define CFP_STATUS_REJECT 3
#define CFP_STATUS_RETRY 4

typedef struct
{
  uint8_t agentNum;
  boolean online;
  uint8_t comType;
} AgentConnectionInfo;

typedef struct
{
  uint8_t agentNum;
  uint8_t temperature;
  uint8_t humidity;
  uint8_t waterLevel;
} PlantAgentInfo;

typedef struct
{
  uint8_t requestUnits;
  uint8_t acceptUnits;
} WaterInfo;

typedef struct
{
  uint8_t waterLevel;
} WaterTankInfo;

typedef struct
{
  // TODO tank agent info display in home screen
  //WaterTankInfo waterTankInfo
  uint8_t totalWaterLevel;
  AgentConnectionInfo connectionList[3];
} HomeScreenData;

typedef struct
{
  PlantAgentInfo plantAgentInfo;
} PlantAgentScreenData;

typedef struct
{
  PlantAgentInfo plantAgentInfoList[3];
  WaterInfo waterInfoList[3];
  uint8_t totalWaterLevel;
} CFPScreenData;

typedef struct
{
	int8_t hour;
	int8_t min;
	int8_t sec;
} TimeInfo;

typedef struct
{
	uint16_t year;
	int8_t month;
	int8_t day;
} DateInfo;

// Icon Definitions
static unsigned const char __attribute__ ((progmem)) iconHome[] =
{
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000001, B11000000, B00000000,
B00000000, B00000011, B11100111, B11100000,
B00000000, B00000111, B11110111, B11100000,
B00000000, B00001111, B11111111, B11100000,
B00000000, B00111111, B11111111, B11100000,
B00000000, B01111111, B11111111, B11100000,
B00000000, B11111111, B11111111, B11100000,
B00000001, B11111111, B11111111, B11000000,
B00000111, B11111111, B11111111, B11100000,
B00001111, B11111111, B11111111, B11111000,
B00011111, B11111111, B11111111, B11111100,
B01111111, B11111111, B11111111, B11111110,
B01111111, B11111111, B11111111, B11111111,
B00111111, B11111111, B11111111, B11111110,
B00011111, B11111111, B11111111, B11111100,
B00001011, B11111000, B00001111, B11101000,
B00000011, B11111000, B00011111, B11100000,
B00000011, B11111000, B00011111, B11100000,
B00000011, B11111000, B00011111, B11100000,
B00000011, B11111000, B00011111, B11100000,
B00000011, B11111000, B00011111, B11100000,
B00000011, B11111000, B00011111, B11100000,
B00000011, B11111000, B00011111, B11100000,
B00000011, B11111000, B00011111, B11100000,
B00000011, B11111000, B00011111, B11100000,
B00000011, B11111000, B00011111, B11100000,
B00000000, B00000000, B00000000, B00000000
};

static unsigned const char __attribute__ ((progmem)) iconNetwork[] =
{
B00111100, B00000000, B00011110, B00000000,
B01111111, B00000000, B00111111, B10000000,
B11111111, B00000000, B01111111, B10000000,
B11111111, B00000000, B01111111, B11000000,
B11111111, B00000000, B01111111, B11000000,
B11111111, B00000000, B01111111, B11000000,
B01111111, B00000000, B00111111, B10000000,
B00111111, B10000000, B00011111, B00000000,
B00000001, B11000000, B00111000, B00000000,
B00000000, B11100000, B00111000, B00000000,
B00000000, B01110111, B11110000, B00000000,
B00000000, B00111111, B11111000, B00000000,
B00000000, B00011111, B11111100, B00000000,
B00000000, B00111111, B11111110, B00000000,
B00000000, B00111111, B11111110, B00000000,
B00000000, B00111111, B11111110, B00111110,
B00000000, B00111111, B11111110, B01111111,
B00000000, B00111111, B11111111, B11111111,
B00000000, B00111111, B11111111, B11111111,
B00000000, B00011111, B11111100, B01111111,
B00000000, B01111111, B11111100, B01111111,
B00000000, B11111111, B11111000, B01111111,
B00111101, B11100011, B11111000, B00111110,
B01111111, B11000000, B00011000, B00000000,
B11111111, B00000000, B00011110, B00000000,
B11111111, B00000000, B00001111, B10000000,
B11111111, B00000000, B00011111, B11000000,
B11111111, B00000000, B00111111, B11100000,
B11111111, B00000000, B00111111, B11100000,
B01111110, B00000000, B00111111, B11100000,
B00011000, B00000000, B00011111, B11100000,
B00000000, B00000000, B00011111, B11000000
};

static unsigned const char __attribute__ ((progmem)) iconLog[] =
{
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000011, B11000000, B00000000,
B00000000, B00000111, B11100000, B00000000,
B00000111, B11110111, B11110111, B11100000,
B00000111, B11110111, B01110111, B11110000,
B00000111, B11110111, B11110111, B11110000,
B00000111, B00001111, B11111000, B01110000,
B00000111, B00111111, B11111110, B01110000,
B00000111, B00111111, B11111110, B01110000,
B00000111, B00111111, B11111110, B01110000,
B00000111, B00000000, B00000000, B01110000,
B00000111, B01111111, B10000000, B01110000,
B00000111, B01111111, B10000000, B01110000,
B00000111, B00000000, B00000000, B01110000,
B00000111, B01111111, B11111110, B01110000,
B00000111, B01111111, B11111110, B01110000,
B00000111, B00000000, B00000000, B01110000,
B00000111, B01111111, B11110000, B01110000,
B00000111, B01111111, B11110000, B01110000,
B00000111, B00000000, B00000000, B01110000,
B00000111, B01111111, B11111000, B01110000,
B00000111, B01111111, B11111000, B01110000,
B00000111, B00000000, B00000000, B01110000,
B00000111, B01111111, B11000000, B01110000,
B00000111, B01111111, B11000000, B01110000,
B00000111, B00000000, B00000000, B01110000,
B00000111, B00000000, B00000000, B01110000,
B00000111, B11111111, B11111111, B11110000,
B00000111, B11111111, B11111111, B11110000,
B00000011, B11111111, B11111111, B11100000,
B00000000, B00000000, B00000000, B00000000
};

static unsigned const char __attribute__ ((progmem)) iconCFP[] =
{
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00011000, B00000000, B00000000,
B00000000, B00011000, B00000000, B00000000,
B00000111, B00111100, B11100000, B00000000,
B00000011, B11111111, B11000000, B00000000,
B00000011, B11111111, B11000000, B00000000,
B00000011, B11111111, B11000000, B00000000,
B00011111, B11100111, B11111100, B00000000,
B00111111, B10000001, B11111100, B00000000,
B00011111, B10000001, B11111000, B00000000,
B00001111, B00000000, B11110000, B00000000,
B00001111, B00000000, B11110000, B00000000,
B00011111, B10000001, B11111000, B00000000,
B00111111, B10000001, B11111100, B00000000,
B00111111, B11100111, B11111100, B00000000,
B00000011, B11111111, B11000000, B00000000,
B00000011, B11111111, B11001000, B00110000,
B00000011, B11111111, B11001111, B11110000,
B00000111, B00111100, B11101111, B11110000,
B00000000, B00011000, B00011111, B11110000,
B00000000, B00011000, B00011111, B11111000,
B00000000, B00000000, B01111110, B01111100,
B00000000, B00000000, B11111100, B01111110,
B00000000, B00000000, B01111100, B01111110,
B00000000, B00000000, B00011111, B11111000,
B00000000, B00000000, B00011111, B11110000,
B00000000, B00000000, B00001111, B11110000,
B00000000, B00000000, B00001111, B11110000,
B00000000, B00000000, B00001100, B00110000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000
};

static unsigned const char __attribute__ ((progmem)) iconPlant[] =
{
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B10000000,
B00000000, B00000000, B00000001, B10000000,
B00000000, B00000000, B00000111, B11000000,
B00000000, B00000000, B00111111, B11000000,
B00000000, B00000001, B11111111, B11000000,
B00000000, B00000111, B11111111, B11000000,
B00000000, B00011111, B11111111, B11100000,
B00000000, B00111111, B11111111, B11100000,
B00000000, B01111111, B11111111, B11100000,
B00000000, B11111111, B11111111, B11100000,
B00000001, B11111111, B11111111, B11100000,
B00000001, B11111111, B11111111, B11100000,
B00000011, B11111111, B11111011, B11100000,
B00000011, B11111111, B11110111, B11100000,
B00000011, B11111111, B11110111, B11100000,
B00000011, B11111111, B11101111, B11100000,
B00000011, B11111111, B11001111, B11100000,
B00000011, B11111111, B11011111, B11100000,
B00000001, B11111111, B10111111, B11100000,
B00000000, B11111111, B00111111, B11000000,
B00000000, B01111110, B01111111, B11000000,
B00000000, B00011100, B11111111, B11000000,
B00000000, B00011001, B11111111, B10000000,
B00000000, B01100011, B11111111, B00000000,
B00000001, B11000011, B11111110, B00000000,
B00011111, B00000000, B11111100, B00000000,
B00011100, B00000000, B00000000, B00000000
};

static unsigned const char __attribute__ ((progmem)) iconAccept[] =
{
B00000000, B00111111, B11100000, B00000000,
B00000000, B11111111, B11111000, B00000000,
B00000011, B11111111, B11111110, B00000000,
B00000111, B11111111, B11111111, B00000000,
B00001111, B11111111, B11111111, B10000000,
B00011111, B11111111, B11111111, B11000000,
B00111111, B11111111, B11111111, B11100000,
B00111111, B11111111, B11111111, B11100000,
B01111111, B11111111, B11111111, B11110000,
B01111111, B11111111, B11111111, B11110000,
B11111111, B11111111, B11111011, B11110000,
B11111111, B11111111, B11110001, B11110000,
B11111111, B11111111, B11100011, B11110000,
B11111111, B11111111, B11000111, B11110000,
B11111111, B01111111, B10001111, B11110000,
B11111111, B00111111, B00011111, B11110000,
B11111111, B10011110, B00111111, B11110000,
B11111111, B10001100, B01111111, B11110000,
B11111111, B11000000, B11111111, B11110000,
B01111111, B11100001, B11111111, B11110000,
B01111111, B11110011, B11111111, B11110000,
B00111111, B11111111, B11111111, B11100000,
B00111111, B11111111, B11111111, B11100000,
B00011111, B11111111, B11111111, B11000000,
B00001111, B11111111, B11111111, B10000000,
B00000111, B11111111, B11111111, B00000000,
B00000011, B11111111, B11111110, B00000000,
B00000000, B11111111, B11111000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000
};

static unsigned const char __attribute__ ((progmem)) iconReject[] =
{
B00000000, B00001111, B10000000, B00000000,
B00000000, B01111111, B11110000, B00000000,
B00000000, B11111111, B11111100, B00000000,
B00000011, B11111111, B11111110, B00000000,
B00000111, B11111111, B11111111, B00000000,
B00001111, B11111111, B11111111, B10000000,
B00011111, B11111111, B01111111, B11000000,
B00011111, B11111110, B00111111, B11100000,
B00111111, B11111110, B00111111, B11100000,
B00111111, B11111111, B11111111, B11110000,
B01111111, B11111111, B11111111, B11110000,
B01111111, B11111000, B01111111, B11110000,
B01111111, B11110000, B01111111, B11110000,
B01111111, B11111100, B01111111, B11110000,
B01111111, B11111000, B11111111, B11110000,
B01111111, B11111000, B11111111, B11110000,
B01111111, B11111000, B11111111, B11110000,
B01111111, B11110001, B11111111, B11110000,
B00111111, B11110001, B11111111, B11110000,
B00111111, B11110001, B01111111, B11110000,
B00111111, B11100000, B11111111, B11100000,
B00011111, B11110001, B11111111, B11100000,
B00001111, B11111111, B11111111, B11000000,
B00001111, B11111111, B11111111, B10000000,
B00000111, B11111111, B11111111, B00000000,
B00000011, B11111111, B11111110, B00000000,
B00000000, B11111111, B11111000, B00000000,
B00000000, B00111111, B11100000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000
};

static unsigned const char __attribute__ ((progmem)) iconInfo[] =
{
B00000000, B00000000, B00000000, B00000000,
B00000000, B00001111, B10000000, B00000000,
B00000000, B11111111, B11111000, B00000000,
B00000001, B11111111, B11111100, B00000000,
B00000111, B11111111, B11111111, B00000000,
B00001111, B11111000, B11111111, B10000000,
B00001111, B11110000, B01111111, B10000000,
B00011111, B11110000, B01111111, B11000000,
B00111111, B11110000, B01111111, B11100000,
B00111111, B11110000, B01111111, B11100000,
B00111111, B11111000, B11111111, B11100000,
B00111111, B11111111, B11111111, B11100000,
B01111111, B11110000, B01111111, B11100000,
B01111111, B11110000, B01111111, B11110000,
B01111111, B11110000, B01111111, B11110000,
B01111111, B11110000, B01111111, B11110000,
B01111111, B11110000, B01111111, B11100000,
B00111111, B11110000, B01111111, B11100000,
B00111111, B11110000, B01111111, B11100000,
B00111111, B11110000, B01111111, B11100000,
B00011111, B11110000, B01111111, B11000000,
B00011111, B11110000, B01111111, B11000000,
B00001111, B11110000, B01111111, B10000000,
B00000111, B11111111, B11111111, B00000000,
B00000011, B11111111, B11111110, B00000000,
B00000001, B11111111, B11111100, B00000000,
B00000000, B01111111, B11110000, B00000000,
B00000000, B00000010, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000
};

static unsigned const char __attribute__ ((progmem)) iconRetry[] =
{
B00000000, B00000111, B00000000, B00000000,
B00000000, B00111111, B11100000, B00000000,
B00000000, B11111111, B11111000, B00000000,
B00000011, B11111111, B11111110, B00000000,
B00000111, B11111111, B11111111, B00000000,
B00001111, B11111111, B11111111, B10000000,
B00011111, B11111111, B11111111, B11000000,
B00111111, B11111111, B11111111, B11000000,
B00111111, B11100000, B00110111, B11100000,
B00111111, B11000000, B00000111, B11100000,
B01111111, B10001111, B10000111, B11110000,
B01111111, B00011111, B10000111, B11110000,
B01111111, B00111111, B00000111, B11110000,
B11111111, B11111111, B11111111, B11110000,
B11111111, B11111111, B11111111, B11110000,
B11111111, B11111111, B11111111, B11110000,
B01111111, B00001111, B11100111, B11110000,
B01111111, B00011111, B11000111, B11110000,
B01111111, B00001111, B10001111, B11110000,
B00111111, B00000000, B00011111, B11100000,
B00111111, B11100000, B00111111, B11100000,
B00011111, B11111111, B11111111, B11000000,
B00011111, B11111111, B11111111, B11000000,
B00001111, B11111111, B11111111, B10000000,
B00000111, B11111111, B11111111, B00000000,
B00000011, B11111111, B11111110, B00000000,
B00000000, B11111111, B11111000, B00000000,
B00000000, B00111111, B11100000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000
};

static unsigned const char __attribute__ ((progmem)) iconWatering[] =
{
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00011110, B00111000, B11110000,
B00000000, B00111111, B11111111, B11111000,
B00000000, B00111111, B11111111, B11111000,
B00000000, B00011110, B01111100, B11110000,
B00000000, B00001100, B00111100, B00100000,
B00000000, B00000000, B00111000, B00000000,
B00000000, B00000000, B00111000, B00000000,
B00000000, B00000000, B01111100, B00000000,
B00000000, B00000000, B11111110, B00000000,
B00000000, B00000000, B11111110, B00000000,
B00000000, B00000000, B11111110, B00000000,
B00000000, B11111111, B11111111, B11111000,
B00000001, B11111111, B11111111, B11111000,
B00000011, B11111111, B11111111, B11111000,
B00000111, B11111111, B11111111, B11111000,
B00000111, B11100000, B00000000, B00000000,
B00000111, B11000000, B00000000, B00000000,
B00000111, B10000000, B00000000, B00000000,
B00000111, B10000000, B00000000, B00000000,
B00000111, B10000000, B00000000, B00000000,
B00000111, B10000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000011, B00000000, B00000000, B00000000,
B00000111, B00000000, B00000000, B00000000,
B00000111, B10000000, B00000000, B00000000,
B00001111, B10000000, B00000000, B00000000,
B00001111, B10000000, B00000000, B00000000,
B00001111, B10000000, B00000000, B00000000,
B00000111, B10000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
};

class AgentUI : public Adafruit_TFTLCD
{
	typedef void ( *ButtonPressedEvent ) (  uint8_t buttonID );
	  
  public:
  	HomeScreenData homeScreenData;
  	PlantAgentScreenData plantAgentScreenDataList[3];
  	CFPScreenData cfpScreenData;

  	// Constructor
  	AgentUI( );

  	// Methods
  	void init();
  	void sync();
  	void setHomeScreenData( HomeScreenData data );
  	void setPlantAgentScreenData( uint8_t agentNum, PlantAgentScreenData data );
  	void setCFPScreenData( CFPScreenData data );
	void addButtonPressedEvent( ButtonPressedEvent event );
	uint8_t getCurrentScreenID();
	
  	void showHomeScreen( HomeScreenData data );
  	void showPlantAgentScreen( PlantAgentScreenData data );
  	void showCFPScreen( CFPScreenData data );
    void showLogScreen();

  	// Update methods
	void updateTime( TimeInfo timeInfo );
	void updateDate( DateInfo timeInfo );
    void updateWaterTankInformation( WaterTankInfo waterTankInfo );
  	void updateAgentConnectionStatus( AgentConnectionInfo info );
    void updatePlantAgentInformation( PlantAgentInfo info );
    void updateTotalWaterLevel( uint8_t totalWaterLevel );
	void updateAgentStatusCFPPanel( uint8_t agentNum, uint8_t status );
    void updateAgentCFPPanel( PlantAgentInfo agentData, WaterInfo waterData );

    void addLogLine( String logLine );
  	//TODO add other update methods

  private:
    uint8_t logLineIndex = 0;
    String logBuffer[15];
    uint8_t waterLevelInd = 100;
	uint8_t waterLevelStep = 10;
	
	ButtonPressedEvent _buttonPressedEvent;
		
    void showHeaderPanel();
    void showMainPanel();
    void showActionPanel( );
    void showActionPanel( uint8_t screenID );
    void showAgentInfoPanel( uint16_t x, uint16_t y, AgentConnectionInfo connection );
    void showPlantAgentScreen( PlantAgentInfo data );
    void showAgentCFPPanel( uint16_t x, uint16_t y, PlantAgentInfo agentData, WaterInfo waterData );
    void showTotalWaterLevel( uint16_t x, uint16_t y, uint8_t totalWaterLevel );
    void showStartButton(uint16_t x, uint16_t y);
    uint16_t getTempMeterColor( uint8_t value );
    uint16_t getHumMeterColor( uint8_t value );
    uint16_t getWaterMeterColor( uint8_t value );
    void displayDigit( uint16_t x, uint16_t y, uint8_t digit, uint16_t color );
    void showLogPanel( uint16_t x, uint16_t y );
    void updateLogPanel( uint16_t x, uint16_t y, String logLine );

    void drawImage( uint16_t x, uint16_t y, uint16_t color );
    void onTouchEvent( int16_t x, int16_t y );
    void onButtonPressed( uint8_t buttonID );

    // Update methods
    void updateMainPanel( uint8_t waterLevel );
    void updateAgentInfoPanel( uint16_t x, uint16_t y, AgentConnectionInfo connection );
    void updatePlantAgentScreen( PlantAgentInfo data );
    void updateTotalWaterLevel( uint16_t x, uint16_t y, uint8_t totalWaterLevel );
	void updateAgentCFPPanel( uint16_t x, uint16_t y, uint8_t status );
    void updateAgentCFPPanel( uint16_t x, uint16_t y, PlantAgentInfo agentData, WaterInfo waterData );

  };
#endif