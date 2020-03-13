/*







 */

#ifndef my_Pixy2_H_
#define my_Pixy2_H_

#include "common.h"
#include "my_i2c.h"

/**************Defines Here************************/

#define pixy2_ID         	 0x54

//#define SYSMOD             0x0B



#define PIXY_NO_CHECKSUM_SYNC                0xc1ae

#define PIXY_TYPE_REQUEST_CHANGE_PROG        0x02
#define PIXY_TYPE_REQUEST_RESOLUTION         0x0c
#define PIXY_TYPE_RESPONSE_RESOLUTION        0x0d
#define PIXY_TYPE_REQUEST_VERSION            0x0e
#define PIXY_TYPE_RESPONSE_VERSION           0x0f
#define PIXY_TYPE_RESPONSE_RESULT            0x01
#define PIXY_TYPE_RESPONSE_ERROR             0x03
#define PIXY_TYPE_REQUEST_BRIGHTNESS         0x10
#define PIXY_TYPE_REQUEST_SERVO              0x12
#define PIXY_TYPE_REQUEST_LED                0x14
#define PIXY_TYPE_REQUEST_LAMP               0x16
#define PIXY_TYPE_REQUEST_FPS                0x18
#define PIXY_TYPE_REQUEST_BLOCKS             0x20
#define PIXY_TYPE_RESPONSE_BLOCKS 			 0x21


#define PIXY_RESULT_OK                       0
#define PIXY_RESULT_ERROR                    -1
#define PIXY_RESULT_BUSY                     -2
#define PIXY_RESULT_CHECKSUM_ERROR           -3
#define PIXY_RESULT_TIMEOUT                  -4
#define PIXY_RESULT_BUTTON_OVERRIDE          -5
#define PIXY_RESULT_PROG_CHANGING            -6
#define PIXY_RESULT_LENGTH_ERROR             -7


/**************Global Variables Here************************/

struct block{
	uint16_t colorCode;	//0 - 255
	uint16_t x_pos;	   	//0 - 315
	uint16_t y_pos;	   	//0 - 207
	uint16_t x_len;		//0 - 316
	uint16_t y_len;		//0 - 208
	int16_t  angle;		//-180 - 180 (0 if not a color code)
	uint8_t index; 		//0 - 255
	uint8_t age;		//0 - 255 (stops incrementing at 255)
};

/******* Prototype Function declarations ******/

int16_t sendPacket(uint8_t type, int length, char *payload);
int16_t recvPacket(uint8_t length, char *buf);

int8_t getResolution(uint16_t *frameWidth, uint16_t *frameHeight);
int8_t getVersion(uint16_t *hardware, uint8_t *firmwareMajor, uint8_t *firmwareMinor, uint16_t *firmwareBuild);
int8_t setCameraBrightness(char brightness);
int8_t setLED(char r, char g, char b);
int8_t setLamp(char upper, char lower);
int8_t getFPS(int *fps);
int8_t getBlocks(uint8_t sigmap, uint8_t maxBlocks, struct block *my_block);

#endif //my_Pixy2_H_

