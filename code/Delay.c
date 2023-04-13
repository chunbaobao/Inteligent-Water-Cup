#include <INTRINS.H>
void Delay(unsigned int xms)		//@11.0592MHz
{
	unsigned char i, j;
		while(xms){
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
		xms--;
	}
}

void Delay70us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 29;
	while (--i);
}

void Delay500us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 227;
	while (--i);
}

void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	i = 2;
	while (--i);
}

void Delay50us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 20;
	while (--i);
}

void Delay5us()		//@11.0592MHz
{
}
