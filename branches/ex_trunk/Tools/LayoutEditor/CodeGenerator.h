/*!
	@file
	@author		George Evmenov
	@date		01/2010
*/
#ifndef __CODE_GENERATOR_H__
#define __CODE_GENERATOR_H__

#include "BaseLayout/BaseLayout.h"
#include "EditorWidgets.h"

ATTRIBUTE_CLASS_LAYOUT(CodeGenerator, "CodeGeneratorWindow.layout");
class CodeGenerator :
	public wraps::BaseLayout
{
public:

	CodeGenerator();
	~CodeGenerator();

	void loadProperties(MyGUI::xml::ElementEnumerator _field);
	void saveProperties(MyGUI::xml::ElementPtr _root);

	MyGUI::Widget* getMainWidget() { return mMainWidget; }
private:
	void parseTemplate(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);
	std::string stringToUpperCase(std::string _str);
	void printWidgetDeclaration(WidgetContainer* _container, std::ofstream& _stream);
	void notifyGeneratePressed(MyGUI::Widget* _sender);
private:
	typedef std::map<std::string, std::string> StringPairs;

	ATTRIBUTE_FIELD_WIDGET_NAME(CodeGenerator, mPanelNameEdit, "PanelName");
	MyGUI::Edit* mPanelNameEdit;
	ATTRIBUTE_FIELD_WIDGET_NAME(CodeGenerator, mSourceDirectoryEdit, "SourceDirectory");
	MyGUI::Edit* mSourceDirectoryEdit;
	ATTRIBUTE_FIELD_WIDGET_NAME(CodeGenerator, mIncludeDirectoryEdit, "IncludeDirectory");
	MyGUI::Edit* mIncludeDirectoryEdit;
	ATTRIBUTE_FIELD_WIDGET_NAME(CodeGenerator, mGenerateButton, "Generate");
	MyGUI::Button* mGenerateButton;

	StringPairs mTemplateFiles;
	StringPairs mTemplateStrings;

	std::string mPanelName;
	std::string mIncludeDirectory;
	std::string mSourceDirectory;
};

#endif // __CODE_GENERATOR_H__
