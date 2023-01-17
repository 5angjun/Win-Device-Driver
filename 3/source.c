#include <ntddk.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegistryPath)
{
	pDrvObj = pDrvObj;
	pRegistryPath = pRegistryPath;
	return STATUS_UNSUCCESSFUL;
}