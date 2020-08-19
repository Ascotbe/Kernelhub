using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace NHttp
{
    internal static class LogManager
    {
        private static readonly MethodInfo _getLoggerMethod;
        private static readonly MethodInfo _debugMethod;
        private static readonly MethodInfo _infoMethod;
        private static readonly MethodInfo _warnMethod;
        private static readonly MethodInfo _errorMethod;
        private static readonly MethodInfo _debugEnabled;
        private static readonly MethodInfo _infoEnabled;
        private static readonly MethodInfo _warnEnabled;
        private static readonly MethodInfo _errorEnabled;

        static LogManager()
        {
            Assembly assembly = null;
            try
            {
                assembly = Assembly.Load("Common.Logging");
            }
            catch (System.IO.FileNotFoundException)
            {
                // Ignore missing assembly, logging will not be enabled.
            }

            if (assembly == null)
                return;

            var logManagerType = assembly.GetType("Common.Logging.LogManager");
            _getLoggerMethod = logManagerType.GetMethod("GetLogger", BindingFlags.Static | BindingFlags.Public, null, new[] { typeof(Type) }, null);
            var logType = _getLoggerMethod.ReturnType;
            _debugMethod = GetMethod(logType, "Debug");
            _debugEnabled = GetEnabledProperty(logType, "Debug");
            _infoMethod = GetMethod(logType, "Info");
            _infoEnabled = GetEnabledProperty(logType, "Debug");
            _warnMethod = GetMethod(logType, "Warn");
            _warnEnabled = GetEnabledProperty(logType, "Debug");
            _errorMethod = GetMethod(logType, "Error");
            _errorEnabled = GetEnabledProperty(logType, "Debug");
        }

        private static MethodInfo GetEnabledProperty(Type logType, string name)
        {
            return logType.GetProperty("Is" + name + "Enabled").GetGetMethod();
        }

        private static MethodInfo GetMethod(Type logType, string name)
        {
            return logType.GetMethod(name, BindingFlags.Instance | BindingFlags.Public, null, new[] { typeof(string) }, null);
        }

        public static ILog GetLogger(Type type)
        {
            if (_getLoggerMethod == null)
                return new NullLog();

            return new CommonLog(_getLoggerMethod.Invoke(null, new object[] { type }));
        }

        private class NullLog : ILog
        {
            public void Debug(string message)
            {
            }

            public void Debug(string message, params object[] args)
            {
            }

            public void Info(string message)
            {
            }

            public void Info(string message, params object[] args)
            {
            }

            public void Warn(string message)
            {
            }

            public void Warn(string message, params object[] args)
            {
            }

            public void Error(string message)
            {
            }

            public void Error(string message, params object[] args)
            {
            }
        }

        private class CommonLog : ILog
        {
            private readonly object _log;
            private readonly bool _debug;
            private readonly bool _info;
            private readonly bool _warn;
            private readonly bool _error;

            public CommonLog(object log)
            {
                _log = log;
                _debug = (bool)_debugEnabled.Invoke(log, null);
                _info = (bool)_infoEnabled.Invoke(log, null);
                _warn = (bool)_warnEnabled.Invoke(log, null);
                _error = (bool)_errorEnabled.Invoke(log, null);
            }

            public void Debug(string message)
            {
                if (!_debug)
                    return;
                _debugMethod.Invoke(_log, new object[] { message });
            }

            public void Debug(string message, params object[] args)
            {
                if (!_debug)
                    return;
                if (args != null && args.Length > 0)
                    message = String.Format(message, args);
                Debug(message);
            }

            public void Info(string message)
            {
                if (!_info)
                    return;
                _infoMethod.Invoke(_log, new object[] { message });
            }

            public void Info(string message, params object[] args)
            {
                if (!_info)
                    return;
                if (args != null && args.Length > 0)
                    message = String.Format(message, args);
                Info(message);
            }

            public void Warn(string message)
            {
                if (!_warn)
                    return;
                _warnMethod.Invoke(_log, new object[] { message });
            }

            public void Warn(string message, params object[] args)
            {
                if (!_warn)
                    return;
                if (args != null && args.Length > 0)
                    message = String.Format(message, args);
                Warn(message);
            }

            public void Error(string message)
            {
                if (!_error)
                    return;
                _errorMethod.Invoke(_log, new object[] { message });
            }

            public void Error(string message, params object[] args)
            {
                if (!_error)
                    return;
                if (args != null && args.Length > 0)
                    message = String.Format(message, args);
                Error(message);
            }
        }
    }
}
