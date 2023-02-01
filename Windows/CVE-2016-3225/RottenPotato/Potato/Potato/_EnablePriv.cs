using System;
using System.Text;
using System.Runtime.InteropServices;
using System.ComponentModel;

namespace Potato
{
    class DuplicateEx
    {
        [StructLayoutAttribute(LayoutKind.Sequential)]
        public struct SECURITY_DESCRIPTOR
        {
            public byte revision;
            public byte size;
            public short control;
            public IntPtr owner;
            public IntPtr group;
            public IntPtr sacl;
            public IntPtr dacl;
        }

        public enum SECURITY_IMPERSONATION_LEVEL
        {
            SecurityAnonymous,
            SecurityIdentification,
            SecurityImpersonation,
            SecurityDelegation
        }

        public enum TOKEN_TYPE
        {
            TokenPrimary = 1,
            TokenImpersonation
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SECURITY_ATTRIBUTES
        {
            public int nLength;
            public IntPtr lpSecurityDescriptor;
            public bool bInheritHandle;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SYSTEM_HANDLE
        {
            ulong ProcessId;
            Byte ObjectTypeNumber;
            Byte Flags;
            ushort Handle;
            IntPtr Object;
            uint ACCESS_MASK;
        }

        [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public extern static bool DuplicateTokenEx(
            IntPtr hExistingToken,
            uint dwDesiredAccess,
            ref SECURITY_ATTRIBUTES lpTokenAttributes,
            SECURITY_IMPERSONATION_LEVEL ImpersonationLevel,
            TOKEN_TYPE TokenType,
            out IntPtr phNewToken);

        [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public extern static bool DuplicateTokenEx(
            IntPtr hExistingToken,
            uint dwDesiredAccess,
            uint lpTokenAttributes,
            SECURITY_IMPERSONATION_LEVEL ImpersonationLevel,
            TOKEN_TYPE TokenType,
            out IntPtr phNewToken);

        [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        public static extern bool InitializeSecurityDescriptor(out SECURITY_DESCRIPTOR sd, uint dwRevision);

        [DllImport("advapi32.dll", CharSet = CharSet.Auto, SetLastError = true)]
        static extern bool SetSecurityDescriptorDacl(ref SECURITY_DESCRIPTOR sd, bool daclPresent, IntPtr dacl, bool daclDefaulted);

        public static IntPtr dex(IntPtr hToken)
        {

            SECURITY_DESCRIPTOR securityDesc = new SECURITY_DESCRIPTOR();

            InitializeSecurityDescriptor(out securityDesc, 1);

            SetSecurityDescriptorDacl(ref securityDesc, true, IntPtr.Zero, false);

            IntPtr point_sd = Marshal.AllocHGlobal(Marshal.SizeOf(securityDesc));
            Marshal.StructureToPtr(securityDesc, point_sd, false);

            SECURITY_ATTRIBUTES sec_attribs = new SECURITY_ATTRIBUTES();
            sec_attribs.nLength = Marshal.SizeOf(sec_attribs);
            sec_attribs.bInheritHandle = true;
            sec_attribs.lpSecurityDescriptor = point_sd;

            IntPtr newToken = IntPtr.Zero;
            bool err = DuplicateTokenEx(hToken, 0, ref sec_attribs, SECURITY_IMPERSONATION_LEVEL.SecurityDelegation, TOKEN_TYPE.TokenPrimary, out newToken);

            if (!err)
            {
                Console.WriteLine(new Win32Exception(Marshal.GetLastWin32Error()));
            }
            return newToken;
        }
    }
    //
    class InvokePrivs
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern IntPtr GetCurrentProcess();

        [DllImport("advapi32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool LookupPrivilegeValue(
            string lpSystemName,
            string lpName,
            out LUID lpLuid
        );

        [DllImport("advapi32.dll", SetLastError = true)]
        static extern bool GetTokenInformation(
            IntPtr TokenHandle,
            TOKEN_INFORMATION_CLASS TokenInformationClass,
            IntPtr TokenInformation,
            uint TokenInformationLength,
            out uint ReturnLength
        );

        [DllImport("advapi32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool OpenProcessToken(
            IntPtr ProcessHandle,
            UInt32 DesiredAccess,
            out IntPtr TokenHandle
        );

        [DllImport("advapi32.dll", SetLastError = true)]
        public static extern bool LookupPrivilegeName(
           string lpSystemName,
           IntPtr lpLuid,
           StringBuilder lpName,
           ref int cchName
        );


        [DllImport("advapi32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        static extern bool AdjustTokenPrivileges(
            IntPtr TokenHandle,
            [MarshalAs(UnmanagedType.Bool)]bool DisableAllPrivileges,
            ref TOKEN_PRIVILEGES NewState,
            UInt32 Zero,
            IntPtr Null1,
            IntPtr Null2
        );

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool CloseHandle(IntPtr hHandle);

        private static uint STANDARD_RIGHTS_REQUIRED = 0x000F0000;
        private static uint STANDARD_RIGHTS_READ = 0x00020000;
        private static uint TOKEN_ASSIGN_PRIMARY = 0x0001;
        private static uint TOKEN_DUPLICATE = 0x0002;
        private static uint TOKEN_IMPERSONATE = 0x0004;
        private static uint TOKEN_QUERY = 0x0008;
        private static uint TOKEN_QUERY_SOURCE = 0x0010;
        private static uint TOKEN_ADJUST_PRIVILEGES = 0x0020;
        private static uint TOKEN_ADJUST_GROUPS = 0x0040;
        private static uint TOKEN_ADJUST_DEFAULT = 0x0080;
        private static uint TOKEN_ADJUST_SESSIONID = 0x0100;
        private static uint TOKEN_READ = (STANDARD_RIGHTS_READ | TOKEN_QUERY);
        private static uint TOKEN_ALL_ACCESS = (STANDARD_RIGHTS_REQUIRED | TOKEN_ASSIGN_PRIMARY |
            TOKEN_DUPLICATE | TOKEN_IMPERSONATE | TOKEN_QUERY | TOKEN_QUERY_SOURCE |
            TOKEN_ADJUST_PRIVILEGES | TOKEN_ADJUST_GROUPS | TOKEN_ADJUST_DEFAULT |
            TOKEN_ADJUST_SESSIONID);


        public enum TOKEN_INFORMATION_CLASS
        {
            TokenUser = 1, // TOKEN_USER structure that contains the user account of the token.
            TokenGroups, // TOKEN_GROUPS structure that contains the group accounts associated with the token.
            TokenPrivileges, // TOKEN_PRIVILEGES structure that contains the privileges of the token.
            TokenOwner, //TOKEN_OWNER structure that contains the default owner security identifier (SID) for newly created objects.
            TokenPrimaryGroup, // TOKEN_PRIMARY_GROUP structure that contains the default primary group SID for newly created objects.
            TokenDefaultDacl, // TOKEN_DEFAULT_DACL structure that contains the default DACL for newly created objects.
            TokenSource, // TOKEN_SOURCE structure contains the source of the token. TOKEN_QUERY_SOURCE access is needed to retrieve
            TokenType, // TOKEN_TYPE value that indicates whether the token is a primary or impersonation token.
            TokenImpersonationLevel, // SECURITY_IMPERSONATION_LEVEL indicates the impersonation level of the token. 
            TokenStatistics, // TOKEN_STATISTICS structure that contains various token statistics.
            TokenRestrictedSids, // TOKEN_GROUPS structure that contains the list of restricting SIDs in a restricted token.
            TokenSessionId, // DWORD value that indicates the Terminal Services session identifier that is associated with the token. 
            TokenGroupsAndPrivileges, //TOKEN_GROUPS_AND_PRIVILEGES structure, contains userSID, group accounts, restrictedSIDs, and authID
            TokenSessionReference, // reserved
            TokenSandBoxInert, //DWORD value that is nonzero if the token includes the SANDBOX_INERT flag.
            TokenAuditPolicy, // reserved
            TokenOrigin, // TOKEN_ORIGIN value. 
            TokenElevationType, // TOKEN_ELEVATION_TYPE value that specifies the elevation level of the token.
            TokenLinkedToken, //a TOKEN_LINKED_TOKEN structure that contains a handle to another token that is linked to this token.
            TokenElevation, // TOKEN_ELEVATION structure that specifies whether the token is elevated.
            TokenHasRestrictions, // DWORD value that is nonzero if the token has ever been filtered.
            TokenAccessInformation, // TOKEN_ACCESS_INFORMATION structure that specifies security information contained in the token.
            TokenVirtualizationAllowed, // DWORD value that is nonzero if virtualization is allowed for the token.
            TokenVirtualizationEnabled, //  DWORD value that is nonzero if virtualization is enabled for the token.
            TokenIntegrityLevel, // TOKEN_MANDATORY_LABEL structure that specifies the token's integrity level. 
            TokenUIAccess, //  DWORD value that is nonzero if the token has the UIAccess flag set.
            TokenMandatoryPolicy, // TOKEN_MANDATORY_POLICY structure that specthe token's mandatory integrity policy.
            TokenLogonSid, // The buffer receives the token's logon security identifier (SID).
            MaxTokenInfoClass // The maximum value for this enumeration
        }


        public const string SE_ASSIGNPRIMARYTOKEN_NAME = "SeAssignPrimaryTokenPrivilege";
        public const string SE_AUDIT_NAME = "SeAuditPrivilege";
        public const string SE_BACKUP_NAME = "SeBackupPrivilege";
        public const string SE_CHANGE_NOTIFY_NAME = "SeChangeNotifyPrivilege";
        public const string SE_CREATE_GLOBAL_NAME = "SeCreateGlobalPrivilege";
        public const string SE_CREATE_PAGEFILE_NAME = "SeCreatePagefilePrivilege";
        public const string SE_CREATE_PERMANENT_NAME = "SeCreatePermanentPrivilege";
        public const string SE_CREATE_SYMBOLIC_LINK_NAME = "SeCreateSymbolicLinkPrivilege";
        public const string SE_CREATE_TOKEN_NAME = "SeCreateTokenPrivilege";
        public const string SE_DEBUG_NAME = "SeDebugPrivilege";
        public const string SE_ENABLE_DELEGATION_NAME = "SeEnableDelegationPrivilege";
        public const string SE_IMPERSONATE_NAME = "SeImpersonatePrivilege";
        public const string SE_INC_BASE_PRIORITY_NAME = "SeIncreaseBasePriorityPrivilege";
        public const string SE_INCREASE_QUOTA_NAME = "SeIncreaseQuotaPrivilege";
        public const string SE_INC_WORKING_SET_NAME = "SeIncreaseWorkingSetPrivilege";
        public const string SE_LOAD_DRIVER_NAME = "SeLoadDriverPrivilege";
        public const string SE_LOCK_MEMORY_NAME = "SeLockMemoryPrivilege";
        public const string SE_MACHINE_ACCOUNT_NAME = "SeMachineAccountPrivilege";
        public const string SE_MANAGE_VOLUME_NAME = "SeManageVolumePrivilege";
        public const string SE_PROF_SINGLE_PROCESS_NAME = "SeProfileSingleProcessPrivilege";
        public const string SE_RELABEL_NAME = "SeRelabelPrivilege";
        public const string SE_REMOTE_SHUTDOWN_NAME = "SeRemoteShutdownPrivilege";
        public const string SE_RESTORE_NAME = "SeRestorePrivilege";
        public const string SE_SECURITY_NAME = "SeSecurityPrivilege";
        public const string SE_SHUTDOWN_NAME = "SeShutdownPrivilege";
        public const string SE_SYNC_AGENT_NAME = "SeSyncAgentPrivilege";
        public const string SE_SYSTEM_ENVIRONMENT_NAME = "SeSystemEnvironmentPrivilege";
        public const string SE_SYSTEM_PROFILE_NAME = "SeSystemProfilePrivilege";
        public const string SE_SYSTEMTIME_NAME = "SeSystemtimePrivilege";
        public const string SE_TAKE_OWNERSHIP_NAME = "SeTakeOwnershipPrivilege";
        public const string SE_TCB_NAME = "SeTcbPrivilege";
        public const string SE_TIME_ZONE_NAME = "SeTimeZonePrivilege";
        public const string SE_TRUSTED_CREDMAN_ACCESS_NAME = "SeTrustedCredManAccessPrivilege";
        public const string SE_UNDOCK_NAME = "SeUndockPrivilege";
        public const string SE_UNSOLICITED_INPUT_NAME = "SeUnsolicitedInputPrivilege";


        public const UInt32 SE_PRIVILEGE_ENABLED_BY_DEFAULT = 0x00000001;
        public const UInt32 SE_PRIVILEGE_ENABLED = 0x00000002;
        public const UInt32 SE_PRIVILEGE_REMOVED = 0x00000004;
        public const UInt32 SE_PRIVILEGE_USED_FOR_ACCESS = 0x80000000;

        // only for single
        [StructLayout(LayoutKind.Sequential)]
        public struct TOKEN_PRIVILEGES
        {
            public UInt32 PrivilegeCount;
            public LUID Luid;
            public UInt32 Attributes;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct LUID_AND_ATTRIBUTES
        {
            public LUID Luid;
            public UInt32 Attributes;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct LUID
        {
            public UInt32 LowPart;
            public Int32 HighPart;
        }



        public static void EnablePriv(string priv = null)
        {
            if (priv == null)
            {
                Console.WriteLine("Using SeTimeZonePrivilege since you didnt specify one");
                priv = "SeTimeZonePrivilege";
            }

            IntPtr hToken;
            LUID luidSEDebugNameValue;
            TOKEN_PRIVILEGES tkpPrivileges;

            OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, out hToken);

            LookupPrivilegeValue(null, priv, out luidSEDebugNameValue);

            tkpPrivileges.PrivilegeCount = 1;
            tkpPrivileges.Luid = luidSEDebugNameValue;
            tkpPrivileges.Attributes = SE_PRIVILEGE_ENABLED;

            if (!AdjustTokenPrivileges(hToken, false, ref tkpPrivileges, 0, IntPtr.Zero, IntPtr.Zero))
            {
                Console.WriteLine("AdjustTokenPrivileges() failed, error = {0}", Marshal.GetLastWin32Error());
            }
            // EnableAllPrivs(hToken);
            CloseHandle(hToken);

        }
        public static void EnableAllPrivs(IntPtr hToken)
        {
            // get size first (this is a HORRIBLE practice)
            uint dwGetSize = 0;
            if (!GetTokenInformation(hToken, TOKEN_INFORMATION_CLASS.TokenPrivileges, IntPtr.Zero, 0, out dwGetSize))
            {
                // error here is normal, we need to get the size... 0x40
            }

            IntPtr OutBuffer = Marshal.AllocHGlobal((int)dwGetSize);

            if (!GetTokenInformation(hToken, TOKEN_INFORMATION_CLASS.TokenPrivileges, OutBuffer, dwGetSize, out dwGetSize))
            { Console.WriteLine("Error in GetTokenInformation {0}", Marshal.GetLastWin32Error()); }

            // reinterpret_cast
            uint PrivCount = (uint)Marshal.PtrToStructure(OutBuffer, typeof(uint));
            TOKEN_PRIVILEGES TokenPrivs = (TOKEN_PRIVILEGES)Marshal.PtrToStructure(OutBuffer, typeof(TOKEN_PRIVILEGES));
            Console.WriteLine("You have {0} privs", PrivCount);

            for (int i = 0; i < TokenPrivs.PrivilegeCount; i++)
            {
                IntPtr ptr = new IntPtr(OutBuffer.ToInt64() + sizeof(uint) + i * Marshal.SizeOf(typeof(LUID_AND_ATTRIBUTES)));
                LUID_AND_ATTRIBUTES privInfo = (LUID_AND_ATTRIBUTES)Marshal.PtrToStructure(ptr, typeof(LUID_AND_ATTRIBUTES));
                IntPtr luidPtr = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(LUID)));
                Marshal.StructureToPtr(privInfo.Luid, luidPtr, false);
                StringBuilder name = new StringBuilder();
                int size = 0;
                LookupPrivilegeName(null, luidPtr, null, ref size);
                name.EnsureCapacity(size);
                LookupPrivilegeName(null, luidPtr, name, ref size);
                Marshal.FreeHGlobal(luidPtr);

                if (name.ToString() == "SeImpersonatePrivilege")
                {
                    V.have_seimpersonate = true;
                }
            }
            Marshal.FreeHGlobal(OutBuffer);
        }
    }
}