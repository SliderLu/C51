#ifndef __UART_RS232_H_
	 #define __UART_RS232_H_
	 
/*
 引入函数库区域
*/
	#include <reg51.h> //C51标准函数库
	
	
/*
 宏参数定义区域
*/
	#define	RX_BUFFER_SIZE 40	//接收缓存区域的大小
	#define	TX_MAX_WAITTIME	10000//最大等待串口发送时间 
	
/*
 错误抛出定义区域
*/
	#define RX_BUFFER_OVERSIZE_EXCEPTION		0xe001
	#define RX_BUFFER_OVERSIZE_EXCEPTION_Once	0xe010
	#define RX_READLN_NOEND						0xe008
	#define TX_BUSY_ALWAYS_EXCEPTION			0xe002
	#define TX_SBUF_REG_EXCEPTION				0xe004//弃用
	
	extern char Tx_Busy;//发送忙区域
	
	extern char rx_buffer[RX_BUFFER_SIZE];//rx缓冲区定义
	extern unsigned int rx_buffer_head;
	extern unsigned int rx_buffer_tail;
	extern unsigned int rx_buffer_next;
	
	extern int error_flag;	//异常抛出定义
	
	
/*
 *******************************************************************************************************************
 RS232函数区域
 *******************************************************************************************************************
*/
/*
	函数名：SerialInit
	参数：buandary（unsigned int）通讯波特率
	功能：初始化串口通信的信息
	返回类型：int
	返回：0(固定值，正常退出)
	使用全局变量：Tx_Busy
*/		
	int SerialInit(unsigned int buandary);
/*
	函数名：ReadSerial
	参数：ReadChar（char *）将读取到的串口信息保存的变量指针
	功能：从串口的接收Buffer中，读取一个字符到参数中，并清楚可能的溢出错误标志
	返回类型：int
	返回：0(功能完成，正常退出)
		  0xffff(串口接收buffer中无数据)
		  RX_BUFFER_OVERSIZE_EXCEPTION(串口buffer中发生过溢出，读取到的并不是第一个字符)
	使用全局变量：rx_buffer_head、rx_buffer_tail、rx_buffer[]、error_flag
*/
	int ReadSerial(char *ReadChar);
/*
	函数名：Readln
	参数：ReadString（char *）接收的字符串起始指针
		  maxLenth（char）可以接收的字符串最大长度
	功能：从串口的接收Buffer中，读取一个字符串到参数中，并清楚可能的溢出错误标志
	返回类型：int
	返回：0(功能完成，正常退出)
		  0xffff(串口接收buffer中无数据)
		  RX_BUFFER_OVERSIZE_EXCEPTION(串口buffer中发生过溢出，读取到的并不是第一个字符)
          RX_READLN_NOEND（接受到的数据没有结尾符号）
	使用全局变量：
*/	
	int Readln(char *ReadString,char maxLenth);	
/*
	函数名：WriteSerial
	参数：payload（char）要发送的字符
	功能：从串口中发送一个字符
	返回类型：int
	返回：0(完成功能，正常退出)
	      TX_BUSY_ALWAYS_EXCEPTION发送超时
	使用全局变量：Tx_Busy、error_flag
*/
	int WriteSerial(char payload);
/*
	函数名：WriteString
	参数：payload（char *）要发送的字符串
	功能：从串口中发送一个字符串
	返回类型：int
	返回：0(完成功能，正常退出)
	      TX_BUSY_ALWAYS_EXCEPTION发送超时
	使用全局变量：
*/
	int WriteString(char *payload);
	
#endif