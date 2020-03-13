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

#include "my_i2c.h"

//  For the onboard accelerometer


void init_I2C0(void)
{
  SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;
  SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
  PORTE_PCR24 = PORT_PCR_MUX(5);
  PORTE_PCR25 = PORT_PCR_MUX(5);
  I2C0_F  = 0x14;
  I2C0_C1 = I2C_C1_IICEN_MASK;
}

void init_I2C0_PTC8_9(void)
{
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTC_MASK;
  SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
  PORTC_PCR8 = PORT_PCR_MUX(2);//SCL
  PORTC_PCR9 = PORT_PCR_MUX(2);//SDA
  I2C0_F  = 0x14;
  I2C0_C1 = I2C_C1_IICEN_MASK;
}

/*
void init_I2C0(void)
{
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTC_MASK;
  SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
  PORTC_PCR8 = PORT_PCR_MUX(2);
  PORTC_PCR9 = PORT_PCR_MUX(2);
  I2C0_F  = 0x14;
  I2C0_C1 = I2C_C1_IICEN_MASK;
}
*/



void IIC0_StartTransmission (char SlaveID, char Mode)
{
  SlaveID = SlaveID << 1;
  SlaveID |= (Mode & 0x01);
  i2c0_Start();
  i2c0_write_byte(SlaveID);
}

void Pause(void){
    int n;
    for(n=1;n<50;n++) {
      asm("nop");
    }
}



char I2C0ReadRegister(char SlaveID, char RegisterAddress)
{
  char result;

  IIC0_StartTransmission(SlaveID,MWSR);
  i2c0_Wait();
  i2c0_write_byte(RegisterAddress);
  i2c0_Wait();
  i2c0_RepeatedStart();
  i2c0_write_byte((SlaveID << 1) | 0x01);
  i2c0_Wait();
  i2c0_EnterRxMode();
  i2c0_DisableAck(); // Desabilita ACK por ser o penultimo byte a ler
  result = I2C0_D ; // Dummy read
  i2c0_Wait();
  i2c0_Stop(); // Envia STOP por ser o ultimo byte
  result = I2C0_D ; // Le o byte
  Pause();
  return result;
}


void I2C0WriteRegister(char SlaveID, char RegisterAddress, char Data)
{
  IIC0_StartTransmission(SlaveID,MWSR);
  i2c0_Wait();
  i2c0_write_byte(RegisterAddress);
  i2c0_Wait();
  i2c0_write_byte(Data);
  i2c0_Wait();
  i2c0_Stop();
  Pause();
}


void I2C0ReadMultiRegisters(char SlaveID, char RegisterAddress, char n, char * r)
{
  char i;

  IIC0_StartTransmission(SlaveID,MWSR);
  i2c0_Wait();
  i2c0_write_byte(RegisterAddress);
  i2c0_Wait();
  i2c0_RepeatedStart();
  i2c0_write_byte((SlaveID << 1) | 0x01);
  i2c0_Wait();
  i2c0_EnterRxMode();
  i2c0_EnableAck();
  i = I2C0_D ; // Dummy read
  i2c0_Wait();

  for(i=0;i<n-2;i++) // le n-1 bytes
  {
    *r = I2C0_D;
    r++;
    i2c0_Wait();
  }
  i2c0_DisableAck(); // Desabilita ACK por ser o penultimo byte a ler
  *r = I2C0_D; // Le penultimo byte
  r++;
  i2c0_Wait();
  i2c0_Stop(); // Envia STOP por ser o ultimo byte
  *r = I2C0_D; // Le ultimo byte
  Pause();
}


/***************************i2C1 routines******************************/

void init_I2C1(void)
{
  SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;
  SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;
  PORTE_PCR0 = PORT_PCR_MUX(5);
  PORTE_PCR1 = PORT_PCR_MUX(5);
  I2C1_F  = 0x14;
  I2C1_C1 = I2C_C1_IICEN_MASK;
}

void IIC1_StartTransmission (char SlaveID, char Mode)
{
  SlaveID = SlaveID << 1;
  SlaveID |= (Mode & 0x01);
  i2c1_Start();
  i2c1_write_byte(SlaveID);
}


char I2C1ReadRegister(char SlaveID, char RegisterAddress)
{
  char result;

  IIC1_StartTransmission(SlaveID,MWSR);
  i2c1_Wait();
  i2c1_write_byte(RegisterAddress);
  i2c1_Wait();
  i2c1_RepeatedStart();
  i2c1_write_byte((SlaveID << 1) | 0x01);
  i2c1_Wait();
  i2c1_EnterRxMode();
  i2c1_DisableAck(); // Desabilita ACK por ser o penultimo byte a ler
  result = I2C1_D ; // Dummy read
  i2c1_Wait();
  i2c1_Stop(); // Envia STOP por ser o ultimo byte
  result = I2C1_D ; // Le o byte
  Pause();
  return result;
}


void I2C1WriteRegister(char SlaveID, char RegisterAddress, char Data)
{
  IIC1_StartTransmission(SlaveID,MWSR);
  i2c1_Wait();
  i2c1_write_byte(RegisterAddress);
  i2c1_Wait();
  i2c1_write_byte(Data);
  i2c1_Wait();
  i2c1_Stop();
  Pause();
}


void I2C1ReadMultiRegisters(char SlaveID, char RegisterAddress, char n, char * r)
{
  char i;

  IIC1_StartTransmission(SlaveID,MWSR);
  i2c1_Wait();
  i2c1_write_byte(RegisterAddress);
  i2c1_Wait();
  i2c1_RepeatedStart();
  i2c1_write_byte((SlaveID << 1) | 0x01);
  i2c1_Wait();
  i2c1_EnterRxMode();
  i2c1_EnableAck();
  i = I2C1_D ; // Dummy read
  i2c1_Wait();

  for(i=0;i<n-2;i++) // le n-1 bytes
  {
    *r = I2C1_D;
    r++;
    i2c1_Wait();
  }
  i2c1_DisableAck(); // Desabilita ACK por ser o penultimo byte a ler
  *r = I2C1_D; // Le penultimo byte
  r++;
  i2c1_Wait();
  i2c1_Stop(); // Envia STOP por ser o ultimo byte
  *r = I2C1_D; // Le ultimo byte
  Pause();
}



/***************************i2C0 routines******************************/
/*
 * I2C functions for reading/writing that do not specify a register 
 * Added by Samuel Newmarco 1/20/2020 
 */

char I2C0Read(char SlaveID)
{
  char result;

  IIC0_StartTransmission(SlaveID,MWSR);
  i2c0_Wait();
  i2c0_RepeatedStart();
  i2c0_write_byte((SlaveID << 1) | 0x01);//why is this here?
  i2c0_Wait();
  i2c0_EnterRxMode();
  i2c0_DisableAck(); // Desabilita ACK por ser o penultimo byte a ler
  result = I2C0_D ; // Dummy read
  i2c0_Wait();
  i2c0_Stop(); // Envia STOP por ser o ultimo byte
  result = I2C0_D ; // Le o byte
  Pause();
  return result;
}


void I2C0Write(char SlaveID, char Data)
{
  IIC0_StartTransmission(SlaveID,MWSR);
  i2c0_Wait();
  i2c0_write_byte(Data);
  i2c0_Wait();
  i2c0_Stop();
  Pause();
}


void I2C0ReadMulti(char SlaveID, char n, char * r)
{
  int i;

  IIC0_StartTransmission(SlaveID,MRSW);
  i2c0_Wait();
  i2c0_EnterRxMode();
  i2c0_EnableAck();
  i = I2C0_D ; // Dummy read
  i2c0_Wait();

  for(i=0;i<n-2;i++) // le n-1 bytes
  {
    r[i] = I2C0_D;
    i2c0_Wait();
  }
  i2c0_DisableAck(); // Desabilita ACK por ser o penultimo byte a ler
  r[i] = I2C0_D; // Le penultimo byte
  i++;
  i2c0_Wait();
  i2c0_Stop(); // Envia STOP por ser o ultimo byte
  r[i] = I2C0_D; // Le ultimo byte
  Pause();
}

void I2C0WriteMulti(char SlaveID, char n, char* r)
{
  int i;
  IIC0_StartTransmission(SlaveID,MWSR);
  i2c0_Wait();
  for(i = 0; i<n; i++){
	  i2c0_write_byte(r[i]);
	  i2c0_Wait();
  }
  i2c0_Stop();
  Pause();
}

int I2C0ReadMultiE(char SlaveID, char n, char * r)
{
  int i;

  IIC0_StartTransmission(SlaveID,MRSW);
  i2c0_Wait_E();
  i2c0_EnterRxMode();
  i2c0_EnableAck();
  i = I2C0_D ; // Dummy read
  i2c0_Wait_E();

  for(i=0;i<n-2;i++) // le n-1 bytes
  {
    r[i] = I2C0_D;
    i2c0_Wait_E();
  }
  i2c0_DisableAck(); // Desabilita ACK por ser o penultimo byte a ler
  r[i] = I2C0_D; // Le penultimo byte
  i++;
  i2c0_Wait_E();
  i2c0_Stop(); // Envia STOP por ser o ultimo byte
  r[i] = I2C0_D; // Le ultimo byte
  Pause();
  return 0;
}

int I2C0WriteMultiE(char SlaveID, char n, char* r)
{
  int i;
  IIC0_StartTransmission(SlaveID,MWSR);
  i2c0_Wait_E();
  for(i = 0; i<n; i++){
	  i2c0_write_byte(r[i]);
	  i2c0_Wait_E();
  }
  i2c0_Stop();
  Pause();
  return 0;
}
