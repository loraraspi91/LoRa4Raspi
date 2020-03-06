/*
  LoRa Simple Gateway/Node Exemple

  This code uses InvertIQ function to create a simple Gateway/Node logic.

  Gateway - Sends messages with enableInvertIQ()
          - Receives messages with disableInvertIQ()

  Node    - Sends messages with disableInvertIQ()
          - Receives messages with enableInvertIQ()

  With this arrangement a Gateway never receive messages from another Gateway
  and a Node never receive message from another Node.
  Only Gateway to Node and vice versa.

  This code receives messages and sends a message every second.

  InvertIQ function basically invert the LoRa I and Q signals.

  See the Semtech datasheet, http://www.semtech.com/images/datasheet/sx1276.pdf
  for more on InvertIQ register 0x33.

  created 05 August 2018
  by Luiz H. Cassettari and adapted by L. Rioux for raspeberry pi (march 2020)
*/

#include "LoRa.h"

const long frequency = 868E6;  // LoRa Frequency

const int csPin = 10;          // LoRa radio chip select
const int resetPin = 6;       // LoRa radio reset
const int irqPin = 24;         // change for your board; must be a hardware interrupt pin

void LoRa_rxMode(){
  LoRa.disableInvertIQ();               // normal mode
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.enableInvertIQ();                // active invert I and Q signals
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }

  cout << "Node Receive: " << message << endl;
}

void onTxDone() {
  printf("TxDone\n");
  LoRa_rxMode();
}


bool runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}

void setup() {

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency)) {
     printf("LoRa init failed. Check your connections.\n");
    while (true);                       // if failed, do nothing
  }

  printf("LoRa init succeeded.\n\n");
  printf("LoRa Simple Node\n");
  printf("Only receive messages from gateways\n");
  printf("Tx: invertIQ disable\n");
  printf("Rx: invertIQ enable\n\n");

  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();
}


void loop() {
  if (runEvery(5000)) { // repeat every 5000 millis

    String message = "HeLoRa World! ";
    message += "I'm a Node ! ";
    message += millis();
   cout << "Message to send: " << message << endl;

    LoRa_sendMessage(message); // send a message

    printf("Send Message!\n");
  }
}


int  main(void) {
   setup();
   while(1) loop();
}
