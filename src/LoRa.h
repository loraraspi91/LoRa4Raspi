// Copyright (c) Sandeep Mistry. All rights reserved.
// adapted to raspberry pi by Laurent Rioux Copyright (c) - mùarch 2020. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef LORA_H
#define LORA_H

#include <wiringPi.h>
#include "Print.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

#define SPI0			   0
#define SPI1			   1
#define LORA_DEFAULT_SPI           SPI0
#define LORA_DEFAULT_SPI_CE0       10 
#define LORA_DEFAULT_SPI_CE1       11 
#define LORA_DEFAULT_SPI_FREQUENCY 1E6
#define LORA_DEFAULT_RESET_PIN     6   // GPIO25 - to adapt according the board
#define LORA_DEFAULT_DIO0_PIN      24  // GPIO19  - to adapt according the board
#define LORA_DEFAULT_SS_PIN	   LORA_DEFAULT_SPI_CE0  // GPIO - to adapt according the SPI

#define LORA_DEFAULT_DIO1_PIN      27  // GPIO20  - to adapt according the board
#define LORA_DEFAULT_DIO2_PIN      29  // GPIO21 - to adapt according the board
#define LORA_DEFAULT_DIO5_PIN      25  // GPIO26  - to adapt according the board


#define PA_OUTPUT_RFO_PIN          0
#define PA_OUTPUT_PA_BOOST_PIN     1

// inline functions to enable & diable interrupts
void ENABLE_INTERRUPTS(void);
void DISABLE_INTERRUPTS(void);
int INTERRUPTS_ENABLED(void);


class LoRaClass : public iostream, public Print {
public:
  LoRaClass();


  int begin(long frequency, int SPI = 0);
  void end();

  int beginPacket(int implicitHeader = false);
  int endPacket(bool async = false);

  int parsePacket(int size = 0);
  int packetRssi();
  float packetSnr();
  long packetFrequencyError();
  
  int rssi();

  // from Print

  virtual size_t write(uint8_t byte);
  virtual size_t write(const uint8_t *buffer, size_t size);

  // from Stream
  virtual int available();
  virtual int read();
  virtual int peek();
  virtual void flush();

  void idle();
  void sleep();

  void setTxPower(int level, int outputPin = PA_OUTPUT_PA_BOOST_PIN);
  void setFrequency(long frequency);
  void setSpreadingFactor(int sf);
  void setSignalBandwidth(long sbw);
  void setCodingRate4(int denominator);
  void setPreambleLength(long length);
  void setSyncWord(int sw);
  void enableCrc();
  void disableCrc();
  void enableInvertIQ();
  void disableInvertIQ();
  void receive(int size=0);

  void setOCP(uint8_t mA); // Over Current Protection control
  void setGain(uint8_t gain); // Set LNA gain

  // deprecated
  void crc() { enableCrc(); }
  void noCrc() { disableCrc(); }

  uint8_t random();

  void setPins(int ss = LORA_DEFAULT_SS_PIN, int reset = LORA_DEFAULT_RESET_PIN, int dio0 = LORA_DEFAULT_DIO0_PIN);
  void setSPI(int SPI);
  void setSPIFrequency(uint32_t frequency);

  void dumpRegisters(ostream& out);

  void onReceive(void(*callback)(int));
  void onTxDone(void(*callback)());

private:
  void explicitHeaderMode();
  void implicitHeaderMode();

  void handleDio0Rise();
  bool isTransmitting();

  int getSpreadingFactor();
  long getSignalBandwidth();

  void setLdoFlag();

  uint8_t readRegister(uint8_t address);
  void writeRegister(uint8_t address, uint8_t value);
  uint8_t singleTransfer(uint8_t address, uint8_t value);
  int _spiSettings(int spi);

  static void onDio0Rise();

private: 
  uint32_t _spi_frequency;
  int _spi;
  int _ss;
  int _reset;
  int _dio0;
  long _frequency;
  int _packetIndex;
  int _implicitHeaderMode;
  void (*_onReceive)(int);
  void (*_onTxDone)();
};

extern LoRaClass LoRa;

#endif
