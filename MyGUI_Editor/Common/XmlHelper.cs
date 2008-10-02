using System.Xml;

namespace Common
{
    /// <summary>
    /// Вспомогательный класс для работы с XML
    /// </summary>
    public class XmlHelper
    {
        protected XmlDocument m_Document = new XmlDocument();
        protected XmlElement m_Element;

        /// <summary>
        /// XML-документ
        /// </summary>
        public XmlDocument Document
        {
            get { return m_Document; }
        }

        /// <summary>
        /// Корневой элемент
        /// </summary>
        public XmlElement MainElement
        {
            get { return m_Element; }
        }

        /// <summary>
        /// Добавить узел
        /// </summary>
        /// <param name="_node">добавляемый узел</param>
        public void AddNode(XmlNode _node)
        {
            if (_node.GetType() == typeof(XmlAttribute))
                m_Element.Attributes.Append(_node as XmlAttribute);
            else 
                m_Element.AppendChild(_node);
        }

        /// <summary>
        /// Создать узел
        /// </summary>
        /// <param name="_nodeName">имя</param>
        /// <param name="_val">значение</param>
        /// <param name="_attribute">создаваемый узел - атрибут</param>
        /// <returns></returns>
        public XmlNode CreateNode(string _nodeName, string _val, bool _attribute)
        {
            XmlNode node;
            if (_attribute)
            {
                node = m_Document.CreateAttribute(_nodeName);
                node.Value = _val;
            }
            else
            {
                node = m_Document.CreateElement(_nodeName);
                node.InnerText = _val;
            }
            return node;
        }

        /// <summary>
        /// Конструктор
        /// </summary>
        public XmlHelper()
        {
            m_Element = m_Document.CreateElement("MyGUI");
            m_Document.AppendChild(m_Element);
        }

        /// <summary>
        /// Инициализация вспомогательного объекта
        /// </summary>
        /// <param name="_typeName">тип сохраняемых элементов</param>
        public void Init(string _typeName)
        {
            XmlElement tmp = m_Element;
            XmlAttribute atr = m_Document.CreateAttribute("type");
            atr.Value = _typeName;
            tmp.Attributes.Append(atr);
            m_Element = m_Document.CreateElement(_typeName);
            tmp.AppendChild(m_Element);
        }

        /// <summary>
        /// Сохранить результаты в файл
        /// </summary>
        /// <param name="_filename">файл</param>
        public void Seve(string _filename)
        {
            Document.Save(_filename);
        }
    }
}
