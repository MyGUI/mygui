using System;
using System.Collections.Generic;
using System.Xml;
using DoxygenWrapper.Wrappers.Compounds.Types;

namespace DoxygenWrapper.Wrappers.Compounds
{
	public class CompoundVariable :
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
