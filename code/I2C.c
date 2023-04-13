#include <REGX52.H>

sbit SCL=P2^1;
sbit SDA=P2^0;

void I2C_Start(){
	SDA=1;
	SCL=1;
	SDA=0;
	SCL=0;
}

void I2C_Stop(){
	SDA=0;
	SCL=1;
	SDA=1;
}

void I2C_SendByte(unsigned char Byte){
	int i;
	for(i=0;i<8;i++){
		SDA=Byte&(0x80>>i);
		SCL=1;
		SCL=0;
	}
}

unsigned char I2C_RecieveByte(){
	unsigned char Byte=0x00;
	int i;
	SDA=1;
	for(i=0;i<8;i++){
		SCL=1;
		if(SDA) Byte=Byte|(0x80>>i);
		SCL=0;
	}
	return Byte;
}

void I2C_SendAck(unsigned char AckBit){
	SDA=AckBit;
	SCL=1;
	SCL=0;
}//0 for avilialbe

unsigned char I2C_RecieveAck(){
	unsigned char Ack;
	SDA=1;
	SCL=1;
	Ack=SDA;
	SCL=0;
	return Ack;
}//0 for avilialbe


