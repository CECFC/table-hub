# 1 "/Users/bransoncamp/Arduino/table-hub/main.ino"
# 1 "/Users/bransoncamp/Arduino/table-hub/main.ino"
/*
table-hub
Created by Branson Camp for CECFC on 2019-10-1
Based on Fischer Moseley's Table-Remote (http://github.com/FischerMoseley/Table-Remote/)

----- Master Remote (#1) Codes -----
UP CODE: 6532833
DOWN CODE: 6532834
*/

# 12 "/Users/bransoncamp/Arduino/table-hub/main.ino" 2

// Switches
RCSwitch receiveSwitch = RCSwitch();
RCSwitch transmitSwitch = RCSwitch();

// Pins
const int RECEIVER_INTERRUPT = 0; // This is pin #2 on the Arduino Uno
const int TRANSMITTER_PIN = 10;
const int BIT_LENGTH = 24; //how many bits to send (used for binary zero padding)
const int LED_PIN = 13;

// Key Codes
const int MASTER_UP_CODE = 6532833;
const int MASTER_DOWN_CODE = 6532834;

const long WAIT_TIME = 500; // How long to wait in between sending codes in milliseconds
const long REPEAT_WAIT_TIME = 15000; // How long to wait in between repeated raising/lowering commands

const int TRANSMIT_ITERATIONS = 3; // How many times to repeat transmit of up/down codes

// Down button code = Up button code + 1
unsigned long int codes[]={ //stores the transmitter up button codes
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

int numCodes = 0; // Number of codes in the code array. Evaluated at setup();

// State
enum State {
    LISTENING,
    TRANSMIT_UP,
    TRANSMIT_DOWN,
} state = State::LISTENING;

void sendCode(unsigned long int code) {
    transmitSwitch.send(code, BIT_LENGTH);
}

void setup() {
    Serial.begin(9600);
    receiveSwitch.enableReceive(RECEIVER_INTERRUPT); // Set up reciever 
    transmitSwitch.enableTransmit(TRANSMITTER_PIN); // Set up transmitter
    numCodes = sizeof(codes) / sizeof(codes[0]); // Get number of codes
    pinMode(LED_PIN, 0x1); // Set up on-board LED pin
 Serial.println("Setup complete");
 Serial.println("Listening...");
}

void loop() {
    switch(state) {
        case State::LISTENING:
            if (receiveSwitch.available()) {
                int value = receiveSwitch.getReceivedValue();
                if (value == MASTER_UP_CODE) {
                    state = State::TRANSMIT_UP;
     Serial.println("Transmitting up codes");
                } else if (value == MASTER_DOWN_CODE) {
                    state = State::TRANSMIT_DOWN;
     Serial.println("Transmitting down codes");
                }
                receiveSwitch.resetAvailable();
            }
        break;
        case State::TRANSMIT_UP:
        case State::TRANSMIT_DOWN:
            for (int j = 0; j < TRANSMIT_ITERATIONS; j++) {
                for (int i = 0; i < numCodes; i++) {
     // Send the code
     unsigned long int code = codes[i] + 1;
     transmitSwitch.send(code, BIT_LENGTH);
     Serial.print("[code] ");
     Serial.println(code);
                    delay(WAIT_TIME);
                }
                delay(REPEAT_WAIT_TIME);
            }
            state = State::LISTENING;
   Serial.println("Listening...");

        break;
    }
}
