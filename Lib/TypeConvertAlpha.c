#include "TypeConvertAlpha.h"
int UInt_to_String(int number,char *str){
	char temp[7];
	int i,j;
	temp[0]=number/100000;
	number-=temp[0]*100000;
	temp[1]=number/10000;
	number-=temp[1]*10000;
	temp[2]=number/1000;
	number-=temp[2]*1000;
	temp[3]=number/100;
	number-=temp[3]*100;
	temp[4]=number/10;
	temp[5]=number%10;
	for(i=0;i<6;i++){
		if(temp[i]!=0) break;
	}
	for(j=0;j<6-i;j++){
		str[j]=temp[i+j]+'0';
	}
	str[j]='\0';
	return 0;
}
	
int Hex_to_String(char hex,char *str){
	str[0]='0';
	str[1]='x';
	str[2]=(hex&0xf0)>>4;
	if(str[2]>=10) str[2]='A'+str[2]-10;
	else	str[2]='0'+str[2];
	str[3]=hex&0xf;
	if(str[3]>=10) str[3]='A'+str[3]-10;
	else	str[3]='0'+str[3];
	str[4]='\0';
	return 0;
}