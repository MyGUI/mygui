using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace DoxygenWrapper.Wrappers.Compounds.Types
{
	public class CompoundType
	{
		static CompoundType()
		{
			mModiferMap = new Dictionary<string, CompoundTypeModifers>();
			mModiferMap["const"] = CompoundTypeModifers.Const;
			mModiferMap["__inline"] = CompoundTypeModifers.Inline;
			mModiferMap["typename"] = CompoundTypeModifers.Typename;
			mModiferMap["struct"] = CompoundTypeModifers.Struct;
			mModiferMap["union"] = CompoundTypeModifers.Union;
			mModiferMap["unsigned"] = CompoundTypeModifers.Unsigned;
			mModiferMap["signed"] = CompoundTypeModifers.Signed;
			mModiferMap["&"] = CompoundTypeModifers.Reference;
			mModiferMap["*"] = CompoundTypeModifers.Pointer;

			mFundamentTypeMap = new Dictionary<string, int>();
			mFundamentTypeMap["void"] = 0;
			mFundamentTypeMap["int"] = 0;
			mFundamentTypeMap["size_t"] = 0;
			mFundamentTypeMap["wchar_t"] = 0;
			mFundamentTypeMap["long"] = 0;
			mFundamentTypeMap["bool"] = 0;
			mFundamentTypeMap["char"] = 0;
			mFundamentTypeMap["short"] = 0;
			mFundamentTypeMap["float"] = 0;
			mFundamentTypeMap["double"] = 0;

			mFundamentTypeMap["std::type_info"] = 0;
			mFundamentTypeMap["std::list"] = 0;
			mFundamentTypeMap["std::pair"] = 0;
			mFundamentTypeMap["std::set"] = 0;
			mFundamentTypeMap["std::map"] = 0;
			mFundamentTypeMap["std::vector"] = 0;
			mFundamentTypeMap["std::deque"] = 0;
			mFundamentTypeMap["std::string"] = 0;
			mFundamentTypeMap["std::wstring"] = 0;
			mFundamentTypeMap["std::basic_string"] = 0;

			mFundamentTypeMap["std::istream"] = 0;
			mFundamentTypeMap["std::ifstream"] = 0;
			mFundamentTypeMap["std::wistream"] = 0;
			mFundamentTypeMap["std::wifstream"] = 0;
			mFundamentTypeMap["std::ostream"] = 0;
			mFundamentTypeMap["std::ofstream"] = 0;
			mFundamentTypeMap["std::ostringstream"] = 0;
			mFundamentTypeMap["std::wostream"] = 0;
			mFundamentTypeMap["std::wofstream"] = 0;
			mFundamentTypeMap["std::wostringstream"] = 0;
		}

		public CompoundType()
		{
		}

		public CompoundType(XmlNode _node, string _valueName)
		{
			mNode = _node;
			mValueName = _valueName;

			List<AlphabetType> commands = new List<AlphabetType>();

			foreach (XmlNode node in _node)
			{
				if (node.NodeType == XmlNodeType.Text)
				{
					string value = node.Value;
					char[] template = new char[] { '<', ',', '>' };
					int index = value.IndexOfAny(template);

					while (index != -1)
					{
						string valueCut = value.Substring(0, index);
						valueCut = valueCut.Trim();
						if (valueCut != "")
							commands.Add(new AlphabetType(valueCut, null, StateDirection.None));

						char command = value[index];
						if (command == '<')
							commands.Add(new AlphabetType("", null, StateDirection.Down));
						else if (command == '>')
							commands.Add(new AlphabetType("", null, StateDirection.Up));
						else if (command == ',')
						{
							commands.Add(new AlphabetType("", null, StateDirection.Up));
							commands.Add(new AlphabetType("", null, StateDirection.Down));
						}

						value = value.Substring(index + 1);
						index = value.IndexOfAny(template);
					}

					value = value.Trim();
					if (value != "")
						commands.Add(new AlphabetType(value, null, StateDirection.None));
				}
				else if (node.NodeType == XmlNodeType.Element)
				{
					commands.Add(new AlphabetType("", (XmlElement)node, StateDirection.None));
				}
			}

			int indexParse = 0;
			ParseType(commands, ref indexParse, mNode);

			if (indexParse != commands.Count)
				ConsoleUtility.WriteErrorLine("Error parse type {0}", mNode.InnerXml);
			else
				ParseComplete(mNode);
		}

		private string GetTypeName(bool _poor)
		{
			string result = "";

			if (!_poor)
			{
				foreach (var modifer in mModifers)
				{
					if (modifer == CompoundTypeModifers.Const)
						result += "const ";
				}
			}

			foreach (var modifer in mModifers)
			{
				if (modifer == CompoundTypeModifers.Unsigned)
					result += "unsigned ";
				else if (modifer == CompoundTypeModifers.Signed)
					result += "signed ";
			}

			if (mBaseType != null)
			{
				if (mBaseType is CompoundMember)
					result += ((CompoundMember)mBaseType).CompoundType.TypeName + " ";
				else if (mBaseType is CompoundClass)
					result += ((CompoundClass)mBaseType).Name + " ";
			}
			else
			{
				result += mBaseTypeName + " ";
			}

			if (mTemplateTypes.Count != 0)
			{
				result += "< ";
				for (int index = 0; index < mTemplateTypes.Count; index++)
				{
					result += mTemplateTypes[index].GetTypeName(false) + " ";

					if (index + 1 < mTemplateTypes.Count)
						result += ", ";
				}
				result += "> ";
			}

			if (!_poor)
			{
				foreach (var modifer in mModifers)
				{
					if (modifer == CompoundTypeModifers.Pointer)
						result += "* ";
					else if (modifer == CompoundTypeModifers.Reference)
						result += "& ";
				}
			}

			result = result.TrimEnd();

			return result;
		}

		private void ParseComplete(XmlNode _typeDefinition)
		{
			ParseTypeComplete(_typeDefinition);

			foreach (var child in mTemplateTypes)
				child.ParseComplete(_typeDefinition);
		}

		private void ParseTypeComplete(XmlNode _typeDefinition)
		{
			if (mParseType != null)
			{
				mParseType = SplitReferenceAndPointers(mParseType);
				string[] values = mParseType.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
				foreach (string value in values)
				{
					CompoundTypeModifers modiffer = CompoundTypeModifers.None;
					if (mModiferMap.TryGetValue(value, out modiffer))
					{
						mModifers.Add(modiffer);
					}
					else if (mFundamentTypeMap.ContainsKey(value))
					{
						mBaseTypeName = value;
					}
					else
					{
						if (mBaseTypeName == null)
						{
							mBaseTypeName = value;
						}
						else
						{
							ConsoleUtility.WriteErrorLine("Error parse type {0}", _typeDefinition.InnerXml);
						}
					}
				}
			}
		}

		private string SplitReferenceAndPointers(string _value)
		{
			char[] template = new char[] { '*', '&' };
			int index = _value.IndexOfAny(template, 0);
			while (index != -1)
			{
				_value = _value.Insert(index, " ");
				index = _value.IndexOfAny(template, index + 2);
			}

			return _value;
		}

		private void ParseType(List<AlphabetType> _commands, ref int _indexParse, XmlNode _typeDefinition)
		{
			while (true)
			{
				if (_indexParse >= _commands.Count)
					break;

				AlphabetType command = _commands[_indexParse];
				_indexParse ++;
				if (command.Direction == StateDirection.Up)
				{
					break;
				}
				else if (command.Direction == StateDirection.Down)
				{
					CompoundType type = new CompoundType();
					mTemplateTypes.Add(type);

					type.ParseType(_commands, ref _indexParse, _typeDefinition);
				}
				else
				{
					if (command.Element != null)
					{
						if (mBaseType == null)
						{
							string refid = command.Element.Attributes["refid"].Value;
							if (CompoundManager.Instance.IsIgnoredCompound(refid))
							{
								ConsoleUtility.WriteErrorLine("{0} id and name {1} not found in type {2}", refid, command.Element.InnerText, _typeDefinition.InnerXml);
							}
							else
							{
								var compound = CompoundManager.Instance.GetCompound(refid);
								if (compound == null)
								{
									ConsoleUtility.WriteErrorLine("{0} id and name {1} not found in type {2}", refid, command.Element.InnerText, _typeDefinition.InnerXml);
								}
								else
								{
									mBaseType = compound;
								}
							}
						}
						else
						{
							ConsoleUtility.WriteErrorLine("Error parse type {0}", _typeDefinition.InnerXml);
						}
					}
					else if (command.Text != null)
					{
						mParseType += command.Text;
					}
					else
					{
						ConsoleUtility.WriteErrorLine("Error parse type {0}", _typeDefinition.InnerXml);
					}
				}
			}
		}

		public List<CompoundTypeModifers>.Enumerator Modifers
		{
			get { return mModifers.GetEnumerator(); }
		}

		public string BaseTypeName
		{
			get { return mBaseTypeName; }
		}

		public string ValueName
		{
			get { return mValueName; }
		}

		public string TypeName
		{
			get
			{
				if (mTypeName == null)
					mTypeName = GetTypeName(false);
				return mTypeName;
			}
		}

		public string PoorTypeName
		{
			get
			{
				if (mPoorTypeName == null)
					mPoorTypeName = GetTypeName(true);
				return mPoorTypeName;
			}
		}

		public Compound BaseType
		{
			get { return mBaseType; }
		}

		public List<CompoundType> TemplateTypes
		{
			get { return mTemplateTypes; }
		}

		private List<CompoundTypeModifers> mModifers = new List<CompoundTypeModifers>();
		private Compound mBaseType;
		private string mBaseTypeName;
		private List<CompoundType> mTemplateTypes = new List<CompoundType>();
		private XmlNode mNode;
		private string mParseType = "";
		private string mValueName = "";
		private string mTypeName;
		private string mPoorTypeName;
		private static Dictionary<string, CompoundTypeModifers> mModiferMap;
		private static Dictionary<string, int> mFundamentTypeMap;
	}
}
