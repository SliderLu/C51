#include "UartRS232.h"

char Tx_Busy;//����æ����

char rx_buffer[RX_BUFFER_SIZE];//rx����������
unsigned int rx_buffer_head=0;
unsigned int rx_buffer_tail=1;
unsigned int rx_buffer_next;

int error_flag=0;	//�쳣�׳�����

/*
	��������SerialInit
	������buandary��unsigned int��ͨѶ������
	���ܣ���ʼ������ͨ�ŵ���Ϣ
	�������ͣ�int
	���أ�0(�̶�ֵ�������˳�)
	ʹ��ȫ�ֱ�����Tx_Busy
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
	��������ReadSerial
	������ReadChar��char *������ȡ���Ĵ�����Ϣ����ı���ָ��
	���ܣ��Ӵ��ڵĽ���Buffer�У���ȡһ���ַ��������У���������ܵ���������־
	�������ͣ�int
	���أ�0(������ɣ������˳�)
		  0xffff(���ڽ���buffer��������)
		  RX_BUFFER_OVERSIZE_EXCEPTION(����buffer�з������������ȡ���Ĳ����ǵ�һ���ַ�)
	ʹ��ȫ�ֱ�����rx_buffer_head��rx_buffer_tail��rx_buffer[]��error_flag
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
*/ /*
int Readln(char *ReadString,char maxLenth){
	int StringLenth=0;
	int StringPtr=rx_buffer_head;
	int isOversizeOccur=0;
	int isNoEnd=0;
	
	if(error_flag&RX_BUFFER_OVERSIZE_EXCEPTION){//ȷ��buffer�Ƿ��������
		isOversizeOccur=1;//��¼���������
		error_flag|=~(0x0fff&RX_BUFFER_OVERSIZE_EXCEPTION);
	}
	if(rx_buffer_head==rx_buffer_tail){//ȷ����ǰ����buffer���Ƿ�Ϊ�����ݣ�����0xffff��ʾ������
		return 0xffff;
	}
	
	while((rx_buffer[StringPtr]=='\0')&&(rx_buffer[StringPtr]=='\n')){
		
		ReadString[StringLenth]=rx_buffer[StringPtr];//��ȡ����
		
		StringLenth++;
		StringPtr++;		
		if(StringPtr=RX_BUFFER_SIZE) StringPtr=0;
		
		if(StringPtr==rx_buffer_tail){ //����ұ���buffer����û�п��������������ֲ����ַ�����������
			error_flag|=RX_READLN_NOEND;//�׳��޽�������
			ReadString[0]='\0';//��Ƕ�ȡʧЧ
			isNoEnd=1;//��Ϊ������λ
			break;
		}
		else if(StringLenth==maxLenth-1){
			ReadString[StringLenth]='\0';
			break;	
		}
	}
	//���ش�������
	if(isNoEnd){
		if(isOversizeOccur) return RX_READLN_NOEND|RX_BUFFER_OVERSIZE_EXCEPTION;
		else return RX_READLN_NOEND;
	}
	else{//����������н������������Ƿ��������������Ҫ����headָ��
		ReadString[StringLenth++]=rx_buffer[StringPtr++];//����β��������
		if(ReadString[StringLenth]=='\n') ReadString[StringLenth++]='\0';//����ǻ��н�β��ĩβӦ�����ӽ�����
		
		rx_buffer_head=StringPtr;//�ƶ�rx_buffer_headָ�룬������ڽ�β����һ���ֽڴ�
		
		error_flag&=~(RX_BUFFER_OVERSIZE_EXCEPTION);//���������������־λ
		
		if(isOversizeOccur) return RX_BUFFER_OVERSIZE_EXCEPTION;
		else return 0;
	}
} */

/*
	��������WriteSerial
	������payload��char��Ҫ���͵��ַ�
	���ܣ��Ӵ����з���һ���ַ�
	�������ͣ�int
	���أ�0(��ɹ��ܣ������˳�)
	      TX_BUSY_ALWAYS_EXCEPTION���ͳ�ʱ
	ʹ��ȫ�ֱ�����Tx_Busy��error_flag
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
	��������WriteString
	������payload��char *��Ҫ���͵��ַ���
	���ܣ��Ӵ����з���һ���ַ���
	�������ͣ�int
	���أ�0(��ɹ��ܣ������˳�)
	      TX_BUSY_ALWAYS_EXCEPTION���ͳ�ʱ
	ʹ��ȫ�ֱ�����
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
 �ж���������
 *******************************************************************************************************************
*/	
/*
	�ж����ͣ�Uart�����ж�
	�ж�ID��4
	��������serial
	�Ƿ�ر��жϣ�ȫ�ֹر�
	���ܣ���ʱ
	ʹ��ȫ�ֱ�����rx_buffer_head��rx_buffer_tail��rx_buffer_next��rx_buffer[]��Tx_Busy
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