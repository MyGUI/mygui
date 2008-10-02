
using System;
using System.Xml;

namespace Common
{
    /// <summary>
    /// Интерфейс элемента проекта
    /// </summary>
    public interface IProjectElement
    {
        /// <summary>
        /// Имя
        /// </summary>
        string Name{ get; }
        /// <summary>
        /// ID типа
        /// </summary>
        Guid Type { get; }
        /// <summary>
        /// Инициализация XML-элементом
        /// </summary>
        /// <param name="_element">XML элемент</param>
        /// <returns>true при успешной инициализации. В противном случае - false</returns>
        bool FromXML(XmlElement _element);
        /// <summary>
        /// Конвертирование в XML элемент.
        /// </summary>
        /// <returns>представление в XML элементе</returns>
        XmlElement ToXML(XmlDocument _doc);
    }
}