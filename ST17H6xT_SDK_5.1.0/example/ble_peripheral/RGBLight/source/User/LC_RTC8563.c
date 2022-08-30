/**
 *	@file		LC_RTC8563.c
 *	@author		YQ(1500861686)
 *	@date		12/07/2020
 *	@version	1.0.0
 */
/*!
 *	@defgroup	LC_RTC8563
 *	@brief
 *	@{*/
/*------------------------------------------------------------------*/
/* 				  head files include							 	*/
/*------------------------------------------------------------------*/
#include "LC_RTC8563.h"
#include "LC_RGBLight_Mode.h"
/*------------------------------------------------------------------*/
/* 					 	public variables		 					*/
/*------------------------------------------------------------------*/
LC_RTC8563_Alarm_Struct	LC_Dev_RTC8563_Alarm[LC_DEV_ALARM_NUM_MAX];
LC_RTC8563_RTCTime		LC_Dev_RTC8563_DateTime		=	{
	.dev_utc_time	=	946828800,	//	2000-01-03 00:00:00 monday
	.dev_year		=	2000,
	.dev_month		=	1,
	.dev_day		=	3,
	.dev_week		=	1,
	.dev_week_old	=	1,
	.dev_week_app	=	1,
	.dev_hour		=	0,
	.dev_minute		=	0,
	.dev_second		=	0,
};
LC_RTC8563_RTCTime		LC_Dev_Read_DateTime	=	{
	.dev_utc_time	=	1514779200,	//	2018-01-01 01:00:00 monday
	.dev_year		=	2018,
	.dev_month		=	1,
	.dev_day		=	1,
	.dev_week		=	1,
	.dev_hour		=	12,
	.dev_minute 	=	0,
	.dev_second 	=	0,
};

/*------------------------------------------------------------------*/
/* 					 LOCOAL FUNCTIONS		 						*/
/*------------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/* 					 PUBLIC FUNCTIONS		 						*/
/*------------------------------------------------------------------*/


static u32 my_rtc_mon_yday[2][12] ={
	{0,31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
	{0,31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335},
};
const char my_rtc_Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

u32 my_10_switch_16_func(u32 data){
	u32 temp = 1 ;
	u32 data_temp = data%10*temp;
	do{
		temp *= 16 ;
		data = data/10 ;
		data_temp += data%10*temp;
	}while(data/10);
	return data_temp;
}
u32 my_16_switch_10_func(u32 data){
	u32 temp = 1 ;
	u32 data_temp = data%10*temp;
	do{
		temp *= 10 ;
		data = data/10 ;
		data_temp += data%10*temp;
	}while(data/10);
	return data_temp;
}
//**************************************************************************************************
//						《当前年月日算出周几�?(Year_h:年高位（20�?,Year_l:年低�?(当前�?-2000))
//*************************************************************************************************
u8 my_rtc_week_count(u8 Year_h, u8 Year_l, u8 Month, u8 Day){
	if(Month<2){
	    Month = Month+12 ;
	    Year_l = Year_l-1 ;
	}
	u32 temp = 0 ;
	temp = Year_l+(Year_l/4)+(Year_h/4)-(2*Year_h)+((26*(Month+1))/10)+Day-1 ;
	temp = temp%7 ;
	if(temp==0) temp = 7 ;
	//printf("week: %d",temp) ;
//	return (temp - 1) ;
	return (temp);
}

//**************************************************************************************************
//						《根据U32秒算出当前年月日时分秒�?(time:U32秒，LC_RTC8563_RTCTime *t:年月�?)
//*************************************************************************************************
void my_rtc_localtime(u32 time,LC_RTC8563_RTCTime *t){
	u32 Pass4year;
	u32 hours_per_year;
	if(time <= 0){
		time = 0;
	}
	t->dev_second=(u32)(time % 60);  //取�?�时�?
	time /= 60;
	t->dev_minute=(u32)(time % 60);  //取分钟时�?
	time /= 60;
	Pass4year=((unsigned int)time / (1461L * 24L));  //取过去�?�少�?四年，每四年�?1461*24小时
	t->dev_year=(Pass4year << 2) + 1970;  //计算年份
	time %= 1461L * 24L;    //四年�?剩下的小时数
	for (;;)    	        //校�?�闰年影响的年份，�?�算一年中剩下的小时数
	{
		hours_per_year = 365 * 24;   //一年的小时�?
		if ((t->dev_year & 3) == 0){  //判断闰年
			hours_per_year += 24;    //�?闰年，一年则�?24小时，即一�?
		}
		if (time < hours_per_year){
			break;
		}
		t->dev_year++;
		time -= hours_per_year;
	}
	t->dev_hour=(int)((time + 8) % 24);   //小时�?
	time = (time + 8)/24;  //一年中剩下的天�?
	time++;      //假定为闰�?
	if((t->dev_year & 3) == 0)  //校�?�闰年的�?�?，�?�算月份，日�?
	{
		if (time > 60){
			time--;
		}else{
			if (time == 60){
				t->dev_month = 1;
				t->dev_day = 29;
				return ;
			}
		}
	}
	//计算月日
	for (t->dev_month = 0; my_rtc_Days[t->dev_month] < time;t->dev_month++){
		time -= my_rtc_Days[t->dev_month];
	}
	t->dev_day = (int)(time);
	t->dev_month+=1;
	return;
}
//**************************************************************************************************
//						《校验是否为闰年�?
//*************************************************************************************************
int my_rtc_isleap(int year){
	return (year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0);
}
//**************************************************************************************************
//						《根�?当前年月日时分�?�算出U32秒�?
//*************************************************************************************************
u32 my_rtc_mktime(LC_RTC8563_RTCTime dt){
	u32 result;
	int i =0;
	//以平年时间�?�算的�?�数
	result = (dt.dev_year - 1970) * 365 * 24 * 3600 +
	(my_rtc_mon_yday[my_rtc_isleap(dt.dev_year)][dt.dev_month-1] + dt.dev_day - 1) * 24 * 3600 +\
	dt.dev_hour * 3600 + dt.dev_minute * 60 + dt.dev_second;

	for(i=1970; i < dt.dev_year; i++){      //加上闰年的�?�数
		if(my_rtc_isleap(i)) result += 24 * 3600;
	}
	result = result - (8*3600) ;
	return(result);
}

void	LC_Get_SysRtcTime(void)
{
	uint32	read_time	=	0;
	
	read_time	=	osal_getClock();//get system UTC time
	if(read_time){
		LC_Dev_RTC8563_DateTime.dev_utc_time	=	read_time;
		my_rtc_localtime(LC_Dev_RTC8563_DateTime.dev_utc_time, &LC_Dev_RTC8563_DateTime);
		LC_Dev_RTC8563_DateTime.dev_week	=	my_rtc_week_count(20,(LC_Dev_RTC8563_DateTime.dev_year-2000),LC_Dev_RTC8563_DateTime.dev_month,LC_Dev_RTC8563_DateTime.dev_day);
		if(LC_Dev_RTC8563_DateTime.dev_week != LC_Dev_RTC8563_DateTime.dev_week_old){
			LC_Dev_RTC8563_DateTime.dev_week_old	=	LC_Dev_RTC8563_DateTime.dev_week;
			LC_Dev_RTC8563_DateTime.dev_week_app++;
			if(LC_Dev_RTC8563_DateTime.dev_week_app == 8){
				LC_Dev_RTC8563_DateTime.dev_week_app	=	1;
			}
		}
//		LOG("System UTC :%d \n",LC_Dev_RTC8563_DateTime.dev_utc_time);
//		LOG("year %d month %d day %d week %d\n", LC_Dev_RTC8563_DateTime.dev_year,LC_Dev_RTC8563_DateTime.dev_month,LC_Dev_RTC8563_DateTime.dev_day,LC_Dev_RTC8563_DateTime.dev_week);
//		LOG("hour %d mintue %d second %d\n",LC_Dev_RTC8563_DateTime.dev_hour,LC_Dev_RTC8563_DateTime.dev_minute,LC_Dev_RTC8563_DateTime.dev_second);
	}
}

void	LC_Set_SysRtcTime(uint32 utc_time)
{
	osal_setClock(utc_time );
}

void	LC_RGBLight_Alarm_TurnOnoff(LC_RTC8563_Alarm_Struct *temp)
{
	//	turn on led
	if(temp->Func == 0x00){
		if(LC_RGBLight_Param.RGB_Light_State == State_Off){
			LC_RGBLight_Turn_Onoff(State_On);
		}
	}
	//	turn off led
	else if(temp->Func == 0x01){
		if(LC_RGBLight_Param.RGB_Light_State == State_On){
			LC_RGBLight_Turn_Onoff(State_Off);
		}
	}
}


/*!
 *	@fn			LC_RGBLight_Alarm_Process
 *	@brief		RGBLight tiemr processing.
 *	@param[in]	none.
 *	@return		none.
 */
void	LC_RGBLight_Alarm_Process(void)
{
	// //	Alarm NO.0(turn on led)
	// if(LC_Dev_RTC8563_Alarm[0].Enable == State_On){
	// 	if((LC_Dev_RTC8563_Alarm[0].Hour == LC_Dev_RTC8563_DateTime.dev_hour) && \
	// 		(LC_Dev_RTC8563_Alarm[0].Minute	== LC_Dev_RTC8563_DateTime.dev_minute) && \
	// 		(LC_Dev_RTC8563_Alarm[0].Second	== LC_Dev_RTC8563_DateTime.dev_second)){
	// 		if(LC_Dev_RTC8563_Alarm[0].WeekDay == 0){
	// 			LC_Dev_RTC8563_Alarm[0].Enable	=	State_Off;
	// 			LC_RGBLight_Alarm_TurnOnoff(&LC_Dev_RTC8563_Alarm[0]);
	// 		}else if(LC_Dev_RTC8563_Alarm[0].WeekDay & BIT(LC_Dev_RTC8563_DateTime.dev_week_app - 1)){
	// 			LC_RGBLight_Alarm_TurnOnoff(&LC_Dev_RTC8563_Alarm[0]);
	// 		}
	// 	}
	// }
	// //	Alarm NO.1(turn off led)
	// if(LC_Dev_RTC8563_Alarm[1].Enable == State_On){
	// 	if((LC_Dev_RTC8563_Alarm[1].Hour == LC_Dev_RTC8563_DateTime.dev_hour) && \
	// 		(LC_Dev_RTC8563_Alarm[1].Minute	== LC_Dev_RTC8563_DateTime.dev_minute) && \
	// 		(LC_Dev_RTC8563_Alarm[1].Second	== LC_Dev_RTC8563_DateTime.dev_second)){
	// 		if(LC_Dev_RTC8563_Alarm[1].WeekDay == 0){
	// 			LC_Dev_RTC8563_Alarm[1].Enable	=	State_Off;
	// 			LC_RGBLight_Alarm_TurnOnoff(&LC_Dev_RTC8563_Alarm[1]);
	// 		}else if(LC_Dev_RTC8563_Alarm[1].WeekDay & BIT(LC_Dev_RTC8563_DateTime.dev_week_app - 1)){
	// 			LC_RGBLight_Alarm_TurnOnoff(&LC_Dev_RTC8563_Alarm[1]);
	// 		}
	// 	}
	// }

	uint8	alaram_index	=	0;

	for(alaram_index = 0; alaram_index < LC_DEV_ALARM_NUM_MAX; alaram_index++)
	{
		if((LC_Dev_RTC8563_Alarm[alaram_index].Hour == LC_Dev_RTC8563_DateTime.dev_hour) && \
			(LC_Dev_RTC8563_Alarm[alaram_index].Minute == LC_Dev_RTC8563_DateTime.dev_minute) && \
			(LC_Dev_RTC8563_Alarm[alaram_index].Second == LC_Dev_RTC8563_DateTime.dev_second))
		{
			if(LC_Dev_RTC8563_Alarm[alaram_index].WeekDay == 0)
			{
				LC_Dev_RTC8563_Alarm[alaram_index].Enable	=	State_Off;
				LC_RGBLight_Alarm_TurnOnoff(&LC_Dev_RTC8563_Alarm[alaram_index]);
			}
			else if(LC_Dev_RTC8563_Alarm[alaram_index].WeekDay & BIT(LC_Dev_RTC8563_DateTime.dev_week_app - 1))
			{
				LC_RGBLight_Alarm_TurnOnoff(&LC_Dev_RTC8563_Alarm[alaram_index]);
			}
		}
	}
}
/** @}*/

