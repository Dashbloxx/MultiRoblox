#pragma once

#include <windows.h>

extern HWND g_hwnd;
extern HWND g_title;
extern HWND g_textbox;
extern HWND g_status;
extern HWND g_startup;
extern HWND g_closeall;

int count_roblox(void);
void close_all_roblox(void);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
