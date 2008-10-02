using System;
using System.Windows.Forms;

namespace WidgetSkinEditor
{
    public partial class VSkinEditor : UserControl
    {
        public VSkinEditor()
        {
            InitializeComponent();
        }

        private void MainForm_Resize(object sender, EventArgs e)
        {
            int cw = SkinList.ClientSize.Width/SkinList.Columns.Count;
            foreach (ColumnHeader header in SkinList.Columns)
                header.Width = cw;
        }

        private void splitContainer1_Panel1_Resize(object sender, EventArgs e)
        {
            if(SkinList.Columns.Count == 0) return;
            int w = SkinList.ClientSize.Width / SkinList.Columns.Count;
            foreach (ColumnHeader header in SkinList.Columns)
                header.Width = w;
        }
    }
}
