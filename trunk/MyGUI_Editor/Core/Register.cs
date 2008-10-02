using System;
using System.Collections.Generic;
using System.Reflection;

namespace Core
{
    /// <summary>
    /// Делегат регистрации элемента в категории
    /// </summary>
    /// <param name="_category">идентификатор категории</param>
    /// <param name="_item">идентификатор элемента</param>
    public delegate void RedisterInCategoryDlgt(Guid _category, Guid _item);

    /// <summary>
    /// Реестр ядра
    /// </summary>
    public class Register
    {
        Dictionary<GuidPair, Guid> m_RegisterCatDictionary = new Dictionary<GuidPair, Guid>();
        Dictionary<Guid, object> m_InnerDictionary = new Dictionary<Guid, object>();

        public event RedisterInCategoryDlgt OnRegister;
        public event RedisterInCategoryDlgt OnUnRegister;

        internal Register()
        {
            
        }

        /// <summary>
        /// Регистрация объекта
        /// </summary>
        /// <param name="_object">регистрируемый объект</param>
        /// <param name="_category">идентификатор категории</param>
        /// <param name="_item">идентификатор объекта в категории</param>
        public void RegisterIt(object _object, Guid _category, Guid _item)
        {
            Guid id = Guid.NewGuid();
            GuidPair key = new GuidPair(_category, _item);
            if (!m_RegisterCatDictionary.ContainsKey(key))
            {
                m_RegisterCatDictionary.Add(key, id);
                m_InnerDictionary.Add(id, _object);
            }
            else
                m_InnerDictionary[m_RegisterCatDictionary[key]] = _object;

            if (OnRegister != null) OnRegister(_category, _item);
        }

        /// <summary>
        /// Удаление объекта из реестра
        /// </summary>
        /// <param name="_category">идентификатор категории</param>
        /// <param name="_item">идентификатор объекта в категории</param>
        public void UnregisterIt(Guid _category, Guid _item)
        {
            GuidPair key = new GuidPair(_category, _item);
            if(m_RegisterCatDictionary.ContainsKey(key))
            {
                if (OnUnRegister != null) OnUnRegister(_category, _item);

                m_InnerDictionary.Remove(m_RegisterCatDictionary[key]);
                m_RegisterCatDictionary.Remove(key);
            }
        }

        /// <summary>
        /// Получить зарегистрированный объект
        /// </summary>
        /// <typeparam name="I">запрашиваемый интерфейс объекта</typeparam>
        /// <param name="_category">идентификатор категории</param>
        /// <param name="_item">идентификатор объекта в категории</param>
        /// <returns>интерфейс найденного объекта. null, в случае, если в указанной категории объект с таким идентификатором 
        /// не зарегистрирован или не реализует запрашиваемый интерфейс.
        /// </returns>
        public I GetItem<I>(Guid _category, Guid _item) where I : class
        {
            GuidPair key = new GuidPair(_category, _item);
            if (m_RegisterCatDictionary.ContainsKey(key))
                return m_InnerDictionary[m_RegisterCatDictionary[key]] as I;
            else return null;
        }

        /// <summary>
        /// Перебор элементов, зарегистрированных в категории
        /// </summary>
        /// <typeparam name="I">запрашиваемый интерфейс объектов</typeparam>
        /// <param name="_category">идентификатор категории</param>
        /// <returns>интерфейсы объектов, зарегистрированных в указанной категории</returns>
        public IEnumerable<I> GetItems<I>(Guid _category) where I : class
        {
            foreach (KeyValuePair<GuidPair, Guid> pair in m_RegisterCatDictionary)
                if (pair.Key.Category == _category)
                {
                    I item = m_InnerDictionary[pair.Value] as I;
                    if(item == null) continue;
                    yield return item;
                }
        }

        /// <summary>
        /// Получить карту соответствий идентификаторов и зарегистрированных объектов категории
        /// </summary>
        /// <typeparam name="I">запрашиваемый интерфейс объектов</typeparam>
        /// <param name="_category">идентификатор категории</param>
        /// <param name="_items">карта соответствий</param>
        public void GetItems<I>(Guid _category, out Dictionary<Guid, I> _items) where I : class
        {
            _items = new Dictionary<Guid, I>();
            foreach (KeyValuePair<GuidPair, Guid> pair in m_RegisterCatDictionary)
                if (pair.Key.Category == _category)
                {
                    I item = m_InnerDictionary[pair.Value] as I;
                    if (item == null) continue;
                    _items.Add(pair.Key.Item, m_InnerDictionary[pair.Value] as I);
                }
        }

        /// <summary>
        /// Создать объект фабричного класса
        /// </summary>
        /// <typeparam name="I">запрашиваемый интерфейс объекта</typeparam>
        /// <param name="_item">идентификатор фабричного класса</param>
        /// <param name="_params">входные параметры конструктора класса</param>
        /// <returns>интерфейс созданного объекта</returns>
        public I CreateItem<I>(Guid _item, params object[] _params) where I : class
        {
            Type type;
            GuidPair key = new GuidPair(CATID_FABRIC_CATEGORY.ID, _item);
            if (!m_RegisterCatDictionary.ContainsKey(key)) 
                throw new Exception("Элемент не зарегистрирован. Категория:'" + CoreFacade.IDTranslator.Translate(CATID_FABRIC_CATEGORY.ID) +
                    "'; Элемент: " + CoreFacade.IDTranslator.Translate(_item) + "'");

            type = m_InnerDictionary[m_RegisterCatDictionary[key]] as Type;
            if(type == null)
                throw new Exception("Элемент в категории '" + CoreFacade.IDTranslator.Translate(CATID_FABRIC_CATEGORY.ID) + 
                    "' должен быть типа 'Type'!!! Зарегистрированный элемент под Guid:'" + _item + "'(" + CoreFacade.IDTranslator.Translate(_item) + "); тип: '" + 
                    m_InnerDictionary[m_RegisterCatDictionary[key]].GetType().Name + "'");

            return CreateItem<I>(type, _params);
        }

        /// <summary>
        /// Создать объект фабричного класса
        /// </summary>
        /// <typeparam name="I">запрашиваемый интерфейс объекта</typeparam>
        /// <param name="_type">тип создаваемого объекта</param>
        /// <param name="_params">входные параметры конструктора класса</param>
        /// <returns>интерфейс созданного объекта</returns>
        public I CreateItem<I>(Type _type, params object[] _params) where I : class 
        {
            Type[] prms = new Type[_params.Length];

            for (int i = 0; i < _params.Length; i++)
            {
                if (_params[i] == null)
                    throw new Exception("Не найден соответствующий конструктор для '" +
                        _type.Name + "', т.к нельзя определить тип параметра 'null'. Параметр №" + i);
                prms[i] = _params[i].GetType();
            }

            ConstructorInfo c = _type.GetConstructor(prms);
            return c.Invoke(_params) as I;
        }
    }

    class GuidPair : IEquatable<GuidPair>
    {
        public readonly Guid Category;
        public readonly Guid Item;

        public GuidPair(Guid _category, Guid _item)
        {
            Category = _category;
            Item = _item;
        }
        
        public bool Equals(GuidPair guidPair)
        {
            if (guidPair == null) return false;
            return Category == guidPair.Category && Item == guidPair.Item;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(this, obj)) return true;
            return Equals(obj as GuidPair);
        }

        public override int GetHashCode()
        {
            return Category.GetHashCode() + 29*Item.GetHashCode();
        }
    }
}
