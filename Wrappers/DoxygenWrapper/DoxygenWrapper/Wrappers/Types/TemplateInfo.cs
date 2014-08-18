using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace DoxygenWrapper.Wrappers.Types
{
	public class TemplateInfo
	{
		public TemplateInfo(XmlNode _node)
		{
			mNode = _node;

			mTemplate = _node.Attributes["template"] != null ? _node.Attributes["template"].Value : "";
			mTemplateFolder = _node.Attributes["type"] != null ? _node.Attributes["type"].Value : "";
			mOutputTemplate = _node.Attributes["name"] != null ? _node.Attributes["name"].Value : "";
			mOutput = _node.Attributes["output"] != null ? _node.Attributes["output"].Value : "";
		}

		public string Template
		{
			get { return mTemplate; }
		}

		public string TemplateFolder
		{
			get { return mTemplateFolder; }
		}

		public string OutputTemplate
		{
			get { return mOutputTemplate; }
		}

		public string Output
		{
			get { return mOutput; }
		}

		private XmlNode mNode;
		private string mTemplate;
		private string mTemplateFolder;
		private string mOutputTemplate;
		private string mOutput;
	}
}
