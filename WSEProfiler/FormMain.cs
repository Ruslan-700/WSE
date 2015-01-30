using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WSEProfiler
{
	public partial class FormMain : Form
	{
		static readonly string NAME = "WSEProfilerGUI v1.2";
		private ListViewColumnSorter _lvwColumnSorter = new ListViewColumnSorter();
		private IProfilerFile _curFile = null;
		private Dictionary<string, TabPage> _openTabs = new Dictionary<string, TabPage>();

		public FormMain()
		{
			InitializeComponent();
			Text = NAME;
			listView.ListViewItemSorter = _lvwColumnSorter;
		}

		private void Cleanup()
		{
			while (tabControl.TabPages.Count > 1)
			{
				tabControl.TabPages.RemoveAt(1);
			}

			listView.Items.Clear();
			toolStripStatusLabel1.Text = "";
			_curFile = null;
			_openTabs.Clear();
			GC.Collect();
		}

		private void openToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (openFileDialog1.ShowDialog() != DialogResult.OK)
				return;

			Cleanup();
			this.ClearWarnings();

			string path = openFileDialog1.FileName;
			
			if (path.EndsWith(".wseprfb"))
				_curFile = new BinaryProfilerFile(path);
			else
				_curFile = new TextProfilerFile(path);

			try
			{
				_curFile.Parse("");
			}
			catch (ErrorException ex)
			{
				MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Critical Error", MessageBoxButtons.OK, MessageBoxIcon.Stop);
				Application.Exit();
			}

			foreach (var info in _curFile.CallInfos)
			{
				float selfSum = info.SumSelf;
				float fullSum = info.SumTotal;
				float selfAvg = info.AvgSelf;
				float fullAvg = info.AvgTotal;
				float selfMin = info.MinSelf;
				float fullMin = info.MinTotal;
				float selfMax = info.MaxSelf;
				float fullMax = info.MaxTotal;
				float percentage = (fullSum / _curFile.TimeTotal) * 100;

				listView.Items.Add(new ListViewItem(new ListViewItem.ListViewSubItem[]
				{
					new ListViewItem.ListViewSubItem() { Text = info.ToString(), Tag = info.ToString() },
					new ListViewItem.ListViewSubItem() { Text = info.Count.ToString(), Tag = info.Count },
					new ListViewItem.ListViewSubItem() { Text = percentage.ToString("0.##"), Tag = percentage },
					new ListViewItem.ListViewSubItem() { Text = selfSum.FormatTime(), Tag = selfSum },
					new ListViewItem.ListViewSubItem() { Text = fullSum.FormatTime(), Tag = fullSum },
					new ListViewItem.ListViewSubItem() { Text = selfAvg.FormatTime(), Tag = selfAvg },
					new ListViewItem.ListViewSubItem() { Text = fullAvg.FormatTime(), Tag = fullAvg },
					new ListViewItem.ListViewSubItem() { Text = selfMin.FormatTime(), Tag = selfMin },
					new ListViewItem.ListViewSubItem() { Text = fullMin.FormatTime(), Tag = fullMin },
					new ListViewItem.ListViewSubItem() { Text = selfMax.FormatTime(), Tag = selfMax },
					new ListViewItem.ListViewSubItem() { Text = fullMax.FormatTime(), Tag = fullMax },
				}, 0));
			}

			ListViewColor.Update(listView, 2);

			_curFile.Close();
			Text = NAME + " (" + Path.GetFileName(openFileDialog1.FileName) + ")";
			toolStripStatusLabel1.Text = _curFile.InfoString;
		}

		private void closeToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Cleanup();
		}

		private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
		{
			MessageBox.Show(string.Format("{0} by cmpxchg8b", NAME), "About", MessageBoxButtons.OK, MessageBoxIcon.Information);
		}
		
		private void listView_ColumnClick(object sender, ColumnClickEventArgs e)
		{
			_lvwColumnSorter.ColumnClicked(e.Column);
			listView.Sort();
		}

		private void listView_MouseDoubleClick(object sender, MouseEventArgs e)
		{
			if (listView.SelectedItems[0] != null)
				OpenDetailsTab(listView.SelectedItems[0].Text);
		}

		private void OpenDetailsTab(string blockName)
		{
			if (blockName == "Self")
				return;

			if (blockName == "Engine")
			{
				tabControl.SelectedIndex = 0;
				return;
			}

			if (!_openTabs.ContainsKey(blockName))
			{
				var page = new TabPage(blockName);

				_curFile.Parse(blockName);

				var callTab = new CallTab(blockName, _curFile.CallDetails);

				callTab.Closed += new EventHandler<BlockEventArgs>(callTab_Closed);
				callTab.Opened += new EventHandler<BlockEventArgs>(callTab_Opened);
				page.Controls.Add(callTab);
				tabControl.TabPages.Add(page);
				_curFile.Close();
				_openTabs.Add(blockName, page);
			}

			tabControl.SelectedTab = _openTabs[blockName];
		}

		private void CloseDetailsTab(string blockName)
		{
			if (tabControl.SelectedIndex < tabControl.TabPages.Count - 1)
				tabControl.SelectedIndex++;
			else
				tabControl.SelectedIndex--;

			tabControl.TabPages.Remove(_openTabs[blockName]);
			_openTabs.Remove(blockName);
		}

		void callTab_Closed(object sender, BlockEventArgs e)
		{
			CloseDetailsTab(e.BlockName);
		}

		void callTab_Opened(object sender, BlockEventArgs e)
		{
			OpenDetailsTab(e.BlockName);
		}
	}
}
