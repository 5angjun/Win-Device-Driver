#include <ntifs.h>

ULONG ProcessID = 0;
NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath)
{

	PEPROCESS pEprocess = NULL;

	pDrvObj = pDrvObj;
	pRegPath = pRegPath;



	if (ProcessID == 0)
		goto exit;

	PsLookupProcessByProcessId((HANDLE)ProcessID,&pEprocess);

	if (pEprocess)
	{
		*(unsigned char*)(((unsigned char*)pEprocess) - 0x15) = 0x14;
	}
exit:
	return STATUS_UNSUCCESSFUL;
}
