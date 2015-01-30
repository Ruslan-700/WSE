using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

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
	}
}
