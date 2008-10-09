
using System;
using System.IO;
using System.Windows.Forms;
using Common;
using Core.Attributes;
using ResourceAnimationProject;

namespace ResourceAnimationProject
{
    [RegisterIt(CATID_PROJECT_KEEPERS.Literal, CLSID_RESOURCE_ANIMATION_PROJECT_KEEPER.Literal)]
    public class Keeper : IProjectKeeper, IPlugin, ILogSender
    {
        #region IProjectKeeper Members

        public Guid ProjectID
        {
            get { return CLSID_RESOURCE_ANIMATION_PROJECT.ID; } 
        }

        public string Name
        {
            get { return "MyGUI_ResourceAnimation project"; }
        }

        public string ProjectFileFilter
        {
            get { return "MyGUI Resource Animation files(*.mgsproj)|*.mgsproj"; }
        }

        public string AddedFileFilter
        {
            get { return string.Empty; }
        }

        #endregion

        #region IPlugin Members


        public void Init()
        {
            
        }

        public ControlTypes CreateControl(ref Control _control)
        {
            return ControlTypes.None;
        }

        #endregion

        #region ILogSender Members

        public event MessageDlgt OnMessage;

        #endregion

        #region IDisposable Members

        public void Dispose()
        {

        }

        #endregion
    }
}