namespace Core.Attributes
{
    /// <summary>
    /// Атрибут, регистрирующий класс в категории фабричных классов
    /// </summary>
    public class RegisterFactoryClass : RegisterIt
    {
        /// <summary>
        /// Конструктор
        /// </summary>
        /// <param name="_item">идентификатор, по которому класс будет находиться в категории и по которому можно будет создавать его объекты</param>
        public RegisterFactoryClass(string _item)
            : base(CATID_FABRIC_CATEGORY.Literal, _item)
        {
        }

        public override void Execute(object _target)
        {
            CoreFacade.Register.RegisterIt(_target, Category, Item);
        }
    }
}
