#pragma once
#include <ntifs.h>

extern "C" {
    NTKERNELAPI NTSTATUS MmCopyVirtualMemory(
        PEPROCESS SourceProcess,
        PVOID SourceAddress,
        PEPROCESS TargetProcess,
        PVOID TargetAddress,
        SIZE_T BufferSize,
        KPROCESSOR_MODE PreviousMode,
        PSIZE_T ReturnSize
    );
}

void debug_print(PCSTR text);

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

    NTSTATUS create(PDEVICE_OBJECT device_object, PIRP irp);
    NTSTATUS close(PDEVICE_OBJECT device_object, PIRP irp);
    NTSTATUS device_control(PDEVICE_OBJECT device_object, PIRP irp);
}
