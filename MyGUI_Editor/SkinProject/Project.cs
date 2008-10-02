using System;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using Common;
using Core;
using Core.Attributes;
using SkinProject;

namespace SkinProject
{
    [RegisterFactoryClass(CLSID_SKIN_PROJECT.Literal)]
    public class Project : IProject, ILogSender
    {
        public event EventHandler OnUpdateElementList;
        public event EventHandler OnChangeCurElement;
        public event MessageDlgt OnMessage;

        private const string PROJ_EXT = ".mgsproj";
        private const string SKIN_EXT = ".skin";

        private string m_Name;
        private string m_Prefix;
        private Dictionary<Guid, List<IProjectElement>> m_SkinElements;
        private bool m_Edited = true;
        private bool m_SendUpdate = true;
        private IProjectElement m_currentElement;
        private string m_Location;
        
        public void Init(string _name, string _prefix)
        {
            m_SkinElements = new Dictionary<Guid, List<IProjectElement>>();
            m_Name = _name;
            m_Prefix = _prefix;
        }

        protected void SendMessage(string _msg)
        {
            if (OnMessage != null) OnMessage(_msg, 0, Name);
        }

        protected void SendWarning(string _msg)
        {
            if (OnMessage != null) OnMessage(_msg, 1, Name);
        }

        protected void SendError(string _msg)
        {
            if (OnMessage != null) OnMessage(_msg, 2, Name);
        }

        protected void SendOnUpdateElementList()
        {
            if (OnUpdateElementList != null && m_SendUpdate) OnUpdateElementList(this, EventArgs.Empty);
        }

        public void Load(string _filename)
        {
            XmlDocument doc = new XmlDocument();
            try { doc.Load(_filename); }
            catch (Exception _e) { SendError(_e.Message); }
            SendMessage("Открытие проекта из файла " + _filename + "'");

            XmlElement el = doc.SelectSingleNode("MyGUI/Project") as XmlElement;
            XmlNodeList nodes = el.SelectNodes("Part");
            foreach (XmlElement element in nodes)
                AddFromFile(element.InnerText);   

            SendOnUpdateElementList();

            m_Location = _filename.Remove(_filename.LastIndexOf('\\'));
        }

        public void Save(string _path)
        {
            string dir_path = _path.TrimEnd('\\') + "\\" + Name;
            if (!Directory.Exists(dir_path))
                Directory.CreateDirectory(dir_path);

            SaveProject(dir_path + "\\" + Name + PROJ_EXT);
            SaveSkin(dir_path + "\\" + Name + SKIN_EXT);

            m_Edited = false;
            SendMessage("Проект был сохранён");
        }

        private void SaveProject(string _path)
        {
            XmlProject xmlP = new XmlProject();
            xmlP.Init(this, CLSID_SKIN_PROJECT.ID);
            xmlP.AddNode(xmlP.CreateNode("Part", Name + SKIN_EXT, false));
            xmlP.Seve(_path);
            m_Location = _path.Remove(_path.LastIndexOf('\\'));
        }

        private void SaveSkin(string _path)
        {
            XmlHelper skin = new XmlHelper();
            skin.AddNode(skin.CreateNode("type", "Skin", true));
            //skin.Init("Skin");

            foreach (IProjectElement element in m_SkinElements[ElementTypeConvertor.ConvertElementType("Skin")])
                skin.AddNode(element.ToXML(skin.Document));
            skin.Seve(_path);
        }

        public void AddFromFile(string _filename)
        {
            XmlDocument doc = new XmlDocument();
            try { doc.Load(_filename); }
            catch(Exception _e) { SendError(_e.Message); }

            SendMessage("Добавление элементов в проект из файла " + _filename + "'");

            bool ex = false;

            m_SendUpdate = false;
            foreach (XmlNode element in doc)
            {
                if (element.Name == "MyGUI")
                {
                    if(element as XmlElement == null) continue;
                    foreach (XmlElement xmlElement in element)
                    {
                        AddElement(xmlElement);    
                    }
                    ex = true;
                }
            }
            if(!ex) SendWarning("Часть 'MyGUI' не найдена в файле '" + _filename + "'");
            m_SendUpdate = true;

            SendOnUpdateElementList();
        }

        public void AddElement(XmlElement _element)
        {
            try
            {
                Guid type;
                IProjectElement element;

                type = ElementTypeConvertor.ConvertElementType(_element.Name);

                if(type == Guid.Empty)
                {
                    SendError("Неизвестный элемент '" + _element.Name + "'");
                    return;
                }

                element = CoreFacade.Register.CreateItem<IProjectElement>(type);
                element.FromXML(_element);

                if(!m_SkinElements.ContainsKey(type)) m_SkinElements.Add(type, new List<IProjectElement>());
                m_SkinElements[type].Add(element);
            }
            catch(Exception _e)
            {
                SendError(_e.Message);
            }

            SendOnUpdateElementList();
        }

        public void Dispose()
        {
            m_SkinElements.Clear();
            SendMessage("Проект был удалён");

            SendOnUpdateElementList();
            m_currentElement = null;
            if (OnChangeCurElement != null) OnChangeCurElement(this, EventArgs.Empty);
        }

        public void RemoveElementsByType(Guid _type)
        {
            bool ch = false;
            if(m_SkinElements.ContainsKey(_type))
            {
                SendMessage("Удаление элементов типа '" + _type + "'");

                if (m_currentElement != null &&
                   m_SkinElements[_type].Contains(m_currentElement) &&
                   OnChangeCurElement != null) ch = true;

                m_SkinElements[_type].Clear();
                m_SkinElements.Remove(_type);
            }
            SendOnUpdateElementList();
            if (ch) OnChangeCurElement(this, EventArgs.Empty);
        }

        public string Name
        {
            get { return m_Name; }
        }

        public string Prefix
        {
            get { return m_Prefix; }
        }

        public bool NeedSave
        {
            get { return m_Edited; }
        }

        public IProjectElement CurrentElement
        {
            get { return m_currentElement; }
            set
            {
                bool ex = false;
                foreach (KeyValuePair<Guid, List<IProjectElement>> pair in m_SkinElements)
                    if(pair.Value.Contains(value))
                    {
                        ex = true;
                        break;
                    }
                if(ex) m_currentElement = value;
                else m_currentElement = null;
                if (OnChangeCurElement != null) OnChangeCurElement(this, EventArgs.Empty);
            }
        }

        public Guid Keeper
        {
            get { return CLSID_SKIN_PROJECT_KEEPER.ID; }
        }

        public string ProjectLocation
        {
            get { return m_Location; }
        }

        public IEnumerable<IProjectElement> GetElements(Guid _type)
        {
            if (m_SkinElements.ContainsKey(_type))
                foreach (IProjectElement element in m_SkinElements[_type])
                    yield return element;
        }
    }
}