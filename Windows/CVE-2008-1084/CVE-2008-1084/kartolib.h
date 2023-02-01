

//////////////////////////////
////
////	Driver defines
////
//////////////////////////////

#define KARTOFFEL_TYPE 50000

#define IOCTL_KARTOFFEL_ALLOCATE_BUFFER \
		CTL_CODE( KARTOFFEL_TYPE, 0x807, METHOD_BUFFERED, FILE_ANY_ACCESS  )

#define IOCTL_KARTOFFEL_RETURN_BUFFER \
		CTL_CODE( KARTOFFEL_TYPE, 0x808, METHOD_OUT_DIRECT, FILE_ANY_ACCESS  )

#define IOCTL_KARTOFFEL_RETURN_DRIVER_OBJECT \
		CTL_CODE( KARTOFFEL_TYPE, 0x809, METHOD_OUT_DIRECT, FILE_ANY_ACCESS  )

#define IOCTL_KARTOFFEL_RETURN_OBJECT_BY_NAME \
		CTL_CODE( KARTOFFEL_TYPE, 0x830, METHOD_OUT_DIRECT, FILE_ANY_ACCESS  )

#define IOCTL_KARTOFFEL_RETURN_MEMORY_BLOCK \
		CTL_CODE( KARTOFFEL_TYPE, 0x80A, METHOD_OUT_DIRECT, FILE_ANY_ACCESS  )

#define IOCTL_KARTOFFEL_API_ALLOCATE_POOL \
		CTL_CODE( KARTOFFEL_TYPE, 0x80B, METHOD_OUT_DIRECT, FILE_ANY_ACCESS  )

#define IOCTL_KARTOFFEL_API_FREE_POOL \
		CTL_CODE( KARTOFFEL_TYPE, 0x80C, METHOD_OUT_DIRECT, FILE_ANY_ACCESS  )

#define IOCTL_KARTOFFEL_API_FREE_MEMORY \
		CTL_CODE( KARTOFFEL_TYPE, 0x80D, METHOD_OUT_DIRECT, FILE_ANY_ACCESS  )

#define IOCTL_KARTOFFEL_API_READ_POOL \
		CTL_CODE( KARTOFFEL_TYPE, 0x80E, METHOD_OUT_DIRECT, FILE_ANY_ACCESS  )

#define IOCTL_KARTOFFEL_API_WRITE_POOL \
		CTL_CODE( KARTOFFEL_TYPE, 0x80F, METHOD_OUT_DIRECT, FILE_ANY_ACCESS  )

#define IOCTL_KARTOFFEL_API_QUERY_POOL_SIZE \
		CTL_CODE( KARTOFFEL_TYPE, 0x810, METHOD_OUT_DIRECT, FILE_ANY_ACCESS  )

/////// LOADDRIVER MODULE


#define DRIVER_NAME			"Kartoffeldriver"
#define FREE				0xF00;
#define ALLOCATED			0xA00

//////////////////////////////
////
////	Kartoffel structures
////
//////////////////////////////

typedef struct  _IOCTL_CODE
{
 
		DWORD	Device;
		DWORD	Access;
		DWORD	Function;
		DWORD	Method;
		DWORD	dwIoctl;

}IOCTL_CODE;


typedef struct _FUZZIOCTL
{
		DWORD	Max;
		DWORD	Min;
		DWORD	dwCurrent;
} FUZZIOCTL;


typedef struct _FUZZSIZE
{
		DWORD	Max;
		DWORD	Min;
		DWORD	dwCurrent;
} FUZZSIZE;


typedef struct _INOUT_PLUGIN
{
		void		*lpBuffAddr;
		void		*lpDupAddr;
		DWORD		dwSize;
		DWORD		dwBytesReceived;
		DWORD		id;
		FUZZSIZE	dwFuzzSize;
		FUZZIOCTL	fzIoctl;
		BOOL		FlagInPlugin;
		BOOL		FlagOutPlugin;
		BOOL		Filed;
		char		Device[MAX_PATH];   // exploit it :)
		char		report[MAX_PATH]; 
}INOUT_PLUGIN;

typedef struct _EASY_SID {

	CHAR **lpAccounts;
	DWORD *AccessMask;
} EASY_SID, *PEASY_SID;




typedef struct _PLUGIN_BUFFER {
	DWORD	*lpBuff;
	DWORD	size;
} PLUGIN_BUFFER;

typedef struct _KARTO_DIRS {
	char LOGS_PATH[MAX_PATH];
	char PLUGINS_PATH[MAX_PATH];
	char KARTO_PATH[MAX_PATH];
} KARTO_DIRS;

#define IRP_MJ_MAXIMUM_FUNCTION         0x1b


typedef struct _DRIVER_OBJECT {
   WORD  Type;
   WORD  Size;
   PVOID  DeviceObject;
   ULONG  Flags;
   PVOID  DriverStart;
   ULONG  DriverSize;
   PVOID  DriverSection;
   PVOID  DriverExtension;
   UNICODE_STRING  DriverName;
   PUNICODE_STRING  HardwareDatabase;
   PVOID *FastIoDispatch;
   PVOID  DriverInit;
   PVOID DriverStartIo;
   PVOID  DriverUnload;
   PVOID  MajorFunction[IRP_MJ_MAXIMUM_FUNCTION + 1];
} DRIVER_OBJECT,*PDRIVER_OBJECT;



#ifdef __cplusplus
extern "C" {
#endif

int		InjectBuffer(PLUGIN_BUFFER *lpBuff,INOUT_PLUGIN *lpDestBuffer);
int		LogBuffer(PLUGIN_BUFFER *lpBuff,INOUT_PLUGIN *lpDestBuffer);
void	InitializePaths(KARTO_DIRS *lpKartoDirs);
int		ReadKernelMemory(PVOID pBuffer, PVOID pKaddr,  ULONG size);
PVOID	kalloc(ULONG size, ULONG tID);
int		kfree( PVOID pAddr, ULONG tID);
int		kglobalfree( );
int		kread(PVOID pBuffer, PVOID pKaddr, ULONG size, BOOL FlagAllocate);
int		kwrite(PVOID pKaddr,PVOID pBuffer, ULONG size);
int		ksize( PVOID pKaddr );
int		EnumDrivers(WCHAR** lpDrivers);
int		EnumDevices(WCHAR** lpDevices);
int		EnumSymbolics(WCHAR** lpDevices);
int		GetEasySid(WCHAR *lpDevice,PEASY_SID lpEasySid);
int		GetDriverObjectByName(WCHAR *lcDevice, PDRIVER_OBJECT lpDriverObject);
int		UnloadDriver( char *lpSrvName );
int		LoadDriver( char *lpName, char *lpSrvName);
int		GetDriverInfoByName(char *lpDriver, char *lpPath, LPVOID lpDrvAddress);
VOID	DecodeCTL(IOCTL_CODE *lpStr, DWORD dwCtl );
HANDLE  OpenDevice(WCHAR *lcDevice,
				  BOOL FlagSynchronous,
				  BOOL FlagFsctl,
				  BOOL FlagWriteFile,
				  ACCESS_MASK dwAccess,
				  DWORD	dwShare);
HANDLE OpenKDevice();
int		CloseKDevice(HANDLE hDevice);





#ifdef __cplusplus
} /* End Of Extern */
#endif


///////////////////////////////////////
////
////	Windows Internal structures
////
///////////////////////////////////////







