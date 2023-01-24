#include <ntddk.h>

void SampleDriverUnload(PDRIVER_OBJECT pDrvObj)
{
	pDrvObj = pDrvObj;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath)
{
	pRegPath = pRegPath;
	pDrvObj->DriverUnload = SampleDriverUnload;
	return STATUS_SUCCESS;
}