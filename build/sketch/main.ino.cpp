#include <Arduino.h>
#line 1 "/Users/bransoncamp/Arduino/table-hub/main.ino"
#line 1 "/Users/bransoncamp/Arduino/table-hub/main.ino"
#include <RCSwitch.h>

typedef unsigned long int Code;

// Switches
RCSwitch receiveSwitch = RCSwitch();
RCSwitch transmitSwitch = RCSwitch();

// Pins
const int RECEIVER_INTERRUPT = 0; 
const int TRANSMITTER_PIN = 10;
const int BIT_LENGTH = 24; 
const int LED_PIN = 13; // Onboard LED pin

// Key Codes
Code MASTER_UP_CODE = 6532833;
Code MASTER_DOWN_CODE = 6532834;

const long CODE_WAIT_TIME = 500;

const int codeNum = 12;


// Up codes for each desk (1-12)
// Down code = Up code + 1
Code codes[] = {   
	6532833, 
  	13806609,
  	10427409,
  	15956497,
  	78353,
  	13108961,
  	473617,
  	6525457,
  	252945,
  	9861857,
  	14031377,
  	711697,
};

// Prototypes
void sendCode(Code code);
void raiseAll();
void lowerAll();

#line 46 "/Users/bransoncamp/Arduino/table-hub/main.ino"
void setup();
#line 55 "/Users/bransoncamp/Arduino/table-hub/main.ino"
void loop();
#line 46 "/Users/bransoncamp/Arduino/table-hub/main.ino"
void setup() {
	Serial.begin(9600);
    receiveSwitch.enableReceive(RECEIVER_INTERRUPT);
    transmitSwitch.enableTransmit(TRANSMITTER_PIN);
    pinMode(LED_PIN, OUTPUT); // Set up on-board LED pin
	Serial.println("Setup complete");
	Serial.println("Listening...");
}

void loop() {
	if (receiveSwitch.available()) {
		Code value = receiveSwitch.getReceivedValue();
		if (value == MASTER_UP_CODE) {
			raiseAll();
		} else if (value == MASTER_DOWN_CODE) {
			lowerAll();
		}	
		receiveSwitch.resetAvailable();
	}
}

void sendCode(Code code) {
	Serial.print("sending");
	Serial.println(code);
	transmitSwitch.send(code, BIT_LENGTH);
}

void raiseAll() {
	Serial.println("Transmitting up codes");
	for (int i = 0; i < codeNum; i++) {
		Code code = codes[i];	
		sendCode(code);
		delay(CODE_WAIT_TIME);
	}	
	Serial.println("Done. \nListening...");
}

void lowerAll() {
	Serial.println("Transmitting down codes");
	for (int i = 0; i < codeNum; i++) {
		Code code = codes[i] + 1;	
		sendCode(code);
		delay(CODE_WAIT_TIME);
	}	
	Serial.println("Done. \nListening...");
}


