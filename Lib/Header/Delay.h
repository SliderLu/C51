#ifndef __Delay_function_H_
	#define __Delay_function_H_

/*
 引入函数库区域
*/
	#include <intrins.h> 


/*
	函数名：DelayX10us
	参数：time（unsigned int）需要延迟time*10us
	功能：延时
	返回类型：void
	使用全局变量：
*/
	void DelayX10us(unsigned char time);
/*
	函数名：DelaySec
	参数：time（unsigned int）需要延迟time秒
	功能：延时
	返回类型：void
	使用全局变量：
*/
	void DelaySec(unsigned int time);
#endif	