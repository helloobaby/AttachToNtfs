//
//DriverObject->FastIoDispatch
//

/*
File systems and file system filters are required to support IRPs, but they are not required to support fast I/O. However, file systems and file system filters must implement fast I/O routines. Even if file systems and file system filters do not support fast I/O, they must define a fast I/O routine that returns FALSE (that is, the fast I/O routine does not implement any functionality). When the I/O Manager receives a request for synchronous file I/O (other than paging I/O), it invokes the fast I/O routine first. If the fast I/O routine returns TRUE, the operation was serviced by the fast I/O routine. If the fast I/O routine returns FALSE, the I/O Manager creates and sends an IRP instead.
*/


#include <wdm.h>

extern "C" {
	FAST_IO_CHECK_IF_POSSIBLE FastIoCheckIfPossible;
	FAST_IO_READ FastIoRead;
	FAST_IO_WRITE FastIoWrite;
	FAST_IO_QUERY_BASIC_INFO FastIoQueryBasicInfo;
	FAST_IO_QUERY_STANDARD_INFO FastIoQueryStandardInfo;
	FAST_IO_LOCK FastIoLock;
	FAST_IO_UNLOCK_SINGLE FastIoUnlockSingle;
	FAST_IO_UNLOCK_ALL FastIoUnlockAll;
	FAST_IO_UNLOCK_ALL_BY_KEY FastIoUnlockAllByKey;
	FAST_IO_DEVICE_CONTROL FastIoDeviceControl;
	FAST_IO_ACQUIRE_FILE AcquireFileForNtCreateSection;
	FAST_IO_RELEASE_FILE ReleaseFileForNtCreateSection;
	FAST_IO_DETACH_DEVICE FastIoDetachDevice;
	FAST_IO_QUERY_NETWORK_OPEN_INFO FastIoQueryNetworkOpenInfo;
	FAST_IO_ACQUIRE_FOR_MOD_WRITE AcquireForModWrite;
	FAST_IO_MDL_READ MdlRead;
	FAST_IO_MDL_READ_COMPLETE MdlReadComplete;
	FAST_IO_PREPARE_MDL_WRITE PrepareMdlWrite;
	FAST_IO_MDL_WRITE_COMPLETE MdlWriteComplete;
	FAST_IO_READ_COMPRESSED FastIoReadCompressed;
	FAST_IO_WRITE_COMPRESSED FastIoWriteCompressed;
	FAST_IO_MDL_READ_COMPLETE_COMPRESSED MdlReadCompleteCompressed;
	FAST_IO_MDL_WRITE_COMPLETE_COMPRESSED MdlWriteCompleteCompressed;
	FAST_IO_QUERY_OPEN FastIoQueryOpen;
	FAST_IO_RELEASE_FOR_MOD_WRITE ReleaseForModWrite;
	FAST_IO_ACQUIRE_FOR_CCFLUSH AcquireForCcFlush;
	FAST_IO_RELEASE_FOR_CCFLUSH ReleaseForCcFlush;
}



































