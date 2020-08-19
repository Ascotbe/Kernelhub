using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SharpCifs.Dcerpc;
using SharpCifs.Dcerpc.Ndr;

namespace SharpCifs.Dcerpc.Msrpc
{

    public class svcctl {

        public static String getSyntax() {
            return "367abb81-9844-35f1-ad32-98f038001003:2.0";
        }

        public static int SC_MANAGER_ALL_ACCESS = 0xF003F;
        public static int SC_MANAGER_CREATE_SERVICE = 0x0002;
        public static int SC_MANAGER_CONNECT = 0x0001;
        public static int SC_MANAGER_ENUMERATE_SERVICE = 0x0004;
        public static int SC_MANAGER_LOCK = 0x0008;
        public static int SC_MANAGER_MODIFY_BOOT_CONFIG = 0x0020;
        public static int SC_MANAGER_QUERY_LOCK_STATUS = 0x0010;
        public static int SC_TYPE_SERVICE_KERNEL_DRIVER = 0x00000001;
        public static int SC_TYPE_SERVICE_FILE_SYSTEM_DRIVER = 0x00000002;
        public static int SC_TYPE_SERVICE_ADAPTER = 0x00000004;
        public static int SC_TYPE_SERVICE_RECOGNIZER_DRIVER = 0x00000008;
        public static int SC_TYPE_SERVICE_WIN32_OWN_PROCESS = 0x00000010;
        public static int SC_TYPE_SERVICE_WIN32_SHARE_PROCESS = 0x00000020;
        public static int SC_TYPE_SERVICE_INTERACTIVE_PROCESS = 0x00000100;
        public static int SC_START_TYPE_SERVICE_SYSTEM_START = 0x00000001;
        public static int SC_START_TYPE_SERVICE_AUTO_START = 0x00000002;
        public static int SC_START_TYPE_SERVICE_DEMAND_START = 0x00000003;
        public static int SC_START_TYPE_SERVICE_DISABLED = 0x00000004;
        public static int SC_START_TYPE_SERVICE_BOOT_START = 0x00000000;
        public static int SC_SERVICE_ERROR_IGNORE = 0x00000000;
        public static int SC_SERVICE_ERROR_NORMAL = 0x00000001;
        public static int SC_SERVICE_ERROR_SEVERE = 0x00000002;
        public static int SC_SERVICE_ERROR_CRITICAL = 0x00000003;
        public static int SC_STATE_SERVICE_CONTINUE_PENDING = 0x00000005;
        public static int SC_STATE_SERVICE_PAUSE_PENDING = 0x00000006;
        public static int SC_STATE_SERVICE_PAUSED = 0x00000007;
        public static int SC_STATE_SERVICE_RUNNING = 0x00000004;
        public static int SC_STATE_SERVICE_START_PENDING = 0x00000002;
        public static int SC_STATE_SERVICE_STOP_PENDING = 0x00000003;
        public static int SC_STATE_SERVICE_STOPPED = 0x00000001;
        public static int SERVICE_CONTROL_STOP = 1;
        public static int SERVICE_CONTROL_PAUSE = 2;
        public static int SERVICE_CONTROL_CONTINUE = 3;
        public static int SERVICE_CONTROL_INTERROGATE = 4;

        public class service_status : NdrObject {

            public int service_type;
            public int current_state;
            public int controls_accepted;
            public int win32_exit_code;
            public int service_specific_exit_code;
            public int check_point;
            public int wait_hint;

            public override  void Encode(NdrBuffer _dst)  {
                _dst.Align(4);
                _dst.Enc_ndr_long(service_type);
                _dst.Enc_ndr_long(current_state);
                _dst.Enc_ndr_long(controls_accepted);
                _dst.Enc_ndr_long(win32_exit_code);
                _dst.Enc_ndr_long(service_specific_exit_code);
                _dst.Enc_ndr_long(check_point);
                _dst.Enc_ndr_long(wait_hint);

            }
            public override  void Decode(NdrBuffer _src)  {
                _src.Align(4);
                service_type = (int)_src.Dec_ndr_long();
                current_state = (int)_src.Dec_ndr_long();
                controls_accepted = (int)_src.Dec_ndr_long();
                win32_exit_code = (int)_src.Dec_ndr_long();
                service_specific_exit_code = (int)_src.Dec_ndr_long();
                check_point = (int)_src.Dec_ndr_long();
                wait_hint = (int)_src.Dec_ndr_long();

            }
        }
        public class CloseServiceHandle : DcerpcMessage {

            public override int GetOpnum() { return 0x00; }

            public int retval;
            public SharpCifs.Dcerpc.Rpc.PolicyHandle handle;

            public CloseServiceHandle(SharpCifs.Dcerpc.Rpc.PolicyHandle handle) {
                this.handle = handle;
                this.Ptype = 0;
            }

            public override void Encode_in(NdrBuffer _dst) {
                handle.Encode(_dst);
            }
            public override  void Decode_out(NdrBuffer _src)  {
                handle.Decode(_src);
                retval = (int)_src.Dec_ndr_long();
            }
        }
        public class ControlService : DcerpcMessage {

            public override int GetOpnum() { return 0x01; }

            public int retval;
            public SharpCifs.Dcerpc.Rpc.PolicyHandle service_handle;
            public int control;
            public service_status status;

            public ControlService(SharpCifs.Dcerpc.Rpc.PolicyHandle service_handle, int control, service_status status) {
                this.service_handle = service_handle;
                this.control = control;
                this.status = status;
                this.Ptype = 0;
            }

            public override void Encode_in(NdrBuffer _dst)  {
                service_handle.Encode(_dst);
                _dst.Enc_ndr_long(control);
            }
            public override void Decode_out(NdrBuffer _src) {
                status.Decode(_src);
                retval = (int)_src.Dec_ndr_long();
            }
        }
        public class DeleteService : DcerpcMessage {

            public override int GetOpnum() { return 0x02; }

            public int retval;
            public Rpc.PolicyHandle service_handle;

            public DeleteService(Rpc.PolicyHandle service_handle) {
                this.service_handle = service_handle;
                this.Ptype = 0;
            }

            public override void Encode_in(NdrBuffer _dst) {
                service_handle.Encode(_dst);
            }
            public override void Decode_out(NdrBuffer _src)  {
                retval = (int)_src.Dec_ndr_long();
            }
        }
        public class QueryServiceStatus : DcerpcMessage {

            public override int GetOpnum() { return 0x06; }

            public int retval;
            public Rpc.PolicyHandle service_handle;
            public service_status status;

            public QueryServiceStatus(Rpc.PolicyHandle service_handle, service_status status) {
                this.service_handle = service_handle;
                this.status = status;
                this.Ptype = 0;
            }

            public override void Encode_in(NdrBuffer _dst)  {
                service_handle.Encode(_dst);
            }
            public override void Decode_out(NdrBuffer _src)  {
                status.Decode(_src);
                retval = (int)_src.Dec_ndr_long();
            }
        }
        public class SetServiceStatus : DcerpcMessage {

            public override int GetOpnum() { return 0x07; }

            public int retval;
            public Rpc.PolicyHandle service_handle;
            public service_status status;

            public SetServiceStatus(Rpc.PolicyHandle service_handle, service_status status) {
                this.service_handle = service_handle;
                this.status = status;
                this.Ptype = 0;
            }

            public override void Encode_in(NdrBuffer _dst)  {
                service_handle.Encode(_dst);
                status.Encode(_dst);
            }
            public override void Decode_out(NdrBuffer _src)  {
                retval = (int)_src.Dec_ndr_long();
            }
        }
        public class CreateServiceW : DcerpcMessage {

            public override int GetOpnum() { return 0x0c; }

            public int retval;
            public Rpc.PolicyHandle scmanager_handle;
            public String service_name;
            public String display_name;
            public int access_mask;
            public int service_type;
            public int start_type;
            public int error_control;
            public String binary_path_name;
            public String load_order_group;
            public NdrLong lpdwTagId;
            public byte[] lpDependencies;
            public int dependency_size;
            public String lpServiceStartName;
            public byte[] password;
            public int password_size;
            public Rpc.PolicyHandle service_handle;

            public CreateServiceW(Rpc.PolicyHandle scmanager_handle,
                        String service_name,
                        String display_name,
                        int access_mask,
                        int service_type,
                        int start_type,
                        int error_control,
                        String binary_path_name,
                        String load_order_group,
                        NdrLong lpdwTagId,
                        byte[] lpDependencies,
                        int dependency_size,
                        String lpServiceStartName,
                        byte[] password,
                        int password_size,
                        Rpc.PolicyHandle service_handle) {
                this.scmanager_handle = scmanager_handle;
                this.service_name = service_name;
                this.display_name = display_name;
                this.access_mask = access_mask;
                this.service_type = service_type;
                this.start_type = start_type;
                this.error_control = error_control;
                this.binary_path_name = binary_path_name;
                this.load_order_group = load_order_group;
                this.lpdwTagId = lpdwTagId;
                this.lpDependencies = lpDependencies;
                this.dependency_size = dependency_size;
                this.lpServiceStartName = lpServiceStartName;
                this.password = password;
                this.password_size = password_size;
                this.service_handle = service_handle;
                this.Ptype = 0;
            }

            public override void Encode_in(NdrBuffer _dst)   {
                scmanager_handle.Encode(_dst);
                _dst.Enc_ndr_string(service_name);
                _dst.Enc_ndr_referent(display_name, 1);
                if (display_name != null) {
                    _dst.Enc_ndr_string(display_name);

                }
                _dst.Enc_ndr_long(access_mask);
                _dst.Enc_ndr_long(service_type);
                _dst.Enc_ndr_long(start_type);
                _dst.Enc_ndr_long(error_control);
                _dst.Enc_ndr_string(binary_path_name);
                _dst.Enc_ndr_referent(load_order_group, 1);
                if (load_order_group != null) {
                    _dst.Enc_ndr_string(load_order_group);

                }
                _dst.Enc_ndr_referent(lpdwTagId, 1);
                if (lpdwTagId != null) {
                    lpdwTagId.Encode(_dst);

                }
                _dst.Enc_ndr_referent(lpDependencies, 1);
                if (lpDependencies != null) {
                    int _lpDependenciess = dependency_size;
                    _dst.Enc_ndr_long(_lpDependenciess);
                    int _lpDependenciesi = _dst.Index;
                    _dst.Advance(1 * _lpDependenciess);

                    _dst = _dst.Derive(_lpDependenciesi);
                    for (int _i = 0; _i < _lpDependenciess; _i++) {
                        _dst.Enc_ndr_small(lpDependencies[_i]);
                    }
                }
                _dst.Enc_ndr_long(dependency_size);
                _dst.Enc_ndr_referent(lpServiceStartName, 1);
                if (lpServiceStartName != null) {
                    _dst.Enc_ndr_string(lpServiceStartName);

                }
                _dst.Enc_ndr_referent(password, 1);
                if (password != null) {
                    int _passwords = password_size;
                    _dst.Enc_ndr_long(_passwords);
                    int _passwordi = _dst.Index;
                    _dst.Advance(1 * _passwords);

                    _dst = _dst.Derive(_passwordi);
                    for (int _i = 0; _i < _passwords; _i++) {
                        _dst.Enc_ndr_small(password[_i]);
                    }
                }
                _dst.Enc_ndr_long(password_size);
            }
            public override void Decode_out(NdrBuffer _src)  {
                int _lpdwTagIdp = _src.Dec_ndr_long();
                if (_lpdwTagIdp != 0) {
                    lpdwTagId.Decode(_src);

                }
                service_handle.Decode(_src);
                retval = (int)_src.Dec_ndr_long();
            }
        }
        public class OpenSCManager : DcerpcMessage {

            public override int GetOpnum() { return 0x0f; }

            public int retval;
            public String machine_name;
            public String database_name;
            public int access_mask;
            public Rpc.PolicyHandle handle;

            public OpenSCManager(String machine_name,
                        String database_name,
                        int access_mask,
                        Rpc.PolicyHandle handle) {
                this.machine_name = machine_name;
                this.database_name = database_name;
                this.access_mask = access_mask;
                this.handle = handle;
                this.Ptype = 0;
            }

            public override void Encode_in(NdrBuffer _dst)  {
                _dst.Enc_ndr_referent(machine_name, 1);
                if (machine_name != null) {
                    _dst.Enc_ndr_string(machine_name);

                }
                _dst.Enc_ndr_referent(database_name, 1);
                if (database_name != null) {
                    _dst.Enc_ndr_string(database_name);

                }
                _dst.Enc_ndr_long(access_mask);
            }
            public override void Decode_out(NdrBuffer _src)  {
                handle.Decode(_src);
                retval = (int)_src.Dec_ndr_long();
            }
        }
        public class OpenService : DcerpcMessage {

            public override int GetOpnum() { return 0x10; }

            public int retval;
            public Rpc.PolicyHandle scmanager_handle;
            public String service_name;
            public int access_mask;
            public Rpc.PolicyHandle handle;

            public OpenService(Rpc.PolicyHandle scmanager_handle,
                        String service_name,
                        int access_mask,
                        Rpc.PolicyHandle handle) {
                this.scmanager_handle = scmanager_handle;
                this.service_name = service_name;
                this.access_mask = access_mask;
                this.handle = handle;
                this.Ptype = 0;
            }

            public override void Encode_in(NdrBuffer _dst)  {
                scmanager_handle.Encode(_dst);
                _dst.Enc_ndr_string(service_name);
                _dst.Enc_ndr_long(access_mask);
            }
            public override void Decode_out(NdrBuffer _src)  {
                handle.Decode(_src);
                retval = (int)_src.Dec_ndr_long();
            }
        }
        public class EnumServicesStatus : DcerpcMessage {

            public override int GetOpnum() { return 0x0e; }

            public int retval;
            public Rpc.PolicyHandle handle;
            public int type;
            public int state;
            public int buf_size;
            public byte[] service;
            public int bytes_needed;
            public int services_returned;
            public int resume_handle;

            public EnumServicesStatus(Rpc.PolicyHandle handle,
                        int type,
                        int state,
                        int buf_size,
                        byte[] service,
                        int bytes_needed,
                        int services_returned,
                        int resume_handle) {
                this.handle = handle;
                this.type = type;
                this.state = state;
                this.buf_size = buf_size;
                this.service = service;
                this.bytes_needed = bytes_needed;
                this.services_returned = services_returned;
                this.resume_handle = resume_handle;
                this.Ptype = 0;
            }

            public override void Encode_in(NdrBuffer _dst)  {
                handle.Encode(_dst);
                _dst.Enc_ndr_long(type);
                _dst.Enc_ndr_long(state);
                _dst.Enc_ndr_long(buf_size);
                _dst.Enc_ndr_long(resume_handle);
            }
            public override void Decode_out(NdrBuffer _src)  {
                int _services = _src.Dec_ndr_long();
                int _servicei = _src.Index;
                _src.Advance(1 * _services);
                if (service == null) {
                    if (_services < 0 || _services > 0xFFFF) throw new NdrException( NdrException.InvalidConformance );
                    service = new byte[_services];
                }
                _src = _src.Derive(_servicei);
                for (int _i = 0; _i < _services; _i++) {
                    service[_i] = (byte)_src.Dec_ndr_small();
                }
                bytes_needed = (int)_src.Dec_ndr_long();
                services_returned = (int)_src.Dec_ndr_long();
                resume_handle = (int)_src.Dec_ndr_long();
                retval = (int)_src.Dec_ndr_long();
            }
        }
        public class StartService : DcerpcMessage {

            public override int GetOpnum() { return 0x13; }

            public int retval;
            public Rpc.PolicyHandle handle;
            public int num_service_args;
            public String[] service_arg_vectors;

            public StartService(Rpc.PolicyHandle handle, int num_service_args, String[] service_arg_vectors) {
                this.handle = handle;
                this.num_service_args = num_service_args;
                this.service_arg_vectors = service_arg_vectors;
                this.Ptype = 0;
            }

            public override void Encode_in(NdrBuffer _dst)  {
                handle.Encode(_dst);
                _dst.Enc_ndr_long(num_service_args);
                _dst.Enc_ndr_referent(service_arg_vectors, 1);
                if (service_arg_vectors != null) {
                    int _service_arg_vectorss = num_service_args;
                    _dst.Enc_ndr_long(_service_arg_vectorss);
                    for (int _i = 0; _i < _service_arg_vectorss; _i++) {
                        _dst.Enc_ndr_referent(service_arg_vectors[_i], 1);
                    }

                    for (int _i = 0; _i < _service_arg_vectorss; _i++) {
                        if (service_arg_vectors[_i] != null) {
                            _dst.Enc_ndr_string(service_arg_vectors[_i]);

                        }
                    }
                }
            }
            public override void Decode_out(NdrBuffer _src) {
                retval = (int)_src.Dec_ndr_long();
            }
        }
    }

}
