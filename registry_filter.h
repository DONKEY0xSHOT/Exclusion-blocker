#pragma once
#include <ntddk.h>
#include <wdf.h>


// Function declarations
NTSTATUS RegFilterCallback(PVOID CallbackContext, PVOID Argument1, PVOID Argument2);

// Global variables
LARGE_INTEGER Cookie;
BOOLEAN CallbackRegistered = FALSE;

// Array of registry paths we want to protect
const WCHAR* ProtectedPaths[] = {
    L"\\REGISTRY\\MACHINE\\SOFTWARE\\Microsoft\\Windows Defender\\Exclusions\\Paths",
    L"\\REGISTRY\\MACHINE\\SOFTWARE\\Microsoft\\Windows Defender\\Exclusions\\Processes",
    L"\\REGISTRY\\MACHINE\\SOFTWARE\\Microsoft\\Windows Defender\\Exclusions\\Extensions"
};

#define PROTECTED_PATHS_COUNT (sizeof(ProtectedPaths) / sizeof(ProtectedPaths[0]))

// Helper function to check if path is protected
BOOLEAN IsPathProtected(PCUNICODE_STRING PathToCheck)
{
    UNICODE_STRING protectedPathUs;

    for (ULONG i = 0; i < PROTECTED_PATHS_COUNT; i++)
    {
        RtlInitUnicodeString(&protectedPathUs, ProtectedPaths[i]);
        if (RtlPrefixUnicodeString(&protectedPathUs, PathToCheck, TRUE))
        {
            return TRUE;
        }
    }
    return FALSE;
}