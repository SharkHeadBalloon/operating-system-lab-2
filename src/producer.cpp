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
    for (int counter = 0; counter < PRODUCER_ROUND; counter ++)
    {// 生产者生产12次
        Sleep(rand() % 5000);
        WaitForSingleObject(empty, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        if (HandInProduct(pbuff, counter)) 
            printf("Producer %d: Round %d\n", GetCurrentProcessId(), counter + 1);
        else 
            printf("Hand in error.\n");
        ShowBuff(pbuff, false);
        ReleaseSemaphore(full, 1, NULL);
        ReleaseMutex(mutex);
    }
    
    UnmapViewOfFile(pFile);
    pFile = NULL;
    CloseHandle(hFileMapping);
    return 0;
}
