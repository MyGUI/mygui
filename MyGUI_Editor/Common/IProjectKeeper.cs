
using System;

namespace Common
{
    /// <summary>
    /// Интерфейс класса, рулящего проектом.
    /// </summary>
    public interface IProjectKeeper
    {
        /// <summary>
        /// Идентификатор типа проекта
        /// </summary>
        Guid ProjectID { get; }
        /// <summary>
        /// Название типа проекта
        /// </summary>
        string Name { get; }
        /// <summary>
        /// Фильтр, используемый при открытии проекта
        /// </summary>
        string ProjectFileFilter { get; }
        /// <summary>
        /// Фильтр, добавляемых файлов
        /// </summary>
        string AddedFileFilter { get; }
        /// <summary>
        /// Плагины проекта
        /// </summary>
        Guid[] Plugins { get; }
    }
}
