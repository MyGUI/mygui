using System;
using System.Xml;
using Core;

namespace Common
{
    public static class DependencePluginGetter
    {
        public static Guid[] GetPlugins(Guid _project)
        {
            XmlNodeList list = CoreFacade.SettingsProvider.Root.SelectNodes(
                    string.Format("project[@ID=\"{0}\" or @ID=\"{1}\"]/plugin", _project, _project.ToString().ToUpper()));

            Guid[] guids;
            if(list == null) guids = new Guid[0];
            else
            {
                guids = new Guid[list.Count];
                for(int i = 0; i < list.Count; i++)
                    guids[i] = new Guid(list[i].InnerText);                    
            }
            return guids;
        }
    }
}
