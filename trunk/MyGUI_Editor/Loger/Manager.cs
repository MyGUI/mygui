
using System;
using System.IO;
using System.Windows.Forms;
using Common;
using Core;
using Core.Attributes;

namespace Loger
{
    [RegisterIt(CATID_STARTUP_PLUGINS.Literal, CLSID_LOGER.Literal)]
    public class Manager : IPlugin
    {
        #region IPlugin Members

        public string Name
        {
            get { return "Loger"; }
        }

        public void Init()
        {
            CoreFacade.Register.OnRegister += Register_OnRegister;
            CoreFacade.Register.OnUnRegister += Register_OnUnRegister;
        }

        public ControlTypes CreateControl(ref Control _control)
        {
            return ControlTypes.None;
        }

        #endregion

        #region IDisposable Members

        public void Dispose()
        {
            
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
            if(ls == null) return;
            ls.OnMessage += ls_OnMessage;
        }

        void ls_OnMessage(string _msg, int _level, string _fromName)
        {
            StreamWriter sw = new StreamWriter(Application.StartupPath + "\\LogOut.log", true);
            string level;
            switch(_level)
            {
                case 0: level = "Info"; break;
                case 1: level = "Warning"; break;
                case 2: level = "Error"; break;
                default: level = "Message level(" + _level + ")"; break;
            }
            sw.Write("{0} {1}({2}): {3}\r\n", DateTime.Now.ToString("hh:mm:ss"), _fromName, level, _msg);
            sw.Close();
        }
    }

    public struct CLSID_LOGER
    {
        public const string Literal = "adf20c60-a900-4eac-9c2e-7e0c8b755c72";
        public static Guid ID = new Guid(Literal);
    }
}
