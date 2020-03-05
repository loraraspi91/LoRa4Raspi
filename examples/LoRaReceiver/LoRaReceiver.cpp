#include "LoRa.h"
#include <stdio.h>
int counter = 0;

void setup() {

  printf("LoRa Receiver\n");
  LoRa.setPins(10, 6, 24);
  
  if (!LoRa.begin(868E6, 0)) {
    printf("Starting LoRa failed!\n");
    while (1);
  }
  printf("Init LoRa Done !!\n");
}

void loop() {
// try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
     printf("Received packet '");

    // read packet
    while (LoRa.available()) {
      printf("%c", (char)LoRa.read());
    }

    // print RSSI of packet
     printf("' with RSSI ");
     printf("%d\n",LoRa.packetRssi());
    
  }
}


int  main(void) {
   setup();
   while(1) loop();
}
