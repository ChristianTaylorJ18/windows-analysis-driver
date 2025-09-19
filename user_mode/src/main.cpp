#include <iostream>
#include "driver.h"
#include "utility.h"

int main() {
    DWORD pid = get_process_id(L"notepad.exe");
    if (!pid) {
        std::cout << "Failed to find notepad\n";
        std::cin.get();
        return 1;
    }

    HANDLE driver = CreateFile(L"\\\\.\\bypass-driver",
        GENERIC_READ | GENERIC_WRITE,
        0, nullptr, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    if (driver == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open driver handle.\n";
        std::cin.get();
        return 1;
    }

    if (driver::attach_to_process(driver, pid)) {
        std::cout << "Attachment successful.\n";
    }

    CloseHandle(driver);
    std::cin.get();
    return 0;
}
