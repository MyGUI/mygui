namespace Common
{
    /// <summary>
    /// Делегат сообщения
    /// </summary>
    /// <param name="_msg">содержание</param>
    /// <param name="_level">уровень</param>
    /// <param name="_fromName">имя источника</param>
    public delegate void MessageDlgt(string _msg, int _level, string _fromName);

    /// <summary>
    /// Интерфейс, отчётности о дествиях по событию
    /// </summary>
    public interface ILogSender
    {
        /// <summary>
        /// Событие сообщения
        /// </summary>
        event MessageDlgt OnMessage;    
    }
}
