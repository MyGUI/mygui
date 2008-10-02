
using System;

namespace Common
{
    /// <summary>
    /// Общая категория для одиноких синглтонов
    /// </summary>
    public struct CATID_COMMON
    {
        public const string Literal = "06ffdcc9-5796-4c8e-b122-7a7de6c768a0";
        public static Guid ID = new Guid(Literal);
    }

    /// <summary>
    /// Словарь отношения имени элемента проекта к Guid'у
    /// </summary>
    public struct CLSID_PROJECT_ELEMENT_DICTIONARY
    {
        public const string Literal = "224aeeb0-d113-4666-927a-88b89e4bb194";
        public static Guid ID = new Guid(Literal);
    }

    /// <summary>
    /// Текущий открытый проект
    /// </summary>
    public struct CLSID_CURRENT_PROJECT
    {
        public const string Literal = "e7a05610-5c95-4b18-8421-114c8bf66d9d";
        public static Guid ID = new Guid(Literal);
    }

    /// <summary>
    /// Категория для киперов проектов
    /// </summary>
    public struct CATID_PROJECT_KEEPERS
    {
        public const string Literal = "c211f5c7-0af0-4eff-9caa-5e3af245aa94";
        public static Guid ID = new Guid(Literal);
    }


    /// <summary>
    /// Плагины, используемые только в проекте
    /// </summary>
    public struct CATID_PROJECT_PLUGINS
    {
        public const string Literal = "3685153f-7a40-4974-8c54-cc7c85eeea52";
        public static Guid ID = new Guid(Literal);
    }

    /// <summary>
    /// Плагины, используемые на протяжении всей работы приложения
    /// </summary>
    public struct CATID_STARTUP_PLUGINS
    {
        public const string Literal = "c13e0e88-538b-4375-877f-01337f4a16cb";
        public static Guid ID = new Guid(Literal);
    }

    /// <summary>
    /// Класс визуализации
    /// </summary>
    public struct CLSID_VISUAL_CONTAINER
    {
        public const string Literal = "6143aa3a-e034-432c-a131-f8a377525df9";
        public static Guid ID = new Guid(Literal);
    }
}
