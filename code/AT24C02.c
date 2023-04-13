#include <REGX52.H>
#include "I2C.h"
#define AT24C02_ADDRESS 0xA0 //W,  R for 0xA1



void AT24C02_WriteByte(unsigned char WordAddress,unsigned char Data){
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);
	I2C_RecieveAck();
	I2C_SendByte(WordAddress);
	I2C_RecieveAck();
	I2C_SendByte(Data);
	I2C_RecieveAck();
	I2C_Stop();
}

unsigned char AT24C02_ReadByte(unsigned char WordAddress){
	unsigned char Data;
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS);
	I2C_RecieveAck();
	I2C_SendByte(WordAddress);
	I2C_RecieveAck();
	I2C_Start();
	I2C_SendByte(AT24C02_ADDRESS|0x01);
	I2C_RecieveAck();
	Data=I2C_RecieveByte();
	I2C_SendAck(1);
	I2C_Stop();
	return Data;
	
}