#include "Delay.h"
/*
	��������DelayX10us
	������time��unsigned int����Ҫ�ӳ�time*10us
	���ܣ���ʱ
	�������ͣ�void
	ʹ��ȫ�ֱ�����
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
	��������DelaySec
	������time��unsigned int����Ҫ�ӳ�time��
	���ܣ���ʱ
	�������ͣ�void
	ʹ��ȫ�ֱ�����
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