/*
  LoRa Duplex communication

  Sends a message every half second, and polls continually
  for new incoming messages. Implements a one-byte addressing scheme,
  with 0xFF as the broadcast address.

  Uses readString() from Stream class to read payload. The Stream class'
  timeout may affect other functuons, like the radio's callback. For an

  created 28 April 2017
  by Tom Igoe and adapted by Laurent Rioux (March 2020)
*/

#include "LoRa.h"
#include "WString.h"
#include <stdio.h>
#include <time.h>

const int csPin = 10;          // LoRa radio chip select
const int resetPin = 6;       // LoRa radio reset
const int irqPin = 24;         // change for your board; must be a hardware interrupt pin

String outgoing;              // outgoing message

uint8_t msgCount = 0;            // count of outgoing messages
uint8_t localAddress = 0xBB;     // address of this device
uint8_t destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
unsigned int interval = 2000;          // interval between sends



void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}


void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  uint8_t sender = LoRa.read();            // sender address
  uint8_t incomingMsgId = LoRa.read();     // incoming msg ID
  uint8_t incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
      printf("error: message length does not match length\n");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
      printf("This message is not for me.\n");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  cout << "Received from: 0x" <<  String(sender, HEX) << endl;
  cout << "Sent to: 0x" << String(recipient, HEX) << endl;
  cout << "Message ID: " << String(incomingMsgId) << endl;
  cout << "Message length: " << String(incomingLength) <<endl;
  cout << "Message: " << incoming << endl;
  cout << "RSSI: " <<  String(LoRa.packetRssi()) << endl;
  cout << "Snr: " << String(LoRa.packetSnr())<< endl;
  std::cout << endl;
}


void setup() {

   printf("LoRa Duplex\n");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);   // set CS, reset, IRQ pin

  if (!LoRa.begin(868E6, 0)) {
    printf("Starting LoRa failed!\n");
    while (1);
  }
  printf("LoRa init succeeded.\n");
 
  /* seed the randaom */
  srand (time(NULL));
}

void loop() {
  if (millis() - lastSendTime > interval) {
    String message = "HeLoRa World!";   // send a message
    sendMessage(message);
    cout << "Sending " << message << endl;
    lastSendTime = millis();            // timestamp the message
    interval = (rand() % 2000) + 1000;    // 1-3 seconds
  }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());

}


int  main(void) {
   setup();
   while(1) loop();
}
