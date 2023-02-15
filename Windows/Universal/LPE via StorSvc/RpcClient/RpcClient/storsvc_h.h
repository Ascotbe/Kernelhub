
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif


#ifndef __storsvc_h_h__
#define __storsvc_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __DefaultIfName_INTERFACE_DEFINED__
#define __DefaultIfName_INTERFACE_DEFINED__ 

typedef struct Struct_22_t
    {
    long StructMember0;
    short StructMember1;
    short StructMember2;
    byte StructMember3[ 8 ];
    } 	Struct_22_t;

typedef struct Struct_34_t
    {
    long StructMember0;
    wchar_t StructMember1[ 260 ];
    long StructMember2;
    short StructMember3;
    short StructMember4;
    long StructMember5;
    short StructMember6;
    short StructMember7;
    struct Struct_22_t StructMember8;
    long StructMember9;
    hyper StructMember10;
    wchar_t StructMember11[ 260 ];
    long StructMember12;
    long StructMember13;
    } 	Struct_34_t;

typedef struct Struct_76_t
    {
    long StructMember0[ 2 ];
    } 	Struct_76_t;

typedef struct Struct_90_t
    {
    long StructMember0;
    short StructMember1;
    long StructMember2;
    short StructMember3;
    short StructMember4;
    } 	Struct_90_t;

typedef struct Struct_112_t
    {
    long StructMember0;
    long StructMember1;
    long StructMember2;
    short StructMember3;
    } 	Struct_112_t;

typedef struct Struct_134_t
    {
    long StructMember0;
    short StructMember1;
    long StructMember2;
    long StructMember3;
    long StructMember4;
    long StructMember5;
    } 	Struct_134_t;

typedef struct Struct_164_t
    {
    double StructMember0;
    wchar_t StructMember1[ 39 ];
    } 	Struct_164_t;

long Proc0_SvcMountVolume( handle_t IDL_handle, short arg_1, long arg_2, long arg_3);

long Proc1_SvcDismountVolume( handle_t IDL_handle, short arg_1, long arg_2, long arg_3);

long Proc2_SvcFormatVolume( handle_t IDL_handle, short arg_1, long arg_2, long arg_3);

long Proc3_SvcGetStorageInstanceCount( handle_t IDL_handle, short arg_1, long *arg_2);

long Proc4_SvcGetStorageDeviceInfo( handle_t IDL_handle, short arg_1, long arg_2, struct Struct_34_t *arg_3);

long Proc5_CCleanupPolicy__CleanupItem( handle_t IDL_handle, short arg_1, short arg_2, long arg_3, struct Struct_76_t *arg_4);

long Proc6_SvcRebootToFlashingMode( handle_t IDL_handle, long arg_1, long arg_2);

long Proc7_SvcRebootToUosFlashing( handle_t IDL_handle, long arg_1, long arg_2);

long Proc8_SvcFinalizeVolume( handle_t IDL_handle, short arg_1, long arg_2);

long Proc9_SvcGetStorageSettings( handle_t IDL_handle, short arg_1, long arg_2, short arg_3, long *arg_4);

long Proc10_SvcResetStoragePolicySettings( handle_t IDL_handle);

long Proc11_SvcSetStorageSettings( handle_t IDL_handle, short arg_1, long arg_2, short arg_3, long arg_4);

long Proc12_SvcTriggerStorageCleanup( handle_t IDL_handle, struct Struct_90_t *arg_1);

long Proc13_SvcTriggerLowStorageNotification( handle_t IDL_handle, short arg_1, long arg_2);

__int3264 Proc14_SvcMoveFileInheritSecurity( handle_t IDL_handle, wchar_t *arg_1, wchar_t *arg_2, long arg_3);

long Proc15_SvcScanVolume( handle_t IDL_handle, short arg_1, long arg_2, struct Struct_112_t *arg_3);

long Proc16_SvcProcessStorageCardChange( handle_t IDL_handle);

long Proc17_SvcProvisionForAppInstall( handle_t IDL_handle, short arg_1, long arg_2);

long Proc18_SvcGetStorageInstanceCountForMaps( handle_t IDL_handle, short arg_1, long *arg_2);

long Proc19_SvcGetStoragePolicySettings( handle_t IDL_handle, short arg_1, wchar_t *arg_2, long *arg_3);

long Proc20_SvcSetStoragePolicySettings( handle_t IDL_handle, short arg_1, wchar_t *arg_2, long arg_3);

long Proc21_SvcTriggerStoragePolicies( handle_t IDL_handle, struct Struct_134_t *arg_1);

long Proc22_SvcPredictStorageHealth( handle_t IDL_handle, struct Struct_164_t **arg_1, long *arg_2);

long Proc23_SvcGetLastFailedSaveLocationPath( handle_t IDL_handle, wchar_t **arg_1);



extern RPC_IF_HANDLE DefaultIfName_v0_0_c_ifspec;
extern RPC_IF_HANDLE DefaultIfName_v0_0_s_ifspec;
#endif

#ifdef __cplusplus
}
#endif

#endif


