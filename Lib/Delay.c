#include "Delay.h"
/*
	函数名：DelayX10us
	参数：time（unsigned int）需要延迟time*10us
	功能：延时
	返回类型：void
	使用全局变量：
*/
void DelayX10us(unsigned char time){
	char ntime=time;
	do {
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	//_nop_();
	//_nop_();
	}while(ntime--);
}
/*
	函数名：DelaySec
	参数：time（unsigned int）需要延迟time秒
	功能：延时
	返回类型：void
	使用全局变量：
*/
void DelaySec(unsigned int time){
	int i,j;
	int ntime;
	for(i=0;i<1000;i++)
		for(j=0;j<10;j++){
			ntime=time;
			do{
					_nop_();
					_nop_();
					_nop_();
					_nop_();
					_nop_();
					_nop_();
					//_nop_();
					//_nop_();
			}while(ntime--);
		}
}