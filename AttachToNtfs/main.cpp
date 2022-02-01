#include <ntifs.h>
#include <ntdef.h>
#include <ntddk.h>

#include "fastio.h"




/*
AttachToNtfs!_DEVICE_OBJECT .LowerDevices
(*((AttachToNtfs!_DEVICE_OBJECT *)0xffffa38f0b8e50a0)).LowerDevices : Immediately below is Device for "\FileSystem\FltMgr" [at 0xffffa38f093f7760]
	[0x0]            : 0xffffa38f093f7760 : Device for "\FileSystem\FltMgr" [Type: _DEVICE_OBJECT *]
	[0x1]            : 0xffffa38f09603030 : Device for "\FileSystem\Ntfs" [Type: _DEVICE_OBJECT *]

挂载后的设备栈 AttachToNtfs.sys -> FltMgr.sys(minifilter) -> Ntfs  (最左边的最先收到IRP)


	UpperDevices     : Immediately above is Device for "\Driver\AttachToNtfs" [at 0xffffa38f0c33a9f0]
	LowerDevices     : Immediately below is Device for "\FileSystem\Ntfs" [at 0xffffa38f09603030]
	Driver           : 0xffffa38f0957bd20 : Driver "\FileSystem\FltMgr" [Type: _DRIVER_OBJECT *]












*/



UNICODE_STRING AnonymousNtfs = RTL_CONSTANT_STRING(L"\\FileSystem\\AnonymousNtfs");
UNICODE_STRING Ntfs = RTL_CONSTANT_STRING(L"\\??\\C:\\Windows\\System32\\drivers\\ntfs.sys");
UNICODE_STRING NtfsDeviceName = RTL_CONSTANT_STRING(L"\\FileSystem\\Ntfs");


UNICODE_STRING Npfs = RTL_CONSTANT_STRING(L"\\??\\C:\\Windows\\System32\\drivers\\npfs.sys");
UNICODE_STRING NpfsDeviceName = RTL_CONSTANT_STRING(L"\\FileSystem\\Npfs");

FILE_OBJECT* NtfsFile;
DEVICE_OBJECT* NtfsDevice;
DEVICE_OBJECT* NewDevice;
DEVICE_OBJECT* TargetDevice;

LARGE_INTEGER MmOneSecond = { (ULONG)(-1 * 1000 * 1000 * 10), -1 };

FAST_IO_DISPATCH* Ptr_FAST_IO_DISPATCH;

#define COMMON_FAILED(Status,FunName)  \
if (!NT_SUCCESS(Status))    \
{							\
	DbgPrintEx(DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, FunName ## " failed with %x\n", Status);\
	return Status;\
}

void DriverUnload(PDRIVER_OBJECT DriverObject);
NTSTATUS ATPassThrouth(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS ATHandler_IRP_CREATE(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
bool FindSubUnicodeString(const WCHAR * sub, PCUNICODE_STRING src);



bool FindSubUnicodeString(const WCHAR * sub, PCUNICODE_STRING src)
{
	if (!sub || !src || !src->Buffer)
		return false;

	WCHAR* tmp = (WCHAR*)ExAllocatePoolWithTag(NonPagedPool, src->Length + sizeof(L'\0'), 'bbs');
	WCHAR* sub_t = (WCHAR*)ExAllocatePoolWithTag(NonPagedPool, wcslen(sub) * 2 + sizeof(L'\0'), 'bbs');

	if (!tmp || !sub_t)
		return false;

	memcpy(tmp, src->Buffer, src->Length);
	tmp[src->Length / 2] = L'\0';


	memcpy(sub_t, sub, wcslen(sub) * 2);

	sub_t[wcslen(sub)] = L'\0';

	//RtlDowncaseUnicodeArray(tmp);
	//RtlDowncaseUnicodeArray(sub_t);

	bool ret = (wcsstr(tmp, sub_t)) ? true : false;

	ExFreePoolWithTag(tmp, 'bbs');
	ExFreePoolWithTag(sub_t, 'bbs');


	return ret;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegPath)
{
	NTSTATUS Status;

	DriverObject->DriverUnload = DriverUnload;

#if 1 //挂载ntfs
	Status = IoGetDeviceObjectPointer(&Ntfs, FILE_ALL_ACCESS, &NtfsFile, &NtfsDevice);

	COMMON_FAILED(Status, "IoGetDeviceObjectPointer");

	Status = IoCreateDevice(DriverObject, 0, &AnonymousNtfs, NtfsDevice->DeviceType, 0, false, &NewDevice);
	COMMON_FAILED(Status, "IoCreateDevice");

	Status = IoAttachDevice(NewDevice, &Ntfs, &TargetDevice);
	if (!NT_SUCCESS(Status))
	{
		IoDeleteDevice(NewDevice);
		DbgPrintEx(DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, "IoAttachDevice failed with %x\n", Status);
		return Status;
	}
#endif

#if 0 //挂载npfs
	Status = IoGetDeviceObjectPointer(&Npfs, FILE_ALL_ACCESS, &NtfsFile, &NtfsDevice);

	COMMON_FAILED(Status, "IoGetDeviceObjectPointer");

	Status = IoCreateDevice(DriverObject, 0, &AnonymousNtfs, NtfsDevice->DeviceType, 0, false, &NewDevice);
	COMMON_FAILED(Status, "IoCreateDevice");

	Status = IoAttachDevice(NewDevice, &Npfs, &TargetDevice);
	if (!NT_SUCCESS(Status))
	{
		IoDeleteDevice(NewDevice);
		DbgPrintEx(DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, "IoAttachDevice failed with %x\n", Status);
		return Status;
	}
#endif
	
	Ptr_FAST_IO_DISPATCH = (PFAST_IO_DISPATCH)ExAllocatePoolWithTag(NonPagedPoolNx, sizeof(FAST_IO_DISPATCH), 'ifMF');
	
	if (!Ptr_FAST_IO_DISPATCH) {
		IoDeleteDevice(NewDevice);
		IoDetachDevice(TargetDevice);
		return STATUS_NO_MEMORY;
	}
	memset(Ptr_FAST_IO_DISPATCH, 0, sizeof(FAST_IO_DISPATCH));

	DriverObject->FastIoDispatch = Ptr_FAST_IO_DISPATCH;
	
	// 初始化fast io

	Ptr_FAST_IO_DISPATCH->SizeOfFastIoDispatch = sizeof(FAST_IO_DISPATCH);
	Ptr_FAST_IO_DISPATCH->AcquireFileForNtCreateSection = AcquireFileForNtCreateSection;
	Ptr_FAST_IO_DISPATCH->AcquireForCcFlush = AcquireForCcFlush;
	Ptr_FAST_IO_DISPATCH->AcquireForModWrite = AcquireForModWrite;
	Ptr_FAST_IO_DISPATCH->FastIoCheckIfPossible = FastIoCheckIfPossible;
	Ptr_FAST_IO_DISPATCH->FastIoDetachDevice = FastIoDetachDevice;
	Ptr_FAST_IO_DISPATCH->FastIoDeviceControl = FastIoDeviceControl;
	Ptr_FAST_IO_DISPATCH->FastIoLock = FastIoLock;
	Ptr_FAST_IO_DISPATCH->FastIoQueryBasicInfo = FastIoQueryBasicInfo;
	Ptr_FAST_IO_DISPATCH->FastIoQueryNetworkOpenInfo = FastIoQueryNetworkOpenInfo;
	Ptr_FAST_IO_DISPATCH->FastIoQueryOpen = FastIoQueryOpen;
	Ptr_FAST_IO_DISPATCH->FastIoQueryStandardInfo = FastIoQueryStandardInfo;
	Ptr_FAST_IO_DISPATCH->FastIoRead = FastIoRead;
	Ptr_FAST_IO_DISPATCH->FastIoReadCompressed = FastIoReadCompressed;
	Ptr_FAST_IO_DISPATCH->FastIoUnlockAll = FastIoUnlockAll;
	Ptr_FAST_IO_DISPATCH->FastIoUnlockAllByKey = FastIoUnlockAllByKey;
	Ptr_FAST_IO_DISPATCH->FastIoUnlockSingle = FastIoUnlockSingle;
	Ptr_FAST_IO_DISPATCH->FastIoWrite = FastIoWrite;
	Ptr_FAST_IO_DISPATCH->FastIoWriteCompressed = FastIoWriteCompressed;
	Ptr_FAST_IO_DISPATCH->MdlRead = MdlRead;
	Ptr_FAST_IO_DISPATCH->MdlReadComplete = MdlReadComplete;
	Ptr_FAST_IO_DISPATCH->MdlReadCompleteCompressed = MdlReadCompleteCompressed;
	Ptr_FAST_IO_DISPATCH->MdlWriteComplete = MdlWriteComplete;
	Ptr_FAST_IO_DISPATCH->MdlWriteCompleteCompressed = MdlWriteCompleteCompressed;
	Ptr_FAST_IO_DISPATCH->PrepareMdlWrite = PrepareMdlWrite;
	Ptr_FAST_IO_DISPATCH->ReleaseFileForNtCreateSection = ReleaseFileForNtCreateSection;
	Ptr_FAST_IO_DISPATCH->ReleaseForCcFlush = ReleaseForCcFlush;
	Ptr_FAST_IO_DISPATCH->ReleaseForModWrite = ReleaseForModWrite;


	//初始化irp dispatch

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		DriverObject->MajorFunction[i] = ATPassThrouth;

	
	// 过滤irp mj create

	DriverObject->MajorFunction[IRP_MJ_CREATE] = ATHandler_IRP_CREATE;
























	return STATUS_SUCCESS;
}


void DriverUnload(PDRIVER_OBJECT DriverObject)
{

	IoDeleteDevice(NewDevice);
	IoDetachDevice(TargetDevice);

	//
	//给三秒时间结束请求再卸载驱动
	//
	for(int i = 0;i<3;i++)
		KeDelayExecutionThread(KernelMode, false, &MmOneSecond);

	return;
}

//
//简单的把Irp转发给FltMgr.sys
//

/*  
	 IRP Stack Location
	 _________________					       地址  低
	|			      |
	|                 |   ->      Ntfs
	|                 |
	|_________________|	
	|				  |	
	|                 |   ->      FltMgr
	|				  |
	|_________________|
	|				  |
	|                 |   ->      AttachToNtfs
	|                 |
	|_________________|					       地址  高
*/
NTSTATUS ATPassThrouth(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{

#if 0		//将irp转发给minifilter
	IoSkipCurrentIrpStackLocation(Irp);
	return IofCallDriver(DeviceObject->DeviceObjectExtension->AttachedTo, Irp);
#endif


#if 1       //将irp直接转发给ntfs

	IoSkipCurrentIrpStackLocation(Irp);
	if (DeviceObject->DeviceObjectExtension->AttachedTo->DeviceObjectExtension->AttachedTo) {
		return IofCallDriver(DeviceObject->DeviceObjectExtension->AttachedTo->DeviceObjectExtension->AttachedTo, Irp);
	}
	else {
		return IofCallDriver(DeviceObject->DeviceObjectExtension->AttachedTo, Irp);
	}
#endif



}

NTSTATUS ATHandler_IRP_CREATE(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{

	ASSERT(Irp->Tail.Overlay.CurrentStackLocation->MajorFunction == IRP_MJ_CREATE);

	FILE_OBJECT* FileObject = Irp->Tail.Overlay.CurrentStackLocation->FileObject;

	//DbgPrintEx(DPFLTR_DEFAULT_ID, DPFLTR_ERROR_LEVEL, "[IRP Create] %wZ\n", FileObject->FileName);

	//
	//过滤irp
	//
	if (FindSubUnicodeString(L"ok.txt", &FileObject->FileName))
	{
		IofCompleteRequest(Irp, IO_NO_INCREMENT);
		Irp->IoStatus.Status = STATUS_ACCESS_DENIED;
		return STATUS_SUCCESS;
	}


	IoSkipCurrentIrpStackLocation(Irp);
	return IofCallDriver(DeviceObject->DeviceObjectExtension->AttachedTo, Irp);
}











