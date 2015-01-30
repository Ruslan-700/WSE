namespace WSEProfiler
{
	partial class CallTab
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
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.listViewCallers = new System.Windows.Forms.ListView();
			this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.listViewCallees = new System.Windows.Forms.ListView();
			this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.buttonClose = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(519, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(50, 13);
			this.label1.TabIndex = 2;
			this.label1.Text = "Called by";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(3, 0);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(68, 13);
			this.label2.TabIndex = 3;
			this.label2.Text = "Spent time in";
			// 
			// listViewCallers
			// 
			this.listViewCallers.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
			this.listViewCallers.FullRowSelect = true;
			this.listViewCallers.Location = new System.Drawing.Point(522, 16);
			this.listViewCallers.MultiSelect = false;
			this.listViewCallers.Name = "listViewCallers";
			this.listViewCallers.Size = new System.Drawing.Size(435, 521);
			this.listViewCallers.TabIndex = 4;
			this.listViewCallers.UseCompatibleStateImageBehavior = false;
			this.listViewCallers.View = System.Windows.Forms.View.Details;
			this.listViewCallers.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.listViewCallers_ColumnClick);
			this.listViewCallers.DoubleClick += new System.EventHandler(this.listViewCallers_DoubleClick);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Context";
			this.columnHeader1.Width = 350;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Calls";
			// 
			// listViewCallees
			// 
			this.listViewCallees.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5});
			this.listViewCallees.FullRowSelect = true;
			this.listViewCallees.Location = new System.Drawing.Point(3, 16);
			this.listViewCallees.MultiSelect = false;
			this.listViewCallees.Name = "listViewCallees";
			this.listViewCallees.Size = new System.Drawing.Size(513, 521);
			this.listViewCallees.TabIndex = 5;
			this.listViewCallees.UseCompatibleStateImageBehavior = false;
			this.listViewCallees.View = System.Windows.Forms.View.Details;
			this.listViewCallees.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.listViewCallees_ColumnClick);
			this.listViewCallees.DoubleClick += new System.EventHandler(this.listViewCallees_DoubleClick);
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "Context";
			this.columnHeader3.Width = 350;
			// 
			// columnHeader4
			// 
			this.columnHeader4.Text = "%";
			this.columnHeader4.Width = 40;
			// 
			// columnHeader5
			// 
			this.columnHeader5.Text = "Time";
			this.columnHeader5.Width = 90;
			// 
			// buttonClose
			// 
			this.buttonClose.Location = new System.Drawing.Point(963, 507);
			this.buttonClose.Name = "buttonClose";
			this.buttonClose.Size = new System.Drawing.Size(80, 30);
			this.buttonClose.TabIndex = 6;
			this.buttonClose.Text = "Close";
			this.buttonClose.UseVisualStyleBackColor = true;
			this.buttonClose.Click += new System.EventHandler(this.buttonClose_Click);
			// 
			// CallTab
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.buttonClose);
			this.Controls.Add(this.listViewCallees);
			this.Controls.Add(this.listViewCallers);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Name = "CallTab";
			this.Size = new System.Drawing.Size(1200, 540);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ListView listViewCallers;
		private System.Windows.Forms.ListView listViewCallees;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private System.Windows.Forms.Button buttonClose;

	}
}
