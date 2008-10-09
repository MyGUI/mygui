
using System;
using System.Collections.Generic;
using System.Windows.Forms;
using Common;
using Core;
using Core.Attributes;

namespace ProjectManagement
{
    [RegisterIt(CATID_STARTUP_PLUGINS.Literal, CLSID_PROJECT_MANAGER.Literal)]
    public class Manager : IPlugin, ILogSender
    {
        private ToolBar m_ToolBar;
        private IProject m_Project;

        #region IPlugin Members

        public string Name
        {
            get { return "Project manager"; }
        }

        public void Init()
        {
        }

        public ControlTypes CreateControl(ref Control _control)
        {
            if (m_ToolBar != null) return ControlTypes.None;
            m_ToolBar = new ToolBar();
            _control = m_ToolBar;

            m_ToolBar.OnNew += m_ToolBar_OnNew;
            m_ToolBar.OnOpen += m_ToolBar_OnOpen;
            m_ToolBar.OnAdd += m_ToolBar_OnAdd;
            m_ToolBar.OnClose += m_ToolBar_OnClose;
            m_ToolBar.OnSave += m_ToolBar_OnSave;
            m_ToolBar.OnSaveAs += m_ToolBar_OnSaveAs;

            return ControlTypes.ToolBar;
        }

        #endregion

        #region ILogSender Members

        public event MessageDlgt OnMessage;

        #endregion

        #region IDisposable Members

        public void Dispose()
        {
            if (m_ToolBar != null && !m_ToolBar.Disposing)
            {
                m_ToolBar.OnNew -= m_ToolBar_OnNew;
                m_ToolBar.OnOpen -= m_ToolBar_OnOpen;
                m_ToolBar.OnAdd -= m_ToolBar_OnAdd;
                m_ToolBar.OnClose -= m_ToolBar_OnClose;
                m_ToolBar.OnSave -= m_ToolBar_OnSave;
                m_ToolBar.OnSaveAs -= m_ToolBar_OnSaveAs;

                m_ToolBar.Dispose();
            }
            m_ToolBar = null;

            if(m_Project != null) m_Project.Dispose();
        }

        #endregion

        void m_ToolBar_OnSave(object sender, EventArgs e)
        {
            if(m_Project == null || !m_Project.NeedSave) return;
            string path;
            if (m_Project.ProjectLocation == string.Empty || m_Project.ProjectLocation == null)
            {
                FolderBrowserDialog frm = new FolderBrowserDialog();
                if (frm.ShowDialog() == DialogResult.Cancel) return;
                path = frm.SelectedPath;
            }
            else path = m_Project.ProjectLocation;
            m_Project.Save(path);
        }

        void m_ToolBar_OnSaveAs(object sender, EventArgs e)
        {
            if (m_Project == null || !m_Project.NeedSave) return;
            string path;
            FolderBrowserDialog frm = new FolderBrowserDialog();
            if (frm.ShowDialog() == DialogResult.Cancel) return;
            path = frm.SelectedPath;
            m_Project.Save(path);
        }

        void m_ToolBar_OnNew(object sender, EventArgs e)
        {
            if (m_Project != null)
            {
                if (MessageBox.Show("Перед тем как сздать новый проект, необходимо закрыть текущий\n Закрыть текущий проект?", "Создание",
                                MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No) return;
                m_ToolBar_OnClose(this, EventArgs.Empty);
                if (m_Project != null) return;
            }

            NewProjectForm new_pr = new NewProjectForm();

            if (new_pr.ShowDialog() == DialogResult.OK)
            {
                PreProjectWork(new_pr.SelectedProjectKeeper);

                m_Project = CoreFacade.Register.CreateItem<IProject>(
                    new_pr.SelectedProjectKeeper.ProjectID);
                m_Project.Init(new_pr.ProjectName.Text, new_pr.Prefix.Text);
                CoreFacade.Register.RegisterIt(m_Project, CATID_COMMON.ID, CLSID_CURRENT_PROJECT.ID);
            }
        }

        void m_ToolBar_OnClose(object sender, EventArgs e)
        {
            if (m_Project == null) return;
            CoreFacade.Register.UnregisterIt(CATID_COMMON.ID, CLSID_CURRENT_PROJECT.ID);
            foreach (IPlugin plugin in CoreFacade.Register.GetItems<IPlugin>(CATID_PROJECT_PLUGINS.ID))
                plugin.Dispose();
            m_Project.Dispose();
            CoreFacade.Register.GetItem<IVisualContainer>(CATID_COMMON.ID, CLSID_VISUAL_CONTAINER.ID).PostCloseProject();
            m_Project = null;
        }

        void m_ToolBar_OnAdd(object sender, EventArgs e)
        {
            if(m_Project == null)
            {
                if(MessageBox.Show("Добавление можно осуществить только в открытый проект.\n Создать?", "Добавление",
                                MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No) return;
                m_ToolBar_OnNew(this, EventArgs.Empty);
                if(m_Project == null) return;
            }

            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = CoreFacade.Register.GetItem<IProjectKeeper>(CATID_PROJECT_KEEPERS.ID, m_Project.Keeper).AddedFileFilter;
            ofd.Filter += (ofd.Filter == string.Empty ? string.Empty : "|") + "All files *.*|*.*";
            if(ofd.ShowDialog() == DialogResult.Cancel) return;
            foreach (string s in ofd.FileNames)
                m_Project.AddFromFile(s);
        }

        void m_ToolBar_OnOpen(object sender, EventArgs e)
        {
            if (m_Project != null)
            {
                if (MessageBox.Show("Перед тем как открыть новый проект, необходимо закрыть текущий\n Закрыть текущий проект?", "Открывание",
                                MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No) return;
                m_ToolBar_OnClose(this, EventArgs.Empty);
                if (m_Project != null) return;
            }

            OpenFileDialog ofd = new OpenFileDialog();
            string filter = string.Empty;
            foreach (IProjectKeeper keeper in CoreFacade.Register.GetItems<IProjectKeeper>(CATID_PROJECT_KEEPERS.ID))
                filter += keeper.ProjectFileFilter + "|";
            ofd.Filter = filter.Trim('|');
            ofd.Filter += (ofd.Filter == string.Empty ? string.Empty : "|") + "All files *.*|*.*";

            ofd.Multiselect = false;

            if (ofd.ShowDialog() == DialogResult.Cancel) return;

            Guid type;
            string name;
            string prefix;
            try
            {
                XmlProject.Load(ofd.FileName, out type, out name, out prefix);
            }
            catch(Exception _e)
            {
                if(OnMessage != null) OnMessage("Ошибка при загрузке файла '" + ofd.FileName + "'", 2, Name);
                return;
            }

            IProjectKeeper found = null;
            foreach (IProjectKeeper keeper in CoreFacade.Register.GetItems<IProjectKeeper>(CATID_PROJECT_KEEPERS.ID))
                if(keeper.ProjectID == type)
                {
                    found = keeper;
                    break;
                }

            PreProjectWork(found);

            m_Project = CoreFacade.Register.CreateItem<IProject>(type);
            m_Project.Init(name, prefix);
            CoreFacade.Register.RegisterIt(m_Project, CATID_COMMON.ID, CLSID_CURRENT_PROJECT.ID);
            m_Project.Load(ofd.FileName);
        }

        void PreProjectWork(IProjectKeeper _keeper)
        {
            IVisualContainer vc =
                CoreFacade.Register.GetItem<IVisualContainer>(CATID_COMMON.ID, CLSID_VISUAL_CONTAINER.ID);
            if(vc == null && OnMessage != null)
                OnMessage("Не найден визуальный компонент", 1, Name);

            Guid[] runablePlugins = DependencePluginGetter.GetPlugins(_keeper.ProjectID);
            
            Dictionary<Guid, IPlugin> plugins;
            CoreFacade.Register.GetItems(CATID_PROJECT_PLUGINS.ID, out plugins);

            foreach (KeyValuePair<Guid, IPlugin> pair in plugins)
            {
                //Отсеивание ненужных плагинов
                {
                    bool found = false;
                    foreach (Guid pluginID in runablePlugins)
                        if(pair.Key == pluginID)
                        {
                            found = true;
                            break;
                        }
                    if (!found) continue;
                }

                pair.Value.Init();
                if (vc == null) continue;

                Control ctrl = null;
                ControlTypes pt;

                while ((pt = pair.Value.CreateControl(ref ctrl)) != ControlTypes.None)
                    vc.InsertControl(ctrl, pt, pair.Value.Name);
            }
        }
    }
}
