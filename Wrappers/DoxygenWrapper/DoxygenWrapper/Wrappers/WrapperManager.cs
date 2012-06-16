using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using DoxygenWrapper.Wrappers.Types;

namespace DoxygenWrapper.Wrappers
{
	public class WrapperManager
	{
		public void Initialise(string _fileName)
		{
			mFileName = _fileName;

			InitialiseData(_fileName);
		}

		private void InitialiseData(string _fileName)
		{
			XmlDocument doc = new XmlDocument();
			doc.Load(_fileName);

			mTypeInfos.Clear();
			foreach (XmlNode node in doc.SelectNodes("Root/TypeHolder/TypeInfo"))
				mTypeInfos.Add(new TypeInfo(node));

			mClassInfos.Clear();
			foreach (XmlNode node in doc.SelectNodes("Root/Item"))
				mClassInfos.Add(new ClassInfo(node));
		}

		private string mFileName;
		private List<TypeInfo> mTypeInfos = new List<TypeInfo>();
		private List<ClassInfo> mClassInfos = new List<ClassInfo>();
	}
}
