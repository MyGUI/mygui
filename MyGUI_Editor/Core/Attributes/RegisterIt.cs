using System;

namespace Core.Attributes
{
    /// <summary>
    /// Атрибут регистрации синглтона объекта в реестре ядра
    /// </summary>
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = true, Inherited = false)]
    public class RegisterIt : Attribute, IAttribute
    {
        public readonly Guid Category;
        public readonly Guid Item;

        /// <summary>
        /// Конструктор
        /// </summary>
        /// <param name="_category">категория, в которой нужно зарегистрироваль объект</param>
        /// <param name="_item">идентификатор, под которым объект будет в категории</param>
        public RegisterIt(string _category, string _item)
        {
            Category = new Guid(_category);
            Item = new Guid(_item);
        }

        public virtual void Execute(object _target)
        {
            Type t = _target as Type;
            if(t == null) return;
            CoreFacade.Register.RegisterIt(CoreFacade.Register.CreateItem<object>(t), Category, Item);
        }
    }
}
