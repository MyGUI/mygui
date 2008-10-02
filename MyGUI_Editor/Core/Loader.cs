using System;
using System.Reflection;
using Core.Attributes;

namespace Core
{
    /// <summary>
    /// Загрузчик динамических модулей
    /// </summary>
    public class Loader
    {
        internal Loader()
        {}

        /// <summary>
        /// Загрузка сборки
        /// </summary>
        /// <param name="_filename">имя файла</param>
        public void LoadAssembly(string _filename)
        {
            Assembly asm = Assembly.LoadFile(_filename);
            Type[] types = asm.GetTypes();

            foreach (Type type in types)
            {
                RunAttributes(type);

                MethodInfo[] methods =
                    type.GetMethods(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static);
                foreach (MethodInfo info in methods)
                    RunAttributes(info);
            }
        }

        /// <summary>
        /// Выполнение операций атрибутов над помеченными объектами
        /// </summary>
        /// <param name="_provider">объект помеченный этим атрибутом</param>
        private void RunAttributes(ICustomAttributeProvider _provider)
        {
            IAttribute[] attrs = _provider.GetCustomAttributes(typeof(IAttribute), false) as IAttribute[];
            if (attrs != null)
                foreach (IAttribute attr in attrs)
                    attr.Execute(_provider);
        }
    }
}
