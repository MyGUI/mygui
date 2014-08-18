using System;
using System.Collections.Generic;
using System.Xml;

namespace DoxygenWrapper.Wrappers.Compounds
{
	public class CompoundContainer :
		Compound
	{
		protected override void OnParse(XmlNode _node)
		{
			base.OnParse(_node);

			AddChildCompounds(_node, "innerclass");
			AddChildCompounds(_node, "innernamespace");

			ParseChildItems(_node);
		}

		private void ParseChildItems(XmlNode _node)
		{
			foreach (XmlNode node in _node.SelectNodes("sectiondef/memberdef"))
			{
				string id = node.Attributes["id"].Value;
				if (CompoundManager.Instance.IsIgnoredCompound(id))
					continue;

				string kind = node.Attributes["kind"].Value;
				if (CompoundManager.Instance.IsIgnoredType(kind))
					continue;

				Compound compound = CompoundManager.Instance.GetCompound(id);
				if (compound == null)
				{
					ConsoleUtility.WriteErrorLine("Item {0} not found (for this {1})", id, RefID);
				}
				else
				{
					if (compound.Parent != this)
					{
						ConsoleUtility.WriteErrorLine("Item {0} not added for {1}", compound.RefID, RefID);
					}
					else
					{
						compound.Parse(node);
					}
				}
			}
		}

		private void AddChildCompounds(XmlNode _node, string _name)
		{
			foreach (XmlNode node in _node.SelectNodes(_name))
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
					AddChild(compound);
				}
			}
		}
	}
}
