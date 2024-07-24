#include <Arduino.h>
#include "mymodbusRtu.h"
#include "ModbusServerRTU.h"
#include "upstype.h"
#include "megatec.h"


std::mutex dataAccessM;
uint8_t _mymodBusID=2;
extern ups_modbus_data_t ups_modbus_data;
//extern int T_Command(uint16_t minute);
// bool RTUutils::validCRC(const uint8_t *data, uint16_t len) { return validCRC(data, len - 2, data[len - 2] | (data[len - 1] << 8));
// }

// bool RTUutils::validCRC(const uint8_t *data, uint16_t len, uint16_t CRC) {
//   uint16_t crc16 = calcCRC(data, len);
//   if (CRC == crc16) return true;
//   return false;
// }
// uint16_t RTUutils::calcCRC(const uint8_t *data, uint16_t len) {
//   // CRC16 pre-calculated tables
//   const uint8_t crcHiTable[] = {
//     0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
//     0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
//     0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
//     0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
//     0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
//     0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
//     0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
//     0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
//     0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
//     0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
//     0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
//     0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
//     0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
//     0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
//     0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
//     0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
//     0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
//     0x40
//   };

//   const uint8_t crcLoTable[] = {
//     0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
//     0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
//     0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
//     0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
//     0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
//     0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
//     0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
//     0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
//     0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
//     0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
//     0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
//     0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
//     0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
//     0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
//     0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
//     0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
//     0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
//     0x40
//   };

//   uint8_t crcHi = 0xFF;
//   uint8_t crcLo = 0xFF;
//   uint8_t index;

//   while (len--) {
//     index = crcLo ^ *data++;
//     crcLo = crcHi ^ crcHiTable[index];
//     crcHi = crcLoTable[index];
//   }
//   return (crcHi << 8 | crcLo);
// }
// void modbusRtu::modbusInit()
// {
//   pinMode(OP_LED , OUTPUT);
//   Serial2.begin(9600,SERIAL_8N1,RX2_PIN ,TX2_PIN );//for 485
// }

// void modbusRtu::receiveData()
// {
//   enum STATES : uint8_t
//   {
//     WAIT_DATA = 0,
//     IN_PACKET,
//     DATA_READ,
//     FINISHED
//   };
//   const uint16_t BUFBLOCKSIZE(512);
//   uint8_t *buffer = new uint8_t[BUFBLOCKSIZE];
//   uint8_t state;
//   int b = 0;
//   uint16_t bufferPtr = 0;

//   unsigned long TimeOut = millis();
//   state = WAIT_DATA;
//   unsigned long lastMicros = micros();
//   while (state != FINISHED)
//   {
//     switch (state)
//     {
//     case WAIT_DATA:
//       if (!Serial2.available())
//         break;
//       b = Serial2.read();
//       if (b >= 0)
//       {
//         // Yes. Note the time.
//         lastMicros = micros();
//         buffer[bufferPtr++] = b ;
//         state = IN_PACKET;
//       }
//       else
//       {
//         state = FINISHED;
//       }
//       break;
//     case IN_PACKET:
//       while (state == IN_PACKET)
//       {
//         while (Serial2.available())
//         {
//           buffer[bufferPtr++] = Serial2.read();
//           // Mark time of last byte
//           lastMicros = micros();
//           // Buffer full?
//           if (bufferPtr >= BUFBLOCKSIZE)
//           {
//             // Yes. Something fishy here - bail out!
//             errorPacket(buffer,MEMORY_PARITY_ERROR);
//             state = FINISHED;
//             break;
//           }
//         }
//         if (state == IN_PACKET)
//         {
//           if (micros() - lastMicros >= (1.0/(float)Serial2.baudRate() )*1000.0*1000.0*100.0)
//           {
//             state = DATA_READ;
//             break;
//           }
//         }
//       }
//       break;
//     case DATA_READ:
//       if (bufferPtr >= 4)
//       {
//         if (!RTUutils::validCRC(buffer, bufferPtr))
//         {
//           int j = 0;
//           errorPacket(buffer,CRC_ERROR);
//         }
//         else
//         { // CRS was fine.
//             // for (uint16_t i = 0; i < bufferPtr; i++)
//             //   Serial.printf("%02x ", buffer[i]);
//             if(buffer[1]==0x03)
//               F03(buffer,bufferPtr);
//             else if(buffer[1]==0x04)
//               F04(buffer,bufferPtr);
//             else if(buffer[1]==0x06)
//               F06(buffer,bufferPtr);
//             else 
//               errorPacket(buffer,ILLEGAL_FUNCTION);
//           // for (uint16_t i = 0; i < bufferPtr; i++)
//           //   Serial.printf("%d:[%02x] ", i, buffer[i]);
//         }
//       }
//       state = FINISHED;
//       break;
//     case FINISHED:
//       while (Serial2.available())
//         Serial2.read();
//       break;
//     }
//   }
// }

// void modbusRtu::errorPacket(uint8_t *buffer,Error errorCode){
//   if(buffer[0]!=_modBusID)return;
//   byte writeBuf[6];
//   uint16_t data_pos=0;
//   uint16_t dataRequestAddress=0;
//   uint16_t requestDataLen=0;
//   writeBuf[data_pos++] = buffer[0]; //0
//   writeBuf[data_pos++] = 0x80+buffer[1]; //1 function 
//   writeBuf[data_pos++] = errorCode; 
//   uint16_t crc16 = RTUutils::calcCRC(writeBuf,data_pos );  // 이미 증가했기 때문에 data_pos는 정확한 숫자다.  
//   writeBuf[data_pos++] = crc16 & 0xff;//checksum 
//   writeBuf[data_pos++] = (crc16 >> 8) & 0xFF;// checksum 
//   writeBuf[data_pos]=0x00; 
//   digitalWrite(OP_LED, 1);
//   Serial2.write(writeBuf,data_pos);
//   delay(1);
//   Serial2.flush();
//   digitalWrite(OP_LED, 0);
// }

ModbusMessage  FC03(ModbusMessage request)
{
  ModbusMessage response;     // response message to be sent back
 
  uint16_t data_pos=0;
  uint16_t requestDataLen=0;
  uint16_t address;           // requested register address
  uint16_t words;             // requested number of registers
  request.get(2, address);
  request.get(4, words);

  //Serial.printf("\r\nRequest");
  if(address+words>60)
  {
    response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
  }
  response.add(request.getServerID(), request.getFunctionCode(), (uint8_t)(words * 2));
  uint16_t *modbusData =(uint16_t *)&ups_modbus_data;
  LOCK_GUARD(errorCntLock,dataAccessM); 
  for(int i=address;i<words+address;i++){
      response.add(modbusData[i]);  
  }

  return response;
};
ModbusMessage FC04(ModbusMessage request)
{
   ModbusMessage response;     // response message to be sent back
  // uint16_t buffer[256];
  //  ModbusMessage response;     // response message to be sent back
  // if(buffer[0]!=_mymodBusID)
  // {
  //   response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
  // }
  return response;

};
// void SendTestCommand(int minute){
//   T_Command(minute);
// }
ModbusMessage FC06(ModbusMessage request)
{
   ModbusMessage response;     // response message to be sent back
   return response;
  // uint16_t buffer[256];
  // if(buffer[0]!=_modBusID)return;
  // byte writeBuf[9];
  // uint16_t data_pos=0;
  // uint16_t dataRequestAddress=0;
  // uint16_t singleDataToWrite=0;

  // dataRequestAddress =   buffer[2]<<8 |buffer[3] ;// address 
  // singleDataToWrite=   buffer[4]<<8 |buffer[5] ;// length

  // if(dataRequestAddress >  61) {
  //   //errorPacket(buffer,ILLEGAL_DATA_ADDRESS);
  //   return;
  // }
  // //SendTestCommand(singleDataToWrite);
  // uint16_t *modbusData =(uint16_t *)&ups_modbus_data;
  // modbusData[dataRequestAddress] =singleDataToWrite; 
  // digitalWrite(OP_LED, 1);
  // Serial2.write(buffer,10);// bug
  // delay(1);
  // Serial2.flush();
  // digitalWrite(OP_LED, 0);
  // if(dataRequestAddress= 60)
  //   T_Command(singleDataToWrite);
};