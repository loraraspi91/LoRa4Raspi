#include "LoRa.h"
#include <stdio.h>
int counter = 0;

void onTxDone() {
  cout << "TxDone\n";
}

void setup() {

  printf("LoRa Sender\n");
  LoRa.setPins(10, 6, 24); // put here your correct pins configuration
  
  if (!LoRa.begin(868E6, 0)) {
    printf("Starting LoRa failed!\n");
    while (1);
  }
  LoRa.onTxDone(onTxDone);
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


void loop() {

  if (runEvery(1000)) { // repeat every 1000 millis

    printf("Sending packet non-blocking: %d\n", counter);
 
    // send in async / non-blocking mode
    LoRa.beginPacket();
    LoRa.print("hello ");
    LoRa.print(counter);
    LoRa.endPacket(true); // true = async / non-blocking mode

    counter++;
  }
}


int  main(void) {
   setup();
   while(1) loop();
}
