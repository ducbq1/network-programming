// Thread1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <windows.h>

#pragma warning(disable:4996)

CRITICAL_SECTION cs;

DWORD WINAPI helloFunc(LPVOID p)
{
    int* pi = (int*)p;
    
    EnterCriticalSection(&cs);

    FILE* f = fopen("C:\\test\\test.txt", "a");
    for (int i = 0; i < 10; i++)
    {
        fprintf(f, "Thread %d - %d\n", *pi, i);
        Sleep(500);
    }
    fclose(f);

    LeaveCriticalSection(&cs);

    return 0;
}

int main()
{
    int params[4];
    HANDLE hThreads[4];

    InitializeCriticalSection(&cs);

    for (int i = 0; i < 4; i++)
    {
        params[i] = i;
        hThreads[i] = CreateThread(0, 0, helloFunc, &params[i], 0, 0);
    }

    WaitForMultipleObjects(4, hThreads, TRUE, INFINITE);

    DeleteCriticalSection(&cs);
}
