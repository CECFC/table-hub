/*
table-hub
Created by Branson Camp for CECFC on 2019-10-1
Based on Fischer Moseley's Table-Remote (http://github.com/FischerMoseley/Table-Remote/)

----- Master Remote (#1) Codes -----
UP CODE: 6532833
DOWN CODE: 6532834
*/

#include <RCSwitch.h>

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
const long STATE_WAIT_TIME = 15000; // How long before repeating the raise/lower task

const int TRANSMIT_ITERATIONS = 3; // How many times to transmit up/down codes

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
    Serial.print("[SENDING] ");
    Serial.println(code);
}

void setup() {
    Serial.begin(9600);
    receiveSwitch.enableReceive(RECEIVER_INTERRUPT); 
    transmitSwitch.enableTransmit(TRANSMITTER_PIN);

    // Get number of codes
    numCodes = sizeof(codes) / sizeof(codes[0]);

    pinMode(LED_PIN, OUTPUT);

    Serial.println("Booted up.\nLISTENING...");
}

void loop() {
    switch(state) {
        case State::LISTENING:
            if (receiveSwitch.available()) {
                int value = receiveSwitch.getReceivedValue();
                if (value == MASTER_UP_CODE) {
                    state = State::TRANSMIT_UP;
                } else if (value == MASTER_DOWN_CODE) {
                    state = State::TRANSMIT_DOWN;
                }
                receiveSwitch.resetAvailable();
            }
        break;
        case State::TRANSMIT_UP:
            for (int j = 0; j < TRANSMIT_ITERATIONS; j++) {
                Serial.print("----- TRANSMITTING UP CODES (");
                Serial.print(j+1);
                Serial.print("/");
                Serial.print(TRANSMIT_ITERATIONS);
                Serial.println(") -----");

                for (int i = 0; i < numCodes; i++) {
                    sendCode(codes[i]);
                    // Toggle LED
                    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
                    delay(WAIT_TIME);
                }
                delay(STATE_WAIT_TIME);
            }
            Serial.println("------------- DONE --------------");
            Serial.println("LISTENING...");
            state = State::LISTENING;
        break;
        case State::TRANSMIT_DOWN:
            for (int j = 0; j < TRANSMIT_ITERATIONS; j++) {
                Serial.print("---- TRANSMITTING DOWN CODES (");
                Serial.print(j+1);
                Serial.print("/");
                Serial.print(TRANSMIT_ITERATIONS);
                Serial.println(") ----");
                for (int i = 0; i < numCodes; i++) {
                    sendCode(codes[i] + 1);
                    // Toggle LED
                    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
                    delay(WAIT_TIME);
                }
                delay(STATE_WAIT_TIME);
            }
            Serial.println("------------- DONE --------------");
            Serial.println("LISTENING...");
            state = State::LISTENING;
        break;
    }
}
