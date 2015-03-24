using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace DoxygenWrapper.Wrappers.Compounds.Types
{
	public struct AlphabetType
	{
		public AlphabetType(string _text, XmlElement _element, StateDirection _direction)
		{
			Text = _text;
			Element = _element;
			Direction = _direction;
		}

		public readonly string Text;
		public readonly XmlElement Element;
		public readonly StateDirection Direction;
	}
}
