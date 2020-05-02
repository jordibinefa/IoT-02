#include "IoT-02_common.h"
#include "IoT-02_modbus.h"
#include "IoT-02_bme280.h"

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

byte byReadInputs() {
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

void vReadHoldingRegisters(byte* ucSt, int len) {
  int i;
  short int nTx100, nRHx100, nPx100, nGr, nAx100;
  unsigned int uiCRC, adr = (ucSt[2] << 8) | ucSt[3]; // Adreça del primer registre
  unsigned int uiHowManyReg = (ucSt[4] << 8) | ucSt[5]; // Quants registres a llegir
  // ucSt[4] i ucSt[5] diuen el número de registres a llegir
  // ucSt[6] i ucSt[7] diuen quin es el CRC
  Serial.print("adr: "); Serial.println(adr, HEX);
  Serial.println("Reading sensor");
  // vReadingBME680(&nTx100,&nPx100,&nRHx100,&nGr,&nAx100);
  switch (adr) {
    case HOLDING_REGISTER_TEMEPRATURE_ADDRESS:
      switch (uiHowManyReg) {
        case 1:
          ucSt[2] = 0x02; // Bytes a trametre
          nTx100 = (short int)nTx100_bme();
          Serial.print("T: "); Serial.print((float)nTx100 / 100); Serial.println(" *C");
          // Serial.print("sizeof(int): "); Serial.println(sizeof(int));
          // Serial.print("sizeof(short int): "); Serial.println(sizeof(short int));
          ucSt[3] = nTx100 >> 8;
          ucSt[4] = nTx100 & 0x00FF;
          break;
        case 2:
          ucSt[2] = 0x04; // Bytes a trametre
          break;
        case 3:
          ucSt[2] = 0x06; // Bytes a trametre
          break;
        case 4:
          ucSt[2] = 0x08; // Bytes a trametre
          break;
        case 5:
          ucSt[2] = 0x0A; // Bytes a trametre
          break;
        default:
          return;
      }
      break;
    case HOLDING_REGISTER_REL_HUMIDITY_ADDRESS:
      switch (uiHowManyReg) {
        case 1:
          ucSt[2] = 0x02; // Bytes a trametre
          nRHx100 = (short int)nRHx100_bme();
          Serial.print("RH: "); Serial.print((float)nRHx100 / 100); Serial.println(" %");
          ucSt[3] = nRHx100 >> 8;
          ucSt[4] = nRHx100 & 0x00FF;
          break;
        case 2:
          ucSt[2] = 0x04; // Bytes a trametre
          break;
        case 3:
          ucSt[2] = 0x06; // Bytes a trametre
          break;
        case 4:
          ucSt[2] = 0x08; // Bytes a trametre
          break;
        default:
          return;
      }
      break;
    case HOLDING_REGISTER_PRESSURE_ADDRESS:
      switch (uiHowManyReg) {
        case 1:
          ucSt[2] = 0x02; // Bytes a trametre
          nPx100 = (short int)(nPx100_bme() / 100);
          Serial.print("P: "); Serial.print(nPx100); Serial.println(" hPa");
          ucSt[3] = nPx100 >> 8;
          ucSt[4] = nPx100 & 0x00FF;
          break;
        case 2:
          ucSt[2] = 0x04; // Bytes a trametre
          break;
        case 3:
          ucSt[2] = 0x06; // Bytes a trametre
          break;
        default:
          return;
      }
      break;
    case HOLDING_REGISTER_ALTITUDE_ADDRESS:
      switch (uiHowManyReg) {
        case 1:
          ucSt[2] = 0x02; // Bytes a trametre
          nAx100 = (short int)nAx100_bme( SEALEVELPRESSURE_HPA );
          Serial.print("Altitude: "); Serial.print((float)nAx100 / 100); Serial.println(" m");
          ucSt[3] = nAx100 >> 8;
          ucSt[4] = nAx100 & 0x00FF;
          break;
        case 2:
          ucSt[2] = 0x04; // Bytes a trametre
          break;
        default:
          return;
      }
      break;
    case HOLDING_REGISTER_VOC_ADDRESS:
      switch (uiHowManyReg) {
        case 1:
          ucSt[2] = 0x02; // Bytes a trametre
          nGr = (short int)nGr_bme();
          Serial.print("VOC: "); Serial.print(nGr); Serial.println(" Ohms");
          //Serial.print("VOC: "); Serial.print(nGr_bme()); Serial.println(" Ohms");
          ucSt[3] = nGr >> 8;
          ucSt[4] = nGr & 0x00FF;
          break;
        default:
          return;
      }
      break;
    default: return;
  }
  vModeTxRxRS485(RS485Transmit);
  //delay(10);
  for (i = 0 ; i < 3+ucSt[2] ; i++) {
    modbusData.write(ucSt[i]);
    //Serial.print(ucSt[i], HEX); Serial.print(" ");
  }
  uiCRC = uiModRTU_CRC(ucSt, 3+ucSt[2]);
  modbusData.write((byte)(uiCRC & 0xFF));
  modbusData.write((byte)(uiCRC >> 8));
  //Serial.print((byte)(uiCRC & 0xFF), HEX); Serial.print(" ");
  //Serial.print((byte)(uiCRC >> 8)); Serial.print(" ");
  delay(ENOUGH_TIME_SENDING_ANSWER); // Giving enough time sending answer
  vModeTxRxRS485(RS485Receive);
}

void vProcessa(byte* ucSt, int len) {
  unsigned int uiR, uiA;

  if (ucSt[0] == MODULE_ADDRESS) {
    switch (ucSt[1]) {
      case 0x02: // http://www.simplymodbus.ca/FC02.htm  Read Input Status (FC=02)
        //Serial.println("Lectura a esclau");
        vReadInputs(ucSt, len);
        break;
      case 0x03: // http://www.simplymodbus.ca/FC03.htm Read Holding Registers (FC=03)
        vReadHoldingRegisters(ucSt, len);
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
