using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WSEProfiler
{
	class ListViewColor
	{
		public static int Interpolate(int f1, int f2, float strength)
		{
			return (int)(f1 * strength + f2 * (1.0f - strength));
		}

		public static Color Interpolate(Color c1, Color c2, float str)
		{
			return Color.FromArgb(10, Interpolate(c1.R, c2.R, str), Interpolate(c1.G, c2.G, str), Interpolate(c1.B, c2.B, str));
		}

		public static Color GetColorForPercentage(float percentage)
		{
			Color upper;
			Color lower;
			float strength;

			if (percentage > 60)
			{
				upper = Color.LightCoral;
				lower = Color.LightCoral;
				strength = 100.0f;
			}
			else if (percentage > 20)
			{
				upper = Color.LightCoral;
				lower = Color.Khaki;
				strength = (percentage - 20) / 40.0f;
			}
			else
			{
				upper = Color.Khaki;
				lower = Color.DarkSeaGreen;
				strength = percentage / 20.0f;
			}

			return Interpolate(upper, lower, strength);
		}

		public static void Update(ListView listView, int column)
		{
			if (column < 0)
			{
				foreach (ListViewItem item in listView.Items)
				{
					item.BackColor = Color.Transparent;
				}
			}
			else
			{
				foreach (ListViewItem item in listView.Items)
				{
					item.BackColor = GetColorForPercentage((float)item.SubItems[column].Tag);
				}
			}
		}
	}
}
