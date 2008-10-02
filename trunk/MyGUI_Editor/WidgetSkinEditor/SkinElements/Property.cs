using System;
using System.Xml;
using Common;
using Core.Attributes;

namespace WidgetSkinEditor.SkinElements
{
    [RegisterFactoryClass(CLSID_PROJELEMENT_PROPERTY.Literal)]
    public class Property : IProjectElement
    {
        private string key;
        private string value;

        public string Value
        {
            get { return value; }
            set { this.value = value; }
        }

        public string Key
        {
            get { return key; }
            set { key = value; }
        }

        public override string ToString()
        {
            return "Property {Type:'" + Type + "'; Value:'" + Value + "'}";
        }

        #region IProjectElement Members

        public bool FromXML(XmlElement _element)
        {
            if (_element.Attributes["key"] == null) throw new Exception("Не найден тип свойства");
            else key = _element.Attributes["key"].Value;
            if (_element.Attributes["value"] == null) throw new Exception("Не найдено значение свойства");
            else value = _element.Attributes["value"].Value;
            return true;
        }

        public XmlElement ToXML(XmlDocument _doc)
        {
            XmlElement root = _doc.CreateElement("Property");

            root.Attributes.Append(_doc.CreateAttribute("key")).Value = key;
            root.Attributes.Append(_doc.CreateAttribute("value")).Value = value;

            return root;
        }

        #endregion

        #region IProjectElement Members

        public string Name
        {
            get { return ToString(); }
        }

        public Guid Type
        {
            get { return CLSID_PROJELEMENT_PROPERTY.ID; }
        }

        #endregion
    }
}