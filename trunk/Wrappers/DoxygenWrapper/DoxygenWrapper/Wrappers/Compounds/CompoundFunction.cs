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
		}

		public CompoundType CompoundType
		{
			get { return mCompoundType; }
		}

		public IEnumerable<CompoundType> CompoundParamTypes
		{
			get { return mCompoundParamTypes; }
		}

		public int CompoundParamTypesCount
		{
			get { return mCompoundParamTypes.Count; }
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

		private CompoundType mCompoundType;
		private List<CompoundType> mCompoundParamTypes = new List<CompoundType>();
		private bool mPublic;
		private bool mStatic;
		private bool mInternal;
	}
}
