/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#include "Precompiled.h"
#include "SeparatorControl.h"
#include "FactoryItemAttribute.h"
#include "FactoryManager.h"
#include "SeparatorUtility.h"

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(SeparatorControl);

	SeparatorControl::SeparatorControl() :
		mHorizontal(false)
	{
	}

	SeparatorControl::~SeparatorControl()
	{
		for (VectorWidget::const_iterator child = mChilds.begin(); child != mChilds.end(); child ++)
		{
			SeparatorData** data = (*child)->getUserData<SeparatorData*>(false);
			if (data != nullptr)
			{
				delete (*data);
				(*child)->setUserData(nullptr);
			}
		}
	}

	void SeparatorControl::Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::Initialise(_parent, _place, _layoutName);

		CreateSeparators();
	}

	void SeparatorControl::CreateSeparators()
	{
		for (size_t index = 0; index < mMainWidget->getChildCount(); index ++)
		{
			MyGUI::Widget* widget = mMainWidget->getChildAt(index);
			mChilds.push_back(widget);

			std::string separator = widget->getUserString("Separator");
			if (separator == "Horizontal")
				mHorizontal = true;
		}

		if (mHorizontal)
			std::sort(mChilds.begin(), mChilds.end(), sortHorizontal);
		else
			std::sort(mChilds.begin(), mChilds.end(), sortVertical);

		MyGUI::Widget* lastPart = nullptr;
		MyGUI::Widget* lastSeparator = nullptr;
		for (VectorWidget::const_iterator child = mChilds.begin(); child != mChilds.end(); child ++)
		{
			std::string separator = (*child)->getUserString("Separator");
			if (separator.empty())
			{
				(*child)->eventMouseButtonPressed += MyGUI::newDelegate(this, &SeparatorControl::notifyMouseButtonPressed);
				(*child)->eventMouseDrag += MyGUI::newDelegate(this, &SeparatorControl::notifyMouseDrag);

				if (lastSeparator != nullptr)
				{
					SeparatorData* data = *lastSeparator->getUserData<SeparatorData*>();
					data->SecondSeparator = (*child);
				}

				SeparatorData* data = new SeparatorData();
				data->FirstPart = lastPart;
				data->FirstSeparator = lastSeparator;
				(*child)->setUserData(data);
			}
			else
			{
				lastPart = (*child);
				if (lastSeparator != nullptr)
				{
					SeparatorData* data = *lastSeparator->getUserData<SeparatorData*>();
					data->SecondPart = lastPart;
				}
			}
		}
	}

	void SeparatorControl::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
		{
			SeparatorData* data = *_sender->getUserData<SeparatorData*>();
			data->PressedPoint = MyGUI::IntPoint(_left, _top);
		}
	}

	void SeparatorControl::notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
	}
}
