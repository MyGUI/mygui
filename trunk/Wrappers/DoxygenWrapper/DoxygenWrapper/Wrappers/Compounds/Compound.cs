using System;
using System.Collections.Generic;
using System.Xml;

namespace DoxygenWrapper.Wrappers.Compounds
{
	public class Compound
	{
		public string RefID
		{
			get { return mRefID; }
		}

		public string Kind
		{
			get { return mKind; }
		}

		public string Name
		{
			get { return mName; }
		}

		public virtual void Initialise(XmlNode _node)
		{
			mRefID = _node.Attributes["refid"].Value;
			mKind = _node.Attributes["kind"].Value;
			mName = _node["name"].InnerText;
		}

		public void AddChild(Compound _child)
		{
			mChilds.Add(_child);
		}

		private string mRefID;
		private string mKind;
		private string mName;
		private List<Compound> mChilds = new List<Compound>();
	}
}
