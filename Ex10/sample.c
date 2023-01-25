#include <ntddk.h>
NTSTATUS MyCreateDispatch(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	pDevObj = pDevObj;

	pIrp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(pIrp, 0);

	return STATUS_SUCCESS;
}

NTSTATUS MyCloseDispatch(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	pDevObj = pDevObj;

	pIrp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(pIrp, 0);

	return STATUS_SUCCESS;
}

void SampleDriverUnload(PDRIVER_OBJECT pDrvObj)
{
	pDrvObj = pDrvObj;
	UNICODE_STRING SymbolicLinkName;
	RtlInitUnicodeString(&SymbolicLinkName, L"\\DosDevices\\MYEX9");

	IoDeleteSymbolicLink(&SymbolicLinkName);

	// Device Object삭제
	IoDeleteDevice(pDrvObj->DeviceObject);
	
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath)
{
	PDEVICE_OBJECT DeviceObject = NULL;
	pRegPath = pRegPath;


	pDrvObj->MajorFunction[IRP_MJ_CREATE] = MyCreateDispatch;
	pDrvObj->MajorFunction[IRP_MJ_CLOSE] = MyCloseDispatch;


	pDrvObj->DriverUnload = SampleDriverUnload;

	UNICODE_STRING DeviceName;
	UNICODE_STRING SymbolicLinkName;
	RtlInitUnicodeString(&DeviceName, L"\\Device\\EX9");
	RtlInitUnicodeString(&SymbolicLinkName, L"\\DosDevices\\MYEX9");
	NTSTATUS ntStatus;
	// Device Object 생성
	ntStatus = IoCreateDevice(
		pDrvObj,
		0,
		&DeviceName,
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&DeviceObject
	);
	ntStatus = IoCreateSymbolicLink(
		&SymbolicLinkName,
		&DeviceName
	);

	return STATUS_SUCCESS;
}