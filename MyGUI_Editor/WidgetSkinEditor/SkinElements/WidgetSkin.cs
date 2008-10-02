
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Xml;
using Common;
using Core.Attributes;
using WidgetSkinEditor.SkinElements;

namespace WidgetSkinEditor.SkinElements
{
    [RegisterFactoryClass(CLSID_PROJECTELEMENT_WIDGETSKIN.Literal)]
    [RegisterFactoryClass(CLSID_PROJECTELEMENT_WIDGETSKIN_CHILD.Literal)]
    public class WidgetSkin : IProjectElement 
    {
        private string m_Name;
        private Size m_Size;
        private Point m_Location;
        private string m_Texture;
        private string m_Material;
        private WidgetAlign m_Align;
        private bool m_IsChild;

        public static int m_ChildLevel;

        private readonly List<Property> m_Properties = new List<Property>();
        private readonly List<WidgetSkin> m_Childes = new List<WidgetSkin>();
        private readonly List<BasisSkin> m_BSkins = new List<BasisSkin>();

        public List<Property> Properties
        {
            get { return m_Properties; }
        }

        public List<WidgetSkin> Childes
        {
            get { return m_Childes; }
        }

        public List<BasisSkin> BasisSkins
        {
            get { return m_BSkins; }
        }

        public string Name
        {
            get { return m_Name; }
            set { m_Name = value; }
        }

        public Size Size
        {
            get { return m_Size; }
            set { m_Size = value; }
        }

        public string Texture
        {
            get { return m_Texture; }
            set { m_Texture = value; }
        }

        public string Material
        {
            get { return m_Material; }
            set { m_Material = value; }
        }

        public WidgetAlign Align
        {
            get { return m_Align; }
            set { m_Align = value; }
        }

        public Point Location
        {
            get { return m_Location; }
            set { m_Location = value; }
        }

        public override string ToString()
        {
            return "Skin{ Name:'" + Name + "'}";
        }

        #region IProjectElement Members

        public bool FromXML(XmlElement _element)
        {
            if (_element.Attributes["name"] == null)
            {
                if (m_ChildLevel == 0) throw new Exception("Не найдено имя скина");
            }
            else m_Name = _element.Attributes["name"].Value;

            if (_element.Attributes["size"] == null && _element.Attributes["offset"] == null) throw new Exception("Не найден размер скина");
            else
            {
                bool size_ex = _element.Attributes["size"] != null;
                try
                {
                    string[] sz = _element.Attributes[size_ex ? "size" : "offset"].Value.Split(' ');
                    m_Location = size_ex ? new Point() : new Point(Convert.ToInt32(sz[0]), Convert.ToInt32(sz[1]));
                    m_Size = new Size(Convert.ToInt32(sz[size_ex ? 0 : 2]), Convert.ToInt32(sz[size_ex ? 1 : 2]));
                }
                catch (Exception _e)
                {
                    throw new Exception("Неверный формат " + (size_ex ? "размера" : "координат") + " скина: '" +
                                        _element.Attributes[size_ex ? "size" : "offset"].Value + "'", _e);
                }
            }

            if (_element.Attributes["texture"] != null) m_Texture = _element.Attributes["texture"].Value.Trim();
            if (_element.Attributes["material"] != null) m_Material = _element.Attributes["material"].Value.Trim();
            if (_element.Attributes["align"] != null) m_Align = (WidgetAlign)Converter.StringToAlign(_element.Attributes["align"].Value);

            //Properties

            XmlNodeList list = _element.SelectNodes("Property");
            foreach (XmlElement element in list)
            {
                Property p = new Property();

                try { p.FromXML(element); }
                catch (Exception _e) { throw new Exception(p + ": " + _e.Message, _e); }

                m_Properties.Add(p);
            }

            //Childes

            list = _element.SelectNodes("Child");
            m_ChildLevel++;
            try
            {
                foreach (XmlElement element in list)
                {
                    WidgetSkin s = new WidgetSkin();

                    try
                    {
                        s.FromXML(element);
                    }
                    catch (Exception _e)
                    {
                        throw new Exception(s + ": " + _e.Message, _e);
                    }

                    m_Childes.Add(s);
                }
            }
            finally
            {
                m_ChildLevel--;
            }

            m_IsChild = m_ChildLevel != 0;

            //BasisSkin

            list = _element.SelectNodes("BasisSkin");
            foreach (XmlElement element in list)
            {
                BasisSkin s = new BasisSkin();

                try { s.FromXML(element); }
                catch (Exception _e) { throw new Exception(s + ": " + _e.Message, _e); }

                m_BSkins.Add(s);
            }

            return true;
        }

        public XmlElement ToXML(XmlDocument _doc)
        {
            XmlElement root = _doc.CreateElement(m_IsChild ? "Child" : "Skin");
            root.Attributes.Append(_doc.CreateAttribute("name")).Value = Name;
            root.Attributes.Append(_doc.CreateAttribute(m_IsChild ? "offset" : "size")).Value = m_IsChild ?
                (StringConverter.LocationToString(Location) + " " + StringConverter.SizeToString(Size)) : StringConverter.SizeToString(Size);
            if (Texture != null && Texture != string.Empty) root.Attributes.Append(_doc.CreateAttribute("texture")).Value = Texture;
            if ((Align)Align != Common.Align.DEFAULT)
                root.Attributes.Append(_doc.CreateAttribute("align")).Value = Converter.AlignToString((Align)Align);
            if (Material != null && Material != string.Empty) root.Attributes.Append(_doc.CreateAttribute("material")).Value = m_Material;

            foreach (Property property in m_Properties)
                root.AppendChild(property.ToXML(_doc));
            foreach (WidgetSkin skin in m_Childes)
                root.AppendChild(skin.ToXML(_doc));
            foreach (BasisSkin skin in m_BSkins)
                root.AppendChild(skin.ToXML(_doc));
            return root;
        }

        #endregion

        #region IProjectElement Members

        public Guid Type
        {
            get { return CLSID_PROJECTELEMENT_WIDGETSKIN.ID; }
        }

        #endregion
    }
}