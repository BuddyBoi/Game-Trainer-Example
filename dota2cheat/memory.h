#pragma once
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <cstddef>
#include <thread>
#include <chrono>

namespace memory
{
	HANDLE _handle;
	DWORD _procid;
	HWND _hwnd;
	uintptr_t module_base;

	uintptr_t get_module_base(DWORD procId, const wchar_t* modName)
	{
		uintptr_t modBaseAddr = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
		if (hSnap != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(modEntry);
			if (Module32First(hSnap, &modEntry))
			{
				do
				{
					if (!_wcsicmp(modEntry.szModule, modName))
					{
						modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
						break;
					}
				} while (Module32Next(hSnap, &modEntry));
			}
		}
		CloseHandle(hSnap);
		return modBaseAddr;
	}

	uintptr_t get_pointer_address(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
	{
		uintptr_t addr = ptr;
		for (unsigned int i = 0; i < offsets.size(); ++i)
		{
			ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
			addr += offsets[i];
		}
		return addr;
	}

	template <class T>
	T Read(uintptr_t address)
	{
		T value;
		ReadProcessMemory(_handle, (BYTE*)address, &value, sizeof(value), nullptr);
		return value;
	}

	template <class T>
	void Write(T setTo, uintptr_t address)
	{
		T newvalue = setTo;
		WriteProcessMemory(_handle, (BYTE*)address, &newvalue, sizeof(newvalue), nullptr);
	}
}