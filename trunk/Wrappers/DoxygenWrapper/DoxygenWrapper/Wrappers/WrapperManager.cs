using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using DoxygenWrapper.Wrappers.Types;
using System.IO;

namespace DoxygenWrapper.Wrappers
{
	public class WrapperManager
	{
		public void Initialise(string _fileName)
		{
			mFile = new FileInfo(_fileName);

			InitialiseData(_fileName);
			DoWrapp();
			mOutputManager.Save();
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

		private void DoWrapp()
		{
			foreach (ClassInfo info in mClassInfos)
				DoWrappClass(info);
		}

		private void DoWrappClass(ClassInfo _info)
		{
			foreach (TemplateInfo info in _info.Templates)
				DoWrappTemplate(info, _info);
		}

		private void DoWrappTemplate(TemplateInfo _info, ClassInfo _classInfo)
		{
			FileData outputFile = mOutputManager.GetOutputFile(_info.Output);

			if (outputFile.Data.Length == 0)
			{
				FileData template = mTemplateManager.GetTemplateCopy(GetTemplateFileName(_info.TemplateFolder, _info.OutputTemplate));
				mReplaceManager.DoReplace(template, _classInfo);
				AppendData(outputFile, template);
			}

			if (_info.Template != "")
			{
				FileData template = mTemplateManager.GetTemplateCopy(GetTemplateFileName(_info.TemplateFolder, _info.Template));
				mReplaceManager.DoReplace(template, _classInfo);
				InsertData(outputFile, template, "//InsertPoint");
			}
		}

		private void InsertData(FileData _target, FileData _source, string _label)
		{
			int index = Find(_target.Data, _label);
			if (index != -1)
			{
				index += _label.Length;
				while (index < _target.Data.Length)
				{
					if (_target.Data[index] != '\n' && _target.Data[index] != '\r')
					{
						break;
					}
					index++;
				}

				if (index < _target.Data.Length)
				{
					_target.Data.Insert(index, _source.Data);
				}
			}
		}

		private int Find(StringBuilder _source, string _subString)
		{
			for (int index = 0; index < _source.Length - _subString.Length; index++)
			{
				bool find = true;
				for (int innerIndex = 0; innerIndex < _subString.Length; innerIndex++)
				{
					if (_source[index + innerIndex] != _subString[innerIndex])
					{
						find = false;
						break;
					}
				}
				if (find)
					return index;
			}
			return -1;
		}

		private void AppendData(FileData _target, FileData _source)
		{
			_target.Data.Append(_source.Data);
		}

		private string GetTemplateFileName(string _folder, string _fileName)
		{
			return Path.Combine(mFile.DirectoryName, Path.Combine(_folder, _fileName));
		}

		private FileInfo mFile;
		private List<TypeInfo> mTypeInfos = new List<TypeInfo>();
		private List<ClassInfo> mClassInfos = new List<ClassInfo>();
		private ReplaceManager mReplaceManager = new ReplaceManager();
		private OutputManager mOutputManager = new OutputManager();
		private TemplateManager mTemplateManager = new TemplateManager();
	}
}
