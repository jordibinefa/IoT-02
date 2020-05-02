#include "IoT-02_common.h"
#include "IoT-02_modbus.h"

void vSetupModBus(int nBauds) {
  modbusData.begin(nBauds);          // U2_RXD:GPIO16, U2_TXD:GPIO17 (UART2)
  pinMode(SSerialTxControl, OUTPUT); // hardwired to GPIO_5
  vModeTxRxRS485(RS485Receive);
}

void vWriteCoil(byte* ucSt, int len) {
  int i;
  unsigned int uiCRC, adr = (ucSt[2] << 8) | ucSt[3]; // Adreça d'inici dels coils

  Serial.print("len: ");
  Serial.println(len);
  if (adr == COIL_RELAY_ADDRESS) {
    vLatchingRelay(ucSt[4] == 0xFF);
    if (ucSt[4] == 0xFF) {
      Serial.println("Relé SET");
    } else
      Serial.println("Relé RESET");
    vModeTxRxRS485(RS485Transmit);
    for (i = 0 ; i < len ; i++) { // echo
      modbusData.write(ucSt[i]);
    }
  }
  delay(ENOUGH_TIME_SENDING_ANSWER); // Giving enough time sending answer
  vModeTxRxRS485(RS485Receive);
}

byte byReadInputs(){
  byte byAnswer = 0x00;

  byAnswer |= (digitalRead(BT_IO0)) ? 0x01 : 0x00;
  byAnswer |= (digitalRead(BT_I34)) ? 0x02 : 0x00;
  byAnswer |= (digitalRead(BT_I35)) ? 0x04 : 0x00;
  byAnswer |= (digitalRead(STATE_LATCHING_RELAY)) ? 0x08 : 0x00;

  return byAnswer;
}

void vReadInputs(byte* ucSt, int len) {
  int i;
  unsigned int uiCRC, adr = (ucSt[2] << 8) | ucSt[3]; // Adreça d'inici dels coils
  // ucSt[4] i ucSt[5] diuen quants coils escriure
  // ucSt[6] diuen quants bytes de dades s'escriuen
  // ucSt[7] i ucSt[8] diuen quines dades s'escriuen
  // ucSt[9] i ucSt[10] diuen quin es el CRC
  //Serial.print("adr: "); Serial.println(adr, HEX);
  if (adr == COIL_INPUTS_ADDRESS) {
    ucSt[2] = 0x01; // Bytes a trametre
    ucSt[3] = byReadInputs();
    vModeTxRxRS485(RS485Transmit);
    //delay(10);
    for (i = 0 ; i < 4 ; i++) {
      modbusData.write(ucSt[i]);
      //Serial.print(ucSt[i], HEX); Serial.print(" ");
    }
    uiCRC = uiModRTU_CRC(ucSt, 4);
    modbusData.write((byte)(uiCRC & 0xFF));
    modbusData.write((byte)(uiCRC >> 8));
    //Serial.print((byte)(uiCRC & 0xFF), HEX); Serial.print(" ");
    //Serial.print((byte)(uiCRC >> 8)); Serial.print(" ");
    delay(ENOUGH_TIME_SENDING_ANSWER); // Giving enough time sending answer
    vModeTxRxRS485(RS485Receive);
  }
  Serial.println();
}

void vProcessa(byte* ucSt, int len) {
  unsigned int uiR, uiA;

  if (ucSt[0] == MODULE_ADDRESS) {
    switch (ucSt[1]) {
      case 0x02: // http://www.simplymodbus.ca/FC02.htm  Read Input Status (FC=02)
        //Serial.println("Lectura a esclau");
        vReadInputs(ucSt, len);
        break;
      case 0x05: // http://www.simplymodbus.ca/FC05.htm Force Single Coil (FC=05)
        //Serial.println("Escriptura a coil");
        vWriteCoil(ucSt, len);
        //vReadInputs(ucSt, len);
        break;
      case 0x0F: // http://www.simplymodbus.ca/FC15.htm Force Multiple Coils (FC=15)
        //Serial.println("Escriptura a esclau");
        //vEscriuCoils(ucSt, len);
        //vPresentaPantallaDemo(byState);
        break;
        //default: //Serial.println("Funcio no implementada");
    }
  }
}

void vModBusReading() {
  int i, nCmpt = 0;
  unsigned int uiCRC;
  byte uiCrcL, uiCrcH;
  unsigned char ucSt[N_MAX];

  while (modbusData.available() > 0) {
    ucSt[nCmpt] = (unsigned char)modbusData.read();
    nCmpt++;
    delay(2);
  }
  if (nCmpt) {
    Serial.print("He llegit aquests "); Serial.print(nCmpt); Serial.println(" bytes: ");
    for (i = 0; i < nCmpt; i++) {
      Serial.print(ucSt[i], HEX); Serial.print(" ");
    }
    Serial.println();
    uiCRC = uiModRTU_CRC(ucSt, nCmpt - 2);
    if ((byte)(uiCRC >> 8) == ucSt[nCmpt - 1] && (byte)(uiCRC & 0xFF) == ucSt[nCmpt - 2]) {
      Serial.println("Trama amb CRC correcte");
      vProcessa(ucSt, nCmpt);
    } else {
      Serial.println("Trama amb CRC incorrecte");
    }
    //Serial.print("Entrada: "); Serial.println(byReadButtons(), HEX);
    //Serial.print("byState: "); Serial.println(byState, HEX);
  }
  nCmpt = 0;
}
