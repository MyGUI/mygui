using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace DoxygenWrapper.Wrappers.Types
{
	public class TypeInfo
	{
		public TypeInfo(XmlNode _node)
		{
			mNode = _node;

			mName = _node.Attributes["name"].Value;

			foreach (XmlNode node in _node)
				mReplace.Add(new Pair<string, string>(node.Name, node.InnerText));
		}

		public string Name
		{
			get { return mName; }
		}

		public IEnumerable<Pair<string, string>> Values
		{
			get { return mReplace; }
		}

		private XmlNode mNode;
		private string mName;
		private List<Pair<string, string>> mReplace = new List<Pair<string, string>>();
	}
}
