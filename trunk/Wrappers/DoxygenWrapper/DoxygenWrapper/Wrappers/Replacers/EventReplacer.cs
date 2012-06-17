using System;
using System.Collections.Generic;
using System.Text;
using DoxygenWrapper.Wrappers.Interfaces;
using DoxygenWrapper.Wrappers.Compounds;

namespace DoxygenWrapper.Wrappers.Replacers
{
	public class EventReplacer :
		IReplacer
	{
		public EventReplacer(CompoundVariable _variable)
		{
			mReplace["DelegateName"] = _variable.EventName;
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
