using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace DoxygenWrapper.Wrappers.Compounds
{
	public class CompoundVariable :
		CompoundMember
	{
		protected override void OnParse(XmlNode _node)
		{
			base.OnParse(_node);

			mPublic = _node.Attributes["prot"].Value == "public";
			mStatic = _node.Attributes["static"].Value == "yes";

			string tmpEventName = "event";
			string tmpRequestName = "request";
			if (Name.ToLowerInvariant().StartsWith(tmpEventName))
			{
				mEventName = Name.Substring(tmpEventName.Length);
			}
			else if (Name.ToLowerInvariant().StartsWith(tmpRequestName))
			{
				mEventName = Name.Substring(tmpRequestName.Length);
			}

			foreach (XmlNode node in _node.SelectNodes("detaileddescription/para/parameterlist/parameteritem/parameternamelist/parametername"))
				mValueNames.Add(node.InnerText);
		}

		public bool Public
		{
			get { return mPublic; }
		}

		public bool Static
		{
			get { return mStatic; }
		}

		public string EventName
		{
			get { return mEventName; }
		}

		public List<string> ValueNames
		{
			get { return mValueNames; }
		}

		private bool mPublic;
		private bool mStatic;
		private string mEventName;
		private List<string> mValueNames = new List<string>();
	}
}
