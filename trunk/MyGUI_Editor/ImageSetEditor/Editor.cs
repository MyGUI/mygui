
using System.Windows.Forms;
using Common;
using Core.Attributes;
using ImageSetEditor;

namespace ImageSetEditor
{
    [RegisterIt(CATID_PROJECT_PLUGINS.Literal, CLSID_IMAGE_SET_EDITOR.Literal)]
    public class Editor : IPlugin, ILogSender
    {
        private VISEditor m_Control;

        #region IProjectPart Members

        public string Name
        {
            get { return "VIS-editor"; }
        }

        public bool Visual
        {
            get { return true; }
        }

        #endregion

        #region ILogSender Members

        public event MessageDlgt OnMessage;

        #endregion

        #region IProjectPart Members


        public void Dispose()
        {
            if(m_Control != null) m_Control.Dispose();
            m_Control = null;
        }

        public void Init()
        {

        }

        #endregion

        #region IPlugin Members


        public ControlTypes CreateControl(ref Control _control)
        {
            if (m_Control != null) return ControlTypes.None;

            m_Control = new VISEditor();
            _control = m_Control;

            return ControlTypes.Common;
        }

        #endregion
    }
}