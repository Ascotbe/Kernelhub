using System;
using System.Linq;

namespace Potato
{
    abstract class Spoofer
    {
        public static byte[] StringToByteArray(string hex)
        {
            return Enumerable.Range(0, hex.Length)
                .Where(x => x % 2 == 0)
                .Select(x => Convert.ToByte(hex.Substring(x, 2), 16))
                .ToArray();
        }
        public abstract void startSpoofing(String localIp, String spoof_host,String spoof_address, bool enableExhaust);
        public abstract void checkSpoof(String host,String address);
    }
}