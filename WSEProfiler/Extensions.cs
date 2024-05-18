using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace WSEProfiler
{
	internal static class Extensions
	{
		private static HashSet<string> _warnings = new HashSet<string>();

		public static void ClearWarnings(this object obj)
		{
			_warnings.Clear();
		}

		public static void ShowWarning(this object obj, string text)
		{
			if (!_warnings.Contains(text))
			{
				MessageBox.Show(text, "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				_warnings.Add(text);
			}
		}

		public static string FormatTime(this float val)
		{
			if (val < 1000)
				return string.Format("{0:0.##} µs", val);
			else if (val < 1000000)
				return string.Format("{0:0.##} ms", val / 1000);
			else
				return string.Format("{0:0.##} s", val / 1000 / 1000);
		}

        public static string FormatTime(this uint val)
        {
            if (val < 1000)
                return string.Format("{0:0.##} µs", val);
            else
            {
                if (val < 1000000)
                    return string.Format("{0:0.##} ms", ((float)val) / 1000);
                else
                    return string.Format("{0:0.##} s", ((float)val) / 1000 / 1000);
            } 
                
        }

        public static uint ToRgb(this Color val)
        {
            return (uint)val.ToArgb() & 0xFFFFFF;
        }

        public static Color ToColor(this uint val)
        {
            int r = (int)(val >> 16) & 0xFF;
            int g = (int)(val >> 8) & 0xFF;
            int b = (int)val & 0xFF;

            return Color.FromArgb(r, g, b);
        }

        public static void ToRgb(this Color val, ref byte[] rgb)
        {
            int col = val.ToArgb();

            rgb[0] = (byte)((col >> 16) & 0xFF);
            rgb[1] = (byte)((col >> 8) & 0xFF);
            rgb[2] = (byte)(col & 0xFF);
        }
	}
}
