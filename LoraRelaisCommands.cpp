#include "LoraRelaisCommands.h"
#include "External.h"
#include "../Secrets/secrets.h"


COMMAND cnetCommands[NUM_COMMANDS] =
{
  cmultiStandardCommands

};

INFORMATION cnetInformation[NUM_INFORMATION]=
{
  {"tx",'C','1','T',FLOAT,1,(void*)&fExternalTemperature,NULL}
};

