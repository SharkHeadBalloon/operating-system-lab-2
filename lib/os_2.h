#ifndef OS_2_H
#define OS_2_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>

#define NUM_PRODUCER 2
#define NUM_CONSUMER 3
#define PRODUCER_ROUND 12
#define CONSUMER_ROUND 8

typedef struct buff_pool
{// 缓冲区定义
    char buffer[6][10];
    int head;
    int tail;
}BuffPool;

HANDLE MakeShared();
bool HandInProduct(BuffPool *, int);
bool ConsumeProduct(BuffPool *);
bool ShowBuff(BuffPool *, bool);

#endif