#include <windows.h>

#include "gui.h"

HWND g_hwnd;
HWND g_title;
HWND g_textbox;

int main()
{
	CreateMutex(0, 1, L"ROBLOX_singletonEvent");

	HINSTANCE hInstance = GetModuleHandle(NULL);

	return WinMain(hInstance, NULL, NULL, SW_SHOWDEFAULT);
}
