#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int beginnum = 1000;

typedef struct _ProdInfo{
    int num;
    struct _ProdInfo *next;
}ProdInfo;

ProdInfo *Head = NULL;

void *thr_producter(void *arg)
{
    
    //负责在链表添加数据
    while(1){
        ProdInfo * prod = malloc(sizeof(ProdInfo));
        prod->num = beginnum++;
        printf("----%s------self=%lu----%d\n",__FUNCTION__,pthread_self(),prod->num);
        pthread_mutex_lock(&mutex);
        //add to list 
        prod->next = Head;
        Head = prod;
        pthread_mutex_unlock(&mutex);
        //发起通知
        pthread_cond_signal(&cond);
        sleep(rand()%2);
    }

    return NULL;
}

void *thr_customer(void *arg)
{
    ProdInfo *prod = NULL;
    while(1){
        //取链表的数据
        pthread_mutex_lock(&mutex);
        //if(Head == NULL) {
        while(Head == NULL) {
            pthread_cond_wait(&cond,&mutex);//在此之前必须先加锁
        }
        prod = Head;
        Head = Head->next;
        printf("----%s------self=%lu----%d\n",__FUNCTION__,pthread_self(),prod->num);
        pthread_mutex_unlock(&mutex);
        sleep(rand()%4);
        free(prod);
    }
    return NULL;
}

int main()
{
    pthread_t tid[3];
    pthread_create(&tid[0],NULL,thr_producter,NULL);
    pthread_create(&tid[1],NULL,thr_customer,NULL);
    pthread_create(&tid[2],NULL,thr_customer,NULL);


    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_join(tid[2],NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}

