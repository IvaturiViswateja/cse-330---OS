#ifndef SEM_H
#define SEM_H
#include "threads.h"

typedef struct semaphore {
    int value;
    struct TCB_t *waitq;
}semaphore;

void voidFunc(int irrelevent)
{
    return;
}

void IntiSem(semaphore *sem,int value){
    sem->waitq = (struct TCB_t*) malloc(sizeof(struct TCB_t));
    InitQueue(&sem->waitq);
    sem->value = value;
    return;
}

void P(semaphore *sem) { 
    while (sem->value <= 0){
        /*if (tids >=0) {
            printf("\n Producer %d is waiting \n", tids);
        }
        else{
            printf("\n Consumer %d is waiting \n",-1*tids);
        }*/
        struct TCB_t *currThread = DelQueue(&runQ); 
        AddQueue(&(sem->waitq),currThread);
        if(runQ == NULL)
        {
            start_thread(voidFunc, 2);
        }
        swapcontext(&(currThread->context),&(runQ->context));
    }
    sem->value--;
}

void V(semaphore *sem){
    if (sem->waitq != NULL){
        struct TCB_t *vThread = DelQueue(&sem->waitq); 
        AddQueue(&(runQ),vThread);
    }
    sem->value++;
}

#endif