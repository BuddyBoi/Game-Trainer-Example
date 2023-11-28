#include <iostream>
#include "memory.h"

#define OFFSET std::vector<unsigned int>

void setup()
{
    memory::_hwnd = FindWindowA(NULL, "Bitcoin Clicker");
    GetWindowThreadProcessId(memory::_hwnd, &memory::_procid);
    memory::_handle = OpenProcess(PROCESS_ALL_ACCESS, 0, memory::_procid);
    memory::module_base = memory::get_module_base(memory::_procid, L"mono.dll") + 0x001F40AC;
}

int main()
{
    setup();
    OFFSET min = { 0x1DC, 0xE4 };
    uintptr_t min_address = memory::get_pointer_address(memory::_handle, memory::module_base, min);
    
    uintptr_t max_address = min_address + 0x04;
    
    std::cout << memory::Read<int>(min_address) << std::endl;
    std::cout << memory::Read<int>(max_address) << std::endl;

    memory::Write<int>(500, min_address);
    memory::Write<int>(501, max_address);
}
