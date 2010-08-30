/*!
	@file
	@author		George Evmenov
	@date		01/2010
*/
#ifndef __CODE_GENERATOR_H__
#define __CODE_GENERATOR_H__

#include "BaseLayout/BaseLayout.h"
#include "WidgetContainer.h"
#include "Dialog.h"
#include "OpenSaveFileDialog.h"

namespace tools
{
	ATTRIBUTE_CLASS_LAYOUT(CodeGenerator, "CodeGeneratorWindow.layout");
	class CodeGenerator :
		public Dialog
	{
	public:

		CodeGenerator();
		~CodeGenerator();

		void loadTemplate();
		void saveTemplate();

	private:
		void parseTemplate(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);
		std::string stringToUpperCase(const std::string& _str);
		void printWidgetDeclaration(WidgetContainer* _container, std::ofstream& _stream);
		void notifyGeneratePressed(MyGUI::Widget* _sender);
		void notifyCancel(MyGUI::Widget* _sender);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);
		void notifyBrowseHeader(MyGUI::Widget* _sender);
		void notifyBrowseSource(MyGUI::Widget* _sender);

		void notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result);

		virtual void onDoModal();
		virtual void onEndModal();

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(CodeGenerator, mPanelNameEdit, "PanelName");
		MyGUI::Edit* mPanelNameEdit;
		ATTRIBUTE_FIELD_WIDGET_NAME(CodeGenerator, mPanelNamespaceEdit, "PanelNamespace");
		MyGUI::Edit* mPanelNamespaceEdit;
		ATTRIBUTE_FIELD_WIDGET_NAME(CodeGenerator, mSourceDirectoryEdit, "SourceDirectory");
		MyGUI::Edit* mSourceDirectoryEdit;
		ATTRIBUTE_FIELD_WIDGET_NAME(CodeGenerator, mIncludeDirectoryEdit, "IncludeDirectory");
		MyGUI::Edit* mIncludeDirectoryEdit;
		ATTRIBUTE_FIELD_WIDGET_NAME(CodeGenerator, mGenerateButton, "Generate");
		MyGUI::Button* mGenerateButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(CodeGenerator, mCancel, "Cancel");
		MyGUI::Button* mCancel;
		ATTRIBUTE_FIELD_WIDGET_NAME(CodeGenerator, mBrowseHeader, "BrowseHeader");
		MyGUI::Button* mBrowseHeader;
		ATTRIBUTE_FIELD_WIDGET_NAME(CodeGenerator, mBrowseSource, "BrowseSource");
		MyGUI::Button* mBrowseSource;

		MyGUI::MapString mTemplateFiles;
		MyGUI::MapString mTemplateStrings;

		OpenSaveFileDialog* mOpenSaveFileDialog;
	};

} // namespace tools

#endif // __CODE_GENERATOR_H__
