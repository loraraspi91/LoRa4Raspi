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

  // Uncomment the next line to disable the default AGC and set LNA gain, only values between 1 - 6 are supported
  // LoRa.setGain(6);


// register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();

}

void loop() {
// just waiting onReceive callback; nothing done here
sleep(60000);
}


int  main(void) {
   setup();
   while(1) loop();
}
