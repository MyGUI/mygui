using System;
using System.Collections.Generic;
using System.Text;
using DoxygenWrapper.Wrappers.Compounds;
using DoxygenWrapper.Wrappers.Interfaces;
using DoxygenWrapper.Wrappers.Types;

namespace DoxygenWrapper.Wrappers.Replacers
{
	public class FunctionReplacer :
		IReplacer
	{
		public FunctionReplacer(CompoundFunction _func)
		{
			mCompound = _func;

			mReplace["MethodName"] = UpFirstLetter(mCompound.Name);
			mReplace["PropertyName"] = mCompound.PropertyName; // метод может быть заменен на свойство
			mReplace["OriginalMethodName"] = mCompound.Name;

			mReplace["OriginalTypeName"] = mCompound.CompoundType.TypeName;
			TypeInfo info = WrapperManager.Instance.GetTypeInfo(mCompound.CompoundType.TypeName);
			if (info != null)
			{
				foreach (var value in info.Values)
					mReplace[value.First] = value.Second;
			}

			int index = 1;
			foreach (var type in mCompound.CompoundParamTypes)
			{
				mReplace["OriginalTypeName" + index.ToString()] = type.TypeName;
				if (type.ValueName != null)
					mReplace["ValueName" + index.ToString()] = type.ValueName;
				else
					mReplace["ValueName" + index.ToString()] = "_value" + index.ToString();

				TypeInfo parameInfo = WrapperManager.Instance.GetTypeInfo(type.TypeName);
				if (parameInfo != null)
				{
					foreach (var value in parameInfo.Values)
						mReplace[value.First + index.ToString()] = value.Second;
				}
				else
				{
					//ConsoleUtility.WriteErrorLine("Type {0} not found", type.TypeName);
				}

				index++;
			}
		}

		private string UpFirstLetter(string _name)
		{
			if (_name.Length != 0)
				return string.Format("{0}{1}", new string(new char[] { _name[0] }).ToUpperInvariant(), _name.Substring(1));
			return _name;
		}

		public string Replace(string _tag)
		{
			string result = null;
			if (mReplace.TryGetValue(_tag, out result))
				return result;
			return "";
		}

		private CompoundFunction mCompound;
		private Dictionary<string, string> mReplace = new Dictionary<string, string>();
	}
}
