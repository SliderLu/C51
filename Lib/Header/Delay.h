#ifndef __Delay_function_H_
	#define __Delay_function_H_

/*
 ���뺯��������
*/
	#include <intrins.h> 


/*
	��������DelayX10us
	������time��unsigned int����Ҫ�ӳ�time*10us
	���ܣ���ʱ
	�������ͣ�void
	ʹ��ȫ�ֱ�����
*/
	void DelayX10us(unsigned char time);
/*
	��������DelaySec
	������time��unsigned int����Ҫ�ӳ�time��
	���ܣ���ʱ
	�������ͣ�void
	ʹ��ȫ�ֱ�����
*/
	void DelaySec(unsigned int time);
#endif	