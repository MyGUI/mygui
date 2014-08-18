using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using DoxygenWrapper.Wrappers.Types;
using System.IO;
using DoxygenWrapper.Wrappers.Compounds;
using DoxygenWrapper.Wrappers.Interfaces;
using DoxygenWrapper.Wrappers.Replacers;
using DoxygenWrapper.Wrappers.Compounds.Types;

namespace DoxygenWrapper.Wrappers
{
	public class WrapperManager
	{
		public static WrapperManager Instance
		{
			get { return mInstance; }
		}

		public WrapperManager()
		{
			mInstance = this;
		}

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
			{
				var info = new TypeInfo(node);
				mTypeInfos[info.Name] = info;
			}

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
			ConsoleUtility.WriteLine("Wrapp {0}", _info.Type);

			Compound compound = CompoundManager.Instance.GetCompoundByName(_info.Type);
			foreach (TemplateInfo info in _info.Templates)
				DoWrappTemplate(info, _info, compound);
		}

		private void DoWrappTemplate(TemplateInfo _info, ClassInfo _classInfo, Compound _compound)
		{
			FileData outputFile = mOutputManager.GetOutputFile(_info.Output);

			if (outputFile.Data.Length == 0)
			{
				FileData template = mTemplateManager.GetTemplateCopy(GetTemplateFileName(_info.TemplateFolder, _info.OutputTemplate));
				mReplaceManager.DoReplace(template, new IReplacer[] { _classInfo });
				AppendData(outputFile, template);
			}

			if (_info.Template != "")
			{
				FileData template = mTemplateManager.GetTemplateCopy(GetTemplateFileName(_info.TemplateFolder, _info.Template));
				mReplaceManager.DoReplace(template, new IReplacer[] { _classInfo });
				InsertData(outputFile, template, mLabelName);
			}
			else
			{
				AddClassData(_compound, _info, _classInfo, outputFile);
			}
		}

		private void AddClassData(Compound _compound, TemplateInfo _info, ClassInfo _classInfo, FileData _outputFile)
		{
			List<CompoundFunction> funtions = new List<CompoundFunction>();
			List<Pair<CompoundFunction, CompoundFunction>> properties = new List<Pair<CompoundFunction, CompoundFunction>>();
			List<CompoundVariable> variables = new List<CompoundVariable>();

			foreach (Compound child in _compound)
			{
				if (child is CompoundFunction)
				{
					CompoundFunction func = (CompoundFunction)child;
					if (GetAviableFunc(func))
					{
						if (
							//func.Const &&
							func.CompoundParamTypes.Count == 0 &&
							func.CompoundType.TypeName != "void" &&
							(func.GetProperty || func.IsProperty))
							properties.Add(new Pair<CompoundFunction, CompoundFunction>(func, null));
						else
							funtions.Add(func);
					}
				}
				else if (child is CompoundVariable)
				{
					CompoundVariable variable = (CompoundVariable)child;
					if (variable.Public &&
						!variable.Static &&
						!variable.Name.StartsWith("_") &&
						CompoundUtility.IsVariableEvent(variable))
					{
						variables.Add(variable);
					}
				}
			}

			// вытаскиваем сетеры для свойств из списка функций
			foreach (var func in properties)
				func.Second = PopSetterFunc(func.First, funtions);

			foreach (var func in properties)
				AddClassProperty(func, _info, _classInfo, _outputFile);

			foreach (var func in funtions)
				AddClassFunction(func, _info, _classInfo, _outputFile);

			foreach (var variable in variables)
				AddClassEvent(variable, _info, _classInfo, _outputFile);
		}

		private bool GetAviableFunc(CompoundFunction _func)
		{
			if (_func.Internal)
				return false;
			if (!_func.Public)
				return false;
			if (_func.Static)
				return false;
			if (_func.Name.StartsWith("_"))
				return false;
			if (_func.Reimplement)
			{
				if (_func.Virtual)
					return false;
				if (_func.Generic)
					return false;
			}
			return true;
		}

		private CompoundFunction PopSetterFunc(CompoundFunction _func, List<CompoundFunction> _funtions)
		{
			for (int index = 0; index < _funtions.Count; index++)
			{
				var func = _funtions[index];
				if (func.PropertyName == _func.PropertyName)
				{
					if (func.CompoundType.TypeName == "void" &&
						func.CompoundParamTypes.Count == 1 &&
						(func.CompoundParamTypes[0].TypeName == _func.CompoundType.TypeName ||
						func.CompoundParamTypes[0].PoorTypeName == _func.CompoundType.TypeName))
					{
						_funtions.RemoveAt(index);
						return func;
					}
				}
			}

			return null;
		}

		private void AddClassProperty(Pair<CompoundFunction, CompoundFunction> _func, TemplateInfo _info, ClassInfo _classInfo, FileData _outputFile)
		{
			string templateName = _classInfo.GetTeplaceTemplate(_func.First.Name);
			if (templateName == "")
				templateName = GetPropertyTemplateName(_func);

			FileData template = mTemplateManager.GetTemplateCopy(GetTemplateFileName(_info.TemplateFolder, templateName));
			mReplaceManager.DoReplace(template, new IReplacer[] { _classInfo, new PropertyReplacer(_func) });
			InsertData(_outputFile, template, mLabelName);
		}

		private void AddClassFunction(CompoundFunction _func, TemplateInfo _info, ClassInfo _classInfo, FileData _outputFile)
		{
			string templateName = _classInfo.GetTeplaceTemplate(_func.Name);
			if (templateName == "")
				templateName = GetFunctionTemplateName(_func);

			FileData template = mTemplateManager.GetTemplateCopy(GetTemplateFileName(_info.TemplateFolder, templateName));
			mReplaceManager.DoReplace(template, new IReplacer[] { _classInfo, new FunctionReplacer(_func) });
			InsertData(_outputFile, template, mLabelName);
		}

		private void AddClassEvent(CompoundVariable _variable, TemplateInfo _info, ClassInfo _classInfo, FileData _outputFile)
		{
			string templateName = _classInfo.GetTeplaceTemplate(_variable.Name);
			if (templateName == "")
				templateName = GetEventTemplateName(_variable);

			FileData template = mTemplateManager.GetTemplateCopy(GetTemplateFileName(_info.TemplateFolder, templateName));
			mReplaceManager.DoReplace(template, new IReplacer[] { _classInfo, new EventReplacer(_variable) });
			InsertData(_outputFile, template, mLabelName);
		}

		private string GetFunctionTemplateName(CompoundFunction _func)
		{
			bool returnType = _func.CompoundType.TypeName != "void";
			return string.Format("Method{0}{1}.txt", returnType ? "Return" : "", _func.CompoundParamTypes.Count);
		}

		private string GetPropertyTemplateName(Pair<CompoundFunction, CompoundFunction> _func)
		{
			return string.Format("Property{0}{1}.txt", _func.First.GetProperty ? "Get" : "Is", _func.Second != null ? "Set" : "");
		}

		private string GetEventTemplateName(CompoundVariable _variable)
		{
			CompoundType eventType = CompoundUtility.GetEventType(_variable);

			bool multiDelegate = CompoundUtility.IsEventMultiDelegate(eventType);
			bool request = _variable.Name.ToLowerInvariant().StartsWith("request");
			return string.Format("{0}{1}{2}.txt", multiDelegate ? "MultiDelegate" : "Delegate", request ? "Request" : "Event", eventType.TemplateTypes.Count);
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

				_target.Data.Insert(index, _source.Data);
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

		public TypeInfo GetTypeInfo(string _name)
		{
			TypeInfo result = null;
			if (mTypeInfos.TryGetValue(_name, out result))
				return result;
			return null;
		}

		private static WrapperManager mInstance;
		private FileInfo mFile;
		private Dictionary<string, TypeInfo> mTypeInfos = new Dictionary<string, TypeInfo>();
		private List<ClassInfo> mClassInfos = new List<ClassInfo>();
		private ReplaceManager mReplaceManager = new ReplaceManager();
		private OutputManager mOutputManager = new OutputManager();
		private TemplateManager mTemplateManager = new TemplateManager();
		private string mLabelName = "//InsertPoint";
	}
}
