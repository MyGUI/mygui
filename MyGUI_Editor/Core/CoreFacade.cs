namespace Core
{
    /// <summary>
    /// Фасад ядра. Позволяет централизовано работать с элементами ядра.
    /// </summary>
    public static class CoreFacade
    {
        private static Register m_Register;
        private static Loader m_Loader;
        private static IDTranslator m_IDTranslator;
        private static SettingsProvider m_SettingsProvider;

        /// <summary>
        /// Реестр
        /// </summary>
        public static Register Register
        {
            get
            {
                if(m_Register == null) m_Register = new Register();
                return m_Register;
            }
        }

        /// <summary>
        /// Загрузчик
        /// </summary>
        public static Loader Loader
        {
            get
            {
                if(m_Loader == null) m_Loader = new Loader();
                return m_Loader;
            }
        }

        /// <summary>
        /// Переводчик идентификаторов
        /// </summary>
        public static IDTranslator IDTranslator
        {
            get
            {
                if(m_IDTranslator == null) m_IDTranslator = new IDTranslator();
                return m_IDTranslator;
            }
        }

        /// <summary>
        /// Хранитель настроек
        /// </summary>
        public static SettingsProvider SettingsProvider
        {
            get
            {
                if (m_SettingsProvider == null) m_SettingsProvider = new SettingsProvider();
                return m_SettingsProvider;
            }
        }
    }
}
