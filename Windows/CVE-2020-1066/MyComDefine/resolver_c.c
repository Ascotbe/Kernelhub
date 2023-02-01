

/* this ALWAYS GENERATED file contains the RPC client stubs */


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

#if !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#include <string.h>

#include "resolver_h.h"

#define TYPE_FORMAT_STRING_SIZE   125                               
#define PROC_FORMAT_STRING_SIZE   141                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _resolver_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } resolver_MIDL_TYPE_FORMAT_STRING;

typedef struct _resolver_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } resolver_MIDL_PROC_FORMAT_STRING;

typedef struct _resolver_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } resolver_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const resolver_MIDL_TYPE_FORMAT_STRING resolver__MIDL_TypeFormatString;
extern const resolver_MIDL_PROC_FORMAT_STRING resolver__MIDL_ProcFormatString;
extern const resolver_MIDL_EXPR_FORMAT_STRING resolver__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: DefaultIfName, ver. 1.0,
   GUID={0x00645e6c,0xfc9f,0x4a0c,{0x98,0x96,0xf0,0x0b,0x66,0x29,0x77,0x98}} */



static const RPC_CLIENT_INTERFACE DefaultIfName___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x00645e6c,0xfc9f,0x4a0c,{0x98,0x96,0xf0,0x0b,0x66,0x29,0x77,0x98}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0x00000000
    };
RPC_IF_HANDLE DefaultIfName_v1_0_c_ifspec = (RPC_IF_HANDLE)& DefaultIfName___RpcClientInterface;

extern const MIDL_STUB_DESC DefaultIfName_StubDesc;

static RPC_BINDING_HANDLE DefaultIfName__MIDL_AutoBindHandle;


long Proc0_RPCClientBindToService( 
    /* [in] */ handle_t hBinding,
    /* [context_handle][out] */ void **arg_1)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&DefaultIfName_StubDesc,
                  (PFORMAT_STRING) &resolver__MIDL_ProcFormatString.Format[0],
                  ( unsigned char * )&hBinding);
    return ( long  )_RetVal.Simple;
    
}


long Proc1_RPCDispatchClientRequest( 
    /* [in] */ handle_t IDL_handle,
    /* [in] */ struct Struct_RpcRequest *arg_1,
    /* [ref][out] */ struct Struct_RpcResponse **arg_2)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&DefaultIfName_StubDesc,
                  (PFORMAT_STRING) &resolver__MIDL_ProcFormatString.Format[40],
                  ( unsigned char * )&IDL_handle);
    return ( long  )_RetVal.Simple;
    
}


long Proc2_RPCDispatchClientUIRequest( 
    /* [context_handle][out][in] */ void **arg_0,
    /* [in] */ struct Struct_RpcRequest *arg_1,
    /* [ref][out] */ struct Struct_RpcResponse **arg_2)
{

    CLIENT_CALL_RETURN _RetVal;

    _RetVal = NdrClientCall2(
                  ( PMIDL_STUB_DESC  )&DefaultIfName_StubDesc,
                  (PFORMAT_STRING) &resolver__MIDL_ProcFormatString.Format[86],
                  ( unsigned char * )&arg_0);
    return ( long  )_RetVal.Simple;
    
}


#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const resolver_MIDL_PROC_FORMAT_STRING resolver__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure Proc0_RPCClientBindToService */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 14 */	NdrFcShort( 0x0 ),	/* 0 */
/* 16 */	NdrFcShort( 0x40 ),	/* 64 */
/* 18 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 20 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hBinding */

/* 28 */	NdrFcShort( 0x110 ),	/* Flags:  out, simple ref, */
/* 30 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 32 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Parameter arg_1 */

/* 34 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 36 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 38 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Proc1_RPCDispatchClientRequest */


	/* Return value */

/* 40 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 42 */	NdrFcLong( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0x1 ),	/* 1 */
/* 48 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 50 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 52 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x8 ),	/* 8 */
/* 58 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 60 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 62 */	NdrFcShort( 0x1 ),	/* 1 */
/* 64 */	NdrFcShort( 0x1 ),	/* 1 */
/* 66 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter IDL_handle */

/* 68 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 70 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 72 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter arg_1 */

/* 74 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 76 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 78 */	NdrFcShort( 0x44 ),	/* Type Offset=68 */

	/* Parameter arg_2 */

/* 80 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 82 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 84 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Proc2_RPCDispatchClientUIRequest */


	/* Return value */

/* 86 */	0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/* 88 */	NdrFcLong( 0x0 ),	/* 0 */
/* 92 */	NdrFcShort( 0x2 ),	/* 2 */
/* 94 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 96 */	0x30,		/* FC_BIND_CONTEXT */
			0xe0,		/* Ctxt flags:  via ptr, in, out, */
/* 98 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 100 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 102 */	NdrFcShort( 0x38 ),	/* 56 */
/* 104 */	NdrFcShort( 0x40 ),	/* 64 */
/* 106 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 108 */	0x8,		/* 8 */
			0x7,		/* Ext Flags:  new corr desc, clt corr check, srv corr check, */
/* 110 */	NdrFcShort( 0x1 ),	/* 1 */
/* 112 */	NdrFcShort( 0x1 ),	/* 1 */
/* 114 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter arg_0 */

/* 116 */	NdrFcShort( 0x118 ),	/* Flags:  in, out, simple ref, */
/* 118 */	NdrFcShort( 0x0 ),	/* x86 Stack size/offset = 0 */
/* 120 */	NdrFcShort( 0x78 ),	/* Type Offset=120 */

	/* Parameter arg_1 */

/* 122 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 124 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 126 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Parameter arg_2 */

/* 128 */	NdrFcShort( 0x2013 ),	/* Flags:  must size, must free, out, srv alloc size=8 */
/* 130 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 132 */	NdrFcShort( 0x44 ),	/* Type Offset=68 */

	/* Return value */

/* 134 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 136 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 138 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const resolver_MIDL_TYPE_FORMAT_STRING resolver__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/*  4 */	NdrFcShort( 0x2 ),	/* Offset= 2 (6) */
/*  6 */	0x30,		/* FC_BIND_CONTEXT */
			0xa0,		/* Ctxt flags:  via ptr, out, */
/*  8 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 10 */	
			0x11, 0x0,	/* FC_RP */
/* 12 */	NdrFcShort( 0x20 ),	/* Offset= 32 (44) */
/* 14 */	0xb7,		/* FC_RANGE */
			0x8,		/* 8 */
/* 16 */	NdrFcLong( 0x0 ),	/* 0 */
/* 20 */	NdrFcLong( 0x1400000 ),	/* 20971520 */
/* 24 */	
			0x25,		/* FC_C_WSTRING */
			0x44,		/* FC_STRING_SIZED */
/* 26 */	0x40,		/* Corr desc:  constant, val=101 */
			0x0,		/* 0 */
/* 28 */	NdrFcShort( 0x65 ),	/* 101 */
/* 30 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 32 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 34 */	NdrFcShort( 0x1 ),	/* 1 */
/* 36 */	0x18,		/* Corr desc:  field pointer, FC_LONG */
			0x0,		/*  */
/* 38 */	NdrFcShort( 0x4 ),	/* 4 */
/* 40 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 42 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 44 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 46 */	NdrFcShort( 0xc ),	/* 12 */
/* 48 */	NdrFcShort( 0x0 ),	/* 0 */
/* 50 */	NdrFcShort( 0xa ),	/* Offset= 10 (60) */
/* 52 */	0x36,		/* FC_POINTER */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 54 */	0x0,		/* 0 */
			NdrFcShort( 0xffd7 ),	/* Offset= -41 (14) */
			0x36,		/* FC_POINTER */
/* 58 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 60 */	
			0x12, 0x0,	/* FC_UP */
/* 62 */	NdrFcShort( 0xffda ),	/* Offset= -38 (24) */
/* 64 */	
			0x12, 0x0,	/* FC_UP */
/* 66 */	NdrFcShort( 0xffde ),	/* Offset= -34 (32) */
/* 68 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] [pointer_deref] */
/* 70 */	NdrFcShort( 0x2 ),	/* Offset= 2 (72) */
/* 72 */	
			0x12, 0x0,	/* FC_UP */
/* 74 */	NdrFcShort( 0x18 ),	/* Offset= 24 (98) */
/* 76 */	0xb7,		/* FC_RANGE */
			0x8,		/* 8 */
/* 78 */	NdrFcLong( 0x0 ),	/* 0 */
/* 82 */	NdrFcLong( 0x1400000 ),	/* 20971520 */
/* 86 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 88 */	NdrFcShort( 0x1 ),	/* 1 */
/* 90 */	0x18,		/* Corr desc:  field pointer, FC_LONG */
			0x0,		/*  */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 96 */	0x2,		/* FC_CHAR */
			0x5b,		/* FC_END */
/* 98 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 100 */	NdrFcShort( 0x8 ),	/* 8 */
/* 102 */	NdrFcShort( 0x0 ),	/* 0 */
/* 104 */	NdrFcShort( 0x8 ),	/* Offset= 8 (112) */
/* 106 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 108 */	NdrFcShort( 0xffe0 ),	/* Offset= -32 (76) */
/* 110 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 112 */	
			0x12, 0x0,	/* FC_UP */
/* 114 */	NdrFcShort( 0xffe4 ),	/* Offset= -28 (86) */
/* 116 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 118 */	NdrFcShort( 0x2 ),	/* Offset= 2 (120) */
/* 120 */	0x30,		/* FC_BIND_CONTEXT */
			0xe1,		/* Ctxt flags:  via ptr, in, out, can't be null */
/* 122 */	0x0,		/* 0 */
			0x0,		/* 0 */

			0x0
        }
    };

static const unsigned short DefaultIfName_FormatStringOffsetTable[] =
    {
    0,
    40,
    86
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
    resolver__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x800025b, /* MIDL Version 8.0.603 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64) && !defined(_ARM_) */

