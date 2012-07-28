/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "FontTextureController.h"
#include "FactoryManager.h"
#include "DataSelectorManager.h"
#include "DataManager.h"
#include "PropertyUtility.h"
#include "ScopeManager.h"
#include "DataUtility.h"
#include "CommandManager.h"
#include <MyGUI_ResourceTrueTypeFont.h>

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(FontTextureController)

	FontTextureController::FontTextureController() :
		mControl(nullptr),
		mParentData(nullptr),
		mActivated(false)
	{
	}

	FontTextureController::~FontTextureController()
	{
	}

	void FontTextureController::setTarget(Control* _control)
	{
		mControl = _control->findControl<ScopeTextureControl>();
	}

	void FontTextureController::activate()
	{
		mParentTypeName = "Root";
		mScopeName = "Font";

		ScopeManager::getInstance().eventChangeScope.connect(this, &FontTextureController::notifyChangeScope);
		notifyChangeScope(ScopeManager::getInstance().getCurrentScope());

		CommandManager::getInstance().getEvent("Command_GenerateFont")->connect(this, &FontTextureController::commandGenerateFont);
	}

	void FontTextureController::deactivate()
	{
		ScopeManager::getInstance().eventChangeScope.disconnect(this);
		CommandManager::getInstance().getEvent("Command_GenerateFont")->disconnect(this);
	}

	void FontTextureController::notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection)
	{
		mParentData = _data;
		if (mParentData != nullptr && mParentData->getType()->getName() != mParentTypeName)
			mParentData = nullptr;

		std::string texture;
		PropertyPtr property = PropertyUtility::getPropertyByName("Font", "FontName");
		if (property != nullptr)
		{
			texture = property->getValue();
		}

		updateTexture(texture);
	}

	void FontTextureController::notifyChangeProperty(PropertyPtr _sender)
	{
		if (!mActivated || !PropertyUtility::isDataSelected(_sender->getOwner()))
			return;

		if (_sender->getOwner()->getType()->getName() == "Font")
		{
			if (_sender->getType()->getName() == "FontName")
				updateTexture(_sender->getValue());
		}
	}

	void FontTextureController::notifyChangeScope(const std::string& _scope)
	{
		if (mControl == nullptr)
			return;

		if (_scope == mScopeName)
		{
			if (!mActivated)
			{
				mControl->clearAll();

				DataSelectorManager::getInstance().getEvent(mParentTypeName)->connect(this, &FontTextureController::notifyChangeDataSelector);
				mParentData = DataUtility::getSelectedDataByType(mParentTypeName);
				notifyChangeDataSelector(mParentData, false);

				mControl->getRoot()->setUserString("CurrentScopeController", mScopeName);

				mActivated = true;
			}
		}
		else
		{
			if (mActivated)
			{
				DataSelectorManager::getInstance().getEvent(mParentTypeName)->disconnect(this);
				mParentData = nullptr;

				// мы еще владельцы контрола сбрасываем его
				std::string value = mControl->getRoot()->getUserString("CurrentScopeController");
				if (value == mScopeName)
				{
					mControl->getRoot()->setUserString("CurrentScopeController", "");
					notifyChangeDataSelector(mParentData, false);

					mControl->clearAll();
				}

				mActivated = false;
			}
		}
	}

	void FontTextureController::updateTexture(const std::string& _value)
	{
		MyGUI::IResource* resource = MyGUI::ResourceManager::getInstance().findByName(_value);
		MyGUI::ResourceTrueTypeFont* font = resource != nullptr ? resource->castType<MyGUI::ResourceTrueTypeFont>(false) : nullptr;
		
		MyGUI::ITexture* texture = font != nullptr ? font->getTextureFont() : nullptr;
		std::string value = texture != nullptr ? texture->getName() : "";

		mControl->setTextureValue(value);
		mControl->resetTextureRegion();
	}

	void FontTextureController::commandGenerateFont(const MyGUI::UString& _commandName, bool& _result)
	{
		if (mParentData != nullptr)
		{
			generateFont();
			notifyChangeDataSelector(mParentData, false);
		}
	}

	void FontTextureController::generateFont()
	{
		DataPtr font = (mParentData != nullptr) ? mParentData->getChildSelected() : nullptr;
		if (font != nullptr)
		{
			std::string fontName = font->getPropertyValue("FontName");

			removeFont(fontName);

			MyGUI::xml::Document document;
			document.createDeclaration();
			MyGUI::xml::ElementPtr root = document.createRoot("MyGUI");
			generateFontTTFXml(root, fontName, font);

			try
			{
				MyGUI::ResourceManager::getInstance().loadFromXmlNode(root, "", MyGUI::Version());

				/*MyGUI::IResource* resource = MyGUI::ResourceManager::getInstance().getByName(fontName, false);
				MYGUI_ASSERT(resource != nullptr, "Could not find font '" << mFontName << "'");
				MyGUI::IFont* font = resource->castType<MyGUI::IFont>();

				// вывод реального размера шрифта
				mFontHeight = font->getDefaultHeight();

				mFontView->setFontName(mFontName);
				mTextureView->setFontName(mFontName);

				MyGUI::UString fileName = mComboFont->getOnlyText();

				MyGUI::UString::size_type dotIndex = fileName.find_last_of(".");

				if (dotIndex != MyGUI::UString::npos)
					fileName.erase(fileName.begin() + dotIndex, fileName.end());

				enableSave(true, MyGUI::utility::toString(fileName, '_', mFontHeight));*/
			}
			catch (MyGUI::Exception & e)
			{
				/*mFontHeight = 0;

				mFontView->setFontName(MyGUI::FontManager::getInstance().getDefaultFont());
				mTextureView->setFontName(std::string());

				enableSave(false);

				MyGUI::Message::createMessageBox("Error", e.getDescription(), MyGUI::MessageBoxStyle::Ok | MyGUI::MessageBoxStyle::IconInfo);*/
			}
		}
	}

	template<typename Type>
	void addProperty(MyGUI::xml::ElementPtr _node, const std::string& _name, Type _value)
	{
		MyGUI::xml::ElementPtr node = _node->createChild("Property");
		node->addAttribute("key", _name);
		node->addAttribute("value", _value);
	}

	void FontTextureController::removeFont(const std::string& _fontName)
	{
		MyGUI::ResourceManager& manager = MyGUI::ResourceManager::getInstance();
			if (manager.isExist(_fontName))
				manager.removeByName(_fontName);
	}

	void FontTextureController::generateFontTTFXml(MyGUI::xml::ElementPtr _root, const std::string& _fontName, DataPtr _data)
	{
		_root->addAttribute("type", "Resource");
		_root->addAttribute("version", "1.1");

		MyGUI::xml::ElementPtr node = _root->createChild("Resource");
		node->addAttribute("type", "ResourceTrueTypeFont");
		node->addAttribute("name", _fontName);

		addProperty(node, "Source", _data->getPropertyValue("Source"));
		addProperty(node, "Size", _data->getPropertyValue<float>("Size"));
		addProperty(node, "Resolution", _data->getPropertyValue<int>("Resolution"));
		addProperty(node, "Hinting", _data->getPropertyValue("Hinting"));
		addProperty(node, "Antialias", _data->getPropertyValue<bool>("Antialias"));
		addProperty(node, "SpaceWidth", _data->getPropertyValue<int>("SpaceWidth"));
		addProperty(node, "TabWidth", _data->getPropertyValue<int>("TabWidth"));
		addProperty(node, "OffsetHeight", _data->getPropertyValue<int>("OffsetHeight"));


		MyGUI::xml::ElementPtr node_codes = node->createChild("Codes");

		/*addCodeRange(mEditRange1A, mEditRange1B, node_codes, "range");
		addCodeRange(mEditRange2A, mEditRange2B, node_codes, "range");
		addCodeRange(mEditHideA, mEditHideB, node_codes, "hide");*/

		if (!node_codes->getElementEnumerator().next())
			node->removeChild(node_codes);

		/*if (mComboSubstituteCode->getIndexSelected() != 0)
			addProperty(node, "SubstituteCode", MyGUI::utility::parseValue<int>(mComboSubstituteCode->getOnlyText()));*/
	}

}
