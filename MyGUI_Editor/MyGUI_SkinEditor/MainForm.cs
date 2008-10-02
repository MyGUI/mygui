using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Xml;
using Common;
using Core;
using Crownwood.Magic.Common;
using Crownwood.Magic.Docking;

namespace MyGUI_SkinEditor
{
    public partial class MainForm : Form, IVisualContainer, ILogSender
    {
        private DockingManager m_DockManager;
        private Dictionary<State, WindowContent> m_DockContents = new Dictionary<State, WindowContent>();

        public MainForm()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            CoreFacade.Register.RegisterIt(this, CATID_COMMON.ID, CLSID_VISUAL_CONTAINER.ID);

            m_DockManager = new DockingManager(this, VisualStyle.IDE);
            m_DockManager.InnerControl = splitContainer3;
            
            Log("Init Core", 0, "[NONE]");
            CoreFacade.Register.OnRegister += Register_OnRegister;

            CoreFacade.SettingsProvider.Load("config.xml");

            XmlNodeList list = CoreFacade.SettingsProvider.Root.SelectNodes("include");
            foreach (XmlElement element in list)
                CoreFacade.SettingsProvider.Load(Application.StartupPath + "\\" + element.InnerText);

            list = CoreFacade.SettingsProvider.Root.SelectNodes("module");
            foreach (XmlElement element in list)
                CoreFacade.Loader.LoadAssembly(Application.StartupPath + "\\" + element.InnerText);
        }

        private void Log(string _msg, int _level, string _from)
        {
            if (OnMessage != null) OnMessage(_msg, _level, _from);
        }

        void Register_OnRegister(Guid _category, Guid _item)
        {
            Log(CoreFacade.IDTranslator.Translate(_item) + " have been registered in category " + CoreFacade.IDTranslator.Translate(_category), 0, "[NONE]");
            if(_category == CATID_STARTUP_PLUGINS.ID)
                InintPlugin(CoreFacade.Register.GetItem<IPlugin>(_category, _item));
        }

        void InintPlugin(IPlugin _plugin)
        {
            _plugin.Init();

            Control ctrl = null;
            ControlTypes pt;

            while ((pt = _plugin.CreateControl(ref ctrl)) != ControlTypes.None)
            {
                InsertControl(ctrl, pt, _plugin.Name);
            }
        }

        private void UpdateFormName()
        {
            string name = Application.ProductName;
            IProject proj = CoreFacade.Register.GetItem<IProject>(CATID_COMMON.ID, CLSID_CURRENT_PROJECT.ID);
            if (proj != null) name += " [" + proj.Name + (proj.NeedSave ? "*" : string.Empty) + "]";
            Text = name;
        }

        private void AddDockWindow(Control _control, string _label, State _align)
        {
            Content c = m_DockManager.Contents.Add(_control, _label);

            if(!m_DockContents.ContainsKey(_align))
                m_DockContents[_align] = m_DockManager.AddContentWithState(c, _align);
            else
                m_DockManager.AddContentToWindowContent(c, m_DockContents[_align]);

            _control.Tag = new Pair(m_DockContents[_align], c);
            _control.Disposed += _control_Disposed;
        }

        void _control_Disposed(object sender, EventArgs e)
        {
            WindowContent w =((sender as Control).Tag as Pair).obj1 as WindowContent;
            Content c =((sender as Control).Tag as Pair).obj2 as Content;
            if(w.Contents.Contains(c))w.Contents.Remove(c);
            c.Control.Dispose();
            if(w.Contents.Count == 0)
            {
                State s = State.Floating;
                foreach (KeyValuePair<State, WindowContent> pair in m_DockContents)
                    if (pair.Value == w) s = pair.Key;
                m_DockContents.Remove(s);
                w.Dispose();
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            UpdateFormName();
        }

        #region IVisualContainer Members

        public void InsertControl(Control _control, ControlTypes _type, object _additionInfo)
        {
            switch (_type)
            {
                case ControlTypes.ToolBar:
                    MenuContainer.TopToolStripPanel.Controls.Add(_control);
                    break;
                case ControlTypes.Common:
                    {
                        TabPage p = new TabPage(_additionInfo.ToString());
                        MainTab.TabPages.Add(p);
                        p.Controls.Add(_control);
                        _control.Dock = DockStyle.Fill;
                    }
                    break;
                case ControlTypes.DockWindowElement_Buttom:
                    {
                        AddDockWindow(_control, _additionInfo.ToString(), State.DockBottom);
                    }
                    break;
                case ControlTypes.DockWindowElement_Left:
                    {
                        AddDockWindow(_control, _additionInfo.ToString(), State.DockLeft);
                    }
                    break;
                case ControlTypes.DockWindowElement_Right:
                    {
                        AddDockWindow(_control, _additionInfo.ToString(), State.DockRight);
                    }
                    break;
                case ControlTypes.DockWindowElement_Top:
                    {
                        AddDockWindow(_control, _additionInfo.ToString(), State.DockTop);
                    }
                    break;
                case ControlTypes.DockWindowElement_Floating:
                    {
                        AddDockWindow(_control, _additionInfo.ToString(), State.Floating);
                    }
                    break;
            }
        }

        public void PostCloseProject()
        {
            MainTab.TabPages.Clear();
        }

        #endregion

        #region ILogSender Members

        public event MessageDlgt OnMessage;

        #endregion
    }

    class Pair
    {
        public Pair(object _obj1, object _obj2)
        {
            obj1 = _obj1;
            obj2 = _obj2;
        }

        public object obj1;
        public object obj2;
    }
}
