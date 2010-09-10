/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "BackgroundControl.h"
#include "SkinManager.h"
#include "Localise.h"

namespace tools
{

	BackgroundControl::BackgroundControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("BackgroundControl.layout", _parent),
		mBackgroundColour(nullptr),
		mBackground(nullptr),
		mBackgroundButton(nullptr),
		mCanvas(nullptr),
		mColourPanel(nullptr)
	{
		assignWidget(mBackgroundColour, "BackgroundColour");
		assignWidget(mBackground, "Background");
		assignWidget(mBackgroundButton, "BackgroundColourButton");
		assignWidget(mCanvas, "Canvas");

		fillColours(mBackgroundColour);
		mBackgroundColour->eventComboChangePosition += MyGUI::newDelegate(this, &BackgroundControl::notifyComboChangePosition);

		mBackgroundButton->eventMouseButtonClick += MyGUI::newDelegate(this, &BackgroundControl::notifyMouseButtonClick);

		mColourPanel = new ColourPanel();
		mColourPanel->eventEndDialog = MyGUI::newDelegate(this, &BackgroundControl::notifyEndDialog);
		mColourPanel->eventPreviewColour = MyGUI::newDelegate(this, &BackgroundControl::notifyPreviewColour);
	}

	BackgroundControl::~BackgroundControl()
	{
		delete mColourPanel;
		mColourPanel = nullptr;

		mBackgroundButton->eventMouseButtonClick -= MyGUI::newDelegate(this, &BackgroundControl::notifyMouseButtonClick);

		mBackgroundColour->eventComboChangePosition -= MyGUI::newDelegate(this, &BackgroundControl::notifyComboChangePosition);
	}

	void BackgroundControl::fillColours(MyGUI::ComboBox* _combo)
	{
		_combo->removeAllItems();

		_combo->addItem(replaceTags("ColourZero"), MyGUI::Colour::Zero);
		_combo->addItem(replaceTags("ColourBlack"), MyGUI::Colour::Black);
		_combo->addItem(replaceTags("ColourWhite"), MyGUI::Colour::White);
		_combo->addItem(replaceTags("ColourRed"), MyGUI::Colour::Red);
		_combo->addItem(replaceTags("ColourGreen"), MyGUI::Colour::Green);
		_combo->addItem(replaceTags("ColourBlue"), MyGUI::Colour::Blue);

		_combo->setIndexSelected(0);
		_combo->beginToItemFirst();

		updateColourByPresets();
	}

	void BackgroundControl::notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		updateColourByPresets();
	}

	void BackgroundControl::updateColourByPresets()
	{
		size_t index = mBackgroundColour->getIndexSelected();
		if (index != MyGUI::ITEM_NONE)
		{
			mCurrentColour = *mBackgroundColour->getItemDataAt<MyGUI::Colour>(index);
			updateColours();
		}
	}

	void BackgroundControl::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		mPreviewColour = mCurrentColour;
		mColourPanel->setColour(mCurrentColour);
		mColourPanel->doModal();
	}

	void BackgroundControl::notifyEndDialog(Dialog* _sender, bool _result)
	{
		mColourPanel->endModal();

		if (_result)
		{
			mBackgroundColour->setIndexSelected(MyGUI::ITEM_NONE);
			mCurrentColour = mColourPanel->getColour();
			mCurrentColour.alpha = 1;
			updateColours();
		}
		else
		{
			mBackgroundColour->setIndexSelected(MyGUI::ITEM_NONE);
			mCurrentColour = mPreviewColour;
			mCurrentColour.alpha = 1;
			updateColours();
		}
	}

	void BackgroundControl::updateColours()
	{
		mBackground->setColour(mCurrentColour);
		mBackground->setAlpha(mCurrentColour.alpha);
		mBackgroundButton->setColour(mCurrentColour);
		mBackgroundButton->setAlpha(mCurrentColour.alpha);
	}

	void BackgroundControl::notifyPreviewColour(const MyGUI::Colour& _value)
	{
		mBackgroundColour->setIndexSelected(MyGUI::ITEM_NONE);
		mCurrentColour = _value;
		mCurrentColour.alpha = 1;
		updateColours();
	}

} // namespace tools
