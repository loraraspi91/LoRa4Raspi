#include "LoRa.h"
#include <stdio.h>

void onReceive(int packetSize) {
  // received a packet
   printf("Received packet '");

  // read packet
  for (int i = 0; i < packetSize; i++) {
     printf("%c",(char)LoRa.read());
  }

  // print RSSI of packet
  printf("' with RSSI ");
  printf("%d\n",LoRa.packetRssi());
}



void setup() {

  printf("LoRa Receiver\n");
  LoRa.setPins(10, 6, 24);
  
  if (!LoRa.begin(868E6, 0)) {
    printf("Starting LoRa failed!\n");
    while (1);
  }
  printf("Init LoRa Done !!\n");

// register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();

}

void loop() {
// do nothing
}


int  main(void) {
   setup();
   while(1) loop();
}
