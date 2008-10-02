using System.Windows.Forms;

namespace MyGUI_SkinEditor
{
    partial class MainForm
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.OpenProjectDialog = new System.Windows.Forms.OpenFileDialog();
            this.splitContainer3 = new System.Windows.Forms.SplitContainer();
            this.MenuContainer = new System.Windows.Forms.ToolStripContainer();
            this.MainTab = new System.Windows.Forms.TabControl();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.OpenFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.splitContainer3.Panel1.SuspendLayout();
            this.splitContainer3.Panel2.SuspendLayout();
            this.splitContainer3.SuspendLayout();
            this.MenuContainer.SuspendLayout();
            this.SuspendLayout();
            // 
            // OpenProjectDialog
            // 
            this.OpenProjectDialog.Filter = "MyGUI skin projects (*.mgsp) | *mgsp";
            // 
            // splitContainer3
            // 
            this.splitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer3.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer3.IsSplitterFixed = true;
            this.splitContainer3.Location = new System.Drawing.Point(0, 0);
            this.splitContainer3.Name = "splitContainer3";
            this.splitContainer3.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer3.Panel1
            // 
            this.splitContainer3.Panel1.Controls.Add(this.MenuContainer);
            // 
            // splitContainer3.Panel2
            // 
            this.splitContainer3.Panel2.Controls.Add(this.MainTab);
            this.splitContainer3.Size = new System.Drawing.Size(792, 573);
            this.splitContainer3.SplitterDistance = 29;
            this.splitContainer3.TabIndex = 1;
            // 
            // MenuContainer
            // 
            this.MenuContainer.BottomToolStripPanelVisible = false;
            // 
            // MenuContainer.ContentPanel
            // 
            this.MenuContainer.ContentPanel.Size = new System.Drawing.Size(792, 4);
            this.MenuContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MenuContainer.LeftToolStripPanelVisible = false;
            this.MenuContainer.Location = new System.Drawing.Point(0, 0);
            this.MenuContainer.Name = "MenuContainer";
            this.MenuContainer.RightToolStripPanelVisible = false;
            this.MenuContainer.Size = new System.Drawing.Size(792, 29);
            this.MenuContainer.TabIndex = 4;
            this.MenuContainer.Text = "toolStripContainer1";
            // 
            // MainTab
            // 
            this.MainTab.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainTab.Location = new System.Drawing.Point(0, 0);
            this.MainTab.Name = "MainTab";
            this.MainTab.SelectedIndex = 0;
            this.MainTab.Size = new System.Drawing.Size(792, 540);
            this.MainTab.TabIndex = 0;
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // OpenFileDialog
            // 
            this.OpenFileDialog.Filter = "MyGUI widget skins (*.skin)|*.skin|MyGUI font (*.font)|*.font|All files(*.*)|*.*";
            this.OpenFileDialog.Multiselect = true;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(792, 573);
            this.Controls.Add(this.splitContainer3);
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "MyGUI SkinEditor";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.splitContainer3.Panel1.ResumeLayout(false);
            this.splitContainer3.Panel2.ResumeLayout(false);
            this.splitContainer3.ResumeLayout(false);
            this.MenuContainer.ResumeLayout(false);
            this.MenuContainer.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private OpenFileDialog OpenProjectDialog;
        private SplitContainer splitContainer3;
        private TabControl MainTab;
        private Timer timer1;
        private OpenFileDialog OpenFileDialog;
        private ToolStripContainer MenuContainer;













    }
}

