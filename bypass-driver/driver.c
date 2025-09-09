#include <ntddk.h>

DRIVER_UNLOAD DriverUnload;

_Use_decl_annotations_
VOID DriverUnload(PDRIVER_OBJECT DriverObject) {
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "bypass-driver: Unload\n");
}

_Use_decl_annotations_
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    DriverObject->DriverUnload = DriverUnload;
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "bypass-driver: Hello from kernel!\n");
    return STATUS_SUCCESS;
}
