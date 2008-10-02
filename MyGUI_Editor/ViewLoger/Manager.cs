using System;
using System.Windows.Forms;
using Common;
using Core;
using Core.Attributes;

namespace ViewLoger
{
    [RegisterIt(CATID_STARTUP_PLUGINS.Literal, CLSID_VIEWLOGER.Literal)]
    public class Manager : IPlugin
    {
        private LogWindow m_Window;

        #region IPlugin Members

        public string Name
        {
            get { return "ViewLog"; }
        }

        public void Init()
        {
            CoreFacade.Register.OnRegister += Register_OnRegister;
            CoreFacade.Register.OnUnRegister += Register_OnUnRegister;
        }

        public ControlTypes CreateControl(ref Control _control)
        {
            if (m_Window != null) return ControlTypes.None;
            m_Window = new LogWindow();
            _control = m_Window;
            return ControlTypes.DockWindowElement_Buttom;
        }

        #endregion

        #region IDisposable Members

        public void Dispose()
        {
            if(m_Window != null) m_Window.Dispose();
            m_Window = null;
        }

        #endregion

        void Register_OnUnRegister(Guid _category, Guid _item)
        {
            ILogSender ls = CoreFacade.Register.GetItem<ILogSender>(_category, _item);
            if (ls == null) return;
            ls.OnMessage -= ls_OnMessage;
        }

        void Register_OnRegister(Guid _category, Guid _item)
        {
            ILogSender ls = CoreFacade.Register.GetItem<ILogSender>(_category, _item);
            if (ls == null) return;
            ls.OnMessage += ls_OnMessage;
        }

        void ls_OnMessage(string _msg, int _level, string _fromName)
        {
            if(m_Window != null) m_Window.AddNote(_level, _fromName, _msg);
        }
    }

    public struct CLSID_VIEWLOGER
    {
        public const string Literal = "f2128e6b-ecb9-44cc-bddc-c6726dc3825b";
        public static Guid ID = new Guid(Literal);
    }
}
