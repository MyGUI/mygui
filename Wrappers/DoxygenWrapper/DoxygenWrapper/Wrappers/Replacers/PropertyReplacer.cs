using System;
using System.Collections.Generic;
using System.Text;
using DoxygenWrapper.Wrappers.Interfaces;
using DoxygenWrapper.Wrappers.Compounds;
using DoxygenWrapper.Wrappers.Types;

namespace DoxygenWrapper.Wrappers.Replacers
{
	public class PropertyReplacer :
		IReplacer
	{
		public PropertyReplacer(Pair<CompoundFunction, CompoundFunction> _func)
		{
			mReplace["PropertyName"] = _func.First.PropertyName;
			mReplace["OriginalTypeName"] = _func.First.CompoundType.TypeName;
			if (_func.Second != null)
				mReplace["OriginalTypeName1"] = _func.Second.CompoundParamTypes[0].TypeName;

			TypeInfo info = WrapperManager.Instance.GetTypeInfo(_func.First.CompoundType.TypeName);
			if (info != null)
			{
				foreach (var value in info.Values)
				{
					mReplace[value.First] = value.Second;
					if (_func.Second != null)
						mReplace[value.First + "1"] = value.Second;
				}
			}
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
