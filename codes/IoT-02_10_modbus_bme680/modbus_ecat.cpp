#include "modbus_ecat.h"

extern HardwareSerial modbusData;
extern unsigned char ucLastMbMsg[N_MAX];
extern int nMbLastLen;
extern uint32_t uiMbTimeOut;
extern bool bMbAnswer;
extern String szDarreraPeticio;
unsigned char ucStG[N_MAX]; 

void vModeTxRxRS485(boolean bMode)
{
    digitalWrite(SSerialTxControl, bMode);
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

void vModbusTx(unsigned char *uc, int nLen)
{
    int i;
    
    //Serial.println("Dins vModbusTx()");

    for (i = 0; i < nLen; i++)
    {
        ucLastMbMsg[i] = uc[i];
         Serial.print(uc[i], HEX);
         //Serial.print(uc[i], DEC);
         Serial.print(" ");
    }
    Serial.println();
    nMbLastLen = nLen;

    vModeTxRxRS485(RS485Transmit);
    delay(1);
    //modbusData.println("vModbusTx()");
    for (i = 0; i < nLen; i++)
    {
        modbusData.write(uc[i]);
    }
    delay(20); // 20 at 4800baus and 2 at 115200

    vModeTxRxRS485(RS485Receive);
    bMbAnswer = false;
    uiMbTimeOut = millis() + TIMEOUT;
}

void modbusFlush()
{
    char t;

    while (modbusData.available() > 0)
    {
        t = modbusData.read();
    }
}

int str2hex(char *s)
{
    int x = 0;
    for (;;)
    {
        char c = *s;
        if (c >= '0' && c <= '9')
        {
            x *= 16;
            x += c - '0';
        }
        else if (c >= 'A' && c <= 'F')
        {
            x *= 16;
            x += (c - 'A') + 10;
        }
        else
            break;
        s++;
    }
    return x;
}

void vPeticioModBus(String sz)
{
    unsigned char ucSt[N_MAX];
    String szSubStr;
    char charBuf[3];
    int i, nMida = sz.length() / 2;

    // Serial.println("Petició a ModBus:");
    // Serial.println(sz);
    // Serial.print("Mida: ");
    // Serial.println(nMida);
    // str2hex
    for (i = 0; i < nMida; i++)
    {
        szSubStr = sz.substring(2 * i, 2 + 2 * i);
        szSubStr.toCharArray(charBuf, 3);
        ucSt[i] = str2hex(charBuf);
        // Serial.print("ucSt[");
        // Serial.print(i);
        // Serial.print("] = ");
        // Serial.println(ucSt[i], HEX);
    }
    unsigned int uiCRC = uiModRTU_CRC(ucSt, nMida);

    ucSt[nMida] = (byte)(uiCRC & 0xFF);
    ucSt[nMida + 1] = (byte)(uiCRC >> 8);

    vModbusTx(ucSt, nMida + 2);
    szDarreraPeticio = sz;
}

short int siBytes2shortInt(byte *b)
{
    union uSI uB;

    uB.by[1] = b[0];
    uB.by[0] = b[1];

    return uB.nV;
}

long int liBytes2longInt(byte *b)
{
    union uLI uW;

    uW.by[1] = b[0];
    uW.by[0] = b[1];
    uW.by[3] = b[2];
    uW.by[2] = b[3];

    return uW.nV;
}

bool bReadingModBus(char *szResposta)
{
    int i, nCmpt = 0;
    unsigned int uiCRC;
    //unsigned char ucSt[N_MAX]; // Posat com a global al .h
    char szByte[3];
    bool bResposta = false;
    strcpy(szResposta, "");

    while (modbusData.available() > 0)
    {
        ucStG[nCmpt] = (unsigned char)modbusData.read();
        nCmpt++;
        delay(5);
        bMbAnswer = true;
    }
    if (nCmpt)
    {
        // Serial.print("He llegit aquests ");
        // Serial.print(nCmpt);
        // Serial.println(" bytes: ");
        for (i = 0; i < nCmpt; i++)
        {
            sprintf(szByte, "%02X", ucStG[i]);
            strcat(szResposta, szByte);
            // Serial.print(ucSt[i], HEX);
            // // Serial.print(" ");
        }
        //Serial.println();
        uiCRC = uiModRTU_CRC(ucStG, nCmpt - 2);
        if ((byte)(uiCRC >> 8) == ucStG[nCmpt - 1] && (byte)(uiCRC & 0xFF) == ucStG[nCmpt - 2])
        {
            Serial.println("Trama amb CRC correcte");

        }
        else
        {
             Serial.println("Trama amb CRC incorrecte");
            // Serial.print("Torno a demanar ");
            // Serial.println(szDarreraPeticio);
            modbusFlush();
            vPeticioModBus(szDarreraPeticio);
        }
    }
    else
    {
        if (!bMbAnswer)
        {
            if (millis() > uiMbTimeOut)
            {
                //Serial.println("ModBus TIMEOUT detected !!!!!!!!!!!!");
                modbusFlush();
                vModbusTx(ucLastMbMsg, nMbLastLen);
            }
        }
    }
    nCmpt = 0;

    return bResposta;
}
