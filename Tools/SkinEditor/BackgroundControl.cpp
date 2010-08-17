/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "BackgroundControl.h"
#include "SkinManager.h"

namespace tools
{

	BackgroundControl::BackgroundControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("BackgroundControl.layout", _parent),
		mBackgroundColour(nullptr),
		mBackground(nullptr),
		mBackgroundButton(nullptr),
		mColourPanel(nullptr),
		mCanvas(nullptr)
	{
		assignWidget(mBackgroundColour, "BackgroundColour");
		assignWidget(mBackground, "Background");
		assignWidget(mBackgroundButton, "BackgroundColourButton");
		assignWidget(mCanvas, "Canvas");

		fillColours(mBackgroundColour);
		mBackgroundColour->eventComboChangePosition += MyGUI::newDelegate(this, &BackgroundControl::notifyComboChangePosition);

		mBackgroundButton->eventMouseButtonClick += MyGUI::newDelegate(this, &BackgroundControl::notifyMouseButtonClick);

		mColourPanel = new ColourPanel();
		mColourPanel->setVisible(false);
		mColourPanel->eventEndDialog = MyGUI::newDelegate(this, &BackgroundControl::notifyEndDialog);
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

		_combo->addItem("Zero", MyGUI::Colour::Zero);
		_combo->addItem("Black", MyGUI::Colour::Black);
		_combo->addItem("White", MyGUI::Colour::White);
		_combo->addItem("Red", MyGUI::Colour::Red);
		_combo->addItem("Green", MyGUI::Colour::Green);
		_combo->addItem("Blue", MyGUI::Colour::Blue);

		_combo->setIndexSelected(0);

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
		mColourPanel->setColour(mCurrentColour);
		mColourPanel->setVisible(true);
	}

	void BackgroundControl::notifyEndDialog(Dialog* _sender, bool _result)
	{
		if (_result)
		{
			mBackgroundColour->setIndexSelected(MyGUI::ITEM_NONE);
			mCurrentColour = mColourPanel->getColour();
			mCurrentColour.alpha = 1;
			updateColours();
		}

		mColourPanel->setVisible(false);
	}

	void BackgroundControl::updateColours()
	{
		mBackground->setColour(mCurrentColour);
		mBackground->setAlpha(mCurrentColour.alpha);
		mBackgroundButton->setColour(mCurrentColour);
		mBackgroundButton->setAlpha(mCurrentColour.alpha);
	}

} // namespace tools
