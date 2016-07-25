#ifndef FSMANALYZER_H
#define FSMANALYZER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATENUM 24
#define CONDITIONS 21

typedef int state;
typedef int condition;

//×ŽÌ¬œáµã
typedef struct StateMachine {
	state current;
}stateMachine;

//±íÊŸËùÓÐ×ŽÌ¬œáµãµÄÊý×é
stateMachine s[STATENUM];

//ŒÇÂŒÁŽœÓµÄÊý×é
static char mylink[1024];

//ÁŽœÓÊý×éµÄ³€¶È
static int linkLen;

state transitionTable[STATENUM][CONDITIONS] = {
/*           0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 */
/*           <  a  A  h  H  r  R  e  E  f  F  =     "  '  /  #  >  -  ! other */
/*0结点*/    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*1结点*/    0, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,
/*2结点*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0,
/*3结点*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0,
/*4结点*/    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 4, 4,
/*5结点*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0,
/*6结点*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*7结点*/    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
/*8结点*/    8, 8, 8, 9, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 8, 8, 8,
/*9结点*/    8, 8, 8, 8, 8,10,10, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 8, 8, 8,
/*10结点*/   8, 8, 8, 8, 8, 8, 8,11,11, 8, 8, 8, 8, 8, 8, 8, 8, 0, 8, 8, 8,
/*11结点*/   8, 8, 8, 8, 8, 8, 8, 8, 8,12,12, 8, 8, 8, 8, 8, 8, 0, 8, 8, 8,
/*12结点*/   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,13,12, 8, 8, 8, 8, 0, 8, 8, 8,
/*13结点*/   8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,13,14,19, 8, 8, 0, 8, 8, 8,
/*14结点*/  17,17,17,17,17,17,17,17,17,17,17,17,14, 0,17,15, 0, 0,17,17,17,
/*15结点*/  15,15,15,15,15,15,15,15,15,15,15,15,15,16,15,15,15, 0,15,15,15,
/*16结点*/   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*17结点*/  17,17,17,17,17,17,17,17,17,17,17,17,17,18,17,17,17, 0,17,17,17,
/*18结点*/   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*19结点*/  20,20,20,20,20,20,20,20,20,20,20,20,19,20, 0,21, 0, 0,20,20,20,
/*20结点*/  20,20,20,20,20,20,20,20,20,20,20,20,20,20,23,20,20, 0,20,20,20,
/*21结点*/  21,21,21,21,21,21,21,21,21,21,21,21,21,21,22,21,21, 0,21,21,21,
/*22结点*/   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/*23结点*/   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};



condition CharToCondition(char c)
{
	if (c == '<') return 0;
	if (c == 'a') return 1;
	if (c == 'A') return 2;
	if (c == 'h') return 3;
	if (c == 'H') return 4;
	if (c == 'r') return 5;
	if (c == 'R') return 6;
	if (c == 'e') return 7;
	if (c == 'E') return 8;
	if (c == 'f') return 9;
	if (c == 'F') return 10;
	if (c == '=') return 11;
	if (c == ' ') return 12;
	if (c == '"') return 13;
	if (c == 39)  return 14;	//single quote
	if (c == '/') return 15;
	if (c == '#') return 16;
	if (c == '>') return 17;
	if (c == '-') return 18;
	if (c == '!') return 19;
	return 20;
}

int CheckMachineState(stateMachine *machine, char c) {
	state cur = machine->current;
	if (cur == 0) {
		linkLen = 0;
		mylink[0] = '\0';
	}
	else if (cur == 14 || cur == 19) {
		mylink[0] = c;
		mylink[1] = '\0';
		linkLen = 1;
	}
	else if (cur == 15 || cur == 17 || cur == 20 || cur == 21) {
		mylink[linkLen++] = c;
		mylink[linkLen] = '\0';
	}
	else if (cur == 16 || cur == 18 || cur == 22 || cur == 23) {
		mylink[linkLen - 1] = '\0';
		
		//the full url match success
		if (cur == 18 || cur == 23) {
			char temp[12];
			strncpy(temp, mylink,11);
			temp[11] = '\0';
			if (strcmp(temp, "javascript:") == 0) return 0;
			//UrlInit(url, link);
			//printf("url is %s,from full url\n", url->url);
		}
		//else {
		//	UrlInitWithHostPath(url, u->host, link);
		//	printf("url is %s,from relative url\n", url->url);
		//}
		//printf("url is %s\n", url->url);
		//send the url
		send_url(mylink);
		puts(mylink);
		FILE* f = fopen("url", "a+");
		fputs(mylink, f);
		fputs("\n", f);
		fclose(f);

		
	}
	else{
		linkLen = 0;
	}
	return 1;
}


stateMachine* Step(stateMachine *machine, char c) {
	condition mycondition;
	mycondition = CharToCondition(c);
	state next = transitionTable[machine->current][mycondition];
	return &s[next];
}


void* Analysis(void* arg)
{
	while(1){
		memset(mylink,'\0',1024);
		
		char *code;
		code = recv_html();
		
		if(code==NULL){
			//printf("recv html failed!\n");
			printf("code NULL");
			continue;
		}
		printf("AnalySis codes:\n");
		puts(code);
		
		stateMachine *mymachine;
		mymachine = &s[0];
		int i;
		linkLen = 0;

		for (i = 0; i < STATENUM; i++) {
			s[i].current = i;
		}
		for (i = 0; code[i]; i++) {
			CheckMachineState(mymachine, code[i]);
			mymachine = Step(mymachine, code[i]);
		}
		

		if(code!=NULL){
			free(code);
		}
	}
	

}

#endif
