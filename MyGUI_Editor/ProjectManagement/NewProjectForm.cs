using System;
using System.Windows.Forms;
using Common;
using Core;

namespace ProjectManagement
{
    public partial class NewProjectForm : Form
    {
        public NewProjectForm()
        {
            InitializeComponent();
        }

        private void NewProjectForm_Resize(object sender, EventArgs e)
        {
            int cw = ProjectTypesList.ClientSize.Width / ProjectTypesList.Columns.Count;
            foreach (ColumnHeader header in ProjectTypesList.Columns)
                header.Width = cw;
        }

        public IProjectKeeper SelectedProjectKeeper
        {
            get
            {
                if (ProjectTypesList.SelectedItems.Count == 0) return null;
                return (IProjectKeeper)ProjectTypesList.SelectedItems[0].Tag;
            }
        }

        private void NewProjectForm_Load(object sender, EventArgs e)
        {
            foreach (IProjectKeeper item in CoreFacade.Register.GetItems<IProjectKeeper>(CATID_PROJECT_KEEPERS.ID))
            {
                ListViewItem li = ProjectTypesList.Items.Add(item.Name);
                li.Tag = item;
            }

            NewProjectForm_Resize(this, e);
            if(ProjectTypesList.Items.Count != 0)
                ProjectTypesList.SelectedIndices.Add(0);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(ProjectName.Text.Trim() == string.Empty || SelectedProjectKeeper == null)
            {
                MessageBox.Show("Project name is obligatory parameter!", "Error", MessageBoxButtons.OK,
                                MessageBoxIcon.Error);
                return;
            }

            DialogResult = DialogResult.OK;
            Close();
        }

        private void ProjectTypesList_ItemActivate(object sender, EventArgs e)
        {
            button1_Click(this, e);
        }
    }
}