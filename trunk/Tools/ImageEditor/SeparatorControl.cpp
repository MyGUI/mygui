/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#include "Precompiled.h"
#include "SeparatorControl.h"
#include "FactoryItemAttribute.h"
#include "FactoryManager.h"

namespace tools
{
	FACTORY_ITEM_ATTRIBUTE(SeparatorControl);

	SeparatorControl::SeparatorControl() :
		mHorizontal(false)
	{
	}

	SeparatorControl::~SeparatorControl()
	{
		mMainWidget->eventChangeCoord -= MyGUI::newDelegate(this, &SeparatorControl::notifyChangeCoord);

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

		mMainWidget->eventChangeCoord += MyGUI::newDelegate(this, &SeparatorControl::notifyChangeCoord);
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
					data->NextSeparator = (*child);
				}

				SeparatorData* data = new SeparatorData();
				data->PreviousPart = lastPart;
				data->PreviousSeparator = lastSeparator;
				(*child)->setUserData(data);

				lastSeparator = (*child);
			}
			else
			{
				lastPart = (*child);
				if (lastSeparator != nullptr)
				{
					SeparatorData* data = *lastSeparator->getUserData<SeparatorData*>();
					data->NextPart = lastPart;
				}
			}
		}
	}

	void SeparatorControl::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id != MyGUI::MouseButton::Left)
			return;

		SeparatorData* data = *_sender->getUserData<SeparatorData*>();
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

	void SeparatorControl::MoveSeparator(MyGUI::Widget* _separator, const MyGUI::IntPoint& _point, SeparatorDirection _modeDirection)
	{
		SeparatorData* data = *_separator->getUserData<SeparatorData*>();

		if (data->PreviousPart == nullptr || data->NextPart == nullptr)
			return;

		MyGUI::IntCoord previousCoord = data->PreviousPart->getCoord();
		MyGUI::IntCoord nextCoord = data->NextPart->getCoord();
		MyGUI::IntCoord separatorCoord = _separator->getCoord();

		previousCoord.width = _point.left - previousCoord.left;
		separatorCoord.left = _point.left;
		nextCoord.width = nextCoord.right() - (_point.left + separatorCoord.width);
		nextCoord.left = _point.left + separatorCoord.width;

		data->PreviousPart->setCoord(previousCoord);
		data->NextPart->setCoord(nextCoord);
		_separator->setCoord(separatorCoord);

		int size = MyGUI::utility::parseValue<int>(data->PreviousPart->getUserString("MinSize"));
		if (size > previousCoord.width && _modeDirection != Next)
		{
			if (data->PreviousSeparator != nullptr)
			{
				MoveSeparator(data->PreviousSeparator, _point - MyGUI::IntPoint(size + data->PreviousSeparator->getWidth(), data->PreviousSeparator->getHeight()), Previous);
			}

			previousCoord = data->PreviousPart->getCoord();
			nextCoord = data->NextPart->getCoord();
			separatorCoord = _separator->getCoord();

			int offset = size - previousCoord.width;

			previousCoord.width += offset;
			separatorCoord.left += offset;
			nextCoord.width -= offset;
			nextCoord.left += offset;

			data->PreviousPart->setCoord(previousCoord);
			data->NextPart->setCoord(nextCoord);
			_separator->setCoord(separatorCoord);
		}

		size = MyGUI::utility::parseValue<int>(data->NextPart->getUserString("MinSize"));
		if (size > nextCoord.width && _modeDirection != Previous)
		{
			if (data->NextSeparator != nullptr)
			{
				MoveSeparator(data->NextSeparator, _point + MyGUI::IntPoint(size + _separator->getWidth(), _separator->getHeight()), Next);
			}

			previousCoord = data->PreviousPart->getCoord();
			nextCoord = data->NextPart->getCoord();
			separatorCoord = _separator->getCoord();

			int offset = size - nextCoord.width;

			previousCoord.width -= offset;
			separatorCoord.left -= offset;
			nextCoord.width += offset;
			nextCoord.left -= offset;

			data->PreviousPart->setCoord(previousCoord);
			data->NextPart->setCoord(nextCoord);
			_separator->setCoord(separatorCoord);
		}
	}

	void SeparatorControl::notifyChangeCoord(MyGUI::Widget* _sender)
	{
		MyGUI::IntSize size = _sender->getSize();
		//if (size.width < mOldSize.width || size.height < mOldSize.height)
		{
			for (VectorWidget::const_iterator child = mChilds.begin(); child != mChilds.end(); child ++)
			{
				SeparatorData** data = (*child)->getUserData<SeparatorData*>(false);
				if (data != nullptr)
				{
					MoveSeparator((*child), (*child)->getPosition());
				}
			}
		}

		mOldSize = size;
	}
}
