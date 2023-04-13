#include <REGX52.H>
#include "Delay.h"

/**
  * @brief  获取独立按键键码
  * @param  无
  * @retval 按下按键的键码，范围：0~4，无按键按下时返回值为0
  */
unsigned char Key()
{
	unsigned char KeyNumber=0;
	
	if(P3_1==0){Delay(20);while(P3_1==0);Delay(20);KeyNumber=1;}
	if(P3_0==0){Delay(20);while(P3_0==0);Delay(20);KeyNumber=2;}
	if(P3_2==0){Delay(20);while(P3_2==0);Delay(20);KeyNumber=3;}
	if(P3_3==0){Delay(20);while(P3_3==0);Delay(20);KeyNumber=4;}
	
	return KeyNumber;
}

unsigned char Key_Loop(){
	static unsigned char Now,Last;
	Last=Now;
	Now=Key();
	if(Last==1 && Now==0) return 1;
	else if(Last==2 && Now==0) return 2;
	else if(Last==3 && Now==0) return 3;
	else if(Last==4 && Now==0) return 4;
	else return 0;
}