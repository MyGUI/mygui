using System;
using System.Collections.Generic;
using DoxygenWrapper.Wrappers.Compounds.Types;
using System.Xml;

namespace DoxygenWrapper.Wrappers.Compounds
{
	public class CompoundMember:
		Compound
	{
		protected override void OnParse(XmlNode _node)
		{
			base.OnParse(_node);

			mCompoundType = new CompoundType(_node["type"], _node["name"].Value);
		}

		public CompoundType CompoundType
		{
			get { return mCompoundType; }
		}

		private CompoundType mCompoundType;
	}
}
