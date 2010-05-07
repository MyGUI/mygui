/*!
	@file
	@author		Alexander Ptakhin
	@date		02/2009
	@module
*/
#include "MyGUI_SizeDescription.h"
#include "MyGUI_Window.h"

namespace MyGUI
{
	bool SizeParam::fromString(const std::string& str)
	{
		if (str.length() < 2)
			return false;

		std::string temp;

		if (str[ 0 ] == 'p')
		{
			temp = str.substr(1, str.length() - 1);
			
			px(utility::parseInt(temp));
		}
		else if (str[ 0 ] == 'f')
		{			
			if (str[ 1 ] == 'f')
				mFlMode = FM_FREE_SPACE;
			else if (str[ 1 ] == 'p')
				mFlMode = FM_PARENT;
			else
				MYGUI_EXCEPT("Second character of float mode in SizeParam is invalid!");

			temp = str.substr(2, str.length() - 2);

			mFl = utility::parseFloat(temp);
		}
		else MYGUI_EXCEPT("First character in SizeParam is invalid!");

		return true;
	}

	void SizeParam::toString(std::string& output) const
	{
		output.clear();

		if (isPx())
		{
			output += "p";
			output += utility::toString(mPx);
		}
		else // float
		{
			if (isParentFl())
				output += "p";
			else
				output += "f";

			output += "f";
			output += utility::toString(mFl); // TODO?: make less characters for float
		}
	}

	bool Dimension::fromString(const std::string& str)
	{
		size_t semicol = str.find_first_of(';');

		if (semicol == str.npos)
			return false;

		std::string temp = str.substr(0, semicol);
		w.fromString(temp);

		temp = str.substr(semicol + 1, str.length() - semicol - 1);
		h.fromString(temp);

		return true;
	}

	void Dimension::toString(std::string& output) const
	{
		std::string tmp;
		w.toString(tmp);
		output += tmp + ";";
		h.toString(tmp);
		output += tmp;
	}

	//SizeDescription::SizeDescription(Widget* _widget)
	//:	mSizeBehaviour(0), 
	//	//mMaxSize(INT_SIZE_UNBOUND),
	//	//mIsInitialized(false),
	//	mWidget(_widget)
	//{
	//}

	//SizeDescription::SizeDescription(Widget* _widget, const Dimension& _dim)
	//:	mSizeBehaviour(0), 
	//	size(_dim), 
	//	//mMinSize(0, 0), 
	//	//mMaxSize(INT_SIZE_UNBOUND),
	//	//mIsInitialized(false),
	//	mWidget(_widget)
	//{
	//}

	/*bool SizeDescription::checkBehaviour(uint8 _beh) const
	{
		if ((_beh & SB_MIN) && (_beh & SB_MAX))
		{
			MYGUI_EXCEPT("Can't be min and max!"); 
			return false;
		}

		return true;
	}*/

	/*void SizeDescription::setSize(const Dimension& _dim)
	{
		size = _dim;
	}*/

	//void SizeDescription::setSizeBehaviour(uint8 _beh)
	//{
	//	MYGUI_ASSERT(checkBehaviour(_beh), "Invalid size benaviour!");

	//	mSizeBehaviour = _beh;
	//	//mIsInitialized = true;
	//}


	/*IntSize SizeDescription::getWidgetMinSize() const
	{
		IntSize result;

		const std::string& type =  mWidget->getTypeName();

		if (type == "Window")
		{
			Window* window = mWidget->castType< Window >(false);
			return window->getMinSize();
		}
		else
			return IntSize(0, 0);
	}*/

	//IntSize SizeDescription::getMinSize() const
	//{
	//	return getWidgetMinSize();
	//	//return mMinSize;
	//}

	//void SizeDescription::setMinSize(const IntSize& _pxSize)
	//{
	//	mMinSize = _pxSize;
	//	mIsInitialized = true;
	//}

	//void SizeDescription::setMaxSize(const IntSize& _pxSize)
	//{
	//	mMaxSize = _pxSize;
	//	mIsInitialized = true;
	//}

	//bool SizeDescription::isPxSize() const
	//{
	//	return mPxSize != IntSize(0, 0) && mFlSize == FloatSize(0, 0); 
	//}

	//bool SizeDescription::isFlSize() const 
	//{ 
	//	return mFlSize != FloatSize(0, 0) && mPxSize == IntSize(0, 0); 
	//}
	
} // namespace MyGUI

