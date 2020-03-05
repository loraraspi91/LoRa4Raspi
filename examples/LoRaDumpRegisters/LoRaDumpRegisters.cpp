#include "LoRa.h"
#include <stdio.h>

void setup() {

  printf("LoRa Dump Registers\n");

  LoRa.setPins(10, 6, 24);
  if (!LoRa.begin(868E6)) {         // initialize ratio at 868 MHz
       printf("LoRa init failed. Check your connections.\n");
    while (true);                   // if failed, do nothing
  }

    LoRa.dumpRegisters(cout);

}

void loop() {
}


int  main(void) {
   setup();
   while(1) loop();
}
