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
			CompoundManager compoundManager = new CompoundManager();
			compoundManager.SetIgnoredNameList(new string[] { "MyGUI::Allocator", "MyGUI::Allocator::rebind" });
			compoundManager.Initialise(@"..\..\..\..\WrapperGenerator\doxygen\xml");
			compoundManager.DumpCompoundTree("CompoundTree.xml");

			WrapperManager wrapperManager = new WrapperManager();
			wrapperManager.Initialise(@"..\..\..\..\WrapperGenerator\Data\SharpData.xml");

			ConsoleUtility.WriteLine("Complite");
		}
	}
}
