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
			//XmlDocument doc = new XmlDocument();
			//doc.LoadXml("<Document><Node1><InnerNode2> test2 </InnerNode2> test3 </Node1></Document>");
			//doc.Load(@"D:\MyGUI\MyGUI_Trunk\Wrappers\WrapperGenerator\doxygen\xml\classMyGUI_1_1WidgetInput.xml");
			CompoundManager manager = new CompoundManager();
			manager.SetIgnoredNameList(new string[] { "MyGUI::Allocator", "MyGUI::Allocator::rebind" });
			manager.SetIgnoredTypeList(new string[] { "define", "friend", "enumvalue", "enum", "file", "example", "dir" });
			manager.Initialise(@"D:\MyGUI\MyGUI_Trunk\Wrappers\WrapperGenerator\doxygen\xml\index.xml");
		}
	}
}
