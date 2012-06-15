using System;
using System.Collections.Generic;
using DoxygenWrapper.Wrappers.Compounds;
using System.Xml;
using System.IO;

namespace DoxygenWrapper.Wrappers
{
	public class CompoundManager
	{
		public CompoundManager()
		{
			RegisterFactories();
		}

		private void RegisterFactories()
		{
			mFatory.RegisterFactory("class", typeof(CompoundClass));
			mFatory.RegisterFactory("dir", typeof(CompoundDir));
			mFatory.RegisterFactory("example", typeof(CompoundExample));
			mFatory.RegisterFactory("file", typeof(CompoundFile));
			mFatory.RegisterFactory("namespace", typeof(CompoundNamespace));
			mFatory.RegisterFactory("struct", typeof(CompoundStruct));
			mFatory.RegisterFactory("typedef", typeof(CompoundTypedef));
			mFatory.RegisterFactory("enum", typeof(CompoundEnum));
			mFatory.RegisterFactory("enumvalue", typeof(CompoundEnumvalue));
			mFatory.RegisterFactory("function", typeof(CompoundFunction));
			mFatory.RegisterFactory("variable", typeof(CompoundVariable));
			mFatory.RegisterFactory("friend", typeof(CompoundFriend));
			mFatory.RegisterFactory("define", typeof(CompoundDefine));
		}

		public void Initialise(string _fileName)
		{
			XmlDocument doc = new XmlDocument();
			doc.Load(_fileName);

			ParseCompound(doc.DocumentElement, mRootCompound);

			PrintUniqumDublicates();
		}

		private void PrintUniqumDublicates()
		{
			foreach (var value in mDubCompounds)
				Console.WriteLine(string.Format("dublicate \ntype : {0}\nname : {1}\nrefid : {2}\n", value.Value.Kind, value.Value.Name, value.Key));
		}

		private void ParseCompound(XmlNode _node, Compound _parent)
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

							ParseCompound(node, compound);
						}
					}
				}
			}
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

		public void SetIgnoredTypeList(string[] _list)
		{
			mIgnoredTypes = _list;
		}

		private CompoundFactory mFatory = new CompoundFactory();
		private Dictionary<string, Compound> mCompounds = new Dictionary<string, Compound>();
		private Dictionary<string, Compound> mDubCompounds = new Dictionary<string, Compound>();
		private Compound mRootCompound = new Compound();
		private string[] mIgnoredNames = new string[] { };
		private string[] mIgnoredTypes = new string[] { };
	}
}
