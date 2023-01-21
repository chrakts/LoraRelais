#define EXTERNALS_H_

#include "LoraRelais.h"
#include "../Secrets/secrets.h"
#include "loraCmulti.h"

const char *Node = "LR";

volatile float fExternalTemperature = 5.0;

ComReceiver cmultiRec(&cmulti,Node,cnetCommands,NUM_COMMANDS,cnetInformation,NUM_INFORMATION,"W1",sendViaRelay);

SPI mySPI(0);
LoRaClass LoRa(&mySPI);

Communication cmulti(0,Node,5,true);
