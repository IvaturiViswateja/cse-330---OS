#include "sem.h"
#include "threads.h"

struct TCB_t *runQ = NULL; 
struct semaphore *rsem = NULL, *wsem = NULL;

int rwc = 0,wwc = 0,rc = 0,wc = 0;  
int i=0;


void readerExit(int rid);
void writerExit(int wid);

void readerEntry(int rid)				
{
	int r = 0;
	while(r<1){
		if (wwc > 0 || wc > 0) {			
			rwc++;
			P(rsem);
			rwc--;			
		}
		rc++;
		//if (rwc > 0){
		//	V(rsem);
		//}
		printf("\n This is the %d th reader reading value i = %d for the first time \n", rid, i );
		r++;   
		yield();
	}
	readerExit(rid);
}

void readerExit(int rid)
{
	printf("\n This is the %d th reader reading value i = %d for the second time \n", rid, i );
	rc--;
	if (rc == 0 && wwc > 0) {	
	    V(wsem);
	} 
	if (runQ == NULL){
		exit(1);   
	}
	TCB_t *rdel = DelQueue(&runQ);
	swapcontext(&(rdel->context),&(runQ->context));
}

void writerEntry(int wid)				
{
	int w = 0;
	while(w<1){
		if (rc > 0 || wc > 0 || rwc > 0 || wwc > 0) {				
			wwc++;
			P(wsem);
			wwc--;
		}
		wc++;
		i = -1* wid;
		printf("\n This is the %d th writer writing value i = %d \n", -1*wid, i );
		w++;
		yield();
	}
	writerExit(wid);
}

void writerExit(int wid)						
{
	printf("\n This is the %d th writer verifying value i = %d \n", -1*wid, i );
	wc--;
	if (rwc > 0) {	
		for (int j =1; j<=rwc; j++){
			V(rsem);
		}
	} else if (wwc > 0) {		
	    V(wsem);
	} 
	if (runQ == NULL){
		exit(1);
	}
	TCB_t *wdel = DelQueue(&runQ);
	swapcontext(&(wdel->context),&(runQ->context));
}
                
int main(int argc, char *argv[]) {    
	char comma;
	int r=0, w=0;

	scanf("%d%c%d",&r,&comma,&w);
	
	runQ = NewItem();
	rsem = (struct semaphore*) malloc(sizeof( struct semaphore));
	wsem = (struct semaphore*) malloc(sizeof(struct semaphore));  
	
	InitQueue(&runQ);
	IntiSem(wsem,0);
	IntiSem(rsem,0);   

	for (int i=0; i< r+w; i++){
		int threads=0;   
		scanf("%d",&threads);  
		if (threads >=0){  
			start_thread(*readerEntry,threads);
		}
		else{
			start_thread(*writerEntry,threads);
		}
	}
	run();
}