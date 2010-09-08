/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "TextureToolControl.h"
#include "Localise.h"

namespace tools
{
	TextureToolControl::TextureToolControl(MyGUI::Widget* _parent) :
		TextureControl("TextureControl.layout", _parent),
		mBackgroundColour(nullptr),
		mScale(nullptr),
		mBackgroundButton(nullptr),
		mColourPanel(nullptr)
	{
		assignWidget(mBackgroundColour, "BackgroundColour");
		assignWidget(mScale, "Scale");
		assignWidget(mBackgroundButton, "BackgroundColourButton");

		fillColours(mBackgroundColour);
		mBackgroundColour->eventComboChangePosition += MyGUI::newDelegate(this, &TextureToolControl::notifyComboChangePosition);

		fillScale();
		mScale->eventComboChangePosition += MyGUI::newDelegate(this, &TextureToolControl::notifyComboChangePosition);

		mBackgroundButton->eventMouseButtonClick += MyGUI::newDelegate(this, &TextureToolControl::notifyMouseButtonClick);

		mColourPanel = new ColourPanel();
		mColourPanel->eventEndDialog = MyGUI::newDelegate(this, &TextureToolControl::notifyEndDialog);
	}

	TextureToolControl::~TextureToolControl()
	{
		mBackgroundButton->eventMouseButtonClick -= MyGUI::newDelegate(this, &TextureToolControl::notifyMouseButtonClick);

		mBackgroundColour->eventComboChangePosition -= MyGUI::newDelegate(this, &TextureToolControl::notifyComboChangePosition);
		mScale->eventComboChangePosition -= MyGUI::newDelegate(this, &TextureToolControl::notifyComboChangePosition);

		delete mColourPanel;
		mColourPanel = nullptr;
	}

	void TextureToolControl::notifyEndDialog(Dialog* _sender, bool _result)
	{
		if (_result)
		{
			mBackgroundColour->setIndexSelected(MyGUI::ITEM_NONE);
			MyGUI::Colour colour = mColourPanel->getColour();
			colour.alpha = 1;

			setColour(colour);
			mBackgroundButton->setColour(colour);
			mBackgroundButton->setAlpha(colour.alpha);
		}

		mColourPanel->endModal();
	}

	void TextureToolControl::fillColours(MyGUI::ComboBox* _combo)
	{
		_combo->removeAllItems();

		_combo->addItem(replaceTags("ColourZero"), MyGUI::Colour::Zero);
		_combo->addItem(replaceTags("ColourBlack"), MyGUI::Colour::Black);
		_combo->addItem(replaceTags("ColourWhite"), MyGUI::Colour::White);
		_combo->addItem(replaceTags("ColourRed"), MyGUI::Colour::Red);
		_combo->addItem(replaceTags("ColourGreen"), MyGUI::Colour::Green);
		_combo->addItem(replaceTags("ColourBlue"), MyGUI::Colour::Blue);

		_combo->setIndexSelected(0);

		updateColour(_combo);
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

		size_t index = 1;

		mScale->setIndexSelected(index);
		double scale = *mScale->getItemDataAt<double>(index);

		setScale(scale);
	}

	void TextureToolControl::updateColour(MyGUI::ComboBox* _sender)
	{
		if (_sender == mBackgroundColour)
		{
			size_t index = mBackgroundColour->getIndexSelected();
			if (index != MyGUI::ITEM_NONE)
			{
				MyGUI::Colour colour = *mBackgroundColour->getItemDataAt<MyGUI::Colour>(index);
				setColour(colour);

				mBackgroundButton->setColour(colour);
				mBackgroundButton->setAlpha(colour.alpha);
			}
		}
		else if (_sender == mScale)
		{
			size_t index = mScale->getIndexSelected();
			if (index != MyGUI::ITEM_NONE)
			{
				double scale = *mScale->getItemDataAt<double>(index);
				setScale(scale);
			}
		}
	}

	void TextureToolControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		updateColour(_sender);
	}

	void TextureToolControl::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		mColourPanel->setColour(getColour());
		mColourPanel->doModal();
	}

} // namespace tools
