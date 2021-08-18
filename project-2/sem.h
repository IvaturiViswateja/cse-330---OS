#ifndef SEM_H
#define SEM_H
#include "threads.h"

//extern FILE *fp;
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

void P(semaphore *sem, int tids) { 
    //printf("this is p");
    //printf("%d\n",sem->value);
    while (sem->value <= 0){
        if (tids >=0) {
            //fprintf(fp,"\n Producer %d is waiting \n", tids);
            printf("\n Producer %d is waiting \n", tids);
        }
        else{
            //fprintf(fp,"\n Consumer %d is waiting \n",-1*tids);
            printf("\n Consumer %d is waiting \n",-1*tids);
        }
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
    //printf("this is v");
    //printf("%d\n",sem->value);
    if (sem->waitq != NULL){
        struct TCB_t *vThread = DelQueue(&sem->waitq); 
        AddQueue(&(runQ),vThread);
    }
    sem->value++;
}

#endif