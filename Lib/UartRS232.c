#include "UartRS232.h"

char Tx_Busy;//·¢ËÍÃ¦ÇøÓò

char rx_buffer[RX_BUFFER_SIZE];//rx»º³åÇø¶¨Òå
unsigned int rx_buffer_head=0;
unsigned int rx_buffer_tail=1;
unsigned int rx_buffer_next;

int error_flag=0;	//Òì³£Å×³ö¶¨Òå

/*
	º¯ÊýÃû£ºSerialInit
	²ÎÊý£ºbuandary£¨unsigned int£©Í¨Ñ¶²¨ÌØÂÊ
	¹¦ÄÜ£º³õÊ¼»¯´®¿ÚÍ¨ÐÅµÄÐÅÏ¢
	·µ»ØÀàÐÍ£ºint
	·µ»Ø£º0(¹Ì¶¨Öµ£¬Õý³£ÍË³ö)
	Ê¹ÓÃÈ«¾Ö±äÁ¿£ºTx_Busy
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
	º¯ÊýÃû£ºReadSerial
	²ÎÊý£ºReadChar£¨char *£©½«¶ÁÈ¡µ½µÄ´®¿ÚÐÅÏ¢±£´æµÄ±äÁ¿Ö¸Õë
	¹¦ÄÜ£º´Ó´®¿ÚµÄ½ÓÊÕBufferÖÐ£¬¶ÁÈ¡Ò»¸ö×Ö·ûµ½²ÎÊýÖÐ£¬²¢Çå³þ¿ÉÄÜµÄÒç³ö´íÎó±êÖ¾
	·µ»ØÀàÐÍ£ºint
	·µ»Ø£º0(¹¦ÄÜÍê³É£¬Õý³£ÍË³ö)
		  0xffff(´®¿Ú½ÓÊÕbufferÖÐÎÞÊý¾Ý)
		  RX_BUFFER_OVERSIZE_EXCEPTION(´®¿ÚbufferÖÐ·¢Éú¹ýÒç³ö£¬¶ÁÈ¡µ½µÄ²¢²»ÊÇµÚÒ»¸ö×Ö·û)
	Ê¹ÓÃÈ«¾Ö±äÁ¿£ºrx_buffer_head¡¢rx_buffer_tail¡¢rx_buffer[]¡¢error_flag
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
	º¯ÊýÃû£ºReadln
	²ÎÊý£ºReadString£¨char *£©½ÓÊÕµÄ×Ö·û´®ÆðÊ¼Ö¸Õë
		  maxLenth£¨char£©¿ÉÒÔ½ÓÊÕµÄ×Ö·û´®×î´ó³¤¶È
	¹¦ÄÜ£º´Ó´®¿ÚµÄ½ÓÊÕBufferÖÐ£¬¶ÁÈ¡Ò»¸ö×Ö·û´®µ½²ÎÊýÖÐ£¬²¢Çå³þ¿ÉÄÜµÄÒç³ö´íÎó±êÖ¾
	·µ»ØÀàÐÍ£ºint
	·µ»Ø£º0(¹¦ÄÜÍê³É£¬Õý³£ÍË³ö)
		  0xffff(´®¿Ú½ÓÊÕbufferÖÐÎÞÊý¾Ý)
		  RX_BUFFER_OVERSIZE_EXCEPTION(´®¿ÚbufferÖÐ·¢Éú¹ýÒç³ö£¬¶ÁÈ¡µ½µÄ²¢²»ÊÇµÚÒ»¸ö×Ö·û)
          RX_READLN_NOEND£¨½ÓÊÜµ½µÄÊý¾ÝÃ»ÓÐ½áÎ²·ûºÅ£©
	Ê¹ÓÃÈ«¾Ö±äÁ¿£º
*/ /*
int Readln(char *ReadString,char maxLenth){
	int StringLenth=0;
	int StringPtr=rx_buffer_head;
	int isOversizeOccur=0;
	int isNoEnd=0;
	
	if(error_flag&RX_BUFFER_OVERSIZE_EXCEPTION){//È·¶¨bufferÊÇ·ñ·¢Éú¹ýÒç³ö
		isOversizeOccur=1;//¼ÇÂ¼·¢Éú¹ýÒç³ö
		error_flag|=~(0x0fff&RX_BUFFER_OVERSIZE_EXCEPTION);
	}
	if(rx_buffer_head==rx_buffer_tail){//È·¶¨µ±Ç°½ÓÊÕbufferÖÐÊÇ·ñÎª¿ÕÊý¾Ý£¬·µ»Ø0xffff±íÊ¾¿ÕÊý¾Ý
		return 0xffff;
	}
	
	while((rx_buffer[StringPtr]=='\0')&&(rx_buffer[StringPtr]=='\n')){
		
		ReadString[StringLenth]=rx_buffer[StringPtr];//¶ÁÈ¡ÇøÓò
		
		StringLenth++;
		StringPtr++;		
		if(StringPtr=RX_BUFFER_SIZE) StringPtr=0;
		
		if(StringPtr==rx_buffer_tail){ //Èç¹ûÕÒ±éÁËbuffer£¬²¢Ã»ÓÐ¿´µ½½áÊø·û£¬·¢ÏÖ²»ÊÇ×Ö·û´®»òÕßÐÐÊý
			error_flag|=RX_READLN_NOEND;//Å×³öÎÞ½áÊø´íÎó
			ReadString[0]='\0';//±ê¼Ç¶ÁÈ¡Ê§Ð§
			isNoEnd=1;//ÖÃÎª´íÎó±ê¼ÇÎ»
			break;
		}
		else if(StringLenth==maxLenth-1){
			ReadString[StringLenth]='\0';
			break;	
		}
	}
	//·µ»Ø´íÎóÇøÓò
	if(isNoEnd){
		if(isOversizeOccur) return RX_READLN_NOEND|RX_BUFFER_OVERSIZE_EXCEPTION;
		else return RX_READLN_NOEND;
	}
	else{//Èç¹û¶ÓÁÐÖÐÓÐ½áÊø·û£¬²»ÂÛÊÇ·ñ·¢Éú¹ýÒç³ö£¬¶¼ÐèÒªºóÒÆheadÖ¸Õë
		ReadString[StringLenth++]=rx_buffer[StringPtr++];//½«½áÎ²¿½±´³öÀ´
		if(ReadString[StringLenth]=='\n') ReadString[StringLenth++]='\0';//Èç¹ûÊÇ»»ÐÐ½áÎ²£¬Ä©Î²Ó¦µ±Ôö¼Ó½áÊø·û
		
		rx_buffer_head=StringPtr;//ÒÆ¶¯rx_buffer_headÖ¸Õë£¬½«Æä·ÅÔÚ½áÎ²µÄÏÂÒ»¸ö×Ö½Ú´¦
		
		error_flag&=~(RX_BUFFER_OVERSIZE_EXCEPTION);//Çå³þÒç³ö·¢Éú´íÎó±êÖ¾Î»
		
		if(isOversizeOccur) return RX_BUFFER_OVERSIZE_EXCEPTION;
		else return 0;
	}
} */

/*
	º¯ÊýÃû£ºWriteSerial
	²ÎÊý£ºpayload£¨char£©Òª·¢ËÍµÄ×Ö·û
	¹¦ÄÜ£º´Ó´®¿ÚÖÐ·¢ËÍÒ»¸ö×Ö·û
	·µ»ØÀàÐÍ£ºint
	·µ»Ø£º0(Íê³É¹¦ÄÜ£¬Õý³£ÍË³ö)
	      TX_BUSY_ALWAYS_EXCEPTION·¢ËÍ³¬Ê±
	Ê¹ÓÃÈ«¾Ö±äÁ¿£ºTx_Busy¡¢error_flag
*/
int WriteSerial(char payload){
	
	int SerialBusyCount=0;//?????????
	while(Tx_Busy){	//?????€???????????
		SerialBusyCount++;
		if(SerialBusyCount>TX_MAX_WAITTIME){//??€???????????????????????
			error_flag|=TX_BUSY_ALWAYS_EXCEPTION;
			return error_flag;
		}		
	}

	SBUF=payload;
	
	SerialBusyCount=0;
	Tx_Busy=1;
	while(Tx_Busy){
		SerialBusyCount++;
		if(SerialBusyCount>TX_MAX_WAITTIME){//??€???????????????????????
			error_flag|=TX_BUSY_ALWAYS_EXCEPTION;
			return error_flag;
		}
	}
	return 0;
}
/*
	º¯ÊýÃû£ºWriteString
	²ÎÊý£ºpayload£¨char *£©Òª·¢ËÍµÄ×Ö·û´®
	¹¦ÄÜ£º´Ó´®¿ÚÖÐ·¢ËÍÒ»¸ö×Ö·û´®
	·µ»ØÀàÐÍ£ºint
	·µ»Ø£º0(Íê³É¹¦ÄÜ£¬Õý³£ÍË³ö)
	      TX_BUSY_ALWAYS_EXCEPTION·¢ËÍ³¬Ê±
	Ê¹ÓÃÈ«¾Ö±äÁ¿£º
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
 ÖÐ¶ÏÏòÁ¿ÇøÓò
 *******************************************************************************************************************
*/	
/*
	ÖÐ¶ÏÀàÐÍ£ºUart´®¿ÚÖÐ¶Ï
	ÖÐ¶ÏID£º4
	º¯ÊýÃû£ºserial
	ÊÇ·ñ¹Ø±ÕÖÐ¶Ï£ºÈ«¾Ö¹Ø±Õ
	¹¦ÄÜ£ºÑÓÊ±
	Ê¹ÓÃÈ«¾Ö±äÁ¿£ºrx_buffer_head¡¢rx_buffer_tail¡¢rx_buffer_next¡¢rx_buffer[]¡¢Tx_Busy
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