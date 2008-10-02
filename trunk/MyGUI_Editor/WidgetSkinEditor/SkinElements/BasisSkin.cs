
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Xml;
using Common;
using Core.Attributes;

namespace WidgetSkinEditor.SkinElements
{

    public enum BasisSkinType
    {
        SubSkin,
        MainSkin,
        SimpleText,
        EditText,
        ColourRect
    }

    [RegisterFactoryClass(CLSID_PROJELEMENT_BASISSKIN.Literal)]
    public class BasisSkin : IProjectElement
    {
        private string m_Name;
        private BasisSkinType m_SkinType;
        private Size m_Size;
        private Point m_Location;
        private WidgetAlign m_Align;
        private List<BasisSkinState> m_States = new List<BasisSkinState>();

        #region IProjectElement Members

        public string Name
        {
            get { return m_Name; }
        }

        public Guid Type
        {
            get { return CLSID_PROJELEMENT_BASISSKIN.ID; }
        }

        public BasisSkinType SkinType
        {
            get { return m_SkinType; }
            set { m_SkinType = value; }
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

        public WidgetAlign Align
        {
            get { return m_Align; }
            set { m_Align = value; }
        }

        public List<BasisSkinState> States
        {
            get { return m_States; }
        }

        public bool FromXML(XmlElement _element)
        {

            SkinType = (BasisSkinType) Enum.Parse(typeof (BasisSkinType), _element.Attributes["type"].Value);

            string[] ofst = _element.Attributes["offset"].Value.Split(' ');
            Location = new Point(Convert.ToInt32(ofst[0]), Convert.ToInt32(ofst[1]));
            Size = new Size(Convert.ToInt32(ofst[2]), Convert.ToInt32(ofst[3]));

            if (_element.Attributes["align"] != null)
                Align = (WidgetAlign) Converter.StringToAlign(_element.Attributes["align"].Value);

            XmlNodeList list = _element.SelectNodes("State");
            foreach (XmlElement element in list)
            {
                BasisSkinState p = new BasisSkinState();
                try
                {
                    p.FromXML(element);
                }
                catch (Exception _e)
                {
                    throw new Exception(p + ": " + _e.Message, _e);
                }
                States.Add(p);
            }

            return true;
        }

        public XmlElement ToXML(XmlDocument _doc)
        {
            XmlElement root = _doc.CreateElement("BasisSkin");

            root.Attributes.Append(_doc.CreateAttribute("type")).Value = SkinType.ToString();
            root.Attributes.Append(_doc.CreateAttribute("offset")).Value = StringConverter.LocationToString(Location) + " " + StringConverter.SizeToString(Size);
            if((Align)Align != Common.Align.DEFAULT) 
                root.Attributes.Append(_doc.CreateAttribute("align")).Value = Converter.AlignToString((Align)Align);

            foreach (BasisSkinState state in m_States)
                root.AppendChild(state.ToXML(_doc));
            return root;
        }

        #endregion
    }
}
