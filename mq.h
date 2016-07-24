#ifndef MQ_H
#define MQ_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define MQ_HTML 0
#define MQ_URL 1

mqd_t mqd_html;
mqd_t mqd_url;

static struct mq_attr attr_html;
static struct mq_attr attr_url;

void init_mq();
void send_html(const char* str);
char* recv_html();
void send_url(const char* str);
char* recv_url();
void print_attr(int type);

void close_mq();

void init_mq(){

	
	int flags;

	flags = O_RDWR|O_CREAT;

	printf("create HTMLmqueue.\n");

	//attr_html.mq_maxmsg=15;
	//attr_html.mq_msgsize=10005;
	if((mqd_html=mq_open("/html",flags,FILE_MODE,NULL))==-1){
		perror("mq_open() error");
		exit(-1);
	}

	mq_getattr(mqd_html,&attr_html);

	print_attr(MQ_HTML);

	printf("create URLmqueue.\n");

	//attr_url.mq_maxmsg=15;
	//attr_url.mq_msgsize=10005;
	if((mqd_url=mq_open("/url",flags,FILE_MODE,NULL))==-1){
		perror("mq_open() error");
		exit(-1);
	}

	mq_getattr(mqd_url,&attr_url);

	print_attr(MQ_URL);
}

void send_html(const char* str){

	printf("send_html\n");
	size_t  len;
	int  prio;

	len=strlen(str);
	printf("%d\n",(int)len);
	prio=1;

	if(mq_send(mqd_html,str,len,prio) == -1)
     {
         perror("mq_HTML_send() error:");
         exit(-1);
     }else{
    	printf("send success!\n");
     }
     print_attr(MQ_HTML);
}

void send_url(const char* str){
	printf("send_url\n");
	size_t  len;
	int  prio;

	len=strlen(str);
	prio=1;

	if(mq_send(mqd_url,str,len,prio) == -1)
     {
         perror("mq_URL_send() error:");
         exit(-1);
     }else{
    	printf("send success!\n");
     }
     print_attr(MQ_URL);
}


char* recv_html(){

	printf("recv_html\n");
	ssize_t n;
	int  prio;
	void *buff;

	buff = malloc(attr_html.mq_msgsize);
    if((n = mq_receive(mqd_html,buff,attr_html.mq_msgsize,&prio)) == -1)
    {
		perror("mq_html receive error: ");
		exit(-1);
	}
	if(n==0){
		return 0;
	}
	char* buf=(char*)buff;
	buf[n]='\0';
	printf("read %ld bytes,priority = %u\n",(long) n,prio);
	return buf;
}

char* recv_url(){
	printf("recv_url\n");
	ssize_t n;
	int  prio;
	void *buff;

	buff = malloc(attr_url.mq_msgsize);
    if((n = mq_receive(mqd_url,buff,attr_url.mq_msgsize,&prio)) == -1)
    {
		perror("mq_html receive error: ");
		printf("mq_html receive error:");
		exit(-1);
	}
	if(n==0){
		return 0;
	}
	char* buf=(char*)buff;
	buf[n]='\0';
	printf("read %ld bytes,priority = %u\n",(long) n,prio);
	return buf;
}

void print_attr(int type){
	if(mq_getattr(mqd_html,&attr_html)<0){
		printf("get the attr_html failed!\n");
	}
	if(mq_getattr(mqd_url,&attr_url)<0){
		printf("get the attr_url failed!\n");
	}
	if(type==MQ_HTML){
		printf("max msgs=%ld, max byte/msg=%ld\ncurrently on queue = %ld\n",attr_html.mq_maxmsg,attr_html.mq_msgsize,attr_html.mq_curmsgs);
	}else if(type==MQ_URL){
		printf("max msgs=%ld, max byte/msg=%ld\ncurrently on queue = %ld\n",attr_url.mq_maxmsg,attr_url.mq_msgsize,attr_url.mq_curmsgs);
	}else{
		printf("type wrong!\n");
	}

}

void close_mq(){
	mq_close(mqd_html);
	mq_unlink("/html");
	mq_close(mqd_url);
	mq_unlink("/url");
}

#endif