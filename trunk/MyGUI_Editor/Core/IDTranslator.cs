
using System;
using System.Collections.Generic;

namespace Core
{
    /// <summary>
    /// Переводчик идентификаторов
    /// </summary>
    public class IDTranslator
    {
        Dictionary<Guid, string> m_Dictionary = new Dictionary<Guid, string>();

        internal IDTranslator()
        {}

        /// <summary>
        /// Добавить запись в переводчик
        /// </summary>
        /// <param name="_id">идентификатор</param>
        /// <param name="_name">имя, соответствующее идентификатору</param>
        public void AddPair(Guid _id, string _name)
        {
            if (m_Dictionary.ContainsKey(_id)) m_Dictionary[_id] = _name;
            else m_Dictionary.Add(_id, _name);
        }

        /// <summary>
        /// Перевести идентифекатор
        /// </summary>
        /// <param name="_id">идентифекатор</param>
        /// <returns>имя, соответствующее идентификатору</returns>
        public string Translate(Guid _id)
        {
            if (m_Dictionary.ContainsKey(_id)) return m_Dictionary[_id];
            
            return _id.ToString();
        }

        /// <summary>
        /// Обратный перевод. Соответствие имени совокупноси идентификаторов.
        /// </summary>
        /// <param name="_name">имя</param>
        /// <param name="_guids">список идентификаторов, поставленных в соответствие указанному имени</param>
        /// <param name="_ignoreCase">игнорировать строчные символы</param>
        /// <returns>true в случае, если соответствия найдены, в противном случае - false</returns>
        public bool BackTranslate(string _name, out List<Guid> _guids, bool _ignoreCase)
        {
            _guids = new List<Guid>();
            if(!m_Dictionary.ContainsValue(_name)) return false;
            foreach (KeyValuePair<Guid, string> pair in m_Dictionary)
                if (_ignoreCase)
                {
                    if (pair.Value.ToLower() == _name.ToLower()) _guids.Add(pair.Key);
                }
                else
                {
                    if (pair.Value == _name) _guids.Add(pair.Key);
                }
            return true;
        }
    }
}
