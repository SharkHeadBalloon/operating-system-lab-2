#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "os_2.h"

int main()
{
    // 打开文件映射
    HANDLE hFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "BUFFER");
    if (hFileMapping == NULL)
    {
        printf("OpenFileMapping error.\n");
        exit(0);
    }

    LPVOID pFile = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (pFile == NULL)
    {
        printf("MapViewOfFile error.\n");
        exit(0);
    }

    BuffPool *pbuff = (BuffPool *) pFile;

    // 打开信号量
    HANDLE empty = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "EMPTY");
    HANDLE full = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "FULL");
    HANDLE mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "MUTEX");

    srand(time(0) + GetCurrentProcessId());
    for (int counter = 0; counter < CONSUMER_ROUND; counter ++)
    {// 消费者消费8次
        Sleep(rand() % 10000);
        WaitForSingleObject(full, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        bool empty_flag = false;
        if (ConsumeProduct(pbuff)) ;
        else
        { 
            empty_flag = true;
        }
        printf("Consumer %d: Round %d\n", GetCurrentProcessId(), counter + 1);
        ShowBuff(pbuff, empty_flag);
        ReleaseSemaphore(empty, 1, NULL);
        ReleaseMutex(mutex);
    }

    UnmapViewOfFile(pFile);
    pFile = NULL;
    CloseHandle(hFileMapping);
    return 0;
}
