using System;
using System.Collections.Generic;
using System.Text;
using DoxygenWrapper.Wrappers.Types;

namespace DoxygenWrapper.Wrappers
{
	public class TemplateManager
	{
		public FileData GetTemplateCopy(string _fileName)
		{
			FileData result = GetTemplate(_fileName);

			FileData newFile = new FileData();
			newFile.Data.Append(result.Data);
			return newFile;
		}

		private FileData GetTemplate(string _fileName)
		{
			FileData result = null;
			if (mCache.TryGetValue(_fileName, out result))
			{
				return result;
			}

			result = new FileData();
			result.Load(_fileName);
			mCache[_fileName] = result;

			return result;
		}

		private Dictionary<string, FileData> mCache = new Dictionary<string, FileData>();
	}
}
