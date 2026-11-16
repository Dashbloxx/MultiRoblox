#include <windows.h>
#include <stdio.h>

#include "gui.h"

#define ID_STARTUP   101
#define ID_CLOSEALL  102
#define ID_TIMER     103

#define RUN_KEY   L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define RUN_VALUE L"MultiRoblox"

static BOOL starts_with_windows(void)
{
    return RegGetValueW(HKEY_CURRENT_USER, RUN_KEY, RUN_VALUE, RRF_RT_REG_SZ, NULL, NULL, NULL) == ERROR_SUCCESS;
}

static void set_starts_with_windows(BOOL enabled)
{
    if (enabled)
    {
        WCHAR path[MAX_PATH];
        WCHAR command[MAX_PATH + 16];
        HKEY key;

        if (GetModuleFileNameW(NULL, path, MAX_PATH) == 0)
            return;

        swprintf(command, MAX_PATH + 16, L"\"%ls\" /startup", path);

        if (RegCreateKeyExW(HKEY_CURRENT_USER, RUN_KEY, 0, NULL, 0, KEY_SET_VALUE, NULL, &key, NULL) == ERROR_SUCCESS)
        {
            RegSetValueExW(key, RUN_VALUE, 0, REG_SZ, (const BYTE *)command, (DWORD)((wcslen(command) + 1) * sizeof(WCHAR)));
            RegCloseKey(key);
        }
    }
    else
    {
        RegDeleteKeyValueW(HKEY_CURRENT_USER, RUN_KEY, RUN_VALUE);
    }
}

static void update_status(void)
{
    WCHAR text[64];
    swprintf(text, 64, L"MultiRoblox is ON  -  ROBLOX windows open: %d", count_roblox());
    SetWindowTextW(g_status, text);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_STARTUP:
            set_starts_with_windows(SendMessageW(g_startup, BM_GETCHECK, 0, 0) == BST_CHECKED);
            return 0;

        case ID_CLOSEALL:
            if (MessageBoxW(hwnd, L"Close ALL ROBLOX windows?", L"MultiRoblox", MB_YESNO | MB_ICONQUESTION) == IDYES)
            {
                close_all_roblox();
                update_status();
            }
            return 0;
        }
        break;

    case WM_TIMER:
        if (wParam == ID_TIMER)
            update_status();
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, ID_TIMER);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

static void set_font(HWND control)
{
    SendMessageW(control, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = L"MULTIROBLOX_CLASS";

    RegisterClassW(&wc);

    g_hwnd = CreateWindowExW(0, L"MULTIROBLOX_CLASS", L"MultiRoblox", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 400, 455, NULL, NULL, hInstance, NULL);
    if (g_hwnd == NULL)
    {
        return 0;
    }

    g_title = CreateWindowW(L"STATIC", L"MultiRoblox 0.0.5", WS_VISIBLE | WS_CHILD, 10, 10, 365, 20, g_hwnd, NULL, hInstance, NULL);
    g_textbox = CreateWindowW(L"EDIT", L"Thanks for using MultiRoblox!\r\n\r\nMultiRoblox is ON. You can now open as many ROBLOX\r\nwindows as you want. Keep MultiRoblox open (you can\r\nminimize it).\r\n\r\nMultiRoblox must be started BEFORE ROBLOX. If ROBLOX\r\nis already open, MultiRoblox offers to close it for you.\r\nTick \"Start with Windows\" so it is always first.\r\n\r\nDiscord: https://discord.gg/g2Jgw8Z6Rb\r\nWeAreDevs: https://forum.wearedevs.net/t/29695\r\nGitHub: https://github.com/Dashbloxx/MultiRoblox\r\n", WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL | ES_MULTILINE | ES_READONLY, 10, 40, 365, 260, g_hwnd, NULL, hInstance, NULL);
    g_status = CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 10, 312, 365, 20, g_hwnd, NULL, hInstance, NULL);
    g_startup = CreateWindowW(L"BUTTON", L"Start with Windows", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_AUTOCHECKBOX, 10, 338, 365, 22, g_hwnd, (HMENU)ID_STARTUP, hInstance, NULL);
    g_closeall = CreateWindowW(L"BUTTON", L"Close all ROBLOX windows", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON, 10, 370, 200, 30, g_hwnd, (HMENU)ID_CLOSEALL, hInstance, NULL);

    set_font(g_title);
    set_font(g_textbox);
    set_font(g_status);
    set_font(g_startup);
    set_font(g_closeall);

    SendMessageW(g_startup, BM_SETCHECK, starts_with_windows() ? BST_CHECKED : BST_UNCHECKED, 0);
    update_status();
    SetTimer(g_hwnd, ID_TIMER, 2000, NULL);

    ShowWindow(g_hwnd, nCmdShow);

    MSG msg = { 0 };
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}
