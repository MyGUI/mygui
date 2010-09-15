/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "TextureToolControl.h"
#include "Localise.h"
#include "SettingsManager.h"

namespace tools
{
	TextureToolControl::TextureToolControl(MyGUI::Widget* _parent) :
		TextureControl("TextureControl.layout", _parent),
		mScale(nullptr)
	{
		assignWidget(mScale, "Scale");

		fillScale();
		mScale->eventComboChangePosition += MyGUI::newDelegate(this, &TextureToolControl::notifyComboChangePositionScale);

		MyGUI::Colour colour = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<MyGUI::Colour>("ColourBackground");
		setColour(colour);

		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &TextureToolControl::notifySettingsChanged);
	}

	TextureToolControl::~TextureToolControl()
	{
		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &TextureToolControl::notifySettingsChanged);
		mScale->eventComboChangePosition -= MyGUI::newDelegate(this, &TextureToolControl::notifyComboChangePositionScale);
	}

	void TextureToolControl::fillScale()
	{
		mScale->removeAllItems();

		mScale->addItem(replaceTags("Scale50"), (double)0.5);
		mScale->addItem(replaceTags("Scale100"), (double)1);
		mScale->addItem(replaceTags("Scale200"), (double)2);
		mScale->addItem(replaceTags("Scale400"), (double)4);
		mScale->addItem(replaceTags("Scale800"), (double)8);
		mScale->addItem(replaceTags("Scale1600"), (double)16);

		mScale->beginToItemFirst();

		size_t index = 1;

		mScale->setIndexSelected(index);
		double scale = *mScale->getItemDataAt<double>(index);

		setScale(scale);
	}

	void TextureToolControl::updateScale(MyGUI::ComboBox* _sender)
	{
		if (_sender == mScale)
		{
			size_t index = mScale->getIndexSelected();
			if (index != MyGUI::ITEM_NONE)
			{
				double scale = *mScale->getItemDataAt<double>(index);
				setScale(scale);
			}
		}
	}

	void TextureToolControl::notifyComboChangePositionScale(MyGUI::ComboBox* _sender, size_t _index)
	{
		updateScale(_sender);
	}

	void TextureToolControl::notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
	{
		if (_sectorName == "Settings")
		{
			if (_propertyName == "ColourBackground")
			{
				MyGUI::Colour colour = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<MyGUI::Colour>("ColourBackground");
				setColour(colour);
			}
		}
	}

} // namespace tools
