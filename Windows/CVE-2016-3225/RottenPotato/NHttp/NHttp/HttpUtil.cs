using System;
using System.Collections.Generic;
using System.Globalization;
using System.Text;

namespace NHttp
{
    internal static class HttpUtil
    {
        private static readonly int _longestHtmlEntity;
        private static readonly Dictionary<int, string> _htmlEntitiesByNumber = new Dictionary<int, string>();

        // HTML entities taken from http://www.w3.org/TR/html4/sgml/entities.html.

        private static readonly Dictionary<string, int> _htmlEntitiesByEntity = new Dictionary<string, int>
        {
            { "Aacute", 193 }, { "aacute", 225 }, { "Acirc", 194 }, { "acirc", 226 }, { "acute", 180 }, { "AElig", 198 },
            { "aelig", 230 }, { "Agrave", 192 }, { "agrave", 224 }, { "alefsym", 8501 }, { "Alpha", 913 }, { "alpha", 945 },
            { "amp", 38 }, { "and", 8743 }, { "ang", 8736 }, { "Aring", 197 }, { "aring", 229 }, { "asymp", 8776 },
            { "Atilde", 195 }, { "atilde", 227 }, { "Auml", 196 }, { "auml", 228 }, { "bdquo", 8222 }, { "Beta", 914 },
            { "beta", 946 }, { "brvbar", 166 }, { "bull", 8226 }, { "cap", 8745 }, { "Ccedil", 199 }, { "ccedil", 231 },
            { "cedil", 184 }, { "cent", 162 }, { "Chi", 935 }, { "chi", 967 }, { "circ", 710 }, { "clubs", 9827 },
            { "cong", 8773 }, { "copy", 169 }, { "crarr", 8629 }, { "cup", 8746 }, { "curren", 164 }, { "dagger", 8224 },
            { "Dagger", 8225 }, { "darr", 8595 }, { "dArr", 8659 }, { "deg", 176 }, { "Delta", 916 }, { "delta", 948 },
            { "diams", 9830 }, { "divide", 247 }, { "Eacute", 201 }, { "eacute", 233 }, { "Ecirc", 202 }, { "ecirc", 234 },
            { "Egrave", 200 }, { "egrave", 232 }, { "empty", 8709 }, { "emsp", 8195 }, { "ensp", 8194 }, { "Epsilon", 917 },
            { "epsilon", 949 }, { "equiv", 8801 }, { "Eta", 919 }, { "eta", 951 }, { "ETH", 208 }, { "eth", 240 },
            { "Euml", 203 }, { "euml", 235 }, { "euro", 8364 }, { "exist", 8707 }, { "fnof", 402 }, { "forall", 8704 },
            { "frac12", 189 }, { "frac14", 188 }, { "frac34", 190 }, { "frasl", 8260 }, { "Gamma", 915 }, { "gamma", 947 },
            { "ge", 8805 }, { "gt", 62 }, { "harr", 8596 }, { "hArr", 8660 }, { "hearts", 9829 }, { "hellip", 8230 },
            { "Iacute", 205 }, { "iacute", 237 }, { "Icirc", 206 }, { "icirc", 238 }, { "iexcl", 161 }, { "Igrave", 204 },
            { "igrave", 236 }, { "image", 8465 }, { "infin", 8734 }, { "int", 8747 }, { "Iota", 921 }, { "iota", 953 },
            { "iquest", 191 }, { "isin", 8712 }, { "Iuml", 207 }, { "iuml", 239 }, { "Kappa", 922 }, { "kappa", 954 },
            { "Lambda", 923 }, { "lambda", 955 }, { "lang", 9001 }, { "laquo", 171 }, { "larr", 8592 }, { "lArr", 8656 },
            { "lceil", 8968 }, { "ldquo", 8220 }, { "le", 8804 }, { "lfloor", 8970 }, { "lowast", 8727 }, { "loz", 9674 },
            { "lrm", 8206 }, { "lsaquo", 8249 }, { "lsquo", 8216 }, { "lt", 60 }, { "macr", 175 }, { "mdash", 8212 },
            { "micro", 181 }, { "middot", 183 }, { "minus", 8722 }, { "Mu", 924 }, { "mu", 956 }, { "nabla", 8711 },
            { "nbsp", 160 }, { "ndash", 8211 }, { "ne", 8800 }, { "ni", 8715 }, { "not", 172 }, { "notin", 8713 },
            { "nsub", 8836 }, { "Ntilde", 209 }, { "ntilde", 241 }, { "Nu", 925 }, { "nu", 957 }, { "Oacute", 211 },
            { "oacute", 243 }, { "Ocirc", 212 }, { "ocirc", 244 }, { "OElig", 338 }, { "oelig", 339 }, { "Ograve", 210 },
            { "ograve", 242 }, { "oline", 8254 }, { "Omega", 937 }, { "omega", 969 }, { "Omicron", 927 }, { "omicron", 959 },
            { "oplus", 8853 }, { "or", 8744 }, { "ordf", 170 }, { "ordm", 186 }, { "Oslash", 216 }, { "oslash", 248 },
            { "Otilde", 213 }, { "otilde", 245 }, { "otimes", 8855 }, { "Ouml", 214 }, { "ouml", 246 }, { "para", 182 },
            { "part", 8706 }, { "permil", 8240 }, { "perp", 8869 }, { "Phi", 934 }, { "phi", 966 }, { "Pi", 928 },
            { "pi", 960 }, { "piv", 982 }, { "plusmn", 177 }, { "pound", 163 }, { "prime", 8242 }, { "Prime", 8243 },
            { "prod", 8719 }, { "prop", 8733 }, { "Psi", 936 }, { "psi", 968 }, { "quot", 34 }, { "radic", 8730 },
            { "rang", 9002 }, { "raquo", 187 }, { "rarr", 8594 }, { "rArr", 8658 }, { "rceil", 8969 }, { "rdquo", 8221 },
            { "real", 8476 }, { "reg", 174 }, { "rfloor", 8971 }, { "Rho", 929 }, { "rho", 961 }, { "rlm", 8207 },
            { "rsaquo", 8250 }, { "rsquo", 8217 }, { "sbquo", 8218 }, { "Scaron", 352 }, { "scaron", 353 }, { "sdot", 8901 },
            { "sect", 167 }, { "shy", 173 }, { "Sigma", 931 }, { "sigma", 963 }, { "sigmaf", 962 }, { "sim", 8764 },
            { "spades", 9824 }, { "sub", 8834 }, { "sube", 8838 }, { "sum", 8721 }, { "sup", 8835 }, { "sup1", 185 },
            { "sup2", 178 }, { "sup3", 179 }, { "supe", 8839 }, { "szlig", 223 }, { "Tau", 932 }, { "tau", 964 },
            { "there4", 8756 }, { "Theta", 920 }, { "theta", 952 }, { "thetasym", 977 }, { "thinsp", 8201 }, { "THORN", 222 },
            { "thorn", 254 }, { "tilde", 732 }, { "times", 215 }, { "trade", 8482 }, { "Uacute", 218 }, { "uacute", 250 },
            { "uarr", 8593 }, { "uArr", 8657 }, { "Ucirc", 219 }, { "ucirc", 251 }, { "Ugrave", 217 }, { "ugrave", 249 },
            { "uml", 168 }, { "upsih", 978 }, { "Upsilon", 933 }, { "upsilon", 965 }, { "Uuml", 220 }, { "uuml", 252 },
            { "weierp", 8472 }, { "Xi", 926 }, { "xi", 958 }, { "Yacute", 221 }, { "yacute", 253 }, { "yen", 165 },
            { "yuml", 255 }, { "Yuml", 376 }, { "Zeta", 918 }, { "zeta", 950 }, { "zwj", 8205 }, { "zwnj", 8204 }
        };

        static HttpUtil()
        {
            _longestHtmlEntity = 0;

            foreach (var item in _htmlEntitiesByEntity)
            {
                _longestHtmlEntity = Math.Max(_longestHtmlEntity, item.Key.Length);
                _htmlEntitiesByNumber.Add(item.Value, item.Key);
            }
        }

        public static Dictionary<string, string> UrlDecode(string content)
        {
            return UrlDecode(content, Encoding.UTF8);
        }

        public static Dictionary<string, string> UrlDecode(string content, Encoding encoding)
        {
            if (encoding == null)
                throw new ArgumentNullException("encoding");

            var result = new Dictionary<string, string>();

            string[] parts = content.Split('&');

            foreach (string part in parts)
            {
                string[] item = part.Split(new[] { '=' }, 2);

                string key = UriDecode(item[0], encoding);
                string value = item.Length == 1 ? "" : UriDecode(item[1], encoding);

                result[key] = value;
            }

            return result;
        }

        public static string UriDecode(string value)
        {
            return UriDecode(value, Encoding.UTF8);
        }

        public static string UriDecode(string value, Encoding encoding)
        {
            if (value == null)
                throw new ArgumentNullException("value");
            if (encoding == null)
                throw new ArgumentNullException("encoding");

            byte[] result = new byte[value.Length];
            int length = 0;

            for (int i = 0; i < value.Length; i++)
            {
                if (
                    value[i] == '%' &&
                    i < value.Length - 2 &&
                    IsHex(value[i + 1]) &&
                    IsHex(value[i + 2])
                ) {
                    result[length++] = (byte)(HexToInt(value[i + 1]) * 16 + HexToInt(value[i + 2]));

                    i += 2;
                }
                else if (value[i] == '+')
                {
                    result[length++] = (byte)' ';
                }
                else
                {
                    int c = value[i];

                    if (c > byte.MaxValue)
                        throw new InvalidOperationException("URI contained unexpected character");

                    result[length++] = (byte)c;
                }
            }

            return encoding.GetString(result, 0, length);
        }

        private static bool IsHex(char value)
        {
            switch (value)
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                    return true;

                default:
                    return false;
            }
        }

        private static int HexToInt(char value)
        {
            switch (value)
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    return value - '0';

                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                    return (value - 'a') + 10;

                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                    return (value - 'A') + 10;

                default:
                    throw new ArgumentOutOfRangeException("value");
            }
        }

        public static void TrimAll(string[] parts)
        {
            for (int i = 0; i < parts.Length; i++)
            {
                parts[i] = parts[i].Trim();
            }
        }

        public static string HtmlEncode(string value)
        {
            if (value == null)
                throw new ArgumentNullException("value");

            var sb = new StringBuilder();

            foreach (char c in value)
            {
                string entity;

                if (_htmlEntitiesByNumber.TryGetValue(c, out entity))
                {
                    sb.Append("&");
                    sb.Append(entity);
                    sb.Append(";");
                }
                else if (Char.IsControl(c))
                {
                    sb.Append("&#");
                    sb.Append((int)c);
                    sb.Append(";");
                }
                else
                {
                    sb.Append(c);
                }
            }

            return sb.ToString();
        }

        public static string HtmlDecode(string value)
        {
            if (value == null)
                throw new ArgumentNullException("value");

            var sb = new StringBuilder();

            for (int i = 0; i < value.Length; i++)
            {
                if (value[i] == '&' && value.Length > i + 2)
                {
                    // Scan for the ;.

                    int maxSearch = Math.Min(value.Length, i + _longestHtmlEntity + 2);
                    int endPosition = -1;

                    for (int j = i + 1; j < maxSearch; j++)
                    {
                        if (value[j] == ';')
                        {
                            endPosition = j;
                            break;
                        }
                    }

                    // If we did not find an end separator, just skip over this
                    // entity and treat is at text.

                    if (endPosition == -1)
                    {
                        sb.Append(value[i]);
                        continue;
                    }

                    // Are we in a numeric separator?

                    if (value[i + 1] == '#')
                    {
                        int offset = 2;

                        bool isHexNumeric = false;

                        if (value[i + 2] == 'x' || value[i + 2] == 'X')
                        {
                            isHexNumeric = true;
                            offset++;
                        }

                        // All parts of the numeric separator must be digits.

                        bool isNumeric = true;

                        for (int j = i + offset; j < endPosition; j++)
                        {
                            if (!(
                                Char.IsDigit(value[j]) ||
                                (isHexNumeric && HttpUtil.IsHex(value[j]))
                            ))
                            {
                                isNumeric = false;
                                break;
                            }
                        }

                        // If not all numeric, just skip over this
                        // entity and treat is at text.

                        if (!isNumeric)
                        {
                            sb.Append(value[i]);
                            continue;
                        }

                        // Convert the numeric entity to unicode.

                        string numericEntity = value.Substring(i + offset, endPosition - (i + offset));

                        sb.Append((char)int.Parse(numericEntity, isHexNumeric ? NumberStyles.HexNumber : NumberStyles.Integer));

                        i = endPosition;
                    }
                    else
                    {
                        string entity = value.Substring(i + 1, endPosition - (i + 1));

                        int codePoint;

                        if (_htmlEntitiesByEntity.TryGetValue(entity, out codePoint))
                        {
                            sb.Append((char)codePoint);

                            i = endPosition;
                        }
                        else
                        {
                            // If we don't know the entity, just skip over this
                            // entity and treat is at text.

                            sb.Append(value[i]);
                        }
                    }
                }
                else
                {
                    sb.Append(value[i]);
                }
            }

            return sb.ToString();
        }
    }
}
