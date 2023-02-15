#if defined(_M_AMD64)

#define WIN10
//#define WIN11
//#define WIN2019
//#define WIN2022

#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )
#pragma warning( disable: 4232 )
#pragma warning( disable: 4024 )

#include <string.h>
#include "storsvc_h.h"

#define TYPE_FORMAT_STRING_SIZE   205                               
#define PROC_FORMAT_STRING_SIZE   1195                              
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _storsvc_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } storsvc_MIDL_TYPE_FORMAT_STRING;

typedef struct _storsvc_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } storsvc_MIDL_PROC_FORMAT_STRING;

typedef struct _storsvc_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } storsvc_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

static const RPC_SYNTAX_IDENTIFIER  _NDR64_RpcTransferSyntax = 
{{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}};


extern const storsvc_MIDL_TYPE_FORMAT_STRING storsvc__MIDL_TypeFormatString;
extern const storsvc_MIDL_PROC_FORMAT_STRING storsvc__MIDL_ProcFormatString;
extern const storsvc_MIDL_EXPR_FORMAT_STRING storsvc__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0

 extern const MIDL_STUBLESS_PROXY_INFO DefaultIfName_ProxyInfo;


static const RPC_CLIENT_INTERFACE DefaultIfName___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    #if defined(WIN10)
    {{0xbe6293d3,0x2827,0x4dda,{0x80,0x57,0x85,0x88,0x24,0x01,0x24,0xc9}},{0,0}},
    #endif
    #if defined(WIN11)
    {{0xec029036,0x297f,0x4f3a,{0xa1,0x69,0x7a,0x2f,0xef,0xa5,0xcc,0x3e}},{0,0}},
    #endif
    #if defined(WIN2019)
    {{0xbe7f785e,0x0e3a,0x4ab7,{0x91,0xde,0x7e,0x46,0xe4,0x43,0xbe,0x29}},{0,0}},
    #endif
    #if defined(WIN2022)
    {{0xd8140e00,0x5c46,0x4ae6,{0x80,0xac,0x2f,0x9a,0x76,0xdf,0x22,0x4c}},{0,0}},
    #endif
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    &DefaultIfName_ProxyInfo,
    0x02000000
    };
RPC_IF_HANDLE DefaultIfName_v0_0_c_ifspec = (RPC_IF_HANDLE)& DefaultIfName___RpcClientInterface;

extern const MIDL_STUB_DESC DefaultIfName_StubDesc;

static RPC_BINDING_HANDLE DefaultIfName__MIDL_AutoBindHandle;


long Proc0_SvcMountVolume( handle_t IDL_handle, short arg_1, long arg_2, long arg_3)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  0,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2,
                  arg_3);
    return ( long  )_RetVal.Simple;
    
}


long Proc1_SvcDismountVolume( handle_t IDL_handle, short arg_1, long arg_2, long arg_3)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  1,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2,
                  arg_3);
    return ( long  )_RetVal.Simple;
    
}


long Proc2_SvcFormatVolume( handle_t IDL_handle, short arg_1, long arg_2, long arg_3)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  2,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2,
                  arg_3);
    return ( long  )_RetVal.Simple;
    
}


long Proc3_SvcGetStorageInstanceCount( handle_t IDL_handle, short arg_1, long *arg_2)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  3,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2);
    return ( long  )_RetVal.Simple;
    
}


long Proc4_SvcGetStorageDeviceInfo( handle_t IDL_handle, short arg_1, long arg_2, struct Struct_34_t *arg_3)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  4,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2,
                  arg_3);
    return ( long  )_RetVal.Simple;
    
}


long Proc5_CCleanupPolicy__CleanupItem( handle_t IDL_handle, short arg_1, short arg_2, long arg_3, struct Struct_76_t *arg_4)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  5,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2,
                  arg_3,
                  arg_4);
    return ( long  )_RetVal.Simple;
    
}


long Proc6_SvcRebootToFlashingMode( handle_t IDL_handle, long arg_1, long arg_2)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  6,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2);
    return ( long  )_RetVal.Simple;
    
}


long Proc7_SvcRebootToUosFlashing( handle_t IDL_handle, long arg_1, long arg_2)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  7,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2);
    return ( long  )_RetVal.Simple;
    
}


long Proc8_SvcFinalizeVolume( handle_t IDL_handle, short arg_1, long arg_2)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  8,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2);
    return ( long  )_RetVal.Simple;
    
}


long Proc9_SvcGetStorageSettings( handle_t IDL_handle, short arg_1, long arg_2, short arg_3, long *arg_4)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  9,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2,
                  arg_3,
                  arg_4);
    return ( long  )_RetVal.Simple;
    
}


long Proc10_SvcResetStoragePolicySettings( handle_t IDL_handle)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  10,
                  0,
                  IDL_handle);
    return ( long  )_RetVal.Simple;
    
}


long Proc11_SvcSetStorageSettings( handle_t IDL_handle, short arg_1, long arg_2, short arg_3, long arg_4)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  11,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2,
                  arg_3,
                  arg_4);
    return ( long  )_RetVal.Simple;
    
}


long Proc12_SvcTriggerStorageCleanup( handle_t IDL_handle, struct Struct_90_t *arg_1)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  12,
                  0,
                  IDL_handle,
                  arg_1);
    return ( long  )_RetVal.Simple;
    
}


long Proc13_SvcTriggerLowStorageNotification( handle_t IDL_handle, short arg_1, long arg_2)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  13,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2);
    return ( long  )_RetVal.Simple;
    
}


__int3264 Proc14_SvcMoveFileInheritSecurity( handle_t IDL_handle, wchar_t *arg_1, wchar_t *arg_2, long arg_3)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  14,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2,
                  arg_3);
    return ( __int3264  )_RetVal.Simple;
    
}


long Proc15_SvcScanVolume( handle_t IDL_handle, short arg_1, long arg_2, struct Struct_112_t *arg_3)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  15,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2,
                  arg_3);
    return ( long  )_RetVal.Simple;
    
}


long Proc16_SvcProcessStorageCardChange( handle_t IDL_handle)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  16,
                  0,
                  IDL_handle);
    return ( long  )_RetVal.Simple;
    
}


long Proc17_SvcProvisionForAppInstall( handle_t IDL_handle, short arg_1, long arg_2)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  17,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2);
    return ( long  )_RetVal.Simple;
    
}


long Proc18_SvcGetStorageInstanceCountForMaps( handle_t IDL_handle, short arg_1, long *arg_2)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  18,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2);
    return ( long  )_RetVal.Simple;
    
}


long Proc19_SvcGetStoragePolicySettings( handle_t IDL_handle, short arg_1, wchar_t *arg_2, long *arg_3)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  19,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2,
                  arg_3);
    return ( long  )_RetVal.Simple;
    
}


long Proc20_SvcSetStoragePolicySettings( handle_t IDL_handle, short arg_1, wchar_t *arg_2, long arg_3)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  20,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2,
                  arg_3);
    return ( long  )_RetVal.Simple;
    
}


long Proc21_SvcTriggerStoragePolicies( handle_t IDL_handle, struct Struct_134_t *arg_1)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  21,
                  0,
                  IDL_handle,
                  arg_1);
    return ( long  )_RetVal.Simple;
    
}


long Proc22_SvcPredictStorageHealth( handle_t IDL_handle, struct Struct_164_t **arg_1, long *arg_2)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  22,
                  0,
                  IDL_handle,
                  arg_1,
                  arg_2);
    return ( long  )_RetVal.Simple;
    
}


long Proc23_SvcGetLastFailedSaveLocationPath( handle_t IDL_handle, wchar_t **arg_1)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall3(
                  ( PMIDL_STUBLESS_PROXY_INFO  )&DefaultIfName_ProxyInfo,
                  23,
                  0,
                  IDL_handle,
                  arg_1);
    return ( long  )_RetVal.Simple;
    
}


#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const storsvc_MIDL_PROC_FORMAT_STRING storsvc__MIDL_ProcFormatString =
    {
        0,
        {

			0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x28 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x16 ),	NdrFcShort( 0x8 ),	0x44,
			0x4,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x20 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x1 ),	NdrFcShort( 0x28 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x16 ),	NdrFcShort( 0x8 ),	0x44,
			0x4,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x20 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x2 ),	NdrFcShort( 0x28 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x16 ),	NdrFcShort( 0x8 ),	0x44,
			0x4,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x20 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x3 ),	NdrFcShort( 0x20 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x6 ),	NdrFcShort( 0x24 ),	0x44,
			0x3,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x2150 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x4 ),	NdrFcShort( 0x28 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x4ba ),	NdrFcShort( 0x4b4 ),	0x44,
			0x4,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x11a ),	NdrFcShort( 0x18 ),	NdrFcShort( 0x22 ),	NdrFcShort( 0x70 ),	NdrFcShort( 0x20 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x5 ),	NdrFcShort( 0x30 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x14 ),	NdrFcShort( 0x44 ),	0x44,
			0x5,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	NdrFcShort( 0x2112 ),	NdrFcShort( 0x20 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x70 ),	NdrFcShort( 0x28 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x6 ),	NdrFcShort( 0x20 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x10 ),	NdrFcShort( 0x8 ),	0x44,
			0x3,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x8,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x7 ),	NdrFcShort( 0x20 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x10 ),	NdrFcShort( 0x8 ),	0x44,
			0x3,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x8,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x8 ),	NdrFcShort( 0x20 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0xe ),	NdrFcShort( 0x8 ),	0x44,
			0x3,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x9 ),	NdrFcShort( 0x30 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x14 ),	NdrFcShort( 0x24 ),	0x44,
			0x5,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x18 ),	0x6,
			0x0,	NdrFcShort( 0x2150 ),	NdrFcShort( 0x20 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x28 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0xa ),	NdrFcShort( 0x10 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x8 ),	0x44,
			0x1,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x70 ),	NdrFcShort( 0x8 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0xb ),	NdrFcShort( 0x30 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x1c ),	NdrFcShort( 0x8 ),	0x44,
			0x5,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x18 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x20 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x28 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0xc ),	NdrFcShort( 0x18 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x8 ),	0x46,
			0x2,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x10b ),	NdrFcShort( 0x8 ),	NdrFcShort( 0x56 ),	NdrFcShort( 0x70 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0xd ),	NdrFcShort( 0x20 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0xe ),	NdrFcShort( 0x8 ),	0x44,
			0x3,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0xe ),	NdrFcShort( 0x28 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x8 ),	NdrFcShort( 0x8 ),	0x46,
			0x4,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x10b ),	NdrFcShort( 0x8 ),	NdrFcShort( 0x68 ),	NdrFcShort( 0x10b ),	NdrFcShort( 0x10 ),	NdrFcShort( 0x68 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x20 ),	0xb8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0xf ),	NdrFcShort( 0x28 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0xe ),	NdrFcShort( 0x8 ),	0x47,
			0x4,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x11b ),	NdrFcShort( 0x18 ),	NdrFcShort( 0x6e ),	NdrFcShort( 0x70 ),	NdrFcShort( 0x20 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x10 ),	NdrFcShort( 0x10 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x8 ),	0x44,
			0x1,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x70 ),	NdrFcShort( 0x8 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x11 ),	NdrFcShort( 0x20 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0xe ),	NdrFcShort( 0x8 ),	0x44,
			0x3,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x48 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x12 ),	NdrFcShort( 0x20 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x6 ),	NdrFcShort( 0x24 ),	0x44,
			0x3,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0x2150 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x13 ),	NdrFcShort( 0x28 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x6 ),	NdrFcShort( 0x24 ),	0x46,
			0x4,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0xb ),	NdrFcShort( 0x10 ),	NdrFcShort( 0x7c ),	NdrFcShort( 0x2150 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x20 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x14 ),	NdrFcShort( 0x28 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0xe ),	NdrFcShort( 0x8 ),	0x46,
			0x4,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x8 ),	0x6,
			0x0,	NdrFcShort( 0xb ),	NdrFcShort( 0x10 ),	NdrFcShort( 0x7c ),	NdrFcShort( 0x48 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x20 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x15 ),	NdrFcShort( 0x18 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x8 ),	0x47,
			0x2,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x11b ),	NdrFcShort( 0x8 ),	NdrFcShort( 0x84 ),	NdrFcShort( 0x70 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x16 ),	NdrFcShort( 0x20 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x24 ),	0x45,
			0x3,	0xa,
			0x3,	NdrFcShort( 0x1 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x2013 ),	NdrFcShort( 0x8 ),	NdrFcShort( 0x94 ),	NdrFcShort( 0x2150 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,	NdrFcShort( 0x70 ),	NdrFcShort( 0x18 ),	0x8,
			0x0,	0x0,
			0x48,	NdrFcLong( 0x0 ),	NdrFcShort( 0x17 ),	NdrFcShort( 0x18 ),	0x32,
			0x0,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x8 ),	0x47,
			0x2,	0xa,
			0x1,	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x201b ),	NdrFcShort( 0x8 ),	NdrFcShort( 0xc8 ),	NdrFcShort( 0x70 ),	NdrFcShort( 0x10 ),	0x8,
			0x0,

			0x0
        }
    };

static const storsvc_MIDL_TYPE_FORMAT_STRING storsvc__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	
			0x11, 0xc,	0x8,
			0x5c,	
			0x11, 0x0,	NdrFcShort( 0x1a ),	
			0x1d,
			0x1,	NdrFcShort( 0x208 ),	0x5,
			0x5b,	
			0x1d,
			0x0,	NdrFcShort( 0x8 ),	0x1,
			0x5b,	
			0x15,
			0x3,	NdrFcShort( 0x10 ),	0x8,
			0x6,	0x6,
			0x4c,	0x0,
			NdrFcShort( 0xfff1 ),
			0x5b,	
			0x15,
			0x7,	NdrFcShort( 0x448 ),	0x8,
			0x4c,	0x0,
			NdrFcShort( 0xffe1 ),
			0x8,	0x6,
			0x6,	0x8,
			0x6,	0x6,
			0x4c,	0x0,
			NdrFcShort( 0xffe3 ),
			0x8,	0xb,
			0x4c,	0x0,
			NdrFcShort( 0xffd1 ),
			0x8,	0x8,
			0x5b,	
			0x11, 0x4,	NdrFcShort( 0x8 ),	
			0x1d,
			0x3,	NdrFcShort( 0x8 ),	0x8,
			0x5b,	
			0x15,
			0x3,	NdrFcShort( 0x8 ),	0x4c,
			0x0,	NdrFcShort( 0xfff4 ),	0x5c,
			0x5b,	
			0x11, 0x0,	NdrFcShort( 0x2 ),	0xb1,
			0x3,	NdrFcShort( 0x10 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	0x8,
			0x6,	0x3e,
			0x8,	0x6,
			0x6,	0x5c,
			0x5b,	
			0x11, 0x8,	
			0x25,
			0x5c,	
			0x11, 0x0,	NdrFcShort( 0x2 ),	
			0x1a,
			0x3,	NdrFcShort( 0x10 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	0x8,
			0x8,	0x8,
			0x6,	0x3e,
			0x5b,	
			0x12, 0x8,	
			0x25,
			0x5c,	
			0x11, 0x0,	NdrFcShort( 0x2 ),	0xb1,
			0x3,	NdrFcShort( 0x18 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	0x8,
			0x6,	0x3e,
			0x8,	0x8,
			0x8,	0x8,
			0x5b,	
			0x11, 0x14,	NdrFcShort( 0x2 ),	
			0x12, 0x20,	NdrFcShort( 0x18 ),	
			0x1d,
			0x1,	NdrFcShort( 0x4e ),	0x5,
			0x5b,	
			0x1a,
			0x7,	NdrFcShort( 0x58 ),	NdrFcShort( 0x0 ),	NdrFcShort( 0x0 ),	0xc,
			0x4c,	0x0,
			NdrFcShort( 0xffef ),
			0x3e,	0x5c,
			0x5b,	
			0x21,
			0x7,	NdrFcShort( 0x0 ),	0x28,
			0x54,	NdrFcShort( 0x10 ),	NdrFcShort( 0x0 ),	NdrFcLong( 0xffffffff ),	NdrFcShort( 0x0 ),	0x4c,
			0x0,	NdrFcShort( 0xffde ),	0x5c,
			0x5b,	
			0x11, 0x14,	NdrFcShort( 0xffb2 ),

			0x0
        }
    };

static const unsigned short DefaultIfName_FormatStringOffsetTable[] =
    {
    0,
    54,
    108,
    162,
    210,
    264,
    324,
    372,
    420,
    468,
    528,
    564,
    624,
    666,
    714,
    768,
    822,
    858,
    906,
    954,
    1008,
    1062,
    1104,
    1152
    };



#endif

#if defined(_M_AMD64)




#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif


#include "ndr64types.h"
#include "pshpack8.h"


typedef 
NDR64_FORMAT_CHAR
__midl_frag124_t;
extern const __midl_frag124_t __midl_frag124;

typedef 
struct _NDR64_CONFORMANT_STRING_FORMAT
__midl_frag123_t;
extern const __midl_frag123_t __midl_frag123;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag122_t;
extern const __midl_frag122_t __midl_frag122;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag121_t;
extern const __midl_frag121_t __midl_frag121;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
}
__midl_frag120_t;
extern const __midl_frag120_t __midl_frag120;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag117_t;
extern const __midl_frag117_t __midl_frag117;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
}
__midl_frag116_t;
extern const __midl_frag116_t __midl_frag116;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_OPERATOR frag2;
    struct _NDR64_EXPR_VAR frag3;
}
__midl_frag115_t;
extern const __midl_frag115_t __midl_frag115;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag114_t;
extern const __midl_frag114_t __midl_frag114;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag113_t;
extern const __midl_frag113_t __midl_frag113;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag112_t;
extern const __midl_frag112_t __midl_frag112;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
}
__midl_frag111_t;
extern const __midl_frag111_t __midl_frag111;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag2;
        struct _NDR64_MEMPAD_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag6;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag7;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag8;
    } frag2;
}
__midl_frag109_t;
extern const __midl_frag109_t __midl_frag109;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag108_t;
extern const __midl_frag108_t __midl_frag108;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
}
__midl_frag107_t;
extern const __midl_frag107_t __midl_frag107;

typedef 
NDR64_FORMAT_CHAR
__midl_frag102_t;
extern const __midl_frag102_t __midl_frag102;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag101_t;
extern const __midl_frag101_t __midl_frag101;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag94_t;
extern const __midl_frag94_t __midl_frag94;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
}
__midl_frag89_t;
extern const __midl_frag89_t __midl_frag89;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
}
__midl_frag85_t;
extern const __midl_frag85_t __midl_frag85;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
}
__midl_frag83_t;
extern const __midl_frag83_t __midl_frag83;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
}
__midl_frag81_t;
extern const __midl_frag81_t __midl_frag81;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag80_t;
extern const __midl_frag80_t __midl_frag80;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag77_t;
extern const __midl_frag77_t __midl_frag77;

typedef 
NDR64_FORMAT_CHAR
__midl_frag76_t;
extern const __midl_frag76_t __midl_frag76;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag73_t;
extern const __midl_frag73_t __midl_frag73;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag70_t;
extern const __midl_frag70_t __midl_frag70;

typedef 
struct 
{
    struct _NDR64_BOGUS_STRUCTURE_HEADER_FORMAT frag1;
    struct 
    {
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag1;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag2;
        struct _NDR64_MEMPAD_FORMAT frag3;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag4;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag5;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag6;
        struct _NDR64_SIMPLE_MEMBER_FORMAT frag7;
    } frag2;
}
__midl_frag64_t;
extern const __midl_frag64_t __midl_frag64;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag63_t;
extern const __midl_frag63_t __midl_frag63;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
}
__midl_frag62_t;
extern const __midl_frag62_t __midl_frag62;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
}
__midl_frag56_t;
extern const __midl_frag56_t __midl_frag56;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
}
__midl_frag47_t;
extern const __midl_frag47_t __midl_frag47;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
}
__midl_frag39_t;
extern const __midl_frag39_t __midl_frag39;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
}
__midl_frag33_t;
extern const __midl_frag33_t __midl_frag33;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag32_t;
extern const __midl_frag32_t __midl_frag32;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
}
__midl_frag28_t;
extern const __midl_frag28_t __midl_frag28;

typedef 
struct 
{
    struct _NDR64_STRUCTURE_HEADER_FORMAT frag1;
}
__midl_frag26_t;
extern const __midl_frag26_t __midl_frag26;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag25_t;
extern const __midl_frag25_t __midl_frag25;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag22_t;
extern const __midl_frag22_t __midl_frag22;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_BIND_AND_NOTIFY_EXTENSION frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
}
__midl_frag12_t;
extern const __midl_frag12_t __midl_frag12;

typedef 
NDR64_FORMAT_UINT32
__midl_frag1_t;
extern const __midl_frag1_t __midl_frag1;

static const __midl_frag124_t __midl_frag124 =
0x5;

static const __midl_frag123_t __midl_frag123 =
{
    {
        0x64,
        {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT16) 2
    }
};

static const __midl_frag122_t __midl_frag122 =
{
    0x21,
    (NDR64_UINT8) 0,
    (NDR64_UINT16) 0,
    &__midl_frag123
};

static const __midl_frag121_t __midl_frag121 =
{
    0x20,
    (NDR64_UINT8) 20,
    (NDR64_UINT16) 0,
    &__midl_frag122
};

static const __midl_frag120_t __midl_frag120 =
{
    {
        (NDR64_UINT32) 17694784,
        (NDR64_UINT32) 24 ,
        (NDR64_UINT32) 0,
        (NDR64_UINT32) 8,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 2,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag121,
        {
            1,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            1
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    }
};

static const __midl_frag117_t __midl_frag117 =
{
    0x20,
    (NDR64_UINT8) 12,
    (NDR64_UINT16) 0,
    &__midl_frag124
};

static const __midl_frag116_t __midl_frag116 =
{
    {
        0x30,
        (NDR64_UINT8) 7,
        {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0,
        (NDR64_UINT32) 88
    }
};

static const __midl_frag115_t __midl_frag115 =
{
    (NDR64_UINT32) 0,
    {
        0x4,
        0x5,
        0x5,
        (NDR64_UINT8) 0
    },
    {
        0x3,
        0x7,
        (NDR64_UINT16) 0,
        (NDR64_UINT32) 16
    }
};

static const __midl_frag114_t __midl_frag114 =
{
    {
        0x41,
        (NDR64_UINT8) 7,
        {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0,
        (NDR64_UINT32) 88,
        &__midl_frag115
    },
    {
        (NDR64_UINT32) 88,
        &__midl_frag116
    }
};

static const __midl_frag113_t __midl_frag113 =
{
    0x21,
    (NDR64_UINT8) 32,
    (NDR64_UINT16) 0,
    &__midl_frag114
};

static const __midl_frag112_t __midl_frag112 =
{
    0x20,
    (NDR64_UINT8) 20,
    (NDR64_UINT16) 0,
    &__midl_frag113
};

static const __midl_frag111_t __midl_frag111 =
{
    {
        (NDR64_UINT32) 21626944,
        (NDR64_UINT32) 32 ,
        (NDR64_UINT32) 0,
        (NDR64_UINT32) 40,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 3,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag112,
        {
            1,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            1
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            1
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        24,
    }
};

static const __midl_frag109_t __midl_frag109 =
{
    {
        0x35,
        (NDR64_UINT8) 3,
        {
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0,
        (NDR64_UINT32) 24,
        0,
        0,
        0,
    },
    {
        {
            0x5,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        },
        {
            0x4,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        },
        {
            0x90,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 2,
            (NDR64_UINT32) 0
        },
        {
            0x5,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        },
        {
            0x5,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        },
        {
            0x5,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        },
        {
            0x5,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        },
        {
            0x93,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        }
    }
};

static const __midl_frag108_t __midl_frag108 =
{
    0x20,
    (NDR64_UINT8) 0,
    (NDR64_UINT16) 0,
    &__midl_frag109
};

static const __midl_frag107_t __midl_frag107 =
{
    {
        (NDR64_UINT32) 17694784,
        (NDR64_UINT32) 24 ,
        (NDR64_UINT32) 0,
        (NDR64_UINT32) 8,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 2,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag109,
        {
            1,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    }
};

static const __midl_frag102_t __midl_frag102 =
0x4;

static const __midl_frag101_t __midl_frag101 =
{
    {
        (NDR64_UINT32) 17563712,
        (NDR64_UINT32) 40 ,
        (NDR64_UINT32) 14,
        (NDR64_UINT32) 8,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 4,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag122,
        {
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        24,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        32,
    }
};

static const __midl_frag94_t __midl_frag94 =
{
    {
        (NDR64_UINT32) 17563712,
        (NDR64_UINT32) 40 ,
        (NDR64_UINT32) 6,
        (NDR64_UINT32) 40,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 4,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag122,
        {
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            1
        },
        (NDR64_UINT16) 0,
        24,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        32,
    }
};

static const __midl_frag89_t __midl_frag89 =
{
    {
        (NDR64_UINT32) 17301568,
        (NDR64_UINT32) 32 ,
        (NDR64_UINT32) 6,
        (NDR64_UINT32) 40,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 3,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            1
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        24,
    }
};

static const __midl_frag85_t __midl_frag85 =
{
    {
        (NDR64_UINT32) 17301568,
        (NDR64_UINT32) 32 ,
        (NDR64_UINT32) 14,
        (NDR64_UINT32) 8,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 3,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        24,
    }
};

static const __midl_frag83_t __midl_frag83 =
{
    {
        (NDR64_UINT32) 17301568,
        (NDR64_UINT32) 16 ,
        (NDR64_UINT32) 0,
        (NDR64_UINT32) 8,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 1,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    }
};

static const __midl_frag81_t __midl_frag81 =
{
    {
        0x30,
        (NDR64_UINT8) 3,
        {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0,
        (NDR64_UINT32) 16
    }
};

static const __midl_frag80_t __midl_frag80 =
{
    0x20,
    (NDR64_UINT8) 0,
    (NDR64_UINT16) 0,
    &__midl_frag81
};

static const __midl_frag77_t __midl_frag77 =
{
    {
        (NDR64_UINT32) 17301568,
        (NDR64_UINT32) 40 ,
        (NDR64_UINT32) 70,
        (NDR64_UINT32) 64,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 4,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag81,
        {
            0,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        24,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        32,
    }
};

static const __midl_frag76_t __midl_frag76 =
0x7;

static const __midl_frag73_t __midl_frag73 =
{
    0x20,
    (NDR64_UINT8) 0,
    (NDR64_UINT16) 0,
    &__midl_frag123
};

static const __midl_frag70_t __midl_frag70 =
{
    {
        (NDR64_UINT32) 17563712,
        (NDR64_UINT32) 40 ,
        (NDR64_UINT32) 8,
        (NDR64_UINT32) 16,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 4,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag123,
        {
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag123,
        {
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        24,
    },
    {
        &__midl_frag76,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        32,
    }
};

static const __midl_frag64_t __midl_frag64 =
{
    {
        0x35,
        (NDR64_UINT8) 3,
        {
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0,
        (NDR64_UINT32) 16,
        0,
        0,
        0,
    },
    {
        {
            0x5,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        },
        {
            0x4,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        },
        {
            0x90,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 2,
            (NDR64_UINT32) 0
        },
        {
            0x5,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        },
        {
            0x4,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        },
        {
            0x4,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        },
        {
            0x93,
            (NDR64_UINT8) 0,
            (NDR64_UINT16) 0,
            (NDR64_UINT32) 0
        }
    }
};

static const __midl_frag63_t __midl_frag63 =
{
    0x20,
    (NDR64_UINT8) 0,
    (NDR64_UINT16) 0,
    &__midl_frag64
};

static const __midl_frag62_t __midl_frag62 =
{
    {
        (NDR64_UINT32) 17563712,
        (NDR64_UINT32) 24 ,
        (NDR64_UINT32) 0,
        (NDR64_UINT32) 8,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 2,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag64,
        {
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    }
};

static const __midl_frag56_t __midl_frag56 =
{
    {
        (NDR64_UINT32) 17301568,
        (NDR64_UINT32) 48 ,
        (NDR64_UINT32) 28,
        (NDR64_UINT32) 8,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 5,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        24,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        32,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        40,
    }
};

static const __midl_frag47_t __midl_frag47 =
{
    {
        (NDR64_UINT32) 17301568,
        (NDR64_UINT32) 48 ,
        (NDR64_UINT32) 20,
        (NDR64_UINT32) 40,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 5,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        24,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            1
        },
        (NDR64_UINT16) 0,
        32,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        40,
    }
};

static const __midl_frag39_t __midl_frag39 =
{
    {
        (NDR64_UINT32) 17301568,
        (NDR64_UINT32) 32 ,
        (NDR64_UINT32) 16,
        (NDR64_UINT32) 8,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 3,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        24,
    }
};

static const __midl_frag33_t __midl_frag33 =
{
    {
        0x30,
        (NDR64_UINT8) 3,
        {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0,
        (NDR64_UINT32) 8
    }
};

static const __midl_frag32_t __midl_frag32 =
{
    0x20,
    (NDR64_UINT8) 4,
    (NDR64_UINT16) 0,
    &__midl_frag33
};

static const __midl_frag28_t __midl_frag28 =
{
    {
        (NDR64_UINT32) 17301568,
        (NDR64_UINT32) 48 ,
        (NDR64_UINT32) 20,
        (NDR64_UINT32) 72,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 5,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        24,
    },
    {
        &__midl_frag33,
        {
            0,
            1,
            0,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            1
        },
        (NDR64_UINT16) 0,
        32,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        40,
    }
};

static const __midl_frag26_t __midl_frag26 =
{
    {
        0x30,
        (NDR64_UINT8) 7,
        {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0,
        (NDR64_UINT32) 1096
    }
};

static const __midl_frag25_t __midl_frag25 =
{
    0x20,
    (NDR64_UINT8) 0,
    (NDR64_UINT16) 0,
    &__midl_frag26
};

static const __midl_frag22_t __midl_frag22 =
{
    {
        (NDR64_UINT32) 17301568,
        (NDR64_UINT32) 40 ,
        (NDR64_UINT32) 1198,
        (NDR64_UINT32) 1192,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 4,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag26,
        {
            0,
            1,
            0,
            1,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        24,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        32,
    }
};

static const __midl_frag12_t __midl_frag12 =
{
    {
        (NDR64_UINT32) 17301568,
        (NDR64_UINT32) 40 ,
        (NDR64_UINT32) 22,
        (NDR64_UINT32) 8,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 0,
        (NDR64_UINT16) 4,
        (NDR64_UINT16) 8
    },
    {
        {
            0x72,
            (NDR64_UINT8) 0,
            0,
            (NDR64_UINT8) 0,
            (NDR64_UINT8) 0
        },
        (NDR64_UINT16) 0
    },
    {
        &__midl_frag102,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        8,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        16,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        24,
    },
    {
        &__midl_frag124,
        {
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0,
            0
        },
        (NDR64_UINT16) 0,
        32,
    }
};

static const __midl_frag1_t __midl_frag1 =
(NDR64_UINT32) 0;


#include "poppack.h"


static const FormatInfoRef DefaultIfName_Ndr64ProcTable[] =
    {
    &__midl_frag12,
    &__midl_frag12,
    &__midl_frag12,
    &__midl_frag89,
    &__midl_frag22,
    &__midl_frag28,
    &__midl_frag39,
    &__midl_frag39,
    &__midl_frag85,
    &__midl_frag47,
    &__midl_frag83,
    &__midl_frag56,
    &__midl_frag62,
    &__midl_frag85,
    &__midl_frag70,
    &__midl_frag77,
    &__midl_frag83,
    &__midl_frag85,
    &__midl_frag89,
    &__midl_frag94,
    &__midl_frag101,
    &__midl_frag107,
    &__midl_frag111,
    &__midl_frag120
    };


static const MIDL_STUB_DESC DefaultIfName_StubDesc = 
    {
    (void *)& DefaultIfName___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &DefaultIfName__MIDL_AutoBindHandle,
    0,
    0,
    0,
    0,
    storsvc__MIDL_TypeFormatString.Format,
    1,
    0x60001,
    0,
    0x801026e,
    0,
    0,
    0,
    0x2000001,
    0,
    (void *)& DefaultIfName_ProxyInfo,
    0
    };

static const MIDL_SYNTAX_INFO DefaultIfName_SyntaxInfo [  2 ] = 
    {
    {
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    storsvc__MIDL_ProcFormatString.Format,
    DefaultIfName_FormatStringOffsetTable,
    storsvc__MIDL_TypeFormatString.Format,
    0,
    0,
    0
    }
    ,{
    {{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}},
    0,
    0 ,
    (unsigned short *) DefaultIfName_Ndr64ProcTable,
    0,
    0,
    0,
    0
    }
    };

static const MIDL_STUBLESS_PROXY_INFO DefaultIfName_ProxyInfo =
    {
    &DefaultIfName_StubDesc,
    storsvc__MIDL_ProcFormatString.Format,
    DefaultIfName_FormatStringOffsetTable,
    (RPC_SYNTAX_IDENTIFIER*)&_RpcTransferSyntax,
    2,
    (MIDL_SYNTAX_INFO*)DefaultIfName_SyntaxInfo
    
    };

#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif

