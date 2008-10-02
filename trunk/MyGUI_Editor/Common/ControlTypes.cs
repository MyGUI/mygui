
namespace Common
{
    /// <summary>
    /// Типы элементов интерфейса
    /// </summary>
    public enum ControlTypes
    {
        /// <summary>
        /// Никакой %)
        /// </summary>
        None,
        /// <summary>
        /// Общий (обычный)
        /// </summary>
        Common,
        /// <summary>
        /// Панель
        /// </summary>
        ToolBar,
        /// <summary>
        /// Дочащееся окно слева
        /// </summary>
        DockWindowElement_Left,
        /// <summary>
        /// Дочащееся окно справа
        /// </summary>
        DockWindowElement_Right,
        /// <summary>
        /// Дочащееся окно сверху
        /// </summary>
        DockWindowElement_Top,
        /// <summary>
        /// Дочащееся окно снизу
        /// </summary>
        DockWindowElement_Buttom,
        /// <summary>
        /// Свободное дочащееся окно 
        /// </summary>
        DockWindowElement_Floating,
    }
}
