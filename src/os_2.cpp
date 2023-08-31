#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>
#include "os_2.h"

HANDLE MakeShared()
{// 创建多个进程使用的文件映射
    // 创建临时文件映射对象
    HANDLE hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
                                        PAGE_READWRITE, 0, sizeof(BuffPool), "BUFFER");

    if (hMapping == NULL)
    {
        printf("CreateFileMapping error.\n");
        exit(0);
    }

    // 在文件映射上创建视图
    LPVOID pData = MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (pData == NULL)
    {
        printf("MapViewOfFile error.\n");
        exit(0);
    }
    ZeroMemory(pData, sizeof(BuffPool));

    // 创建完解除空间映射
    UnmapViewOfFile(pData);

    // 返回映射内核对象的句柄
    return hMapping;
}

bool HandInProduct(BuffPool * buff, int counter)
{// 生产者提交产品
    char product[10];
    // 随机生成10个字符
    srand(time(0) + GetCurrentProcessId() + counter);
    for (int i = 0; i < 9; i++)
    {
        product[i] = ' ' + rand() % 95;
    }
    product[9] = '\0';
    strcpy(buff->buffer[buff->head], product);
    buff->head = (buff->head + 1) % 6;  // 提交完毕，head指向下一个插槽

    return true;
}

bool ConsumeProduct(BuffPool * buff)
{// 消费者消费产品
    char * consumed;
    consumed = new char[10];
    consumed[0] = '\0';
    strcpy(buff->buffer[buff->tail], consumed);
    buff->tail = (buff->tail + 1) % 6;  // 消费完毕，tail指向下一个插槽
    if (buff->tail == buff->head)   // 判断是否为空
        return false;

    return true;
}

bool ShowBuff(BuffPool * buff, bool empty_flag)
{// 展示缓冲区
    SYSTEMTIME time;
    GetLocalTime(&time);
    printf("Time: %02d:%02d:%02d:%d\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
    printf("Buff:");
    if (empty_flag)
        printf(" (empty)");
    printf("\n");
    for (int i = 0; i < 6; i++)
    {
        printf("Slot %d: ", i + 1);
        puts(buff->buffer[i]);
    }
    printf("\n");
    return true;
}