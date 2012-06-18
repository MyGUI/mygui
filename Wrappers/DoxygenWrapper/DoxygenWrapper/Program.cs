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
			compoundManager.Initialise(@"..\..\..\doxygen\xml");
			//compoundManager.DumpCompoundTree("CompoundTree.xml");

			WrapperManager wrapperManager = new WrapperManager();
			//wrapperManager.Initialise(@"..\..\..\Data\SharpData.xml");
			wrapperManager.Initialise(@"..\..\..\Data\ManagedData.xml");

			ConsoleUtility.WriteLine("Complite");
		}
	}
}
