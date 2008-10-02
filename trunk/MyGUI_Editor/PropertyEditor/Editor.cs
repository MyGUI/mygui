using System;
using System.Windows.Forms;
using Common;
using Core;
using Core.Attributes;

namespace PropertyEditor
{
    [RegisterIt(CATID_PROJECT_PLUGINS.Literal, CLSID_PROPERTY_EDITOR.Literal)]
    public class Editor : IPlugin
    {
        private PropertyWindow m_Window;

        #region IPlugin Members

        public string Name
        {
            get { return "PropertyEditor plugin"; }
        }

        public void Init()
        {
            CoreFacade.Register.OnRegister += Register_OnRegister;
            CoreFacade.Register.OnUnRegister += Register_OnUnRegister;
        }

        public ControlTypes CreateControl(ref Control _control)
        {
            if (m_Window != null) return ControlTypes.None;
            m_Window = new PropertyWindow();
            _control = m_Window;
            return ControlTypes.DockWindowElement_Right;
        }

        #endregion

        #region IDisposable Members

        public void Dispose()
        {
            CoreFacade.Register.OnRegister -= Register_OnRegister;
            CoreFacade.Register.OnUnRegister -= Register_OnUnRegister;

            if(m_Window != null) m_Window.Dispose();
            m_Window = null;
        }

        #endregion

        void Register_OnUnRegister(Guid _category, Guid _item)
        {
            if (_category == CATID_COMMON.ID && _item == CLSID_CURRENT_PROJECT.ID)
            {
                IProject p = CoreFacade.Register.GetItem<IProject>(_category, _item);
                p.OnChangeCurElement -= p_OnChangeCurElement;
            }
        }

        void Register_OnRegister(Guid _category, Guid _item)
        {
            if (_category == CATID_COMMON.ID && _item == CLSID_CURRENT_PROJECT.ID)
            {
                IProject p = CoreFacade.Register.GetItem<IProject>(_category, _item);
                p.OnChangeCurElement += p_OnChangeCurElement;
            }
        }

        void p_OnChangeCurElement(object sender, EventArgs e)
        {
            if (m_Window != null) m_Window.SelectedObject = (sender as IProject).CurrentElement;
        }
    }

    public struct CLSID_PROPERTY_EDITOR
    {
        public const string Literal = "6be840b6-9b0f-43d5-b40d-25871a62e16e";
        public static Guid ID = new Guid(Literal);
    }
}
