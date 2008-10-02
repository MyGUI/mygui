using System;
using System.Windows.Forms;

namespace ProjectManagement
{
    public partial class ToolBar : ToolStrip 
    {
        public event EventHandler OnNew;
        public event EventHandler OnOpen;
        public event EventHandler OnAdd;
        public event EventHandler OnClose;
        public event EventHandler OnSave;
        public event EventHandler OnSaveAs;

        public ToolBar()
        {
            InitializeComponent();

            toolStripButton1.Click += toolStripButton1_Click;
            toolStripButton2.Click += toolStripButton2_Click;
            toolStripButton3.Click += toolStripButton3_Click;
            toolStripButton4.Click += toolStripButton4_Click;
            toolStripButton5.Click += toolStripButton5_Click;
            toolStripButton6.Click += toolStripButton6_Click;
        }

        void toolStripButton6_Click(object sender, EventArgs e)
        {
            if (OnClose != null) OnSaveAs(this, e);
        }

        void toolStripButton4_Click(object sender, EventArgs e)
        {
            if (OnClose != null) OnClose(this, e);
        }

        void toolStripButton5_Click(object sender, EventArgs e)
        {
            if (OnSave != null) OnSave(this, e);
        }

        void toolStripButton3_Click(object sender, EventArgs e)
        {
            if (OnAdd != null) OnAdd(this, e);
        }

        void toolStripButton2_Click(object sender, EventArgs e)
        {
            if (OnOpen != null) OnOpen(this, e);
        }

        void toolStripButton1_Click(object sender, EventArgs e)
        {
            if (OnNew != null) OnNew(this, e);
        }        
    }
}