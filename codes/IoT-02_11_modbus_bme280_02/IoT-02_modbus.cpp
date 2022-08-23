#include "IoT-02_common.h"
#include "IoT-02_modbus.h"
#include "IoT-02_bme280.h"

// Millora aconseguida a partir de https://pastebin.com/2PtWJvd6
// que li manca un free(data) al final del loop()

void vSetupModBus(int nBauds) {
  //configure UART_NUM_2 for RS485
  const uart_port_t uart_num = MODBUS_UART_PORT;
    uart_config_t uart_config = {
        .baud_rate = nBauds,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };
    // Configure UART parameters
    uart_param_config(uart_num, &uart_config);

    // Set UART2 pins(TX: IO17, RX: IO16, RTS: IO33, CTS: NC)
    uart_set_pin(uart_num, TX_MODBUS, RX_MODBUS, SSerialTxControl, UART_PIN_NO_CHANGE);

    // Install UART driver (we don't need an event queue here)
    // In this example we don't even use a buffer for sending data.
    uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0);

    // Set RS485 half duplex mode
    uart_set_mode(uart_num, UART_MODE_RS485_HALF_DUPLEX);
}

void vWriteCoil(byte* ucSt, int len) {
  const uart_port_t uart_num = MODBUS_UART_PORT;
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
    uart_write_bytes(uart_num, ucSt, len);
  }
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
  const uart_port_t uart_num = MODBUS_UART_PORT;
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
    uiCRC = uiModRTU_CRC(ucSt, 4);
    ucSt[4] = (byte)(uiCRC & 0xFF);
    ucSt[5] = (byte)(uiCRC >> 8);
    uart_write_bytes(uart_num, ucSt, 6);
    //Serial.print((byte)(uiCRC & 0xFF), HEX); Serial.print(" ");
    //Serial.print((byte)(uiCRC >> 8)); Serial.print(" ");
  }
  // Serial.println();
}

void vReadHoldingRegisters(byte* ucSt, int len) {
  const uart_port_t uart_num = MODBUS_UART_PORT;
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
  uiCRC = uiModRTU_CRC(ucSt, 3+ucSt[2]);
  ucSt[3+ucSt[2]] = (byte)(uiCRC & 0xFF);
  ucSt[3+ucSt[2]+1] = (byte)(uiCRC >> 8);
  uart_write_bytes(uart_num, ucSt, 3+ucSt[2]+2);
  //Serial.print((byte)(uiCRC & 0xFF), HEX); Serial.print(" ");
  //Serial.print((byte)(uiCRC >> 8)); Serial.print(" ");
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
  const uart_port_t uart_num = MODBUS_UART_PORT;
  int i, nCmpt = 0;
  unsigned int uiCRC;
  byte uiCrcL, uiCrcH;
  unsigned char ucSt[N_MAX];

  nCmpt = uart_read_bytes(uart_num, ucSt, N_MAX, PACKET_READ_TICS);
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

// Compute the MODBUS RTU CRC
// Adapted to Arduino from http://www.ccontrolsys.com/w/How_to_Compute_the_Modbus_RTU_Message_CRC
unsigned int uiModRTU_CRC(byte *buf, int len)
{
    unsigned int crc = 0xFFFF;

    for (int pos = 0; pos < len; pos++)
    {
        crc ^= (unsigned int)buf[pos]; // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--)
        { // Loop over each bit
            if ((crc & 0x0001) != 0)
            {              // If the LSB is set
                crc >>= 1; // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else           // Else LSB is not set
                crc >>= 1; // Just shift right
        }
    }
    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return crc;
}
