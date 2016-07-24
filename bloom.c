#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include <string.h>

#define MAX 21000000

int *bloom;
int init=0;

hash_function func[11]={RSHash,JSHash,PJWHash,ELFHash,BKDRHash,SDBMHash,DJBHash,DEKHash,BPHash,FNVHash,APHash};
int funclength=11;

unsigned int mod(unsigned int hash)
{
	int modnum=MAX<<5;
	while(hash>modnum)
	{
		hash-=modnum;
	}
	return hash;
}

void setbit(unsigned int hash)
{
	bloom[hash>>5]=(1<<(hash&31))|bloom[hash>>5];
}

int getbit(unsigned int  hash)
{
	return (bloom[hash>>5]>>(hash&31))&1;
}

int urlBloom(char *url)
{
   	unsigned int hash = 0;
   	int i = 0,j=0;
   	int bit;
   	int flag=1;
   	if(init==0)
   	{
   		bloom=(int*)malloc(sizeof(int)*MAX);
		memset(bloom,0,sizeof(int)*MAX);
		init=1;
   	}

	j=strlen(url);
	while(url[j -1] == 10 || url[j -1] == 13)
	{
		url[j-1] = '\0';
		j --;
	}
	for(i=0;i<funclength;i++){
		hash = func[i](url,strlen(url));
		hash = mod(hash);
		bit = getbit(hash);
		if(bit == 0) break;
	}
	if(bit==0)
	{
		flag=0;
		for(i=0;i<funclength;i++){
			hash = func[i](url,strlen(url));
			hash = mod(hash);
			setbit(hash);
		}
	}

   	return flag;
}
