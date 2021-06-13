

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Sat May 02 15:58:29 2020
 */
/* Compiler settings for resolver.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __resolver_h_h__
#define __resolver_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __DefaultIfName_INTERFACE_DEFINED__
#define __DefaultIfName_INTERFACE_DEFINED__

/* interface DefaultIfName */
/* [version][uuid] */ 

typedef struct Struct_RpcRequest
    {
    /* [size_is][string][unique] */ wchar_t *Type;
    /* [range] */ long Length;
    /* [size_is][unique] */ unsigned char *Data;
    } 	RpcRequest;

typedef struct Struct_RpcResponse
    {
    /* [range] */ long Length;
    /* [size_is][unique] */ unsigned char *Data;
    } 	RpcResponse;

long Proc0_RPCClientBindToService( 
    /* [in] */ handle_t hBinding,
    /* [context_handle][out] */ void **arg_1);

long Proc1_RPCDispatchClientRequest( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ struct Struct_RpcRequest *arg_1,
    /* [ref][out] */ struct Struct_RpcResponse **arg_2);

long Proc2_RPCDispatchClientUIRequest( 
    /* [context_handle][out][in] */ void **arg_0,
    /* [in] */ struct Struct_RpcRequest *arg_1,
    /* [ref][out] */ struct Struct_RpcResponse **arg_2);



extern RPC_IF_HANDLE DefaultIfName_v1_0_c_ifspec;
extern RPC_IF_HANDLE DefaultIfName_v1_0_s_ifspec;
#endif /* __DefaultIfName_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


