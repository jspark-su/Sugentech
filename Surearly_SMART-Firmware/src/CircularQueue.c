#include "main.h"

void QueueInit(Queue * pq)
{
	pq->front = 0;
	pq->rear = 0;
}

int QIsEmpty(Queue * pq)
{
    if(pq->front == pq->rear){
		return TRUE;
        }
	else{
		return FALSE;
        }
}

int NextPosIdx(int pos)
{
    if(pos == QUE_LEN-1){
		return 0;
        }
	else{
		return pos+1;
        }
}

void Enqueue(Queue * pq, Data data)
{
	if(NextPosIdx(pq->rear) == pq->front)// Memory Full
	{
		//printf("Queue Memory Error!");
		//exit(-1);
	}
	else
	{
		pq->rear = NextPosIdx(pq->rear);
		pq->queArr[pq->rear] = data;
	}
}

Data Dequeue(Queue * pq)
{
	if(QIsEmpty(pq))// Memory Empty
	{
		//printf("Queue Memory Error!");
		//exit(-1);
		return 0;
	}
	else
	{
		pq->front = NextPosIdx(pq->front);
		return pq->queArr[pq->front];
	}
}

Data QPeek(Queue * pq)
{
	if(QIsEmpty(pq))
	{
		//printf("Queue Memory Error!");
		//exit(-1);
	}

	return pq->queArr[NextPosIdx(pq->front)];
}