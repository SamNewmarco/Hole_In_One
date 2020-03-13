/*Note from developer
Hello Mauricio,

 

I am giving a software lab course at the University. As students are learning the basics of low-level programming (register level), 
I do not use Processor Expert in this course. The final experiment is accessing the accelerometer through I2C, so I have developed 
a library for this purpose, derived from an example library for Kinetis Family, which I simplified and suited for my needs. I am 
sending the .h and .c files attached to this reply.

For controlling the accelerometer, you just have to write the right values to the right config registers. Remember that the 
accelerometer address (SlaveID) is 0x1D.
If you just want to read raw (unfiltered) data from XYZ axes, using a full scale range of 2g, you may initialize the accelerometer using this code:

I2CWriteRegister(0x1D, 0x0E, 0x00); // Writes 0x00 to data config register (0x0E).
I2CWriteRegister(0x1D, 0x2A, 0x39); // Writes 0x39 to general config register 1 (0x2A)

Now the accelerometer starts working. It will make a new read (all 3 axes) every 640ms. If you want other periods, you just 
have to write the right value on 0x2A register, instead of 0x39. For 160ms, write 0x31; for 80ms, write 0x29; for 20ms, write 0x21.
You may read the latest collected values this way:

I2CReadMultiRegisters(0x1D, 0x01, 6, data); // Reads 6 sequential registers starting from MSB of X axis (0x01). "data" is a 6-element array of char.

The result will be the values of 3 axes loaded on "data" array: [MSB_X, LSB_X, MSB_Y, LSB_Y, MSB_Z, LSB_Z]. You may also test if new data is 
available before reading using:

dr = (I2CReadRegister(0x1D, STATUS) & 0x08); // dr is zero if no new data is available; it is 0x08 if new data is available.
I hope that it helps. Just message me if you need something else.
 

Antonio

I2C0_SCL PTE0
I2C0_SDA PTE1

*/
#ifndef my_i2c_H_
#define my_i2c_H_

#include "common.h"

/* Defines and enums */

#define MWSR                   0x00  /* Master write  */
#define MRSW                   0x01  /* Master read */

/********************** I2C0 *****************************/

#define i2c0_DisableAck()       I2C0_C1 |= I2C_C1_TXAK_MASK
#define i2c0_EnableAck()        I2C0_C1 &= ~I2C_C1_TXAK_MASK
#define i2c0_RepeatedStart()    I2C0_C1 |= I2C_C1_RSTA_MASK
#define i2c0_EnterRxMode()      I2C0_C1 &= ~I2C_C1_TX_MASK
#define i2c0_write_byte(data)   I2C0_D = data

#define i2c0_Start()            I2C0_C1 |= I2C_C1_TX_MASK;\
                               I2C0_C1 |= I2C_C1_MST_MASK

#define i2c0_Stop()             I2C0_C1 &= ~I2C_C1_MST_MASK;\
                               I2C0_C1 &= ~I2C_C1_TX_MASK

#define i2c0_Wait()               while((I2C0_S & I2C_S_IICIF_MASK)==0) {} \
                                  I2C0_S |= I2C_S_IICIF_MASK;

int wait_cnt;
#define i2c0_Wait_E()               wait_cnt = 0;\
									while((I2C0_S & I2C_S_IICIF_MASK)==0) \
									{if(wait_cnt > 5000){I2C0_S |= I2C_S_IICIF_MASK;i2c0_Stop();Pause();return -1;}else {wait_cnt++;}}\
									I2C0_S |= I2C_S_IICIF_MASK;

/********************** I2C1 *****************************/

#define i2c1_DisableAck()       I2C1_C1 |= I2C_C1_TXAK_MASK
#define i2c1_EnableAck()        I2C1_C1 &= ~I2C_C1_TXAK_MASK
#define i2c1_RepeatedStart()    I2C1_C1 |= I2C_C1_RSTA_MASK
#define i2c1_EnterRxMode()      I2C1_C1 &= ~I2C_C1_TX_MASK
#define i2c1_write_byte(data)   I2C1_D = data

#define i2c1_Start()            I2C1_C1 |= I2C_C1_TX_MASK;\
                               I2C1_C1 |= I2C_C1_MST_MASK

#define i2c1_Stop()             I2C1_C1 &= ~I2C_C1_MST_MASK;\
                               I2C1_C1 &= ~I2C_C1_TX_MASK

#define i2c1_Wait()               while((I2C1_S & I2C_S_IICIF_MASK)==0); \
                                  I2C1_S |= I2C_S_IICIF_MASK;

#define i2c1_Wait_E()               wait_cnt = 0;\
									while((I2C1_S & I2C_S_IICIF_MASK)==0) \
									{if(wait_cnt > 5000){i2c0_Stop();Pause();return -1;}else {wait_cnt++;}}\
									I2C1_S |= I2C_S_IICIF_MASK;
/********************i2c0  routines ******************************************/


void init_I2C0(void);
void IIC0_StartTransmission (char , char );
void Pause(void);
void init_I2C0_PTC8_9(void);

char I2C0ReadRegister(char , char );
void I2C0WriteRegister(char , char , char );
void I2C0ReadMultiRegisters(char , char , char , char * );
/********************i2c1  routines ******************************************/
void init_I2C1(void);
void IIC1_StartTransmission (char , char );
char I2C1ReadRegister(char , char );
void I2C1WriteRegister(char , char , char );
void I2C1ReadMultiRegisters(char , char , char , char * );


/********************Sam Newmarco i2c1  routines *******************************/
char I2C0Read(char SlaveID);
void I2C0Write(char SlaveID, char Data);
void I2C0ReadMulti(char SlaveID, char n, char * r);
void I2C0WriteMulti(char SlaveID, char n, char * r);


int I2C0ReadMultiE(char SlaveID, char n, char * r);
int I2C0WriteMultiE(char SlaveID, char n, char * r);

#endif //my_i2c_H_








