using System;
using System.Collections.Generic;
using System.Text;
using DoxygenWrapper.Wrappers.Interfaces;
using DoxygenWrapper.Wrappers.Compounds;
using DoxygenWrapper.Wrappers.Types;
using DoxygenWrapper.Wrappers.Compounds.Types;

namespace DoxygenWrapper.Wrappers.Replacers
{
	public class EventReplacer :
		IReplacer
	{
		public EventReplacer(CompoundVariable _variable)
		{
			mReplace["DelegateName"] = _variable.EventName;

			CompoundType eventType = CompoundUtility.GetEventType(_variable);

			int index = 1;
			foreach (var type in eventType.TemplateTypes)
			{
				mReplace["OriginalTypeName" + index.ToString()] = type.TypeName;
				mReplace["ValueName" + index.ToString()] = GetValueName(index - 1, _variable.ValueNames);

				TypeInfo parameInfo = WrapperManager.Instance.GetTypeInfo(type.TypeName);
				if (parameInfo != null)
				{
					foreach (var value in parameInfo.Values)
						mReplace[value.First + index.ToString()] = value.Second;
				}

				index++;
			}
		}

		private string GetValueName(int _index, List<string> _values)
		{
			if (_index < _values.Count)
				return _values[_index];
			return string.Format("_value{0}", _index + 1);
		}

		public string Replace(string _tag)
		{
			string result = null;
			if (mReplace.TryGetValue(_tag, out result))
				return result;
			return "";
		}

		private Dictionary<string, string> mReplace = new Dictionary<string, string>();
	}
}
