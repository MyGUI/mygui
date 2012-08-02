/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "BackgroundControl.h"
#include "SettingsManager.h"

namespace tools
{

	BackgroundControl::BackgroundControl() :
		mBackground(nullptr),
		mCanvas(nullptr)
	{
	}

	BackgroundControl::~BackgroundControl()
	{
		SettingsManager::getInstance().eventSettingsChanged.disconnect(this);
	}

	void BackgroundControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, "BackgroundControl.layout");

		assignWidget(mBackground, "Background");
		assignWidget(mCanvas, "Canvas");

		mColourValueName = "ColourBackground";
		MyGUI::Colour colour = SettingsManager::getInstance().getValue<MyGUI::Colour>("Workspace/Colours/" + mColourValueName);
		setColour(colour);

		SettingsManager::getInstance().eventSettingsChanged.connect(this, &BackgroundControl::notifySettingsChanged);
	}

	void BackgroundControl::setColour(const MyGUI::Colour& _colour)
	{
		mBackground->setColour(_colour);
		mBackground->setAlpha(_colour.alpha);
	}

	MyGUI::Widget* BackgroundControl::getCanvas()
	{
		return mCanvas;
	}

	void BackgroundControl::notifySettingsChanged(const std::string& _path)
	{
		MyGUI::Colour colour = SettingsManager::getInstance().getValue<MyGUI::Colour>("Workspace/Colours/" + mColourValueName);
		setColour(colour);
	}

}
