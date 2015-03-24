using System;
using System.Collections.Generic;
using DoxygenWrapper.Wrappers.Compounds;
using System.Xml;
using System.IO;
using DoxygenWrapper.Utilities;

namespace DoxygenWrapper.Wrappers
{
	public class CompoundManager
	{
		public static CompoundManager Instance { get { return mInstance; } }

		public CompoundManager()
		{
			mInstance = this;

			RegisterFactories();
		}

		private void RegisterFactories()
		{
			mFatory.RegisterFactory("class", typeof(CompoundClass));
			mFatory.RegisterFactory("struct", typeof(CompoundClass));
			mFatory.RegisterFactory("namespace", typeof(CompoundContainer));
			mFatory.RegisterFactory("typedef", typeof(CompoundTypedef));
			mFatory.RegisterFactory("function", typeof(CompoundFunction));
			mFatory.RegisterFactory("variable", typeof(CompoundVariable));
			mFatory.RegisterFactory("enum", typeof(Compound));
			mFatory.RegisterFactory("enumvalue", typeof(Compound));
		}

		public void Initialise(string _folder)
		{
			mFolder = _folder;

			XmlDocument doc = new XmlDocument();
			doc.Load(Path.Combine(mFolder, "index.xml"));

			ConsoleUtility.WriteLine("Initialise items ...");

			InitialiseCompounds(doc.DocumentElement, mRootCompound);

			ConsoleUtility.WriteLine("{0} items initialised", mCompounds.Count);

			PrintUniqumDublicates();

			ConsoleUtility.WriteLine("Parse items ...");

			ParseCompounds();
		}

		private void ParseCompounds()
		{
			foreach (var compound in mCompounds)
			{
				if (!compound.Value.Member)
					compound.Value.Parse(mFolder);
			}
		}

		private void PrintUniqumDublicates()
		{
			foreach (var value in mDubCompounds)
				ConsoleUtility.WriteErrorLine("dublicate \ntype : {0}\nname : {1}\nrefid : {2}\n", value.Value.Kind, value.Value.Name, value.Key);
		}

		private void InitialiseCompounds(XmlNode _node, Compound _parent)
		{
			ParseCompoundByNodeName(_node, _parent, "compound");
			ParseCompoundByNodeName(_node, _parent, "member");
		}

		private void ParseCompoundByNodeName(XmlNode _node, Compound _parent, string _name)
		{
			foreach (XmlNode node in _node.SelectNodes(_name))
			{
				string type = node.Attributes["kind"].Value;
				if (!IsTypeIgnored(type))
				{
					var compound = mFatory.CreateCompound(type);
					if (compound != null)
					{
						compound.Initialise(node);

						if (!IsNameIgnored(compound.Name))
						{
							RegisterCompound(compound);

							_parent.AddChild(compound);

							InitialiseCompounds(node, compound);
						}
						else
						{
							AddToIgnoredList(compound);
						}
					}
				}
			}
		}

		private void AddToIgnoredList(Compound _compound)
		{
			mIgnoredItems.Add(_compound);
		}

		private bool IsTypeIgnored(string _type)
		{
			foreach (var name in mIgnoredTypes)
			{
				if (name == _type)
					return true;
			}
			return false;
		}

		private bool IsNameIgnored(string _name)
		{
			foreach (var name in mIgnoredNames)
			{
				if (name == _name)
					return true;
			}
			return false;
		}

		public void RegisterCompound(Compound _compound)
		{
			if (mCompounds.ContainsKey(_compound.RefID))
				mDubCompounds[_compound.RefID] = _compound;

			mCompounds[_compound.RefID] = _compound;
		}

		public void SetIgnoredNameList(string[] _list)
		{
			mIgnoredNames = _list;
		}

		public Compound GetCompound(string _refid)
		{
			if (mCompounds.ContainsKey(_refid))
				return mCompounds[_refid];
			return null;
		}

		public bool IsIgnoredCompound(string _refid)
		{
			foreach (var compound in mIgnoredItems)
			{
				if (compound.RefID == _refid)
					return true;
			}
			return false;
		}

		public bool IsIgnoredType(string _kind)
		{
			foreach (string type in mIgnoredTypes)
			{
				if (type == _kind)
					return true;
			}
			return false;
		}

		public void DumpCompoundTree(string _fileName)
		{
			if (File.Exists(_fileName))
				File.Delete(_fileName);

			XmlDocument doc = new XmlDocument();
			XmlUtility.CreateXmlDeclaration(doc);
			XmlElement root = XmlUtility.CreateElement(doc, "Root");

			WriteCompound(root, mRootCompound);

			doc.Save(_fileName);
		}

		private void WriteCompound(XmlElement _node, Compound _compound)
		{
			foreach (Compound child in _compound)
			{
				XmlElement node = XmlUtility.CreateElement(_node, child.Kind);
				XmlUtility.CreateAttribute(node, "name", child.Name);
				XmlUtility.CreateAttribute(node, "id", child.RefID);
				WriteCompound(node, child);
			}
		}

		public Compound GetCompoundByName(string _name)
		{
			foreach (var item in mCompounds)
			{
				if (item.Value.Name == _name)
					return item.Value;
			}

			return null;
		}

		private static CompoundManager mInstance = null;
		private CompoundFactory mFatory = new CompoundFactory();
		private Dictionary<string, Compound> mCompounds = new Dictionary<string, Compound>();
		private Dictionary<string, Compound> mDubCompounds = new Dictionary<string, Compound>();
		private List<Compound> mIgnoredItems = new List<Compound>();
		private Compound mRootCompound = new Compound();
		private string[] mIgnoredNames = new string[] { };
		private string[] mIgnoredTypes = new string[] { "define", "friend", "file", "example", "dir" };
		private string mFolder;
	}
}
