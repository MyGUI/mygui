using System;
using System.Collections.Generic;
using System.Xml;

namespace DoxygenWrapper.Wrappers.Compounds
{
	public class CompoundClass :
		CompoundContainer
	{
		protected override void OnParse(XmlNode _node)
		{
			base.OnParse(_node);

			AddBaseClasses(_node);
		}

		private void AddBaseClasses(XmlNode _node)
		{
			foreach (XmlNode node in _node.SelectNodes("basecompoundref"))
			{
				string refid = node.Attributes["refid"].Value;
				if (CompoundManager.Instance.IsIgnoredCompound(refid))
					continue;

				Compound compound = CompoundManager.Instance.GetCompound(refid);
				if (compound == null)
				{
					ConsoleUtility.WriteErrorLine("Item {0} not found", refid);
				}
				else
				{
					AddBase(compound);
				}
			}
		}

		public List<Compound>.Enumerator BaseClasses
		{
			get { return mBaseClases.GetEnumerator(); }
		}

		private void AddBase(Compound _compound)
		{
			mBaseClases.Add(_compound);
		}

		private List<Compound> mBaseClases = new List<Compound>();
	}
}
