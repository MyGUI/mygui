using System;
using System.Collections.Generic;
using System.Text;
using DoxygenWrapper.Wrappers.Types;

namespace DoxygenWrapper.Wrappers
{
	public class OutputManager
	{
		public FileData GetOutputFile(string _output)
		{
			FileData result = null;
			if (mOutputFiles.TryGetValue(_output, out result))
				return result;

			result = new FileData();
			mOutputFiles[_output] = result;

			return result;
		}

		public void Save()
		{
			foreach (var item in mOutputFiles)
				item.Value.Save(item.Key);
		}

		private Dictionary<string, FileData> mOutputFiles = new Dictionary<string, FileData>();
	}
}
