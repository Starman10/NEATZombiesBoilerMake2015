#include <windows.h>

void SendCommandToConsole(char* command)
{
	__asm
	{
		push    command
			push    0
			push    0
			mov    esi, 0x00553E70
			call    esi
			add    esp, 0x0C
	}
}

bool WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		{
			SendCommandToConsole("say I just sended a command to con");
			SendCommandToConsole("r_fullscreen 0");
		}
	}
}