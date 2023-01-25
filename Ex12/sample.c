#include <ntddk.h>
typedef struct
{
	unsigned char Buffer[4];
	int DataSize;
}DEVICE_EXTENSION;

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


NTSTATUS MyWriteDispatch(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PIO_STACK_LOCATION pStack;	
	int Length;
	unsigned char* pUserBuffer;
	DEVICE_EXTENSION* pDE;

	pStack = IoGetCurrentIrpStackLocation(pIrp);

	Length = pStack->Parameters.Write.Length;
	if (Length > 4)
	{
		Length = 4;
	}
	pUserBuffer = pIrp->UserBuffer;
	pDE = (DEVICE_EXTENSION*)pDevObj->DeviceExtension;
	memcpy(pDE->Buffer, pUserBuffer, Length);
	pDE->DataSize = Length;

	pIrp->IoStatus.Information = Length;
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(pIrp, 0);
	return STATUS_SUCCESS;
}


NTSTATUS MyReadDispatch(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	PIO_STACK_LOCATION pStack;
	int Length;
	unsigned char* pUserBuffer;
	DEVICE_EXTENSION* pDE;

	pStack = IoGetCurrentIrpStackLocation(pIrp);

	Length = pStack->Parameters.Read.Length;
	pDE = (DEVICE_EXTENSION*)pDevObj->DeviceExtension;
	if (Length > pDE->DataSize)
	{
		Length = pDE->DataSize;
	}
	pUserBuffer = pIrp->UserBuffer;
	
	memcpy(pUserBuffer,pDE->Buffer, Length);
	pDE->DataSize = Length;

	pIrp->IoStatus.Information = Length;
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	IoCompleteRequest(pIrp, 0);
	return STATUS_SUCCESS;
}




void SampleDriverUnload(PDRIVER_OBJECT pDrvObj)
{
	pDrvObj = pDrvObj;
	UNICODE_STRING SymbolicLinkName;
	RtlInitUnicodeString(&SymbolicLinkName, L"\\DosDevices\\MYSAMPLE");

	IoDeleteSymbolicLink(&SymbolicLinkName);

	// Device Object삭제
	IoDeleteDevice(pDrvObj->DeviceObject);
	
}



NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath)
{
	PDEVICE_OBJECT DeviceObject = NULL;
	pRegPath = pRegPath;
	DEVICE_EXTENSION* pDE;

	pDrvObj->MajorFunction[IRP_MJ_CREATE] = MyCreateDispatch;
	pDrvObj->MajorFunction[IRP_MJ_CLOSE] = MyCloseDispatch;
	pDrvObj->MajorFunction[IRP_MJ_READ] = MyReadDispatch;
	pDrvObj->MajorFunction[IRP_MJ_WRITE] = MyWriteDispatch;



	pDrvObj->DriverUnload = SampleDriverUnload;

	UNICODE_STRING DeviceName;
	UNICODE_STRING SymbolicLinkName;
	RtlInitUnicodeString(&DeviceName, L"\\Device\\SAMPLE");
	RtlInitUnicodeString(&SymbolicLinkName, L"\\DosDevices\\MYSAMPLE");
	NTSTATUS ntStatus;
	// Device Object 생성
	ntStatus = IoCreateDevice(
		pDrvObj,
		sizeof(DEVICE_EXTENSION),
		&DeviceName,
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&DeviceObject
	);

	pDE = (DEVICE_EXTENSION*)DeviceObject->DeviceExtension;
	pDE->DataSize = 0;
	ntStatus = IoCreateSymbolicLink(
		&SymbolicLinkName,
		&DeviceName
	);

	return STATUS_SUCCESS;
}