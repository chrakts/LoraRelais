#include "LoraRelais.h"
#include "LoraRelaisCommands.h"

class LoRaClass;

uint32_t counter = 0;

void setup()
{
	init_clock(SYSCLK,PLL,true,CLOCK_CALIBRATION);

	PORTA_DIRSET = 0xf0; // 4x Eingang, dann 4x LED's
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
  cmulti.sendInfo("\n\rHello from LORA-Relais-Neu","BR");

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

  loraCmulti.setChecksum();
  //LoRa.onTxDone(onTxDone);
  LoRa.onReceive(onReceive);
  LoRa_rxMode();

}

int main()
{
  setup();
  loraCmulti.setChecksum();
  while(1)
  {
    test4Received();
    processRelaisInfos(&cmulti);
    cmultiRec.comStateMachine();
    cmultiRec.doJob();
  }
}

ISR ( RTC_OVF_vect )
{

}

