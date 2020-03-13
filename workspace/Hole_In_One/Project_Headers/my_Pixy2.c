/*






 */

#include "my_Pixy2.h"


/**************Function Definitions Here************************/

int16_t sendPacket(uint8_t type, int length, char *payload){
  char buf[16];
  int i;
  // write header info to buffer   
  buf[0] = PIXY_NO_CHECKSUM_SYNC&0xff;//16-bit sync
  buf[1] = PIXY_NO_CHECKSUM_SYNC>>8;//16-bit sync
  buf[2] = type; //Type of packet
  buf[3] = length;//length of payload
  
  //Write payload to buffer
  for(i = 0; i < length; i++){
	  buf[i+4] = payload[i];
  }
  
  if(I2C0WriteMultiE(pixy2_ID, length+4, buf) == PIXY_RESULT_OK){
	  return PIXY_RESULT_OK;
  }
  else{
	  init_I2C0_PTC8_9();
	  return PIXY_RESULT_ERROR;
  }
}//End of sendPacket()

///////////////////////////////////////////////////////////////////////

int16_t recvPacket(uint8_t length, char *buf){
  uint16_t csCalc = 0, csSerial;

  if(I2C0ReadMultiE(pixy2_ID, length, buf) == -1){//Get data from Pixy2
	  init_I2C0_PTC8_9();
	  return PIXY_RESULT_ERROR; 
  }
  
  csSerial = (buf[5]<<8)|buf[4];//Get check sum bytes 
  
  //Verify length
  if(length-6 != buf[3]){
	if(csSerial)
		return PIXY_RESULT_LENGTH_ERROR;//bit stream error 
	else
		return PIXY_RESULT_BUSY;//Pixy is sending 0s. No data to send.
  }
  
  int i;
  for(i = 6; i < length; i++){
	csCalc += buf[i];//Sum up bytes in payload. 
  }
  
  if (csSerial!=csCalc){
    return PIXY_RESULT_CHECKSUM_ERROR;//bit stream error
  }	

  return PIXY_RESULT_OK;//All good
}//End of recvPacket

///////////////////////////////////////////////////////////////////////

int8_t getResolution(uint16_t *frameWidth, uint16_t *frameHeight){
  char recv_buf[10];
  char payload[1];
  
  payload[0] = 0x00;//Payload: Type (unused - reserved for future versions)
  sendPacket(PIXY_TYPE_REQUEST_RESOLUTION, 1, payload);
  
  if (recvPacket(10, recv_buf) == PIXY_RESULT_OK){   
      *frameWidth = (recv_buf[7]<<8)|recv_buf[6]; 
      *frameHeight = (recv_buf[9]<<8)|recv_buf[8];
      return PIXY_RESULT_OK; // success
  }
  else
    return PIXY_RESULT_ERROR;  // some kind of bitstream error
}//End getResolution

///////////////////////////////////////////////////////////////////////

int8_t getVersion(uint16_t *hardware, uint8_t *firmwareMajor, uint8_t *firmwareMinor, uint16_t *firmwareBuild){
  char buf[13];
  sendPacket(PIXY_TYPE_REQUEST_VERSION, 0, 0);
  if (recvPacket(13, buf) == PIXY_RESULT_OK || 1)
  {   
    *hardware = (buf[7]<<8)|buf[6];
	*firmwareMajor = buf[8];
	*firmwareMinor = buf[9];
	*firmwareBuild = (buf[11]<<8)|buf[10];
	//specs.firmwareType? come back to.
    return PIXY_RESULT_OK;
  }
  return PIXY_RESULT_ERROR;  // some kind of bitstream error
}//End getVersion

///////////////////////////////////////////////////////////////////////

int8_t setCameraBrightness(char brightness){
  char recv_buf[10];
  char payload = brightness;
  
  sendPacket(PIXY_TYPE_REQUEST_BRIGHTNESS, 1, &payload);

  if (recvPacket(10, recv_buf) == PIXY_RESULT_OK){   
      return PIXY_RESULT_OK; // success
  }
  else
    return PIXY_RESULT_ERROR;  // some kind of bitstream error
}//End setCameraBrightness()

///////////////////////////////////////////////////////////////////////

int8_t setLED(char r, char g, char b){
  char recv_buf[10];
  char payload[3];
  
  payload[0] = r;
  payload[1] = g;
  payload[2] = b;
  sendPacket(PIXY_TYPE_REQUEST_LED , 3, payload);
  int8_t res = recvPacket(10, recv_buf);
  if (res == PIXY_RESULT_OK){   
      return PIXY_RESULT_OK; // success
  }
  else
    return res;  // some kind of bitstream error
}//End setLED()

///////////////////////////////////////////////////////////////////////

int8_t setLamp(char upper, char lower){
  char recv_buf[10];
  char payload[2];
  
  payload[0] = upper;//Payload: upper two LEDs. 0 (off) or 1 (on)
  payload[1] = lower;//Payload: RGB LED. 0 (off) or 1 (on)
  sendPacket(PIXY_TYPE_REQUEST_LAMP, 2, payload);

  if (recvPacket(10, recv_buf) == PIXY_RESULT_OK){   
      return PIXY_RESULT_OK; // success
  }
  else
    return PIXY_RESULT_ERROR;  // some kind of bitstream error
}//End setLamp

///////////////////////////////////////////////////////////////////////

int8_t getFPS(int *fps){
  char recv_buf[10];
  
  sendPacket(PIXY_TYPE_REQUEST_FPS, 0, 0);

  if (recvPacket(10, recv_buf) == PIXY_RESULT_OK){   
      int buf[4] = {recv_buf[6],recv_buf[7],recv_buf[8],recv_buf[9]}; 
	  *fps = buf[3]<<24|buf[2]<<16|buf[1]<<8|buf[0];
      return PIXY_RESULT_OK; // success
  }
  else
    return PIXY_RESULT_ERROR;  // some kind of bitstream error
}//End getFPS

///////////////////////////////////////////////////////////////////////

int8_t getBlocks(uint8_t sigmap, uint8_t maxBlocks, struct block *my_block){
  char buf[20];
  char payload[2];
  int16_t res;
  payload[0] = sigmap; //0 (none) - 255 (all)
  payload[1] = maxBlocks; //0 (none) - 255 (all blocks)
  
  sendPacket(PIXY_TYPE_REQUEST_BLOCKS, 2, payload);
  res = recvPacket(20, buf);
  
  if (res == PIXY_RESULT_OK || res == PIXY_RESULT_LENGTH_ERROR)
  {   
    if(buf[2] == PIXY_TYPE_RESPONSE_BLOCKS){
    	my_block->colorCode = (buf[7]<<8)|buf[6];
        my_block->x_pos =     (buf[9]<<8)|buf[8];
        my_block->y_pos = 	  (buf[11]<<8)|buf[10];
        my_block->x_len = 	  (buf[13]<<8)|buf[12];
        my_block->y_len =     (buf[15]<<8)|buf[14];
        my_block->angle = 	  (buf[16]<<8)|buf[17];
        my_block->index =      buf[18];
        my_block->age =        buf[19];
        return PIXY_RESULT_OK;
    }
    else{
      return PIXY_RESULT_BUSY; // new data not available yet
    }
  }
  else if(res == PIXY_RESULT_BUSY){
  	return PIXY_RESULT_BUSY;
  }
  else{
	 return PIXY_RESULT_ERROR;  // some kind of bitstream error  
  }
}//End getBlocks
