#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "os_2.h"

int main()
{
    system("Pause");
    HANDLE hMapping = MakeShared();

    // 打开文件映射
    HANDLE hFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "BUFFER");
    if (hFileMapping == NULL)
    {
        printf("OpenFileMapping error.\n");
        exit(0);
    }

    // 映射地址空间
    LPVOID pFile = MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (pFile == NULL)
    {
        printf("MapViewOfFile error.\n");
        exit(0);
    }

    BuffPool *pbuff = (BuffPool *) pFile;
    pbuff->head = 0;
    pbuff->tail = 0;

    HANDLE empty = CreateSemaphore(NULL, 6, 6, "EMPTY");
    HANDLE full = CreateSemaphore(NULL, 0, 6, "FULL");
    HANDLE mutex = CreateMutex(NULL, FALSE, "MUTEX");

    // 停止内存映射，关闭句柄
    UnmapViewOfFile(pFile);
    pFile = NULL;
    CloseHandle(hFileMapping);

    // 创建子进程
    PROCESS_INFORMATION producer[2];
    PROCESS_INFORMATION consumer[3];

    // 生产者子进程创建
    WaitForSingleObject(mutex, INFINITE);
    for (int i = 0; i < NUM_PRODUCER; i++)
    {
        printf("Creating producer %d:\n", i + 1);
        TCHAR szFilename[MAX_PATH];
        TCHAR szCmdLine[MAX_PATH];
        PROCESS_INFORMATION pi;
        sprintf(szFilename, "./producer.exe");
        sprintf(szCmdLine, "\"%s\"", szFilename);

        STARTUPINFO si;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(si);

        if (CreateProcess(szFilename, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi));
        else
            printf("Create process error.\n");
        producer[i] = pi;
    }

    // 消费者子进程创建
    for (int i = 0; i < NUM_CONSUMER; i++)
    {
        printf("Creating consumer %d:\n", i + 1);
        TCHAR szFilename[MAX_PATH];
        TCHAR szCmdLine[MAX_PATH];
        PROCESS_INFORMATION pi;
        sprintf(szFilename, "./consumer.exe");
        sprintf(szCmdLine, "\"%s\"", szFilename);

        STARTUPINFO si;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(si);

        if (CreateProcess(szFilename, szCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi));
        else
            printf("Create process error.\n");
        consumer[i] = pi;
    }
    ReleaseMutex(mutex);

    // 等待子进程结束
    for (int i = 0; i < NUM_PRODUCER + NUM_CONSUMER; i++)
    {
        if (i < NUM_PRODUCER)
            WaitForSingleObject(producer[i].hProcess, INFINITE);
        else
            WaitForSingleObject(consumer[i - NUM_PRODUCER].hProcess, INFINITE);
    }

    // 关闭句柄
    for (int i = 0; i < NUM_PRODUCER + NUM_CONSUMER; i++)
    {
        if (i < NUM_PRODUCER)
            CloseHandle(producer[i].hProcess);
        else
            CloseHandle(consumer[i - NUM_PRODUCER].hProcess);
    }
    CloseHandle(hMapping);
    hMapping = INVALID_HANDLE_VALUE;
    CloseHandle(mutex);
    CloseHandle(full);
    CloseHandle(empty);
    system("Pause");
    return 0;
}
