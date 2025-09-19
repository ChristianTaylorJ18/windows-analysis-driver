#pragma once
#include <Windows.h>
#include <cstdint>

DWORD get_process_id(const wchar_t* process_name);
std::uintptr_t get_module_base(DWORD pid, const wchar_t* module_name);
