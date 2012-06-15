using System;
using System.Collections.Generic;
using System.Xml;
using DoxygenWrapper.Wrappers;

namespace DoxygenWrapper
{
	class Program
	{
		private static void Main(string[] _args)
		{
			CompoundManager manager = new CompoundManager();
			manager.SetIgnoredNameList(new string[] { "MyGUI::Allocator", "MyGUI::Allocator::rebind" });
			manager.Initialise(@"..\..\..\..\WrapperGenerator\doxygen\xml");

			manager.DumpCompoundTree("CompoundTree.xml");
		}
	}
}
