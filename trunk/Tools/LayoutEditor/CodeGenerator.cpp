/*!
	@file
	@author		George Evmenov
	@date		01/2010
*/

#include "Precompiled.h"
#include "CodeGenerator.h"
#include "EditorWidgets.h"
#include "UndoManager.h"
#include "Localise.h"
#include "SettingsManager.h"

namespace tools
{

	// FIXME hardcoded template
	const std::string TemplateName = "BaseLayoutCPP.xml";

	CodeGenerator::CodeGenerator() :
		mOpenSaveFileDialog(nullptr)
	{
		initialiseByAttributes(this);

		setDialogRoot(mMainWidget);

		mOpenSaveFileDialog = new OpenSaveFileDialog();
		mOpenSaveFileDialog->Initialise(SettingsManager::getInstance().getValue("EditorState/OpenSaveFileDialogLayout"));
		mOpenSaveFileDialog->setDialogInfo(replaceTags("CaptionOpenFolder"), replaceTags("ButtonOpenFolder"), true);
		mOpenSaveFileDialog->eventEndDialog.connect(this, &CodeGenerator::notifyEndDialogOpenSaveFile);

		mGenerateButton->eventMouseButtonClick += MyGUI::newDelegate(this, &CodeGenerator::notifyGeneratePressed);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &CodeGenerator::notifyCancel);
		mBrowseHeader->eventMouseButtonClick += MyGUI::newDelegate(this, &CodeGenerator::notifyBrowseHeader);
		mBrowseSource->eventMouseButtonClick += MyGUI::newDelegate(this, &CodeGenerator::notifyBrowseSource);

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &CodeGenerator::notifyWindowButtonPressed);

		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("LECodeTemplate") = MyGUI::newDelegate(this, &CodeGenerator::parseTemplate);
		MyGUI::ResourceManager::getInstance().load(TemplateName);
	}

	CodeGenerator::~CodeGenerator()
	{
		delete mOpenSaveFileDialog;
		mOpenSaveFileDialog = nullptr;
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
			mTemplateFiles.insert(MyGUI::PairString(templateFile, outputFile));
		}

		MyGUI::xml::ElementEnumerator string = _node->getElementEnumerator();
		while (string.next("String"))
		{
			std::string key = string->findAttribute("key");
			std::string value = string->findAttribute("value");
			mTemplateStrings.insert(MyGUI::PairString(key, value));
		}
	}

	std::string CodeGenerator::stringToUpperCase(const std::string& _str)
	{
		// replace lower case sharacters with upper case characters and add '_' between words
		// words is either Word or WORD, for example TestXMLPanelName return TEST_XML_PANEL_NAME
		if (_str.empty()) return "";
		std::string ret;
		bool previousIsLowerCase = false;
		for (size_t i = 0; i < _str.length(); i++)
		{
			if ((i != 0) &&
				(
				(previousIsLowerCase && isupper(_str[i])) ||
				(isupper(_str[i]) && (i + 1 < _str.length()) && islower(_str[i+1]))
				)
				)
			{
				ret.push_back('_');
			}
			ret.push_back((char)toupper(_str[i]));
			previousIsLowerCase = (islower(_str[i]) != 0);
		}
		return ret;
	}

	void CodeGenerator::printWidgetDeclaration(WidgetContainer* _container, std::ofstream& _stream)
	{
		if (!_container->getName().empty() && _container->getName() != "_Main")
		{
			MyGUI::LanguageManager& lm = MyGUI::LanguageManager::getInstance();
			lm.addUserTag("Widget_Name", _container->getName());
			lm.addUserTag("Widget_Type", _container->getType());
			for (MyGUI::MapString::iterator iterS = mTemplateStrings.begin(); iterS != mTemplateStrings.end(); ++iterS)
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
		std::string panelName = mPanelNameEdit->getOnlyText();
		std::string panelNamespace = mPanelNamespaceEdit->getOnlyText();
		std::string includeDirectory = mIncludeDirectoryEdit->getOnlyText();
		std::string sourceDirectory = mSourceDirectoryEdit->getOnlyText();

		if (panelName.empty() || mPanelNameEdit->getCaption() == ("#FF0000" + replaceTags("Error")))
		{
			mPanelNameEdit->setCaption("#FF0000" + replaceTags("Error"));
			return;
		}

		if (panelNamespace.empty() || mPanelNamespaceEdit->getCaption() == ("#FF0000" + replaceTags("Error")))
		{
			mPanelNamespaceEdit->setCaption("#FF0000" + replaceTags("Error"));
			return;
		}

		if (includeDirectory.empty()) includeDirectory = ".";
		if (sourceDirectory.empty()) sourceDirectory = ".";

		lm.addUserTag("Panel_Name", panelName);
		lm.addUserTag("Panel_Namespace", panelNamespace);
		lm.addUserTag("Layout_Name", MyGUI::LanguageManager::getInstance().getTag("ResourceName"));
		lm.addUserTag("Include_Directory", includeDirectory);
		lm.addUserTag("Source_Directory", sourceDirectory);
		lm.addUserTag("Uppercase_Panel_Name", stringToUpperCase(panelName));

		for (MyGUI::MapString::iterator iter = mTemplateFiles.begin(); iter != mTemplateFiles.end(); ++iter)
		{
			std::ifstream input_file(MyGUI::DataManager::getInstance().getDataPath(iter->first).c_str());
			std::ofstream output_file(lm.replaceTags(iter->second).asUTF8_c_str());
			while (!input_file.eof() && !input_file.fail() && !output_file.fail())
			{
				char str[256];
				input_file.getline(str, sizeof(str));
				output_file << lm.replaceTags(str) << std::endl;
				if (strstr(str, "//%LE Widget_Declaration list start") != 0)
				{
					EnumeratorWidgetContainer widget = EditorWidgets::getInstance().getWidgets();
					while (widget.next())
						printWidgetDeclaration(widget.current(), output_file);
				}
			}
			output_file.close();
			input_file.close();
		}

		eventEndDialog(this, true);
	}

	void CodeGenerator::onDoModal()
	{
		MyGUI::IntSize windowSize = mMainWidget->getSize();
		MyGUI::IntSize parentSize = mMainWidget->getParentSize();

		mMainWidget->setPosition((parentSize.width - windowSize.width) / 2, (parentSize.height - windowSize.height) / 2);
	}

	void CodeGenerator::onEndModal()
	{
	}

	void CodeGenerator::notifyCancel(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, false);
	}

	void CodeGenerator::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		if (_name == "close")
			eventEndDialog(this, false);
	}

	void CodeGenerator::loadTemplate()
	{
		MyGUI::MapString& codeGeneratorSettings = EditorWidgets::getInstance().getCodeGeneratorSettings();
		mPanelNameEdit->setCaption(codeGeneratorSettings["PanelName"]);
		mPanelNamespaceEdit->setCaption(codeGeneratorSettings["PanelNamespace"]);
		mIncludeDirectoryEdit->setCaption(codeGeneratorSettings["IncludeDirectory"]);
		mSourceDirectoryEdit->setCaption(codeGeneratorSettings["SourceDirectory"]);
	}

	void CodeGenerator::saveTemplate()
	{
		MyGUI::MapString& codeGeneratorSettings = EditorWidgets::getInstance().getCodeGeneratorSettings();
		codeGeneratorSettings["PanelName"] = mPanelNameEdit->getOnlyText();
		codeGeneratorSettings["PanelNamespace"] = mPanelNamespaceEdit->getOnlyText();
		codeGeneratorSettings["IncludeDirectory"] = mIncludeDirectoryEdit->getOnlyText();
		codeGeneratorSettings["SourceDirectory"] = mSourceDirectoryEdit->getOnlyText();

		UndoManager::getInstance().setUnsaved(true);
	}

	void CodeGenerator::notifyBrowseHeader(MyGUI::Widget* _sender)
	{
		mOpenSaveFileDialog->setCurrentFolder(mIncludeDirectoryEdit->getOnlyText());
		mOpenSaveFileDialog->setMode("Header");
		mOpenSaveFileDialog->doModal();
	}

	void CodeGenerator::notifyBrowseSource(MyGUI::Widget* _sender)
	{
		mOpenSaveFileDialog->setCurrentFolder(mSourceDirectoryEdit->getOnlyText());
		mOpenSaveFileDialog->setMode("Source");
		mOpenSaveFileDialog->doModal();
	}

	void CodeGenerator::notifyEndDialogOpenSaveFile(Dialog* _sender, bool _result)
	{
		if (_result)
		{
			if (mOpenSaveFileDialog->getMode() == "Header")
			{
				mIncludeDirectoryEdit->setCaption(mOpenSaveFileDialog->getCurrentFolder());
			}
			else if (mOpenSaveFileDialog->getMode() == "Source")
			{
				mSourceDirectoryEdit->setCaption(mOpenSaveFileDialog->getCurrentFolder());
			}
		}

		mOpenSaveFileDialog->endModal();
	}

}
