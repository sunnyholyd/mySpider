#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "asyncIO.h"
#include "threadpool.h"
#include "FSManalyzer.h"

#define THREAD_NUM 20
int main(int argc, char const *argv[])
{

	//init mq
	close_mq();

	init_mq();

	send_url(argv[1]);
/*
	pthread_t crawler;
	if((pthread_create (&crawler, NULL, asyncCrawling,NULL)!=0)){
		printf("create crawler pthread failed!\n");
		return 0;
	}

	pthread_t analysis;
	if((pthread_create (&analysis, NULL, analysis_thread_dispatch,NULL)!=0)){
		printf("create analysis pthread failed!\n");
		return 0;
	}

	pthread_join(crawler, NULL);
	printf("quit the crawler thread!\n");
	pthread_join(analysis, NULL);
	printf("quit the analysis thread!\n");
	*/
	pool_init(THREAD_NUM);

	sleep(1);
	
	int i;
	for(i=0;i<THREAD_NUM;i++){
		if(i==THREAD_NUM-1){
	 		pool_add_worker(asyncCrawling,NULL);
		}else{
			pool_add_worker(Analysis,NULL);	
		}
	}
	
	printf("nihao!\n");
	sleep(2);
	pool_destroy();

	close_mq();

	return 0;
}

