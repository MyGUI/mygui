using System;
using System.Collections.Generic;
using System.Text;
using DoxygenWrapper.Wrappers.Types;
using DoxygenWrapper.Wrappers.Interfaces;

namespace DoxygenWrapper.Wrappers
{
	public class ReplaceManager
	{
		public void DoReplace(FileData _data, IReplacer[] _replacers)
		{
			for (int index = 0; index < _data.Data.Length; )
			{
				if ((_data.Data[index] == '#') && (index + 1 < _data.Data.Length) && (_data.Data[index + 1] == '{'))
				{
					int endIndex = index + 1;
					while (endIndex < _data.Data.Length)
					{
						if (_data.Data[endIndex] == '}')
							break;
						endIndex++;
					}

					if (endIndex < _data.Data.Length)
					{
						int count = endIndex - index + 1;
						char[] destination = new char[count];
						_data.Data.CopyTo(index, destination, 0, count);

						_data.Data.Remove(index, count);
						_data.Data.Insert(index, ReplaceTag(destination, _replacers));
					}
				}
				else
				{
					index++;
				}
			}
		}

		private string ReplaceTag(char[] _tag, IReplacer[] _replacers)
		{
			string tag = new string(_tag, 2, _tag.Length - 3);

			foreach (IReplacer replacer in _replacers)
			{
				string result = replacer.Replace(tag);
				if (result != "")
					return result;
			}
			return "";
		}
	}
}
