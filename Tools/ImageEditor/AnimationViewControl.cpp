/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "AnimationViewControl.h"
#include "FactoryManager.h"
#include "DataSelectorManager.h"
#include "DataManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(AnimationViewControl)

	AnimationViewControl::AnimationViewControl() :
		mImage(nullptr),
		mParentData(nullptr),
		mMaxCountFrame(64),
		mCurrentFrame(0),
		mTime(0)
	{
	}

	AnimationViewControl::~AnimationViewControl()
	{
	}

	void AnimationViewControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		assignWidget(mImage, "Image");

		std::string parentType = "Index";
		DataSelectorManager::getInstance().getEvent(parentType)->connect(this, &AnimationViewControl::notifyChangeDataSelector);
		mParentData = DataManager::getInstance().getSelectedDataByType(parentType);
		notifyChangeDataSelector(mParentData, false);

		mImage->getParent()->eventChangeCoord += MyGUI::newDelegate(this, &AnimationViewControl::notifyChangeCoord);
		updateImageCoord();

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &AnimationViewControl::notifyFrameStart);
	}

	void AnimationViewControl::notifyChangeDataSelector(Data* _parent, bool _changeSelectOnly)
	{
		mParentData = _parent;

		if (!_changeSelectOnly)
		{
			connectToProperties();
			rebuildAnimations();
		}
	}

	void AnimationViewControl::connectToProperties()
	{
		if (mParentData != nullptr)
		{
			// Index
			Property* property = mParentData->getProperties().find("Rate")->second;
			if (!property->eventChangeProperty.compare(this, &AnimationViewControl::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &AnimationViewControl::notifyChangeProperty);

			// Group
			property = mParentData->getParent()->getProperties().find("Size")->second;
			if (!property->eventChangeProperty.compare(this, &AnimationViewControl::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &AnimationViewControl::notifyChangeProperty);

			// Group
			property = mParentData->getParent()->getProperties().find("Texture")->second;
			if (!property->eventChangeProperty.compare(this, &AnimationViewControl::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &AnimationViewControl::notifyChangeProperty);
		}
	}

	void AnimationViewControl::rebuildAnimations()
	{
		mAnimation.setTextureName("");
		mAnimation.clearFrames();
		mCurrentFrame = 0;
		mTime = 0;

		if (mParentData != nullptr)
		{
			mAnimation.setTextureName(mParentData->getParent()->getPropertyValue("Texture"));
			mAnimation.setSize(MyGUI::IntSize::parse(mParentData->getParent()->getPropertyValue("Size")));
			mAnimation.setRate(MyGUI::utility::parseValue<float>(mParentData->getPropertyValue("Rate")));

			for (Data::VectorData::const_iterator child = mParentData->getChilds().begin(); child != mParentData->getChilds().end(); child ++)
			{
				size_t count = MyGUI::utility::parseValue<size_t>((*child)->getPropertyValue("Count"));
				count = (std::min)(count, mMaxCountFrame);
				count = (std::max)(count, 1u);

				MyGUI::IntPoint point = MyGUI::IntPoint::parse((*child)->getPropertyValue("Point"));

				for (size_t index = 0; index < count; index ++)
					mAnimation.addFrame(point);
			}
		}

		mImage->setImageTexture(mAnimation.getTextureName());
		mImage->setImageCoord(MyGUI::IntCoord());

		updateImageCoord();
	}

	void AnimationViewControl::notifyChangeProperty(Property* _sender)
	{
		rebuildAnimations();
	}

	void AnimationViewControl::updateImageCoord()
	{
		MyGUI::IntSize size = mAnimation.getSize();
		MyGUI::IntSize parentSize = mImage->getParentSize();

		mImage->setCoord((parentSize.width - size.width) / 2, (parentSize.height - size.height) / 2, size.width, size.height);
	}

	void AnimationViewControl::notifyChangeCoord(MyGUI::Widget* _sender)
	{
		updateImageCoord();
	}

	void AnimationViewControl::notifyFrameStart(float _frame)
	{
		if (mAnimation.getFrames().size() == 0)
			return;

		mTime += _frame;

		if (mTime > mAnimation.getRate())
		{
			mTime -= mAnimation.getRate();
			mCurrentFrame ++;

			mCurrentFrame %= mAnimation.getFrames().size();
			MyGUI::IntPoint point = mAnimation.getFrames()[mCurrentFrame];

			mImage->setImageCoord(MyGUI::IntCoord(point.left, point.top, mAnimation.getSize().width, mAnimation.getSize().height));
		}
	}

}
