#pragma once

#include <windows.h>

extern HWND g_hwnd;
extern HWND g_title;
extern HWND g_textbox;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);