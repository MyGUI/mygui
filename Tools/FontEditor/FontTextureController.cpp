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
#include "FontExportSerializer.h"

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
			DataPtr font = (mParentData != nullptr) ? mParentData->getChildSelected() : nullptr;
			if (font != nullptr)
			{
				FontExportSerializer::generateFont(font);
				notifyChangeDataSelector(mParentData, false);
				updateResultPropery(font);

				CommandManager::getInstance().executeCommand("Command_OnGenerateFont");
			}
		}
	}

	void FontTextureController::updateResultPropery(DataPtr _data)
	{
		MyGUI::IResource* resource = MyGUI::ResourceManager::getInstance().findByName(_data->getPropertyValue("FontName"));
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
