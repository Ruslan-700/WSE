namespace WSEProfiler
{
    partial class Timeline
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Timeline));
            this.button_zoom = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.panel1 = new System.Windows.Forms.Panel();
            this.checkBox_filter_items = new System.Windows.Forms.CheckBox();
            this.checkBox_filter_props = new System.Windows.Forms.CheckBox();
            this.checkBox_filter_other = new System.Windows.Forms.CheckBox();
            this.checkBox_filter_scripts = new System.Windows.Forms.CheckBox();
            this.checkBox_filter_mst_cons = new System.Windows.Forms.CheckBox();
            this.checkBox_filter_mst_cond = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label_drawinfo = new System.Windows.Forms.Label();
            this.btn_clear_search = new System.Windows.Forms.Label();
            this.vScrollBar1 = new System.Windows.Forms.VScrollBar();
            this.canvas1 = new WSEProfiler.Canvas();
            this.button1 = new System.Windows.Forms.Button();
            this.toolStrip1.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.canvas1)).BeginInit();
            this.SuspendLayout();
            // 
            // button_zoom
            // 
            this.button_zoom.Location = new System.Drawing.Point(3, 3);
            this.button_zoom.Name = "button_zoom";
            this.button_zoom.Size = new System.Drawing.Size(75, 23);
            this.button_zoom.TabIndex = 1;
            this.button_zoom.Text = "Zoom Out";
            this.button_zoom.UseVisualStyleBackColor = true;
            this.button_zoom.Click += new System.EventHandler(this.button_zoom_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(84, 5);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(181, 20);
            this.textBox1.TabIndex = 4;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            this.textBox1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBox1_KeyDown);
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Checked = true;
            this.checkBox1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox1.Location = new System.Drawing.Point(271, 7);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(33, 17);
            this.checkBox1.TabIndex = 5;
            this.checkBox1.Text = "P";
            this.toolTip1.SetToolTip(this.checkBox1, "Performance mode\r\nRender a simplified view when zoomed out > 5000ms\r\nSome calls m" +
        "ay not be drawn at medium zoom levels");
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // toolStrip1
            // 
            this.toolStrip1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripDropDownButton1});
            this.toolStrip1.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
            this.toolStrip1.Location = new System.Drawing.Point(969, 3);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
            this.toolStrip1.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.toolStrip1.Size = new System.Drawing.Size(32, 25);
            this.toolStrip1.TabIndex = 6;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // toolStripDropDownButton1
            // 
            this.toolStripDropDownButton1.BackColor = System.Drawing.SystemColors.Control;
            this.toolStripDropDownButton1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.toolStripDropDownButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripDropDownButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton1.Image")));
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = new System.Drawing.Size(29, 22);
            this.toolStripDropDownButton1.Text = "Show";
            this.toolStripDropDownButton1.ToolTipText = "Filter";
            // 
            // panel1
            // 
            this.panel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.panel1.BackColor = System.Drawing.SystemColors.Control;
            this.panel1.Controls.Add(this.checkBox_filter_items);
            this.panel1.Controls.Add(this.checkBox_filter_props);
            this.panel1.Controls.Add(this.checkBox_filter_other);
            this.panel1.Controls.Add(this.checkBox_filter_scripts);
            this.panel1.Controls.Add(this.checkBox_filter_mst_cons);
            this.panel1.Controls.Add(this.checkBox_filter_mst_cond);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Location = new System.Drawing.Point(460, 8);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(122, 184);
            this.panel1.TabIndex = 9;
            // 
            // checkBox_filter_items
            // 
            this.checkBox_filter_items.AutoSize = true;
            this.checkBox_filter_items.Checked = true;
            this.checkBox_filter_items.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_filter_items.Location = new System.Drawing.Point(4, 113);
            this.checkBox_filter_items.Name = "checkBox_filter_items";
            this.checkBox_filter_items.Size = new System.Drawing.Size(87, 17);
            this.checkBox_filter_items.TabIndex = 15;
            this.checkBox_filter_items.Text = "Item Triggers";
            this.checkBox_filter_items.UseVisualStyleBackColor = true;
            this.checkBox_filter_items.CheckedChanged += new System.EventHandler(this.checkBox_filter_CheckedChanged);
            // 
            // checkBox_filter_props
            // 
            this.checkBox_filter_props.AutoSize = true;
            this.checkBox_filter_props.Checked = true;
            this.checkBox_filter_props.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_filter_props.Location = new System.Drawing.Point(4, 89);
            this.checkBox_filter_props.Name = "checkBox_filter_props";
            this.checkBox_filter_props.Size = new System.Drawing.Size(89, 17);
            this.checkBox_filter_props.TabIndex = 14;
            this.checkBox_filter_props.Text = "Prop Triggers";
            this.checkBox_filter_props.UseVisualStyleBackColor = true;
            this.checkBox_filter_props.CheckedChanged += new System.EventHandler(this.checkBox_filter_CheckedChanged);
            // 
            // checkBox_filter_other
            // 
            this.checkBox_filter_other.AutoSize = true;
            this.checkBox_filter_other.Checked = true;
            this.checkBox_filter_other.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_filter_other.Location = new System.Drawing.Point(3, 136);
            this.checkBox_filter_other.Name = "checkBox_filter_other";
            this.checkBox_filter_other.Size = new System.Drawing.Size(52, 17);
            this.checkBox_filter_other.TabIndex = 13;
            this.checkBox_filter_other.Text = "Other";
            this.checkBox_filter_other.UseVisualStyleBackColor = true;
            this.checkBox_filter_other.CheckedChanged += new System.EventHandler(this.checkBox_filter_CheckedChanged);
            // 
            // checkBox_filter_scripts
            // 
            this.checkBox_filter_scripts.AutoSize = true;
            this.checkBox_filter_scripts.Checked = true;
            this.checkBox_filter_scripts.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_filter_scripts.Location = new System.Drawing.Point(4, 65);
            this.checkBox_filter_scripts.Name = "checkBox_filter_scripts";
            this.checkBox_filter_scripts.Size = new System.Drawing.Size(58, 17);
            this.checkBox_filter_scripts.TabIndex = 12;
            this.checkBox_filter_scripts.Text = "Scripts";
            this.checkBox_filter_scripts.UseVisualStyleBackColor = true;
            this.checkBox_filter_scripts.CheckedChanged += new System.EventHandler(this.checkBox_filter_CheckedChanged);
            // 
            // checkBox_filter_mst_cons
            // 
            this.checkBox_filter_mst_cons.AutoSize = true;
            this.checkBox_filter_mst_cons.Checked = true;
            this.checkBox_filter_mst_cons.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_filter_mst_cons.Location = new System.Drawing.Point(4, 41);
            this.checkBox_filter_mst_cons.Name = "checkBox_filter_mst_cons";
            this.checkBox_filter_mst_cons.Size = new System.Drawing.Size(116, 17);
            this.checkBox_filter_mst_cons.TabIndex = 11;
            this.checkBox_filter_mst_cons.Text = "mst Consequences";
            this.checkBox_filter_mst_cons.UseVisualStyleBackColor = true;
            this.checkBox_filter_mst_cons.CheckedChanged += new System.EventHandler(this.checkBox_filter_CheckedChanged);
            // 
            // checkBox_filter_mst_cond
            // 
            this.checkBox_filter_mst_cond.AutoSize = true;
            this.checkBox_filter_mst_cond.Checked = true;
            this.checkBox_filter_mst_cond.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_filter_mst_cond.Location = new System.Drawing.Point(6, 17);
            this.checkBox_filter_mst_cond.Name = "checkBox_filter_mst_cond";
            this.checkBox_filter_mst_cond.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.checkBox_filter_mst_cond.Size = new System.Drawing.Size(94, 17);
            this.checkBox_filter_mst_cond.TabIndex = 10;
            this.checkBox_filter_mst_cond.Text = "mst Conditions";
            this.checkBox_filter_mst_cond.UseVisualStyleBackColor = true;
            this.checkBox_filter_mst_cond.CheckedChanged += new System.EventHandler(this.checkBox_filter_CheckedChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 13);
            this.label2.TabIndex = 9;
            this.label2.Text = "Filter";
            // 
            // label_drawinfo
            // 
            this.label_drawinfo.AutoSize = true;
            this.label_drawinfo.Location = new System.Drawing.Point(310, 8);
            this.label_drawinfo.Name = "label_drawinfo";
            this.label_drawinfo.Size = new System.Drawing.Size(0, 13);
            this.label_drawinfo.TabIndex = 10;
            // 
            // btn_clear_search
            // 
            this.btn_clear_search.AutoSize = true;
            this.btn_clear_search.BackColor = System.Drawing.SystemColors.Window;
            this.btn_clear_search.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btn_clear_search.Location = new System.Drawing.Point(250, 8);
            this.btn_clear_search.Name = "btn_clear_search";
            this.btn_clear_search.Size = new System.Drawing.Size(12, 13);
            this.btn_clear_search.TabIndex = 12;
            this.btn_clear_search.Text = "x";
            this.btn_clear_search.Visible = false;
            this.btn_clear_search.Click += new System.EventHandler(this.btn_clear_search_Click);
            // 
            // vScrollBar1
            // 
            this.vScrollBar1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.vScrollBar1.Location = new System.Drawing.Point(982, 32);
            this.vScrollBar1.Name = "vScrollBar1";
            this.vScrollBar1.Size = new System.Drawing.Size(19, 432);
            this.vScrollBar1.TabIndex = 13;
            this.vScrollBar1.Visible = false;
            this.vScrollBar1.Scroll += new System.Windows.Forms.ScrollEventHandler(this.vScrollBar1_Scroll);
            // 
            // canvas1
            // 
            this.canvas1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.canvas1.DrawPadding = new System.Windows.Forms.Padding(7, 5, 7, 5);
            this.canvas1.Location = new System.Drawing.Point(3, 32);
            this.canvas1.Name = "canvas1";
            this.canvas1.Size = new System.Drawing.Size(998, 432);
            this.canvas1.TabIndex = 14;
            this.canvas1.TabStop = false;
            this.canvas1.Paint += new System.Windows.Forms.PaintEventHandler(this.canvas1_Paint);
            this.canvas1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.canvas1_MouseDown);
            this.canvas1.MouseLeave += new System.EventHandler(this.canvas1_MouseLeave);
            this.canvas1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.canvas1_MouseMove);
            this.canvas1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.canvas1_MouseUp);
            this.canvas1.MouseWheel += new System.Windows.Forms.MouseEventHandler(this.canvas1_Wheel);
            this.canvas1.Resize += new System.EventHandler(this.canvas1_Resize);
            // 
            // button1
            // 
            this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.button1.Location = new System.Drawing.Point(930, 3);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(27, 23);
            this.button1.TabIndex = 15;
            this.button1.Text = "?";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Timeline
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.button1);
            this.Controls.Add(this.vScrollBar1);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.canvas1);
            this.Controls.Add(this.btn_clear_search);
            this.Controls.Add(this.label_drawinfo);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.button_zoom);
            this.DoubleBuffered = true;
            this.Name = "Timeline";
            this.Size = new System.Drawing.Size(1004, 467);
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.canvas1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button_zoom;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.CheckBox checkBox_filter_mst_cond;
        private System.Windows.Forms.CheckBox checkBox_filter_other;
        private System.Windows.Forms.CheckBox checkBox_filter_scripts;
        private System.Windows.Forms.CheckBox checkBox_filter_mst_cons;
        private System.Windows.Forms.CheckBox checkBox_filter_items;
        private System.Windows.Forms.CheckBox checkBox_filter_props;
        private System.Windows.Forms.Label label_drawinfo;
        private System.Windows.Forms.Label btn_clear_search;
        private System.Windows.Forms.VScrollBar vScrollBar1;
        private Canvas canvas1;
        private System.Windows.Forms.Button button1;
    }
}
