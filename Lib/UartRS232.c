#include "UartRS232.h"

char Tx_Busy;//发送忙区域

char rx_buffer[RX_BUFFER_SIZE];//rx缓冲区定义
unsigned int rx_buffer_head=0;
unsigned int rx_buffer_tail=1;
unsigned int rx_buffer_next;

int error_flag=0;	//异常抛出定义

/*
	函数名：SerialInit
	参数：buandary（unsigned int）通讯波特率
	功能：初始化串口通信的信息
	返回类型：int
	返回：0(固定值，正常退出)
	使用全局变量：Tx_Busy
*/
int SerialInit(unsigned int buandary){

	//PCON|=0x80;//??????


	REN=1;//??????????????????

	EA=1;
	
	
    SCON  = 0x50;  //???????1
    TMOD &= 0x0F;  //??T1????
    TMOD |= 0x20;  //??T1???2
    TH1 = 256 - (11059200/12/32)/buandary;  //??T1???
    TL1 = TH1;     //???????
    ET1 = 0;       //??T1??
    ES  = 1;       //??????
    TR1 = 1;       //??T1
	
	Tx_Busy=0;

	rx_buffer_head=0;
	rx_buffer_tail=1;
	return 0;
}
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
int ReadSerial(char *ReadChar){
	int isOversizeOccur=0;
	if(error_flag&RX_BUFFER_OVERSIZE_EXCEPTION){
		isOversizeOccur=1;
		error_flag&=~(0x0fff&RX_BUFFER_OVERSIZE_EXCEPTION);
	}
	if(rx_buffer_head==rx_buffer_tail){
		return 0xffff;
	}
	*ReadChar=rx_buffer[rx_buffer_head];

	rx_buffer_head++;
	if(rx_buffer_head==RX_BUFFER_SIZE) rx_buffer_head=0;

	if(isOversizeOccur)	return RX_BUFFER_OVERSIZE_EXCEPTION;
	else return 0; 
}
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
*/ /*
int Readln(char *ReadString,char maxLenth){
	int StringLenth=0;
	int StringPtr=rx_buffer_head;
	int isOversizeOccur=0;
	int isNoEnd=0;
	
	if(error_flag&RX_BUFFER_OVERSIZE_EXCEPTION){//确定buffer是否发生过溢出
		isOversizeOccur=1;//记录发生过溢出
		error_flag|=~(0x0fff&RX_BUFFER_OVERSIZE_EXCEPTION);
	}
	if(rx_buffer_head==rx_buffer_tail){//确定当前接收buffer中是否为空数据，返回0xffff表示空数据
		return 0xffff;
	}
	
	while((rx_buffer[StringPtr]=='\0')&&(rx_buffer[StringPtr]=='\n')){
		
		ReadString[StringLenth]=rx_buffer[StringPtr];//读取区域
		
		StringLenth++;
		StringPtr++;		
		if(StringPtr=RX_BUFFER_SIZE) StringPtr=0;
		
		if(StringPtr==rx_buffer_tail){ //如果找遍了buffer，并没有看到结束符，发现不是字符串或者行数
			error_flag|=RX_READLN_NOEND;//抛出无结束错误
			ReadString[0]='\0';//标记读取失效
			isNoEnd=1;//置为错误标记位
			break;
		}
		else if(StringLenth==maxLenth-1){
			ReadString[StringLenth]='\0';
			break;	
		}
	}
	//返回错误区域
	if(isNoEnd){
		if(isOversizeOccur) return RX_READLN_NOEND|RX_BUFFER_OVERSIZE_EXCEPTION;
		else return RX_READLN_NOEND;
	}
	else{//如果队列中有结束符，不论是否发生过溢出，都需要后移head指针
		ReadString[StringLenth++]=rx_buffer[StringPtr++];//将结尾拷贝出来
		if(ReadString[StringLenth]=='\n') ReadString[StringLenth++]='\0';//如果是换行结尾，末尾应当增加结束符
		
		rx_buffer_head=StringPtr;//移动rx_buffer_head指针，将其放在结尾的下一个字节处
		
		error_flag&=~(RX_BUFFER_OVERSIZE_EXCEPTION);//清楚溢出发生错误标志位
		
		if(isOversizeOccur) return RX_BUFFER_OVERSIZE_EXCEPTION;
		else return 0;
	}
} */

/*
	函数名：WriteSerial
	参数：payload（char）要发送的字符
	功能：从串口中发送一个字符
	返回类型：int
	返回：0(完成功能，正常退出)
	      TX_BUSY_ALWAYS_EXCEPTION发送超时
	使用全局变量：Tx_Busy、error_flag
*/
int WriteSerial(char payload){
	
	int SerialBusyCount=0;//?????????
	while(Tx_Busy){	//?????�???????????
		SerialBusyCount++;
		if(SerialBusyCount>TX_MAX_WAITTIME){//??�???????????????????????
			error_flag|=TX_BUSY_ALWAYS_EXCEPTION;
			return error_flag;
		}		
	}

	SBUF=payload;
	
	SerialBusyCount=0;
	Tx_Busy=1;
	while(Tx_Busy){
		SerialBusyCount++;
		if(SerialBusyCount>TX_MAX_WAITTIME){//??�???????????????????????
			error_flag|=TX_BUSY_ALWAYS_EXCEPTION;
			return error_flag;
		}
	}
	return 0;
}
/*
	函数名：WriteString
	参数：payload（char *）要发送的字符串
	功能：从串口中发送一个字符串
	返回类型：int
	返回：0(完成功能，正常退出)
	      TX_BUSY_ALWAYS_EXCEPTION发送超时
	使用全局变量：
*/
int WriteString(char *payload){
	int isErrorOccur=0;
	while(*payload!='\0'){
		isErrorOccur|=WriteSerial(*payload++);
		if(isErrorOccur!=0) break;
	}
	return isErrorOccur;	
}


/*
 *******************************************************************************************************************
 中断向量区域
 *******************************************************************************************************************
*/	
/*
	中断类型：Uart串口中断
	中断ID：4
	函数名：serial
	是否关闭中断：全局关闭
	功能：延时
	使用全局变量：rx_buffer_head、rx_buffer_tail、rx_buffer_next、rx_buffer[]、Tx_Busy
*/
void serial() interrupt 4{
	
	char sbuf;
	
	
	ES=0;
	EA=0;
	
	if(RI){
		
		RI=0;
		rx_buffer_next=rx_buffer_tail+1;
		if(rx_buffer_next==RX_BUFFER_SIZE){
			rx_buffer_next=0;
		}
		if(rx_buffer_next==rx_buffer_head){
			error_flag|=RX_BUFFER_OVERSIZE_EXCEPTION;
			error_flag|=RX_BUFFER_OVERSIZE_EXCEPTION_Once;
			rx_buffer_head++;
			if(rx_buffer_head==RX_BUFFER_SIZE) rx_buffer_head=0;
		}
		sbuf=SBUF;
		rx_buffer[rx_buffer_tail]=sbuf;
		rx_buffer_tail=rx_buffer_next;
	}
	if(TI==1){
		Tx_Busy=0;
		TI=0;
	}
	
	
	
	EA=1;
	ES=1;
}