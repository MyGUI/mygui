
using System;
using System.Collections.Generic;
using Core;

namespace Common
{
    /// <summary>
    /// Класс конвертирования имени элемента в Guid
    /// </summary>
    public static class ElementTypeConvertor
    {
        /// <summary>
        /// Коонвертировать имя элемента в Guid
        /// </summary>
        /// <param name="_type"></param>
        /// <returns></returns>
        public static Guid ConvertElementType(string _type)
        {
            Dictionary<string, Guid> dict =
                CoreFacade.Register.GetItem<Dictionary<string, Guid>>
                (CATID_COMMON.ID, CLSID_PROJECT_ELEMENT_DICTIONARY.ID);

            if (dict.ContainsKey(_type)) return dict[_type];
            return Guid.Empty;
        }

        /// <summary>
        /// Зарегистрировать отношение
        /// </summary>
        /// <param name="_name">имя</param>
        /// <param name="_id">Guid</param>
        public static void WritePair(string _name, Guid _id)
        {
            Dictionary<string, Guid> dict =
                CoreFacade.Register.GetItem<Dictionary<string, Guid>>
                (CATID_COMMON.ID, CLSID_PROJECT_ELEMENT_DICTIONARY.ID);
            if (dict == null)
            {
                dict = new Dictionary<string, Guid>();
                CoreFacade.Register.RegisterIt(dict, CATID_COMMON.ID, CLSID_PROJECT_ELEMENT_DICTIONARY.ID);
            }
            dict.Add(_name, _id);
        }
    }
}
