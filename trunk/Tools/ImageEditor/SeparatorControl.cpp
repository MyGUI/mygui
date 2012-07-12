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
			if (!separator.empty())
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

				lastSeparator = (*child);
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
		if (_id != MyGUI::MouseButton::Left)
			return;

		SeparatorData* data = *_sender->getUserData<SeparatorData*>();
		//data->PressedPoint = MyGUI::IntPoint(_left, _top);
		data->OffsetPoint = MyGUI::IntPoint(_left, _top) - _sender->getAbsolutePosition();
	}

	void SeparatorControl::notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id != MyGUI::MouseButton::Left)
			return;

		SeparatorData* data = *_sender->getUserData<SeparatorData*>();
		MyGUI::IntPoint parentAbsolutePosition = _sender->getParent()->getAbsolutePosition();
		MyGUI::IntPoint offset = MyGUI::IntPoint(_left, _top) - parentAbsolutePosition - data->OffsetPoint;

		MoveSeparator(_sender, offset);
	}

	void SeparatorControl::MoveSeparator(MyGUI::Widget* _separator, const MyGUI::IntPoint& _point)
	{
		SeparatorData* data = *_separator->getUserData<SeparatorData*>();

		if (data->FirstPart == nullptr || data->SecondPart == nullptr)
			return;

		MyGUI::IntCoord firstCoord = data->FirstPart->getCoord();
		MyGUI::IntCoord secondCoord = data->SecondPart->getCoord();
		MyGUI::IntCoord separatorCoord = _separator->getCoord();

		firstCoord.width = _point.left - firstCoord.left;
		separatorCoord.left = _point.left;
		secondCoord.width = secondCoord.right() - (_point.left + separatorCoord.width);
		secondCoord.left = _point.left + separatorCoord.width;

		data->FirstPart->setCoord(firstCoord);
		data->SecondPart->setCoord(secondCoord);
		_separator->setCoord(separatorCoord);

		int size = MyGUI::utility::parseValue<int>(data->FirstPart->getUserString("MinSize"));
		if (size > firstCoord.width)
		{
			if (data->FirstSeparator != nullptr)
			{
				MoveSeparator(data->FirstSeparator, _point - MyGUI::IntPoint(size + data->FirstSeparator->getWidth(), data->FirstSeparator->getHeight()));
			}

			firstCoord = data->FirstPart->getCoord();
			secondCoord = data->SecondPart->getCoord();
			separatorCoord = _separator->getCoord();

			int offset = size - firstCoord.width;

			firstCoord.width += offset;
			separatorCoord.left += offset;
			secondCoord.width -= offset;
			secondCoord.left += offset;

			data->FirstPart->setCoord(firstCoord);
			data->SecondPart->setCoord(secondCoord);
			_separator->setCoord(separatorCoord);
		}

		/*size = MyGUI::utility::parseValue<int>(data->SecondPart->getUserString("MinSize"));
		if (size > secondCoord.width)
		{
			if (data->SecondSeparator != nullptr)
			{
				MoveSeparator(data->SecondSeparator, _point + MyGUI::IntPoint(size + _separator->getWidth(), _separator->getHeight()));
			}

			firstCoord = data->FirstPart->getCoord();
			secondCoord = data->SecondPart->getCoord();
			separatorCoord = _separator->getCoord();

			int offset = size - secondCoord.width;

			firstCoord.width -= offset;
			separatorCoord.left -= offset;
			secondCoord.width += offset;
			secondCoord.left -= offset;

			data->FirstPart->setCoord(firstCoord);
			data->SecondPart->setCoord(secondCoord);
			_separator->setCoord(separatorCoord);
		}*/
	}
}
