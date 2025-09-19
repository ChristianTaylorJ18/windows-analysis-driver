#include "driver.h"
#include <iostream>

namespace driver {

    bool attach_to_process(HANDLE driver_handle, const DWORD pid) {
        Request r = {};
        r.process_id = reinterpret_cast<HANDLE>(pid);

        return DeviceIoControl(driver_handle, codes::attach,
            &r, sizeof(r),
            &r, sizeof(r),
            nullptr, nullptr);
    }

    template <class T>
    T read_memory(HANDLE driver_handle, const std::uintptr_t addr) {
        T temp = {};
        Request r{};
        r.target = reinterpret_cast<PVOID>(addr);
        r.buffer = &temp;
        r.size = sizeof(T);

        DeviceIoControl(driver_handle, codes::read,
            &r, sizeof(r),
            &r, sizeof(r),
            nullptr, nullptr);
        return temp;
    }

    template <class T>
    void write_memory(HANDLE driver_handle, const std::uintptr_t addr, const T& value) {
        Request r{};
        r.target = reinterpret_cast<PVOID>(addr);
        r.buffer = (PVOID)&value;
        r.size = sizeof(T);

        DeviceIoControl(driver_handle, codes::write,
            &r, sizeof(r),
            &r, sizeof(r),
            nullptr, nullptr);
    }

    // Explicit instantiations for types you actually use
    template int read_memory<int>(HANDLE driver_handle, std::uintptr_t addr);
    template void write_memory<int>(HANDLE driver_handle, std::uintptr_t addr, const int& value);
}
