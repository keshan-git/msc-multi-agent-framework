#ifndef Agent_h
#define Agent_h

#include "Arduino.h"

#ifndef MAX_AGENT_COUNT
	#define MAX_AGENT_COUNT 3
#endif

// Max topic count for each behaviour type
#ifndef MAX_TOPIC_COUNT 
	#define MAX_TOPIC_COUNT 3 
#endif

#ifdef ARDUINO_AVR_MEGA2560 
	#define SERIAL1 Serial1
	#define SERIAL2 Serial2
	#define SERIAL3 Serial3
#elif ARDUINO_AVR_NANO 
	#define SERIAL1 Serial
	#define SERIAL2 Serial
	#define SERIAL3 Serial
#else 
	#define SERIAL1 Serial
	#define SERIAL2 Serial
	#define SERIAL3 Serial
#endif

#define COMM_SERIAL 1
#define SERIAL_1 11
#define SERIAL_2 12
#define SERIAL_3 13

#define COMM_RF 2

// Define Performative typed
#define P_REQUEST 1
#define P_INFORM 2
#define P_QUERY_IF 3
#define P_CFP 4 // Call fro Proposal
#define P_PROPOSE 5
#define P_ACCEPT_PROPOSAL 6
#define P_REJECT_PROPOSAL 7

// Define Behaviour Types (http://www.iro.umontreal.ca/~vaucher/Agents/Jade/primer6.html#6.1)
#define BEHAVIOUR_TYPE_COUNT 5

#define BEHAVIOUR_SIMPLE 1
#define BEHAVIOUR_TICKER 2
#define BEHAVIOUR_WAKER 3
#define BEHAVIOUR_RECEIVER 4

#define COMMON_TOPIC "0"

// Define Hardware PINS
#define DEFAULT_STATUS_PIN 40

#define SERIAL_SPEED 9600
#define DUMMY_AGENT "0"

#define NILL -1

#define TOPIC_HART_BEAT "1"
#define TOPIC_PARAMETER "2"
#define TOPIC_WATERING "3"

// Define AgentConnection struct
typedef struct 
{
  String agentIdentifier;
  int communicationType;
  int conectionNumber;
} AgentConnection; 

// Define ACL message structure
typedef struct
{
	String sender;
	String receiver;
	int performative; // Type of the ACL message according to the FIPA
	String topic; // Or conversation ID
	String content;
} ACLMessage;

// Define TickerBehaviourType structure
typedef struct
{
	int period;
} TickerBehaviourInfo;

class Agent
{
  typedef void ( *MessageReceivedEvent ) ( ACLMessage message );
  typedef void ( *BehaviourEvent ) ( ACLMessage message );
  typedef void ( *TickerBehaviourEvent ) ( );
  typedef void ( *LogEvent ) ( String message );
  
  public:
  
	// Constructor
    Agent( String agentIdentifier, int maxConnections );
	
	// Methods
	void init( );
	void sync();
	boolean registerAgent( int index, int conectionNumber, String agentIdentifier, boolean indirectAgent );
    boolean registerAgent( int index, int conectionNumber, String agentIdentifier );
	boolean registerAgent( int index, int conectionType, String agentIdentifier, int txPin, int rxPin );
	boolean send( String receiver, String message );
	boolean sendToAll( String message );
	void addMessageReceivedEvent( MessageReceivedEvent event );
	void serialReceive( int conectionNumber );
	void addLogEvent( LogEvent logEvent );
	
	// Methods related to behaviours
	boolean send( ACLMessage message );
	boolean sendToAll( ACLMessage message );
	void addBehaviour( int behaviourType, int topicIndex, BehaviourEvent behaviourEvent );
	void addBehaviour( int behaviourType, int topicIndex, TickerBehaviourEvent behaviourEvent, int seconds );
	void addBehaviour( int behaviourType, BehaviourEvent behaviourEvent );
	
	// Debug only methods
	void printAgentMap();

	
  private:
	int _maxConnectionCount; // Maximum connections that this agent can manage
    String _agentIdentifier; // Used to identify the each agent uniquely
	AgentConnection _agentMap[ MAX_AGENT_COUNT ];
	MessageReceivedEvent _messageReceivedEvent;
	LogEvent _logEvent; // ONLY used for Debugging
	
	// attributes related to behaviours
	BehaviourEvent _behaviourMap [ BEHAVIOUR_TYPE_COUNT ][ MAX_TOPIC_COUNT ];
	TickerBehaviourEvent _tickerBehaviourEventList [ MAX_TOPIC_COUNT ];
	TickerBehaviourInfo _tickerBehaviourInfoList [ MAX_TOPIC_COUNT ];
	
	// private methods
	boolean setupSerial( int conectionNumber );
	boolean setupRFConnection( int txPin, int rxPin );
	boolean send( AgentConnection connection, String message );
	boolean send( AgentConnection connection, String message, boolean forward );
	boolean forward( AgentConnection connection, String message );
	AgentConnection getAgentConnection( String receiver );
	AgentConnection getAgentConnection( int communicationType, int conectionNumber );
	void processMessage( int conectionNumber, String fullMessage );
	void checkRFBuffer();
	void updateTimer();
	
	void performBehaviourEvent( ACLMessage aclMessage );
	String generateMessageFromACL( ACLMessage aclMessage );
	void timerISR();
};


#endif