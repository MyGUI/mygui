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

			MyGUI::ResourceManager::getInstance().loadFromXmlNode(root, "", MyGUI::Version());

			updateResultPropery(fontName, font);
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
		addProperty(node, "SubstituteCode", _data->getPropertyValue<int>("SubstituteCode"));


		MyGUI::xml::ElementPtr node_codes = node->createChild("Codes");

		std::string ranges = _data->getPropertyValue("FontCodeRanges");
		std::vector<std::string> values = MyGUI::utility::split(ranges, "|");
		for (size_t index = 0; index < values.size(); index ++)
		{
			MyGUI::IntSize size = MyGUI::IntSize::parse(values[index]);
			node_codes->createChild("Code")->addAttribute("range", size.print());
		}

		if (!node_codes->getElementEnumerator().next())
			node->removeChild(node_codes);
	}

	void FontTextureController::updateResultPropery(const std::string& _fontName, DataPtr _data)
	{
		MyGUI::IResource* resource = MyGUI::ResourceManager::getInstance().findByName(_fontName);
		MyGUI::ResourceTrueTypeFont* font = resource != nullptr ? resource->castType<MyGUI::ResourceTrueTypeFont>(false) : nullptr;
		MyGUI::ITexture* texture = font != nullptr ? font->getTextureFont() : nullptr;

		if (texture != nullptr)
			_data->setPropertyValue("TextureSizeResult", MyGUI::utility::toString(texture->getWidth(), " ", texture->getHeight()));
		else
			_data->setPropertyValue("TextureSizeResult", "0 0");

		if (font != nullptr)
			_data->setPropertyValue("FontHeightPix", font->getDefaultHeight());
		else
			_data->setPropertyValue("FontHeightPix", "0");
	}

}
