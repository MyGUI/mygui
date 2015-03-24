using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using DoxygenWrapper.Wrappers.Interfaces;

namespace DoxygenWrapper.Wrappers.Types
{
	public class ClassInfo :
		IReplacer
	{
		public ClassInfo(XmlNode _node)
		{
			mNode = _node;

			mType = _node.Attributes["type"].Value;

			foreach (XmlNode node in _node.SelectNodes("Tag"))
				mReplace[node.Attributes["key"].Value] = node.Attributes["value"].Value;

			foreach (XmlNode node in _node.SelectNodes("Member"))
				mReplaceTemplate[node.Attributes["name"].Value] = node.Attributes["template"].Value;

			foreach (XmlNode node in _node.SelectNodes("Template"))
				mTemplates.Add(new TemplateInfo(node));
		}

		public string Type
		{
			get { return mType; }
		}

		public IEnumerable<TemplateInfo> Templates
		{
			get { return mTemplates; }
		}

		public string GetTeplaceTemplate(string _name)
		{
			string result = null;
			if (mReplaceTemplate.TryGetValue(_name, out result))
				return result;
			return "";
		}

		public string Replace(string _tag)
		{
			string result = null;
			if (mReplace.TryGetValue(_tag, out result))
				return result;
			return "";
		}

		private XmlNode mNode;
		private string mType;
		private Dictionary<string, string> mReplace = new Dictionary<string, string>();
		private Dictionary<string, string> mReplaceTemplate = new Dictionary<string, string>();
		private List<TemplateInfo> mTemplates = new List<TemplateInfo>();
	}
}
