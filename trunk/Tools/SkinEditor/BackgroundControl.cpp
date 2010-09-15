/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "BackgroundControl.h"
#include "SkinManager.h"
#include "Localise.h"
#include "SettingsManager.h"

namespace tools
{

	BackgroundControl::BackgroundControl(MyGUI::Widget* _parent) :
		wraps::BaseLayout("BackgroundControl.layout", _parent),
		mBackground(nullptr),
		mCanvas(nullptr)
	{
		assignWidget(mBackground, "Background");
		assignWidget(mCanvas, "Canvas");

		MyGUI::Colour colour = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<MyGUI::Colour>("ColourBackground");
		setColour(colour);

		SettingsManager::getInstance().eventSettingsChanged += MyGUI::newDelegate(this, &BackgroundControl::notifySettingsChanged);
	}

	BackgroundControl::~BackgroundControl()
	{
		SettingsManager::getInstance().eventSettingsChanged -= MyGUI::newDelegate(this, &BackgroundControl::notifySettingsChanged);
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

	void BackgroundControl::notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName)
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
