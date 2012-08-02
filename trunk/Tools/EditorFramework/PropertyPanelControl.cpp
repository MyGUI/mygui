/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "PropertyPanelControl.h"
#include "FactoryManager.h"

namespace tools
{

	PropertyPanelControl::PropertyPanelControl() :
		mCurrentData(nullptr),
		mDistance(0),
		mScrollView(nullptr),
		mContentHeight(0)
	{
	}

	PropertyPanelControl::~PropertyPanelControl()
	{
		mMainWidget->eventChangeCoord -= MyGUI::newDelegate(this, &PropertyPanelControl::notifyChangeCoord);
	}

	void PropertyPanelControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		mDistance = MyGUI::utility::parseValue<int>(mMainWidget->getUserString("HeightDistance"));
		assignWidget(mScrollView, "ScrollView");

		mMainWidget->eventChangeCoord += MyGUI::newDelegate(this, &PropertyPanelControl::notifyChangeCoord);
	}

	void PropertyPanelControl::HideControls()
	{
		for (VectorPairControl::iterator control = mPropertyControls.begin(); control != mPropertyControls.end(); control ++)
		{
			(*control).second->setProperty(nullptr);
			(*control).second->getRoot()->setVisible(false);
		}

		mScrollView->setCanvasSize(0, 0);
		mContentHeight = 0;
	}

	void PropertyPanelControl::InitialiseProperty(PropertyPtr _property, int& _height)
	{
		std::string type = _property->getType()->getType();
		PropertyControl* control = nullptr;

		for (VectorPairControl::iterator child = mPropertyControls.begin(); child != mPropertyControls.end(); child ++)
		{
			if ((*child).first == type && !(*child).second->getRoot()->getVisible())
			{
				control = (*child).second;
				control->getRoot()->setVisible(true);
				break;
			}
		}

		if (control == nullptr)
		{
			control = components::FactoryManager::GetInstance().CreateItem<PropertyControl>(_property->getType()->getType());
			if (control != nullptr)
			{
				control->Initialise(this, mScrollView, "");

				mPropertyControls.push_back(std::make_pair(_property->getType()->getType(), control));
			}
		}

		if (control != nullptr)
		{
			control->setProperty(_property);
			control->getRoot()->setPosition(0, _height);

			_height += control->getRoot()->getHeight() + mDistance;
		}
	}

	void PropertyPanelControl::setCurrentData(DataPtr _data)
	{
		mCurrentData = _data;

		HideControls();

		if (mCurrentData != nullptr)
		{
			mContentHeight = 0;
			const DataType::VectorProperty& properties = mCurrentData->getType()->getProperties();
			for (DataType::VectorProperty::const_iterator property = properties.begin(); property != properties.end(); property ++)
			{
				if ((*property)->getVisible())
				{
					PropertyPtr pr = mCurrentData->getProperty((*property)->getName());
						InitialiseProperty(pr, mContentHeight);
				}
			}

			updateView();
		}
	}

	void PropertyPanelControl::updateView()
	{
		mScrollView->setCanvasSize((std::numeric_limits<int>::max)() / 2, (std::numeric_limits<int>::max)() / 2);
		mScrollView->setCanvasSize(2, mContentHeight);
		mScrollView->setCanvasSize(mScrollView->getViewCoord().width, mContentHeight);
	}

	void PropertyPanelControl::notifyChangeCoord(MyGUI::Widget* _sender)
	{
		updateView();
	}

}
