#include <windows.h>
#include <tlhelp32.h>

#include "gui.h"

HWND g_hwnd;
HWND g_title;
HWND g_textbox;
HWND g_status;
HWND g_startup;
HWND g_closeall;

/*
 * ROBLOX allows only one client by creating a mutex with this name. If we own it
 * before any client starts, every client thinks it is the first one.
 * (Older versions of MultiRoblox created "ROBLOX_singletonEvent", which current
 * ROBLOX uses as an *event*, not the single-instance lock, so it only worked by luck.)
 */
#define ROBLOX_MUTEX_NAME L"ROBLOX_singletonMutex"
#define SELF_MUTEX_NAME   L"MultiRoblox_SelfInstance"
#define ROBLOX_EXE_NAME   L"RobloxPlayerBeta.exe"

static HANDLE g_roblox_mutex;

int count_roblox(void)
{
	int count = 0;
	PROCESSENTRY32W pe = { 0 };
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshot == INVALID_HANDLE_VALUE)
		return 0;

	pe.dwSize = sizeof(pe);
	if (Process32FirstW(snapshot, &pe))
	{
		do
		{
			if (_wcsicmp(pe.szExeFile, ROBLOX_EXE_NAME) == 0)
				count++;
		} while (Process32NextW(snapshot, &pe));
	}

	CloseHandle(snapshot);
	return count;
}

void close_all_roblox(void)
{
	PROCESSENTRY32W pe = { 0 };
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshot == INVALID_HANDLE_VALUE)
		return;

	pe.dwSize = sizeof(pe);
	if (Process32FirstW(snapshot, &pe))
	{
		do
		{
			if (_wcsicmp(pe.szExeFile, ROBLOX_EXE_NAME) == 0)
			{
				HANDLE process = OpenProcess(PROCESS_TERMINATE | SYNCHRONIZE, FALSE, pe.th32ProcessID);
				if (process != NULL)
				{
					TerminateProcess(process, 0);
					WaitForSingleObject(process, 5000);
					CloseHandle(process);
				}
			}
		} while (Process32NextW(snapshot, &pe));
	}

	CloseHandle(snapshot);
}

/* Take ROBLOX's single-instance mutex. If ROBLOX is already running it owns the
 * mutex, so offer to close it instead of making the user use Task Manager. */
static BOOL acquire_roblox_mutex(void)
{
	int i;

	if (count_roblox() > 0)
	{
		int answer = MessageBoxW(NULL,
			L"ROBLOX is already open.\n\n"
			L"MultiRoblox must start BEFORE ROBLOX, so ROBLOX has to be closed first.\n\n"
			L"Close all ROBLOX windows now?",
			L"MultiRoblox", MB_YESNO | MB_ICONWARNING);

		if (answer != IDYES)
			return FALSE;

		close_all_roblox();
	}

	/* ROBLOX may need a moment to release the mutex after it closes. */
	for (i = 0; i < 40; i++)
	{
		HANDLE mutex = CreateMutexW(NULL, TRUE, ROBLOX_MUTEX_NAME);
		DWORD error = GetLastError();

		if (mutex != NULL && error != ERROR_ALREADY_EXISTS)
		{
			g_roblox_mutex = mutex;
			return TRUE;
		}

		if (mutex != NULL)
			CloseHandle(mutex);

		/* A ROBLOX window may have been opened in the meantime. */
		if (i == 20)
			close_all_roblox();

		Sleep(250);
	}

	MessageBoxW(NULL,
		L"Could not take over ROBLOX's single-instance lock.\n\n"
		L"Close every ROBLOX window (also in Task Manager) and start MultiRoblox again.",
		L"MultiRoblox", MB_OK | MB_ICONERROR);
	return FALSE;
}

int main()
{
	HANDLE self_mutex = CreateMutexW(NULL, TRUE, SELF_MUTEX_NAME);
	if (self_mutex != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBoxW(NULL, L"MultiRoblox is already running.", L"MultiRoblox", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	if (!acquire_roblox_mutex())
		return 0;

	HINSTANCE hInstance = GetModuleHandle(NULL);

	return WinMain(hInstance, NULL, NULL, SW_SHOWDEFAULT);
}
