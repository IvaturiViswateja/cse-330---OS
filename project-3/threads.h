#ifndef THREADS_HEADER
#define THREADS_HEADER
#include "q.h"

extern TCB_t *runQ; //global variable 

void start_thread(void (*function)(int),int tid) {
	void *stackptr = malloc(8192);
	struct TCB_t *nptr  = (TCB_t*)malloc(sizeof(TCB_t));
	init_TCB(nptr, function, stackptr, 8192,tid);

	//add this tcb
	AddQueue(&runQ, nptr);
}

void run() {
	// real code
	ucontext_t parent;     // get a place to store the main context, for faking
	getcontext(&parent);   // magic sauce
	swapcontext(&parent, &(runQ->context));  // start the first thread
}

void yield() {
	// similar to run
	//rotate the run Q.swap the context, from previous thread to the thread pointed to by RunQ
	TCB_t *temp = runQ;
	RotateQ(&runQ);
	swapcontext(&(temp->context), &(runQ->context));
}

#endif
