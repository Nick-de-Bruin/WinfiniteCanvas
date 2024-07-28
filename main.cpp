#include <Windows.h>
#include "WindowManager.h"

int main() 
{
	WindowManager manager;

	while (true)
	{
		// Since we're hooking into Windows
		// we should process messages to make sure the application doesn't hang
		MSG message;

		if (GetMessageW(&message, NULL, 0, 0))
		{
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}
		else
		{
			break;
		}
	}
}