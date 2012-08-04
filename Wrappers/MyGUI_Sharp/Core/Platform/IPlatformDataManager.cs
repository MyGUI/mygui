using System;
using System.Collections.Generic;
using System.Text;

namespace MyGUI.Sharp
{
	public interface IPlatformDataManager
	{
		bool IsDataExist(string _name);

		byte[] GetData(string _name);

		void DestroyData(string _name);

		string[] GetDataListNames(string _pattern);

		string GetDataPath(string _name);
	}
}
