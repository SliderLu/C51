#ifndef __UART_RS232_H_
	 #define __UART_RS232_H_
	 
/*
 ���뺯��������
*/
	#include <reg51.h> //C51��׼������
	
	
/*
 �������������
*/
	#define	RX_BUFFER_SIZE 40	//���ջ�������Ĵ�С
	#define	TX_MAX_WAITTIME	10000//���ȴ����ڷ���ʱ�� 
	
/*
 �����׳���������
*/
	#define RX_BUFFER_OVERSIZE_EXCEPTION		0xe001
	#define RX_BUFFER_OVERSIZE_EXCEPTION_Once	0xe010
	#define RX_READLN_NOEND						0xe008
	#define TX_BUSY_ALWAYS_EXCEPTION			0xe002
	#define TX_SBUF_REG_EXCEPTION				0xe004//����
	
	extern char Tx_Busy;//����æ����
	
	extern char rx_buffer[RX_BUFFER_SIZE];//rx����������
	extern unsigned int rx_buffer_head;
	extern unsigned int rx_buffer_tail;
	extern unsigned int rx_buffer_next;
	
	extern int error_flag;	//�쳣�׳�����
	
	
/*
 *******************************************************************************************************************
 RS232��������
 *******************************************************************************************************************
*/
/*
	��������SerialInit
	������buandary��unsigned int��ͨѶ������
	���ܣ���ʼ������ͨ�ŵ���Ϣ
	�������ͣ�int
	���أ�0(�̶�ֵ�������˳�)
	ʹ��ȫ�ֱ�����Tx_Busy
*/		
	int SerialInit(unsigned int buandary);
/*
	��������ReadSerial
	������ReadChar��char *������ȡ���Ĵ�����Ϣ����ı���ָ��
	���ܣ��Ӵ��ڵĽ���Buffer�У���ȡһ���ַ��������У���������ܵ���������־
	�������ͣ�int
	���أ�0(������ɣ������˳�)
		  0xffff(���ڽ���buffer��������)
		  RX_BUFFER_OVERSIZE_EXCEPTION(����buffer�з������������ȡ���Ĳ����ǵ�һ���ַ�)
	ʹ��ȫ�ֱ�����rx_buffer_head��rx_buffer_tail��rx_buffer[]��error_flag
*/
	int ReadSerial(char *ReadChar);
/*
	��������Readln
	������ReadString��char *�����յ��ַ�����ʼָ��
		  maxLenth��char�����Խ��յ��ַ�����󳤶�
	���ܣ��Ӵ��ڵĽ���Buffer�У���ȡһ���ַ����������У���������ܵ���������־
	�������ͣ�int
	���أ�0(������ɣ������˳�)
		  0xffff(���ڽ���buffer��������)
		  RX_BUFFER_OVERSIZE_EXCEPTION(����buffer�з������������ȡ���Ĳ����ǵ�һ���ַ�)
          RX_READLN_NOEND�����ܵ�������û�н�β���ţ�
	ʹ��ȫ�ֱ�����
*/	
	int Readln(char *ReadString,char maxLenth);	
/*
	��������WriteSerial
	������payload��char��Ҫ���͵��ַ�
	���ܣ��Ӵ����з���һ���ַ�
	�������ͣ�int
	���أ�0(��ɹ��ܣ������˳�)
	      TX_BUSY_ALWAYS_EXCEPTION���ͳ�ʱ
	ʹ��ȫ�ֱ�����Tx_Busy��error_flag
*/
	int WriteSerial(char payload);
/*
	��������WriteString
	������payload��char *��Ҫ���͵��ַ���
	���ܣ��Ӵ����з���һ���ַ���
	�������ͣ�int
	���أ�0(��ɹ��ܣ������˳�)
	      TX_BUSY_ALWAYS_EXCEPTION���ͳ�ʱ
	ʹ��ȫ�ֱ�����
*/
	int WriteString(char *payload);
	
#endif