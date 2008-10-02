using System;
using System.Windows.Forms;
using Common;
using Core;
using Core.Attributes;

namespace WidgetSkinEditor
{
    [RegisterIt(CATID_PROJECT_PLUGINS.Literal, CLSID_WIDGET_SKIN_EDITOR.Literal)]
    public class Editor : IPlugin, ILogSender
    {
        private VSkinEditor m_Control;

        #region IProectPart Members

        public string Name
        {
            get { return "VWS-Editor"; }
        }

        #endregion

        #region ILogSender Members

        public event MessageDlgt OnMessage;

        #endregion

        #region IProjectPart Members


        public void Dispose()
        {
            CoreFacade.Register.OnRegister -= Register_OnRegister;
            CoreFacade.Register.OnUnRegister -= Register_OnUnRegister;
            if(m_Control != null)
            {
                m_Control.SkinList.SelectedIndexChanged -= SkinList_SelectedIndexChanged;
                m_Control.Dispose();
            }
            m_Control = null;
        }

        public void Init()
        {
            CoreFacade.Register.OnRegister += Register_OnRegister;
            CoreFacade.Register.OnUnRegister += Register_OnUnRegister;
        }

        #endregion

        #region IPlugin Members

        public ControlTypes CreateControl(ref Control _control)
        {
            if (m_Control != null) return ControlTypes.None;
            
            m_Control = new VSkinEditor();
            _control = m_Control;

            m_Control.SkinList.SelectedIndexChanged += SkinList_SelectedIndexChanged;

            return ControlTypes.Common;
        }

        #endregion

        void SkinList_SelectedIndexChanged(object sender, EventArgs e)
        {
            IProject p = CoreFacade.Register.GetItem<IProject>(CATID_COMMON.ID, CLSID_CURRENT_PROJECT.ID);
            if(p == null) return;
            p.CurrentElement = m_Control.SkinList.SelectedIndices.Count == 0
                                   ? null : m_Control.SkinList.SelectedItems[0].Tag as IProjectElement;
        }

        void Register_OnRegister(Guid _category, Guid _item)
        {
            if(_category == CATID_COMMON.ID && _item == CLSID_CURRENT_PROJECT.ID)
            {
                IProject p = CoreFacade.Register.GetItem<IProject>(_category, _item);
                p.OnUpdateElementList += p_OnUpdateElementList;
            }
        }

        void Register_OnUnRegister(Guid _category, Guid _item)
        {
            if (_category == CATID_COMMON.ID && _item == CLSID_CURRENT_PROJECT.ID)
            {
                IProject p = CoreFacade.Register.GetItem<IProject>(_category, _item);
                p.OnUpdateElementList -= p_OnUpdateElementList;
            }
        }

        void p_OnUpdateElementList(object sender, EventArgs e)
        {
            m_Control.SkinList.Items.Clear();
            IProject p = sender as IProject;
            foreach (IProjectElement element in p.GetElements(CLSID_PROJECTELEMENT_WIDGETSKIN.ID))
            {
                string name;
                if(p.Prefix == string.Empty) name = element.Name;
                else
                {
                    if(element.Name.Contains(p.Prefix))
                        name = element.Name.Remove(element.Name.IndexOf(p.Prefix), p.Prefix.Length);
                    else
                    {
                        name = element.Name;
                        if (OnMessage != null)
                            OnMessage("Элемент с имнем '" + name + "' не содержит в имнеи префикса проекта '" + p.Prefix + "'", 1, Name);
                    }
                }
                m_Control.SkinList.Items.Add(name).Tag = element;
            }
        }
    }
}
