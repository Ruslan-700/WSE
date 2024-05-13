using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace WSEProfiler
{
	public partial class FormMain : Form
	{
		static readonly string NAME = "WSEProfilerGUI v2.0";
		private ListViewColumnSorter _lvwColumnSorter = new ListViewColumnSorter();
		private BinaryProfilerFile _curFile = null;
		private Dictionary<string, TabPage> _openTabs = new Dictionary<string, TabPage>();
        private List<ListViewItem> _master_list;

        //For searchfield placeholder
        private const int EM_SETCUEBANNER = 0x1501;
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        private static extern Int32 SendMessage(IntPtr hWnd, int msg, int wParam, [MarshalAs(UnmanagedType.LPWStr)]string lParam);

		public FormMain()
		{
			InitializeComponent();
			Text = NAME;
			listView.ListViewItemSorter = _lvwColumnSorter;
            tabControl.DrawMode = TabDrawMode.OwnerDrawFixed;
            SendMessage(textBox1.Handle, EM_SETCUEBANNER, 0, "Search");
		}

		private void Cleanup()
		{
			while (tabControl.TabPages.Count > 2)
			{
				tabControl.TabPages.RemoveAt(2);
			}

			listView.Items.Clear();
			toolStripStatusLabel1.Text = "";
			_curFile = null;
			_openTabs.Clear();
            timeline1.reset();
			GC.Collect();
		}

		private void openToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (openFileDialog1.ShowDialog() != DialogResult.OK)
				return;

			Cleanup();
			this.ClearWarnings();

			string path = openFileDialog1.FileName;
			
            //if (path.EndsWith(".wseprfb"))
				_curFile = new BinaryProfilerFile(path);
            //else
                //_curFile = new TextProfilerFile(path);

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

            _master_list = new List<ListViewItem>();
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

                _master_list.Add(new ListViewItem(new ListViewItem.ListViewSubItem[]
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
            filter_list("");

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
			MessageBox.Show(string.Format("{0} by cmpxchg8b & AgentSmith", NAME), "About", MessageBoxButtons.OK, MessageBoxIcon.Information);
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
                var page = new TabPage(blockName + "     ");

				_curFile.Parse(blockName);

				var callTab = new CallTab(blockName, _curFile.CallDetails);

				callTab.Closed += new EventHandler<BlockEventArgs>(callTab_Closed);
				callTab.Opened += new EventHandler<BlockEventArgs>(callTab_Opened);

				page.Controls.Add(callTab);
				tabControl.TabPages.Add(page);

                callTab.Dock = DockStyle.Fill;
                
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

        void filter_list(string txt)
        {
            listView.Items.Clear();

            txt = txt.ToLower();
            foreach (ListViewItem itm in _master_list)
            {
                if (itm.SubItems[0].Text.ToLower().Contains(txt))
                {
                    listView.Items.Add(itm);
                }
            }
            label1.Text = string.Format("Showing {0}/{1}", listView.Items.Count, _master_list.Count);
            label1.Show();
        }

        Rectangle tab_getXrect(Rectangle tabTextArea)
        {
            return new Rectangle(tabTextArea.Right - 20, tabTextArea.Top + 5, 16, 16);
        }

        private void tabControl_DrawItem(object sender, DrawItemEventArgs e)
        {
            //a bit of background for selected tab
            if (e.Index == this.tabControl.SelectedIndex)
            {
                var bc = Color.FromArgb(20, SystemColors.Highlight);
                var bb = new SolidBrush(bc);
                e.Graphics.FillRectangle(bb, e.Bounds);
            }

            //title
            TextRenderer.DrawText(e.Graphics, this.tabControl.TabPages[e.Index].Text, e.Font, e.Bounds, Color.Black, TextFormatFlags.HorizontalCenter | TextFormatFlags.SingleLine | TextFormatFlags.GlyphOverhangPadding | TextFormatFlags.VerticalCenter | TextFormatFlags.WordEllipsis);

            //close "x"
            if (e.Index >= 2)
            {
                Rectangle x_rect = tab_getXrect(e.Bounds);
                var hc = Color.FromArgb(50, SystemColors.Highlight);
                var br = new SolidBrush(hc);


                //.tag is used for mouseover
                if (this.tabControl.Tag == this.tabControl.TabPages[e.Index])
                {
                    e.Graphics.FillRectangle(br, x_rect);
                    e.Graphics.DrawString("x", new Font(e.Font, FontStyle.Bold), Brushes.Black, x_rect.Left + 3, x_rect.Top - 0);
                    e.Graphics.DrawRectangle(SystemPens.Highlight, x_rect);
                }
                else
                {
                    e.Graphics.DrawString("x", new Font(e.Font, FontStyle.Bold), Brushes.Black, x_rect.Left + 3, x_rect.Top - 0);
                }
            }
           
            e.DrawFocusRectangle();

        }

        private void tabControl_MouseClick(object sender, MouseEventArgs e)
        {
            for (int i = 2; i < this.tabControl.TabPages.Count; i++)
            {
                Rectangle closeButton = tab_getXrect(tabControl.GetTabRect(i));

                if (closeButton.Contains(e.Location))
                {
                    var cTab = (CallTab)this.tabControl.TabPages[i].Controls[0];
                    CloseDetailsTab(cTab.BlockName);
                    return;
                }
            }
        }

        private void tabControl_MouseMove(object sender, MouseEventArgs e)
        {
            for (int i = 2; i < this.tabControl.TabPages.Count; i++)
            {
                Rectangle closeButton = tab_getXrect(tabControl.GetTabRect(i));

                if (closeButton.Contains(e.Location))
                {
                    if (tabControl.Tag != this.tabControl.TabPages[i])
                    {
                        tabControl.Tag = this.tabControl.TabPages[i];
                        tabControl.Invalidate();
                    }
                    return;
                }
            }

            if (tabControl.Tag != null)
            {
                tabControl.Tag = null;
                tabControl.Invalidate();
            }
        }

        private void tabControl_MouseLeave(object sender, EventArgs e)
        {
            if (tabControl.Tag != null)
            {
                tabControl.Tag = null;
                tabControl.Invalidate();
            }
        }

        private void tabControl_MouseWheel(object sender, MouseEventArgs e)
        {
            Rectangle r = this.tabControl.ClientRectangle;
            r.Height = this.tabControl.ItemSize.Height;

            if (!r.Contains(e.Location))
                return;

            if (e.Delta > 0)
            {
                if (this.tabControl.SelectedIndex > 0)
                {
                    this.tabControl.SelectedIndex--;
                }
            }
            else
            {
                if (this.tabControl.SelectedIndex < this.tabControl.TabCount - 1)
                {
                    this.tabControl.SelectedIndex++;
                }
            }
        }

        private void btn_clear_search_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
            filter_list("");
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            if (textBox1.Text == "")
            {
                btn_clear_search.Hide();
            }
            else
            {
                btn_clear_search.Show();
            }
            filter_list(textBox1.Text);
        }
	}
}
