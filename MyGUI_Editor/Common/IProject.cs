
using System;
using System.Xml;
using System.Collections.Generic;

namespace Common
{
    /// <summary>
    /// Интерфейс проекта
    /// </summary>
    public interface IProject : IDisposable
    {
        /// <summary>
        /// Инициализация проекта
        /// </summary>
        /// <param name="_name">имя</param>
        /// <param name="_prefix">префикс</param>
        void Init(string _name, string _prefix);
        /// <summary>
        /// Событие, когда список элементов проекта изменился
        /// </summary>
        event EventHandler OnUpdateElementList;
        /// <summary>
        /// Событие, когда изменился текущий элемент
        /// </summary>
        event EventHandler OnChangeCurElement;
        /// <summary>
        /// Открыть из файла
        /// </summary>
        /// <param name="_filename">имя файла</param>
        void Load(string _filename);
        /// <summary>
        /// Сохранить по указанному пути
        /// </summary>
        /// <param name="_path">путь сохранения</param>
        void Save(string _path);
        /// <summary>
        /// Добавить из файла
        /// </summary>
        /// <param name="_filename">имя файла</param>
        void AddFromFile(string _filename);
        /// <summary>
        /// Добавить элемент
        /// </summary>
        /// <param name="_element">элемент</param>
        void AddElement(XmlElement _element);
        /// <summary>
        /// Удалить элемент по типу
        /// </summary>
        /// <param name="_type">ID типа. Guid.Empty - удалить все</param>
        void RemoveElementsByType(Guid _type);
        /// <summary>
        /// Перебрать элементы по типу
        /// </summary>
        /// <param name="_type">Guid типа элемета. Guid.Empty - перебрать все</param>
        /// <returns></returns>
        IEnumerable<IProjectElement> GetElements(Guid _type);
        /// <summary>
        /// Имя
        /// </summary>
        string Name { get; }
        /// <summary>
        /// Префикс, испльзующийся для формирования имён элементов проекта
        /// </summary>
        string Prefix { get; }
        /// <summary>
        /// Необходимо сохранение - были изменения
        /// </summary>
        bool NeedSave { get; }
        /// <summary>
        /// Текущий элемент
        /// </summary>
        IProjectElement CurrentElement{ get; set; }
        /// <summary>
        /// Кипер проекта
        /// </summary>
        Guid Keeper{ get; }
        /// <summary>
        /// Местоположение проекта
        /// </summary>
        string ProjectLocation { get; }
    }
}
