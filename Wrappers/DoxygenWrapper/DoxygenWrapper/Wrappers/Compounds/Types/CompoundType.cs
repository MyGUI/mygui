using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace DoxygenWrapper.Wrappers.Compounds.Types
{
	public class CompoundType
	{
		public CompoundType()
		{
		}

		public CompoundType(XmlNode _node)
		{
			mNode = _node;

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
			string typeDefinition = mNode.InnerXml;
			ParseType(commands, ref indexParse, typeDefinition);

			if (indexParse != commands.Count)
				ConsoleUtility.WriteErrorLine("Error parse type {0}", typeDefinition);
			else
				ParseComplete(typeDefinition);
		}

		private void ParseComplete(string _typeDefinition)
		{
			ParseTypeComplete(_typeDefinition);

			foreach (var child in mTemplateTypes)
				child.ParseComplete(_typeDefinition);
		}

		private void ParseTypeComplete(string _typeDefinition)
		{
			if (mBeforeType != null)
			{
				mBeforeType = SplitReferenceAndPointers(mBeforeType);
				string[] values = mBeforeType.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
				foreach (string value in values)
				{
					if (value == "const")
					{
						mModifers.Add(CompoundTypeModifers.Const);
					}
					else if (value == "unsigned")
					{
						mModifers.Add(CompoundTypeModifers.Unsigned);
					}
					else if (value == "signed")
					{
						mModifers.Add(CompoundTypeModifers.Signed);
					}
					else if (value == "&")
					{
						if (mBaseTypeName != null || mBaseType != null)
						{
							mPointers.Add(CompoundTypePointers.Reference);
						}
						else
						{
							ConsoleUtility.WriteErrorLine("Error parse type {0}", _typeDefinition);
						}
					}
					else if (value == "*")
					{
						if (mBaseTypeName != null || mBaseType != null)
						{
							mPointers.Add(CompoundTypePointers.Pointer);
						}
						else
						{
							ConsoleUtility.WriteErrorLine("Error parse type {0}", _typeDefinition);
						}
					}
					else
					{
						if (mBaseTypeName == null)
						{
							mBaseTypeName = value;
						}
						else
						{
							ConsoleUtility.WriteErrorLine("Error parse type {0}", _typeDefinition);
						}
					}
				}
			}
			else if (mPostType != null)
			{
				mPostType = SplitReferenceAndPointers(mPostType);
				string[] values = mPostType.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
				foreach (string value in values)
				{
					if (value == "&")
					{
						mPointers.Add(CompoundTypePointers.Reference);
					}
					else if (value == "*")
					{
						mPointers.Add(CompoundTypePointers.Pointer);
					}
					else
					{
						ConsoleUtility.WriteErrorLine("Error parse type {0}", _typeDefinition);
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
				_value = _value.Insert(index + 1, " ");
				index = _value.IndexOfAny(template, index + 1);
			}

			return _value;
		}

		/*private bool IsSkip(string value)
		{
			return value == "T" ||
				value == "TP1" ||
				value == "TP2" ||
				value == "TP3" ||
				value == "TP4" ||
				value == "TP5" ||
				value == "TP6" ||
				value == "void(T::*" ||
				value == "void(*" ||
				value == "ListDelegate::iterator" ||
				value == "ListDelegate::const_iterator";
		}

		private bool IsFundamental(string value)
		{
			return value == "int" ||
				value == "size_t" ||
				value == "bool" ||
				value == "char" ||
				value == "short" ||
				value == "float" ||
				value == "double" ||
				value == "std::vector" ||
				value == "std::deque" ||
				value == "std::string" ||
				value == "std::basic_string" ||
				value == "std::list" ||
				value == "std::pair" ||
				value == "std::set" ||
				value == "std::map";
		}*/

		private void ParseType(List<AlphabetType> _commands, ref int _indexParse, string _typeDefinition)
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
								ConsoleUtility.WriteErrorLine("{0} id and name {1} not found in type {2}", refid, command.Element.InnerText, _typeDefinition);
							}
							else
							{
								var compound = CompoundManager.Instance.GetCompound(refid);
								if (compound == null)
								{
									ConsoleUtility.WriteErrorLine("{0} id and name {1} not found in type {2}", refid, command.Element.InnerText, _typeDefinition);
								}
								else
								{
									mBaseType = compound;
								}
							}
						}
						else
						{
							ConsoleUtility.WriteErrorLine("Error parse type {0}", _typeDefinition);
						}
					}
					else if (command.Text != null)
					{
						if (mBaseType == null)
						{
							if (mBeforeType == null)
							{
								mBeforeType = command.Text;
							}
							else
							{
								ConsoleUtility.WriteErrorLine("Error parse type {0}", _typeDefinition);
							}
						}
						else
						{
							if (mPostType == null)
							{
								mPostType = command.Text;
							}
							else
							{
								ConsoleUtility.WriteErrorLine("Error parse type {0}", _typeDefinition);
							}
						}
					}
					else
					{
						ConsoleUtility.WriteErrorLine("Error parse type {0}", _typeDefinition);
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

		public Compound BaseType
		{
			get { return mBaseType; }
		}

		public List<CompoundType>.Enumerator TemplateTypes
		{
			get { return mTemplateTypes.GetEnumerator(); }
		}

		public List<CompoundTypePointers>.Enumerator Pointers
		{
			get { return mPointers.GetEnumerator(); }
		}

		private List<CompoundTypeModifers> mModifers = new List<CompoundTypeModifers>();
		private Compound mBaseType;
		private string mBaseTypeName;
		private List<CompoundType> mTemplateTypes = new List<CompoundType>();
		private List<CompoundTypePointers> mPointers = new List<CompoundTypePointers>();
		private XmlNode mNode;
		private string mBeforeType;
		private string mPostType;
	}
}
