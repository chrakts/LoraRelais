#ifndef LORARELAIS_H_INCLUDED
#define LORARELAIS_H_INCLUDED

#define RELAISINFONUM 5
#define LORAMESSAGELENGTH 30


#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "myConstants.h"

#include "Serial.h"
#include "External.h"
#include "../Secrets/secrets.h"
#include "Communication.h"
#include "ComReceiver.h"
#include "cmultiStandardCommands.h"
#include "LoraRelaisCommands.h"
#include "../xmegaClocks/xmegaClocks.h"
#include "LoRa.h"
#include "Cmulti2Buffer.h"

struct relaisInfo
{
  char   target[3];
  char   source[3];
  char   function;
  char   address;
  char   job;
  char   endChar;
  char   content[LORAMESSAGELENGTH];
  uint8_t medium;
};
typedef struct relaisInfo RelaisInfo;

void processRelaisInfos();
void sendViaRelay(char *relayText);
void string2RelaisInfo(RelaisInfo *rinfo,const char *info);
void setup();
void onTxDone();
void onReceive(int packetSize);
void LoRa_sendMessage(char *message);
void LoRa_txMode();
void LoRa_rxMode();
void evaluate(void);
void gotInfoFromLora();

enum{VIA_CNET=1,VIA_LORA};
extern RelaisInfo relInfo[RELAISINFONUM];
extern uint8_t relInfoToSend;
extern uint8_t relInfoFinished;

#endif // LORARELAIS_H_INCLUDED
