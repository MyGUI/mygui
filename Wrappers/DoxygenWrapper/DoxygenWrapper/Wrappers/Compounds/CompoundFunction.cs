using System;
using System.Collections.Generic;
using System.Xml;
using DoxygenWrapper.Wrappers.Compounds.Types;

namespace DoxygenWrapper.Wrappers.Compounds
{
	public class CompoundFunction :
		Compound
	{
		protected override void OnParse(XmlNode _node)
		{
			base.OnParse(_node);

			mCompoundType = new CompoundType(_node["type"], "");
			foreach (XmlNode node in _node.SelectNodes("param"))
				mCompoundParamTypes.Add(new CompoundType(node["type"], node["declname"] != null ? node["declname"].InnerText : ""));

			mPublic = _node.Attributes["prot"].Value == "public";
			mStatic = _node.Attributes["static"].Value == "yes";
			mInternal = _node["type"].InnerText == "";
			mGeneric = _node["templateparamlist"] != null;
			mConst = _node.Attributes["const"].Value == "yes";
			mVirtual = _node.Attributes["virt"].Value == "virtual";
			mReimplement = _node.SelectNodes("reimplements").Count != 0;

			string tmpGetName = "get";
			string tmpSetName = "set";
			string tmpIsName = "is";
			if (Name.ToLowerInvariant().StartsWith(tmpGetName))
			{
				mPropertyName = Name.Substring(tmpGetName.Length);
				mGetProperty = true;
			}
			else if (Name.ToLowerInvariant().StartsWith(tmpSetName))
			{
				mPropertyName = Name.Substring(tmpSetName.Length);
			}
			else if (Name.ToLowerInvariant().StartsWith(tmpIsName))
			{
				mPropertyName = Name.Substring(tmpIsName.Length);
				mIsProperty = true;
			}
		}

		public CompoundType CompoundType
		{
			get { return mCompoundType; }
		}

		public List<CompoundType> CompoundParamTypes
		{
			get { return mCompoundParamTypes; }
		}

		public bool Public
		{
			get { return mPublic; }
		}

		public bool Static
		{
			get { return mStatic; }
		}

		public bool Internal
		{
			get { return mInternal; }
		}

		public bool Generic
		{
			get { return mGeneric; }
		}

		public bool Const
		{
			get { return mConst; }
		}

		public string PropertyName
		{
			get { return mPropertyName; }
		}

		public bool GetProperty
		{
			get { return mGetProperty; }
		}

		public bool IsProperty
		{
			get { return mIsProperty; }
		}

		public bool Virtual
		{
			get { return mVirtual; }
		}

		public bool Reimplement
		{
			get { return mReimplement; }
		}

		private CompoundType mCompoundType;
		private List<CompoundType> mCompoundParamTypes = new List<CompoundType>();
		private bool mPublic;
		private bool mStatic;
		private bool mInternal;
		private bool mGeneric;
		private bool mConst;
		private string mPropertyName;
		private bool mGetProperty;
		private bool mIsProperty;
		private bool mVirtual;
		private bool mReimplement;
	}
}
