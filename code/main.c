#include <REGX52.H>
#include "LCD1602.h"
#include "DS1302.h"
#include "Key.h"
#include "Timer0.h"
#include "DS18B20.h"
#include "Delay.h"
#include "AT24C02.h"
#include "Buzzer.h"
#include "HX711.h"

sbit Heat=P1^6; //加热控制端口
sbit Fefri=P1^7;//制冷控制端口

//五种模式
//0 for 显示模式
//1 for 时间设置
//2 for 温度设置
//3 for 是否加热
//4 for 是否提醒喝水

unsigned char KeyNum,MODE,TimeSetPos,SetFlash;
float T;
unsigned char Temp,TempJudgeFlag;
unsigned char BuzzerFlag;
unsigned long Weight,Weight_Maopi;
unsigned long FrontWeight;
//Temp为设定温度储存

void DataShow(void)//信息显示功能
{
	DS1302_ReadTime();//读取时间
	LCD_ShowNum(1,6,DS1302_Time[3],2);//显示时
	LCD_ShowNum(1,9,DS1302_Time[4],2);//显示分
	LCD_ShowNum(1,12,DS1302_Time[5],2);//显示秒	
	DS18B20_ConvertT();
	T=DS18B20_ReadT();
	if(T>0.1) LCD_ShowNum(2,3,T,2);//显示温度
	Weight=HX711_Read()-Weight_Maopi;
	if(Weight<=0)
	{
		Weight=0;
		LCD_ShowNum(2,11,0,4);
	}
	else
	{
	Weight=(unsigned int)((float)Weight/629);
	LCD_ShowNum(2,11,Weight,4);
	}
}
//DS1302_Time为时间数组，分别为年月日时分秒


void TimeSet(void)//时间设置功能
{
	if(KeyNum==2)//按键2按下
	{
		TimeSetPos++;//设置选择位加1
		if(TimeSetPos==6) TimeSetPos=3;//越界清零
	}
	if(KeyNum==3)//按键3按下
	{
		DS1302_Time[TimeSetPos]++;//时间设置位数值加1
		if(DS1302_Time[3]>23){DS1302_Time[3]=0;}//时越界判断
		if(DS1302_Time[4]>59){DS1302_Time[4]=0;}//分越界判断
		if(DS1302_Time[5]>59){DS1302_Time[5]=0;}//秒越界判断
	}
	if(KeyNum==4)//按键4按下
	{
		DS1302_Time[TimeSetPos]--;//时间设置位数值减1
		if(DS1302_Time[3]<0){DS1302_Time[3]=23;}//时越界判断
		if(DS1302_Time[4]<0){DS1302_Time[4]=59;}//分越界判断
		if(DS1302_Time[5]<0){DS1302_Time[5]=59;}//秒越界判断
	}
	//更新显示，根据TimeSetPos和SetFlash判断可完成闪烁功能
	if(TimeSetPos==3 && SetFlash==1){LCD_ShowString(1,6,"  ");}
	else {LCD_ShowNum(1,6,DS1302_Time[3],2);}
	if(TimeSetPos==4 && SetFlash==1){LCD_ShowString(1,9,"  ");}
	else {LCD_ShowNum(1,9,DS1302_Time[4],2);}
	if(TimeSetPos==5 && SetFlash==1){LCD_ShowString(1,12,"  ");}
	else {LCD_ShowNum(1,12,DS1302_Time[5],2);}
}


void TemperatureSet(){
	if(KeyNum==2)//按键2按下
	{
		Temp=10+Temp;//温度设置位数值加1
		if(Temp>=100){Temp=99;}//TH越界判断
	}
	if(KeyNum==3)//按键3按下
	{
		Temp++;//温度设置位数值加1
		if(Temp>=100){Temp=99;}//TH越界判断
	}
	if(KeyNum==4)//按键4按下
	{
		Temp-=10;//时间设置位数值减1
		if(Temp<0){Temp=0;}//TL越界判断
	}
	//更新显示，根据TimeSetPos和SetFlash判断可完成闪烁功能
	/*if(SetFlash==1){LCD_ShowString(2,13,"  ");}
	else {LCD_ShowNum(2,13,Temp,2);}*/
	LCD_ShowNum(2,13,Temp,2);
}

void LCD_Clear(){ //清除LCD屏幕
	LCD_ShowString(1,1,"                ");
	LCD_ShowString(2,1,"                ");
}

void HeatOrFefri(){ //判断是否加热或制冷
	if(TempJudgeFlag){
		if (T>Temp+5) Fefri=0;
		else Fefri=1;
		if (T<Temp-5) Heat=0;
		else Heat=1;
	}
	else
	{
		Heat=1;
		Fefri=1;;
	}
}
void Remind(){//提醒喝水
	if(BuzzerFlag){
		if(DS1302_Time[3]%3==0&&DS1302_Time[5]<2&&DS1302_Time[4]==0){
			Buzzer_Time(2000);
		}
	}
}


void TempControl(){
	if(KeyNum==2)//按键2按下
	{
		TempJudgeFlag=1;
	}
	if(KeyNum==3)//按键3按下
	{
		TempJudgeFlag=0;
	}
	if(TempJudgeFlag) LCD_ShowString(2,1,"YES");
	else LCD_ShowString(2,1,"NO ");
}

void BuzzerControl(){
	if(KeyNum==2)//按键2按下
	{
		BuzzerFlag=1;
	}
	if(KeyNum==3)//按键3按下
	{
		BuzzerFlag=0;
	}
	if(BuzzerFlag) LCD_ShowString(2,1,"YES");
	else LCD_ShowString(2,1,"NO ");
}
void Get_Maopi()
{
	Weight_Maopi = HX711_Read();	
}

void main()
{
	LCD_Init();
	DS1302_Init();
	DS1302_SetTime();//设置时间
	Delay(100);
		Timer0Init();
	LCD_ShowString(1,1,"Time:  :  :  ");//静态字符初始化显示
	LCD_ShowString(2,1,"T:   C  V:    mL");
	LCD_ShowChar(2,5,0xDF);
	MODE=0;
	Temp=AT24C02_ReadByte(1);//从RoM读取设定温度
	TempJudgeFlag=AT24C02_ReadByte(2);
	BuzzerFlag=AT24C02_ReadByte(3);
	Get_Maopi();
	while(1)
	{
		KeyNum=Key();//读取键码
		if(KeyNum==1)//按键1按下
		{
			if(MODE==0){MODE=1;TimeSetPos=3;LCD_ShowString(2,1,"                ");}//功能切换
			else if(MODE==1){MODE=2;DS1302_SetTime();LCD_Clear();LCD_ShowString(2,1,"Temperature:   C");LCD_ShowChar(2,15,0xDF);}
			else if(MODE==2) {MODE=3;LCD_Clear();AT24C02_WriteByte(1,Temp);LCD_ShowString(1,1,"Temp Control:");}
			else if(MODE==3) {MODE=4;LCD_Clear();AT24C02_WriteByte(2,TempJudgeFlag);LCD_ShowString(1,1,"Buzzer Control:");}
			else if(MODE==4) {MODE=0;LCD_Clear();AT24C02_WriteByte(3,BuzzerFlag);LCD_ShowString(1,1,"Time:  :  :  ");LCD_ShowString(2,1,"T:   C  V:    mL");LCD_ShowChar(2,5,0xDF);}
		}
		
		
		//五种模式
		//0 for 显示模式 加热制冷(正常模式)
		//1 for 时间设置
		//2 for 温度设置
		//3 for 是否加热
		//4 for 是否提醒喝水
		
		switch(MODE)//根据不同的功能执行不同的函数
		{
			case 0:DataShow();HeatOrFefri();Remind();break;
			case 1:TimeSet();break;
			case 2:TemperatureSet();break;
			case 3:TempControl();break;
			case 4:BuzzerControl();break;
		}
		
	}
}

void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=500)//每500ms进入一次
	{
		T0Count=0;
		SetFlash=!SetFlash;//闪烁标志位取反
	}
}