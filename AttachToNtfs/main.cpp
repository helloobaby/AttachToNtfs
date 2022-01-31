#include <ntifs.h>
#include <ntdef.h>
#include <ntddk.h>
#include <stdio.h>

/*
AttachToNtfs!_DEVICE_OBJECT .LowerDevices
(*((AttachToNtfs!_DEVICE_OBJECT *)0xffffa38f0b8e50a0)).LowerDevices : Immediately below is Device for "\FileSystem\FltMgr" [at 0xffffa38f093f7760]
	[0x0]            : 0xffffa38f093f7760 : Device for "\FileSystem\FltMgr" [Type: _DEVICE_OBJECT *]
	[0x1]            : 0xffffa38f09603030 : Device for "\FileSystem\Ntfs" [Type: _DEVICE_OBJECT *]

挂载后的设备栈 AttachToNtfs.sys -> FltMgr.sys(minifilter) -> Ntfs  (最左边的最先收到IRP)















*/

void DriverUnload(PDRIVER_OBJECT DriverObject);

UNICODE_STRING AnonymousNtfs = RTL_CONSTANT_STRING(L"\\Device\\AnonymousNtfs");
UNICODE_STRING Ntfs = RTL_CONSTANT_STRING(L"\\??\\C:\\Windows\\System32\\drivers\\ntfs.sys");
UNICODE_STRING NtfsDeviceName = RTL_CONSTANT_STRING(L"\\FileSystem\\Ntfs");

FILE_OBJECT* NtfsFile;
DEVICE_OBJECT* NtfsDevice;
DEVICE_OBJECT* NewDevice;
DEVICE_OBJECT* TargetDevice;

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


	// 初始化fast io



	//初始化irp dispatch

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		DriverObject->MajorFunction[i] = ATPassThrouth;

	


























	return STATUS_SUCCESS;
}


void DriverUnload(PDRIVER_OBJECT DriverObject)
{

	if (NewDevice)
		IoDeleteDevice(NewDevice);
	

	return;
}

//
//简单的把Irp转发给FltMgr.sys
//
NTSTATUS ATPassThrouth(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	IoSkipCurrentIrpStackLocation(Irp);
	return IofCallDriver(DeviceObject->DeviceObjectExtension->AttachedTo, Irp);
}















