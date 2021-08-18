/*  
 * Project 1 * SaiSVIvaturi 
 */
#include "threads.h"
#include <stdio.h>

int a,b,count = 0;
char comma;
TCB_t *runQ = 0;

void func1();

int main(int argc, char *argv[]) {
	int b;
	scanf("%d%c%d",&a,&comma,&b);
	//a = atoi(argv[1]);
	//b = atoi(argv[2]);
	if (a == 0)
	{
		printf("No Threads\n");
		return (1);
	}
	else if (b == 0)
	{
		return (0);
	}
	TCB_t* threads[a];
	InitQueue(&runQ);
	for (int m = 1; m <= a; m++)
	{
		start_thread(func1,m,b);	
	}
	run();
	return (0);
}

void func1(int threadid, int bb) {
	int var1 = 0;
	int var2 = 0;
	while(1)
	{	
		count = count + threadid;
		var1++;
		printf("\n This is %d th execution of thread %d with global var value %d \n",var1, threadid,count);
		yield();
		var2++;
		if (var2 >= bb)
		{
			break;
		}
	}
}




