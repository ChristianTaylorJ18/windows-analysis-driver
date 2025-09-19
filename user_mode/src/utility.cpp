#include "utility.h"
#include <TlHelp32.h>

DWORD get_process_id(const wchar_t* process_name) {
    DWORD process_id = 0;
    HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (snap_shot == INVALID_HANDLE_VALUE) return process_id;

    PROCESSENTRY32W entry = { sizeof(entry) };
    if (Process32FirstW(snap_shot, &entry)) {
        do {
            if (_wcsicmp(process_name, entry.szExeFile) == 0) {
                process_id = entry.th32ProcessID;
                break;
            }
        } while (Process32NextW(snap_shot, &entry));
    }

    CloseHandle(snap_shot);
    return process_id;
}

std::uintptr_t get_module_base(DWORD pid, const wchar_t* module_name) {
    std::uintptr_t base = 0;
    HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (snap_shot == INVALID_HANDLE_VALUE) return base;

    MODULEENTRY32W entry = { sizeof(entry) };
    if (Module32FirstW(snap_shot, &entry)) {
        do {
            if (_wcsicmp(module_name, entry.szModule) == 0) {
                base = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
                break;
            }
        } while (Module32NextW(snap_shot, &entry));
    }

    CloseHandle(snap_shot);
    return base;
}
