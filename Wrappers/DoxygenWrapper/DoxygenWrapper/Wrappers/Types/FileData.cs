using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace DoxygenWrapper.Wrappers.Types
{
	public class FileData
	{
		public void Load(string _fileName)
		{
			mData.Length = 0;
			using (var stream = new StreamReader(_fileName))
			{
				string read = null;
				while ((read = stream.ReadLine()) != null)
					mData.AppendLine(read);
			}
		}

		public void Save(string _fileName)
		{
			FileInfo info = new FileInfo(_fileName);
			if (!info.Directory.Exists)
				info.Directory.Create();

			using (var stream = new StreamWriter(_fileName, false))
			{
				stream.Write(mData);
			}
		}

		public StringBuilder Data
		{
			get { return mData; }
		}

		private StringBuilder mData = new StringBuilder();
	}
}
