#define EXTERNALS_H_

#include "LoraRelais.h"
#include "../Secrets/secrets.h"


const char *Node = "LR";

volatile float fExternalTemperature = 5.0;

SPI mySPI(0);
LoRaClass LoRa(&mySPI);

Communication cmulti(0,Node,5,true);
ComReceiver cmultiRec(&cmulti,Node,cnetCommands,NUM_COMMANDS,cnetInformation,NUM_INFORMATION,"W1",sendViaRelay);

volatile uint8_t txIsReady = false;
volatile uint8_t rxIsReady = false;

volatile int8_t rxRssi = 0;
char rxMessage[LORAMESSAGELENGTH];

uint8_t key[16] = {AES256_KEY};

char LoraCmultiBuffer[LORACMULTIBUFFERR_LENGTH];
Cmulti2Buffer loraCmulti(LoraCmultiBuffer,LORACMULTIBUFFERR_LENGTH,Node);

RelaisInfo relInfo[RELAISINFONUM];
uint8_t relInfoToSend   = 0;
uint8_t relInfoFinished = 0;
