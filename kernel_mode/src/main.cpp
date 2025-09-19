#include <ntifs.h>
#include "driver.h"

extern "C" {
    NTKERNELAPI NTSTATUS IoCreateDriver(PUNICODE_STRING DriverName, PDRIVER_INITIALIZE InitializationFunction);
}

NTSTATUS driver_main(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path) {
    UNREFERENCED_PARAMETER(registry_path);

    UNICODE_STRING device_name = {};
    RtlInitUnicodeString(&device_name, L"\\Device\\bypass-driver");

    PDEVICE_OBJECT device_object = nullptr;
    NTSTATUS status = IoCreateDevice(
        driver_object, 0, &device_name, FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN, FALSE, &device_object
    );
    if (!NT_SUCCESS(status)) {
        debug_print("[-] Failed to create driver device.\n");
        return status;
    }

    debug_print("[+] Driver Device successfully created.\n");

    UNICODE_STRING symbolic_link = {};
    RtlInitUnicodeString(&symbolic_link, L"\\DosDevices\\bypass-driver");
    status = IoCreateSymbolicLink(&symbolic_link, &device_name);
    if (!NT_SUCCESS(status)) {
        debug_print("[-] Failed to establish symbolic link.\n");
        return status;
    }

    debug_print("[+] Driver symbolic link successfully established.\n");

    SetFlag(device_object->Flags, DO_BUFFERED_IO);

    driver_object->MajorFunction[IRP_MJ_CREATE] = driver::create;
    driver_object->MajorFunction[IRP_MJ_CLOSE] = driver::close;
    driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = driver::device_control;

    ClearFlag(device_object->Flags, DO_DEVICE_INITIALIZING);
    debug_print("[+] Driver initialized successfully.\n");
    return status;
}

NTSTATUS DriverEntry() {
    debug_print("[+] Hello from DriverEntry!\n");
    UNICODE_STRING driver_name = {};
    RtlInitUnicodeString(&driver_name, L"\\Driver\\bypass-driver");
    return IoCreateDriver(&driver_name, &driver_main);
}
