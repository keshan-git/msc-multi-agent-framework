#include "Arduino.h"
#include "Agent.h"
#include "VirtualWire.h"

#define DEBUG

/*
	Main Constructor of the Agent - initialize the agent with given name
*/
Agent::Agent( String agentIdentifier, int maxConnections )
{
	// Assign agent ID
	_maxConnectionCount = maxConnections;
	_agentIdentifier = agentIdentifier;
	
	pinMode( DEFAULT_STATUS_PIN, OUTPUT );
}

void Agent::addLogEvent( LogEvent logEvent )
{
	_logEvent = logEvent;
}

boolean Agent::registerAgent( int index, int conectionNumber, String agentIdentifier )
{
	registerAgent( index, conectionNumber, agentIdentifier, false );
}

boolean Agent::registerAgent( int index, int conectionNumber, String agentIdentifier, boolean indirectAgent )
{
	// Serial Communications
	if ( ( conectionNumber == SERIAL_1 ) || ( conectionNumber == SERIAL_2 ) || ( conectionNumber == SERIAL_3 ) )
	{
		// Add to Agent Map
		AgentConnection agentCon = { agentIdentifier, COMM_SERIAL, conectionNumber };
		String agentID = String( agentCon.agentIdentifier );

		if ( indirectAgent )
		{
			// This is indirect agent, no need of setup the actual communication channel. 
			// It should be already setup for the direct agent
			_agentMap[index] = agentCon;
			
			#ifdef DEBUG
			_logEvent("Indirect Agent Added [S] " + agentID);
			#endif
		}
		else
		{
			#ifdef DEBUG
			_logEvent( "Connected Agent Added [S] " + agentID );
			#endif
		
			boolean setupResult = setupSerial( conectionNumber );
			if ( setupResult )
			{
				_agentMap[index] = agentCon;
			}

			#ifdef DEBUG
			if ( setupResult )
			{
				_logEvent( "Initialized Serial Connection [SUCCESS]" );
			}
			else
			{
				_logEvent( "Initialized Serial Connection [FAIL]" );
			}
			#endif
		}
		
		return true;
	}
	else if ( ( conectionNumber == COMM_RF ) && indirectAgent )
	{
		// Add to Agent Map
		AgentConnection agentCon = { agentIdentifier, COMM_RF, NILL };
		String agentID = String( agentCon.agentIdentifier );
		
		// This is indirect agent, no need of setup the actual communication channel. 
		// It should be already setup for the direct agent
		_agentMap[index] = agentCon;
			
		#ifdef DEBUG
		_logEvent("Indirect Agent Added [RF] " + agentID);
		#endif
	}
	else
	{
		#ifdef DEBUG
		_logEvent("INVALID CONNECTION TYPE");
		#endif
		return false;
	}
}

boolean Agent::registerAgent( int index, int conectionType, String agentIdentifier, int txPin, int rxPin )
{
	// RF Communication
	if ( conectionType == COMM_RF )
	{
		// Add to Agent Map
		AgentConnection agentCon = { agentIdentifier, COMM_RF, NILL };

		#ifdef DEBUG
		String agentID = String( agentCon.agentIdentifier );
		_logEvent( "Connected Agent Added [RF] " + agentID );
		#endif
		
		
		boolean setupResult = setupRFConnection( txPin, rxPin );
		if ( setupResult )
		{
			_agentMap[index] = agentCon;
		}
		
		#ifdef DEBUG
		if ( setupResult )
		{
			_logEvent( "Initialized RF Connection [SUCCESS]" );
		}
		else
		{
			_logEvent( "Initialized RF Connection [FAIL]" );
		}
		#endif
		
		return true;
	}
	else
	{
		#ifdef DEBUG
		_logEvent("INVALID CONNECTION TYPE");
		#endif
		return false;
	}
}

void Agent::printAgentMap()
{
	#ifdef DEBUG
	_logEvent( "----Agent List-------" );
	int i;
	for (i = 0; i < _maxConnectionCount; i = i + 1)
	{
		AgentConnection agentCon = _agentMap[i];
		
		_logEvent( "Agent Details " + agentCon.agentIdentifier + " " + agentCon.communicationType + " " + agentCon.conectionNumber);

	}
	_logEvent( "----------------------" );
	#endif
}

// This method will sync the agent process
void Agent::sync()
{
	// Update timers
	updateTimer();
	
	// Check for RF messages
	checkRFBuffer();
}

boolean Agent::send( String receiver, String message )
{
	// Get connection details based on the receiver id
	AgentConnection connection = getAgentConnection( receiver );
	return send( connection, message );
}

boolean Agent::sendToAll( String message )
{
	int i;
	for (i = 0; i < _maxConnectionCount; i = i + 1)
	{
		AgentConnection agentCon = _agentMap[i];
		send( agentCon, message );
	}
	
	return true;
}

boolean Agent::send( AgentConnection connection, String message )
{
	return send( connection, message, false );
}

boolean Agent::forward( AgentConnection connection, String message )
{
	return send( connection, message, true );
}

boolean Agent::send( AgentConnection connection, String message, boolean forward )
{			
	if ( !connection.agentIdentifier.equals( DUMMY_AGENT ))
	{
		String finalMessage = message;
		if ( !forward )
		{
			finalMessage = _agentIdentifier + "#" + connection.agentIdentifier + "#" + message;
			#ifdef DEBUG
			_logEvent( "Final Message : " + finalMessage );
			#endif
		}
		
		if ( connection.communicationType == COMM_SERIAL )
		{
			#ifdef DEBUG
			if ( !forward )
			{
				_logEvent( "Sending ... : " + finalMessage );
			}
			else
			{
				_logEvent( "Forwading ... : " + finalMessage );	
			}
			#endif
			
			if ( connection.conectionNumber == SERIAL_1 )
			{
				digitalWrite( DEFAULT_STATUS_PIN, HIGH );
				SERIAL1.println( finalMessage );
				digitalWrite( DEFAULT_STATUS_PIN, LOW );
				return true;
			}
			else if ( connection.conectionNumber == SERIAL_2 )
			{
				digitalWrite( DEFAULT_STATUS_PIN, HIGH );
				SERIAL2.println( finalMessage );
				digitalWrite( DEFAULT_STATUS_PIN, LOW );
				return true;
			}
			else if ( connection.conectionNumber == SERIAL_3 )
			{
				digitalWrite( DEFAULT_STATUS_PIN, HIGH );
				SERIAL3.println( finalMessage );
				digitalWrite( DEFAULT_STATUS_PIN, LOW );
				return true;
			}
		}
		else if ( connection.communicationType == COMM_RF )
		{
			#ifdef DEBUG
			if ( !forward )
			{
				_logEvent( "Sending [RF] ... : " + finalMessage );
			}
			else
			{
				_logEvent( "Forwading [RF] ... : " + finalMessage );	
			}
			#endif
			
			//char *msg = "R1#R2#2#FreeTest#Hello Agent 123";
			const char * msg = finalMessage.c_str();
			uint8_t len = strlen( msg );
			
			if ( len > VW_MAX_PAYLOAD )
			{
				#ifdef DEBUG
				_logEvent( "Max length exceeded - " + String(len) );
				#endif
				return false;
			}

			digitalWrite( DEFAULT_STATUS_PIN, HIGH ); // Flash a light to show transmitting
			vw_send( (uint8_t *)msg, len );
			vw_wait_tx(); // Wait until the whole message is gone

			digitalWrite( DEFAULT_STATUS_PIN, LOW );
			_logEvent( msg );
			
			return true;
		}
	}
	else
	{
		#ifdef DEBUG
		_logEvent( "DUMMY" );
		#endif
	}

	return false;
}

void Agent::addMessageReceivedEvent( MessageReceivedEvent event )
{
	_messageReceivedEvent = event;
}

boolean Agent::setupSerial( int conectionNumber )
{
	delay( 100 );
	if ( conectionNumber == SERIAL_1 )
	{
		SERIAL1.begin( SERIAL_SPEED );
		return true;
	}
	else if ( conectionNumber == SERIAL_2 )
	{
		SERIAL2.begin( SERIAL_SPEED );
		return true;
	}
	else if ( conectionNumber == SERIAL_3 )
	{
		SERIAL3.begin( SERIAL_SPEED );
		return true;
	}
	
	return false;
}

// This method will setup the RF connection using the VirtualWire library
boolean Agent::setupRFConnection( int txPin, int rxPin )
{
	vw_set_tx_pin( txPin );
    vw_set_rx_pin( rxPin );
    vw_set_ptt_inverted( true ); // Required for DR3100
    vw_setup( 2000 );	 // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
}

void Agent::checkRFBuffer()
{
	uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if ( vw_get_message( buf, &buflen ) ) // Non-blocking
    {
		int i;
        String fullMessage;
        digitalWrite( DEFAULT_STATUS_PIN, HIGH );
		for (i = 0; i < buflen; i++)
		{
            fullMessage += char(buf[i]);
		}
        digitalWrite( DEFAULT_STATUS_PIN, LOW );
		processMessage( COMM_RF, fullMessage );
    }
}

AgentConnection Agent::getAgentConnection( String agent )
{
	int i = 0;
	for (i = 0; i < _maxConnectionCount; i = i + 1)
	{
		AgentConnection agentCon = _agentMap[i];
		if ( agentCon.agentIdentifier.equals( agent ) )
		{
			#ifdef DEBUG
			_logEvent( "Agent available - " + agent);
			#endif
			return agentCon;
		}
	}
	
	#ifdef DEBUG
	_logEvent( "Agent NOT available " + agent );
	#endif
	AgentConnection dummyConnection = { DUMMY_AGENT, COMM_SERIAL, SERIAL_1 };
	return dummyConnection;
}

AgentConnection Agent::getAgentConnection( int communicationType, int conectionNumber )
{
	int i = 0;
	for (i = 0; i < _maxConnectionCount; i = i + 1)
	{
		AgentConnection agentCon = _agentMap[i];
		if ( agentCon.communicationType == communicationType && agentCon.conectionNumber == conectionNumber )
		{
			#ifdef DEBUG
			_logEvent( "Sender - " + agentCon.agentIdentifier );
			#endif
			return agentCon;
		}
	}
	
	#ifdef DEBUG
	_logEvent( "Unknown Sender " );
	#endif
	AgentConnection dummyConnection = { DUMMY_AGENT, COMM_SERIAL, SERIAL_1 };
	return dummyConnection;
}

void Agent::serialReceive( int conectionNumber )
{
	if ( conectionNumber == SERIAL_1 )
	{
		while ( SERIAL1.available() ) 
		{
			String fullMessage = SERIAL1.readString();
			//#ifdef DEBUG
			//_logEvent( "Received : " + fullMessage );
			//#endif
			processMessage( SERIAL_1, fullMessage );
		}
	}
	else if ( conectionNumber == SERIAL_2 )
	{
		while ( SERIAL2.available() ) 
		{
			String fullMessage = SERIAL2.readString();
			processMessage( SERIAL_2, fullMessage );
		}
	}
	else if ( conectionNumber == SERIAL_3 )
	{
		while ( SERIAL3.available() ) 
		{
			String fullMessage = SERIAL3.readString();
			processMessage( SERIAL_3, fullMessage );
		}
	}
}

void Agent::processMessage( int conectionNumber, String fullMessage )
{
	//_logEvent( "L" + fullMessage.length() );

	#ifdef DEBUG
	//_logEvent( "Processing Message ... [START]" );
	#endif
	
	if ( fullMessage.length() == 0 )
	{
		#ifdef DEBUG
		_logEvent( "Empty Message" );
		#endif
		
		return;
	}	
	
	// Sender 
	int senderSeparator = fullMessage.indexOf( '#', 0 );
	String sender = fullMessage.substring( 0, senderSeparator );
	
	// Receiver
	int receiverSeparator = fullMessage.indexOf( '#', senderSeparator + 1 );
	String receiver = fullMessage.substring( senderSeparator + 1, receiverSeparator );
	
	// Receiver
	int performativeSeparator = fullMessage.indexOf( '#', receiverSeparator + 1 );
	String performative = fullMessage.substring( receiverSeparator + 1, performativeSeparator );
	int performativeInt  = performative.toInt();
	
	// Topic
	int topicSeparator = fullMessage.indexOf( '#', performativeSeparator + 1 );
	String topic = fullMessage.substring( performativeSeparator + 1, topicSeparator );
	
	// Content
	int contentSeparator = fullMessage.indexOf( '#', topicSeparator + 1 );
	String content = fullMessage.substring( topicSeparator + 1, contentSeparator );
	
	#ifdef DEBUG
	//_logEvent( "Processing Message ... [END]" );
	#endif
	
	if ( !_agentIdentifier.equals( sender ) )
	{			
		#ifdef DEBUG
		_logEvent( "Receive : " + sender + ":" + receiver + ":" + performativeInt + ":" + topic + ":" + content );
		#endif
				
		// Get connection details based on the sender id
		AgentConnection connection = getAgentConnection( sender );
		String senderID = connection.agentIdentifier;
			
		// This message is for ME
		if ( _agentIdentifier.equals( receiver ) )
		{
			// Sender
			if ( sender.equals( senderID ) )
			{
				#ifdef DEBUG
				_logEvent( "Actual senderID " + sender + " SenderID in Map : " + senderID );
				#endif
				
				// Call back to the message receive event
				ACLMessage acl = { sender, receiver, performativeInt, topic, content };
				_messageReceivedEvent( acl );
				
				// Perform behavioural based event call backs
				performBehaviourEvent( acl );
			}
			else
			{
				#ifdef DEBUG
				_logEvent( "Actual senderID " + sender + " SenderID in Map : " + senderID );
				#endif
			}
		}
		else
		{
			// Forward mode will be disable for RF communication
			if ( conectionNumber != COMM_RF )
			{
				// Receive indirect Agent Message; need to forward to correct agent
				// Get forward connection details
				AgentConnection forwardConnection = getAgentConnection( receiver );
				if ( forwardConnection.agentIdentifier != DUMMY_AGENT )
				{
					#ifdef DEBUG
					_logEvent( "Forwading the Message to : " +  receiver );
					#endif
					
					forward( forwardConnection, fullMessage );
				}
				else
				{
					#ifdef DEBUG
					_logEvent( "Fail to forward message to : " + receiver  );
					#endif
				}
			}
		}
	}
}

/* ------------------- Define Behaviour related methods ---------------- */
boolean Agent::send( ACLMessage message )
{
	// Get connection details based on the receiver id
	AgentConnection connection = getAgentConnection( message.receiver );
	String messageString = generateMessageFromACL( message );
	return send( connection, messageString );
}

boolean Agent::sendToAll( ACLMessage message )
{
	String messageString = generateMessageFromACL( message );
	
	int i;
	for (i = 0; i < _maxConnectionCount; i = i + 1)
	{
		AgentConnection agentCon = _agentMap[i];
		send( agentCon, messageString );
	}
	
	return true;
}

String Agent::generateMessageFromACL( ACLMessage inputACL )
{
	String aclContent = String(inputACL.performative) + "#" + inputACL.topic + "#" + inputACL.content;
	
	#ifdef DEBUG
	_logEvent( "ACL : " + aclContent );
	#endif
	
	return aclContent;
}

void Agent::addBehaviour( int behaviourType, BehaviourEvent behaviourEvent )
{
	addBehaviour( behaviourType, 0, behaviourEvent);
}

void Agent::addBehaviour( int behaviourType, int topicIndex, BehaviourEvent behaviourEvent )
{
	#ifdef DEBUG
	_logEvent( "Behaviour : " + String( behaviourType ) + " Topic : " + String( topicIndex ) );
	#endif
	_behaviourMap[ behaviourType ][ topicIndex ] = behaviourEvent;
}

void Agent::addBehaviour( int behaviourType, int topicIndex, TickerBehaviourEvent tickerBehaviourEvent, int seconds )
{
	if ( behaviourType == BEHAVIOUR_TICKER )
	{
		// Setup Timers for the 
		_tickerBehaviourEventList[ topicIndex ] = tickerBehaviourEvent;
		_tickerBehaviourInfoList[ topicIndex ] = { seconds };
			
		#ifdef DEBUG
		_logEvent( "Time period for Ticker Behaviour : " + String(seconds) + " seconds" );
		#endif
	}
}

void Agent::performBehaviourEvent( ACLMessage aclMessage )
{
	for ( int i = 0; i < MAX_TOPIC_COUNT; i++ )
	{
		BehaviourEvent behaviourEvent = _behaviourMap [ BEHAVIOUR_RECEIVER ][ i ]; // Behaviour Receiver need to receive the message real time
		if ( behaviourEvent != NULL )
		{
			behaviourEvent ( aclMessage );
		}
	}
}

/*------------------------------------ This method will start the internal timer to perform behaviourEvent periodically -------------------------------*/
unsigned long prevMillis = 0;
unsigned long currentMillis = 0;
unsigned long executionID = 0;
void Agent::updateTimer()
{
	if ( prevMillis == 0 )
	{
		prevMillis = millis();
	}
	
	currentMillis = millis();
	if ( currentMillis - prevMillis >= 1 * 1000 )
	{
		for ( int i = 0; i < MAX_TOPIC_COUNT; i++ )
		{
			TickerBehaviourInfo tickerBehaviourInfo = _tickerBehaviourInfoList [ i ];

			if ( tickerBehaviourInfo.period != NULL )
			{
				if ( executionID % tickerBehaviourInfo.period == 0 )
				{
					#ifdef DEBUG
					_logEvent( "Ticker Behaviour Event Fired" );
					#endif
					
					// Callback
					TickerBehaviourEvent tickerBehaviourEvent = _tickerBehaviourEventList[ i ];
					tickerBehaviourEvent();
				}
				//executionID = executionID + 1;
			}
		}
		executionID = executionID + 1;
		
		prevMillis = currentMillis;
		
		if ( executionID > 8 )
		{
			executionID = 1;
		}
	}
}


