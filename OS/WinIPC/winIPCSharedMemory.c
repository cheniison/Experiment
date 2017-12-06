#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define EXE_NAME L"../Debug/WinIPC.exe"

int main(int argc, char *argv[])
{
	LPCWSTR shm_name = L"ExpWinIPCSharedMemory";
	LPCWSTR sem_name_empty = L"ExpWinIPCSemaphoreEmpty", sem_name_full = L"ExpWinIPCSemaphoreFull";
	HANDLE shm_handle, sem_handle_empty, sem_handle_full;
	LPVOID buffer_ptr;		/* 共享内存指针 */
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	BOOL res;
	char string[BUFFER_SIZE];


	shm_handle = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, shm_name);

	if (NULL == shm_handle) {
		/* 打开失败，是父进程 */

		ZeroMemory(&pi, sizeof(pi));
		ZeroMemory(&si, sizeof(si));
		
		/* 创建共享内存 */
		shm_handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFFER_SIZE, shm_name);\
		buffer_ptr = MapViewOfFile(shm_handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);	

		/* 创建信号量 */
		sem_handle_empty = CreateSemaphore(NULL, 0, 1, sem_name_empty);
		sem_handle_full = CreateSemaphore(NULL, 1, 1, sem_name_full);

		/* 创建进程 */
		printf("创建子进程： %s\n", EXE_NAME);
		res = CreateProcess(EXE_NAME, "", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		if (FALSE == res) {
			/* 创建子进程失败 */
			printf("创建子进程失败\n");
			exit(1);
		}
		printf("创建子进程成功\n");
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);

		for (;;) {
			printf("\n输入消息： ");
			fgets(string, BUFFER_SIZE, stdin);

			printf("等待写入共享内存。。。\n");
			/* full信号量-1 */
			WaitForSingleObject(sem_handle_full, INFINITE);
			strcpy((char *)buffer_ptr, string);
			printf("写入共享内存成功\n");
			/* empty信号量+1 */
			ReleaseSemaphore(sem_handle_empty, 1, NULL);
		}
	}
	else {
		/* 打开成功，是子进程 */

		/* 打开共享内存 */
		buffer_ptr = MapViewOfFile(shm_handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);

		/* 打开信号量 */
		sem_handle_empty = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, sem_name_empty);
		sem_handle_full = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, sem_name_full);

		for (;;) {
			printf("等待其他进程写入数据。。。\n");
			/* empty信号量-1 */
			WaitForSingleObject(sem_handle_empty, INFINITE);
			printf("读出： %s\n", (char *)buffer_ptr);
			/* full信号量-1 */
			ReleaseSemaphore(sem_handle_full, 1, NULL);
		}
			
	}

	return 0;
}