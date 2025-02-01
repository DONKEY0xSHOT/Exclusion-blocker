#include <ntddk.h>
#include <wdf.h>
#include "registry_filter.h"

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_UNLOAD DriverUnload;


NTSTATUS RegFilterCallback(PVOID CallbackContext, PVOID Argument1, PVOID Argument2)
{
    UNREFERENCED_PARAMETER(CallbackContext);
    UNREFERENCED_PARAMETER(Argument2);

    NTSTATUS status = STATUS_SUCCESS;
    PREG_SET_VALUE_KEY_INFORMATION setValueInfo;
    PREG_CREATE_KEY_INFORMATION createKeyInfo;
    PREG_DELETE_VALUE_KEY_INFORMATION deleteValueInfo;
    PCUNICODE_STRING regPath;

    REG_NOTIFY_CLASS operation = (REG_NOTIFY_CLASS)(ULONG_PTR)Argument1;

    // Block any modification of the Windows Defender exclusions registry
    switch (operation)
    {


    case RegNtPreCreateKey:
        createKeyInfo = (PREG_CREATE_KEY_INFORMATION)Argument2;
        if (createKeyInfo && IsPathProtected(createKeyInfo->CompleteName))
        {
            return STATUS_ACCESS_DENIED;
        }
        break;

    case RegNtPreSetValueKey:
        setValueInfo = (PREG_SET_VALUE_KEY_INFORMATION)Argument2;
        if (setValueInfo)
        {
            status = CmCallbackGetKeyObjectID(&Cookie, setValueInfo->Object, NULL, &regPath);
            if (NT_SUCCESS(status))
            {
                if (IsPathProtected(regPath))
                {
                    return STATUS_ACCESS_DENIED;
                }
            }
        }
        break;

    case RegNtPreDeleteValueKey:
        deleteValueInfo = (PREG_DELETE_VALUE_KEY_INFORMATION)Argument2;
        if (deleteValueInfo)
        {
            status = CmCallbackGetKeyObjectID(&Cookie, deleteValueInfo->Object, NULL, &regPath);
            if (NT_SUCCESS(status))
            {
                if (IsPathProtected(regPath))
                {
                    return STATUS_ACCESS_DENIED;
                }
            }
        }
        break;

    default:
        break;
    }

    return status;
}

VOID DriverUnload(IN WDFDRIVER Driver)
{
    UNREFERENCED_PARAMETER(Driver);

    if (CallbackRegistered)
    {
        CmUnRegisterCallback(Cookie);
        CallbackRegistered = FALSE;
    }
}

_Use_decl_annotations_
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    NTSTATUS status;
    WDF_DRIVER_CONFIG config;

    // Initialize the config structure and set the unload routine
    WDF_DRIVER_CONFIG_INIT(&config, NULL);
    config.EvtDriverUnload = DriverUnload;

    // Create the driver object
    status = WdfDriverCreate(DriverObject,
        RegistryPath,
        WDF_NO_OBJECT_ATTRIBUTES,
        &config,
        WDF_NO_HANDLE);
    if (!NT_SUCCESS(status))
    {
        return status;
    }

    // Register the callback routine
    status = CmRegisterCallback(RegFilterCallback,
        NULL,
        &Cookie);
    if (NT_SUCCESS(status))
    {
        CallbackRegistered = TRUE;
    }
    else
    {
        return status;
    }

    return STATUS_SUCCESS;
}