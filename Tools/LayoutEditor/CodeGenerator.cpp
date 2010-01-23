/*!
	@file
	@author		George Evmenov
	@date		01/2010
	@module
*/

#include "precompiled.h"
#include "CodeGenerator.h"
#include "EditorWidgets.h"

// FIXME
const std::string TemplateName = "BaseLayoutCPP.xml";

CodeGenerator::CodeGenerator()
{
	initialiseByAttributes(this);
	mGenerateButton->eventMouseButtonClick = MyGUI::newDelegate(this, &CodeGenerator::notifyGeneratePressed);

	MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("LECodeTemplate") = MyGUI::newDelegate(this, &CodeGenerator::parseTemplate);
	MyGUI::ResourceManager::getInstance().load(TemplateName);
}

CodeGenerator::~CodeGenerator()
{
}

void CodeGenerator::parseTemplate(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
{
	mTemplateFiles.clear();
	mTemplateStrings.clear();

	MyGUI::xml::ElementEnumerator file = _node->getElementEnumerator();
	while (file.next("File"))
	{
		std::string templateFile = file->findAttribute("template");
		std::string outputFile = file->findAttribute("out_file");
		mTemplateFiles.insert(PairString(templateFile, outputFile));
	}

	MyGUI::xml::ElementEnumerator string = _node->getElementEnumerator();
	while (string.next("String"))
	{
		std::string key = string->findAttribute("key");
		std::string value = string->findAttribute("value");
		mTemplateStrings.insert(PairString(key, value));
	}
}

std::string CodeGenerator::stringToUpperCase(std::string _str)
{
	for(unsigned int i=0;i<_str.length();i++)
	{
		_str[i] = toupper(_str[i]);
	}
	return _str;
}

void CodeGenerator::printWidgetDeclaration(WidgetContainer* _container, std::ofstream& _stream)
{
	if (!_container->name.empty() && _container->name != "_Main")
	{
		MyGUI::LanguageManager& lm = MyGUI::LanguageManager::getInstance();
		lm.addUserTag("Widget_Name", _container->name);
		lm.addUserTag("Widget_Type", _container->type);
		for (StringPairs::iterator iterS = mTemplateStrings.begin(); iterS != mTemplateStrings.end(); ++iterS)
		{
			lm.addUserTag(iterS->first, lm.replaceTags(iterS->second));
		}
		std::string declaration = lm.getTag("Widget_Declaration");
		while (declaration.find("\\n") != std::string::npos)
			declaration.replace(declaration.find("\\n"), 2, "\n");
		_stream << declaration;
	}

	for (std::vector<WidgetContainer*>::iterator iter = _container->childContainers.begin(); iter != _container->childContainers.end(); ++iter)
	{
		printWidgetDeclaration(*iter, _stream);
	}
}

void CodeGenerator::notifyGeneratePressed(MyGUI::Widget* _sender)
{
	MyGUI::LanguageManager& lm = MyGUI::LanguageManager::getInstance();
	std::string panelName = mPanelNameEdit->getCaption();
	std::string includeDirectory = mIncludeDirectoryEdit->getCaption();
	std::string sourceDirectory = mSourceDirectoryEdit->getCaption();

	lm.addUserTag("Panel_Name", panelName);
	lm.addUserTag("Include_Directory", includeDirectory);
	lm.addUserTag("Source_Directory", sourceDirectory);
	lm.addUserTag("Uppercase_Panel_Name", stringToUpperCase(panelName));

	for (StringPairs::iterator iter = mTemplateFiles.begin(); iter != mTemplateFiles.end(); ++iter)
	{
		std::ifstream input_file(MyGUI::DataManager::getInstance().getDataPath(iter->first).c_str());
		std::ofstream output_file(lm.replaceTags(iter->second).asUTF8_c_str());
		while (!input_file.eof() && !input_file.fail() && !output_file.fail())
		{
			char str[256];
			input_file.getline(str, sizeof(str));
			output_file << lm.replaceTags(str) << std::endl;
			if (strcmp(str, "//%LE Widget_Declaration list start") == 0)
			{
				EditorWidgets& ew = EditorWidgets::getInstance();
				for (std::vector<WidgetContainer*>::iterator iter = ew.widgets.begin(); iter != ew.widgets.end(); ++iter )
				{
					printWidgetDeclaration(*iter, output_file);
				}
			}
		}
		output_file.close();
		input_file.close();
	}
	
	getMainWidget()->setVisible(false);
}

void CodeGenerator::loadProperties(MyGUI::xml::ElementEnumerator _field)
{
	MyGUI::xml::ElementEnumerator field = _field->getElementEnumerator();

	while (field.next("Property"))
	{
		std::string key, value;

		if (field->getName() == "Property")
		{
			if (!field->findAttribute("key", key)) continue;
			if (!field->findAttribute("value", value)) continue;

			if (key == "PanelName") mPanelNameEdit->setCaption(value);
			else if (key == "IncludeDirectory") mIncludeDirectoryEdit->setCaption(value);
			else if (key == "SourceDirectory") mSourceDirectoryEdit->setCaption(value);
		}
	}
}

void CodeGenerator::saveProperties(MyGUI::xml::ElementPtr _root)
{
	_root = _root->createChild("CodeGenaratorSettings");
	MyGUI::xml::ElementPtr nodeProp;

	nodeProp = _root->createChild("Property");
	nodeProp->addAttribute("key", "PanelName");
	nodeProp->addAttribute("value", mPanelNameEdit->getCaption());

	nodeProp = _root->createChild("Property");
	nodeProp->addAttribute("key", "IncludeDirectory");
	nodeProp->addAttribute("value", mIncludeDirectoryEdit->getCaption());

	nodeProp = _root->createChild("Property");
	nodeProp->addAttribute("key", "SourceDirectory");
	nodeProp->addAttribute("value", mSourceDirectoryEdit->getCaption());
}
