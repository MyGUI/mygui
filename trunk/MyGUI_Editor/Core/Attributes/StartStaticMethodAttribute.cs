using System;
using System.Reflection;

namespace Core.Attributes
{
    /// <summary>
    /// Атрибут, указывающий, что помеченый метод будет выполнен при загрузке модуля.
    /// Выполняются только статические методы без параметров.
    /// </summary>
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false, Inherited = false)]
    public class StartStaticMethodAttribute : Attribute, IAttribute
    {
        #region IAttribute Members

        public void Execute(object _target)
        {
            MethodInfo info = _target as MethodInfo;
            if(info == null) return;
            info.Invoke(null, new object[0]);
        }

        #endregion
    }
}
