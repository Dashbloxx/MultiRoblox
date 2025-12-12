/*
 *	MultiRoblox - Optimized Version
 *	Allows multiple ROBLOX instances by controlling the singleton mutex
 */
// Define UNICODE for MinGW compatibility
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <Windows.h>
#include <iostream>
#include <csignal>
#include "color.h"

// Global mutex handle for cleanup
HANDLE g_hMutex = NULL;

// Signal handler for graceful shutdown
BOOL WINAPI ConsoleHandler(DWORD dwType)
{
	if (dwType == CTRL_C_EVENT || dwType == CTRL_CLOSE_EVENT)
	{
		if (g_hMutex != NULL)
		{
			ReleaseMutex(g_hMutex);
			CloseHandle(g_hMutex);
			g_hMutex = NULL;
		}
		std::cout << "\n" << dye::yellow("MultiRoblox is shutting down...") << "\n";
		std::cout << dye::grey("Mutex released. ROBLOX will return to single-instance mode.") << "\n";
		ExitProcess(0);
	}
	return TRUE;
}

int main()
{
	// Set console handler for graceful shutdown
	SetConsoleCtrlHandler(ConsoleHandler, TRUE);

	// Try to create the mutex (use CreateMutexW for explicit Unicode support)
	g_hMutex = CreateMutexW(NULL, TRUE, L"ROBLOX_singletonMutex");
	
	if (g_hMutex == NULL)
	{
		DWORD error = GetLastError();
		std::cout << dye::red("ERROR: Failed to create mutex! Error code: ") << error << "\n";
		std::cout << dye::grey("Press any key to exit...") << "\n";
		std::cin.get();
		return 1;
	}

	// Check if mutex was already owned (shouldn't happen, but good practice)
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		std::cout << dye::yellow("WARNING: Mutex already exists. Another instance might be running.") << "\n";
	}

	// Set console to UTF-8 for proper character display
	SetConsoleOutputCP(65001); // UTF-8
	
	// Display header
	std::cout << "\n";
	std::cout << dye::aqua("=========================================================") << "\n";
	std::cout << dye::aqua("|") << dye::white("              MultiRoblox v0.0.2 (Optimized)            ") << dye::aqua("|") << "\n";
	std::cout << dye::aqua("=========================================================") << "\n";
	std::cout << "\n";
	
	std::cout << dye::green("[OK]") << " " << dye::white("Mutex created successfully!") << "\n";
	std::cout << dye::green("[OK]") << " " << dye::white("You can now open multiple ROBLOX instances!") << "\n";
	std::cout << "\n";
	
	std::cout << dye::grey("Status: ") << dye::green("ACTIVE") << "\n";
	std::cout << dye::grey("CPU Usage: ") << dye::green("MINIMAL (< 0.1%)") << "\n";
	std::cout << "\n";
	
	std::cout << dye::grey("Press `Ctrl + C` or close this window to exit.") << "\n";
	std::cout << dye::grey("When closed, all ROBLOX instances except one will be terminated.") << "\n";
	std::cout << "\n";
	std::cout << dye::grey("---------------------------------------------------------") << "\n";
	std::cout << "\n";

	// Optimized loop: Sleep instead of busy-waiting
	// This reduces CPU usage from 100% to < 0.1%
	while (true)
	{
		// Sleep for 1 second - keeps mutex alive without consuming CPU
		Sleep(1000);
		
		// Optional: Check if mutex is still valid (defensive programming)
		if (g_hMutex == NULL || WaitForSingleObject(g_hMutex, 0) != WAIT_OBJECT_0)
		{
			std::cout << dye::red("ERROR: Mutex was lost! Exiting...") << "\n";
			break;
		}
	}

	// Cleanup (shouldn't reach here, but good practice)
	if (g_hMutex != NULL)
	{
		ReleaseMutex(g_hMutex);
		CloseHandle(g_hMutex);
	}

	return 0;
}