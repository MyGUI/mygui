
using System;
using System.Windows.Forms;

namespace Common
{
    /// <summary>
    /// Интерфейс подключаемого модуля
    /// </summary>
    public interface IPlugin : IDisposable
    {
        /// <summary>
        /// Имя
        /// </summary>
        string Name { get; }
        /// <summary>
        /// Инициализировать плагин для дальнейшей работы
        /// </summary>
        void Init();
        /// <summary>
        /// Запросить у плагина контрол.
        /// </summary>
        /// <param name="_control">созданный контрол</param>
        /// <returns>тип созданного контрола</returns>
        ControlTypes CreateControl(ref Control _control);
    }
}
