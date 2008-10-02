
using System;
using System.Drawing;
using System.Xml;
using Common;
using Core.Attributes;

namespace WidgetSkinEditor.SkinElements
{
    [RegisterFactoryClass(CLSID_PROJELEMENT_BASISSKIN_STATE.Literal)]
    public class BasisSkinState : IProjectElement 
    {
        private string m_Name;
        private Size m_Size;
        private Point m_Location;
        private Color m_TextColor;
        #region IProjectElement Members

        public string Name
        {
            get { return m_Name; }
        }

        public Guid Type
        {
            get { return CLSID_PROJELEMENT_BASISSKIN_STATE.ID; }
        }

        public Size Size
        {
            get { return m_Size; }
            set { m_Size = value; }
        }

        public Point Location
        {
            get { return m_Location; }
            set { m_Location = value; }
        }

        public Color TextColor
        {
            get { return m_TextColor; }
            set { m_TextColor = value; }
        }

        public bool FromXML(XmlElement _element)
        {
            if (_element.Attributes["name"] == null) throw new Exception("Не найдено имя состояния");
            if ((_element.Attributes["offset"] == null) && (_element.Attributes["colour"] == null)) 
                throw new Exception("Не найдено значение состояния");

            m_Name = _element.Attributes["name"].Value;

            try
            {
                if (_element.Attributes["offset"] != null)
                {
                    string[] ofst = _element.Attributes["offset"].Value.Split(' ');
                    m_Location = new Point(Convert.ToInt32(ofst[0]), Convert.ToInt32(ofst[1]));
                    m_Size = new Size(Convert.ToInt32(ofst[2]), Convert.ToInt32(ofst[3]));
                }
                else
                {
                    if (_element.Attributes["colour"] != null)
                    {
                        TextColor = StringConverter.StringToColor(_element.Attributes["colour"].Value);
                    }
                }
            }
            catch(Exception _e)
            {
                throw new Exception("Состояние скина '" + Name + "': " + _e.Message, _e);
            }

            return true;
        }

        public XmlElement ToXML(XmlDocument _doc)
        {
            XmlElement root = _doc.CreateElement("State");
            root.Attributes.Append(_doc.CreateAttribute("name")).Value = Name;
            if(m_TextColor.IsEmpty)
                root.Attributes.Append(_doc.CreateAttribute("offset")).Value = StringConverter.LocationToString(m_Location) + " " + StringConverter.SizeToString(m_Size);
            else
                root.Attributes.Append(_doc.CreateAttribute("colour")).Value = StringConverter.ColorToString(m_TextColor);
            return root;
        }

        #endregion
    }
}
