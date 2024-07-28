#pragma once
#include <ctime>
#include <chrono>
#include <Windows.h>
#include <vector>
#include <thread>

class WindowManager
{
public:
	WindowManager() 
	{
		hook_id = SetWindowsHookExA(WH_MOUSE_LL, hook_callback, NULL, 0); 
	}
	~WindowManager() { UnhookWindowsHookEx(hook_id); }
	static LPWSTR get_window_type(HWND window);
	static void get_all_windows();
	static void move_windows(int dx, int dy);
private:
	inline static bool is_dragging;
	HHOOK hook_id;
	inline static int previous_x;
	inline static int previous_y;
	inline static std::chrono::system_clock::time_point last_drag;
	inline static std::chrono::system_clock::time_point last_windows;
	inline static std::vector<HWND> windows;
	static long long hook_callback(int code, WPARAM wparam, LPARAM lparam);
	static int enum_windows(HWND window, LPARAM _);
};

