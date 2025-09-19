#include "driver.h"

void debug_print(PCSTR text) {
    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, text));
}

namespace driver {
    NTSTATUS create(PDEVICE_OBJECT device_object, PIRP irp) {
        UNREFERENCED_PARAMETER(device_object);
        IoCompleteRequest(irp, IO_NO_INCREMENT);
        return irp->IoStatus.Status;
    }

    NTSTATUS close(PDEVICE_OBJECT device_object, PIRP irp) {
        UNREFERENCED_PARAMETER(device_object);
        IoCompleteRequest(irp, IO_NO_INCREMENT);
        return irp->IoStatus.Status;
    }

    NTSTATUS device_control(PDEVICE_OBJECT device_object, PIRP irp) {
        UNREFERENCED_PARAMETER(device_object);
        debug_print("[+] Device control called.\n");

        NTSTATUS status = STATUS_UNSUCCESSFUL;
        PIO_STACK_LOCATION stack_irp = IoGetCurrentIrpStackLocation(irp);
        auto request = reinterpret_cast<Request*>(irp->AssociatedIrp.SystemBuffer);

        if (stack_irp == nullptr || request == nullptr) {
            IoCompleteRequest(irp, IO_NO_INCREMENT);
            return status;
        }

        static PEPROCESS target_process = nullptr;
        const ULONG control_code = stack_irp->Parameters.DeviceIoControl.IoControlCode;

        switch (control_code) {
        case codes::attach:
            status = PsLookupProcessByProcessId(request->process_id, &target_process);
            break;

        case codes::read:
            if (target_process != nullptr) {
                status = MmCopyVirtualMemory(
                    target_process, request->target,
                    PsGetCurrentProcess(), request->buffer,
                    request->size, KernelMode, &request->return_size
                );
            }
            break;

        case codes::write:
            if (target_process != nullptr) {
                status = MmCopyVirtualMemory(
                    PsGetCurrentProcess(), request->buffer,
                    target_process, request->target,
                    request->size, KernelMode, &request->return_size
                );
            }
            break;

        default:
            break;
        }

        irp->IoStatus.Status = status;
        irp->IoStatus.Information = sizeof(request);
        IoCompleteRequest(irp, IO_NO_INCREMENT);
        return irp->IoStatus.Status;
    }
}
