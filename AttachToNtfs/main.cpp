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















*/

void DriverUnload(PDRIVER_OBJECT DriverObject);

UNICODE_STRING AnonymousNtfs = RTL_CONSTANT_STRING(L"\\FileSystem\\AnonymousNtfs");
UNICODE_STRING Ntfs = RTL_CONSTANT_STRING(L"\\??\\C:\\Windows\\System32\\drivers\\ntfs.sys");
UNICODE_STRING NtfsDeviceName = RTL_CONSTANT_STRING(L"\\FileSystem\\Ntfs");

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

NTSTATUS ATPassThrouth(PDEVICE_OBJECT DeviceObject, PIRP Irp);

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegPath)
{
	NTSTATUS Status;

	DriverObject->DriverUnload = DriverUnload;

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
	IoSkipCurrentIrpStackLocation(Irp);
	return IofCallDriver(DeviceObject->DeviceObjectExtension->AttachedTo, Irp);
}













