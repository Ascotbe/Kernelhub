using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;


namespace Potato
{
    // Taken from here http://stackoverflow.com/questions/29474022/call-advapi32-dll-native-eventwrite-function-from-c#
    class EventTriggers
    {
        [StructLayout(LayoutKind.Explicit, Size=16)]
        public class EVENT_DESCRIPTOR
        {
            [FieldOffset(0)]ushort Id = 1;
            [FieldOffset(2)]byte Version = 0;
            [FieldOffset(3)]byte Channel = 0;
            [FieldOffset(4)]byte Level = 4;
            [FieldOffset(5)]byte Opcode = 0;
            [FieldOffset(6)]ushort Task = 0;
            [FieldOffset(8)]long Keyword = 0;
        }

        [StructLayout(LayoutKind.Explicit, Size = 16)]
        public struct EventData
        {
            [FieldOffset(0)]
            internal UInt64 DataPointer;
            [FieldOffset(8)]
            internal uint Size;
            [FieldOffset(12)]
            internal int Reserved;
        }



        [DllImport("Advapi32.dll", SetLastError = true)]
        public static extern uint EventRegister(
            ref Guid guid, 
            [Optional] IntPtr EnableCallback, 
            [Optional] IntPtr CallbackContext, 
            [In][Out] ref long RegHandle
        );

        [DllImport("Advapi32.dll", SetLastError = true)]
        public static extern uint EventWrite(
            long RegHandle, 
            ref EVENT_DESCRIPTOR EventDescriptor, 
            uint UserDataCount, 
            IntPtr UserData
        );

        [DllImport("Advapi32.dll", SetLastError = true)]
        public static extern uint EventUnregister(long RegHandle);

        public static void start_service(string trigger)
        {
            Guid webCleintTrigger = new Guid(0x22B6D684, 0xFA63, 0x4578, 0x87, 0xC9, 0xEF, 0xFC, 0xBE, 0x66, 0x43, 0xC7);
            if (trigger.ToLower().Equals("true"))
            {
                Console.WriteLine("Attempting to start WebClient via ServiceTrigger");
            }
            else
            {
                Console.WriteLine("Attempting to start {0} via ServiceTrigger", trigger);
                webCleintTrigger = new Guid(trigger);

            }

            long handle = 0;
            uint output = EventRegister(ref webCleintTrigger, IntPtr.Zero, IntPtr.Zero, ref handle);
            bool success = false;

            if (output == 0)
            {
                //Create event descriptor
                EVENT_DESCRIPTOR desc = new EVENT_DESCRIPTOR();

                //Write the event
                uint writeOutput = EventWrite(handle, ref desc, 0, IntPtr.Zero);
                success = writeOutput == 0;
                EventUnregister(handle);
                Console.WriteLine("Service should be started, verify with \"sc.exe query servicename\"");
            }
        }
    }
}