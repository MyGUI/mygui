using System;
using System.Collections.Generic;
using System.Xml;

namespace DoxygenWrapper.Utilities
{
	public class XmlUtility
	{
		public static void CreateXmlDeclaration(XmlDocument _doc)
		{
			XmlDeclaration declaration = _doc.CreateXmlDeclaration("1.0", "utf-8", "");
			_doc.AppendChild(declaration);
		}

		public static XmlElement CreateElement(XmlDocument _doc, string _name)
		{
			XmlElement root = _doc.CreateElement(_name);
			_doc.AppendChild(root);
			return root;
		}

		public static XmlElement CreateElement(XmlElement _node, string _name)
		{
			XmlElement node = _node.OwnerDocument.CreateElement(_name);
			_node.AppendChild(node);
			return node;
		}

		public static XmlElement CreateElement(XmlElement _node, string _name, object _value)
		{
			XmlElement node = _node.OwnerDocument.CreateElement(_name);
			_node.AppendChild(node);
			node.InnerText = _value.ToString();
			return node;
		}

		public static void CreateAttribute(XmlElement _node, string _name, object _value)
		{
			XmlAttribute attribute = _node.OwnerDocument.CreateAttribute(_name);
			attribute.Value = _value.ToString();
			_node.Attributes.Append(attribute);
		}
	}
}
