#include "WindowManager.h"

LPWSTR WindowManager::get_window_type(HWND window)
{
	LPWSTR class_name = new wchar_t[256];
	GetClassNameW(window, class_name, 256);
	return class_name;
}

void WindowManager::get_all_windows()
{
	windows.clear();
	EnumWindows(enum_windows, 0);
}

void WindowManager::move_windows(int dx, int dy)
{
	auto current_moment = std::chrono::system_clock::now();
	auto millis_since_last = std::chrono::duration_cast<
		std::chrono::milliseconds>(current_moment - last_windows)
		.count();

	if (millis_since_last > 1000)
	{
		last_windows = current_moment;
		get_all_windows();
	}

	for (auto window : windows)
	{
		RECT rect;
		GetWindowRect(window, &rect);
		SetWindowPos(window, NULL, rect.left + dx, rect.top + dy, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

long long WindowManager::hook_callback(int code, WPARAM wparam, LPARAM lparam)
{
	if (code == HC_ACTION)
	{
		MOUSEHOOKSTRUCT mouse = *((MOUSEHOOKSTRUCT*)lparam);

		if (wparam == WM_LBUTTONDOWN)
		{
			is_dragging = true;
			previous_x = mouse.pt.x;
			previous_y = mouse.pt.y;
		}
		else if (wparam == WM_LBUTTONUP)
		{
			is_dragging = false;
		}
		else if (wparam == WM_MOUSEMOVE)
		{
			auto current_moment = std::chrono::system_clock::now();
			auto millis_since_last = std::chrono::duration_cast<
				std::chrono::milliseconds>(current_moment - last_drag)
				.count();

			if (millis_since_last > 20)
			{
				last_drag = current_moment;

				auto foreground_window = GetForegroundWindow();

				auto type = get_window_type(foreground_window);

				if (is_dragging && (
					foreground_window == 0
					|| wcsncmp(type, L"WorkerW", 7) == 0
					|| wcsncmp(type, L"Progman", 7) == 0))
				{
					auto dx = mouse.pt.x - previous_x;
					auto dy = mouse.pt.y - previous_y;
					move_windows(dx, dy);
					previous_x = mouse.pt.x;
					previous_y = mouse.pt.y;
				}

				delete type;
			}
		}
	}
	return CallNextHookEx(NULL, code, wparam, lparam);
}

int WindowManager::enum_windows(HWND window, LPARAM _)
{
	if (
		IsWindowVisible(window) == TRUE &&
		IsIconic(window) == FALSE &&
		GetWindowTextLengthW(window) > 0
		)
	{
		windows.push_back(window);
	}

	return 1;
}
