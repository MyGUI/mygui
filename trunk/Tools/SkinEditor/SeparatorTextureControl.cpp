/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SeparatorTextureControl.h"

namespace tools
{

	SeparatorTextureControl::SeparatorTextureControl(MyGUI::Widget* _parent) :
		TextureControl(_parent),
		mTextureVisible(false),
		mHorizontalSelectorControl(nullptr),
		mVerticalSelectorControl(nullptr),
		mHorizontal(false)
	{
		mTypeName = MyGUI::utility::toString((int)this);

		addSelectorControl(mHorizontalSelectorControl);
		addSelectorControl(mVerticalSelectorControl);

		mHorizontalSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &SeparatorTextureControl::notifyChangePosition);
		mVerticalSelectorControl->eventChangePosition += MyGUI::newDelegate(this, &SeparatorTextureControl::notifyChangePosition);

		initialiseAdvisor();
	}

	SeparatorTextureControl::~SeparatorTextureControl()
	{
		shutdownAdvisor();

		mHorizontalSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &SeparatorTextureControl::notifyChangePosition);
		mVerticalSelectorControl->eventChangePosition -= MyGUI::newDelegate(this, &SeparatorTextureControl::notifyChangePosition);
	}

	void SeparatorTextureControl::updateSeparatorProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
		{
			if (_sender->getName() == "Position")
				updatePosition();
			else if (_sender->getName() == "Visible")
				updateVisible();
		}
	}

	void SeparatorTextureControl::updateSeparatorProperties()
	{
		updateVisible();
		updatePosition();
	}

	void SeparatorTextureControl::updateTextureControl()
	{
		if (mTextureVisible && !mTextureName.empty() && getCurrentSkin() != nullptr && getCurrentState() != nullptr)
		{
			setTextureName(mTextureName);
			setTextureRegion(mTextureRegion);
		}
		else
		{
			setTextureRegion(MyGUI::IntCoord());
		}
	}

	void SeparatorTextureControl::updateSkinProperties()
	{
		updateTextureName();
		updateRegionSize();
	}

	void SeparatorTextureControl::updateSkinProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Texture")
			updateTextureName();
		else if (_sender->getName() == "Coord")
			updateRegionSize();
	}

	void SeparatorTextureControl::updateStateProperties()
	{
		updateRegionPosition();
		updateTextureVisible();
	}

	void SeparatorTextureControl::updateStateProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_sender->getName() == "Position")
			updateRegionPosition();
		else if (_sender->getName() == "Visible")
			updateTextureVisible();
	}

	void SeparatorTextureControl::updateTextureName()
	{
		mTextureName = "";
		if (getCurrentSkin() != nullptr)
		{
			Property* prop = getCurrentSkin()->getPropertySet()->getChild("Texture");
			if (prop != nullptr)
				mTextureName = prop->getValue();
		}

		updateTextureControl();
	}

	void SeparatorTextureControl::updateRegionSize()
	{
		mTextureRegion.width = 0;
		mTextureRegion.height = 0;

		if (getCurrentSkin() != nullptr)
		{
			Property* prop = getCurrentSkin()->getPropertySet()->getChild("Coord");
			if (prop != nullptr)
			{
				MyGUI::IntCoord coord = MyGUI::IntCoord::parse(prop->getValue());
				mTextureRegion.width = coord.width;
				mTextureRegion.height = coord.height;
			}
		}

		updateTextureControl();
	}

	void SeparatorTextureControl::updateRegionPosition()
	{
		mTextureRegion.left = 0;
		mTextureRegion.top = 0;

		if (getCurrentState() != nullptr)
		{
			Property* prop = getCurrentState()->getPropertySet()->getChild("Position");
			if (prop != nullptr)
			{
				MyGUI::IntPoint position = MyGUI::IntPoint::parse(prop->getValue());
				mTextureRegion.left = position.left;
				mTextureRegion.top = position.top;
			}
		}

		updateTextureControl();
	}

	void SeparatorTextureControl::updateTextureVisible()
	{
		mTextureVisible = false;

		if (getCurrentState() != nullptr)
		{
			Property* prop = getCurrentState()->getPropertySet()->getChild("Visible");
			if (prop != nullptr)
				mTextureVisible = prop->getValue() == "True";
		}

		updateTextureControl();
	}

	void SeparatorTextureControl::updatePosition()
	{
		MyGUI::UString value;

		if (getCurrentSeparator() != nullptr)
		{
			Property* prop = getCurrentSeparator()->getPropertySet()->getChild("Position");
			if (prop != nullptr)
				value = prop->getValue();
		}

		int position = 0;
		if (MyGUI::utility::parseComplex(value, position))
		{
			if (mHorizontal)
				mHorizontalSelectorControl->setCoord(MyGUI::IntCoord(0, position, mTextureRegion.width, 1));
			else
				mVerticalSelectorControl->setCoord(MyGUI::IntCoord(position, 0, 1, mTextureRegion.height));
		}
	}

	void SeparatorTextureControl::updateVisible()
	{
		mHorizontalSelectorControl->setVisible(false);
		mVerticalSelectorControl->setVisible(false);

		if (getCurrentSeparator() != nullptr)
		{
			Property* prop = getCurrentSeparator()->getPropertySet()->getChild("Visible");
			if (prop != nullptr)
			{
				if (prop->getValue() == "True")
				{
					if (getCurrentSeparator()->getHorizontal())
					{
						mHorizontalSelectorControl->setVisible(true);
						mHorizontal = true;
					}
					else
					{
						mVerticalSelectorControl->setVisible(true);
						mHorizontal = false;
					}
				}
			}
		}
	}

	void SeparatorTextureControl::notifyChangePosition()
	{
		int position = 0;
		if (mHorizontal)
			position = mHorizontalSelectorControl->getPosition().top;
		else
			position = mVerticalSelectorControl->getPosition().left;

		if (getCurrentSeparator() != nullptr)
		{
			Property* prop = getCurrentSeparator()->getPropertySet()->getChild("Position");
			if (prop != nullptr)
				prop->setValue(MyGUI::utility::toString(position), mTypeName);
		}
	}

} // namespace tools
