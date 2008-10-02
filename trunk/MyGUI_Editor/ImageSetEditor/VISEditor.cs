using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using Common;
using ViewChipsInterfaces;

namespace ImageSetEditor
{
    public partial class VISEditor : UserControl
    {
        #region Properties

        private Dictionary<string, TabPage> m_Pages = new Dictionary<string, TabPage>();

        readonly int m_Zoomk = 2;
        readonly Size m_NewChipSize = new Size(50, 50);
        private readonly string m_MainItemName = "[MANE]";

        #endregion

        #region Costructors

        public VISEditor()
        {
            InitializeComponent();
        }

        #endregion

        #region Handlers

        private void VISEditor_Resize(object sender, EventArgs e)
        {
            int cw = ChipList.ClientSize.Width / ChipList.Columns.Count;
            foreach (ColumnHeader header in ChipList.Columns)
                header.Width = cw;

            ChipList.Height = splitContainer1.Panel1.Height - 28;
        }

        private void splitContainer1_Panel1_Resize(object sender, EventArgs e)
        {
            if (ChipList.Columns.Count == 0) return;
            int w = ChipList.ClientSize.Width / ChipList.Columns.Count;
            foreach (ColumnHeader header in ChipList.Columns)
                header.Width = w;
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            OpenImageDoc();
        }

        private void toolStripButton6_Click(object sender, EventArgs e)
        {
            if (CurDoc != null) ZoomDoc(CurDoc, true);
        }

        private void toolStripButton7_Click(object sender, EventArgs e)
        {
            if (CurDoc != null) ZoomDoc(CurDoc, false);
        }

        private void toolStripButton8_Click(object sender, EventArgs e)
        {
            if (CurDoc != null) NormalizeDoc(CurDoc);
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            if(CurDoc== null) return;
            CreateChip();
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            if (CurDoc == null) return;
            RemoveDoc(CurDoc);
        }

        #endregion

        #region Inner functions

        Doc CurDoc
        {
            get
            {
                return Tab.SelectedTab == null ? null : (Doc)Tab.SelectedTab.Tag;
            }
        }

        void CreateChip()
        {
            if (CurDoc == null) return;
            ChipList.Items.Add("NewChip").BeginEdit();
        }

        void ZoomDoc(Doc _doc, bool _in)
        {
            IItem item = _doc.ViewArea[m_MainItemName];
            if (_in) item.ScaleKoof++;
            else if (item.ScaleKoof != 1) item.ScaleKoof--;
            item.Invalidate();
        }

        private void NormalizeDoc(Doc _doc)
        {
            IItem item = _doc.ViewArea[m_MainItemName];
            item.ScaleKoof = 1;
            item.MoveToCenter();
            item.Invalidate();
        }

        private void InjectImage(string _filename)
        {
            Doc document = new Doc();
            string name = _filename.Contains("\\") ? _filename.Substring(_filename.LastIndexOf('\\') + 1) : _filename;
            name = name.Contains(".") ? name.Remove(name.LastIndexOf('.')) : name;
            if (m_Pages.ContainsKey(name))
            {
                MessageBox.Show("Это изображение уже открыто", "Внимание!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            document.Page = new TabPage(name);
            document.Page.Tag = document;
            document.ViewArea = Core.CoreFacade.Register.CreateItem<IArea>(CLSID_VIEWCHIP_AREA.ID);
            document.ViewArea.AutoScroll = true;
            document.Page.Controls.Add(document.ViewArea as Control);
            document.ViewArea.Dock = DockStyle.Fill;
            document.ViewArea.AddItem(m_MainItemName, Image.FromFile(_filename), Align.DEFAULT, null);
            document.ViewArea[m_MainItemName].ResizeToImage();
            document.Page.AutoScroll = true;
            //document.ViewArea[m_MainItemName].SetSelector(CLSID_DEFAULT_ITEMCONTROLLER.ID);
            Tab.TabPages.Add(document.Page);
            m_Pages.Add(name, document.Page);
            
            Tab.SelectedIndex = Tab.TabPages.Count - 1;
            NormalizeDoc(document);
            document.Page.Refresh();
        }

        void RemoveDoc(Doc _doc)
        {
            _doc.ViewArea.Dispose();
            m_Pages.Remove(_doc.Page.Text);
            Tab.TabPages.Remove(_doc.Page);
        }

        private void OpenImageDoc()
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "All supported files|*.bmp;*.wmf;*.ico;*.jpeg;*.jpg;*.gif;*.png";
            ofd.Multiselect = true;
            if (ofd.ShowDialog() == DialogResult.Cancel) return;
            foreach (string fileName in ofd.FileNames)
                InjectImage(fileName);
        }

        #endregion

        private void ChipList_AfterLabelEdit(object sender, LabelEditEventArgs e)
        {
            if(CurDoc == null) return;
            if(e.Label == m_MainItemName)
            {
                MessageBox.Show("Это зарезервированное имя!", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if(ChipList.Items[e.Item].Tag == null)
            {
                IItem itm = CurDoc.ViewArea[m_MainItemName].AddItem(ChipList.Items[e.Item].Text, null, Align.DEFAULT, ChipList.Items[e.Item]);
                ChipList.Items[e.Item].Tag = itm;

                itm.SetSelector(CLSID_DEFAULT_ITEMCONTROLLER.ID);

                Size viewSize = new Size(Math.Min(CurDoc.Page.Size.Width, CurDoc.ViewArea[m_MainItemName].Size.Width),
                                         Math.Min(CurDoc.Page.Size.Height, CurDoc.ViewArea[m_MainItemName].Size.Height));

                itm.Rect = new Rectangle(new Point((viewSize.Width - m_NewChipSize.Width) / 2,
                                         (viewSize.Width - m_NewChipSize.Height) / 2), 
                                         m_NewChipSize);
            }
            else
            {
                IItem itm = ChipList.Items[e.Item].Tag as IItem;
                itm.Name = ChipList.Items[e.Item].Text;
            }
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            foreach (ListViewItem item in ChipList.SelectedItems)
            {
                IItem itm = item.Tag as IItem;
                itm.Dispose();
                item.Remove();
            }
        }
    }

    class Doc
    {
        private TabPage m_Page;
        private IArea m_ViewArea;

        public TabPage Page
        {
            get { return m_Page; }
            set { m_Page = value; }
        }

        public IArea ViewArea
        {
            get { return m_ViewArea; }
            set { m_ViewArea = value; }
        }
    }
}