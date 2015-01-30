using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WSEProfiler
{
	public class AlphanumericComparer : IComparer
	{
		public int Compare(object x, object y)
		{
			string s1 = x as string;
			string s2 = y as string;

			if (s1 == null || s2 == null)
			{
				return CaseInsensitiveComparer.Default.Compare(x, y);
			}

			int len1 = s1.Length;
			int len2 = s2.Length;
			int marker1 = 0;
			int marker2 = 0;

			while (marker1 < len1 && marker2 < len2)
			{
				char ch1 = s1[marker1];
				char ch2 = s2[marker2];

				char[] space1 = new char[len1];
				int loc1 = 0;
				char[] space2 = new char[len2];
				int loc2 = 0;

				do
				{
					space1[loc1++] = ch1;
					marker1++;

					if (marker1 < len1)
					{
						ch1 = s1[marker1];
					}
					else
					{
						break;
					}
				}
				while (char.IsDigit(ch1) == char.IsDigit(space1[0]));

				do
				{
					space2[loc2++] = ch2;
					marker2++;

					if (marker2 < len2)
					{
						ch2 = s2[marker2];
					}
					else
					{
						break;
					}
				}
				while (char.IsDigit(ch2) == char.IsDigit(space2[0]));

				string str1 = new string(space1);
				string str2 = new string(space2);

				int result;

				if (char.IsDigit(space1[0]) && char.IsDigit(space2[0]))
				{
					int thisNumericChunk = int.Parse(str1);
					int thatNumericChunk = int.Parse(str2);
					result = thisNumericChunk.CompareTo(thatNumericChunk);
				}
				else
				{
					result = str1.CompareTo(str2);
				}

				if (result != 0)
				{
					return result;
				}
			}
			return len1 - len2;
		}
	}

	public class ListViewColumnSorter : IComparer
	{
		private int ColumnToSort;
		private SortOrder OrderOfSort;
		private IComparer ObjectCompare;

		public ListViewColumnSorter()
		{
			ColumnToSort = 0;
			OrderOfSort = SortOrder.None;
			ObjectCompare = new AlphanumericComparer();
		}

		public void ColumnClicked(int column)
		{
			if (column == SortColumn)
			{
				if (Order == SortOrder.Ascending)
					Order = SortOrder.Descending;
				else
					Order = SortOrder.Ascending;
			}
			else
			{
				SortColumn = column;
				Order = SortOrder.Ascending;
			}
		}

		public int Compare(object x, object y)
		{
			ListViewItem listviewX = (ListViewItem)x;
			ListViewItem listviewY = (ListViewItem)y;

			int compareResult = ObjectCompare.Compare(listviewX.SubItems[ColumnToSort].Tag, listviewY.SubItems[ColumnToSort].Tag);

			if (OrderOfSort == SortOrder.Ascending)
				return compareResult;
			else if (OrderOfSort == SortOrder.Descending)
				return -compareResult;
			else
				return 0;
		}

		public int SortColumn
		{
			set { ColumnToSort = value; }
			get { return ColumnToSort; }
		}

		public SortOrder Order
		{
			set { OrderOfSort = value; }
			get { return OrderOfSort; }
		}
	}
}
