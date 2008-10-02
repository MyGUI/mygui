
namespace Core.Attributes
{
    /// <summary>
    /// Интерфейс атрибута
    /// </summary>
    interface IAttribute
    {
        /// <summary>
        /// Выполнение необходимого действия над объектом помеченного класса
        /// </summary>
        /// <param name="_target">объект помеченного класса</param>
        void Execute(object _target);
    }
}
