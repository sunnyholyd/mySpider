#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <event2/event.h>
#include <event.h>
#include <fcntl.h>
#include "mq.h"
#include "url.h"

#define SERVERPORT 80
#define RECV_SIZE 8050


void initSocket();
void makeRequestAndReceive(struct url *adr);
void makeHttpRequest(int sock,short event,void *arg);
void revHttpResponse(int sock,short event,void *arg);

struct event_base* base;

void initSocket(){

}

void makeRequestAndReceive(struct url *adr){
	printf("makeRequestAndReceive\n");

	struct sockaddr_in server_addr;
	int sockfd;

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){//get a socket fd
		printf("socket build failed!\n");
		return ;
	}

	//flags=fcntl(sockfd,F_GETFL);
	//flags|=O_NONBLOCK;
	//fcntl(sockfd,F_SETFL,O_NONBLOCK);

	memset(&server_addr,0,sizeof(struct sockaddr_in));//init sockaddr
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(SERVERPORT);
	server_addr.sin_addr.s_addr=inet_addr(adr->host);

	if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr_in))==-1){
		printf("connect %s failed!\n",adr->host);
		close(sockfd);
		return ;
	}
	struct event send_ev;

	base=event_base_new();
	
	event_set(&send_ev,sockfd,EV_WRITE,makeHttpRequest,adr);
	event_base_set(base,&send_ev);

	event_add(&send_ev,NULL);

	event_base_dispatch(base);

}

void makeHttpRequest(int sock,short event,void *arg){
	printf("makeHttpRequest\n");
	struct url *adr=(struct url *)arg;

	//char host[20];
	//sprintf(host,adr->host);

	//char path[1024];
	//sprintf(path,adr->path);//*request file path

	char req[1024];
	char postdata[1]={'\0'};
	sprintf(req, "POST %s HTTP/1.1\r\n",adr->path);
	sprintf(req, "%sHost: %s\r\n", req,adr->host);
	sprintf(req, "%sUser-Agent:Mozilla/4.0 (compatible: MSIE8.0; Windows NT6.0; Trident/4.0\r\n",req);
	//sprintf(req, "%sAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-us,en;q=0.5\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\nKeep-Alive: 300\r\nConnection: keep-alive\r\n",req);
	//sprintf(req, "%sContent-Type: application/x-www-form-urlencoded\r\n",req);
	sprintf(req, "%sConnection:Close\r\n", req);
	sprintf(req, "%s\r\n%d",req,(int)strlen(req)); 		
	// puts(req);
	if(send(sock,req,(int)strlen(req),0)==-1){
		printf("send error!");
		close(sock);
		return ;
	}

	printf("Http request success!\n");

	struct event rev_ev;

	event_set(&rev_ev,sock,EV_READ,revHttpResponse,&rev_ev);
	event_base_set(base,&rev_ev);
	event_add(&rev_ev,NULL);

	if(adr!=NULL){
		free(adr);
	}
	
}
void revHttpResponse(int sock,short event,void *arg){
	printf("rev Http response\n");

	char buf[RECV_SIZE];
	int buflen,i=0;
	char temp[1000];
	while((buflen=recv(sock,temp,1000,0))>=0){
		
		if(mq_getattr(mqd_html,&attr_html)<0){
			printf("get the attr_html failed!\n");
		}
		if(attr_html.mq_curmsgs>=5){
			printf("attr_html.mq_curmsgs>=5 so close receive.\n");
			close(sock);
			return ;
		}

		strcat(buf,temp);
		memset(temp,'\0',1000);
		
		if(buflen==0){
			printf("len=%d\n",(int)strlen(buf));
			printf("send the message\n");
			send_html(buf);
			break;
		}
		i++;
		if(i>=7){
			printf("len=%d\n",(int)strlen(buf));
			printf("send the message\n");
			send_html(buf);

			memset(buf,'\0',RECV_SIZE);
			i=0;
		}

	}
	printf("recv done!\n");
	close(sock);

	if(buflen<0){
		if(errno==EAGAIN){
			printf("recv done!\n");
			return ;
		}
	}

		
	
}

void* asyncCrawling(void * arg){
	while(1){
		//sleep(2);
		char* buf;
		buf=recv_url();
		//puts(buf);
		

		struct url *adr;
		adr=(struct url*)malloc(sizeof(struct url));
		//strcpy(adr->host,"121.40.90.197");  
		//strcpy(adr->path,"/");

		parseURL(buf,adr->host,adr->path);

		
		if(buf==NULL){
			printf("free buf failed!");	
		}else{
			free(buf);	
		}
		
		makeRequestAndReceive(adr);

	}
	
}