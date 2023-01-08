#ifndef LORARELAIS_H_INCLUDED
#define LORARELAIS_H_INCLUDED


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
#include "Cmulti2Buffer_v02.h"
#include "loraCmulti.h"
#include "secrets.h"


void setup();


#endif // LORARELAIS_H_INCLUDED
