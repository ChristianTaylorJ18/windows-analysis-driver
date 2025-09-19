#pragma once
#include <Windows.h>
#include <cstdint>

namespace driver {
    namespace codes {
        constexpr ULONG attach = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x696, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
        constexpr ULONG read = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x697, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
        constexpr ULONG write = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x698, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
    }

    struct Request {
        HANDLE process_id;
        PVOID target;
        PVOID buffer;
        SIZE_T size;
        SIZE_T return_size;
    };

    bool attach_to_process(HANDLE driver_handle, DWORD pid);

    template <class T>
    T read_memory(HANDLE driver_handle, std::uintptr_t addr);

    template <class T>
    void write_memory(HANDLE driver_handle, std::uintptr_t addr, const T& value);
}
