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
				mReplace[node.Name] = node.InnerText;
		}

		public string Name
		{
			get { return mName; }
		}

		private XmlNode mNode;
		private string mName;
		private Dictionary<string, string> mReplace = new Dictionary<string, string>();
	}
}
