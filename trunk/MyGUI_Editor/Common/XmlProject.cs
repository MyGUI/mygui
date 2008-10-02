using System;
using System.Xml;

namespace Common
{
    /// <summary>
    /// Впомогательный класс для работы с XML-представлениями проектов
    /// </summary>
    public class XmlProject : XmlHelper
    {
        public static void Load(string _filename, out Guid _type, out string _name, out string _prefix)
        {
            XmlDocument doc = new XmlDocument();
            doc.Load(_filename);

            XmlElement el = doc.SelectSingleNode("MyGUI/Project") as XmlElement;
            _type = new Guid(el["Type"].InnerText);
            _name = el["Name"].InnerText;
            _prefix = el["Prefix"].InnerText;

        }

        public void Init(IProject _project, Guid _projectID)
        {
            Init("Project");
            
            AddNode(CreateNode("Type", _projectID.ToString(), false));
            AddNode(CreateNode("Name", _project.Name, false));
            AddNode(CreateNode("Prefix", _project.Prefix, false));
        }


    }
}
            