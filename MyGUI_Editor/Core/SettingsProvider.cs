
using System.Xml;

namespace Core
{
    /// <summary>
    /// Хранитель настроек
    /// </summary>
    public class SettingsProvider
    {
        private XmlElement m_Root;
        private XmlDocument m_Doc;

        internal SettingsProvider()
        {
            m_Doc = new XmlDocument();
            m_Root = m_Doc.CreateElement("ROOT");
            m_Doc.AppendChild(m_Root);
        }

        /// <summary>
        /// Загрузить файл настроек
        /// </summary>
        /// <param name="_filename">файл настроек в формате XML</param>
        public void Load(string _filename)
        {
            XmlDocument doc = new XmlDocument();
            doc.Load(_filename);
            XmlElement root = doc.SelectSingleNode("ROOT") as XmlElement;
            if(root == null) return;
            foreach (XmlElement element in root)
                m_Root.AppendChild(m_Doc.ImportNode(element, true));
        }

        /// <summary>
        /// Корневой узел настроек в формате XML
        /// </summary>
        public XmlElement Root
        {
            get { return m_Root; }
        }
    }
}
