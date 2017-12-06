
#include <Windows.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define EXE_NAME L"../Debug/WinIPC.exe"

int main()
{

	HGLOBAL hmem, hmem2;
	LPCWSTR mutex_name = L"WinIPC_Mutex";
	LPCWSTR sem_name_empty = L"ExpWinIPCSemaphoreEmpty", sem_name_full = L"ExpWinIPCSemaphoreFull";
	char string[BUFFER_SIZE];
	LPSTR lpstr, lpstr2;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	BOOL res;
	HANDLE hmutex, sem_handle_empty, sem_handle_full;
	DWORD dreturn;


	hmutex = CreateMutex(NULL, FALSE, mutex_name);
	dreturn = WaitForSingleObject(hmutex, 0);

	if (dreturn != WAIT_TIMEOUT) {
		/* 是父进程 */

		/* 创建信号量 */
		sem_handle_empty = CreateSemaphore(NULL, 0, 1, sem_name_empty);
		sem_handle_full = CreateSemaphore(NULL, 1, 1, sem_name_full);
		
		/* 创建子进程 */
		ZeroMemory(&pi, sizeof(pi));
		ZeroMemory(&si, sizeof(si));
		res = CreateProcess(EXE_NAME, L"", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		if (FALSE == res) {
			/* 创建子进程失败 */
			printf("创建子进程失败\n");
			exit(1);
		}
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		printf("创建子进程成功\n");

		for (;;) {

			printf("\n输入消息： ");
			fgets(string, BUFFER_SIZE, stdin);

			printf("等待打开剪切板\n");
			/* 信号量-1 */
			WaitForSingleObject(sem_handle_full, INFINITE);

			if (OpenClipboard(NULL) && EmptyClipboard()) {
				printf("打开剪切板成功\n");

				hmem = GlobalAlloc(GMEM_MOVEABLE, strlen(string) + 1);
				if (hmem == NULL) {
					printf("分配空间失败\n");
				}

				lpstr = (LPSTR)GlobalLock(hmem);
				memcpy(lpstr, string, strlen(string) + 1);
				GlobalUnlock(hmem);

				SetClipboardData(CF_TEXT, hmem);

				printf("写入成功\n");
			}
			else {
				printf("打开剪切板失败\n");
			}

			CloseClipboard();
			printf("关闭剪切板\n");
			
			/* 信号量+1 */
			ReleaseSemaphore(sem_handle_empty, 1, NULL);
		}
		

	}
	else {
		/* 是子进程 */

		/* 打开信号量 */
		sem_handle_empty = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, sem_name_empty);
		sem_handle_full = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, sem_name_full);

		for (;;) {
			printf("等待其他进程写入数据。。。\n");
			/* 信号量-1 */
			WaitForSingleObject(sem_handle_empty, INFINITE);
			
			if (OpenClipboard(NULL)) {
				printf("打开剪切板成功\n");

				hmem2 = GetClipboardData(CF_TEXT);
				if (hmem2 == NULL) {
					printf("获取数据失败\n");
				}

				lpstr2 = (LPSTR)GlobalLock(hmem2);
				printf("读出： %s\n", lpstr2);
				GlobalUnlock(hmem2);
			}
			CloseClipboard();
			printf("关闭剪切板\n");
			/* 信号量+1 */
			ReleaseSemaphore(sem_handle_full, 1, NULL);
		}

	}

	return 0;
}