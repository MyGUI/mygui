
using System.Windows.Forms;

namespace Common
{
    /// <summary>
    /// Интерфейс визуального модуля
    /// </summary>
    public interface IVisualContainer
    {
        /// <summary>
        /// Добавить контрол
        /// </summary>
        /// <param name="_control">контрол</param>
        /// <param name="_type">тип контрола</param>
        /// <param name="_additionInfo">дополнительная инфа</param>
        void InsertControl(Control _control, ControlTypes _type, object _additionInfo);
        /// <summary>
        /// Провести завершающие работы при закрытие проекта
        /// </summary>
        void PostCloseProject();
    }
}
