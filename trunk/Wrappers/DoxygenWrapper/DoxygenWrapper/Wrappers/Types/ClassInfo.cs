using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace DoxygenWrapper.Wrappers.Types
{
	public class ClassInfo
	{
		public ClassInfo(XmlNode _node)
		{
			mNode = _node;

			foreach (XmlNode node in _node.SelectNodes("Tag"))
				mReplace[node.Attributes["key"].Value] = node.Attributes["value"].Value;

			foreach (XmlNode node in _node.SelectNodes("Member"))
				mReplaceTemplate[node.Attributes["name"].Value] = node.Attributes["template"].Value;

			foreach (XmlNode node in _node.SelectNodes("Template"))
				mTemplates.Add(new TemplateInfo(node));
		}

		public IEnumerable<TemplateInfo> Templates
		{
			get { return mTemplates; }
		}

		public string Replace(string _tag)
		{
			string result = null;
			if (mReplace.TryGetValue(_tag, out result))
				return result;
			return "";
		}

		private XmlNode mNode;
		private Dictionary<string, string> mReplace = new Dictionary<string, string>();
		private Dictionary<string, string> mReplaceTemplate = new Dictionary<string, string>();
		private List<TemplateInfo> mTemplates = new List<TemplateInfo>();
	}
}
