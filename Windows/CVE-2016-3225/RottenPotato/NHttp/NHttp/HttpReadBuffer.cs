using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace NHttp
{
    internal class HttpReadBuffer
    {
        private readonly int _bufferSize;
        private StringBuilder _lineBuffer;
        private int _totalRead;
        private int _offset;
        private byte[] _buffer;
        private int _available;
        private bool _forceNewRead;

        public bool DataAvailable
        {
            get { return !_forceNewRead && _offset < _available; }
        }

        public HttpReadBuffer(int size)
        {
            _bufferSize = size;

            _buffer = new byte[size];
        }

        public string ReadLine()
        {
            if (_lineBuffer == null)
                _lineBuffer = new StringBuilder();

            while (_offset < _available)
            {
                int c = _buffer[_offset++];

                _totalRead++;

                if (c == '\n')
                {
                    string line = _lineBuffer.ToString();

                    _lineBuffer = new StringBuilder();

                    return line;
                }
                else if (c != '\r')
                {
                    _lineBuffer.Append((char)c);
                }
            }

            return null;
        }

        public void Reset()
        {
            _lineBuffer = null;
            _totalRead = 0;
        }

        public void CopyToStream(Stream stream, int maximum)
        {
            CopyToStream(stream, maximum, null);
        }

        public bool CopyToStream(Stream stream, int maximum, byte[] boundary)
        {
            int toRead = Math.Min(
                _available - _offset,
                maximum - _totalRead
            );

            bool atBoundary = false;
            bool partialBoundaryMatch = false;

            if (boundary != null)
            {
                int boundaryOffset = -1;

                for (int i = 0; i < toRead; i++)
                {
                    bool boundaryMatched = true;

                    for (int j = 0; j < boundary.Length; j++)
                    {
                        if (i + j >= toRead)
                        {
                            partialBoundaryMatch = true;
                            break;
                        }

                        if (_buffer[i + _offset + j] != boundary[j])
                        {
                            boundaryMatched = false;
                            break;
                        }
                    }

                    if (boundaryMatched)
                    {
                        boundaryOffset = i;
                        break;
                    }
                }

                if (boundaryOffset != -1)
                {
                    // If we have a boundary, we can read up until the boundary offset.

                    toRead = boundaryOffset;
                    atBoundary = !partialBoundaryMatch;

                    // When we have a partial boundary match, we need more data.

                    if (partialBoundaryMatch)
                        _forceNewRead = true;
                }
            }

            stream.Write(_buffer, _offset, toRead);

            _offset += toRead;
            _totalRead += toRead;

            // If we found the boundary, we also skip it.

            if (atBoundary)
            {
                _offset += boundary.Length;
                _totalRead += boundary.Length;
            }
            else if (boundary != null && maximum - _totalRead < boundary.Length)
            {
                throw new ProtocolException("Not enough data available for multipart boundary to match");
            }

            return atBoundary;
        }

        public bool? AtBoundary(byte[] boundary, int maximum)
        {
            if (boundary == null)
                throw new ArgumentNullException("boundary");

            if (maximum - _totalRead < boundary.Length)
                throw new ProtocolException("Not enough data available for multipart boundary to match");

            if (_available - _offset < boundary.Length)
                return null;

            for (int i = 0; i < boundary.Length; i++)
            {
                if (boundary[i] != _buffer[i + _offset])
                    return false;
            }

            _offset += boundary.Length;
            _totalRead += boundary.Length;

            return true;
        }

        public void BeginRead(Stream stream, AsyncCallback callback, object state)
        {
            // A new read was requested. Reset the flag.

            _forceNewRead = false;

            if (_offset == _available)
            {
                // If the offset is at the end, we can just reset the
                // positions.

                _offset = 0;
                _available = 0;
            }
            else if (_buffer.Length - _available < _bufferSize)
            {
                // If there is less than the initial buffer size room left,
                // we need to move some data.

                if (_buffer.Length - (_available - _offset) < _bufferSize)
                {
                    // If the available size is less than the initial buffer size,
                    // enlarge the buffer.

                    var buffer = new byte[_buffer.Length * 2];

                    // Copy the unprocessed bytes to the start of the new buffer.

                    Array.Copy(_buffer, _offset, buffer, 0, _available - _offset);

                    _buffer = buffer;
                }
                else
                {
                    // Else, just move the unprocessed bytes to the beginning.

                    Array.Copy(_buffer, _offset, _buffer, 0, _available - _offset);
                }

                // Reset the position and available to reflect the moved
                // bytes.

                _available -= _offset;
                _offset = 0;
            }

            // We don't use the whole buffer, only what we were assigned in
            // the beginning. This is to prevent the read buffer from
            // resizing too much.

            int bufferAvailable = Math.Min(_buffer.Length - _available, _bufferSize);

            stream.BeginRead(_buffer, _available, bufferAvailable, callback, state);
        }

        public void EndRead(Stream stream, IAsyncResult asyncResult)
        {
            int read = stream.EndRead(asyncResult);

            _available += read;
        }
    }
}
