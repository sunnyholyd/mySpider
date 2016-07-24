#ifndef URL_H
#define URL_H

#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

static char stay_ip[30];
struct url{
	char host[1024];
	char path[1024];
};

void parseURL(char* temp,char* ip,char* path){
	printf("adr:%s",temp);
	
	char head[8];
	char adr[1024];
	strncpy(head,temp,7);
	head[7]='\0';
	
	char http[]="http://";
	int flag_res=0;
	if(strcmp(head,http)==0){//检索"http://"字符串并删除
		strcpy(adr,temp+7);
	}else{
		strcpy(adr,temp);
		flag_res=1;
	}

	if(flag_res==1){
		strcpy(ip,stay_ip);
		strcpy(path,adr);
	}else{

		char host[1024];
		char *p=strchr(adr,'/');//分割域名和资源地址
		if(p==NULL){
			sprintf(path,"/");
			strcpy(host,adr);
		}else{
			strcpy(path,p);//资源地址	
			strncpy(host,adr,p-adr);
			host[p-adr]='\0';
		}
		printf("host:%s\n",host);	
		struct hostent *h;
	
		if((h=gethostbyname(host))==NULL){
			printf("get the IP error!");
		}
	
		inet_ntop(h->h_addrtype,h->h_addr_list[0],ip,16);//ip地址
		sprintf(stay_ip,"%s",ip);
		printf("stay_ip:%s\n",stay_ip);
	}
	printf("path:%s\n",path);	
}

#endif
