#include "fastio.h"

#pragma warning (disable : 4100) //未引用的形参
#pragma warning (disable : 6101) //返回参数的问题


BOOLEAN
FastIoCheckIfPossible(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PLARGE_INTEGER FileOffset,
	_In_ ULONG Length,
	_In_ BOOLEAN Wait,
	_In_ ULONG LockKey,
	_In_ BOOLEAN CheckForReadOperation,
	_Pre_notnull_
	_When_(return != FALSE, _Post_equal_to_(_Old_(IoStatus)))
	_When_(return == FALSE, _Post_valid_)
	PIO_STATUS_BLOCK IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}

BOOLEAN
FastIoRead(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PLARGE_INTEGER FileOffset,
	_In_ ULONG Length,
	_In_ BOOLEAN Wait,
	_In_ ULONG LockKey,
	_Out_ PVOID Buffer,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}

BOOLEAN
FastIoWrite(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PLARGE_INTEGER FileOffset,
	_In_ ULONG Length,
	_In_ BOOLEAN Wait,
	_In_ ULONG LockKey,
	_In_ PVOID Buffer,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{

	return false;
}

BOOLEAN
FastIoQueryBasicInfo(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ BOOLEAN Wait,
	_Out_ PFILE_BASIC_INFORMATION Buffer,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{

	return false;
}

BOOLEAN
FastIoQueryStandardInfo(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ BOOLEAN Wait,
	_Out_ PFILE_STANDARD_INFORMATION Buffer,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}

BOOLEAN
FastIoLock(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PLARGE_INTEGER FileOffset,
	_In_ PLARGE_INTEGER Length,
	_In_ PEPROCESS ProcessId,
	_In_ ULONG Key,
	_In_ BOOLEAN FailImmediately,
	_In_ BOOLEAN ExclusiveLock,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}

BOOLEAN
FastIoUnlockSingle(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PLARGE_INTEGER FileOffset,
	_In_ PLARGE_INTEGER Length,
	_In_ PEPROCESS ProcessId,
	_In_ ULONG Key,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{

	return false;
}

BOOLEAN
FastIoUnlockAll(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PEPROCESS ProcessId,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{

	return false;
}

BOOLEAN
FastIoUnlockAllByKey(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PVOID ProcessId,
	_In_ ULONG Key,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{

	return false;
}

BOOLEAN
FastIoDeviceControl(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ BOOLEAN Wait,
	_In_opt_ PVOID InputBuffer,
	_In_ ULONG InputBufferLength,
	_Out_opt_ PVOID OutputBuffer,
	_In_ ULONG OutputBufferLength,
	_In_ ULONG IoControlCode,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{

	return false;
}

VOID AcquireFileForNtCreateSection(
	_In_ struct _FILE_OBJECT* FileObject
)
{
	return;
}


VOID
ReleaseFileForNtCreateSection(
	_In_ struct _FILE_OBJECT* FileObject
)
{
	return;
}


VOID
FastIoDetachDevice(
	_In_ struct _DEVICE_OBJECT* SourceDevice,
	_In_ struct _DEVICE_OBJECT* TargetDevice
)
{
	return;
}


BOOLEAN
FastIoQueryNetworkOpenInfo(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ BOOLEAN Wait,
	_Out_ struct _FILE_NETWORK_OPEN_INFORMATION* Buffer,
	_Out_ struct _IO_STATUS_BLOCK* IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}

NTSTATUS
AcquireForModWrite(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PLARGE_INTEGER EndingOffset,
	_Out_ struct _ERESOURCE** ResourceToRelease,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{


	return STATUS_UNSUCCESSFUL;
}

BOOLEAN
MdlRead(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PLARGE_INTEGER FileOffset,
	_In_ ULONG Length,
	_In_ ULONG LockKey,
	_Out_ PMDL * MdlChain,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}


BOOLEAN
MdlReadComplete(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PMDL MdlChain,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}


BOOLEAN
PrepareMdlWrite(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PLARGE_INTEGER FileOffset,
	_In_ ULONG Length,
	_In_ ULONG LockKey,
	_Out_ PMDL * MdlChain,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}


BOOLEAN
MdlWriteComplete(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PLARGE_INTEGER FileOffset,
	_In_ PMDL MdlChain,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{

	return false;
}

BOOLEAN
FastIoReadCompressed(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PLARGE_INTEGER FileOffset,
	_In_ ULONG Length,
	_In_ ULONG LockKey,
	_Out_ PVOID Buffer,
	_Out_ PMDL * MdlChain,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_Out_writes_bytes_(CompressedDataInfoLength) struct _COMPRESSED_DATA_INFO* CompressedDataInfo,
	_In_ ULONG CompressedDataInfoLength,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}

BOOLEAN
FastIoWriteCompressed(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PLARGE_INTEGER FileOffset,
	_In_ ULONG Length,
	_In_ ULONG LockKey,
	_In_ PVOID Buffer,
	_Out_ PMDL * MdlChain,
	_Out_ PIO_STATUS_BLOCK IoStatus,
	_In_reads_bytes_(CompressedDataInfoLength) struct _COMPRESSED_DATA_INFO* CompressedDataInfo,
	_In_ ULONG CompressedDataInfoLength,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}

BOOLEAN
MdlReadCompleteCompressed(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PMDL MdlChain,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}


BOOLEAN
MdlWriteCompleteCompressed(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ PLARGE_INTEGER FileOffset,
	_In_ PMDL MdlChain,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}


BOOLEAN
FastIoQueryOpen(
	_Inout_ struct _IRP* Irp,
	_Out_ PFILE_NETWORK_OPEN_INFORMATION NetworkInformation,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return false;
}

NTSTATUS
ReleaseForModWrite(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ struct _ERESOURCE* ResourceToRelease,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return STATUS_UNSUCCESSFUL;
}

NTSTATUS
AcquireForCcFlush(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ struct _DEVICE_OBJECT* DeviceObject
)
{
	return STATUS_UNSUCCESSFUL;
}

NTSTATUS
ReleaseForCcFlush(
	_In_ struct _FILE_OBJECT* FileObject,
	_In_ struct _DEVICE_OBJECT* DeviceObject

)
{

	return STATUS_UNSUCCESSFUL;
}

