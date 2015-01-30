using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WSEProfiler
{
	public partial class CallTab : UserControl
	{
		public event EventHandler<BlockEventArgs> Closed;
		public event EventHandler<BlockEventArgs> Opened;
		private ListViewColumnSorter _callersColumnSorter = new ListViewColumnSorter();
		private ListViewColumnSorter _calleesColumnSorter = new ListViewColumnSorter();
		private string _blockName;

		public CallTab(string blockName, ICallDetails callDetails)
		{
			_blockName = blockName;
			InitializeComponent();
			listViewCallers.ListViewItemSorter = _callersColumnSorter;
			listViewCallees.ListViewItemSorter = _calleesColumnSorter;

			foreach (var caller in callDetails.Callers)
			{
				listViewCallers.Items.Add(new ListViewItem(new ListViewItem.ListViewSubItem[]
				{
					new ListViewItem.ListViewSubItem() { Text = caller.Key.ToString(), Tag = caller.Key.ToString() },
					new ListViewItem.ListViewSubItem() { Text = caller.Value.ToString(), Tag = caller.Value },
				}, 0));
			}

			callDetails.Callees.Add("Self", callDetails.TimeSelf);

			foreach (var callee in callDetails.Callees.OrderByDescending(t => t.Value))
			{
				var percentage = callDetails.TimeTotal == 0.0f ? 0.0f : (callee.Value / callDetails.TimeTotal) * 100;

				listViewCallees.Items.Add(new ListViewItem(new ListViewItem.ListViewSubItem[]
				{
					new ListViewItem.ListViewSubItem() { Text = callee.Key.ToString(), Tag = callee.Key.ToString() },
					new ListViewItem.ListViewSubItem() { Text = percentage.ToString("0.##"), Tag = percentage },
					new ListViewItem.ListViewSubItem() { Text = callee.Value.FormatTime(), Tag = callee.Value },
				}, 0));
			}

			ListViewColor.Update(listViewCallees, 1);
		}

		private void listViewCallers_ColumnClick(object sender, ColumnClickEventArgs e)
		{
			_callersColumnSorter.ColumnClicked(e.Column);
			listViewCallers.Sort();
		}

		private void listViewCallees_ColumnClick(object sender, ColumnClickEventArgs e)
		{
			_calleesColumnSorter.ColumnClicked(e.Column);
			listViewCallees.Sort();
		}

		private void listViewCallers_DoubleClick(object sender, EventArgs e)
		{
			if (listViewCallers.SelectedItems[0] != null)
				OnOpened(listViewCallers.SelectedItems[0].Text);
		}

		private void listViewCallees_DoubleClick(object sender, EventArgs e)
		{
			if (listViewCallees.SelectedItems[0] != null)
				OnOpened(listViewCallees.SelectedItems[0].Text);
		}

		private void buttonClose_Click(object sender, EventArgs e)
		{
			OnClosed();
		}

		private void OnOpened(string blockName)
		{
			if (Opened != null)
				Opened(this, new BlockEventArgs(blockName));
		}

		private void OnClosed()
		{
			if (Closed != null)
				Closed(this, new BlockEventArgs(_blockName));
		}
	}

	public class BlockEventArgs : EventArgs
	{
		public string BlockName { get; private set; }

		public BlockEventArgs(string blockName)
		{
			BlockName = blockName;
		}
	}
}
