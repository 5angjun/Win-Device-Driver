#include <ntddk.h>

WCHAR g_TempString[512] = { 0, };
void NotifyRountine(PEPROCESS Process, HANDLE ProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo)
{
	Process = Process;
	ProcessId = ProcessId;

	if (CreateInfo == NULL)
		goto exit;


	memset(g_TempString, 0, sizeof(WCHAR)*512);
	memcpy(g_TempString, CreateInfo->ImageFileName->Buffer, CreateInfo->ImageFileName->Length);

	_wcsupr(g_TempString);
	
	if (wcswcs(g_TempString, L"NOTEPAD.EXE"))
	{
		CreateInfo->CreationStatus = STATUS_UNSUCCESSFUL;
	}


exit:
	return;
}

void SampleDriverUnload(PDRIVER_OBJECT pDrvObj)
{
	pDrvObj = pDrvObj;

	PsSetCreateProcessNotifyRoutineEx(NotifyRountine, TRUE);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath)
{
	pRegPath = pRegPath;

	pDrvObj->DriverUnload = SampleDriverUnload;
	PsSetCreateProcessNotifyRoutineEx(NotifyRountine, FALSE);
	return STATUS_SUCCESS;
}