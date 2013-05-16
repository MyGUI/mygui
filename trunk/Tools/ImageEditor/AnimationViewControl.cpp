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
#include "DataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(AnimationViewControl)

	AnimationViewControl::AnimationViewControl() :
		mImage(nullptr),
		mFrameInfo(nullptr),
		mButtonPlay(nullptr),
		mButtonLeft(nullptr),
		mButtonRight(nullptr),
		mParentData(nullptr),
		mCurrentFrame(0),
		mTime(0),
		mPlay(true)
	{
	}

	AnimationViewControl::~AnimationViewControl()
	{
	}

	void AnimationViewControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		InitialiseByAttributes(this);

		std::string parentType = "Index";
		DataSelectorManager::getInstance().getEvent(parentType)->connect(this, &AnimationViewControl::notifyChangeDataSelector);
		mParentData = DataUtility::getSelectedDataByType(parentType);
		notifyChangeDataSelector(mParentData, false);

		mImage->getParent()->eventChangeCoord += MyGUI::newDelegate(this, &AnimationViewControl::notifyChangeCoord);
		updateImageCoord();

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &AnimationViewControl::notifyFrameStart);

		mButtonPlay->eventMouseButtonClick += MyGUI::newDelegate(this, &AnimationViewControl::notifyMouseButtonClick);
		mButtonLeft->eventMouseButtonClick += MyGUI::newDelegate(this, &AnimationViewControl::notifyMouseButtonClick);
		mButtonRight->eventMouseButtonClick += MyGUI::newDelegate(this, &AnimationViewControl::notifyMouseButtonClick);
	}

	void AnimationViewControl::notifyChangeDataSelector(DataPtr _parent, bool _changeSelectOnly)
	{
		mParentData = _parent;

		if (!_changeSelectOnly)
		{
			connectToProperties();
			rebuildAnimations();
		}
		else
		{
			updateSelectedFrame();
		}
	}

	void AnimationViewControl::connectToProperties()
	{
		if (mParentData != nullptr)
		{
			// Index
			PropertyPtr property = mParentData->getProperty("Rate");
			if (!property->eventChangeProperty.exist(this, &AnimationViewControl::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &AnimationViewControl::notifyChangeProperty);

			// Group
			property = mParentData->getParent()->getProperty("Size");
			if (!property->eventChangeProperty.exist(this, &AnimationViewControl::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &AnimationViewControl::notifyChangeProperty);

			// Group
			property = mParentData->getParent()->getProperty("Texture");
			if (!property->eventChangeProperty.exist(this, &AnimationViewControl::notifyChangeProperty))
				property->eventChangeProperty.connect(this, &AnimationViewControl::notifyChangeProperty);

			for (Data::VectorData::const_iterator child = mParentData->getChilds().begin(); child != mParentData->getChilds().end(); child ++)
			{
				property = (*child)->getProperty("Point");
				if (!property->eventChangeProperty.exist(this, &AnimationViewControl::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &AnimationViewControl::notifyChangeProperty);

				property = (*child)->getProperty("Count");
				if (!property->eventChangeProperty.exist(this, &AnimationViewControl::notifyChangeProperty))
					property->eventChangeProperty.connect(this, &AnimationViewControl::notifyChangeProperty);
			}
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
			mAnimation.setSize(mParentData->getParent()->getPropertyValue<MyGUI::IntCoord>("Size").size());
			mAnimation.setRate(mParentData->getPropertyValue<float>("Rate"));

			for (Data::VectorData::const_iterator child = mParentData->getChilds().begin(); child != mParentData->getChilds().end(); child ++)
			{
				size_t count = (*child)->getPropertyValue<size_t>("Count");
				MyGUI::IntPoint point = (*child)->getPropertyValue<MyGUI::IntPoint>("Point");
				mAnimation.addFrame(point, count);
			}

			if (!mPlay)
				updateSelectedFrame();
		}

		mImage->setImageTexture(mAnimation.getTextureName());

		if (mAnimation.getFrames().size() == 0)
			mImage->setImageCoord(MyGUI::IntCoord());
		else
			updateFrame();

		mFrameInfo->setCaption(MyGUI::utility::toString(mCurrentFrame, " : ", mAnimation.getFrames().size()));

		updateImageCoord();
	}

	void AnimationViewControl::notifyChangeProperty(PropertyPtr _sender)
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
		if (mAnimation.getFrames().size() == 0 || !mPlay)
			return;

		mTime += _frame;

		float len = mAnimation.getFrames()[mCurrentFrame].second * mAnimation.getRate();

		if (mTime > len)
		{
			mTime -= len;
			mCurrentFrame ++;

			updateFrame();
		}
	}

	void AnimationViewControl::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		if (_sender == mButtonPlay)
		{
			mPlay = !mPlay;
			mButtonPlay->setStateSelected(!mPlay);

			if (!mPlay)
				updateSelectedFrame();
		}
		else if (_sender == mButtonLeft)
		{
			if (mAnimation.getFrames().size() != 0 && !mPlay)
			{
				mCurrentFrame += mAnimation.getFrames().size();

				mCurrentFrame --;
				updateFrame();
			}
		}
		else if (_sender == mButtonRight && !mPlay)
		{
			if (mAnimation.getFrames().size() != 0)
			{
				mCurrentFrame ++;
				updateFrame();
			}
		}
	}

	void AnimationViewControl::updateFrame()
	{
		mCurrentFrame %= mAnimation.getFrames().size();
		MyGUI::IntPoint point = mAnimation.getFrames()[mCurrentFrame].first;

		mImage->setImageCoord(MyGUI::IntCoord(point.left, point.top, mAnimation.getSize().width, mAnimation.getSize().height));
		mImage->setImageTile(mAnimation.getSize());
		mImage->setImageIndex(0);
		mFrameInfo->setCaption(MyGUI::utility::toString(mCurrentFrame, " : ", mAnimation.getFrames().size()));
	}

	void AnimationViewControl::updateSelectedFrame()
	{
		if (mParentData == nullptr || mParentData->getChildSelected() == nullptr)
		{
			mImage->setImageCoord(MyGUI::IntCoord());
			mCurrentFrame = 0;
			mFrameInfo->setCaption(MyGUI::utility::toString(mCurrentFrame, " : ", mAnimation.getFrames().size()));
		}
		else
		{
			DataPtr selected = mParentData->getChildSelected();

			mCurrentFrame = mParentData->getChildIndex(selected);
			updateFrame();
		}
	}

}
