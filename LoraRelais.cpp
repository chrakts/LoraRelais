#include "LoraRelais.h"
#include "LoraRelaisCommands.h"

class LoRaClass;

uint32_t counter = 0;

void setup()
{
	init_clock(SYSCLK,PLL,true,CLOCK_CALIBRATION);

	PORTA_DIRSET = 0xf0; // 4x Eingang, dann 4x LEDs
	PORTB_DIRSET = 0x00;; // nichts angeschlossen
	PORTC_DIRSET = 0b00001011; // 4x LED, dann 4x Ausgang
	PORTD_DIRSET = 0b10111000; // nur RS232 an Pin6/7
	PORTE_DIRSET = 0x00; // nichts angeschlossen

	PORTA_OUTSET = 0xf0;

  uint8_t i,led;
	led = 0x10;
	for(i=0;i<=3;i++)
	{
		PORTA_OUT = ~led | 0x0f;
		_delay_ms(100);
		led = led<<1;
	}
	PORTA_OUT = 0xFF;
	//init_mytimer();

	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
	sei();
	cmulti.open(Serial::BAUD_57600,F_CPU);

  cmulti.sendInfo("Hello from LORA-Relais","BR");

  if (!LoRa.begin(868E6))
  {
    while(1)
    {
      LED_ROT_TOGGLE;
      _delay_ms(100);
    }
  }

  RTC.PERL = 3;
  RTC.PERH = 0;
  CLK.RTCCTRL = CLK_RTCSRC_ULP_gc | CLK_RTCEN_bm; // 1kHz ULP
  RTC.CTRL = RTC_PRESCALER_DIV1024_gc;
  RTC.INTCTRL = RTC_OVFINTLVL_LO_gc;

  loraCmulti.clearChecksum();
  //LoRa.onTxDone(onTxDone);
  LoRa.onReceive(onReceive);
  LoRa_rxMode();

}

int main()
{
  setup();


  while(1)
  {

    if(rxIsReady==true)
    {
      gotInfoFromLora();
      rxIsReady=false;
      LED_BLAU_ON;
    }
    processRelaisInfos();
    cmultiRec.comStateMachine();
    cmultiRec.doJob();

  }
}

void sendViaRelay(char *relayText)
{
  cmulti.sendStandardInt("PP",'P','P','P',strlen(relayText));
  string2RelaisInfo( &(relInfo[relInfoToSend]),relayText );
  relInfo[relInfoToSend].medium = VIA_LORA; // Funk
  relInfoToSend++;
  if(relInfoToSend>=RELAISINFONUM)
    relInfoToSend = 0;
  free(relayText);
  //_delay_ms(5);
}

void string2RelaisInfo(RelaisInfo *rinfo,const char *info)
{
// TTSSFAJETTTT....TTT
int8_t l;
  l = strlen(info);
  if( l>=8 )
  {
    strncpy(rinfo->target,&(info[0]),2);
    rinfo->target[2] = '\0';
    strncpy(rinfo->source,&(info[2]),2);
    rinfo->source[2] = '\0';
    rinfo->function = info[5];
    rinfo->address = info[6];
    rinfo->job = info[7];
    l-=8;
    if( (l>0) & (l<LORACMULTIBUFFERR_LENGTH-10) )
    {
      rinfo->endChar = info[8];
      strncpy(rinfo->content,&(info[9]),l);
      rinfo->content[l] = 0;
    }
    else
      rinfo->endChar = '?';
  }

}

void processRelaisInfos()
{
  if(relInfoToSend!=relInfoFinished)
  {
    switch(relInfo[relInfoFinished].medium)
    {
      case VIA_CNET:
        cmulti.setAlternativeNode(relInfo[relInfoFinished].source);
        cmulti.sendStandard(relInfo[relInfoFinished].content,
                            relInfo[relInfoFinished].target,
                            relInfo[relInfoFinished].function,
                            relInfo[relInfoFinished].address,
                            relInfo[relInfoFinished].job,
                            relInfo[relInfoFinished].endChar);
        cmulti.resetNode();
        LED_BLAU_OFF;
      break;
      case VIA_LORA:
        LED_GRUEN_ON;
        loraCmulti.clearChecksum();
        loraCmulti.sendStandard(relInfo[relInfoFinished].content,
                            relInfo[relInfoFinished].target,
                            relInfo[relInfoFinished].function,
                            relInfo[relInfoFinished].address,
                            relInfo[relInfoFinished].job,
                            relInfo[relInfoFinished].endChar);

       LoRa_sendMessage(&(loraCmulti.get()[1]));
       LED_GRUEN_OFF;

        //while(txIsReady==false)
        //{}
        LoRa_rxMode();
      break;
    }
    relInfoFinished++;
    if(relInfoFinished>=RELAISINFONUM)
      relInfoFinished = 0;
  }
}

void gotInfoFromLora()
{
  string2RelaisInfo( &(relInfo[relInfoToSend]),rxMessage );
  relInfo[relInfoToSend].medium = VIA_CNET; // Funk
  relInfoToSend++;
  if(relInfoToSend>=RELAISINFONUM)
    relInfoToSend = 0;

}


void LoRa_rxMode(){
  //LoRa.enableInvertIQ();                // active invert I and Q signals // war auskommentiert
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode
}

void LoRa_sendMessage(char *message) {
  txIsReady = false;
  //LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.write((uint8_t*)message,strlen(message));                  // add payload
  LoRa.endPacket(false);                 // finish packet and send it !!!!!!!!!!!!! war true
}

void onReceive(int packetSize)
{
  uint8_t cointer=0;
  while (LoRa.available()) {
    rxMessage[cointer] = (char)LoRa.read();
    cointer++;
    if (cointer>=LORAMESSAGELENGTH-1)
      cointer--;
  }
  rxMessage[cointer]=0;
  rxRssi = LoRa.packetRssi();
  rxIsReady=true;
}

void onTxDone() {
  txIsReady = true;
}


ISR ( RTC_OVF_vect )
{

}

