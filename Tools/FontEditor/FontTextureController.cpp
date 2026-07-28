/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "FontTextureController.h"
#include "FactoryManager.h"
#include "DataSelectorManager.h"
#include "PropertyUtility.h"
#include "ScopeManager.h"
#include "DataUtility.h"
#include "CommandManager.h"
#include <MyGUI_ResourceTrueTypeFont.h>
#include "FontExportSerializer.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(FontTextureController)

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

		CommandManager::getInstance()
			.getEvent("Command_GenerateFont")
			->connect(this, &FontTextureController::commandGenerateFont);
	}

	void FontTextureController::deactivate()
	{
		if (mMsdfModeProperty != nullptr)
		{
			mMsdfModeProperty->eventChangeProperty.disconnect(this);
			mMsdfModeProperty = nullptr;
		}

		ScopeManager::getInstance().eventChangeScope.disconnect(this);
		CommandManager::getInstance().getEvent("Command_GenerateFont")->disconnect(this);
	}

	void FontTextureController::notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection)
	{
		if (mMsdfModeProperty != nullptr)
		{
			mMsdfModeProperty->eventChangeProperty.disconnect(this);
			mMsdfModeProperty = nullptr;
		}

		mParentData = _data;
		if (mParentData != nullptr && mParentData->getType()->getName() != mParentTypeName)
			mParentData = nullptr;

		DataPtr font = mParentData != nullptr ? mParentData->getChildSelected() : nullptr;
		if (font != nullptr && font->getType()->getName() == "Font")
		{
			mMsdfModeProperty = font->getProperty("MsdfMode");
			if (mMsdfModeProperty != nullptr)
				mMsdfModeProperty->eventChangeProperty.connect(this, &FontTextureController::notifyChangeProperty);
		}

		std::string_view texture;
		PropertyPtr property = PropertyUtility::getPropertyByName("Font", "FontName");
		if (property != nullptr)
		{
			texture = property->getValue();
		}

		updateTexture(texture);
		updateReadOnlyForMsdfMode();
	}

	void FontTextureController::notifyChangeProperty(PropertyPtr _sender)
	{
		if (!mActivated || _sender == nullptr)
			return;

		if (_sender->getOwner() != nullptr && _sender->getOwner()->getType()->getName() == "Font" &&
			_sender->getType()->getName() == "MsdfMode")
		{
			updateReadOnlyForMsdfMode();
		}
	}

	void FontTextureController::updateReadOnlyForMsdfMode()
	{
		DataPtr font = mParentData != nullptr ? mParentData->getChildSelected() : nullptr;
		if (font == nullptr || font->getType()->getName() != "Font")
			return;

		bool msdfMode = font->getPropertyValue<bool>("MsdfMode");

		auto setReadOnly = [&](const char* _name, bool _readOnly)
		{
			font->getProperty(_name)->setReadOnly(_readOnly);
		};

		setReadOnly("Antialias", msdfMode);
		setReadOnly("Hinting", msdfMode);
		setReadOnly("DpiScale", msdfMode);
		setReadOnly("AutoDpi", msdfMode);
		setReadOnly("MsdfRange", !msdfMode);
	}

	void FontTextureController::notifyChangeScope(std::string_view _scope)
	{
		if (mControl == nullptr)
			return;

		if (_scope == mScopeName)
		{
			if (!mActivated)
			{
				mControl->clearAll();

				DataSelectorManager::getInstance()
					.getEvent(mParentTypeName)
					->connect(this, &FontTextureController::notifyChangeDataSelector);
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
				if (mMsdfModeProperty != nullptr)
				{
					mMsdfModeProperty->eventChangeProperty.disconnect(this);
					mMsdfModeProperty = nullptr;
				}

				DataSelectorManager::getInstance().getEvent(mParentTypeName)->disconnect(this);
				mParentData = nullptr;

				// we are still owner of the control, reset it
				std::string_view value = mControl->getRoot()->getUserString("CurrentScopeController");
				if (value == mScopeName)
				{
					mControl->getRoot()->setUserString("CurrentScopeController", std::string_view{});
					notifyChangeDataSelector(mParentData, false);

					mControl->clearAll();
				}

				mActivated = false;
			}
		}
	}

	void FontTextureController::updateTexture(std::string_view _value)
	{
		MyGUI::IResource* resource = MyGUI::ResourceManager::getInstance().findByName(_value);
		MyGUI::ResourceTrueTypeFont* font =
			resource != nullptr ? resource->castType<MyGUI::ResourceTrueTypeFont>(false) : nullptr;

		MyGUI::ITexture* texture = font != nullptr ? font->getTextureFont() : nullptr;
		MyGUI::UString value;
		if (texture != nullptr)
			value = texture->getName();

		mControl->setTextureValue(value);
		mControl->resetTextureRegion();
	}

	void FontTextureController::commandGenerateFont(const MyGUI::UString& _commandName, bool& _result)
	{
		if (mParentData != nullptr)
		{
			DataPtr font = (mParentData != nullptr) ? mParentData->getChildSelected() : nullptr;
			if (font != nullptr && !font->getPropertyValue("Source").empty())
			{
				FontExportSerializer::generateFont(font, font->getPropertyValue("FontName"));
				notifyChangeDataSelector(mParentData, false);
				updateResultPropery(font);

				CommandManager::getInstance().executeCommand("Command_OnGenerateFont");
			}
		}
	}

	void FontTextureController::updateResultPropery(DataPtr _data)
	{
		MyGUI::IResource* resource =
			MyGUI::ResourceManager::getInstance().findByName(_data->getPropertyValue("FontName"));
		MyGUI::ResourceTrueTypeFont* font =
			resource != nullptr ? resource->castType<MyGUI::ResourceTrueTypeFont>(false) : nullptr;
		MyGUI::ITexture* texture = font != nullptr ? font->getTextureFont() : nullptr;

		if (texture != nullptr)
			_data->setPropertyValue(
				"TextureSizeResult",
				MyGUI::utility::toString(texture->getWidth(), " ", texture->getHeight()));
		else
			_data->setPropertyValue("TextureSizeResult", "0 0");

		if (font != nullptr)
			_data->setPropertyValue("FontHeightPix", font->getDefaultHeight());
		else
			_data->setPropertyValue("FontHeightPix", "0");
	}

}
