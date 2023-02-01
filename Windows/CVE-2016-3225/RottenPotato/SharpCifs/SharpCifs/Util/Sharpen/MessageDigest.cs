using System;
using System.IO;
using System.Security.Cryptography;
using SharpCifs.Util.Sharpen;

namespace SharpCifs.Util.Sharpen
{
    public abstract class MessageDigest
	{
	    public void Digest (byte[] buffer, int o, int len)
		{
			byte[] d = Digest ();
			d.CopyTo (buffer, o);
		}

		public byte[] Digest (byte[] buffer)
		{
			Update (buffer);
			return Digest ();
		}

		public abstract byte[] Digest ();
		public abstract int GetDigestLength ();
		public static MessageDigest GetInstance (string algorithm)
		{
			switch (algorithm.ToLower ()) {
			case "sha-1":
				return new MessageDigest<SHA1Managed> ();
			case "md5":
				return new MessageDigest<MD5Managed> ();
			}
			throw new NotSupportedException (string.Format ("The requested algorithm \"{0}\" is not supported.", algorithm));
		}

		public abstract void Reset ();
		public abstract void Update (byte[] b);
		public abstract void Update (byte b);
		public abstract void Update (byte[] b, int offset, int len);
	}


	public class MessageDigest<TAlgorithm> : MessageDigest where TAlgorithm : HashAlgorithm, new()
	{
		private TAlgorithm _hash;
		private CryptoStream _stream;

		public MessageDigest ()
		{
			Init ();
		}

		public override byte[] Digest ()
		{
			_stream.FlushFinalBlock ();
			byte[] hash = _hash.Hash;
			Reset ();
			return hash;
		}

		public void Dispose ()
		{
			if (_stream != null) {
				_stream.Dispose ();
			}
			_stream = null;
		}

		public override int GetDigestLength ()
		{
			return (_hash.HashSize / 8);
		}

		private void Init ()
		{
			_hash = Activator.CreateInstance<TAlgorithm> ();
			_stream = new CryptoStream (Stream.Null, _hash, CryptoStreamMode.Write);
		}

		public override void Reset ()
		{
			Dispose ();
			Init ();
		}

		public override void Update (byte[] input)
		{
			_stream.Write (input, 0, input.Length);
		}

		public override void Update (byte input)
		{
			_stream.WriteByte (input);
		}

		public override void Update (byte[] input, int index, int count)
		{
			if (count < 0)
				Console.WriteLine ("Argh!");
			_stream.Write (input, index, count);
		}
	}
}
